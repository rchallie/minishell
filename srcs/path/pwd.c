/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 09:31:08 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/20 10:24:41 by rchallie         ###   ########.fr       */
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

	if(!get_pwd(pwd))
		return (ERROR);
	pwd_len = ft_secure_strlen(*pwd);
	while (pwd[pwd_len] != '/' && pwd_len != 0)
		pwd_len--;
	pritnf("Char : %c\n", pwd[pwd_len]);
}

// get pwd short
// get pwd long
// change pwd
// change oldpwd