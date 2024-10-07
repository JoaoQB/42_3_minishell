/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:35:28 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/07 20:22:00 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal = 1;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	// rl_point = 0;
	// rl_done = 1;
	// exit(0);
}

void	handle_sigquit(int sig)
{
	(void)sig;
	g_signal = 2;
	// printf("this is an forced core dump\n");
	// abort(); //TODO exit with abort value or free and core dump
	free_main_input();
	// cleanup_main();
	// exit(1); // General error
}

int	set_sig_handlers(int signal, void (*func_name)(int))
{
	struct sigaction	sa;

	sa.sa_handler = func_name; //handle_sigint;
	sa.sa_flags = 0; //teels the karnel no flags no to restart system calls
	sigemptyset(&sa.sa_mask); //block all signals while handling SIGINT
	if (sigaction(signal, &sa, NULL) == -1)
	{
		perror("sigaction failed");
		return (1);
	}
	return (0);
}
