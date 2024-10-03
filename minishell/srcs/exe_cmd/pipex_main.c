/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:44:00 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/03 12:08:33 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_child_pid(t_pipex *curr_pipex_s)
{//an v2 of this will be called when an child finish is detected.
//maybe this one can work just fine, just need to add an free_node, to make it even better.
	int		status;

	if (waitpid(curr_pipex_s->pid, &status, WNOHANG))
	{
		curr_pipex_s->pid = -1;
		if (curr_pipex_s->pipe_fd[0] > 2)
		{
			if (close(curr_pipex_s->pipe_fd[0]) == -1)
				perror("Error closing pipe_fd[0]"); //TODO Handle error s
			if (curr_pipex_s->prev && curr_pipex_s->prev->pid > 0)
				kill(curr_pipex_s->prev->pid, SIGPIPE);
		}
		if (curr_pipex_s->pipe_fd[1] > 2)
			if (close(curr_pipex_s->pipe_fd[1]) == -1)
				perror("Error closing pipe_fd[1]"); //TODO Handle error s
		if (WIFEXITED(status))
			curr_pipex_s->status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			curr_pipex_s->status = 128 + WTERMSIG(status);
		// free_pipex_node(curr_pipex_s); reddy to test
	}
}

int	process_child_pipes(t_pipex *pipex_s)
{//i can still run this at the end, but there is the possibility of incompatibility
//the signal processe will search in minishell()->pipex for the id, maybe run an altered version of
//this one that checks the full list once per signal
	t_pipex	*curr_pipex_s;
	int 	rep;

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
}

int	ft_shell_pipex()
{
	int status;

	if (minishell()->silence_info == true)
		return (0);
	//my_print_env();
	ft_process_tokens_s();
	add_to_history();
	// print_struct();
	ft_exe_pipex_s();
	status = process_child_pipes(minishell()->pipex); //manage_pid
	minishell()->status = status;
	// printf("\n	COMMAND ERR: %d\n", status);
	//print_check_processes(minishell()->pipex);
	// free_pipex_s(minishell()->pipex); //temp free
	//recieve signal when i do exit, so i can properly free it and pass responsability
	return (status);
}

