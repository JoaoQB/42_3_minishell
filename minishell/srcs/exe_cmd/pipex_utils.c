/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:43:44 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/09 17:02:28 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		ft_close(int fd)
{
	if (fd > 2)
	{
		if (close(fd) == -1)
		{
			fd = -1;
			return (-1);
		}
		fd = -1;
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

char* get_file_name_from_fd(int fd) 
{//TODO delete
    static char filename[1024];
    char path[64];

    snprintf(path, sizeof(path), "/proc/self/fd/%d", fd);
    ssize_t len = readlink(path, filename, sizeof(filename) - 1);
    if (len != -1) {
        filename[len] = '\0';
    } else {
        strcpy(filename, "unknown");
    }
    return filename;
}

void	print_struct()
{//TODO delete
	t_pipex *pipex_s;
	int	i;
	int j;

	j = 0;
	pipex_s = minishell()->pipex;
	printf("\n\n --> Printing pipe_struct an cmds<--\n\n");
	while (pipex_s)
	{
		printf("   {pipe %d}\n", j++);
		printf("pid %d with status %d\n", pipex_s->pid, pipex_s->status);
		printf("inout_fd are: %d / %d\n", pipex_s -> pipe_fd[0], pipex_s -> pipe_fd[1]);
		printf(" -input name: %s\n", get_file_name_from_fd(pipex_s -> pipe_fd[0]));
		printf(" -output name: %s\n", get_file_name_from_fd(pipex_s -> pipe_fd[1]));
		printf("will execute:\n  --> ");
		i = -1;
		while (pipex_s->cmd[++i])
			printf("%s ", pipex_s->cmd[i]);
		if (pipex_s->cmd[i] == NULL)
			printf("%s", pipex_s->cmd[i]);
		printf("\n\n");
		pipex_s = pipex_s->next;
	}
	return ;
}

void	print_check_processes(t_pipex *pipex_s)
{//TODO delete
	// if (pipex_s == NULL)
	// 	return ;
	while (pipex_s->next)
	{
		if (pipex_s->pid > 0)
		{
			printf("\n-->process with pid: %d from cmd: %s unclosed\n", pipex_s->pid, pipex_s->cmd[0]);
			return ;
		}
		pipex_s = pipex_s->next;
	}
	printf("\n-->all processes have been closed\n");
}

// void *handle_error(char *err_print)

void *safe_malloc(size_t size) 
{
    void *ptr;

	ptr = (void *) malloc(size);
    if (ptr == NULL) 
	{
        perror("safe_malloc"); //the actuall error handlefunction.
        free_main_input();
		cleanup_main();
		exit(1); // General error
    }
    return ptr;
}

void process_err()
{//TODO chat_gpt generated
	//i wanto to handle errnos and convert then into the errors shell gives

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
	// else
	// 	ft_putstr_fd("Error\n", 2);
}

void hdl_err(int err, char *format, ...) 
{
    va_list args;

    va_start(args, format);
    while(*format) 
	{
		if(*format == '%') 
		{
			format++;
			if(*format == 's') 
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
