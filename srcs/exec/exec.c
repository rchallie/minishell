/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 14:34:30 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/27 17:59:07 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		is_exec(t_minishell *ms)
{
	char *exec_path;
	char *env_path;
	char **path_list;
	char **argv;

	exec_path = NULL;
	env_path = NULL;
	path_list = NULL;

	int ret;
	argv = NULL;
	char *exec = ms->treated[ms->seq_cursor];
	ms->seq_cursor++;
	// faire un argv correct
	while (ms->sequence[ms->seq_cursor] == 2)
		add_word_to_tab(ms->treated[ms->seq_cursor++], &argv);

	int i = 0;
	while (argv[i])
	{
		printf("argv[i] : %s\n", argv[i]);
		i++;
	}
	get_pwd(&exec_path);
	exec_path = add_char_to_word(exec_path, '/');
	exec_path = ft_strjoin(exec_path, exec);
	printf("Exec path : %s\n", exec_path);
	pid_t pid = 0;
	if ((ret = open(exec_path, O_RDONLY)) > 0 && (pid = fork()) == 0)
	{
		close(ret);
		default_term_mode();
		printf("PLOP %d\n", ret);
		ret = execve(ms->treated[ms->seq_cursor], argv, ms->envp);
		printf("RET : %d\n", ret);
		default_term_mode();
		exit(0);
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
	}
	
	if (ret == -1){
		env_path = get_env_var_by_name("path", ms->envp);
		path_list = ft_split(env_path, ':');
		int i = 0;
		while (i < get_double_char_tab_len(path_list))
		{
			free(exec_path);
			exec_path = NULL;
			exec_path = add_char_to_word(path_list[i], '/');
			exec_path = ft_strjoin(exec_path, exec);
			printf("Exec : %s\n", exec_path);
			if ((ret = open(exec_path, O_RDONLY)) > 0 /*&& (pid = fork()) == 0*/)
			{
				close(ret);
				default_term_mode();
				printf("PLOP (2) %d\n", ret);
				ret = execve(exec_path, argv, ms->envp);
				extern int	errno;
				printf("RET : %d\n", ret);
				if (ret == -1)
					printf("Errno : %s\n", strerror(errno));

				default_term_mode();
				// exit(0);
			}
			else
			{
				int status;
				waitpid(pid, &status, 0);
				break;
			}
			write(1, "a\n", 2);
			i++;
		}
	}
	
	return (ERROR);
}