/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:48:15 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/25 19:11:03 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

char	*get_env_var_by_name(char *name, char **envp)
{
	int		i;
	char	*rename;
	int		j;

	rename = ft_strdup(name);
	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && rename[j] && envp[i][j] == rename[j])
			j++;
		if (rename[j] == '\0' && envp[i][j] == '=')
		{
			j++;
			free(rename);
			return (ft_strdup(&envp[i][j]));
		}
		i++;
	}
	(void)envp;
	free(rename);
	return (NULL);
}

int remove_var_env(char *var_name)
{
	int     i;
	char	*end_name;

    i = 0;
    while (envp[i])
    {
		end_name = ft_strchr(envp[i], '=');
        if (!ft_strncmp(envp[i], var_name, end_name - envp[i]))
            break ;
        i++;
    }
    if (envp[i] != NULL)
        return (double_char_tab_remove(&envp[i], &envp));
    return (SUCCESS);
}

/*
** Function: add_var_to_env
** ------------
**		Add/update a variable to environement variable if is form
**		is <name>=<variable>
**
**		(char *)	var : variable to add.
**
**		returns: return 1 : if no problem
**				 return 0 : if an error appear
*/

int				add_var_to_env(char *var)
{
	int		i;
	char	*end_name;

	i = 0;
	end_name = ft_strchr(var, '=');
	while (envp[i] != NULL)
	{
		if (!ft_strncmp(envp[i], var, end_name - var)
			&& envp[i][(end_name - var)] == '=')
			break ;
		i++;
	}
	if (envp[i] == NULL)
		return (add_word_to_tab(var, &envp));
	else
	{
		ft_strdel(&envp[i]);
		envp[i] = ft_strdup(var);
	}
	return (SUCCESS);
}

// REMOVE IT
int		check_word(char *word)
{
	int		i;

	i = 0;
	while (word[i] != '=')
		i++;
	if (i == 0 || word[i] != '=')
		return (ERROR);
	return (SUCCESS);
}

int		env(int argc, char **argv, char **envp)
{
	char	*env_list;
	int		cursor;
	int		i;
	
	(void)argc;
	i = 0;
	cursor = 0;
	env_list = NULL;
	while (envp[i])
	{
		env_list = ft_strjoin(env_list, envp[i]);
		env_list = add_char_to_word(env_list, '\n');
		i++;
	}
	ft_printf("%s", env_list);
	while (argv[++cursor])
		/* Voir pour remplace un truc comme 
	end_name = ft_strchr(argv[i], '=');
		if (end_name == NULL)
			end_name = (argv[i] + ft_secure_strlen(argv[i]));
			*/
		if (check_word(argv[cursor]) >= 1)
			ft_printf("%s\n", argv[cursor]);
	return (SUCCESS);
}
