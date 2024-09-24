/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_struct.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:30:04 by fandre-b          #+#    #+#             */
/*   Updated: 2024/09/24 11:01:54 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		ft_process_tokens_s(t_main *main_s)
{
	t_token	*tokens_s;
	t_pipex	*pipex_s;

	tokens_s = main_s->tokens;
	ft_create_pipeline(main_s);
	pipex_s = main_s->pipex;
	while (tokens_s->next != NULL)
		tokens_s = tokens_s->next;
	while (pipex_s->next != NULL)
		pipex_s = pipex_s->next;
	while (tokens_s != NULL)
	{
		if (tokens_s->prev == NULL || tokens_s->prev->type == PIPE)
		{
			ft_update_pipex_s(tokens_s, pipex_s);
			pipex_s = pipex_s->prev;
		}
		tokens_s = tokens_s->prev;
	}
	return (0);
}

int	ft_update_pipex_s(t_token *tokens_s, t_pipex *pipex_s)
{
	int count;
	t_token *curr_token;

	curr_token = tokens_s;
	count = 0;
	while (curr_token && curr_token->type != PIPE)
	{
		if (curr_token->type == CMD || curr_token->type == ARG)
			count += 1;
		curr_token = curr_token->next;
	}
	pipex_s->cmd =(char **) safe_malloc(sizeof(char *) *(count + 1));

	pipex_s->status = ft_update_cmds(tokens_s, pipex_s);
	if (pipex_s->status == 0)
		pipex_s->status = ft_update_fds(tokens_s, pipex_s);
	return (pipex_s->status);
}

int	ft_update_cmds(t_token *tokens_s, t_pipex *pipex_s)
{
	int count;

	count = 0;
	pipex_s->cmd[0] = NULL;
	while (tokens_s && tokens_s->type != PIPE) //funcao para este
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



int read_heredoc(t_token *tokens_s)
{
	char *input;
	char *delim;
	int		piper[2];

	delim = tokens_s->value;
	if (pipe(piper) == -1)
		return (perror("failed pipe"), 0);
	while (1)
	{
		input = readline("> ");
		if (!input)
			break ;
		if (ft_strcmp(input, delim) == 0)
		{
			free(input);
			break ;
		}
		write(piper[1], input, ft_strlen(input));
		write(piper[1], "\n", 1);
		free(input);
	}
	close(piper[1]);
	return (piper[0]);
}

int	ft_update_fds(t_token *tokens_s, t_pipex *pipex_s)
{
	int		*io_fd;

	io_fd = pipex_s->pipe_fd;
	while (tokens_s && tokens_s->type != PIPE)
	{
		if ((tokens_s->type == RED_IN || tokens_s->type == HERE_DOC) && io_fd[0] > 2)
			close (io_fd[0]);
		if ((tokens_s->type == RED_OUT || tokens_s->type == RED_OUT_APP) && io_fd[1] > 2)
			close (io_fd[1]);
		if(tokens_s->type == DELIM)
			io_fd[0] = read_heredoc(tokens_s);
		else if (tokens_s->type == RED_IN)
			io_fd[0] = open(tokens_s->next->value, O_RDONLY, 0666);
		else if (tokens_s->type == RED_OUT)
			io_fd[1] = open(tokens_s->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		else if (tokens_s->type == RED_OUT_APP)
			io_fd[1] = open(tokens_s->next->value, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (io_fd[0] ==-1 || io_fd[1] == -1)
			return(printf("%s: %s\n", tokens_s->next->value, strerror(errno)), errno); //TODO Handle error s
		tokens_s = tokens_s->next;
	}
	return (0);
}

int ft_create_pipeline(t_main *main_s)
{
	t_pipex *pipex_s;
	t_token *tokens_s;
	int	piper[2];

	if (!main_s->pipex)
		main_s->pipex = ft_init_pipex_s(main_s);
	tokens_s = main_s->tokens;
	pipex_s = main_s->pipex;
	pipex_s->pipe_fd[0] = STDIN_FILENO;
	pipex_s->pipe_fd[1] = STDOUT_FILENO;
	while (tokens_s != NULL)
	{
		if (tokens_s->type == PIPE)
		{
			pipex_s->next = ft_init_pipex_s(main_s);
			pipex_s->next->prev = pipex_s;
			if (pipe(piper) == -1)
				return (perror("pipe"), errno);
			pipex_s->next->pipe_fd[1] = pipex_s->pipe_fd[1];
			pipex_s->pipe_fd[1] = piper[1];
			pipex_s->next->pipe_fd[0] = piper[0];
			pipex_s = pipex_s->next;
		}
		tokens_s = tokens_s->next;
	}
	return (0);
}

t_pipex *ft_init_pipex_s(t_main *main_s)
{
    t_pipex *pipex_s;

    pipex_s =(t_pipex *)safe_malloc(sizeof(t_pipex));
    pipex_s->pid = -1;
    pipex_s->status = 0;
	pipex_s->path = NULL;
	pipex_s->cmd = NULL;
    pipex_s->pipe_fd[0] = -2;
    pipex_s->pipe_fd[1] = -2;
	pipex_s->prev = NULL;
    pipex_s->next = NULL;
	pipex_s->main_s = main_s;
    return (pipex_s);
}
