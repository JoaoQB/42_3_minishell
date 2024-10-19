/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_exe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:09:12 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/18 22:37:05 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token *find_next_pipe(t_token *tokens_s)
{
	while (tokens_s && tokens_s->type != PIPE) //funcao para este
		tokens_s = tokens_s->next;
	if (tokens_s && tokens_s->type == PIPE)
		tokens_s = tokens_s->next;
	return (tokens_s);
}

void	ft_n_update_cmds(t_pipex *pipex_s)
{
	int count;
	t_token *tokens_s;

	tokens_s = pipex_s->token;
	count = 0;
	while (tokens_s && tokens_s->type != PIPE) //---------------
	{
		if (tokens_s->type == CMD || tokens_s->type == ARG)// || tokens_s->type == EMPTY)
			count += 1;
		tokens_s = tokens_s->next;
	}
	if (count)
		pipex_s->cmd = (char **) safe_malloc(sizeof(char *) *(count + 1)); //---------------
	count = 0;
	tokens_s = pipex_s->token;
	while (tokens_s && tokens_s->type != PIPE) //funcao para este
	{
		if (tokens_s->type == CMD || tokens_s->type == ARG)// || tokens_s->type == EMPTY)
		{
			pipex_s->cmd[count++] = ft_strnjoin(NULL, tokens_s->value, -1);
			pipex_s->cmd[count] = NULL;
		}
		tokens_s = tokens_s->next;
	}
}

void ft_n_update_fds(t_pipex *pipex_s)
{
	int piper[2];

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

t_pipex *add_back_pipex_s(void)
{
	t_pipex *pipex_s;

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

int ft_n_update_path(t_pipex *pipex_s)
{
	char	*path;
	int		status;

	if (pipex_s->status || !pipex_s->cmd)
		return(pipex_s->status);
	path = pipex_s->cmd[0];
	if (path[0] == '.' && !path[1])
	{
		print_err("%s: filename argument required\n", path);
		print_err("usage: %s filename [arguments]\n", path);
		return (2);
	}
	status = is_directory(path);
	if (status != 0)
		return (status);
	pipex_s->path = get_cmd_path(pipex_s);
	if (pipex_s->path == NULL && pipex_s->status == EACCES)
		return (print_err ("%s: %s\n", path, strerror(pipex_s->status)), 126);
	if (pipex_s->path == NULL && pipex_s->status == ENOENT)
		return (print_err ("%s: command not found\n", path), 127);
	return (0);
}

void new_process_tokens(void)
{
	t_token *token_s;
	t_pipex *pipex_s;

	minishell()->status = 0;
	free_double_array(minishell()->menv);
	minishell()->menv = get_array_env();
	token_s = minishell()->tokens;
	while (token_s && minishell()->status == 0)
	{
		pipex_s = add_back_pipex_s();
		ft_n_update_cmds(pipex_s);
		if (!check_for_pipeline())
			break;
		ft_n_update_fds(pipex_s);
		pipex_s->pid = fork();
		if (pipex_s->pid == -1)
			return (perror("fork failed")); //TODO Handle error s
		else if (pipex_s->pid == 0)
		{
			set_signals(SIGCMD);
			exe_cmd_child(pipex_s, minishell()->menv);
		}
		token_s = find_next_pipe(pipex_s->token);
	}
}
