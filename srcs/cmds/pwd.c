/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:16:10 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/25 15:36:31 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		get_pwd(char **pwd)
{
	if (!(*pwd = (char *)malloc(sizeof(char) * 1025)))
		return (ERROR);
	ft_bzero(*pwd, 1025);
	getcwd(*pwd, sizeof(char) * 1024);
	return (SUCCESS);
}

int		get_pwd_short(char **pwd)
{
	int		pwd_len;
	int		new_pwd_len;
	char	*pwd_tmp;
	char	*pwd_s;

	if (!get_pwd(&pwd_tmp))
		return (ERROR);
	pwd_len = ft_secure_strlen(pwd_tmp);
	while (pwd_tmp[pwd_len] != '/' && pwd_len != 0)
		pwd_len--;
	new_pwd_len = ft_secure_strlen(pwd_tmp + pwd_len);
	if (!(pwd_s = (char *)malloc(sizeof(char) * (new_pwd_len + 1))))
		return (ERROR);
	ft_bzero(pwd_s, sizeof(char) * (new_pwd_len + 1));
	new_pwd_len = 0;
	while (pwd_tmp[pwd_len])
	{
		pwd_s[new_pwd_len] = pwd_tmp[pwd_len];
		new_pwd_len++;
		pwd_len++;
	}
	free(pwd_tmp);
	*pwd = pwd_s;
	return (SUCCESS);
}

int		print_work_dir(t_minishell *ms)
{
	char *pwd;

	if (!get_pwd(&pwd))
		return (ERROR);
	ms->output = pwd;
	if (treat_output(ms) == 5)
		ft_printf("PWD : %s\n", pwd);	//A term, le sortir avec echo
	free(pwd);
	return (SUCCESS);
}
