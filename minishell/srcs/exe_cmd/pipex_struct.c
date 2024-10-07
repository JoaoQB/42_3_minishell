/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_struct.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:30:04 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/07 17:24:30 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// TODO ADD EMPTY TOKENS
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

// TODO Handle error s
int	ft_update_fds(t_token *tk_s, t_pipex *pipex_s)
{
	int		*fd;

	fd = pipex_s->pipe_fd;
	while (tk_s && tk_s->type != PIPE)
	{
		if ((tk_s->type == RED_IN || tk_s->type == HERE_DOC) && fd[0] > 2)
			close (fd[0]);
		if ((tk_s->type == RED_OUT || tk_s->type == RED_OUT_APP) && fd[1] > 2)
			close (fd[1]);
		if (tk_s->type == HERE_DOC && tk_s->next && *tk_s->next->value)
			fd[0] = read_heredoc(tk_s->next);
		else if (tk_s->type == RED_IN && tk_s->next && *tk_s->next->value)
			fd[0] = open(tk_s->next->value, O_RDONLY, 0666);
		else if (tk_s->type == RED_OUT && tk_s->next && *tk_s->next->value)
			fd[1] = open(tk_s->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		else if (tk_s->type == RED_OUT_APP && tk_s->next && *tk_s->next->value)
			fd[1] = open(tk_s->next->value,
					O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (fd[0] == -1 || fd[1] == -1)
			return (printf("%s: %s\n",
					tk_s->next->value, strerror(errno)), errno);
		tk_s = tk_s->next;
	}
	return (0);
}

static int	init_next_pipex(t_pipex *pipex_s, int piper[2])
{
	pipex_s->next = ft_init_pipex_s();
	pipex_s->next->prev = pipex_s;
	if (pipe(piper) == -1)
		return (perror("pipe"), errno);
	pipex_s->next->pipe_fd[1] = pipex_s->pipe_fd[1];
	pipex_s->pipe_fd[1] = piper[1];
	pipex_s->next->pipe_fd[0] = piper[0];
	return (0);
}

int	ft_create_pipeline(void)
{
	t_pipex	*pipex_s;
	t_token	*tokens_s;
	int		piper[2];

	if (!minishell()->pipex)
		minishell()->pipex = ft_init_pipex_s();
	tokens_s = minishell()->tokens;
	pipex_s = minishell()->pipex;
	pipex_s->pipe_fd[0] = STDIN_FILENO;
	pipex_s->pipe_fd[1] = STDOUT_FILENO;
	while (tokens_s)
	{
		if (tokens_s->type == PIPE)
		{
			if (init_next_pipex(pipex_s, piper) != 0)
				return (errno);
			pipex_s = pipex_s->next;
		}
		tokens_s = tokens_s->next;
	}
	return (0);
}

t_pipex	*ft_init_pipex_s(void)
{
	t_pipex	*pipex_s;

	pipex_s = (t_pipex *)safe_malloc(sizeof(t_pipex));
	if (!pipex_s)
		return (NULL);
	pipex_s->pid = -1;
	pipex_s->status = 0;
	pipex_s->path = NULL;
	pipex_s->cmd = NULL;
	pipex_s->pipe_fd[0] = -2;
	pipex_s->pipe_fd[1] = -2;
	pipex_s->prev = NULL;
	pipex_s->next = NULL;
	return (pipex_s);
}
