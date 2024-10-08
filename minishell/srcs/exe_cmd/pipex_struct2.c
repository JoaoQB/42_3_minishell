/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_struct2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:25:14 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/08 15:42:06 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_process_tokens_s(void)
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
	int		count;
	t_token	*curr_token;

	curr_token = tokens_s;
	count = 0;
	while (curr_token && curr_token->type != PIPE)
	{
		if (curr_token->type == CMD || curr_token->type == ARG)
			count += 1;
		curr_token = curr_token->next;
	}
	pipex_s->cmd = (char **) safe_malloc(sizeof(char *) *(count + 1));
	if (!pipex_s->cmd)
		return (-1);
	pipex_s->status = ft_update_cmds(tokens_s, pipex_s);
	if (pipex_s->status == 0)
		ft_update_fds(tokens_s, pipex_s);
	return (pipex_s->status);
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
		write(piper[1], "\n", 1);
		ft_free(&expanded_input);
	}
	close(piper[1]);
	return (piper[0]);
}
