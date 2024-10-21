/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:37:46 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/21 13:15:11 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// symbolic link?
/*	Check if file exists
	Check if is regular file
	Check if not regular file
	Check if is executable */
int	file_access(char *file_path)
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

char *get_cmd_path(t_pipex *pipex_s)
{
    char *temp;
    char *paths;
    char *path_dir;
    int i;

    // Try the command directly first (absolute or relative path)
    temp = ft_strdup(pipex_s->cmd[0]);
    if (file_access(temp) == 0)
        return (temp); // Command found at given path

    // Check if it's a permission error for the direct command
    if (file_access(temp) == EACCES)
        pipex_s->status = EACCES;

    ft_free(&temp);

    // Get the $PATH environment variable
    paths = ft_getenv("PATH");
	if (paths)
	{
		while (*paths)
		{
			i = 0;
			// Find next ':' in $PATH
			while (paths[i] && paths[i] != ':')
				i++;

			// Extract the directory from $PATH
			path_dir = extract_before_i(paths, i);
			temp = ft_strnjoin(ft_strnjoin(path_dir, "/", 1), pipex_s->cmd[0], -1);

			// Check if the command exists in this directory
			if (file_access(temp) == 0)
				return (temp); // Command found

			// Handle permission error case
			if (file_access(temp) == EACCES)
				pipex_s->status = EACCES;

			ft_free(&temp);

			// Move to the next directory in $PATH
			if (paths[i] == ':')
				i++;
			paths += i;
		}
	}

    // No command found, return appropriate error
    if (pipex_s->status != EACCES)
        pipex_s->status = ENOENT;

    return (NULL);
}

// char	*get_cmd_path(t_pipex *pipex_s)
// {
// 	char	*temp;
// 	char	*paths;
// 	int		i;

// 	paths = ft_getenv("PATH");
// 	temp = ft_strnjoin(NULL, pipex_s->cmd[0], -1);
// 	while (1)
// 	{
// 		if (access(temp, F_OK | R_OK | X_OK) == 0)
// 			return (temp);
// 		if (file_acess(temp) == EACCES)
// 			pipex_s->status = EACCES;
// 		free(temp);
// 		i = 0;
// 		while (paths && paths[i] && paths[i] != ':')
// 			i++;
// 		temp = ft_strnjoin(ft_strnjoin(NULL, paths, i++), "/", 1);
// 		temp = ft_strnjoin(temp, pipex_s->cmd[0], -1);
// 		if (!paths || !paths[i - 1])
// 			break ;
// 		paths += i;
// 	}
// 	if (pipex_s->status != EACCES)
// 		pipex_s->status = ENOENT;
// 	return (free(temp), NULL);
// }

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

int	ft_n_update_path(t_pipex *pipex_s)
{
	char	*path;
	int		status;

	if (pipex_s->status || !pipex_s->cmd)
		return (pipex_s->status);
	path = pipex_s->cmd[0];
	if (path[0] == '.' && !path[1])
	{
		print_err("%s: filename argument required\n", path);
		print_err("usage: %s filename [arguments]\n", path);
		return (2);
	}
	status = is_directory(path);
	if (status != 0)
		return (status);
	pipex_s->path = get_cmd_path(pipex_s);
	if (pipex_s->path == NULL && pipex_s->status == EACCES)
		return (print_err ("%s: %s\n", path, strerror(pipex_s->status)), 126);
	if (pipex_s->path == NULL && pipex_s->status == ENOENT)
		return (print_err ("%s: command not found\n", path), 127);
	return (0);
}

// int	ft_n_update_path(t_pipex *pipex_s)
// {
// 	char	*path;
// 	// int		status;

// 	if (pipex_s->status || !pipex_s->cmd)
// 		return (pipex_s->status);
// 	path = pipex_s->cmd[0];
// 	if (path[0] == '.' && !path[1])
// 	{
// 		print_err("%s: filename argument required\n", path);
// 		print_err("usage: %s filename [arguments]\n", path);
// 		return (2);
// 	}
// 	// status = is_directory(path);
// 	// if (status != 0)
// 	// 	return (status);
// 	pipex_s->path = get_cmd_path(pipex_s);
// 	if (pipex_s->path == NULL && pipex_s->status == ENOENT)
// 		return (print_err ("%s: command not found\n", path), 127);
// 	if (pipex_s->path == NULL && pipex_s->status == EACCES)
// 		return (print_err ("%s: %s\n", path, strerror(pipex_s->status)), 126);
// 	return (0);
// }
