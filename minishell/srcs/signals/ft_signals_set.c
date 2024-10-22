/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals_set.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:35:28 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/22 16:02:04 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	sigint_handler_main(int sig)
{
	(void)sig;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	minishell()->status = 130;
}

void	set_signals(int sigmode)
{
	if (sigmode == SIGMAIN)
	{
		set_sig_handlers(SIGCHLD, handle_sigchild);
		set_sig_handlers(SIGINT, sigint_handler_main);
		set_sig_handlers(SIGQUIT, SIG_IGN);
	}
	else if (sigmode == SIGCMD)
	{
		set_sig_handlers(SIGINT, sigint_handler_cmd);
		set_sig_handlers(SIGQUIT, sigquit_handler_cmd);
	}
	else if (sigmode == SIGHDC)
	{
		set_sig_handlers(SIGINT, sigint_handler_hdc);
		set_sig_handlers(SIGQUIT, SIG_IGN);
	}
	else if (sigmode == SIGHD)
	{
		set_sig_handlers(SIGINT, sigint_handler_hd);
		set_sig_handlers(SIGCHLD, SIG_IGN);
	}
}

int	set_sig_handlers(int signal, void (*func_name)(int))
{
	struct sigaction	sa;

	sa.sa_handler = func_name;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(signal, &sa, NULL) == -1)
	{
		print_err("sigaction failed\n");
		return (1);
	}
	return (0);
}
