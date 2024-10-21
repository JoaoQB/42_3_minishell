/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:37:46 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/21 15:52:22 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_directory(char *path)
{
	struct stat	buffer;

	if (stat(path, &buffer) != 0)
	{
		if (ft_is_path(path))
		{
			print_err("%s: %s\n", path, strerror(errno));
			if (errno == ENOENT)
				return (127);
			else
				return (126);
		}
	}
	else if (S_ISDIR(buffer.st_mode) && ft_is_path(path))
	{
		print_err("%s: is a directory\n", path);
		return (126);
	}
	return (0);
}

int	check_cmd_path(t_pipex *pipex_s)
{
	int		status;
	char	*temp;

	if (!pipex_s || !pipex_s->path)
		return (-1);
	temp = pipex_s->path;
	status = is_directory(temp);
	if (status != 0)
		return (status);
	if (access(temp, F_OK) != 0 || !ft_is_path(temp))
	{
		print_err ("%s: command not found\n", temp);
		return (127);
	}
	if (access(temp, X_OK) != 0)
	{
		print_err ("%s: permission denied\n", temp);
		return (126);
	}
	return (0);
}

static char	*find_in_path(char *paths, t_pipex *pipex_s)
{
	char	*temp;
	char	*path_dir;
	int		i;

	if (!paths)
		return (NULL);
	while (*paths)
	{
		i = 0;
		while (paths[i] && paths[i] != ':')
			i++;
		path_dir = extract_before_i(paths, i);
		temp = ft_strnjoin(ft_strnjoin(path_dir, "/", 1), pipex_s->cmd[0], -1);
		if (file_access(temp) == 0)
			return (temp);
		if (file_access(temp) == EACCES)
			pipex_s->status = EACCES;
		ft_free(&temp);
		if (paths[i] == ':')
			i++;
		paths += i;
	}
	return (NULL);
}

char	*get_cmd_path(t_pipex *pipex_s)
{
	char	*temp;
	char	*paths;

	temp = ft_strdup(pipex_s->cmd[0]);
	if (file_access(temp) == 0)
		return (temp);
	if (file_access(temp) == EACCES)
		pipex_s->status = EACCES;
	ft_free(&temp);
	paths = ft_getenv("PATH");
	if (paths)
		temp = find_in_path(paths, pipex_s);
	if (!temp && pipex_s->status != EACCES)
		pipex_s->status = ENOENT;
	if (!temp)
		return (ft_strdup(pipex_s->cmd[0]));
	return (temp);
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
	pipex_s->path = get_cmd_path(pipex_s);
	status = check_cmd_path(pipex_s);
	if (status != 0)
		return (status);
	if (pipex_s->path == NULL && pipex_s->status == ENOENT)
		return (print_err ("%s: command not found\n", path), 127);
	if (pipex_s->path == NULL && pipex_s->status == EACCES)
		return (print_err ("%s: %s\n", path, strerror(pipex_s->status)), 126);
	return (0);
}
