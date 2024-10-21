/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:35:28 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/20 14:56:44 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	sigint_handler_hd(int sig)
{
	(void)sig;
	ft_exit(2);
}

static void	sigquit_handler_cmd(int sig)
{
	(void)sig;
	print_err("%s\n", " Quit (core dumped)");
}

static void	sigint_handler_cmd(int sig)
{
	(void)sig;
	printf("\n");
	minishell()->status = 130;
}

static void	sigint_handler_main(int sig)
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
		set_sig_handlers(SIGINT, sigint_handler_hd);
		set_sig_handlers(SIGQUIT, SIG_IGN);
	}
	else if (sigmode == SIGHD)
	{
		set_sig_handlers(SIGINT, SIG_IGN);
		set_sig_handlers(SIGCHLD, SIG_IGN);
	}
}
