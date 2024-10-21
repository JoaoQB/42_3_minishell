/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/21 13:13:53 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_for_pipeline(void)
{
	t_pipex	*first_pipe;

	first_pipe = minishell()->pipex;
	if (first_pipe && !first_pipe->next && !find_next_pipe(first_pipe->token))
		if (first_pipe->status || special_edge_cases(first_pipe))
			return (0);
	return (1);
}

void	exe_cmd_child(t_pipex *pipex_s, char **envp)
{
	if (pipex_s->status || minishell()->status || !pipex_s->cmd)
		ft_exit_pid(pipex_s);
	if (pipex_s->pipe_fd[0] != STDIN_FILENO)
		dup2(pipex_s->pipe_fd[0], STDIN_FILENO);
	if (pipex_s->pipe_fd[1] != STDOUT_FILENO)
		dup2(pipex_s->pipe_fd[1], STDOUT_FILENO);
	dup2(minishell()->err_fd[1], STDERR_FILENO);
	close_all_fd(pipex_s);
	if (edge_cases(pipex_s) || special_edge_cases(pipex_s))
		ft_exit_pid(pipex_s);
	pipex_s->status = ft_n_update_path(pipex_s);
	if (pipex_s->status || !pipex_s->path)
		ft_exit_pid(pipex_s);
	else if (execve(pipex_s->path, pipex_s->cmd, envp) == -1)
		pipex_s->status = errno;
	ft_exit_pid(pipex_s);
}
