/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:44:00 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/22 15:24:07 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exe_cmd_child(t_pipex *pipex_s, char **envp)
{
	if (pipex_s->status || minishell()->status || !pipex_s->cmd)
		ft_exit_pid(pipex_s);
	if (pipex_s->pipe_fd[0] != STDIN_FILENO)
		dup2(pipex_s->pipe_fd[0], STDIN_FILENO);
	if (pipex_s->pipe_fd[1] != STDOUT_FILENO)
		dup2(pipex_s->pipe_fd[1], STDOUT_FILENO);
	dup2(minishell()->err_fd[1], STDERR_FILENO);
	close_all_fd(pipex_s);
	if (edge_cases(pipex_s) || special_edge_cases(pipex_s))
		ft_exit_pid(pipex_s);
	pipex_s->status = ft_n_update_path(pipex_s);
	if (pipex_s->status || !pipex_s->path)
		ft_exit_pid(pipex_s);
	else if (execve(pipex_s->path, pipex_s->cmd, envp) == -1)
		pipex_s->status = errno;
	ft_exit_pid(pipex_s);
}

int	check_for_pipeline(void)
{
	t_pipex	*first_pipe;

	first_pipe = minishell()->pipex;
	if (first_pipe && !first_pipe->next && !find_next_pipe(first_pipe->token))
		if (first_pipe->status || special_edge_cases(first_pipe))
			return (0);
	return (1);
}

void	new_process_tokens(void)
{
	t_token	*token_s;
	t_pipex	*pipex_s;

	minishell()->status = 0;
	token_s = minishell()->tokens;
	while (token_s && minishell()->status == 0)
	{
		pipex_s = add_back_pipex_s();
		ft_n_update_cmds(pipex_s);
		if (!check_for_pipeline())
			break ;
		ft_n_update_fds(pipex_s);
		pipex_s->pid = fork();
		if (pipex_s->pid == -1)
			return (perror("fork failed"));
		else if (pipex_s->pid == 0)
		{
			set_signals(SIGCMD);
			exe_cmd_child(pipex_s, minishell()->menv);
		}
		token_s = find_next_pipe(pipex_s->token);
	}
}

int	get_final_status(void)
{
	t_pipex	*pipex_s;

	if (minishell()->status)
		return (minishell()-> status);
	pipex_s = minishell()->pipex;
	while (pipex_s->next)
		pipex_s = pipex_s->next;
	return (pipex_s->status);
}

int	ft_shell_pipex(void)
{
	if (minishell()->silence_info == true)
		return (0);
	create_error_fd();
	add_to_history();
	new_process_tokens();
	if (minishell()->pipex)
		process_child_pipes(minishell()->pipex);
	minishell()->status = get_final_status();
	read_error_fd();
	return (0);
}
