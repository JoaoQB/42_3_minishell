/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/22 12:43:13 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_child_pid(t_pipex *curr_pipex_s)
{
	int		status;

	status = 0;
	if (curr_pipex_s->pid < 1)
		return ;
	if (waitpid(curr_pipex_s->pid, &status, WNOHANG))
	{
		ft_close(&curr_pipex_s->pipe_fd[0]);
		ft_close(&curr_pipex_s->pipe_fd[1]);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			status = 128 + WTERMSIG(status);
		curr_pipex_s->pid = -1;
	}
	if (status > 0)
		curr_pipex_s->status = status;
}

int	process_child_pipes(t_pipex *pipex_s)
{
	t_pipex	*curr_pipex_s;
	int		rep;

	set_signals(SIGCMD);
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
	close_all_fd(NULL);
}

void	read_error_fd(void)
{
	int			ret;
	static char	buffer[10];

	ft_close(&minishell()->err_fd[1]);
	ret = 1;
	while (ret && minishell()->status != 130)
	{
		ret = read(minishell()->err_fd[0], buffer, 10);
		if (ret > 0)
			write(2, buffer, ret);
	}
	if (ret == -1)
		perror("read error");
	ret = -1;
	while (ret++ < 10)
		buffer[ret] = 0;
	ft_close(&minishell()->err_fd[0]);
}

void	create_error_fd(void)
{
	int	err_fd[2];

	if (pipe(err_fd) == -1)
	{
		print_err("%s\n", strerror(errno));
		minishell()->status = 1;
		return ;
	}
	minishell()->err_fd[0] = err_fd[0];
	minishell()->err_fd[1] = err_fd[1];
}
