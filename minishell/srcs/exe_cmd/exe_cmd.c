/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jk <jk@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/03 10:51:05 by jk               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_for_pipeline()
{
	t_token *tokens_s;
	int		check;

	check = 0;
	tokens_s = minishell()->tokens;
	while (tokens_s)
	{
		if (tokens_s->type == PIPE)
			return (1);
		else if (tokens_s->type == PATH)
		{
			check = 1;
			if (!special_edge_cases(minishell()->pipex))
				return (1);
		}
		tokens_s = tokens_s->next;
	}
	if (!check && !special_edge_cases(minishell()->pipex))
		return (1);
	return (0);
}

bool is_directory(t_pipex *pipex_s)
{
	char	*path;
    struct stat buffer;

	path = pipex_s->cmd[0];
    if (stat(path, &buffer) != 0)
        return false;
    if (S_ISDIR(buffer.st_mode))
	{
		if (path[0] == '.' && !path[1])
		{
			printf("%s: filename argument required\n", path); //TODO Handle error s
			printf("usage: %s filename [arguments]\n", path); //TODO Handle error s
			pipex_s->status = 2;
		}
		else
		{
			printf("%s: %s\n", path, strerror(EISDIR)); //TODO Handle error s
			pipex_s->status = 126;
		}
		return (true);
	}
	return (false);
}

void ft_exe_pipex_s()
{//join function with execute_command
	t_pipex *pipex_s;

	pipex_s = minishell()->pipex;
	if (!check_for_pipeline()) //handle no pipeline
		return ; //this worked but i did simplier
	while (pipex_s)
	{
		if (pipex_s->status == 0)
		{
			pipex_s->pid = fork();
			if (pipex_s->pid == -1)
				return (perror("fork failed")); //TODO Handle error s
			else if (pipex_s->pid == 0)
			{
				ft_update_fds(pipex_s->token, pipex_s); //TODO TEST
				if (pipex_s->prev == NULL)
					pipex_s->pipe_fd[0] = STDIN_FILENO;
				if (pipex_s->next == NULL)
					pipex_s->pipe_fd[0] = STDOUT_FILENO;
				exe_cmd_child(pipex_s, minishell()->menv);
			}
			// else <parent> change stuff here latter
			//	exe_cmd_parent()
		}
		pipex_s = pipex_s->next;
	}
}

void	exe_cmd_child(t_pipex *pipex_s, char **envp)
{
	if (pipex_s->pipe_fd[0] != STDIN_FILENO)
		dup2(pipex_s->pipe_fd[0], STDIN_FILENO);
	if (pipex_s->pipe_fd[1] != STDOUT_FILENO)
		dup2(pipex_s->pipe_fd[1], STDOUT_FILENO);
	close_all_fd(pipex_s);
	if (special_edge_cases(pipex_s) || edge_cases(pipex_s))
		ft_exit_pid(pipex_s);
	else if (!is_directory(pipex_s))
	{//this will be deprecated
		pipex_s->path = get_cmd_path(pipex_s); //TODO Handle error s
		if (pipex_s->status == 126)
			printf("%s: %s\n", pipex_s->cmd[0], strerror(EACCES));
		else if (!pipex_s->path && pipex_s->status == 0)
		{
			ft_putstr_fd(pipex_s->cmd[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			pipex_s->status = 127;
		}
	}
	if (pipex_s->status != 0 || !pipex_s->cmd || !*pipex_s->cmd)
		ft_exit_pid(pipex_s);
	else if (execve(pipex_s->path, pipex_s->cmd, envp) == -1)
			pipex_s->status = errno;
	ft_exit_pid(pipex_s);
}

char	*get_cmd_path(t_pipex *pipex_s)
{
	char	*temp;
	char	*paths;
	int		i;

	paths = ft_getenv("PATH");
	temp = ft_strnjoin(ft_strdup("./"), pipex_s->cmd[0], -1);
	while (paths && *paths != '\0')
	{
        if (access(temp, F_OK) == 0)
		{
            if (access(temp, R_OK | X_OK) == 0)
                return (temp);
            pipex_s->status = 126; //TODO run handle that will do this and more
        }
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
	return (free(temp), NULL);
}

