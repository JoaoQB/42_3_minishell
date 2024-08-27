/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juka <juka@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by juka              #+#    #+#             */
/*   Updated: 2024/08/25 13:58:31 by juka             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


void ft_exe_pipex_s(t_pipex *pipex_s, char **envp)
{//sera usado para possivelmente chamar edge cases
	while(pipex_s)
	{
		pipex_s->path = get_cmd_path(pipex_s->cmd[0], envp);
		if (pipex_s->path)
			execute_command(pipex_s, envp);
		else
			printf("%s: command not found\n", pipex_s->cmd[0]);
		pipex_s = pipex_s->next;
	}
}

int	execute_command(t_pipex *pipex_s, char **envp) //temp
{
	int		status;

	status = 0;
	pipex_s->pid = fork();
	if (pipex_s->pid == -1)
		return (perror("fork failed"), errno);
	else if (pipex_s->pid == 0)
		exe_cmd_child(pipex_s, envp);
	// else change stuff here latter
	// 	exe_cmd_parent()
	return (status);
}

void handle_sigpipe(int sig)
{
    printf("Received SIGPIPE(%d), exiting...\n", sig);
    exit(1);
}

void	exe_cmd_child(t_pipex *pipex_s, char **envp)
{
	int	status;

    //signal(SIGPIPE, handle_sigpipe);
	status = 0;
	if (pipex_s->pipe_fd[0] != STDIN_FILENO)
	{
		dup2(pipex_s->pipe_fd[0], STDIN_FILENO);
		close(pipex_s->pipe_fd[0]);
	}
	if (pipex_s->pipe_fd[1] != STDOUT_FILENO)
	{
		dup2(pipex_s->pipe_fd[1], STDOUT_FILENO);
		close(pipex_s->pipe_fd[1]);
	}
	close_all_fd(pipex_s);
	if (execve(pipex_s->path, pipex_s->cmd, envp) == -1)
		status = errno;
	pipex_s->status = status;
	exit (status);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	char	*temp;
	char	*paths;
	int		i;

	paths = NULL;
	temp = NULL;
    while (*envp != NULL)
    {
        if (ft_strncmp(*envp, "PATH=", 5) == 0)
            paths = *envp + 5; // Skip "PATH="
		envp++;
    }
	while (paths && *paths != '\0')
	{
		i = 0;
		free (temp);
		while (paths[i] && paths[i] != ':')
			i++;
		temp = ft_strnjoin(ft_strnjoin(NULL, paths, i++), "/", 1);
		temp = ft_strnjoin(temp, cmd, -1);
		if (access(temp, R_OK | X_OK) == 0)
			return(temp);
		paths += i;
	}
	return (free (temp), NULL);
}

