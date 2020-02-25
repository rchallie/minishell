/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/21 15:42:56 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

int main(int ac, char **av, char **envp)
{
	char	*entry;
	char	**treated;
	int		*sequence;
	int		ret;

	(void)ac;
	(void)av;
	(void)envp;
	ret = 1;
	entry = NULL;
	treated = NULL;
	default_term_mode();
	while (ret == SUCCESS)
	{
		char *pwd;
		if (!get_pwd_short(&pwd))
			return (ERROR);
		ft_printf("[minishell] %s > ", pwd);
		free(pwd);
		ret = line_edition(&entry);	
		treated = NULL;
		if (!sanitize(entry, &treated))
			return (0);
		int i = 0;
		while (treated[i])
		{
			ft_printf("\n[%d] : %s", i, treated[i]);
			i++;
		}
		get_sequence(treated, &sequence);
		// printf("Sanitized : %s\n", treated);
		// t_args args;
		// t_args args2;
		// char *test;
		// char *test2;
		// test = ft_strdup("\"test= test\"");
		// test2 = ft_strdup("test2=test2");
		// args.content = test;
		// args2.content = test2;
		// args2.next = NULL;
		// args.next = &args2;
		// export(&args);
		// env(envp);
		// printf("Treated entry : |%s|\n", treated);
		// print_work_dir();
		// cd("srcs/err/error.c");
		// while(1);
	}
	exit(0);
}