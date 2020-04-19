/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 16:27:25 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/19 16:07:23 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

// int			is_spec

/*
** Function : is_special
** -------------------------
**		Init the value at i in sequ in function of passed string (to_test)
**
**		(char *)to_test : char to compare with special things
**		(int *) seq		: sequence
**		(int)	i		: index in sequ
**
**		returns:	return 1 : if to_test is a special thing
**					return 0 : if not
*/

static int		is_special(char *to_test)
{
	if (!ft_strcmp(to_test, ">") && ft_strlen(to_test) == 1)
		return (SUCCESS);
	else if (!ft_strcmp(to_test, ">>") && ft_strlen(to_test) == 2)
		return (SUCCESS);
	else if (!ft_strcmp(to_test, "<") && ft_strlen(to_test) == 1)
		return (SUCCESS);
	else if (!ft_strcmp(to_test, "|") && ft_strlen(to_test) == 1)
		return (SUCCESS);
	else if (!ft_strcmp(to_test, ";") && ft_strlen(to_test) == 1)
		return (SUCCESS);
	else if (!ft_strcmp(to_test, "\n") && ft_strlen(to_test) == 1)
		return (SUCCESS);
	return (ERROR);
}


char		*add_char_to_word_front(char *word, char c)
{
	int		i;
	char	*save_word;
	int		new_word_len;

	if (!word)
	{
		if (!(word = (char *)malloc(sizeof(char) * 2)))
			return (ERROR);
		word[0] = c;
		word[1] = '\0';
		return (word);
	}
	new_word_len = ft_secure_strlen(word) + 2;
	save_word = word;
	if (!(word = (char *)malloc(sizeof(char) * new_word_len)))
		return (ERROR);
	ft_bzero(word, new_word_len);
	word[0] = c;
	i = 1;
	while (save_word[i - 1])
	{
		word[i] = save_word[i - 1];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char		*add_char_to_word_ads(char *word, char c, int nb)
{
	int		i;
	char	*save_word;
	int		new_word_len;

	if (!word)
	{
		if (!(word = (char *)malloc(sizeof(char) * 2)))
			return (ERROR);
		word[0] = c;
		word[1] = '\0';
		return (word);
	}
	i = -1;
	new_word_len = ft_secure_strlen(word) + 2;
	save_word = word;
	if (!(word = (char *)malloc(sizeof(char) * new_word_len)))
		return (ERROR);
	ft_bzero(word, new_word_len);
	while (++i <= nb)
		word[i] = save_word[i];
	word[i] = c;
	i++;
	while (save_word[i - 1])
	{
		word[i] = save_word[i - 1];
		i++;
	}
	word[i] = '\0';
	return (word);
}
char		*add_char_to_word(char *word, char c)
{
	int		i;
	char	*save_word;
	int		new_word_len;

	if (!word)
	{
		if (!(word = (char *)malloc(sizeof(char) * 2)))
			return (ERROR);
		word[0] = c;
		word[1] = '\0';
		return (word);
	}
	i = 0;
	new_word_len = ft_secure_strlen(word) + 2;
	save_word = word;
	if (!(word = (char *)malloc(sizeof(char) * new_word_len)))
		return (ERROR);
	ft_bzero(word, new_word_len);
	while (save_word[i])
	{
		word[i] = save_word[i];
		i++;
	}
	word[i++] = c;
	word[i] = '\0';
	return (word);
}

void		free_double_char_tab(char **tab_to_free)
{
	int i = 0;
	int tab_len;
	if (!tab_to_free)
		return;
	tab_len = get_double_char_tab_len(tab_to_free);
	if (tab_len == 0)
	{
		free(tab_to_free);	
		return ;
	}
	while (i < tab_len)
		free(tab_to_free[i++]);
	free(tab_to_free);
}

int		get_word(t_minishell *ms, char *entry, char **word)
{
	int simple_q;
	int double_q;
	int char_count;
	char *env_var_name;

	simple_q = 0;
	double_q = 0;
	char_count = 0;
	env_var_name = NULL;
	// printf("word : |%s|\n", *word);
	while (*entry)
	{
		if (*entry == '$')
		{
			entry++;
			char_count++;
			while (entry && (ft_isalnum(*entry) || *entry == '?'))
			{
				env_var_name = add_char_to_word(env_var_name, *entry);
				entry++;
				char_count++;
			}
			// ft_printf("var name : |%s|\n", env_var_name);
			if (ft_secure_strlen(env_var_name) == 1 && env_var_name[0] == '?')
				env_var_name = ft_itoa(ms->last_cmd_rtn);
			else
				env_var_name = get_env_var_by_name(env_var_name, ms->envp); // else env_perso else ""
			*word = ft_strjoin(*word, env_var_name);
			// printf("WORD : |%s|\n", *word);
			*word = add_char_to_word(*word, '\0');
		}
		if ((*entry == ' ' || *entry == '>' || *entry == '<'
			|| *entry == '|' || *entry == ';') && !(simple_q || double_q))
			break ;
		if (*entry == '\\' && simple_q == 0 && double_q == 0 && *(entry + 1))
		{
			entry++;
			*word = add_char_to_word(*word, *entry);
			char_count++;
		}
		else if (double_q == 1 && *entry == '\\' && *(entry + 1) == '\"')
		{
			entry++;
			char_count++;
			*word = add_char_to_word(*word, *entry);
			char_count++;
			entry++;
		}
		if (*entry == '\'' && simple_q == 0 && double_q == 0)
			simple_q = 1;
		else if (*entry == '\'' && double_q == 1)
			*word = add_char_to_word(*word, *entry);
		else if (*entry == '\'')
			simple_q = 0;
		
		if (*entry == '\"' && double_q == 0 && simple_q == 0)
			double_q = 1;
		else if (*entry == '\"' && simple_q == 1)
			*word = add_char_to_word(*word, *entry);
		else if (*entry == '\"')
			double_q = 0;
		if (*entry != '\'' && *entry != '\"')
			*word = add_char_to_word(*word, *entry);
		if (!word)
			return (ERROR);
		// ft_printf("word = |%s|\n", *word);
		if (entry)
			entry++;
		char_count++;
	}
	if (simple_q || double_q)
	{
		// printf("s_q : %d | d_q : %d\n", simple_q, double_q);
		exit(1);
	}
	// ft_printf("\n-1 = |%c|\n", (char)(-1));
	if (is_special(*word) == SUCCESS)
		*word = add_char_to_word(*word, 3);
	return (char_count);
}

char *get_env_var_by_name(char *name, char **envp)
{
	int i = 0;
	char *rename = ft_strdup(name);
	i = 0;
	int j = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && rename[j] && envp[i][j] == rename[j])
			j++;
		if (rename[j] == '\0' && envp[i][j] == '=')
		{
			j++;
			free(rename);
			return(ft_strdup(&envp[i][j]));
		}
		i++;
	}
	(void)envp;
	free(rename);
	return (NULL);
}






