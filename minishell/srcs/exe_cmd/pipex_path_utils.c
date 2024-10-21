/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_path_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 15:43:13 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/21 15:43:38 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	file_access(char *file_path)
{
	struct stat	buffer;

	if (stat(file_path, &buffer) != 0)
		return (errno);
	if (lstat(file_path, &buffer) != 0)
		return (errno);
	if (!S_ISREG(buffer.st_mode))
		return (ENOENT);
	return (0);
}

int	ft_is_path(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[0] == '/')
		return (1);
	if (str[0] == '.' && str[1] == '/')
		return (1);
	if (str[0] == '.' && str[1] == '.' && str[2] == '/')
		return (1);
	while (str[i])
	{
		if (str[i] == '/')
			return (1);
		i++;
	}
	return (0);
}
