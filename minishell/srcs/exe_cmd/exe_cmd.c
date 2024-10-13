/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/13 18:17:45 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	check_for_pipeline(void)
// {
// 	t_token	*tokens_s;
// 	int		check;

// 	check = 0;
// 	tokens_s = minishell()->tokens;
// 	while (tokens_s)
// 	{
// 		if (tokens_s->type == PIPE)
// 			return (1);
// 		else if (tokens_s->type == PATH)
// 		{
// 			check = 1;
// 			if (!special_edge_cases(minishell()->pipex))
// 				return (1);
// 		}
// 		tokens_s = tokens_s->next;
// 	}
// 	if (!check && !special_edge_cases(minishell()->pipex))
// 		return (1);
// 	return (0);
// }

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
    struct stat buffer;

    if (stat(path, &buffer) != 0)
        return (0);
    else if (S_ISDIR(buffer.st_mode))
        return (EISDIR);
    return (0);
}

// bool is_directory(t_pipex *pipex_s)
// {
// 	char	*path;
//     struct stat buffer;

// 	path = pipex_s->cmd[0];
//     if (stat(path, &buffer) != 0)
//         return false;
//     if (S_ISDIR(buffer.st_mode))
// 	{
// 		if (path[0] == '.' && !path[1])
// 		{
// 			printf("%s: filename argument required\n", path); //TODO Handle error s
// 			printf("usage: %s filename [arguments]\n", path); //TODO Handle error s
// 			pipex_s->status = 2;
// 		}
// 		else
// 		{
// 			printf("%s: %s\n", path, strerror(EISDIR)); //TODO Handle error s
// 			pipex_s->status = 126;
// 		}
// 		return (true);
// 	}
// 	return (false);
// }


// bool	is_directory(t_pipex *pipex_s)
// {
// 	char		*path;
// 	struct stat	buffer;

// 	path = pipex_s->cmd[0];
// 	if (stat(path, &buffer) != 0)
// 		return (false);
// 	if (S_ISDIR(buffer.st_mode))
// 	{
// 		if (path[0] == '.' && !path[1])
// 		{
// 			print_err("%s: filename argument required\n", path);
// 			print_err("usage: %s filename [arguments]\n", path);
// 			// printf("%s: filename argument required\n", path);
// 			// printf("usage: %s filename [arguments]\n", path);
// 			pipex_s->status = 2;
// 		}
// 		else
// 		{
// 			print_err("%sdada: %s\n", path, strerror(EISDIR));
// 			// printf("%s: %s\n", path, strerror(EISDIR));
// 			pipex_s->status = 126;
// 		}
// 		return (true);
// 	}
// 	return (false);
// }

//join function with execute_command
void	ft_exe_pipex_s(void)
{
	t_pipex	*pipex_s;

	pipex_s = minishell()->pipex;
	if (!check_for_pipeline()) //handle no pipeline //TODO exit
		return ; //this worked but i did simplier
	while (pipex_s)
	{
		pipex_s->pid = fork();
		if (pipex_s->pid == -1)
			return (perror("fork failed")); //TODO Handle error s
		else if (pipex_s->pid == 0 && !special_edge_cases(pipex_s))
			exe_cmd_child(pipex_s, minishell()->menv);
		// else <parent> change stuff here latter
		//	exe_cmd_parent()
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
	if (pipex_s->status || special_edge_cases(pipex_s) || edge_cases(pipex_s))
		ft_exit_pid(pipex_s);
	pipex_s->status = ft_n_update_path(pipex_s);
	if (pipex_s->status != 0 || !pipex_s->cmd || !*pipex_s->cmd)
		ft_exit_pid(pipex_s);
	else if (execve(pipex_s->path, pipex_s->cmd, envp) == -1)
	{
		pipex_s->status = errno;
		print_err("excve error reads : %s\n", strerror(pipex_s->status));
	}
	ft_exit_pid(pipex_s);
}

// //TODO Handle error s
// void	exe_cmd_child(t_pipex *pipex_s, char **envp)
// {
// 	// if (!is_directory(pipex_s) && pipex_s->cmd && pipex_s->cmd[0])
// 	// {//todo better is dir
// 	// 	pipex_s->path = get_cmd_path(pipex_s);
// 	// 	if (pipex_s->status == 126)
// 	// 		print_err("%s", strerror(EACCES));
// 	// 	else if (!pipex_s->path && pipex_s->status == 0)
// 	// 	{
// 	// 		print_err("%s: command not found\n");
// 	// 		pipex_s->status = 127;
// 	// 	}
// 	// }
// 	if (pipex_s->pipe_fd[0] != STDIN_FILENO)
// 		dup2(pipex_s->pipe_fd[0], STDIN_FILENO);
// 	if (pipex_s->pipe_fd[1] != STDOUT_FILENO)
// 		dup2(pipex_s->pipe_fd[1], STDOUT_FILENO);
// 	close_all_fd(pipex_s);
// 	if (special_edge_cases(pipex_s) || edge_cases(pipex_s))
// 		ft_exit_pid(pipex_s);
// 	if (pipex_s->status != 0 || minishell()->status != 0)
// 		ft_exit_pid(pipex_s);
// 	else if (pipex_s->path && execve(pipex_s->path, pipex_s->cmd, envp) == -1)
// 		pipex_s->status = errno;
// 	ft_exit_pid(pipex_s);
// }

int		file_acess(char *file_path)
{
		if (access(file_path, F_OK) != 0)
			return (errno); //unixistent file 13 ENOENT 
		if (access(file_path, R_OK | X_OK) != 0)
			return (errno); //no permitions 2 EACCES
		else
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
		if (file_acess (temp) == EACCES)
			pipex_s->status = EACCES;
		if(access(temp, F_OK | R_OK | X_OK) == 0)
			return (temp);
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
// char	*get_cmd_path(t_pipex *pipex_s)
// {
// 	char	*temp;
// 	char	*paths;
// 	int		i;

// 	paths = ft_getenv("PATH");
// 	temp = ft_strnjoin(NULL, pipex_s->cmd[0], -1);
// 	while (paths && *paths != '\0')
// 	{
// 		if (access(temp, F_OK) == 0)
// 		{//TODO access function
// 			if (access(temp, R_OK | X_OK) == 0)
// 				return (temp);
// 			pipex_s->status = 126;
// 		}
// 		free(temp);
// 		i = 0;
// 		while (paths[i] && paths[i] != ':')
// 			i++;
// 		temp = ft_strnjoin(ft_strnjoin(NULL, paths, i++), "/", 1);
// 		temp = ft_strnjoin(temp, pipex_s->cmd[0], -1);
// 		if (!paths[i - 1])
// 			break ;
// 		paths += i;
// 	}
// 	return (free(temp), NULL);
// }
