/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by fandre-b          #+#    #+#             */
/*   Updated: 2024/09/18 10:57:46 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_for_pipeline(t_main *main_s)
{
	t_token *tokens_s;
	int		check;

	check = 0;
	tokens_s = main_s->tokens;
	while(tokens_s)
	{
		if (tokens_s->type == PIPE)
			return (1);
		else if(tokens_s->type == PATH)
		{
			check = 1;
			if (!special_edge_cases(main_s, main_s->pipex))
				return (1);
		}
		tokens_s = tokens_s->next;
	}
	if (!check && !special_edge_cases(main_s, main_s->pipex))
		return(1);
	return (0);
}

void ft_exe_pipex_s(t_main *main_s, char **envp)
{//join function with execute_command
	t_pipex *pipex_s;

	pipex_s = main_s->pipex;
	if (!check_for_pipeline(main_s))
		return; //this worked but i did simplier
	// if(!pipex_s->next && special_edge_cases(pipex_s))
	// 	return ;
	while(pipex_s)
	{
		pipex_s->path = get_cmd_path(pipex_s->cmd[0], envp);
		execute_command(main_s, pipex_s, envp);
		pipex_s = pipex_s->next;
	}
}

int	execute_command(t_main *main_s, t_pipex *pipex_s, char **envp) //temp
{
	// if (pipex_s->status != 0)
	//  	return(-1);
	pipex_s->pid = fork();
	if (pipex_s->pid == -1)
		return (perror("fork failed"), errno); //TODOfunction for errors
	else if (pipex_s->pid == 0)
		exe_cmd_child(main_s, pipex_s, envp);
	// else <parent> change stuff here latter
	//		exe_cmd_parent()
	return (0);
}

// void handle_sigpipe(int sig)
// {//TODO signal handling for exit to. Se bem que consigo sem signals...
//     printf("Received SIGPIPE(%d), exiting...\n", sig);
//     exit(1);
// }



void	exe_cmd_child(t_main *main_s, t_pipex *pipex_s, char **envp)
{
	if(pipex_s->status != 0)
		exit(pipex_s->status);
	if (pipex_s->pipe_fd[0] != STDIN_FILENO)
		dup2(pipex_s->pipe_fd[0], STDIN_FILENO);
	if (pipex_s->pipe_fd[1] != STDOUT_FILENO)
		dup2(pipex_s->pipe_fd[1], STDOUT_FILENO);
	close_all_fd(pipex_s);
	if (special_edge_cases(main_s, pipex_s) || edge_cases(pipex_s))
		exit(pipex_s->status);
	else if (!pipex_s->path && pipex_s->cmd[0])
		printf("%s: command not found\n", pipex_s->cmd[0]); //TODO err 127
	else if (execve(pipex_s->path, pipex_s->cmd, envp) == -1)
		pipex_s->status = errno;
	exit(pipex_s->status);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	char	*temp;
	char	*paths;
	int		i;

	paths = NULL;
	temp = NULL;
	if(!cmd)
		return(NULL);
	paths = *envp;
	paths = getenv("PATH");
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

