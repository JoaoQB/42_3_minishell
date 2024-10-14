/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_exe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:09:12 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/14 15:20:40 by jqueijo-         ###   ########.fr       */
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
	// pipex_s->cmd = NULL; //safeguard
	count = 0;
	while (tokens_s && tokens_s->type != PIPE) //---------------
	{
		if (tokens_s->type == CMD || tokens_s->type == ARG) // || tokens_s->type == EMPTY
			count += 1;
		tokens_s = tokens_s->next;
	}
	if (count)
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
}

void ft_n_update_fds(t_pipex *pipex_s)
{
	int piper[2];

	if (pipex_s->next)
	{
		if (pipe(piper) == -1)
		{
			print_err("%s\n", strerror(errno));
			free_main_input();
			cleanup_main();
			exit(1);
		}
		pipex_s->pipe_fd[1] = piper[1];
		pipex_s->next->pipe_fd[0] = piper[0];
	}
	// ft_update_fds2(pipex_s->token, pipex_s);
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

// int is_directory(const char *path)
// {
// 	struct stat buffer;

// 	if (stat(path, &buffer) != 0)
// 		return (errno);
// 	else if (S_ISDIR(buffer.st_mode))
// 		return (EISDIR);
// 	return (0);
// }

int ft_n_update_path(t_pipex *pipex_s)
{
	char *path;

	if (pipex_s->status != 0 || !pipex_s->cmd)
		return(pipex_s->status);
	path = pipex_s->cmd[0];
	if (path[0] == '.' && !path[1])
	{
		print_err("%s: filename argument required\n", path);
		print_err("usage: %s filename [arguments]\n", path);
		return (2);
	}
	if(is_directory(path) != 0)
	{
		print_err("%s: %s\n", path, strerror(EISDIR));
		return (126);
	}
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

	token_s = minishell()->tokens;
	while (token_s)
	{
		pipex_s = add_back_pipex_s();
		ft_n_update_cmds(pipex_s);
		if (pipex_s == minishell()->pipex && !check_for_pipeline())
			break;
		ft_n_update_fds(pipex_s);
		pipex_s->pid = fork();
		if (pipex_s->pid == -1)
			return (perror("fork failed")); //TODO Handle error s
		else if (pipex_s->pid == 0)
		{
			set_signals(SIGCMD);
			exe_cmd_child(pipex_s, minishell()->menv);
			printf("ola2");
		}
		token_s = find_next_pipe(pipex_s->token);
	}
}
