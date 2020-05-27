/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 21:46:16 by thervieu          #+#    #+#             */
/*   Updated: 2020/05/26 17:55:34 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

static void	if_in_out(int nb_cmd_p,
	int **tab_fpipe, int *in_out)
{
	if (nb_cmd_p == 0)
	{
		if (in_out[3] != STDOUT_FILENO)
			dup2(tab_fpipe[nb_cmd_p][1], in_out[3]);
		else
			dup2(tab_fpipe[nb_cmd_p][1], STDOUT_FILENO);
		close(tab_fpipe[nb_cmd_p][0]);
		// close(tab_fpipe[nb_cmd_p][1]);
	}
	else if (nb_cmd_p < g_ms.has_pipe - 1)
	{
		dup2(tab_fpipe[nb_cmd_p - 1][0], in_out[2]);
		if (in_out[3] != STDOUT_FILENO)
			dup2(tab_fpipe[nb_cmd_p][1], in_out[3]);
		else
			dup2(tab_fpipe[nb_cmd_p][1], STDOUT_FILENO);
	}
	else if (nb_cmd_p == g_ms.has_pipe - 1)
		dup2(tab_fpipe[nb_cmd_p - 1][0], in_out[2]);
}

static void	treat_child(int nb_cmd_p, int **tab_fpipe, char **cmd, int *seq)
{
	int		in_out[4];
	int		cmd_return;
	int		cursor = 1;
	
	in_out[0] = dup(STDOUT_FILENO);
	in_out[1] = dup(STDIN_FILENO);
	if ((in_out[2] = has_redir_input(0,
		cursor + 1, STDIN_FILENO, cmd, seq)) == -1
	|| (in_out[3] = has_redir_output(0,
		cursor + 1, STDOUT_FILENO, cmd, seq)) == -1)
		return ;
	// in_out[2] = has_redir_input(0, cursor + 1, STDIN_FILENO, cmd, seq);
	// in_out[3] = has_redir_output(0, cursor + 1, STDOUT_FILENO, cmd, seq);
	if (in_out[3] != STDOUT_FILENO)
		dup2(in_out[3], STDOUT_FILENO);
	if (in_out[2] != STDIN_FILENO)
		dup2(in_out[2], STDIN_FILENO);
	if_in_out(nb_cmd_p, tab_fpipe, (int *)in_out);
	cmd_return = treat_command(cmd, seq);
	dup2(in_out[0], STDOUT_FILENO);
	close(in_out[0]);
	dup2(in_out[1], STDIN_FILENO);
	close(in_out[1]);
	if (cmd_return == ERROR)
		error_command(cmd[cursor]);
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
	int **tab_fpipe, char **cmd, int *seq)
{
	if (pipe(tab_fpipe[nb_cmd_p]) == -1)
		return (0);
	if ((*fork_ = fork()) < 0)
		return (0);
	(*fork_ == 0) ? treat_child(nb_cmd_p, tab_fpipe, cmd, seq) : 0;
	(*fork_ == 0) ? exit(0) : 0;
	(*fork_ != 0) ? close(tab_fpipe[nb_cmd_p][1]) : 0;
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
	if ((gen_fork = fork()) < 0)
		exit(1258);
	if (gen_fork == 0)
	{
		tab_fpipe = malloc_tab_fpipe(tab_fpipe);
		while (nb_cmd_p < g_ms.has_pipe)
		{
			signal(SIGINT, NULL);
			if (!child(&fork_, nb_cmd_p, tab_fpipe, cmd, seq))
				break ;
			nb_cmd_p++;
		}
		waitpid(fork_, &status, 0);
		exit(0);
	}
	else
		waitpid(gen_fork, &gen_status, 0);
}
