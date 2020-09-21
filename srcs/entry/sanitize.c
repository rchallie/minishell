/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 19:14:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/28 15:31:14 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

/*
**	Function: special_char
**	--------------------
**		Check for all for special char can be converted to
**		token. Check error.
**
**		(char ***)	treated : a pointer to the double array
**							  of tokens.
**		(char *)	entry : the command line.
**		(int)		int : up (cursor) value in entry.
**		(char)		c : char to check.
**
**		returns:	return : the number of how many treatement cursor
**							 evolved.
*/

static int		special_char(char ***treated, char *entry, int up, char c)
{
	char	*word;
	char	*word_free;

	word = NULL;
	if (*(entry + up) == c)
	{
		while (*(entry + up) == c)
		{
			word_free = word;
			word = add_char_to_word(word, *(entry + up++));
			if (word_free)
				free(word_free);
		}
		if (ft_secure_strlen(word) > 2)
		{
			ft_printf(2,
				"minishell: syntax error near unexpected token `%c'\n", c);
			return (-1);
		}
		else
			add_word_to_tab(word, treated);
		while (ft_is_whitespace(*(entry + up)))
			up++;
	}
	free(word);
	return (up);
}

/*
**	Function: check_special_chars
**	--------------------
**		Check for all possible special characters.
**
**		(char *)	entry : the command line.
**		(char ***)	treated : a pointer to the double array
**							  of tokens.
**		(int)		int : up (cursor) value in entry.
**
**		returns:	return : the number of how many treatement cursor
**							 evolved.
*/

static int		check_special_chars(char ***treated, char *entry, int up)
{
	if (((up = special_char(treated, entry, up, '>')) == -1)
		|| ((up = special_char(treated, entry, up, '<')) == -1)
		|| ((up = special_char(treated, entry, up, '|')) == -1)
		|| ((up = special_char(treated, entry, up, ';')) == -1))
		return (-1);
	return (up);
}

/*
**	Function: sanitize_home
**	--------------------
**		Modify '~' to home directory in and create new word.
**
**		(char *)	word : the word to treat.
**
**		returns:	return : a word with treated '~'.
*/

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
	char *word;
	int		error_check;

	word = NULL;
	while (ft_is_whitespace(*(entry + *up)))
		(*up)++;
	//ft_printf(1, "entry = %s\n", entry);
	//ft_printf(1, "lasting = %s\n", entry + *up);
	error_check = get_word((entry + *up), &entry, &word);
	if (error_check == -1)
		return (ERROR);
	*up += error_check;
	if (word && word[0] == '~' && (!word[1] || word[1] == '/'))
		word = sanitize_home(word);
	add_word_to_tab(word, treated);
	free(word);
	while (ft_is_whitespace(*(entry + *up)))
		(*up)++;
	if ((*up = check_special_chars(treated, entry, *up)) == -1)
		return (ERROR);
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
		free(word);
		free(entry);
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
