/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/27 17:18:24 by rchallie         ###   ########.fr       */
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
	ms->isexecret = -1;
	ms->has_spec_uf = 0;
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
	int (*cmd[4])(t_minishell *) = {&cd,&print_work_dir,&exit_minishell,&env};
	while (ret == SUCCESS)
	{
		init_minishell_var(&ms, envp);
		if (!get_pwd_short(&pwd))
			return (ERROR);
		ft_printf("\e[97m[\e[91mm\e[92mi\e[93mn\e[94mi\e[95ms\e[96mh\e[91me\e[92ml\e[93ml\e[97m] \e[91m%s \e[97m> ", pwd);
		free(pwd);
		ret = line_edition(&ms);

		// write(1, "\n", 1); //voir avec Thibault, pour mettre ça à la fin cd la line
		if (!sanitize(ms.entry, &ms.treated))
			return (0); 
		// ms.treated_len = get_double_char_tab_len(ms.treated);
		get_sequence(ms.treated, &ms.sequence);
		// int i = 0;
		// while (ms.treated[i])
		// {
		// 	printf("[%d] : %s\n", i, ms.treated[i]);
		// 	i++;
		// }
		while (ms.seq_cursor < ms.treated_len && ms.treated[ms.seq_cursor])
		{
			//write(1, "d\n", 2);
			printf("Cursor : %d\n", ms.seq_cursor);
			if (ms.sequence[ms.seq_cursor] == 0 && (ms.isexecret = is_exec(&ms)) == ERROR && (ms.iscmdret = is_cmd(ms.treated[ms.seq_cursor])) != -1)
			{
				//write(1, "A\n", 2);
				if (ms.iscmdret >= 0 && ms.iscmdret <= 3)
				{
					// has_pipe
					int cmd_ret = 0;
					int o = ms.seq_cursor + 1;
					while (ms.sequence[o]
						&& !(ms.sequence[o] >= 3
						&& ms.sequence[o] <= 6))
						o++;
					if (ms.sequence[o] >= 3 && ms.sequence[o] <= 6)
						ms.has_spec_uf = 1;
					if((cmd_ret = cmd[ms.iscmdret](&ms)) == TREAT)
						treat_output(&ms);
					else
						printf("CMD RET : %d\n", cmd_ret);
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