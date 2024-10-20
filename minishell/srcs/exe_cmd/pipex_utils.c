/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:43:44 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/20 15:37:18 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_close(int *fd)
{
	if (*fd > 2)
	{
		if (close(*fd) == -1)
		{
			*fd = -1;
			return (-1);
		}
		*fd = -1;
	}
	return (0);
}

char	*ft_strnjoin(char *old_str, char *str_add, int size)
{
	int		len;
	int		i;
	char	*new_str;

	if (!str_add || !str_add[0] || size == 0)
		return (old_str);
	len = 0;
	while (old_str && old_str[len])
		len++;
	i = 0;
	while (str_add && str_add[i])
		i++;
	if (i < size || size == -1)
		size = i;
	new_str = (char *) safe_malloc(size + len + 1);
	i = -1;
	while (old_str && ++i < len)
		new_str[i] = old_str[i];
	i = -1;
	while (str_add && ++i < size)
		new_str[len + i] = str_add[i];
	new_str[len + i] = '\0';
	return (free(old_str), new_str);
}

void	critical_error(char *err_print)
{
	perror(err_print);
	ft_exit(1);
}

void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = (void *) malloc(size);
	if (ptr == NULL)
		critical_error("malloc failed");
	return (ptr);
}

void	print_err(char *format, ...)
{
	va_list	args;
	int		fd_err;

	fd_err = minishell()->err_fd[1];
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (*format == 's')
				ft_putstr_fd(va_arg(args, char *), fd_err);
		}
		else
			write(fd_err, format, 1);
		format++;
	}
	va_end(args);
}
