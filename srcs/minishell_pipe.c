/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 21:46:16 by thervieu          #+#    #+#             */
/*   Updated: 2020/05/28 14:38:57 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

static void	if_in_out(int nb_cmd_p,
	int *pipe_, int *in_out, char **cmd, int *seq)
{
	(void)in_out;

	//ft_printf(1, "g_ms has_pipe = %d\n", g_ms.has_pipe);
	//ft_printf(1, "ND CMD P = %d\n", nb_cmd_p);
	int new_put = -1;
	int new_put2 = -1;
	if (nb_cmd_p != 0)
	{
	//	ft_printf(1, "{%d} FD PIPE IN = %d\n", nb_cmd_p, pipe_[(nb_cmd_p - 1) * 2]);
		new_put = dup2(pipe_[(nb_cmd_p - 1) * 2], STDIN_FILENO);
	//	ft_printf(1, "NP1 = %d | NP2 = %d\n", new_put, pipe_[(nb_cmd_p - 1) * 2]);

		// char buffer[80000];
		// ft_bzero(buffer, sizeof(char) * 80000);
		// read(pipe_[(nb_cmd_p - 1) * 2], buffer, 79999);
		// ft_printf(1, "BUFFER = |%s| $ LEN = %d\n", buffer, ft_secure_strlen(buffer));
	}
	if (nb_cmd_p != g_ms.has_pipe - 1)
	{
	//	ft_printf(1, "{%d} FD PIPE OUT = %d\n", nb_cmd_p, pipe_[(nb_cmd_p * 2) + 1]);
		new_put2 = dup2(pipe_[(nb_cmd_p * 2) + 1], STDOUT_FILENO);
	//	ft_printf(2, "NEWPUT 2 = %d\n", new_put2);
	}
	int e = 0;
	while (e < ((g_ms.has_pipe - 1) * 2))
	{
	// 	if (nb_cmd_p != 0)
	// 		close(pipe_[(e)]);
	// 	if (nb_cmd_p != g_ms.has_pipe - 1)
	// 		close(pipe_[(e+ 1)]);
	// //	ft_printf(1, "e = %d | pipe_[e] = %d\n", e, pipe_[e]);
		close(pipe_[e]);
		e++;
	}
	int cmd_return;
	cmd_return = treat_command(cmd, seq);
	//ft_printf(2, "cmd = |%s|\n", *cmd);
	//ft_printf(2, "CMD RTN = %d\n", cmd_return);

	if (cmd_return == ERROR)
	{
	//	ft_printf(2, "When Cmd[0] = |%s|\n", cmd[0]);
		error_command(cmd[0]);
	}
}

static void	treat_child(int nb_cmd_p, int *pipe_, char **cmd, int *seq)
{
	int		in_out[4];
	// // int		cmd_return;
	// int		cursor = 1;
	
	// in_out[0] = dup(STDOUT_FILENO);
	// in_out[1] = dup(STDIN_FILENO);
	// if ((in_out[2] = has_redir_input(0,
	// 	cursor + 1, STDIN_FILENO, cmd, seq)) == -1
	// || (in_out[3] = has_redir_output(0,
	// 	cursor + 1, STDOUT_FILENO, cmd, seq)) == -1)
	// 	return ;
	// ft_printf(1, "FD in_out[2] = %d\n", in_out[2]);
	// ft_printf(1, "FD in_out[3] = %d\n", in_out[3]);
	// in_out[2] = has_redir_input(0, cursor + 1, STDIN_FILENO, cmd, seq);
	// in_out[3] = has_redir_output(0, cursor + 1, STDOUT_FILENO, cmd, seq);
	// if (in_out[2] != STDIN_FILENO)
	// 	dup2(in_out[2], STDIN_FILENO);
	// if (in_out[3] != STDOUT_FILENO)
	// 	dup2(in_out[3], STDOUT_FILENO);
	return (if_in_out(nb_cmd_p, pipe_, (int *)in_out, cmd, seq));
	//int e = 0;
	// while (cmd[e])
	// {
	// 	ft_printf(1, " cmd = |%s|\n", cmd[e]);
	// 	ft_printf(1, " seq = |%d|\n", seq[e]);
	// 	e++;
	// }
	//cmd_return = treat_command(cmd, seq);
	// dup2(in_out[0], STDOUT_FILENO);
	// close(in_out[0]);
	// dup2(in_out[1], STDIN_FILENO);
	// close(in_out[1]);
	//if (cmd_return == ERROR)
	//	error_command(cmd[cursor]);
}

int			**malloc_tab_fpipe(int **tab_fpipe)
{
	int		i;

	i = 0;
	if (!(tab_fpipe = malloc(sizeof(int *) * (g_ms.has_pipe + 1))))
		return (NULL);
	ft_bzero(tab_fpipe, sizeof(int *) * (g_ms.has_pipe + 1));
	while (i < g_ms.has_pipe + 1)
	{
		if (!(tab_fpipe[i] = malloc(sizeof(int) * 3)))
			return (NULL);
		ft_bzero(tab_fpipe[i], sizeof(int) * 3);
		i++;
	}
	return (tab_fpipe);
}

int			child(int *fork_, int nb_cmd_p,
	int *pipe_, char **cmd, int *seq)
{
	if ((*fork_ = fork()) < 0)
		return (0);
	(*fork_ == 0) ? treat_child(nb_cmd_p, pipe_, cmd, seq) : 0;
	(*fork_ == 0) ? exit(0) : 0;
	(*fork_ != 0) ? close(pipe_[(nb_cmd_p * 2) + 1]) : 0;
	return (1);
}

void		cmd_has_pipe(int gen_fork,
	int fork_, int nb_cmd_p, char **cmd, int *seq)
{
	int		status;
	int		gen_status;
	int		**tab_fpipe;

	nb_cmd_p = 0;
	status = 0;
	gen_status = 0;
	tab_fpipe = NULL;
	int 	i = 0;
	int		*pipe_;

	pipe_ = NULL;

	if ((gen_fork = fork()) < 0)
		exit(1258);
	if (gen_fork == 0)
	{
		tab_fpipe = malloc_tab_fpipe(tab_fpipe);

		int j = 0;
		pipe_ = malloc(sizeof(int) * (g_ms.has_pipe * 2) - 1 );

		while (i < g_ms.has_pipe - 1)
		{
			if (pipe(pipe_ + (i * 2)) == -1)
				exit(1);
			i++;
		}

		while (nb_cmd_p < g_ms.has_pipe)
		{
			signal(SIGINT, NULL);
			char **final_cmd = NULL;
			while (cmd[j] && seq[j] != 6 && seq[j] != 9)
			{
				add_word_to_tab(cmd[j], &final_cmd);
				j++;
			}
			if (cmd[j])
				j++;
			int *final_seq = NULL;
			get_sequence(final_cmd, &final_seq);
	//		int e = 0;
	//		while (final_cmd[e])
	//		{
	//			ft_printf(1, "final cmd = |%s|\n", final_cmd[e]);
	//			ft_printf(1, "final seq = |%d|\n", final_seq[e]);
	//			e++;
	//		}
	//		ft_printf(1, "-----\n");
			if (!child(&fork_, nb_cmd_p, pipe_, final_cmd, final_seq))
				break ;
			nb_cmd_p++;
		}
		waitpid(fork_, &status, 0);
		exit(0);
	}
	else
		waitpid(gen_fork, &gen_status, 0);
}
