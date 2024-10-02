/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_struct.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:30:04 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/02 22:12:11 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	new_process_pipe();
{
	pipe
}

int		ft_process_tokens_s()
{
	t_token	*tokens_s;
	t_pipex	*pipex_s;

	tokens_s = minishell()->tokens;
	ft_create_pipeline();
	pipex_s = minishell()->pipex;
	while (tokens_s->next != NULL)
		tokens_s = tokens_s->next;
	while (pipex_s->next != NULL)
		pipex_s = pipex_s->next;
	while (tokens_s != NULL)
	{//why tf i did back to front???
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
	pipex_s->cmd = (char **) safe_malloc(sizeof(char *) *(count + 1));
	ft_update_cmds(tokens_s, pipex_s);
	if (pipex_s->status == 0)//this part will be done in the child pipe executer
		ft_update_fds(tokens_s, pipex_s); //TODO test fds
	return (pipex_s->status);
}

int	ft_update_cmds(t_token *tokens_s, t_pipex *pipex_s)
{
	int count;

	count = 0;
	pipex_s->cmd[0] = NULL;
	pipex_s->token = tokens_s; //to be used 
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



int	read_heredoc(t_token *tokens_s)
{
	char	*input;
	char	*delim;
	char	*expanded_input;
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
		expanded_input = heredoc_expand(tokens_s, input);
		write(piper[1], expanded_input, ft_strlen(expanded_input));
		write(piper[1], "\n", 1);//TODO check this new line
		ft_free(&expanded_input);
	}
	close(piper[1]);
	return (piper[0]);
}

int	ft_update_fds(t_token *tokens_s, t_pipex *pipex_s)
{//i dont need to change mutch in here, maybe find a way to do here_doc anyways
//i can try to ro return errno regarless, or just do an errorhandler, it will not exit but process will
	int		*io_fd;

	io_fd = pipex_s->pipe_fd;
	while (tokens_s && tokens_s->type != PIPE)
	{
		if ((tokens_s->type == RED_IN || tokens_s->type == HERE_DOC) && io_fd[0] > 2)
			close (io_fd[0]);
		if ((tokens_s->type == RED_OUT || tokens_s->type == RED_OUT_APP) && io_fd[1] > 2)
			close (io_fd[1]);
		if (tokens_s->type == HERE_DOC && tokens_s->next && *tokens_s->next->value)
			io_fd[0] = read_heredoc(tokens_s->next);
		else if (tokens_s->type == RED_IN && tokens_s->next && *tokens_s->next->value)
			io_fd[0] = open(tokens_s->next->value, O_RDONLY, 0666);
		else if (tokens_s->type == RED_OUT && tokens_s->next && *tokens_s->next->value)
			io_fd[1] = open(tokens_s->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		else if (tokens_s->type == RED_OUT_APP && tokens_s->next && *tokens_s->next->value)
			io_fd[1] = open(tokens_s->next->value, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (io_fd[0] ==-1 || io_fd[1] == -1)
			return(printf("%s: %s\n", tokens_s->next->value, strerror(errno)), errno); //TODO Handle error s
		tokens_s = tokens_s->next;
	}
	return (0);
}

int ft_create_pipeline()
{//this will be deprecated
	t_pipex *pipex_s;
	t_token *tokens_s;
	int	piper[2];

	if (!minishell()->pipex)
		minishell()->pipex = ft_init_pipex_s();
	tokens_s = minishell()->tokens;
	pipex_s = minishell()->pipex;
	pipex_s->pipe_fd[0] = STDIN_FILENO;
	pipex_s->pipe_fd[1] = STDOUT_FILENO;
	while (tokens_s != NULL)
	{
		if (tokens_s->type == PIPE)
		{
			pipex_s->next = ft_init_pipex_s();
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

t_pipex *ft_init_pipex_s()
{
    t_pipex *pipex_s;

    pipex_s = (t_pipex *)safe_malloc(sizeof(t_pipex));
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
