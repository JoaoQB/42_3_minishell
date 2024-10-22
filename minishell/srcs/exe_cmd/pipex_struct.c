/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_struct.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:30:04 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/22 12:45:39 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*find_next_pipe(t_token *tokens_s)
{
	while (tokens_s && tokens_s->type != PIPE)
		tokens_s = tokens_s->next;
	if (tokens_s && tokens_s->type == PIPE)
		tokens_s = tokens_s->next;
	return (tokens_s);
}

void	ft_n_update_fds(t_pipex *pipex_s)
{
	int	piper[2];

	if (pipex_s->next)
	{
		if (pipe(piper) == -1)
		{
			print_err("%s\n", strerror(errno));
			minishell()->status = 1;
			return ;
		}
		pipex_s->pipe_fd[1] = piper[1];
		pipex_s->next->pipe_fd[0] = piper[0];
	}
	ft_update_fds(pipex_s->token, pipex_s);
}

void	ft_n_update_cmds(t_pipex *pipex_s)
{
	int		count;
	t_token	*tokens_s;

	tokens_s = pipex_s->token;
	count = 0;
	while (tokens_s && tokens_s->type != PIPE)
	{
		if (tokens_s->type == CMD || tokens_s->type == ARG)
			count += 1;
		tokens_s = tokens_s->next;
	}
	if (count)
		pipex_s->cmd = (char **) safe_malloc(sizeof(char *) *(count + 1));
	count = 0;
	tokens_s = pipex_s->token;
	while (tokens_s && tokens_s->type != PIPE)
	{
		if (tokens_s->type == CMD || tokens_s->type == ARG)
		{
			pipex_s->cmd[count++] = ft_strnjoin(NULL, tokens_s->value, -1);
			pipex_s->cmd[count] = NULL;
		}
		tokens_s = tokens_s->next;
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

t_pipex	*add_back_pipex_s(void)
{
	t_pipex	*pipex_s;

	if (!minishell()->pipex)
	{
		minishell()->pipex = ft_init_pipex_s();
		minishell()->pipex->token = minishell()->tokens;
	}
	pipex_s = minishell()->pipex;
	while (pipex_s->next)
		pipex_s = pipex_s->next;
	if (find_next_pipe(pipex_s->token))
	{
		pipex_s->next = ft_init_pipex_s();
		pipex_s->next->prev = pipex_s;
		pipex_s->next->token = find_next_pipe(pipex_s->token);
	}
	return (pipex_s);
}
