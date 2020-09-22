/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 19:14:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/09/22 17:45:08 by excalibur        ###   ########.fr       */
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

// static int		special_char(char ***treated, char *entry, int up, char c)
// {
// 	char	*word;
// 	char	*word_free;

// 	word = NULL;
// 	if (*(entry + up) == c)
// 	{
// 		while (*(entry + up) == c)
// 		{
// 			word_free = word;
// 			word = add_char_to_word(word, *(entry + up++));
// 			if (word_free)
// 				free(word_free);
// 		}
// 		if (ft_secure_strlen(word) > 2)
// 		{
// 			ft_printf(2,
// 				"minishell: syntax error near unexpected token `%c'\n", c);
// 			return (-1);
// 		}
// 		else
// 			add_word_to_tab(word, treated);
// 		while (ft_is_whitespace(*(entry + up)))
// 			up++;
// 	}
// 	free(word);
// 	return (up);
// }

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

// static int		check_special_chars(char ***treated, char *entry, int up)
// {
// 	if (((up = special_char(treated, entry, up, '>')) == -1)
// 		|| ((up = special_char(treated, entry, up, '<')) == -1)
// 		|| ((up = special_char(treated, entry, up, '|')) == -1)
// 		|| ((up = special_char(treated, entry, up, ';')) == -1))
// 		return (-1);
// 	return (up);
// }

/*
**	Function: sanitize_home
**	--------------------
**		Modify '~' to home directory in and create new word.
**
**		(char *)	word : the word to treat.
**
**		returns:	return : a word with treated '~'.
*/

// static char		*sanitize_home(char *word)
// {
// 	char *home;
// 	char *tmp_word;

// 	home = get_env_var_by_name("HOME");
// 	if (ft_secure_strlen(home) != 0)
// 		word = ft_strjoin(home, (word + 1));
// 	else
// 	{
// 		tmp_word = ft_strjoin("/Users/", get_env_var_by_name("USER"));
// 		word = ft_strjoin(tmp_word, (word + 1));
// 		free(tmp_word);
// 	}
// 	free(home);
// 	return (word);
// }

int	env_var(char **entry, char **word)
{
 	int rtn;
 	char *env_var_name;
 	int accolade;
 	//	 ft_printf(1, "WORD IS NULL 0 = %d\n", (!*word) ? 10 : 20);

 	accolade = 0;
 	env_var_name = NULL;
 	rtn = 0;
 	// ft_printf(1, "Actual char (pre) = %c (%.12s)| %d\n", **entry, *entry, **entry);
 	if (**entry == '$' && *(*entry + 1) && *(*entry + 1) != ' ')
 	{
 		// ft_printf(1, "WORD IS NULL 1 = %d\n", (!*word) ? 10 : 20);
 		(*entry)++;
 		rtn++;
 		if (**entry == '{')
 		{
 			(*entry)++;
 			rtn++;
 			accolade = 1;
 		}
 	 //ft_printf(1, "WORD IS NULL 2 = %d\n", (!*word) ? 10 : 20);
 		while (**entry && **entry != '}' && **entry != '\'' && **entry != '\"' && **entry != '=' && **entry != '$'
 			&& *(*entry - 1) != '?' && (ft_isalnum(**entry) || **entry == '?'
 			|| **entry == '_'))
 		{
 			env_var_name = add_char_to_word_free(env_var_name, **entry);
 			(*entry)++;
 			rtn++;
 			if (ft_isdigit(*(*entry - 1)) && *(*entry - 2) == '$')
 				break ;
 		}
 //	 ft_printf(1, "WORD IS NULL 3 = %d\n", (!*word) ? 10 : 20);
 		//ft_printf(1, "accolade = |%d|\n**entry = |%c|\n", accolade, **entry);
 		if (env_var_name && ft_secure_strlen(env_var_name) == 1
 			&& env_var_name[0] == '?')								// Last return
 		{
 			env_var_name = (g_ms.last_cmd_rtn != -1) ?
 				ft_itoa(g_ms.last_cmd_rtn) : ft_strdup("0");
 		}
 		else if (env_var_name && ((accolade == 1 && **entry == '}') || (accolade == 0)))
 		{								//	 Get en var normaly
 		//	ft_printf(1, "accolade = |%d|\n**entry = |%c|\n", accolade, **entry);
 			env_var_name = get_env_var_by_name(env_var_name);
 			if (accolade == 1)
 			{
 				(*entry)++;
 				rtn++;
 			}
 		}
 		else if (accolade == 1 && **entry != '}')
 		{
 			ft_printf(2, "minishell: unexpected EOF while looking for matching `}'\n");
 			ft_printf(2, "minishell: syntax error: unexpected end of file\n");
 			return (-1);
 		}
 //	 ft_printf(1, "WORD IS NULL 4 = %d\n", (!*word) ? 10 : 20);
 		if (env_var_name && *env_var_name)
 		{
 			*word = ft_strjoin(*word, env_var_name);
 			*word = add_char_to_word_free(*word, '\0');
 		}
 	// ft_printf(1, "WORD IS NULL 5 = %d\n", (!*word) ? 10 : 20);
 		if (*(*entry - 1) == '$' && (**entry == '=' || **entry == '\"'))
 			*word = add_char_to_word_free(*word, '$');
 //	 ft_printf(1, "WORD IS NULL 6 = %d\n", (!*word) ? 10 : 20);
 	}
 	else if (**entry == '$')
 		*word = add_char_to_word_free(*word, '$');
 //	ft_printf(1, "Actual char (end) = %c (%.12s)| %d\n", **entry, *entry, **entry);
 //	ft_printf(1, "wordaaa = |%s|\n", *word);
 	return (rtn);
 }

