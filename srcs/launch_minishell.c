/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_minishell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 21:00:18 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/27 21:00:20 by thervieu         ###   ########.fr       */
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
	ms->has_spec_uf = 0;
}

void	launch(t_minishell *ms, int i, int j)
{
	t_line line;
	char *str = NULL;
	char *launching = "Launching minishell.exe ...";
	int delay = 0;

	(void)ms;
    raw_term_mode();
    ft_bzero(&line, sizeof(line));
    get_cursor_start_pos(&line);
	ft_getwinsz(&line.winsz);
	tputs(tgoto(tgetstr("SF", NULL), 0, line.winsz.row - 1), 1, &tc_putchar);
	while (ft_secure_strlen(str) < ft_secure_strlen(launching))
	{
		i = 0;
		str = add_char_to_word(str, launching[ft_secure_strlen(str)]);
		line.start.row = line.winsz.row / 2;
		line.start.col = line.winsz.col / 2 - ft_secure_strlen(launching) / 2;
		set_curpos(&line);
		ft_putstr_fd(str, 0);
		while (i++ < 100000)
		{
			j = 0;
			if (ft_secure_strlen(str) < ft_secure_strlen(launching) - 3
				&& str[ft_strlen(str) - 1] != ' ')
				delay = 300;
			else
				delay = 3000;
			while (j < delay)
				j++;
		}
	}
	launch_(line);
}

int main(int ac, char **av, char **envp)
{
	t_minishell		ms;
	int				ret;

	(void)ac;
	(void)av;
	(void)envp;
	ret = 1;
	init_minishell_var(&ms, envp);
	launch(&ms, 0, 0);
	default_term_mode();
	int (*cmd[4])(t_minishell *) = {&cd,&print_work_dir,&exit_minishell,&env};
	while (ret == SUCCESS)
	{
		ret = line_edition(&ms);	
		write(1, "\n", 1); //voir avec Thibault, pour mettre ça à la fin cd la line
		if (!sanitize(ms.entry, &ms.treated))
			return (0); 
		ms.treated_len = get_double_char_tab_len(ms.treated);
		get_sequence(ms.treated, &ms.sequence);
		while (ms.seq_cursor < ms.treated_len && ms.treated[ms.seq_cursor])
		{
			//write(1, "d\n", 2);
			printf("Cursor : %d\n", ms.seq_cursor);
			if (ms.sequence[ms.seq_cursor] == 0 && (ms.iscmdret = is_cmd(ms.treated[ms.seq_cursor])) != -1)
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
