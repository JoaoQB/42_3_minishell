/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by fandre-b              #+#    #+#             */
/*   Updated: 2024/08/27 11:12:31 by fandre-b         ###   ########.fr       */
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

void free_pipex_s(t_pipex **pipex_s)
{
	t_pipex *temp;

	close_all_fd((* pipex_s));
	while((* pipex_s))
	{
		free((* pipex_s)->path);
		(* pipex_s)->path = NULL;
		free_double_array((* pipex_s)->cmd);
		(* pipex_s)->cmd = NULL;
		temp = (* pipex_s);
		(* pipex_s) = (* pipex_s)->next;
		free(temp);
		temp = NULL;
	}
}

int	process_child_pipes(t_pipex *pipex_s)//TODO to many lines
{
	t_pipex	*curr_pipex_s;
	int 	rep;
	int		status;

	while(pipex_s->next)
		pipex_s = pipex_s->next;
	curr_pipex_s = pipex_s;
	rep = 1;
	while(rep)
	{
		rep = 0;
		while(curr_pipex_s != NULL)
		{
			if (curr_pipex_s->pid > 0)
			{
				rep = 1;
				if (waitpid(curr_pipex_s->pid, &status, WNOHANG))
				{
					curr_pipex_s->pid = -1;
					pipex_s->main_s->status = status;
					if (curr_pipex_s->pipe_fd[0] > 2)
					{
						close(curr_pipex_s->pipe_fd[0]);
						if (curr_pipex_s->prev && curr_pipex_s->prev->pid > 0)
							kill(curr_pipex_s->prev->pid, SIGPIPE);
					}
					if (curr_pipex_s->pipe_fd[1] > 2)
						close(curr_pipex_s->pipe_fd[1]);
					if (WIFEXITED(status))
						status = WEXITSTATUS(status);
				}
			}
			curr_pipex_s = curr_pipex_s->prev;
		}
		if (rep == 1)
			curr_pipex_s = pipex_s;
	}
	return (pipex_s->main_s->status);
}

int	ft_shell_pipex(t_main *main_s)
{
	int status;

	if (main_s->silence_info == true)
	{
		printf("silencer activated as %d\n", main_s->silence_info);
		return(0);
	}
	//my_print_env(main_s);
	ft_process_tokens_s(main_s);
	add_to_history(main_s);
	// print_struct(main_s);
	ft_exe_pipex_s(main_s, main_s->menv);
	status = process_child_pipes(main_s->pipex); //manage_pid
	printf("this is the process status: %d\n", status);
	//print_check_processes(main_s->pipex);
	// free_pipex_s(main_s->pipex); //temp free
	//recieve signal when i do exit, so i can properly free it and pass responsability
	return (status);
}

