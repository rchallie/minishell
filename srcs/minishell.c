/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/03/04 18:57:13 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// PAS DE CONTROL D IF PROMPT (len != 0)

#include "../incs/minishell.h"

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
	ft_printf("(pour ne pas être confondu avec ceux demandé dans le sujet)\n");
	ft_printf("\n\e[94mLes commandes éxecutables sont :\n");
	ft_printf("	- echo\n");
	ft_printf("	- cd\n");
	ft_printf("	- pwd\n");
	ft_printf("	- export\n");
	ft_printf("	- unset\n");
	ft_printf("	- env\n");
	ft_printf("	- exit\n\n");

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
		// int i = 0;
		// while (ms.treated[i])
		// {
		// 	printf("[%d] : %s\n", i, ms.treated[i]);
		// 	i++;
		// }
		int has_pipe = 0;
		int cursor = 0;
		while (ms.treated[cursor])
			has_pipe += (ms.sequence[cursor++] == 6) ? 1 : 0; 
		// ft_printf("-------- CHILD ---------\n");
		// ft_printf("Haspipe : %d\n", has_pipe);
		has_pipe += (has_pipe) ? 1 : 0; 
		// ft_printf("Nombre child : %d\n", has_pipe);
		if (has_pipe == 0)
		{
			printf("Seq_cur : %d < Treated_len : %d && Treated : %s\n", ms.seq_cursor, ms.treated_len, ms.treated[ms.seq_cursor]);
			while (ms.seq_cursor < ms.treated_len && ms.treated[ms.seq_cursor])
			{
				int (*cmd[5])(t_minishell *) = {&cd,&print_work_dir,&exit_minishell, &env, &echo_};
				// objectif c'est que les forks se fasse ici
				printf("Cursor : %d\n", ms.seq_cursor);
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
							int o = ms.seq_cursor + 1;
							// while o < ms->treated len
							while (ms.sequence[o]
								&& !(ms.sequence[o] >= 3
								&& ms.sequence[o] <= 6))
								o++;
							if (ms.sequence[o] >= 3 && ms.sequence[o] <= 6)
								ms.has_spec_uf = 1;
							// -------------------
							if((cmd_ret = cmd[ms.iscmdret](&ms)) == TREAT)
								treat_output(&ms);
							else
								printf("CMD RET : %d\n", cmd_ret);
						}
					}
					else if (ms.sequence[ms.seq_cursor] == 0 && ms.iscmdret == -1 && ms.treated[ms.seq_cursor][0])
						error_command(ms.treated[ms.seq_cursor]);
				}
				// while (ms.sequence[ms.seq_cursor])
				ms.seq_cursor++;
			}
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
					if (nb_cmd_p == 0)
					{
						// SORTIE
						dup2(tab_fpipe[nb_cmd_p][1], STDOUT_FILENO);
						close(tab_fpipe[nb_cmd_p][0]);
						close(tab_fpipe[nb_cmd_p][1]);
					}
					else if (nb_cmd_p < has_pipe - 1)
					{
						// ENTREE
						dup2(tab_fpipe[nb_cmd_p - 1][0], STDIN_FILENO);
						// SORTIE 
						dup2(tab_fpipe[nb_cmd_p][1], STDOUT_FILENO);
						// READ
						// read(tab_fpipe[nb_cmd_p - 1][0], buffer, 3999);
					}
					else if (nb_cmd_p == has_pipe - 1)
					{
						// ENTREE
						dup2(tab_fpipe[nb_cmd_p - 1][0], STDIN_FILENO);
						close(tab_fpipe[nb_cmd_p][1]);
						close(tab_fpipe[nb_cmd_p][0]);
						close(tab_fpipe[nb_cmd_p - 1][1]);
						close(tab_fpipe[nb_cmd_p - 1][0]);
						close(tab_fpipe[nb_cmd_p - 1][0]);
						close(tab_fpipe[nb_cmd_p][0]);
						close(tab_fpipe[nb_cmd_p - 1][1]);
						// int ret = 0;
						// ret = read(tab_fpipe[nb_cmd_p - 1][0], buffer2, 3999);
						// printf("BUFFER : %s\nRET : %d\n", buffer2, ret);
						// ms.output = "test\n";
					}
