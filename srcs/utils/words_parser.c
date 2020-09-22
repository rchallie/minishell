/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 14:40:44 by excalibur         #+#    #+#             */
/*   Updated: 2020/09/22 17:58:46 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

// Sep : ' ' , char special pas quoté, quand no quote

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cutted_tree_forest.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/21 16:10:38 by excalibur         #+#    #+#             */
/*   Updated: 2020/09/21 19:03:45 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

// static int tree_named_env(char **entry, char **word)
// {
// 	int rtn;
// 	char *env_var_name;
// 	int accolade;
// 		// ft_printf(1, "WORD IS NULL 0 = %d\n", (!*word) ? 10 : 20);

// 	accolade = 0;
// 	env_var_name = NULL;
// 	rtn = 0;
// 	// ft_printf(1, "Actual char (pre) = %c (%.12s)| %d\n", **entry, *entry, **entry);
// 	if (**entry == '$' && *(*entry + 1) && *(*entry + 1) != ' ')
// 	{
// 		// ft_printf(1, "WORD IS NULL 1 = %d\n", (!*word) ? 10 : 20);
// 		(*entry)++;
// 		rtn++;
// 		if (**entry == '{')
// 		{
// 			(*entry)++;
// 			rtn++;
// 			accolade = 1;
// 		}
// 	// ft_printf(1, "WORD IS NULL 2 = %d\n", (!*word) ? 10 : 20);
// 		while (**entry && **entry != '}' && **entry != '\'' && **entry != '\"' && **entry != '=' && **entry != '$'
// 			&& *(*entry - 1) != '?' && (ft_isalnum(**entry) || **entry == '?'
// 			|| **entry == '_'))
// 		{
// 			env_var_name = add_char_to_word_free(env_var_name, **entry);
// 			(*entry)++;
// 			rtn++;
// 			if (ft_isdigit(*(*entry - 1)) && *(*entry - 2) == '$')
// 				break ;
// 		}
// 	// ft_printf(1, "WORD IS NULL 3 = %d\n", (!*word) ? 10 : 20);
// 	//	ft_printf(1, "accolade = |%d|\n**entry = |%c|\n", accolade, **entry);
// 		if (env_var_name && ft_secure_strlen(env_var_name) == 1
// 			&& env_var_name[0] == '?')								 //Last return
// 		{
// 			env_var_name = (g_ms.last_cmd_rtn != -1) ?
// 				ft_itoa(g_ms.last_cmd_rtn) : ft_strdup("0");
// 		}
// 		else if (env_var_name && ((accolade == 1 && **entry == '}') || (accolade == 0)))
// 		{									// Get en var normaly
// 			//ft_printf(1, "accolade = |%d|\n**entry = |%c|\n", accolade, **entry);
// 			env_var_name = get_env_var_by_name(env_var_name);
// 			if (accolade == 1)
// 			{
// 				(*entry)++;
// 				rtn++;
// 			}
// 		}
// 		else if (accolade == 1 && **entry != '}')
// 		{
// 			ft_printf(2, "minishell: unexpected EOF while looking for matching `}'\n");
// 			ft_printf(2, "minishell: syntax error: unexpected end of file\n");
// 			return (-1);
// 		}
// 	// ft_printf(1, "WORD IS NULL 4 = %d\n", (!*word) ? 10 : 20);
// 		if (env_var_name && *env_var_name)
// 		{
// 			*word = ft_strjoin(*word, env_var_name);
// 			*word = add_char_to_word_free(*word, '\0');
// 		}
// 	// ft_printf(1, "WORD IS NULL 5 = %d\n", (!*word) ? 10 : 20);
// 		if (*(*entry - 1) == '$' && (**entry == '=' || **entry == '\"'))
// 			*word = add_char_to_word_free(*word, '$');
// 	// ft_printf(1, "WORD IS NULL 6 = %d\n", (!*word) ? 10 : 20);
// 	}
// 	else if (**entry == '$')
// 		*word = add_char_to_word_free(*word, '$');
// 	//ft_printf(1, "Actual char (end) = %c (%.12s)| %d\n", **entry, *entry, **entry);
// 	//ft_printf(1, "wordaaa = |%s|\n", *word);
// 	return (rtn);
// }

static int		no_quotes(char **entry, char **word, int *simple_q,
	int *double_q)
{
	// ft_printf(1, "No quotes [ %d ] \n", **entry);
	if (**entry == '\'' || **entry == '\"')
	{
		// *simple_q = 1;
        (**entry == '\'') ? (*simple_q = 1) : (*double_q = 1); 
        *word = add_char_to_word_free(*word, **entry);
	}
    else
		*word = add_char_to_word_free(*word, **entry);
    return (0);
	// else if (**entry == '\"')
	// {
	// 	*double_q = 1;
	// 	if (!(*word))
	// 		*word = add_char_to_word(word, '"');
    //     else
    //         *word = add_char_to_word_free(word, '"');
	// }
	// else if (**entry == '\\')
	// {
	// 	//ft_printf(1, "startword3bis = |%s|\n", *entry);
	// 	(*entry)++;
	// 	//ft_printf(1, "startword3bis = |%s|\n", *entry);
	// 	*word = add_char_to_word_free(*word, **entry);
	// 	//ft_printf(1, "word = |%s|\n", *word);
	// 	return (1);
	// }
	// else if (**entry != '$' && **entry != '\0')
}

int				if_quotes(char **entry, char **word, int *simple_q,
	int *double_q)
{
	if (*simple_q == 0 && *double_q == 0)
		return (no_quotes(entry, word, simple_q, double_q));
	else if (*simple_q == 1)
    {
		if (**entry == '\'')
			*simple_q = 0;
    	*word = add_char_to_word_free(*word, **entry);
    }
	else if (*double_q == 1)
	{
		// if (**entry == '\\' && (*(*entry + 1) == '\"' || *(*entry + 1) == '$' || *(*entry + 1) == '\\'))
		// {
		// 	(*entry)++;
		// 	*word = add_char_to_word_free(*word, **entry);
		// 	return (1);
		// }
		// else
        if (**entry == '\"')
			*double_q = 0;
		// else if (**entry != '$')
		*word = add_char_to_word_free(*word, **entry);
	}
	return (0);
}

static void		quote_error(char **startword, char **entry_addr,
char **save_startword, int simple_q)
{
	char		*test;
	int			startword_advencement;

	(simple_q == 1) ? ft_printf(STDOUT_FILENO, "squote > ")
		: ft_printf(STDOUT_FILENO, "dquote > ");
	test = edit_line();
	startword_advencement = *startword - *save_startword;
	test = ft_strjoin("\n", test);
	*startword = ft_strjoin(*save_startword, test);
	*entry_addr = ft_strjoin(*entry_addr, test);
	*save_startword = *startword;
	*startword += startword_advencement;
}

int				get_word(char *startword, char **entry_addr, char **word)
{
	int		simple_q;
	int		double_q;
	int		char_count;
	char	*save_startword;

	simple_q = 0;
	double_q = 0;
	char_count = 0;
	save_startword = startword;
	*word = NULL;
		// ft_printf(1, "WORD IS NULL -2 = %d\n", (!*word) ? 10 : 20);
	// if (ft_secure_strlen(startword) == 1 && *startword == '$')
	// {
	// 	*word = add_char_to_word_free(*word, *startword);
	// 	return (1);
	// }
	//ft_printf(1, "startword1 = |%s|\n", startword);
	while (*startword)
	{
		// ft_printf(1, "WORD IS NULL -1 = %d\n", (!*word) ? 10 : 20);
		//ft_printf(1, "startword2 = |%s|\n", startword);
		// if (*startword == '$' && simple_q == 0)
		// 	char_count += tree_named_env(&startword, word);
		//ft_printf(1, "startword2 = |%s|\n", startword);
		// ft_printf(1, "WORD IS NULL 7 = %d\n", (!*word) ? 10 : 20);

        // COND D'ARRET 


		if ((((*startword == ' ' || *startword == '>' || *startword == '<'
			|| *startword == '|' || *startword == ';')
            && (&(*startword) != &(**entry_addr))) && *(startword - 1) != '\\')
			&& !(simple_q || double_q))
            {
                if (*word)
        			break ;
                else
                {
                    char c = *startword;
                    while (*startword == c)
                    {
                        *word = add_char_to_word_free(*word, *startword);
                        startword++;
                        char_count++;
                    }
                    if ((ft_secure_strlen(*word) > 2 && c == '>') || (ft_secure_strlen(*word) > 1 && c != '>'))
                    {
                        ft_printf(2,
                            "minishell: syntax error near unexpected token `%c%c'\n", c, c);
                        return (-1);
                    }
                    break;
                }
            }
		// ft_printf(1, "WORD IS NULL 8 = %d\n", (!*word) ? 10 : 20);
		//ft_printf(1, "startword3 = |%s|\n", startword);

        // QUOTE 
		if_quotes(&startword, word, &simple_q, &double_q);
        
		// ft_printf(1, "WORD IS NULL 9 = %d\n", (!*word) ? 10 : 20);
		//ft_printf(1, "startword4 = |%s|\n", startword);
        
		// if (*startword*(startword + 1) == ' '))))
		// {
        if (*startword)
        {
            startword++;
            char_count++;
        }
			
		// }
		// ft_printf(1, "WORD IS NULL 10 = %d\n", (!*word) ? 10 : 20);
		//ft_printf(1, "startword5 = |%s|\n", startword);
		if (*startword == '\0' && (simple_q || double_q))
			quote_error(&startword, entry_addr, &save_startword, simple_q);
		// ft_printf(1, "WORD IS NULL 11 = %d\n", (!*word) ? 10 : 20);
	}
    // ft_printf(1, "WORD = |%s|\n", *word);
	// if (word && *word && (is_special_token(*word) == SUCCESS))
	// 	*word = add_char_to_word_free(*word, 3);
	//ft_printf(1, "startword6 = |%s|\n", startword);
	//ft_printf(1, "word = |%s|\n", *word);
	return (char_count);
}


// int get_word_parser(
//     char *start_word,
//     char **entry_addr,
//     char **word
// )
// {
//     int char_count;
//     int simple_quote;
//     int double_quote;

//     char_count = 0;
//     while (*start_word)
//     {
//         if (*start_word == '"')
//             double_quote = (double_quote == 1) ? 0 : 1;
//         else if (*start_word == '\'')
//             simple_quote = (simple_quote == 1) ? 0 : 1;
//         else if (*start_word == ' ' && !(double_quote || simple_quote))
//             break;
//         if (word)
//             *word = add_char_to_word_free();
//         else
//             *word = add_char_to_word();
//         start_word++;
//     }
//     return (char_count);
// }