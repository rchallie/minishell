/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/03/11 19:01:59 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

static void treat_command(t_minishell *ms)
{
	int			cmd_ret;
	static int	(*cmd[5])(t_minishell *) = {
		&cd,&print_work_dir,&exit_minishell,&env,&echo_};

	cmd_ret = 0;
	if (ms->sequence[ms->seq_cursor] == 0
		&& (ms->isexecret = is_exec(ms)) == ERROR)
	{
		if ((ms->iscmdret = is_cmd(ms->treated[ms->seq_cursor])) != -1
			&& (ms->iscmdret >= 0 && ms->iscmdret <= 4))
				cmd_ret = cmd[ms->iscmdret](ms);
		else if (ms->sequence[ms->seq_cursor] == 0
			&& ms->iscmdret == -1 && ms->treated[ms->seq_cursor][0])
			error_command(ms->treated[ms->seq_cursor]);
	}
}

static void		cmd_has_pipe(t_minishell *ms, int has_pipe)
{
	// int		bbbbbb = 0;
			int		nb_cmd_p;
			// char	*b = NULL;
			char	buffer[4000];
			ft_bzero(buffer, sizeof(char) * 3999);
			char	buffer2[4000];
			ft_bzero(buffer2, sizeof(char) * 3999);
			nb_cmd_p = 0;
			int		fork_ = 0;
			int		status = 0;
			// FAIRE UNE STRUCT AVEC PID, STATUS ET MON CUL
			int		tab_fpipe[has_pipe][2];
			int		fpipe[2];

			int gen_fork;
			int gen_status;
			if ((gen_fork = fork()) < 0)
				exit(1258);
			if (gen_fork == 0)
			{
				while (nb_cmd_p < has_pipe)
				{
					default_term_mode();
					if (pipe(tab_fpipe[nb_cmd_p]) == -1)
						break ;
					if ((fork_ = fork()) < 0)
						break ;
					if(fork_ == 0)   
					{
						int in_out[4];
						in_out[0] = dup(STDOUT_FILENO);
						in_out[1] = dup(STDIN_FILENO);
						in_out[2] = has_redir_input(ms, 0, ms->seq_cursor + 1, STDIN_FILENO);
						in_out[3] = has_redir_input(ms, 0, ms->seq_cursor + 1, STDOUT_FILENO);
						if (in_out[3] != STDOUT_FILENO)
							dup2(in_out[3], STDOUT_FILENO);
						dup2(in_out[2], STDIN_FILENO);
						if (nb_cmd_p == 0)
						{
							if (in_out[3] != STDOUT_FILENO)
								dup2(tab_fpipe[nb_cmd_p][1], in_out[3]);
							else
								dup2(tab_fpipe[nb_cmd_p][1], STDOUT_FILENO);
							close(tab_fpipe[nb_cmd_p][0]);
							close(tab_fpipe[nb_cmd_p][1]);
						}
						else if (nb_cmd_p < has_pipe - 1)
						{
							dup2(tab_fpipe[nb_cmd_p - 1][0], in_out[2]);
							if (in_out[3] != STDOUT_FILENO)
								dup2(tab_fpipe[nb_cmd_p][1], in_out[3]);
							else
								dup2(tab_fpipe[nb_cmd_p][1], STDOUT_FILENO);
						}
						else if (nb_cmd_p == has_pipe - 1)
							dup2(tab_fpipe[nb_cmd_p - 1][0], in_out[2]);
						treat_command(ms);
						dup2(in_out[0], STDOUT_FILENO);
						close(in_out[0]);
						dup2(in_out[1], STDIN_FILENO);
						close(in_out[1]);
						exit(0);
					}
					else
						close(tab_fpipe[nb_cmd_p][1]);
					ms->seq_cursor++;
					while (ms->sequence[ms->seq_cursor] && ms->seq_cursor < ms->treated_len)
						ms->seq_cursor++;
					nb_cmd_p++;
				}
				close(fpipe[1]);
				close(fpipe[0]);
				waitpid(fork_, &status, 0);
				exit(0);
			}
			else
			{
				waitpid(gen_fork, &gen_status, 0);
			}	
}

static void		cmd_no_pipe(t_minishell *ms)
{
	int saved_stdout;
	int saved_stdin;
	int	fdinput;
	int fdoutput;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	fdoutput = has_redir_output(ms, 0, ms->seq_cursor + 1, STDOUT_FILENO);
	fdinput = has_redir_input(ms, 0, ms->seq_cursor + 1, STDIN_FILENO);
	dup2(fdinput, STDIN_FILENO);
	dup2(fdoutput, STDOUT_FILENO);
	while (ms->seq_cursor < ms->treated_len && ms->treated[ms->seq_cursor])
	{
		treat_command(ms);
		ms->seq_cursor++;
	}
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
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
	while (ret == SUCCESS)
	{
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
			cmd_no_pipe(&ms);
		}
		else
		{
			cmd_has_pipe(&ms, has_pipe);
		}
		free_double_char_tab(ms.treated);
		free(ms.sequence);
	}
	return (0);
}

//Famine version 1.0 (c)oded oct-2015 by wandre