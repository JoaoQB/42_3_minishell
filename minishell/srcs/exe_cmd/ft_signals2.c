/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:53:06 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/20 15:39:07 by fandre-b         ###   ########.fr       */
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
