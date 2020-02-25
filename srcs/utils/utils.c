/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 16:27:25 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/25 13:20:12 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

// int			is_spec

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
	if (save_word)
		free(save_word);
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

int		get_word(char *entry, char **word)
{
	int simple_q;
	int double_q;
	int char_count;

	simple_q = 0;
	double_q = 0;
	char_count = 0;
	// printf("word : |%s|\n", *word);
	// if (*word)
		// printf("PLPO\n");
	while (*entry)
	{
		if ((*entry == ' ' || *entry == '>' || *entry == '<'
			|| *entry == '|' || *entry == ';') && !(simple_q || double_q))
			break ;
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
		entry++;
		char_count++;
	}
	if (simple_q || double_q)
	{
		// printf("s_q : %d | d_q : %d\n", simple_q, double_q);
		exit(1);
	}
	return (char_count);
}

char *get_env_var_by_name(char *name, char **envp)
{
	int i = 0;
	char *rename = ft_strdup(name);
	// printf("prename : %s\n", name);
	while (rename[i])
	{
		// write(1,"a\n", 2);
		rename[i] = ft_toupper(rename[i]);
		i++;
	}
	// printf("Name : %s\n", rename);
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
	printf("NOP\n");
	free(rename);
	return (NULL);
}






