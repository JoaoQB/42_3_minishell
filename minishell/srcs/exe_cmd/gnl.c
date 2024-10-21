/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:00:56 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/21 17:05:31 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strchr_index(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == '\0')
		return (-1);
	return (i);
}

char	*ft_clearbuffer(void *str, int size)
{
	char	*char_str;

	char_str = (char *) str;
	while (size > 0)
		char_str[--size] = (char) '\0';
	return (str);
}

char	*ft_memshift(void *str, int n_shift)
{
	int		i;
	char	*shifted_str;

	i = 0;
	if (n_shift == 0)
		return (str);
	shifted_str = (char *) str;
	while (shifted_str[i + n_shift])
	{
		shifted_str[i] = shifted_str[n_shift + i];
		i++;
	}
	while (i != (BUFFERSIZE + 1))
		shifted_str[i++] = '\0';
	return (shifted_str);
}

char	*ft_process_buffer(int fd, char *new_str, char *buffer)
{
	int	count;
	int	newline_pos;

	count = 1;
	while (count > 0 || buffer[0] != '\0')
	{
		newline_pos = ft_strchr_index(buffer, '\n');
		if (newline_pos >= 0 && buffer[0] != '\0')
		{
			new_str = ft_strnjoin(new_str, buffer, newline_pos + 1);
			buffer = ft_memshift(buffer, newline_pos + 1);
			break ;
		}
		new_str = ft_strnjoin(new_str, buffer, BUFFERSIZE);
		buffer = ft_clearbuffer(buffer, BUFFERSIZE + 1);
		count = read (fd, buffer, BUFFERSIZE);
	}
	if (count == -1 || (count == 0 && !new_str))
	{
		ft_clearbuffer(buffer, BUFFERSIZE + 1);
		if (new_str)
			free(new_str);
		return (NULL);
	}
	return (new_str);
}

char	*get_next_line(int fd)
{
	char		*new_str;
	static char	buffer[BUFFERSIZE + 1];

	if (fd < 0 || fd > FOPEN_MAX || read(fd, 0, 0) < 0)
	{
		ft_clearbuffer(buffer, BUFFERSIZE + 1);
		return (NULL);
	}
	new_str = NULL;
	new_str = ft_process_buffer(fd, new_str, buffer);
	return (new_str);
}
