/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 19:14:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/13 18:06:54 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

static int		special_char(char ***treated, char *entry, int up, char c)
{
	char	*word;

	word = NULL;
	if (*(entry + up) == c)
	{
		if (word)
			free(word);
		word = NULL;
		while (*(entry + up) == c)
		{
			word = add_char_to_word(word, *(entry + up));
			up++;
		}
		if (ft_secure_strlen(word) > 2)
		{
			ft_printf("minishell: syntax error near unexpected token `%c'\n",
				c); //Need Output sur l'error
			return (ERROR);
		}
		else
			add_word_to_tab(word, treated);
		while (ft_is_whitespace(*(entry + up)))
			up++;
	}
	return (up);
}

static int		check_special_chars(char ***treated, char *entry, int up)
{
	if (!(up = special_char(treated, entry, up, '>'))
		|| !(up = special_char(treated, entry, up, '<'))
		|| !(up = special_char(treated, entry, up, '|'))
		|| !(up = special_char(treated, entry, up, '>'))
		|| !(up = special_char(treated, entry, up, ';')))
		return (ERROR);
	return (up);
}

static char		*sanitize_home(char *word)
{
	char *home;
	char *tmp_word;

	home = get_env_var_by_name("HOME");
	if (ft_secure_strlen(home) != 0)
		word = ft_strjoin(home, (word + 1));
	else
	{
		tmp_word = ft_strjoin("/Users/", get_env_var_by_name("USER"));
		word = ft_strjoin(tmp_word, (word + 1));
		free(tmp_word);
	}
	free(home);
	return (word);
}

static int		sanitize_loop(int *up, char *entry, char ***treated)
{
	char *word;

	word = NULL;
	while (ft_is_whitespace(*(entry + *up)))
		(*up)++;
	*up += get_word((entry + *up), &entry, &word);
	if (word && word[0] == '~' && (!word[1] || word[1] == '/'))
		word = sanitize_home(word);
	add_word_to_tab(word, treated);
	free(word);
	while (ft_is_whitespace(*(entry + *up)))
		(*up)++;
	if (!(*up = check_special_chars(treated, entry, *up)))
		return (ERROR);
	while (ft_is_whitespace(*(entry + *up)))
		(*up)++;
	return (SUCCESS);
}

int				sanitize(char *entry, char ***treated)
{
	char	*word;
	int		up;

	word = NULL;
	if (!entry || !*entry || entry[0] == '\n')
	{
		if (!(word = (char *)ft_strnew(sizeof(char) * 1)))
			return (ERROR);
		add_word_to_tab(word, treated);
		add_word_to_tab("\n", treated);
		free(word);
		return (SUCCESS);
	}
	up = 0;
	while (*(entry + up))
		if (sanitize_loop(&up, entry, treated) == ERROR)
			return (ERROR);
	free(entry);
	add_word_to_tab("\n", treated);
	return (SUCCESS);
}
