/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/20 15:35:58 by fandre-b         ###   ########.fr       */
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

int	is_directory(const char *path)
{
	struct stat	buffer;

	if (stat(path, &buffer) != 0)
	{
		if ((path[0] == '.' || path[0] == '/'))
		{
			print_err("%s: %s\n", path, strerror(errno));
			return (127);
		}
		return (0);
	}
	else if (S_ISDIR(buffer.st_mode))
	{
		if ((path[0] == '.' || path[0] == '/'))
		{
			print_err("%s: %s\n", path, strerror(EISDIR));
			return (126);
		}
		else
		{
			print_err ("%s: command not found\n", path);
			return (127);
		}
	}
	return (0);
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

// symbolic link?
int	file_acess(char *file_path)
{
	struct stat	buffer;

	if (access(file_path, F_OK) != 0)
		return (errno);
	if (stat(file_path, &buffer) != 0)
		return (errno);
	if (!S_ISREG(buffer.st_mode))
		return (ENOENT);
	if (access(file_path, X_OK) != 0)
		return (errno);
	return (0);
}

char	*get_cmd_path(t_pipex *pipex_s)
{
	char	*temp;
	char	*paths;
	int		i;

	paths = ft_getenv("PATH");
	temp = ft_strnjoin(NULL, pipex_s->cmd[0], -1);
	while (1)
	{
		if (access(temp, F_OK | R_OK | X_OK) == 0)
			return (temp);
		if (file_acess(temp) == EACCES)
			pipex_s->status = EACCES;
		free(temp);
		i = 0;
		while (paths && paths[i] && paths[i] != ':')
			i++;
		temp = ft_strnjoin(ft_strnjoin(NULL, paths, i++), "/", 1);
		temp = ft_strnjoin(temp, pipex_s->cmd[0], -1);
		if (!paths || !paths[i - 1])
			break ;
		paths += i;
	}
	if (pipex_s->status != EACCES)
		pipex_s->status = ENOENT;
	return (free(temp), NULL);
}