// 					nb_cmd_p != 0 ? read(fpipe[0], buffer, 3999) : 0;
// // 					//close(fpipe[1]);      //close write pipe
// // 					bbbbbb = ft_atoi(buffer);
// // 					bbbbbb++;
// // 					printf("Child : %d\n", bbbbbb);
// // 					write(fpipe[1], ft_itoa(bbbbbb), 5);
// // //					perror("First program execution failed");
// // 					exit(1);
			
					int (*cmd[5])(t_minishell *) = {&cd,&print_work_dir,&exit_minishell,&env, &echo_};
// 					// objectif c'est que les forks se fasse ici
// 					printf("Cursor : %d\n", ms.seq_cursor);
					if (ms.sequence[ms.seq_cursor] == 0 && (ms.isexecret = is_exec(&ms)) == ERROR)
					{
						if ((ms.iscmdret = is_cmd(ms.treated[ms.seq_cursor])) != -1)
						{
							//write(1, "A\n", 2);
							if (ms.iscmdret >= 0 && ms.iscmdret <= 4)
							{
								// has_pipe
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
								// write(1, ms.output, ft_strlen(ms.output));
								if((cmd_ret = cmd[ms.iscmdret](&ms)) == TREAT)
									treat_output(&ms);
								// else
								// 	printf("CMD RET : %d\n", cmd_ret);
							}
						}
						else if (ms.sequence[ms.seq_cursor] == 0 && ms.iscmdret == -1 && ms.treated[ms.seq_cursor][0])
							error_command(ms.treated[ms.seq_cursor]);
					// ft_printf("nb = |%d|\n", nb_cmd_p);
					}
					// if (nb_cmd_p == 0)
					// 	close(tab_fpipe[nb_cmd_p][1]);
					// else if (nb_cmd_p < has_pipe - 1)
					// {
					// 	close(tab_fpipe[nb_cmd_p - 1][0]);
					// 	close(tab_fpipe[nb_cmd_p][1]);
					// 	close(tab_fpipe[nb_cmd_p - 1][1]);
					// }
					// else if (nb_cmd_p == has_pipe - 1)
					// {
					// 	close(tab_fpipe[nb_cmd_p - 1][0]);
					// 	close(tab_fpipe[nb_cmd_p][0]);
					// 	close(tab_fpipe[nb_cmd_p - 1][1]);
					// }
					exit(0);
				}
				else
				{
					// if (nb_cmd_p == 0)
					// {
					// 	// close(tab_fpipe[nb_cmd_p][0]);
					// 	close(tab_fpipe[nb_cmd_p][1]);
					// }
					// else if (nb_cmd_p < has_pipe - 1)
					// {
					// 	close(tab_fpipe[nb_cmd_p - 1][0]);
					// 	close(tab_fpipe[nb_cmd_p][0]);
					// 	close(tab_fpipe[nb_cmd_p][1]);
					// 	close(tab_fpipe[nb_cmd_p - 1][1]);
					// }
					// else if (nb_cmd_p == has_pipe - 1)
					// {
					// 	close(tab_fpipe[nb_cmd_p - 1][0]);
					// 	close(tab_fpipe[nb_cmd_p][0]);
					// 	close(tab_fpipe[nb_cmd_p][1]);
					// 	close(tab_fpipe[nb_cmd_p - 1][1]);
					// }
					close(tab_fpipe[nb_cmd_p][1]);
					waitpid(fork_, &status, 0);
				}
				// if(fork() == 0)   // child 2
				// {
				// 	dup2(fpipe[0], STDIN_FILENO);   // Redirect STDIN to Input part of pipe         
				// 	close(fpipe[1]);       //closing pipe write
				// 	close(fpipe[0]);       //close read pipe 

				// 	execvp(args[k+1], args+k+1);    // pass the second part of command line as argument
				// 	perror("Second program execution failed");
				// 	exit(1);
				// }
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
