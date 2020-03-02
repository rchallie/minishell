/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 14:34:30 by rchallie          #+#    #+#             */
/*   Updated: 2020/03/02 13:48:45 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

typedef struct	s_exec
{
	char	*exec;
	char	*exec_path;
	char	*env_path;
	char	**path_list;
	char	**argv;
	int		save_seq_cursor;
}	t_exec;

/*
** Function : init_exec
** -------------------------
**		Init all values need for exec binaries
**
**		(t_exec *)ex :	a pointer to the exec structure at init
**
**		returns:	null
*/

static void 	init_exec(t_exec *ex, t_minishell *ms)
{
	ex->exec = NULL;
	ex->exec_path = NULL;
	ex->env_path = NULL;
	ex->path_list = NULL;
	ex->argv = NULL;
	ex->save_seq_cursor = ms->seq_cursor;
}

static int		init_for_absolute_path(t_exec *ex, t_minishell *ms)
{
	ex->exec = ms->treated[ex->save_seq_cursor];
	add_word_to_tab(ms->treated[ex->save_seq_cursor], &ex->argv);
	// printf("argv : %s\n", ex.argv[0]);
	ex->save_seq_cursor++;
	if (ms->sequence[ex->save_seq_cursor] > 2)
	{
		if (!(ex->argv = (char **)malloc(sizeof(char *) * 2)))
			return (ERROR);
		ft_bzero(ex->argv, sizeof(char *) * 2);
		*ex->argv = NULL;
	}
	else
		while (ms->sequence[ex->save_seq_cursor] && ms->sequence[ex->save_seq_cursor] <= 2)
			add_word_to_tab(ms->treated[ex->save_seq_cursor++], &ex->argv);

	get_pwd(&ex->exec_path);
	ex->exec_path = add_char_to_word(ex->exec_path, '/');
	ex->exec_path = ft_strjoin(ex->exec_path, ex->exec);
	printf("Exec path : %s\n", ex->exec_path);
	return (SUCCESS);
}

// static int		absolute_path(t_exec *ex, t_minishell *ms)
// {

// }

int		is_exec(t_minishell *ms)
{
	t_exec	ex;
	int		ret;
	pid_t	pid;

	init_exec(&ex, ms);
	if (!init_for_absolute_path(&ex, ms))
		return (ERROR);
	// exec = ms->treated[ex.save_seq_cursor];
	// add_word_to_tab(ms->treated[ex.save_seq_cursor], &ex.argv);
	// // printf("argv : %s\n", ex.argv[0]);
	// ex.save_seq_cursor++;
	// faire un argv correct
	
	pid = 0;

	if ((ret = open(ex.exec_path, O_RDONLY)) > 0)
	{
		if ((pid = fork()) == 0)
		{
			close(ret);
			default_term_mode();
			printf("PLOP %d\n", ret);
			ret = execve(ex.exec, ex.argv, ms->envp);
			exit(0);
		} 
		else
		{
			int status;
			waitpid(pid, &status, 0);
			return (SUCCESS);
		}
	}
	
	if (ret == -1){
		ex.env_path = get_env_var_by_name("path", ms->envp);
		ex.path_list = ft_split(ex.env_path, ':');
		int i = 0;
			int j = 0;
		while (ex.path_list[j])
			printf("path : %s\n", ex.path_list[j++]);
		while (i < get_double_char_tab_len(ex.path_list))
		{
			if (ft_strcmp(ex.path_list[i], "/usr/bin") != 0 && ft_strcmp(ex.path_list[i], "/bin") != 0)
			{
				free(ex.exec_path);
				ex.exec_path = NULL;
				ex.exec_path = add_char_to_word(ex.path_list[i], '/');
				ex.exec_path = ft_strjoin(ex.exec_path, ex.exec);
				printf("Exec : %s\n", ex.exec_path);
				
				if ((ret = open(ex.exec_path, O_RDONLY)) > 0)
				{
					if ((pid = fork()) == 0)
					{
						close(ret);
						default_term_mode();
						printf("EXEC FINAL : %s\n", ex.exec_path);
						ret = execve(ex.exec_path, ex.argv, ms->envp);
					}
					else
					{
						int status;
						waitpid(pid, &status, 0);		/* code */
						return (SUCCESS);
					}
				}
			
			}
			i++;
		}
	}
	return (ERROR);
}
