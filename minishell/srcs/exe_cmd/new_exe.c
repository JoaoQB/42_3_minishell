/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_exe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:09:12 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/03 17:49:55 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//start with tokens only

// /should i create the full struct of pipes at the start of doi it dinamicly?

//run all tokens
//while tokens != NULL
//	if tokens->type == PIPE
// 		tokens = tokens->next
//  process_pipe(tokens)
//  exe_cmd(pipe)
//  tokens = tokens->next



//process_pipe
//pipex_s = ft_init_pipex_s
//pipex_s->token = tokens
//update_cmd
	//will malloc and update the cmd array
//if (first_pipe)
	//fd_in = STDIN_FILENO
//if (no_next_pipe)
	//fd_out = STDOUT_FILENO
//update_fds
	//will update the fds

t_token *find_next_pipe(t_token *tokens_s)
{
    while (tokens_s && tokens_s->type != PIPE) //funcao para este
		tokens_s = tokens_s->next;
}

int	ft_new_update_cmds(t_pipex *pipex_s)
{
	int count;
    t_token *tokens_s;

    tokens_s = pipex_s->token;
	pipex_s->cmd[0] = NULL; //safeguard
	count = 0;
	while (tokens_s && tokens_s->type != PIPE) //---------------
	{
		if (tokens_s->type == CMD || tokens_s->type == ARG) // || tokens_s->type == EMPTY
			count += 1;
		tokens_s = tokens_s->next;
	}
	pipex_s->cmd = (char **) safe_malloc(sizeof(char *) *(count + 1)); //---------------
	count = 0;
	tokens_s = pipex_s->token; //to be used
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

void ft_n_update_fds(t_pipex *pipex_s)
{
    int *piper;
    
    if (!pipex_s->next)
        return;
    if (pipe(piper) == -1)
        return (perror("pipe"), errno);//TODO hdl err
    pipex_s->pipe_fd[1] = piper[1];
    pipex_s->next->pipe_fd[0] = piper[0];
    ft_update_fds(pipex_s->token, pipex_s);
}

t_pipex *add_back_pipex_s(void)
{
    t_pipex *pipex_s;

	if (minishell()->pipex)
		minishell()->pipex = ft_init_pipex_s();
    pipex_s = minishell()->pipex;
    while (pipex_s->next)
        pipex_s = pipex_s->next;
    if (find_next_pipe(pipex_s->token))
    {
        pipex_s->next = ft_init_pipex_s();
        pipex_s->next->prev = pipex_s; //TODO do i need this?
    }
    ft_n_update_cmds(pipex_s);
    ft_n_update_fds(pipex_s);
    return (pipex_s);
}

void new_process_tokens(void)
{
    t_token *token_s;
    t_pipex *pipex_s;

    token_s = minishell()->tokens;
    while (token_s)
    {
        if (token_s->type == PIPE)
            token_s = token_s->next;
        pipex_s->token = token_s;
        pipex_s = add_back_pipex_s();
        exe_cmd_child(pipex_s, minishell()->menv);
        token_s = find_next_pipe(token_s); //TODO
    }
}