int	env_var_boucle(char **entry)
{
	int simple_q;
	char *word;
	char *save;
	char *new_entry;

	word = NULL;
	simple_q = 0;
	new_entry = NULL;
	while (**entry)
	{
		word = NULL;
		if (**entry == '\'' && simple_q == 0)
			simple_q = 1;
		else if (**entry == '\'' && simple_q == 1)
			simple_q = 0;
		if (**entry == '$' && simple_q == 0)
		{
			save = *entry;
			if (env_var(entry, &word) == -1)
				return (-1);
			char *to_free;
			to_free = NULL;
			to_free = new_entry;
			new_entry = ft_strjoin(new_entry, word);
			if (word)
				free(word);
			if (to_free)
				free(to_free);
		}
		else
		{
			new_entry = add_char_to_word_free(new_entry, **entry);
			(*entry)++;
		}
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
	char *word;
	int		error_check;

	word = NULL;
	while (ft_is_whitespace(*(entry + *up)))
		(*up)++;
	//ft_printf(1, "entry = %s\n", entry);
	//ft_printf(1, "lasting = %s\n", entry + *up);
	// ft_printf(1, "WORD IS NULL A = %d\n", (!word) ? 10 : 20);
	error_check = get_word((entry + *up), &entry, &word);
		// ft_printf(1, "WORD IS NULL B = %d\n", (!word) ? 10 : 20);
	if (error_check == -1)
		return (ERROR);
	*up += error_check;

	// if (word && word[0] == '~' && (!word[1] || word[1] == '/'))
	// 	word = sanitize_home(word);
	add_word_to_tab(word, treated);
	free(word);
	while (ft_is_whitespace(*(entry + *up)))
		(*up)++;
	// if ((*up = check_special_chars(treated, entry, *up)) == -1)
	// 	return (ERROR);
	// while (ft_is_whitespace(*(entry + *up)))
	// 	(*up)++;
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
	if (env_var_boucle(&entry) == -1)
		return (ERROR); 
	while (*(entry + up))
		if (sanitize_loop(&up, entry, treated) == ERROR)
			return (ERROR);
//	int i = 0;
//	ft_printf(1, "LEN TAB = %d\n",  get_double_char_tab_len(*treated));
//	while (i != get_double_char_tab_len(*treated))
//	{
//		ft_printf(1, "Word = |%s|\n", (*treated)[i]);
//		i++;
//	}
	free(entry);
	add_word_to_tab("\n", treated);
	return (SUCCESS);
}
