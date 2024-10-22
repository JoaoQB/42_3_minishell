/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_files_ctl.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:28:53 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/22 12:29:59 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_close(int *fd)
{
	if (*fd > 2)
	{
		if (close(*fd) == -1)
		{
			*fd = -1;
			return (-1);
		}
		*fd = -1;
	}
	return (0);
}

void	close_all_fd(t_pipex *pipex_s)
{
	t_pipex	*save;

	save = NULL;
	if (!pipex_s)
		pipex_s = minishell()->pipex;
	while (pipex_s && pipex_s->prev)
		pipex_s = pipex_s->prev;
	while (pipex_s)
	{
		if (pipex_s != save)
		{
			ft_close(&pipex_s->pipe_fd[0]);
			ft_close(&pipex_s->pipe_fd[1]);
		}
		pipex_s = pipex_s->next;
	}
}

int	ft_process_redirect(t_token *tk_s, int *fd)
{
	int	status;

	status = 0;
	if (tk_s->type == RED_IN)
		ft_close (&fd[0]);
	else if (tk_s->type == RED_OUT || tk_s->type == RED_OUT_APP)
		ft_close (&fd[1]);
	else
		return (0);
	if (tk_s->type == RED_IN && tk_s->next && *tk_s->next->value)
		fd[0] = open(tk_s->next->value, O_RDONLY, 0666);
	else if (tk_s->type == RED_OUT && tk_s->next && *tk_s->next->value)
		fd[1] = open(tk_s->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (tk_s->type == RED_OUT_APP && tk_s->next && *tk_s->next->value)
		fd[1] = open(tk_s->next->value, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (tk_s->next && tk_s->next->type == PATH && !*tk_s->next->value)
		status = EINVAL;
	else if (fd[0] == -1 || fd[1] == -1)
		status = errno;
	return (status);
}

void	ft_update_fds(t_token *tk_s, t_pipex *pipex_s)
{
	int	*fd;
	int	status;

	ft_update_heredoc(tk_s, pipex_s);
	errno = 0;
	status = 0;
	fd = pipex_s->pipe_fd;
	while (tk_s && tk_s->type != PIPE && minishell()->status == 0)
	{
		status = ft_process_redirect(tk_s, fd);
		tk_s = tk_s->next;
		if (status == EACCES || status == ENOENT || status == EINVAL)
			break ;
	}
	if ((fd[0] == -1 || fd[1] == -1) || status > 0)
	{
		pipex_s->status = 1;
		if (status == EINVAL)
			print_err("%s\n", "ambiguous redirect");
		else
			print_err("%s: %s\n", tk_s->value, strerror(status));
	}
}
