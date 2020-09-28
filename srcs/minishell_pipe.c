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
	int			fdinput = STDIN_FILENO;
	int			fdoutput = STDOUT_FILENO;
	// if ((fdoutput = has_redir_output(0,
	// 	1, STDOUT_FILENO, child_cmd, child_seq)) == -1
	// || (fdinput = has_redir_input(0,
	// 	1, STDIN_FILENO, child_cmd, child_seq)) == -1)
	// 	return ;
	
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
	char **child_cmd
)
{
	int cmd_return;
	int count_closed_pipes;
	int *child_seq;

	get_sequence(child_cmd, &child_seq);
	reorder_sequence(&child_cmd, &child_seq);
	child_in_out_put(nb_cmd_p, pipes_, child_cmd, child_seq);
	count_closed_pipes = 0;
	//while (count_closed_pipes < ((g_ms.has_pipe - 1) * 2))
	//	close(pipes_[count_closed_pipes++]);
	/*for (int i = 0; i < get_double_char_tab_len(child_cmd); i++)
	{
		ft_printf(2, "child = |%d|\n", child_seq[i]);
	}*/
	cmd_return = treat_command(child_cmd, child_seq, 0);
	(child_seq) ? free(child_seq) : 0;
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
/*
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
	(*fork_ == 0) ? exit(g_ms.last_cmd_rtn) : 0;
	(*fork_ != 0) ? close(pipes_[(nb_cmd_p * 2) + 1]) : 0;
	return (1);
}
*/
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
/*
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
		free_double_char_tab(child_cmd);
	}
	waitpid(fork_, &status, 0);
	g_ms.last_cmd_rtn = WEXITSTATUS(status);
	free(pipes_);
	exit(g_ms.last_cmd_rtn);
}
*/
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

#include <errno.h>

void			cmd_has_pipe(
	char **cmd,
	int *seq
)
{
	int		gen_fork;
	int		gen_status;
	int		pipes_[2];
	int		cmd_nb;
	int j = 0;

	gen_fork = 0;
	gen_status = 0;
	cmd_nb = 0;
	if ((gen_fork = fork()) < 0)
		exit(1258);
	if (gen_fork == 0)
	{
		int fd_in = 0;
		int gen = 0;
		while (cmd_nb < g_ms.has_pipe)
		{
			if (pipe(pipes_) == -1)
				exit(2);
			char **child_cmd;
	
			child_cmd = NULL;
			while (cmd[j] && seq[j] != 6 && seq[j] != 9)
				add_word_to_tab(cmd[j++], &child_cmd);
			if (cmd[j])
				j++;	
			int fork_ = 0;

			if ((fork_ = fork()) == 0)
			{
				dup2(fd_in, 0);
				if (cmd_nb != g_ms.has_pipe - 1)
					dup2(pipes_[1], 1);
				close(pipes_[0]);
				
				signal(SIGINT, NULL);


				child_treat(cmd_nb, pipes_, child_cmd);			
				
				close(pipes_[1]);
				exit(g_ms.last_cmd_rtn);
			}
			else
			{
				waitpid(fork_, &gen, 0);
				free_double_char_tab(child_cmd);
				close(pipes_[1]);
				fd_in = pipes_[0];
			}
			cmd_nb++;
		}
		g_ms.last_cmd_rtn = WEXITSTATUS(gen);
		exit(g_ms.last_cmd_rtn);
	}
	else
	{
		waitpid(gen_fork, &gen_status, 0);
		g_ms.last_cmd_rtn = WEXITSTATUS(gen_status);
	}
}
