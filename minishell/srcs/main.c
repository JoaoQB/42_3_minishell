/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:42:25 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/20 15:14:18 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// print_tokens(minishell()->tokens);
static void	repl(char **envp)
{
	init_main(envp);
	while (1)
	{
		minishell()->user_input = readline("minishell-> ");
		if (!minishell()->user_input)
			break ;
		input_process(minishell()->user_input);
		ft_shell_pipex();
		free_main_input();
	}
}

t_main	*minishell(void)
{
	static t_main	main_s;

	return (&main_s);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	if (argc != 1)
		return (1);
	if (set_sig_handlers(SIGCHLD, handle_sigchild) != 0)
		return (1);
	repl(envp);
	cleanup_main();
	return (0);
}
