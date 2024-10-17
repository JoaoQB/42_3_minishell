/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_struct.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:30:04 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/17 16:04:43 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_update_cmds(t_token *tokens_s, t_pipex *pipex_s)
{
	int	count;

	count = 0;
	pipex_s->cmd[0] = NULL;
	while (tokens_s && tokens_s->type != PIPE)
	{
		if (tokens_s->type == CMD || tokens_s->type == ARG)
		{
			pipex_s->cmd[count++] = ft_strnjoin(NULL, tokens_s->value, -1);
			pipex_s->cmd[count] = NULL;
		}
		tokens_s = tokens_s->next;
	}
	return (0);
}

int ft_open_fd(t_token *tk_s, int *fd)
{
	int status;

	errno = 0;
	status = 0;
	if (tk_s->type == HERE_DOC && tk_s->next && *tk_s->next->value)
		fd[0] = read_heredoc(tk_s->next);
	else if (tk_s->type == RED_IN && tk_s->next && *tk_s->next->value)
		fd[0] = open(tk_s->next->value, O_RDONLY, 0666);
	else if (tk_s->type == RED_OUT && tk_s->next && *tk_s->next->value)
		fd[1] = open(tk_s->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (tk_s->type == RED_OUT_APP && tk_s->next && *tk_s->next->value)
		fd[1] = open(tk_s->next->value, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if ((fd[0] == -1  || fd[1] == -1))
		status = errno;
	return (status);
}

void	ft_update_fds(t_token *tk_s, t_pipex *pipex_s)
{
	int		*fd;
	int status;

	errno = 0;
	status = 0;
	fd = pipex_s->pipe_fd;
	while (tk_s && tk_s->type != PIPE && status != EACCES && status != ENOENT)
	{
		if (minishell()->status)
			break ;
		if ((tk_s->type == RED_IN || tk_s->type == HERE_DOC))
			ft_close (fd[0]);
		else if ((tk_s->type == RED_OUT || tk_s->type == RED_OUT_APP))
			ft_close (fd[1]);
		status = ft_open_fd(tk_s, fd);
		tk_s = tk_s->next;
	}
	if ((fd[0] == -1  || fd[1] == -1) && status)
	{
		pipex_s->status = 1;
		print_err("%s: %s\n",tk_s->value, strerror(status));
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
	//pipex_s->pipe_fd[0] = -2;
	//pipex_s->pipe_fd[1] = -2;
	pipex_s->pipe_fd[0] = STDIN_FILENO; //add ti the initer
	pipex_s->pipe_fd[1] = STDOUT_FILENO; //add ti the initer
	pipex_s->prev = NULL;
	pipex_s->next = NULL;
	return (pipex_s);
}
