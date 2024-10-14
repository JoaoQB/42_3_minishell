/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:35:28 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/14 20:38:50 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// HEREDOC NOT WORKING
static void	sigint_handler_hd(int sig)
{
	(void)sig;
	g_signal = 2;
	free_main_input();
	// printf("exiting heredoc");
	// close(STDIN_FILENO);
}

static void	sigquit_handler_cmd(int sig)
{
	(void)sig;
	printf(" Quit (core dumped)\n");
}

static void	sigint_handler_cmd(int sig)
{
	(void)sig;
	printf("\n");
	minishell()->status = 130;
	// rl_replace_line("", 0);
	// rl_on_new_line();
	// rl_redisplay();
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

// static void	handle_sigquit(int sig)
// {
// 	(void)sig;
// 	g_signal = 2;
// 	// printf("this is an forced core dump\n");
// 	// abort(); //TODO exit with abort value or free and core dump
// 	// free_main_input();
// 	// cleanup_main();
// 	// exit(1); // General error
// }

void handle_sigchild(int sig)
{
    t_pipex *pipex_s;
    pid_t   pid;
    int     status;

    (void)sig;
    while(1)
    {
		status = 0;
        pid = waitpid(-1, &status, WNOHANG);
        if(pid == 0 || pid == -1)
            break;
        pipex_s = minishell()->pipex;
        while (pipex_s && pipex_s->pid != pid)
            pipex_s = pipex_s->next;
        if (pipex_s)
        {
            process_child_pid(pipex_s);
			pipex_s->pid = -1;
            // free_pipex_node(pipex_s); // TODO reddy to test
        }
    }
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

void	set_signals(int sigmode)
{
	if (sigmode == SIGMAIN)
	{
		if (set_sig_handlers(SIGINT, sigint_handler_main) != 0)
			return;
		if (set_sig_handlers(SIGQUIT, SIG_IGN) != 0)
			return;
	}
	else if (sigmode == SIGCMD)
	{
		if (set_sig_handlers(SIGINT, sigint_handler_cmd) != 0)
			return;
		if (set_sig_handlers(SIGQUIT, sigquit_handler_cmd) != 0)
			return;
	}
	// HEREDOC NOT WORKING
	else if (sigmode == SIGHD)
	{
		if (set_sig_handlers(SIGINT, sigint_handler_hd) != 0)
			return;
		if (set_sig_handlers(SIGQUIT, SIG_IGN) != 0)
			return;
	}
}
