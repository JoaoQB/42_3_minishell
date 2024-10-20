/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:44:00 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/20 15:36:42 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	process_child_pipes(t_pipex *pipex_s)
{
	t_pipex	*curr_pipex_s;
	int		rep;

	set_signals(SIGCMD);
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

void	create_error_fd(void)
{
	int	err_fd[2];

	if (pipe(err_fd) == -1)
	{
		print_err("%s\n", strerror(errno));
		minishell()->status = 1;
		return ;
	}
	minishell()->err_fd[0] = err_fd[0];
	minishell()->err_fd[1] = err_fd[1];
}

//TODO perror?
void	read_error_fd(void)
{
	int			ret;
	static char	buffer[10];

	ft_close(&minishell()->err_fd[1]);
	ret = 1;
	while (ret)
	{
		ret = read(minishell()->err_fd[0], buffer, 10);
		if (ret > 0)
			write(2, buffer, ret);
	}
	if (ret == -1)
		perror("read error");
	ret = -1;
	while (ret++ < 10)
		buffer[ret] = 0;
	ft_close(&minishell()->err_fd[0]);
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
