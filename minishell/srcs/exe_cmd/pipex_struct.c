/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_struct.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:30:04 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/20 23:01:21 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_update_heredoc(t_token *tk_s, t_pipex *pipex_s)
{
	int	*fd;

	fd = pipex_s->pipe_fd;
	while (tk_s && tk_s->type != PIPE)
	{
		if (minishell()->status)
			break ;
		if (tk_s->type == HERE_DOC)
			ft_close (&fd[0]);
		if (tk_s->type == HERE_DOC && tk_s->next && *tk_s->next->value)
		{
			fd[0] = read_heredoc(tk_s->next);
			minishell()->temp_fd[1] = -2;
			minishell()->temp_fd[1] = -2;
		}
		tk_s = tk_s->next;
	}
}

int	ft_process_redirect(t_token *tk_s, int *fd)
{
	int	status;

	status = 0;
	if ((tk_s->type == RED_IN))
		ft_close (&fd[0]);
	else if ((tk_s->type == RED_OUT || tk_s->type == RED_OUT_APP))
		ft_close (&fd[1]);
	if (tk_s->type == RED_IN && tk_s->next && *tk_s->next->value)
		fd[0] = open(tk_s->next->value, O_RDONLY, 0666);
	else if (tk_s->type == RED_OUT && tk_s->next && *tk_s->next->value)
		fd[1] = open(tk_s->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (tk_s->type == RED_OUT_APP && tk_s->next && *tk_s->next->value)
		fd[1] = open(tk_s->next->value, O_WRONLY | O_CREAT | O_APPEND, 0666);
	else if (tk_s->next && tk_s->next->type == PATH && !*tk_s->next->value)
		status = EINVAL;
	if ((fd[0] == -1 || fd[1] == -1))
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

t_pipex	*ft_init_pipex_s(void)
{
	t_pipex	*pipex_s;

	pipex_s = (t_pipex *)safe_malloc(sizeof(t_pipex));
	pipex_s->pid = -1;
	pipex_s->status = 0;
	pipex_s->path = NULL;
	pipex_s->cmd = NULL;
	pipex_s->pipe_fd[0] = STDIN_FILENO;
	pipex_s->pipe_fd[1] = STDOUT_FILENO;
	pipex_s->prev = NULL;
	pipex_s->next = NULL;
	return (pipex_s);
}
