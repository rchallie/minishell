/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/03/10 18:57:17 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

int		has_redir_output(t_minishell *ms, int redir_type, int cursor, int fd)
{
	int			o;
	int			s;
	extern int	errno;

	s = S_IRWXG | S_IRWXU | S_IRWXO;
	if (!ms->sequence[cursor] || ms->sequence[cursor] == 6
		|| ms->sequence[cursor] == 7 || ms->sequence[cursor] == 9)
		return (fd);
	else if (ms->sequence[cursor] == 2)
		return (has_redir_output(ms, redir_type, cursor + 1, fd));
	else if (ms->sequence[cursor] == 4 || ms->sequence[cursor] == 3)
		return (has_redir_output(ms, ms->sequence[cursor], cursor + 1, fd));
	else if (ms->sequence[cursor] == 8 && (redir_type == 4 || redir_type == 3))
	{
		(fd >= 3) ? close(fd) : 0;
		o = (redir_type == 3) ? O_CREAT | O_RDWR | O_TRUNC : O_CREAT | O_RDWR;
		if (!(fd = open(ms->treated[cursor], o, s)))
			return (-1); //Error path à gérer
		redir_type = 0;
	}
	return (has_redir_output(ms, redir_type, cursor + 1, fd));
}

int		has_redir_input(t_minishell *ms, int redir_type, int cursor, int fd)
{
	if (!ms->sequence[cursor] || ms->sequence[cursor] == 6
		|| ms->sequence[cursor] == 7 || ms->sequence[cursor] == 9)
		return (fd);
	else if (ms->sequence[cursor] == 2)
		return (has_redir_input(ms, redir_type, cursor + 1, fd));
	else if (ms->sequence[cursor] == 5)
		return (has_redir_input(ms, ms->sequence[cursor], cursor + 1, fd));
	else if (ms->sequence[cursor] == 8 && redir_type == 5)
	{
		(fd >= 3) ? close(fd) : 0;
		if (!(fd = open(ms->treated[cursor], O_RDONLY)))
			return (-1); //Error path à gérer
		redir_type = 0;
	}
	return (has_redir_input(ms, redir_type, cursor + 1, fd));
}

// ctrl-\" a gérer?
int		main(int ac, char **av, char **envp)
{
	// DEAL WITH ./ms < file
	t_minishell		ms;
	int				ret;
	char			*pwd;
	int				cmd_ret;

	cmd_ret = 0;
	(void)ac;
	(void)av;
	ret = 1;
	put_beg();
	int (*cmd[5])(t_minishell *) = {&cd,&print_work_dir,&exit_minishell, &env, &echo_};
	while (ret == SUCCESS)
	{
		// init_minishell_var(&ms, envp);
		ms = (t_minishell){.envp = envp, .iscmdret = -1, .isexecret = -1};
		if (!get_pwd_short(&pwd))
			return (ERROR);
		ft_printf("\e[97m[\e[91mm\e[92mi\e[93mn\e[94mi\e[95ms\e[96mh\e[91me");
		ft_printf("\e[92ml\e[93ml\e[97m] \e[91m%s \e[97m: ", pwd);
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
			int saved_stdout;
			int saved_stdin;
			int	fdinput;
			int fdoutput;

			saved_stdout = dup(STDOUT_FILENO);
			saved_stdin = dup(STDIN_FILENO);
			fdoutput = has_redir_output(&ms, 0, ms.seq_cursor + 1, STDOUT_FILENO);
			fdinput = has_redir_input(&ms, 0, ms.seq_cursor + 1, STDIN_FILENO);
			dup2(fdoutput, STDOUT_FILENO);
			dup2(fdinput, STDIN_FILENO);
			while (ms.seq_cursor < ms.treated_len && ms.treated[ms.seq_cursor])
			{
				if (ms.sequence[ms.seq_cursor] == 0 && (ms.isexecret = is_exec(&ms)) == ERROR)
				{
					if ((ms.iscmdret = is_cmd(ms.treated[ms.seq_cursor])) != -1
						&& (ms.iscmdret >= 0 && ms.iscmdret <= 4))
							cmd_ret = cmd[ms.iscmdret](&ms);
					else if (ms.sequence[ms.seq_cursor] == 0
						&& ms.iscmdret == -1 && ms.treated[ms.seq_cursor][0])
						error_command(ms.treated[ms.seq_cursor]);
				}
				ms.seq_cursor++;
			}
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdout);
			dup2(saved_stdin, STDIN_FILENO);
			close(saved_stdin);
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
					// int saved_stdout = dup(STDOUT_FILENO);
					// int fdoutput = has_redir_output(&ms, 0, ms.seq_cursor + 1, STDOUT_FILENO);
					// printf ("%-20s : %10.2d\n", "fdoutput", fdoutput);
					// dup2(fdoutput, STDOUT_FILENO);
					int saved_stdout = dup(STDOUT_FILENO);
					int saved_stdin = dup(STDIN_FILENO);
					int	fdinput = has_redir_input(&ms, 0, ms.seq_cursor + 1, STDIN_FILENO);
					int fdoutput = has_redir_output(&ms, 0, ms.seq_cursor + 1, STDOUT_FILENO);
					printf ("%-20s : %10.2d\n", "fdoutput", fdoutput);
					printf ("%-20s : %10.2d\n", "fdinput", fdinput);
					dup2(fdoutput, STDOUT_FILENO);
					dup2(fdinput, STDIN_FILENO);
					if (nb_cmd_p == 0)
					{
						// SORTIE
						dup2(tab_fpipe[nb_cmd_p][1], fdoutput);
						close(tab_fpipe[nb_cmd_p][0]);
						close(tab_fpipe[nb_cmd_p][1]);
					}
					else if (nb_cmd_p < has_pipe - 1)
					{
						dup2(tab_fpipe[nb_cmd_p - 1][0], fdinput);
						dup2(tab_fpipe[nb_cmd_p][1], fdoutput);
					}
					else if (nb_cmd_p == has_pipe - 1)
					{
						// ENTREE
						dup2(tab_fpipe[nb_cmd_p - 1][0], fdinput);
						printf ("%-20s : %10.2d\n", "fd in", tab_fpipe[nb_cmd_p - 1][0]);
						// close(tab_fpipe[nb_cmd_p][1]);
						// close(tab_fpipe[nb_cmd_p][0]);
						// close(tab_fpipe[nb_cmd_p - 1][1]);
						// close(tab_fpipe[nb_cmd_p - 1][0]);
						// close(tab_fpipe[nb_cmd_p][0]);
						// close(tab_fpipe[nb_cmd_p - 1][1]);
					}
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
						// dup2(saved_stdout, 1);
						// close(saved_stdout);
					dup2(saved_stdout, STDOUT_FILENO);
					close(saved_stdout);
					dup2(saved_stdin, STDIN_FILENO);
					close(saved_stdin);
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
