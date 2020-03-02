/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/03/02 11:37:23 by rchallie         ###   ########.fr       */
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

void	print_middle_term(char *str, int str_len, int row)
{
	t_line line;
	
	raw_term_mode();
    ft_bzero(&line, sizeof(line));
    get_cursor_start_pos(&line);
	ft_getwinsz(&line.winsz);
	line.start.row = row;
	line.start.col = line.winsz.col / 2 - str_len / 2;
	set_curpos(&line);
	ft_printf(str);
	line.start.row = row + 1;
	line.start.col = 0;
	set_curpos(&line);
	default_term_mode();
}
void	clear_term()
{
	t_line line;
	
    raw_term_mode();
    ft_bzero(&line, sizeof(line));
    get_cursor_start_pos(&line);
	ft_getwinsz(&line.winsz);
	tputs(tgoto(tgetstr("SF", NULL), 0, line.winsz.row - 1), 1, &tc_putchar);
	line.start.row = 0;
	line.start.col = 0;
	set_curpos(&line);
	default_term_mode();
}

int main(int ac, char **av, char **envp)
{
	t_minishell		ms;
	int				ret;
    char            *pwd;
	(void)ac;
	(void)av;
	(void)envp;
	ret = 1;

	clear_term();
	print_middle_term("\e[91m┌┬┐\e[92m┬┌┐\e[93m┌┬┌\e[94m─┐┬\e[95m ┬┌\e[96m─┐\e[91m┬  \e[92m┬  ", 24,1);
	print_middle_term("\e[91m││\e[92m│││\e[93m│││\e[94m└─┐\e[95m├─┤\e[96m├┤ \e[91m│  \e[92m│  ", 24,2);
	print_middle_term("\e[91m┴ \e[92m┴\e[93m┴┘└┘\e[94m┴└─\e[95m┘┴ \e[96m┴└─\e[91m┘┴─\e[92m┘┴─┘", 24,3);
	print_middle_term("", 0, 4);
	ft_printf("Il est possible de lancer des executables : (Ex : brew doctor)\n");
	ft_printf("/!\\ Les executables dans les dossiers suivants ne sont pas utilisable :\n");
	ft_printf("	- /usr/bin\n");
	ft_printf("	- /bin\n");
	ft_printf("(pour ne pas être confondu avec ceux demandé dans le sujet)\n\n");

	// print_middle_term("\n",1, 6);

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
		int i = 0;
		while (ms.treated[i])
		{
			printf("[%d] : %s\n", i, ms.treated[i]);
			i++;
		}
		while (ms.seq_cursor < ms.treated_len && ms.treated[ms.seq_cursor])
		{
			//write(1, "d\n", 2);
			printf("Cursor : %d\n", ms.seq_cursor);
			if (ms.sequence[ms.seq_cursor] == 0 && (ms.isexecret = is_exec(&ms)) == ERROR)
			{
				if ((ms.iscmdret = is_cmd(ms.treated[ms.seq_cursor])) != -1)
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
					error_command(ms.treated[ms.seq_cursor]);
			}
			ms.seq_cursor++;
		}
		free_double_char_tab(ms.treated);
		free(ms.sequence);
	}
	exit(0);
}
