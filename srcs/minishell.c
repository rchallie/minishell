/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/03/10 13:51:33 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// PAS DE CONTROL D IF PROMPT (len != 0)

#include "../incs/minishell.h"

int has_redir(t_minishell *ms, int redir_type, int cursor, int fd)
{
    int O = 0;
    int S = 0;
    if (!ms->sequence[cursor] || ms->sequence[cursor] == 6
        || ms->sequence[cursor] == 7 || ms->sequence[cursor] == 9)
        return (fd);
    else if (ms->sequence[cursor] == 2)
        return (has_redir(ms, redir_type, ++cursor, fd));
    else if (ms->sequence[cursor] == 3)
        return (has_redir(ms, 3, ++cursor, fd));
    else if (ms->sequence[cursor] == 8)
    {
		(fd >= 3) ? close(fd) : 0;
        if (redir_type == 0)
            exit (123456789);
        if (redir_type == 3)
        {
            O = O_CREAT | O_RDWR | O_TRUNC;
            S = S_IRWXG | S_IRWXU | S_IRWXO;
        }
        else if (redir_type == 4)
        {
            O = O_CREAT | O_RDWR;
            S = S_IRWXG | S_IRWXU | S_IRWXO;
        }
        if (!(fd = open(ms->treated[cursor], O, S)))
        {
            printf("Error overwrite : FD : %d\n", fd)	;
            return (-1);
        }
    }
    return (has_redir(ms, redir_type, ++cursor, fd));
}

static void	init_minishell_var(t_minishell *ms, char **envp)
{
	ms->entry = NULL;
	ms->treated = NULL;
	ms->envp = envp;
	ms->sequence = NULL;
	ms->seq_cursor = 0;
	ms->iscmdret = -1;
	ms->isexecret = -1;
	ms->has_spec_uf = 0;
}

void	print_middle_term(char *str, int str_len, int row)
{
	t_line line;
	
	raw_term_mode();
    ft_bzero(&line, sizeof(line));
    get_cursor_start_pos(&line);
	ft_getwinsz(&line.winsz);
	line.start.row = row;
	line.start.col = line.winsz.col / 2 - str_len / 2;
	set_curpos(&line);
	ft_printf(str);
	line.start.row = row + 1;
	line.start.col = 0;
	set_curpos(&line);
	default_term_mode();
}
void	clear_term()
{
	t_line line;
	
    raw_term_mode();
    ft_bzero(&line, sizeof(line));
    get_cursor_start_pos(&line);
	ft_getwinsz(&line.winsz);
	tputs(tgoto(tgetstr("SF", NULL), 0, line.winsz.row - 1), 1, &tc_putchar);
	line.start.row = 0;
	line.start.col = 0;
	set_curpos(&line);
	default_term_mode();
}

