/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by fandre-b          #+#    #+#             */
/*   Updated: 2024/09/19 05:01:51 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_for_pipeline(t_main *main_s)
{
	t_token *tokens_s;
	int		check;

	check = 0;
	tokens_s = main_s->tokens;
	while (tokens_s)
	{
		if (tokens_s->type == PIPE)
			return (1);
		else if (tokens_s->type == PATH)
		{
			check = 1;
			if (!special_edge_cases(main_s->pipex))
				return (1);
		}
		tokens_s = tokens_s->next;
	}
	if (!check && !special_edge_cases(main_s->pipex))
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
			printf("%s: Is a directory\n", path); //TODO Handle error s
			pipex_s->status = 126;
		}
		return (true);
	}
	return (false);
}

void ft_exe_pipex_s(t_main *main_s, char **envp)
{//join function with execute_command
	t_pipex *pipex_s;

	pipex_s = main_s->pipex;
	if (!check_for_pipeline(main_s))
		return ; //this worked but i did simplier
	// if (!pipex_s->next && special_edge_cases(pipex_s))
	// 	return ;
	while (pipex_s)
	{
		pipex_s->path = get_cmd_path(pipex_s);
		execute_command(pipex_s, envp);
		pipex_s = pipex_s->next;
	}
}

int	execute_command(t_pipex *pipex_s, char **envp) //temp
{
	// if (pipex_s->status != 0)
	//  	return (-1);
	pipex_s->pid = fork();
	if (pipex_s->pid == -1)
		return (perror("fork failed"), errno); //TODO Handle error s
	else if (pipex_s->pid == 0)
		exe_cmd_child(pipex_s, envp);
	// else <parent> change stuff here latter
	//		exe_cmd_parent()
	return (0);
}

void	exe_cmd_child(t_pipex *pipex_s, char **envp)
{
	if (pipex_s->status != 0)
		ft_exit_pid(pipex_s); // safe guard
	if (pipex_s->pipe_fd[0] != STDIN_FILENO)
		dup2(pipex_s->pipe_fd[0], STDIN_FILENO);
	if (pipex_s->pipe_fd[1] != STDOUT_FILENO)
		dup2(pipex_s->pipe_fd[1], STDOUT_FILENO);
	close_all_fd(pipex_s);
	if (special_edge_cases(pipex_s) || edge_cases(pipex_s))
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

	paths = ft_getenv(pipex_s->main_s, "PATH");
	temp = ft_strnjoin(ft_strdup("./"), pipex_s->cmd[0], -1);
	while (!is_directory(pipex_s) && paths && *paths != '\0')
	{
        if (access(temp, F_OK) == 0) 
		{
            if (access(temp, R_OK | X_OK) == 0) 
                return (temp);
            pipex_s->status = 126;
        }
		free(temp);
		i = 0;
		while (paths[i] && paths[i] != ':')
			i++;
		temp = ft_strnjoin(ft_strnjoin(NULL, paths, i++), "/", 1);
		temp = ft_strnjoin(temp, pipex_s->cmd[0], -1);
		paths += i;
	}
	if (pipex_s->status == 0)
		pipex_s->status = 127;
	return (free(temp), NULL);
}

// char	*get_cmd_path(char *cmd, char **envp)
// {
// 	char	*cmd;
// 	char	*temp;
// 	char	*paths;
// 	int		i;

// 	temp = NULL;
// 	paths = *envp;
// 	paths = getenv("PATH");
// 	temp = ft_strnjoin(ft_strnjoin(NULL, ".", 1), "/", 1);
// 	temp = ft_strnjoin(temp, cmd, -1);
// 	while (paths && *paths != '\0')
// 	{
// 		if (access(temp, R_OK | X_OK) == 0)
// 			return (temp);
// 		free(temp);
// 		i = 0;
// 		while (paths[i] && paths[i] != ':')
// 			i++;
// 		temp = ft_strnjoin(ft_strnjoin(NULL, paths, i++), "/", 1);
// 		temp = ft_strnjoin(temp, cmd, -1);
// 		paths += i;
// 	}
// 	return (free(temp), NULL);
// }

