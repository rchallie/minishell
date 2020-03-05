/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:16:10 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/27 14:54:47 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

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
	getcwd(new_pwd, sizeof(char) * 1024);
	*pwd = new_pwd;
	// *pwd = add_char_to_word(new_pwd, '\n');
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
	while (pwd_tmp[pwd_len/* + 1*/])
	{
		pwd_s[new_pwd_len] = pwd_tmp[pwd_len];
		new_pwd_len++;
		pwd_len++;
	}
	free(pwd_tmp);
	*pwd = pwd_s;
	return (SUCCESS);
}

/*
** Function : print_work_dir
** -------------------------
**		Print actual working directory
**
**		(t_minishell *)ms : minishell global variables
**
**		returns:	return 0 :	if an error has catch during the
**								recuperation of the pwd
**					return 1 :	if no treatement is needed
**					return 2 : 	if a treatement is needed
*/

int		print_work_dir(t_minishell *ms)
{
	char *pwd;

	(void)ms;
	if (!get_pwd(&pwd))
		return (ERROR);
	write(1, pwd, ft_strlen(pwd));
	return (SUCCESS);
}
