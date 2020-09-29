/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 21:46:16 by thervieu          #+#    #+#             */
/*   Updated: 2020/09/27 23:31:28 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

/*
** Function: child_in_out_put
** ------------
**		Set the input and the output of the command.  If
**		an input / output file is set  in  the  command,
**		the input / output of the command was not set on
**		the pipes, otherwise the input / output  is  set
**		on the pipes for the previous (if it's  not  the
**		first) / next (if it's not the last) child.
**
**		params:
**			"nb_cmd_p"	the number of pipes in the command.
**			"pipes_"	the pipes array.
**			"child_cmd"	the child command.
**			"child_seq"	the sequence of elements in the
**						child command.
*/

static void		child_in_out_put(
	int nb_cmd_p,
	int *pipes_,
	char **child_cmd,
	int *child_seq
)
{
	int			fdinput;
	int			fdoutput;

	fdinput = STDIN_FILENO;
	fdoutput = STDOUT_FILENO;
	if (has_redir(child_cmd, child_seq, &fdinput, &fdoutput) != SUCCESS)
		return ;
	if (fdinput != STDIN_FILENO)
		dup2(fdinput, STDIN_FILENO);
	else if (nb_cmd_p != 0)
		dup2(pipes_[0], STDIN_FILENO);
	if (fdoutput != STDOUT_FILENO)
		dup2(fdoutput, STDOUT_FILENO);
	else if (nb_cmd_p != g_ms.has_pipe - 1)
		dup2(pipes_[1], STDOUT_FILENO);
}

/*
** Function: child_treat
** ------------
**		Get the sequence  of  the  "child  command".  Call
**		the function to set the input in  conformity  with
**		sequence / the pipes. Call the command treatement.
**
**		params:
**			"nb_cmd_p"	the number of pipes in the command.
**			"pipes_"	the pipes array.
**			"child_cmd"	the child command.
*/

static void		child_treat(
	int nb_cmd_p,
	int *pipes_,
	char **child_cmd,
	int *fd_in
)
{
	int cmd_return;
	int count_closed_pipes;
	int *child_seq;

	dup2(*fd_in, 0);
	(nb_cmd_p != g_ms.has_pipe - 1) ? dup2(pipes_[1], 1) : 0;
	close(pipes_[0]);
	signal(SIGINT, NULL);
	get_sequence(child_cmd, &child_seq);
	reorder_sequence(&child_cmd, &child_seq);
	child_in_out_put(nb_cmd_p, pipes_, child_cmd, child_seq);
	count_closed_pipes = 0;
	cmd_return = treat_command(child_cmd, child_seq, 0);
	(child_seq) ? free(child_seq) : 0;
	if (cmd_return == ERROR)
		error_command(child_cmd[0]);
	close(pipes_[1]);
	exit(g_ms.last_cmd_rtn);
}

void			treat_command_while(
	int cmd_nb,
	char **cmd,
	int *seq,
	int gen
)
{
	int		fd;
	int		pipes_[3];
	char	**ch_cmd;
	int		fork_;

	fd = 0;
	pipes_[2] = 0;
	while (++cmd_nb < g_ms.has_pipe)
	{
		(pipe(pipes_) == -1) ? exit(2) : 0;
		ch_cmd = NULL;
		while (cmd[pipes_[2]] && seq[pipes_[2]] != 6 && seq[pipes_[2]] != 9)
			add_word_to_tab(cmd[pipes_[2]++], &ch_cmd);
		(cmd[pipes_[2]]) ? pipes_[2]++ : 0;
		((fork_ = fork()) == 0) ? child_treat(cmd_nb, pipes_, ch_cmd, &fd) : 0;
		if (fork_ > 0)
		{
			waitpid(fork_, &gen, 0);
			free_double_char_tab(ch_cmd);
			close(pipes_[1]);
			fd = pipes_[0];
		}
	}
	g_ms.last_cmd_rtn = WEXITSTATUS(gen);
	exit(g_ms.last_cmd_rtn);
}

/*
** Function: cmd_has_pipe
** ------------
**		Create a general fork to split the main process
**		and the process of the "pipes command". Create
**		the pipes array containing the pipes for every
**		childs of the the general (parent) fork.
**
**		params:
**			"cmd"		the command.
**			"seq"		the sequence of elements in the command.
*/

void			cmd_has_pipe(
	char **cmd,
	int *seq
)
{
	int		cmd_nb;
	int		gen_fork;
	int		gen_status;

	gen_fork = 0;
	gen_status = 0;
	cmd_nb = -1;
	if ((gen_fork = fork()) < 0)
		exit(1258);
	if (gen_fork == 0)
	{
		treat_command_while(cmd_nb, cmd, seq, 0);
	}
	else
	{
		waitpid(gen_fork, &gen_status, 0);
		g_ms.last_cmd_rtn = WEXITSTATUS(gen_status);
	}
}
