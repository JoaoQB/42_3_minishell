/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juka <juka@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by juka              #+#    #+#             */
/*   Updated: 2024/08/24 18:14:04 by juka             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_all_fd(t_pipex *pipex_s)
{
	t_pipex *save;

	save = NULL;
	while (pipex_s->prev)
		pipex_s = pipex_s->prev;
	while(pipex_s->next)
	{
		if (pipex_s != save)
		{
			if (pipex_s->pipe_fd[0] > 2)
			close(pipex_s->pipe_fd[0]);
			if (pipex_s->pipe_fd[1] > 2)
			close(pipex_s->pipe_fd[1]);
		}
		pipex_s = pipex_s->next;
	}
}

void free_pipex_s(t_pipex *pipex_s)
{
	t_pipex *temp;

	close_all_fd(pipex_s);
	while(pipex_s)
	{
		free(pipex_s->path);
		free_double_array(pipex_s->cmd);
		temp = pipex_s;
		pipex_s = pipex_s->next;
		free(temp);
	}
}

void	process_child_pipes(t_pipex *pipex_s)
{
	t_pipex	*curr_pipex_s;
	int		status;

	while(pipex_s->next)
		pipex_s = pipex_s->next;
	curr_pipex_s = pipex_s;
	while(curr_pipex_s)
	{
		status = waitpid(curr_pipex_s->pid, &status, WNOHANG);
		if(status > 0)
		{
			pipex_s->status = WIFEXITED(status);
			close(pipex_s->pipe_fd[1]);
			close(pipex_s->pipe_fd[0]);
			curr_pipex_s = pipex_s;
		}
		else if(status == 0)
			curr_pipex_s = pipex_s;
		else
			curr_pipex_s = curr_pipex_s->prev;
		
	}
	if (curr_pipex_s == NULL)
		printf("all processes have been closed\n");
}

int	ft_shell_pipex(t_main *main_s)
{
	if (!main_s->tokens)
		return(0);
	ft_process_tokens_s(main_s);
	//print_struct(main_s);
	ft_exe_pipex_s(main_s->pipex, main_s->menv);
	//process_child_pipes(main_s->pipex); //manage_pid
	free_pipex_s(main_s->pipex); //temp free
	sleep(1);
	//recieve signal when i do exit, so i can properly free it and pass responsability
	return (0);
}
