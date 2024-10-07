/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:43:44 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/07 17:27:49 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	if (!new_str)
		return (NULL);
	i = -1;
	while (old_str && ++i < len)
		new_str[i] = old_str[i];
	i = -1;
	while (str_add && ++i < size)
		new_str[len + i] = str_add[i];
	new_str[len + i] = '\0';
	return (free(old_str), new_str);
}

char	*ft_strstr(const char *big, const char *little)
{
	size_t	i;
	size_t	j;

	if (little[0] == '\0')
		return ((char *)(big));
	i = 0;
	while (big[i])
	{
		j = 0;
		while (big[i + j] == little[j] && little[j])
			j++;
		if (little[j] == '\0')
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}

// void *handle_error(char *err_print)

void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = (void *) malloc(size);
	if (ptr == NULL)
	{
		perror("safe_malloc");
		free_main_input();
		cleanup_main();
		exit(1);
	}
	return (ptr);
}

//TODO chat_gpt generated
//i wanto to handle errnos and convert then into the errors shell gives
void	process_err(void)
{
	if (errno == 0)
		return ;
	if (errno == 2)
		ft_putstr_fd("No such file or directory\n", 2);
	else if (errno == 13)
		ft_putstr_fd("Permission denied\n", 2);
	else if (errno == 21)
		ft_putstr_fd("Is a directory\n", 2);
	else if (errno == 126)
		ft_putstr_fd("Permission denied\n", 2);
	else if (errno == 127)
		ft_putstr_fd("Command not found\n", 2);
	else if (errno == 128)
		ft_putstr_fd("Invalid argument\n", 2);
	else if (errno == 130)
		ft_putstr_fd("Terminated by Ctrl+C\n", 2);
	else if (errno == 134)
		ft_putstr_fd("Abort trap\n", 2);
	else if (errno == 139)
		ft_putstr_fd("Segmentation fault\n", 2);
	else if (errno == 141)
		ft_putstr_fd("Broken pipe\n", 2);
	else if (errno == 255)
		ft_putstr_fd("Exit status out of range\n", 2);
	else
		ft_putstr_fd(strerror(errno), 2);
}

void	hdl_err(int err, char *format, ...)
{
	va_list	args;

	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (*format == 's')
				ft_putstr_fd(va_arg(args, char *), 2);
		}
		else
			write(2, format, 1);
		format++;
	}
	va_end(args);
	minishell()->status = err;
	process_err();
}
