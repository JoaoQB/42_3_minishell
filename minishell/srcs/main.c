/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:42:25 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/11 19:09:31 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t g_signal; // Definition

static void	repl(t_main *main_s, char **envp)
{
	// rl_catch_signals = 0;
	init_main(main_s, envp);
	while (1)
	{
		// get_input(main_s);
		main_s->user_input = readline("minishell-> ");
		if (!main_s->user_input)
			break ;
		input_process(main_s, main_s->user_input);
		//print_tokens(main_s->tokens);
		ft_shell_pipex(main_s);
		free_main_input(main_s);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_main	main_s;

	g_signal = 0;
	(void)argv;
	if (argc != 1)
		return (1);
	if (set_sig_handlers(SIGINT, handle_sigint) != 0)
        return (1);
	if (set_sig_handlers(SIGQUIT, handle_sigquit) != 0)
        return (1);
	repl(&main_s, envp);
	cleanup_main(&main_s);
	return (0);
}
