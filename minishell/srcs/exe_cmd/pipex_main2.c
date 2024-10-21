/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 15:09:00 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/21 15:53:30 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*find_next_pipe(t_token *tokens_s)
{
	while (tokens_s && tokens_s->type != PIPE)
		tokens_s = tokens_s->next;
	if (tokens_s && tokens_s->type == PIPE)
		tokens_s = tokens_s->next;
	return (tokens_s);
}

void	close_all_fd(t_pipex *pipex_s)
{
	t_pipex	*save;

	save = NULL;
	if (!pipex_s)
		pipex_s = minishell()->pipex;
	while (pipex_s && pipex_s->prev)
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
