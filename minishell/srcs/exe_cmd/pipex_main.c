/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:44:00 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/20 12:58:11 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_all_fd(t_pipex *pipex_s)
{
	t_pipex	*save;

	save = NULL;
	if (!pipex_s)
		pipex_s = minishell()->pipex;
	while (pipex_s->prev)
		pipex_s = pipex_s->prev;
	while (pipex_s)
	{
		if (pipex_s != save)
		{
			ft_close(&pipex_s->pipe_fd[0]);
			ft_close(&pipex_s->pipe_fd[1]);
		}
		pipex_s = pipex_s->next;
	}
}

void	free_pipex_s(void)
{
	t_pipex	*temp;

	close_all_fd(minishell()->pipex);
	while (minishell()->pipex)
	{
		temp = minishell()->pipex;
		minishell()->pipex = temp->next;
		free_double_array(temp->cmd);
		free(temp->path);
		free(temp);
	}
}

void	process_child_pid(t_pipex *curr_pipex_s)
{
	int		status;

	status = 0;
	if (curr_pipex_s->pid < 1)
		return;
	if (waitpid(curr_pipex_s->pid, &status, WNOHANG))
	{
		ft_close(&curr_pipex_s->pipe_fd[0]);
		ft_close(&curr_pipex_s->pipe_fd[1]);
		// if (curr_pipex_s->prev && curr_pipex_s->prev->pid > 0)
			//kill(curr_pipex_s->prev->pid, SIGPIPE);
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

int get_final_status(void)
{
	t_pipex *pipex_s;

	if (minishell()->status)
		return (minishell()-> status);
	pipex_s = minishell()->pipex;
	while (pipex_s->next)
		pipex_s = pipex_s->next;
	return (pipex_s->status);
}

void	create_error_fd()
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

void	read_error_fd()
{
	int			ret;
	static char	buffer[10];

	ft_close(&minishell()->err_fd[1]);
	//use read until EOF do NOT use get_next_line
	ret = 1;
	while (ret)
	{
		ret = read(minishell()->err_fd[0], buffer, 10);
		if (ret > 0)
			write(2, buffer, ret);
	}
	if (ret == -1)
		perror("read error"); //TODO handle read write errors?
	ret = -1;
	while (ret++ < 10)
		buffer[ret] = 0;
	ft_close(&minishell()->err_fd[0]);	
}

int	ft_shell_pipex()
{
	if (minishell()->silence_info == true)
		return (0);
	create_error_fd();
	add_to_history();
	new_process_tokens();
	if (minishell()->pipex)
		process_child_pipes(minishell()->pipex);
	minishell()->status = get_final_status();
	read_error_fd();
	return (0);
}

