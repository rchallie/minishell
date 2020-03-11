/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/09 13:13:15 by thervieu          #+#    #+#             */
/*   Updated: 2020/03/09 13:13:17 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

char    *export_bash_word(char **word)
{
    char    *declare_x;
    char    *save;
    int     i;

    declare_x = "declare -x ";
    i = -1;
    save = NULL;
    save = *word;
    save = add_char_to_word(save, '\"');
    while (++i <= (int)ft_strlen(declare_x) - 1)
        save = add_char_to_word_front(save, declare_x[ft_strlen(declare_x) - 1 - i]);
    i = 0;
    while (save[i] != '=')
        i++;
    if (save[i] == '=')
        save = add_char_to_word_ads(save, '\"', i);
    return (save);
}

char    **export_bash(char ***sort_me)
{
    char    *declare_x;
    char    **save_sort_me;
    int     i;

    i = -1;
    declare_x = "declare -x ";
    save_sort_me = *sort_me;
    while (save_sort_me[++i] && ft_strncmp(declare_x, save_sort_me[i], ft_strlen(declare_x) != 0))
        save_sort_me[i] = export_bash_word(&save_sort_me[i]);
    return (save_sort_me);
}

char    **bubble_sort(char ***sort_me)
{
    char    *tmp;
    char    **save;
    int     i;
    int     j;

    i = -1;
    save = *sort_me;
    while (save[++i])
    {
        j = -1;
        while (save[++j] && save[j + 1])
        {
            if (ft_strcmp((const char *)save[j], (const char *)save[j + 1]) > 0)
            {
                tmp = save[j];
                save[j] = save[j + 1];
                save[j + 1] = tmp;
            }
        }
    }
    return (save);
}

int     check_word_envp(t_minishell *ms, char *word)
{
    int     i;

    i = -1;
    ft_printf("BEG CHECK\n");
    while (word[++i])
    {
        if (is_spec(word[i]) == SUCCESS)
        {
            ft_printf("bash: export: `%s': not a valid identifier\n", word);
            return (0);
        }
    }
    ft_printf("no spec\n");
    i = -1;
    while (ms->envp[++i])
        if (ft_strcmp(ms->envp[i], word) == 0)
            return (0);
    ft_printf("not in envp\n");
    i = -1;
    while (word[++i])
        if (word[i] == '=')
            return (1);
    ft_printf("no =\n");
    i = -1;
    if (ms->envp_local)
        while (ms->envp_local[++i])
            if (ft_strcmp(ms->envp_local[i], word) == 0)
                return (0);
    ft_printf("not in local\n");
    ft_printf("END CHECK\n");
    return (2);
    }

int     export_(t_minishell *ms)
{
    char    **sort_me;
    int     ret;
    int     cursor;
    int     i;

    i = -1;
    ret = 1;
    sort_me = NULL;
    cursor = ms->seq_cursor;
    while (ret >= SUCCESS && ms->sequence[++cursor] == 2)
        if ((ret = check_word_envp(ms, ms->treated[cursor])) == SUCCESS)
            ret = add_word_to_tab(ms->treated[cursor], &ms->envp);
        else if (ret >= 2)
            ret = add_word_to_tab(ms->treated[cursor], &ms->envp_local);
    ft_printf("about to write envp_local[0]\n");
    ft_printf("i = |%d| envp_local[%d] = |%s|\n", 0, 0, ms->envp_local[0]);
    if (cursor != ms->seq_cursor + 1)
        return (SUCCESS);
    ret = 1;
    while (ret >= SUCCESS && ms->envp[++i])
        ret = add_word_to_tab(ms->envp[i], &sort_me);
    i = 0;
    ret = 1;
    while (ret >= SUCCESS && ms->envp_local[i])
    {
        ft_printf("i = |%d|\n", i);
        ret = add_word_to_tab(ms->envp_local[i], &sort_me);
        i++;
    }
    // i = -1;
    // while (ms->envp_local[++i])
    //     ft_printf("i = |%d| envp_local[%d] = |%s|\n", i, i, ms->envp_local[i]);
    ft_printf("i = |%d| envp_local[%d] = |%s|\n", 0, 0, ms->envp_local[0]);
    if (ret == ERROR)
    {
        free_double_char_tab(sort_me);
        exit(1);
    }
    sort_me = bubble_sort(&sort_me);
    sort_me = export_bash(&sort_me);
    i = -1;
    while (sort_me[++i])
        ft_printf("%s\n", sort_me[i]);
    return (SUCCESS);
}
