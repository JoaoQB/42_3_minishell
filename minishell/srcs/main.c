/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:42:25 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/15 12:15:09 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal; // Definition

static void	repl(char **envp)
{
	// rl_catch_signals = 0;
	init_main(envp);
	while (1)
	{
		// get_input();
		minishell()->user_input = readline("minishell-> ");
		if (!minishell()->user_input)
			break ;
		input_process(minishell()->user_input);
		// print_tokens(minishell()->tokens);
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
	g_signal = 0;
	(void)argv;
	if (argc != 1)
		return (1);
	// if (set_sig_handlers(SIGINT, handle_sigint) != 0)
	// 	return (1);
	// if (set_sig_handlers(SIGQUIT, handle_sigquit) != 0)
    //     	return (1);
	// if (set_sig_handlers(SIGCHLD, handle_sigchild) != 0)
	// 	return (1);
	repl(envp);
	cleanup_main();
	return (0);
}
