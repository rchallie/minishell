/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/01 18:32:36 by user42            #+#    #+#             */
/*   Updated: 2020/10/01 18:36:39 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

int			beg_shlvl(void)
{
	int		i;
	int		shlvl;
	char	*shlvl_final;
	char	*shlvl_string;

	i = 0;
	shlvl = 0;
	shlvl_string = get_env_var_by_name("SHLVL");
	(shlvl_string[0] == '-' || shlvl_string[0] == '+') ? ++i : 0;
	while (shlvl_string[i] <= '9' && shlvl_string[i] >= '0')
		i++;
	if (shlvl_string[i] != '\0')
		shlvl = 1;
	else if (shlvl_string[0] == '-')
		shlvl = 0;
	else
		shlvl = ft_atoi(shlvl_string) + 1;
	(shlvl_string) ? free(shlvl_string) : 0;
	shlvl_string = ft_itoa(shlvl);
	shlvl_final = ft_strjoin("SHLVL=", shlvl_string);
	add_var_to_env(shlvl_final);
	(shlvl_string) ? free(shlvl_string) : 0;
	(shlvl_final) ? free(shlvl_final) : 0;
	return (1);
}

int			beg_pwd(char **env)
{
	char	*pwd;
	char	*forfree;

	pwd = NULL;
	forfree = NULL;
	if (!get_pwd(&g_pwd))
		return (ERROR);
	dup_double_char_tab(env, &g_envp);
	if (bool_get_env_var_by_name("OLDPWD") == 0)
		add_var_to_env("OLDPWD");
	pwd = ft_strdup("PWD=");
	forfree = pwd;
	pwd = ft_strjoin(pwd, g_pwd);
	(forfree) ? free(forfree) : 0;
	add_var_to_env(pwd);
	(pwd) ? free(pwd) : 0;
	return (SUCCESS);
}
