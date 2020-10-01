/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/01 16:03:22 by user42            #+#    #+#             */
/*   Updated: 2020/10/01 16:13:22 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

static int		sanitize_env_add_char(
	char ***entry,
	char **save,
	char **new_entry,
	int simple_q
)
{
	char	*word;
	char	*to_free;

	to_free = NULL;
	word = NULL;
	if (***entry == '$' && simple_q == 0)
	{
		if (sanitize_env_var(*entry, &word, save) == -1)
			return (-1);
		to_free = *new_entry;
		*new_entry = ft_strjoin(*new_entry, word);
		(word) ? free(word) : 0;
		(to_free) ? free(to_free) : 0;
	}
	else
		*new_entry = add_char_to_word_free(*new_entry, *(**entry)++);
	return (0);
}

static int		sanitize_env_loop(char **entry)
{
	int		simple_q;
	char	*save;
	char	*new_entry;

	simple_q = 0;
	new_entry = NULL;
	save = *entry;
	while (**entry)
	{
		if (**entry == '\'' && simple_q == 0)
			simple_q = 1;
		else if (**entry == '\'' && simple_q == 1)
			simple_q = 0;
		else if (**entry == '\\')
		{
			new_entry = add_char_to_word_free(new_entry, *(*entry)++);
			new_entry = add_char_to_word_free(new_entry, *(*entry)++);
			continue;
		}
		if (sanitize_env_add_char(&entry, &save, &new_entry, simple_q) == -1)
			return (-1);
	}
	*entry = new_entry;
	return (1);
}

/*
**	Function: sanitize_loop
**	--------------------
**		Evolve in commande line and add token to treated
**		double array of tokens.
**
**		(int *)		int : a pointer to the up (cursor) value.
**		(char *)	entry : the command line.
**		(char ***)	treated : a pointer to the double array
**							  of tokens.
**
**		returns:	return 0 : an error appear.
**					return 1 : no problem.
*/

static int		sanitize_loop(int *up, char *entry, char ***treated)
{
	char	*word;
	int		error_check;

	word = NULL;
	while (ft_is_whitespace(*(entry + *up)))
		(*up)++;
	error_check = 0;
	if (get_word_bonus((entry + *up), &entry, &word, &error_check) == -1)
		return (ERROR);
	if (error_check == -1)
		return (ERROR);
	*up += error_check;
	add_word_to_tab(word, treated);
	(word) ? free(word) : 0;
	while (ft_is_whitespace(*(entry + *up)))
		(*up)++;
	return (SUCCESS);
}

/*
**	Function: sanitize
**	--------------------
**		Sanitize the command line and fill it into a treated
**		double array of tokens.
**
**		(char *)	entry : the command line.
**		(char ***)	treated : a pointer to the double array
**							  of tokens.
**
**		returns:	return 0 : an error appear.
**					return 1 : no problem.
*/

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
		(word) ? free(entry) : 0;
		(entry) ? free(entry) : 0;
		return (SUCCESS);
	}
	up = 0;
	if (sanitize_env_loop(&entry) == -1)
		return (ERROR);
	while (*(entry + up))
		if (sanitize_loop(&up, entry, treated) == ERROR)
			return (ERROR);
	(entry) ? free(entry) : 0;
	add_word_to_tab("\n", treated);
	return (SUCCESS);
}
