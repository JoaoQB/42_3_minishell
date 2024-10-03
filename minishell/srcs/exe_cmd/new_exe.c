/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_exe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:09:12 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/03 13:00:56 by fandre-b         ###   ########.fr       */
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




void ft_n_update_fds(t_pipex *pipex_s)
{
    int *piper;
    
	pipex_s->pipe_fd[0] = STDIN_FILENO; //add ti the initer
	pipex_s->pipe_fd[1] = STDOUT_FILENO; //add ti the initer
    if (!pipex_s->next)
        return;
    if (pipe(piper) == -1)
        return (perror("pipe"), errno);
    pipex_s->pipe_fd[1] = piper[1];
    pipex_s->next->pipe_fd[0] = piper[0];
    ft_update_fds(pipex_s);
}

t_pipex *add_back_pipex_s(void) //make sure there is allways 2 pipes
{
    t_pipex *pipex_s;

	if (minishell()->pipex)
    {
		minishell()->pipex = ft_init_pipex_s();
	    minishell()->pipex->pipe_fd[0] = STDIN_FILENO;
        pipex_s = minishell()->pipex;
    }
    while (pipex_s->next)
        pipex_s = pipex_s->next;
    if (find_next_pipe())
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
    t_pipex *pipex_s

    token_s = minishell()->tokens;
    while (token_s)
    {
        if (token_s->type == PIPE)
            token_s = token_s->next;
        pipex_s = add_back_pipex_s()
        pipex_s->token = tokens_s;
    }
}
//if (no_next_pipe)
	//fd_out = STDOUT_FILENO
    while (pipex_s)
}