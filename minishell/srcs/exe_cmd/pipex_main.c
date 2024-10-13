/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:44:00 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/13 15:56:07 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_all_fd(t_pipex *pipex_s)
{
	t_pipex	*save;

	save = NULL; 
	if (!pipex_s)
		pipex_s = minishell()->pipex;
	while (pipex_s->prev)
		pipex_s = pipex_s->prev;
	while (pipex_s)
	{
		if (pipex_s != save)
		{
			ft_close(pipex_s->pipe_fd[0]);
			ft_close(pipex_s->pipe_fd[1]); //TODO 1ft_close
		}
		pipex_s = pipex_s->next;
	}
}

void	free_pipex_s(void)
{
	t_pipex	*temp;

	close_all_fd(minishell()->pipex);
	while (minishell()->pipex)
	{
		temp = minishell()->pipex;
		minishell()->pipex = temp->next;
		free_double_array(temp->cmd);
		free(temp->path);
		free(temp);
	}
}

//TODO Handle error s
void	process_child_pid(t_pipex *curr_pipex_s)
{
	int		status;

	if (waitpid(curr_pipex_s->pid, &status, WNOHANG))
	{
		curr_pipex_s->pid = -1;
		ft_close(curr_pipex_s->pipe_fd[0]);
		ft_close(curr_pipex_s->pipe_fd[1]);
		if (curr_pipex_s->prev && curr_pipex_s->prev->pid > 0)
			kill(curr_pipex_s->prev->pid, SIGPIPE);
		if (WIFEXITED(status))
			curr_pipex_s->status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			curr_pipex_s->status = 128 + WTERMSIG(status);
	}
}

int	process_child_pipes(t_pipex *pipex_s)
{
	t_pipex	*curr_pipex_s;
	int		rep;

	while (pipex_s->next)
		pipex_s = pipex_s->next;
	curr_pipex_s = pipex_s;
	rep = 1;
	while (rep)
	{
		rep = 0;
		while (curr_pipex_s != NULL)
		{
			if (curr_pipex_s->pid > 0)
			{
				rep = 1;
				process_child_pid(curr_pipex_s);
			}
			curr_pipex_s = curr_pipex_s->prev;
		}
		if (rep == 1)
			curr_pipex_s = pipex_s;
	}
	return (pipex_s->status);
	close_all_fd(NULL);
}

int	ft_shell_pipex()
{
	int status;

	if (minishell()->silence_info == true)
		return (0);
	add_to_history();
	new_process_tokens(); //TODO test comment ft_process_tokens_s and ft_exe_pipex_s
	status = process_child_pipes(minishell()->pipex); //manage_pid
	minishell()->status = status;
	return (0);
}

//TODO dir name


// int	ft_shell_pipex(void)
// {
// 	int	status;

// 	if (minishell()->silence_info == true)
// 		return (0);
// 	//my_print_env();
// 	ft_process_tokens_s();
// 	add_to_history();
// 	// print_struct();
// 	ft_exe_pipex_s();
// 	status = process_child_pipes(minishell()->pipex); //manage_pid
// 	minishell()->status = status;
// 	// printf("\n	COMMAND ERR: %d\n", status);
// 	//print_check_processes(minishell()->pipex);
// 	// free_pipex_s(minishell()->pipex); //temp free
// 	//recieve signal when i do exit,
// 	// so i can properly free it and pass responsability
// 	return (status);
// }
