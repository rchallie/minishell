/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 09:31:08 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/20 11:29:33 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int get_pwd(char **pwd)
{
	if (!(*pwd = (char *)malloc(sizeof(char) * 1025)))
		return (ERROR);
	ft_bzero(*pwd, 1025);
	getcwd(*pwd, sizeof(char) * 1024);
	return (SUCCESS);
}

int get_pwd_short(char **pwd)
{
	int pwd_len;
	char *pwd_s;
	(void)pwd;
	if(!get_pwd(&pwd_s))
		return (ERROR);
	pwd_len = ft_secure_strlen(pwd_s);
	while (pwd_s[pwd_len] != '/' && pwd_len != 0)
		pwd_len--;
	printf("Char : %c\n", pwd_s[pwd_len]);
	return (SUCCESS);
}

// get pwd short
// get pwd long
// change pwd
// change oldpwd