// \" a gérer?
int main(int ac, char **av, char **envp)
{
	t_minishell		ms;
	int				ret;
    char            *pwd;
	(void)ac;
	(void)av;
	(void)envp;
	ret = 1;

	clear_term();
	print_middle_term("\e[91m┌┬┐\e[92m┬┌┐\e[93m┌┬┌\e[94m─┐┬\e[95m ┬┌\e[96m─┐\e[91m┬  \e[92m┬  ", 24,1);
	print_middle_term("\e[91m││\e[92m│││\e[93m│││\e[94m└─┐\e[95m├─┤\e[96m├┤ \e[91m│  \e[92m│  ", 24,2);
	print_middle_term("\e[91m┴ \e[92m┴\e[93m┴┘└┘\e[94m┴└─\e[95m┘┴ \e[96m┴└─\e[91m┘┴─\e[92m┘┴─┘", 24,3);
	print_middle_term("", 0, 4);

	// print_middle_term("\n",1, 6);

	default_term_mode();
	
	while (ret == SUCCESS)
	{
        init_minishell_var(&ms, envp);
        if (!get_pwd_short(&pwd))
			return (ERROR);
		ft_printf("\e[97m[\e[91mm\e[92mi\e[93mn\e[94mi\e[95ms\e[96mh\e[91me\e[92ml\e[93ml\e[97m] \e[91m%s \e[97m> ", pwd);
		free(pwd);
		ret = line_edition(&ms);
		if (!sanitize(&ms, ms.entry, &ms.treated))
			return (0); 
		ms.treated_len = get_double_char_tab_len(ms.treated);
		get_sequence(ms.treated, &ms.sequence);

		/* REORDER DEVRA CHECKER LA VALIDITER DES FILES DE REDIR */
		reorder_sequence(&ms);
		int has_pipe = 0;
		int cursor = 0;
		while (ms.treated[cursor])
			has_pipe += (ms.sequence[cursor++] == 6) ? 1 : 0; 
		has_pipe += (has_pipe) ? 1 : 0; 
		if (has_pipe == 0)
		{
			int saved_stdout = dup(STDOUT_FILENO);
			int fdoutput = has_redir(&ms, 0, ms.seq_cursor + 1, STDOUT_FILENO);
			// printf ("%-20s : %10.2d\n", "fdoutput", fdoutput);
			dup2(fdoutput, STDOUT_FILENO);
			// printf("Seq_cur : %d < Treated_len : %d && Treated : %s\n", ms.seq_cursor, ms.treated_len, ms.treated[ms.seq_cursor]);
			while (ms.seq_cursor < ms.treated_len && ms.treated[ms.seq_cursor])
			{
				int (*cmd[5])(t_minishell *) = {&cd,&print_work_dir,&exit_minishell, &env, &echo_};
				// objectif c'est que les forks se fasse ici
				// printf("Cursor : %d\n", ms.seq_cursor);
				if (ms.sequence[ms.seq_cursor] == 0 && (ms.isexecret = is_exec(&ms)) == ERROR)
				{
				// objectif c'est que les forks se fasse ici
					if ((ms.iscmdret = is_cmd(ms.treated[ms.seq_cursor])) != -1)
					{
						//write(1, "A\n", 2);
						if (ms.iscmdret >= 0 && ms.iscmdret <= 4)
						{
							// has_pipe
							// A REPRENDRE
							int cmd_ret = 0;
							// int o = ms.seq_cursor;
							// // while o < ms->treated len
							// while (ms.sequence[o]
							// 	&& !(ms.sequence[o] >= 3
							// 	&& ms.sequence[o] <= 6))
							// 	o++;
							// if (ms.sequence[o] >= 3 && ms.sequence[o] <= 6)
							// // 	ms.has_spec_uf = 1;
							
							cmd_ret = cmd[ms.iscmdret](&ms);
							
							// dup2(STDOUT_FILENO, fdoutput);
							// -------------------
							// if((cmd_ret = cmd[ms.iscmdret](&ms)) == TREAT)
							// treat_output(&ms);
							// else
							// 	printf("CMD RET : %d\n", cmd_ret);
						}
					}
					else if (ms.sequence[ms.seq_cursor] == 0 && ms.iscmdret == -1 && ms.treated[ms.seq_cursor][0])
						error_command(ms.treated[ms.seq_cursor]);
				}
				// while (ms.sequence[ms.seq_cursor])
				ms.seq_cursor++;
			}
			dup2(saved_stdout, 1);
			close(saved_stdout);	
		}
		else
		{
			// int		bbbbbb = 0;
			int		nb_cmd_p;
			// char	*b = NULL;
			char	buffer[4000];
			ft_bzero(buffer, sizeof(char) * 3999);
			char	buffer2[4000];
			ft_bzero(buffer2, sizeof(char) * 3999);
			nb_cmd_p = 0;
			int		fork_;
			int		status;
			// FAIRE UNE STRUCT AVEC PID, STATUS ET MON CUL
			int		tab_fpipe[has_pipe][2];
			int		fpipe[2];
			// int ret = 0;
			while (nb_cmd_p < has_pipe)
			{
				
				if (pipe(tab_fpipe[nb_cmd_p]) == -1)
					break ;
				if ((fork_ = fork()) < 0)
					break ;
				if(fork_ == 0)   // child 1        
				{
					int saved_stdout = dup(STDOUT_FILENO);
					int fdoutput = has_redir(&ms, 0, ms.seq_cursor + 1, STDOUT_FILENO);
					printf ("%-20s : %10.2d\n", "fdoutput", fdoutput);
					dup2(fdoutput, STDOUT_FILENO);
					if (nb_cmd_p == 0)
					{
						// SORTIE
						dup2(tab_fpipe[nb_cmd_p][1], fdoutput);
						close(tab_fpipe[nb_cmd_p][0]);
						close(tab_fpipe[nb_cmd_p][1]);
					}
					else if (nb_cmd_p < has_pipe - 1)
					{
						dup2(tab_fpipe[nb_cmd_p - 1][0], STDIN_FILENO);
						dup2(tab_fpipe[nb_cmd_p][1], fdoutput);
					}
					else if (nb_cmd_p == has_pipe - 1)
					{
						// ENTREE
						dup2(tab_fpipe[nb_cmd_p - 1][0], STDIN_FILENO);
						printf ("%-20s : %10.2d\n", "fd in", tab_fpipe[nb_cmd_p - 1][0]);
						// close(tab_fpipe[nb_cmd_p][1]);
						// close(tab_fpipe[nb_cmd_p][0]);
						// close(tab_fpipe[nb_cmd_p - 1][1]);
						// close(tab_fpipe[nb_cmd_p - 1][0]);
						// close(tab_fpipe[nb_cmd_p][0]);
						// close(tab_fpipe[nb_cmd_p - 1][1]);
					}
					
					int (*cmd[5])(t_minishell *) = {&cd,&print_work_dir,&exit_minishell,&env, &echo_};
					if (ms.sequence[ms.seq_cursor] == 0 && (ms.isexecret = is_exec(&ms)) == ERROR)
					{
						if ((ms.iscmdret = is_cmd(ms.treated[ms.seq_cursor])) != -1)
						{
							if (ms.iscmdret >= 0 && ms.iscmdret <= 4)
							{
								// A REPRENDRE
								int cmd_ret = 0;
								int o = ms.seq_cursor + 1;
								// while o < ms->treated len
								while (ms.sequence[o]
									&& !(ms.sequence[o] >= 3
									&& ms.sequence[o] <= 6))
									o++;
								if (ms.sequence[o] >= 3 && ms.sequence[o] <= 5)
									ms.has_spec_uf = 1;
								// -------------------
								// cmd_ret = cmd[ms.iscmdret](&ms);
								if((cmd_ret = cmd[ms.iscmdret](&ms)) == TREAT)
									treat_output(&ms);
							}
						}
						else if (ms.sequence[ms.seq_cursor] == 0 && ms.iscmdret == -1 && ms.treated[ms.seq_cursor][0])
							error_command(ms.treated[ms.seq_cursor]);
					}
						dup2(saved_stdout, 1);
						close(saved_stdout);
					exit(0);
				}
				else
				{
					close(tab_fpipe[nb_cmd_p][1]);
					waitpid(fork_, &status, 0);
				}
				ms.seq_cursor++;
				while (ms.sequence[ms.seq_cursor] && ms.seq_cursor < ms.treated_len)
					ms.seq_cursor++;
				nb_cmd_p++;
			}
			close(fpipe[1]);
			close(fpipe[0]);
			
		}
		free_double_char_tab(ms.treated);
		free(ms.sequence);
	}
	exit(0);
	return (0);
}
