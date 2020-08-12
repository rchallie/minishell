/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:16:10 by rchallie          #+#    #+#             */
/*   Updated: 2020/08/12 23:39:35 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

void	get_shell_pwd(char **pwd)
{
	*pwd = ft_strdup(g_pwd);
}

/*
** Function : get_pwd
** -------------------------
**		Set in the first argument the the actual working directory path
**
**		(char **)pwd :	a pointer on a char string, is the value where is
**						stocked the short pwd
**
**		returns:	return 0 error during a malloc
**					return 1 if the actual working directory pwth was get
*/

int		get_pwd(char **pwd)
{
	char *new_pwd;

	new_pwd = NULL;
	if (!(new_pwd = (char *)malloc(sizeof(char) * 1025)))
		return (ERROR);
	ft_bzero(new_pwd, 1025);
	if (getcwd(new_pwd, sizeof(char) * 1024) == NULL)
	{
		free(new_pwd);
		return (ERROR_NO_CURRENT_WORK_DIR);
	}
	*pwd = new_pwd;
	return (SUCCESS);
}

/*
** Function : get_pwd_short
** -------------------------
**		Set in the first argument the actual working directory
**
**		(char **)pwd :	a pointer on a char string, is the value where is
**						stocked the short pwd
**
**		returns:	return 0 :	if an error has catch during the recuperation
**								of the pwd or during a malloc
**					return 1 :	if the actual working directory was get
*/

int		get_pwd_short(char **pwd)
{
	int		pwd_len;
	char	*pwd_tmp;
	char	*pwd_s;
	char	*home;

	get_shell_pwd(&pwd_tmp);
	home = get_env_var_by_name("HOME");
	if (!ft_strcmp(home, pwd_tmp))
	{
		free(pwd_tmp);
		if ((*pwd = ft_strdup("~/")) != NULL)
		{
			free(home);
			return (SUCCESS);
		}
	}
	free(home);
	pwd_len = ft_secure_strlen(pwd_tmp);
	while (pwd_tmp[pwd_len] != '/' && pwd_len != 0)
		pwd_len--;
	pwd_s = ft_strdup(&pwd_tmp[pwd_len]);
	free(pwd_tmp);
	*pwd = pwd_s;
	return (SUCCESS);
}

int		get_pwd_short_from(char **pwd)
{
	int		pwd_len;
	char	*home;
	char	*pwd_s;
	char	*pwd_tmp;

	pwd_tmp = *pwd;
	home = get_env_var_by_name("HOME");
	if (!ft_strcmp(home, pwd_tmp))
	{
		free(pwd_tmp);
		if ((*pwd = ft_strdup("~/")) != NULL)
		{
			free(home);
			return (SUCCESS);
		}
	}
	free(home);
	pwd_len = ft_secure_strlen(pwd_tmp);
	while (pwd_tmp[pwd_len] != '/' && pwd_len != 0)
		pwd_len--;
	pwd_s = ft_strdup(&pwd_tmp[pwd_len]);
	free(pwd_tmp);
	*pwd = pwd_s;
	return (SUCCESS);
}

/*
** Function : print_work_dir
** -------------------------
**		Print actual working directory
**
**		(int)		argc : argument count.
**		(char **)	argv : arguments.
**		(char **)	envp : environment variables.
**
**		returns:	return 0 :	if an error has catch during the
**								recuperation of the pwd
**					return 1 :	if no treatement is needed
**					return 2 : 	if a treatement is needed
*/

int		print_work_dir(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	write(1, g_pwd, ft_strlen(g_pwd));
	write(1, "\n", 1);
	return (0);
}
