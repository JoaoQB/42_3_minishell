/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/15 18:45:32 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_for_pipeline(void)
{
	t_token	*tokens_s;

	tokens_s = minishell()->tokens;
	while (tokens_s)
	{
		if (tokens_s->type == PIPE)
			return (1);
		tokens_s = tokens_s->next;
	}
	if (special_edge_cases(minishell()->pipex))
		return (0);
	return(1);
}

//TODO Handle error s

int is_directory(const char *path)
{
	struct stat	buffer;

	if (stat(path, &buffer) != 0)
	{
		if ((path[0] == '.' || path[0] == '/'))
		{
			print_err("%s: %s\n", path, strerror(errno));
			return (127);
		}
		return(0);
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
			return(127);
		}
	}
	return (0);
}

void	exe_cmd_child(t_pipex *pipex_s, char **envp)
{
	if (pipex_s->pipe_fd[0] != STDIN_FILENO)
		dup2(pipex_s->pipe_fd[0], STDIN_FILENO);
	if (pipex_s->pipe_fd[1] != STDOUT_FILENO)
		dup2(pipex_s->pipe_fd[1], STDOUT_FILENO);
	close_all_fd(pipex_s);
	if (pipex_s->status || special_edge_cases(pipex_s) || edge_cases(pipex_s))
		ft_exit_pid(pipex_s);
	pipex_s->status = ft_n_update_path(pipex_s);
	if (pipex_s->status != 0 || !pipex_s->cmd || !*pipex_s->cmd)
		ft_exit_pid(pipex_s);
	else if (execve(pipex_s->path, pipex_s->cmd, envp) == -1)
	{
		// pipex_s->status = errno;
		print_err("excve error reads : %s\n", strerror(pipex_s->status));
	}
	ft_exit_pid(pipex_s);
}

int		file_acess(char *file_path)
{
		if (access(file_path, F_OK) != 0)
			return (errno); //unixistent file 13 ENOENT
		if (access(file_path, R_OK) != 0)
			return (errno); //no permitions 2 EACCES
		return (0);
}

char	*get_cmd_path(t_pipex *pipex_s)
{
	char	*temp;
	char	*paths;
	int		i;

	paths = ft_getenv("PATH");
	temp = ft_strnjoin(NULL, pipex_s->cmd[0], -1);
	while (paths && *paths != '\0')
	{
		if(access(temp, F_OK | R_OK | X_OK) == 0)
			return (temp);
		if (file_acess(temp) == EACCES)
			pipex_s->status = EACCES;
		free(temp);
		i = 0;
		while (paths[i] && paths[i] != ':')
			i++;
		temp = ft_strnjoin(ft_strnjoin(NULL, paths, i++), "/", 1);
		temp = ft_strnjoin(temp, pipex_s->cmd[0], -1);
		if (!paths[i - 1])
			break ;
		paths += i;
	}
	if (pipex_s->status != EACCES)
		pipex_s->status = ENOENT;
	return (free(temp), NULL);
}
