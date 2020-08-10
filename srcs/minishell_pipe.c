/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 21:46:16 by thervieu          #+#    #+#             */
/*   Updated: 2020/08/10 23:24:19 by excalibur        ###   ########.fr       */
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

	if ((fdoutput = has_redir_output(0,
		1, STDOUT_FILENO, child_cmd, child_seq)) == -1
	|| (fdinput = has_redir_input(0,
		1, STDIN_FILENO, child_cmd, child_seq)) == -1)
		return ;
	if (fdinput != STDIN_FILENO)
		dup2(fdinput, STDIN_FILENO);
	else if (nb_cmd_p != 0)
		dup2(pipes_[(nb_cmd_p - 1) * 2], STDIN_FILENO);
	if (fdoutput != STDOUT_FILENO)
		dup2(fdoutput, STDOUT_FILENO);
	else if (nb_cmd_p != g_ms.has_pipe - 1)
		dup2(pipes_[(nb_cmd_p * 2) + 1], STDOUT_FILENO);
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
	char **child_cmd
)
{
	int cmd_return;
	int count_closed_pipes;
	int *child_seq;

	get_sequence(child_cmd, &child_seq);
	child_in_out_put(nb_cmd_p, pipes_, child_cmd, child_seq);
	count_closed_pipes = 0;
	while (count_closed_pipes < ((g_ms.has_pipe - 1) * 2))
		close(pipes_[count_closed_pipes++]);
	cmd_return = treat_command(child_cmd, child_seq);
	if (cmd_return == ERROR)
		error_command(child_cmd[0]);
}

/*
** Function: child
** ------------
**		Protocol  for   every   childs   of    forks.
**		Send to input / output and command treatment.
**		Exit after treatment. Close the pipe  if  the
**		fork doesn't work.
**
**		params:
**			"fork_"		the address that will contain the fork pid.
**			"nb_cmd_p"	the number of pipes in the command.
**			"pipes_"	the pipes array.
**			"child_cmd"	the child command.
*/

static int		child(
	int *fork_,
	int nb_cmd_p,
	int *pipes_,
	char **child_cmd
)
{
	if ((*fork_ = fork()) < 0)
		return (0);
	(*fork_ == 0) ? child_treat(nb_cmd_p, pipes_, child_cmd) : 0;
	(*fork_ == 0) ? exit(0) : 0;
	(*fork_ != 0) ? close(pipes_[(nb_cmd_p * 2) + 1]) : 0;
	return (1);
}

/*
** Function: parent
** ------------
**		Protocol  for  the  parent.  Segment  the  group
**		"pipes command" to get all commands individualy.
**		and create a child for every commands.
**
**		params:
**			"pipes_"	the pipes array.
**			"cmd"		the command.
**			"seq"		the sequence of elements in the command.
*/

static int		parent(
	int *pipes_,
	char **cmd,
	int *seq
)
{
	int		status;
	int		nb_cmd_p;
	int		fork_;
	int		j;
	char	**child_cmd;

	nb_cmd_p = 0;
	fork_ = 0;
	j = 0;
	while (nb_cmd_p < g_ms.has_pipe)
	{
		signal(SIGINT, NULL);
		child_cmd = NULL;
		while (cmd[j] && seq[j] != 6 && seq[j] != 9)
			add_word_to_tab(cmd[j++], &child_cmd);
		if (cmd[j])
			j++;
		if (!child(&fork_, nb_cmd_p, pipes_, child_cmd))
			break ;
		nb_cmd_p++;
	}
	waitpid(fork_, &status, 0);
	free(pipes_);
	exit(0);
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
	int		gen_fork;
	int		gen_status;
	int		*pipes_;
	int		pipes_count;

	gen_fork = 0;
	gen_status = 0;
	pipes_ = NULL;
	pipes_count = 0;
	if ((gen_fork = fork()) < 0)
		exit(1258);
	if (gen_fork == 0)
	{
		pipes_ = malloc(sizeof(int) * (g_ms.has_pipe * 2) - 1);
		while (pipes_count < g_ms.has_pipe - 1)
		{
			if (pipe(pipes_ + (pipes_count * 2)) == -1)
				exit(1);
			pipes_count++;
		}
		parent(pipes_, cmd, seq);
	}
	else
		waitpid(gen_fork, &gen_status, 0);
}
