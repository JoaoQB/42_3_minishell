/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juka <juka@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by juka              #+#    #+#             */
/*   Updated: 2024/08/25 12:12:23 by juka             ###   ########.fr       */
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
	int 	rep;
	int		status;
	int		signal;
	int		result;
	int		times;

	while(pipex_s->next)
		pipex_s = pipex_s->next;
	curr_pipex_s = pipex_s;
	rep = 1;
	times = 0;
	printf("\n_________________________________________\n");
	printf("\n	..::handle processes START::..\n");
	while(rep && times < 5)
	{
		rep = 0;
		printf("\n-> starting from last pipex_s\n");
		while(curr_pipex_s != NULL)
		{
			printf("    //%s(%d)\n", curr_pipex_s->cmd[0], curr_pipex_s->pid);
			if (curr_pipex_s->pid > 0)
			{
				// printf("process %s(%d)\n", pipex_s->cmd[0], pipex_s->pid);
				result = waitpid(curr_pipex_s->pid, &status, WNOHANG);
				if (result)
				{
					rep = 1;
					curr_pipex_s->pid = -1;
					pipex_s->status = status;
					if (curr_pipex_s->pipe_fd[0] > 2)
					{//second if forces close of process by sending SIGKILL
						close(curr_pipex_s->pipe_fd[0]);
						if (curr_pipex_s->prev && curr_pipex_s->prev->pid > 0)
							kill(curr_pipex_s->prev->pid, SIGKILL);
					}
					if (curr_pipex_s->pipe_fd[1] > 2)
						close(curr_pipex_s->pipe_fd[1]);
					if (WIFEXITED(status))
					{
						status = WEXITSTATUS(status);
						printf("[✓] Exit status was: %d\n", status);
					}
					else if (WIFSIGNALED(status))
					{
						signal = WTERMSIG(status);
						printf("[✓] Child terminated by signal: %d\n", signal);
					}
					else
						printf("[✗] Child did not exit normally\n");
				}
				else
				{
					printf("[✗] process still running\n");
					rep = 1;
				}
			}
			else
					printf("[✓] closed\n");
			curr_pipex_s = curr_pipex_s->prev;
		}
		if (rep == 1)
		{
			curr_pipex_s = pipex_s;
			usleep(10000);
			times += 1;
		}
	}
	if (times == 5)
		printf("\n-->Somo processes are still running\n");
	if (curr_pipex_s == NULL)
		printf("\n-->all processes have been closed\n");
	printf("\n	..::handle processes END::..\n");
	printf("_________________________________________\n\n");
}

int	ft_shell_pipex(t_main *main_s)
{
	if (!main_s->tokens)
		return(0);
	ft_process_tokens_s(main_s);
	//print_struct(main_s);
	ft_exe_pipex_s(main_s->pipex, main_s->menv);
	sleep(1);
	process_child_pipes(main_s->pipex); //manage_pid
	free_pipex_s(main_s->pipex); //temp free
	//recieve signal when i do exit, so i can properly free it and pass responsability
	return (0);
}
