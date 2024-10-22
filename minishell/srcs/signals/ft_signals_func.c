/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals_func.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:53:06 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/22 15:25:09 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_sigchild(int sig)
{
	t_pipex	*pipex_s;
	pid_t	pid;
	int		status;

	(void)sig;
	status = 0;
	while (1)
	{
		pid = waitpid(-1, &status, WNOHANG);
		if (pid == 0 || pid == -1)
			break ;
		pipex_s = minishell()->pipex;
		while (pipex_s && pipex_s->pid != pid)
			pipex_s = pipex_s->next;
		if (pipex_s && pipex_s->pid == pid)
		{
			process_child_pid(pipex_s);
			pipex_s->pid = -1;
		}
	}
}

void	sigint_handler_hdc(int sig)
{
	(void)sig;
	ft_exit(2);
}

void	sigint_handler_hd(int sig)
{
	(void)sig;
	minishell()->status = 130;
}

void	sigquit_handler_cmd(int sig)
{
	(void)sig;
	print_err("%s\n", " Quit (core dumped)");
	minishell()->status = 131;
}

void	sigint_handler_cmd(int sig)
{
	(void)sig;
	printf("\n");
	minishell()->status = 130;
}
