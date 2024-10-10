/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:44:00 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/09 22:39:32 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_child_pid(t_pipex *curr_pipex_s)
{
	int		status;

	if (waitpid(curr_pipex_s->pid, &status, WNOHANG))
	{
		curr_pipex_s->pid = -1;
		if (curr_pipex_s->pipe_fd[0] > 2)
		{
			ft_close(curr_pipex_s->pipe_fd[0]);
			if (curr_pipex_s->prev && curr_pipex_s->prev->pid > 0)
				kill(curr_pipex_s->prev->pid, SIGPIPE);
		}
		if (curr_pipex_s->pipe_fd[1] > 2)
			ft_close(curr_pipex_s->pipe_fd[1]);
		if (WIFEXITED(status))
			curr_pipex_s->status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			curr_pipex_s->status = 128 + WTERMSIG(status);
	}
}

int	process_child_pipes(t_pipex *pipex_s)
{
	t_pipex	*curr_pipex_s;
	int 	rep;

	while (pipex_s->next)
		pipex_s = pipex_s->next;
	curr_pipex_s = pipex_s;
	rep = 1;
	while (rep)
	{
		rep = 0;
		while (curr_pipex_s != NULL)
		{
			if (curr_pipex_s->pid > 0)
			{
				rep = 1;
				process_child_pid(curr_pipex_s);
			}
			curr_pipex_s = curr_pipex_s->prev;
		}
		if (rep == 1)
			curr_pipex_s = pipex_s;
	}
	return (pipex_s->status);
}

int	ft_shell_pipex()
{
	int status;

	if (minishell()->silence_info == true)
		return (0);
	add_to_history();
	new_process_tokens(); //TODO test comment ft_process_tokens_s and ft_exe_pipex_s
	status = process_child_pipes(minishell()->pipex); //manage_pid
	minishell()->status = status;
	return (0);
}

