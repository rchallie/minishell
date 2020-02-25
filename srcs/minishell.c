/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/25 15:27:11 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

static void	init_minishell_var(t_minishell *ms, char **envp)
{
	ms->entry = NULL;
	ms->treated = NULL;
	ms->envp = envp;
	ms->sequence = NULL;
	ms->seq_cursor = 0;
	ms->iscmdret = -1;
}

int main(int ac, char **av, char **envp)
{
	t_minishell		ms;
	int				ret;
	char			*pwd;

	(void)ac;
	(void)av;
	ret = 1;
	default_term_mode();
	int (*cmd[2])(t_minishell *) = {&cd,&print_work_dir};
	while (ret == SUCCESS)
	{
		init_minishell_var(&ms, envp);
		if (!get_pwd_short(&pwd))
			return (ERROR);
		ft_printf("[minishell] %s > ", pwd);
		free(pwd);
		ret = line_edition(&ms.entry);	
		write(1, "\n", 1); //voir avec Thibault, pour mettre ça à la fin cd la line
		if (!sanitize(ms.entry, &ms.treated))
			return (0); 
		get_sequence(ms.treated, &ms.sequence);
		while (ms.treated[ms.seq_cursor])
		{
			if (ms.sequence[ms.seq_cursor] == 0 && (ms.iscmdret = is_cmd(ms.treated[ms.seq_cursor])) != -1)
			{
				if (ms.iscmdret >= 0 && ms.iscmdret <= 1)
				{
					/*int retcmd = */cmd[ms.iscmdret](&ms);
					// printf("ret cmd : %d\n", retcmd);
				}
			}
			else if (ms.sequence[ms.seq_cursor] == 0 && ms.iscmdret == -1 && ms.treated[ms.seq_cursor][0])
			{
				error_command(ms.treated[ms.seq_cursor]);
			}
			ms.seq_cursor++;
		}
		free_double_char_tab(ms.treated);
		free(ms.sequence);
	}
	exit(0);
}