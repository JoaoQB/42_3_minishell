/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:48:17 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/16 09:42:23 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		else if(file_acess (temp) == 0)
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