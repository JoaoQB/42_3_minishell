/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:25:22 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/22 15:14:38 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	input_error(t_token *token)
{
	ft_putstr_fd("warning: here-document delimited", 2);
	ft_putstr_fd(" by end-of-file (wanted ", 2);
	if (token && token->value)
		ft_putstr_fd(token->value, 2);
	ft_putstr_fd(")\n", 2);
}

void	heredoc_child(int *piper, t_token *tokens_s)
{
	char	*input;

	set_signals(SIGHDC);
	while (1)
	{
		input = readline("> ");
		if (!input)
		{
			input_error(tokens_s);
			break ;
		}
		else if (ft_strcmp(input, tokens_s->value) == 0)
		{
			free(input);
			break ;
		}
		input = heredoc_expand(tokens_s, input);
		ft_putendl_fd(input, piper[1]);
		ft_free(&input);
	}
	ft_close(&piper[0]);
	ft_close(&piper[1]);
	ft_exit(0);
}

// TODO PERROR?
int	read_heredoc(t_token *tokens_s)
{
	int		piper[2];
	pid_t	pid;
	int		status;

	set_signals(SIGHD);
	if (pipe(piper) == -1)
		return (perror("failed pipe"), 0);
	minishell()->temp_fd[0] = piper[0];
	minishell()->temp_fd[1] = piper[1];
	pid = fork();
	if (pid == -1)
		return (perror("failed fork"), 0);
	else if (pid == 0)
		heredoc_child(piper, tokens_s);
	else
	{
		status = 0;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			minishell()->status = 130;
		ft_close(&piper[1]);
		set_signals(SIGMAIN);
	}
	return (piper[0]);
}

void	ft_update_heredoc(t_token *tk_s, t_pipex *pipex_s)
{
	while (tk_s && tk_s->type != PIPE)
	{
		if (minishell()->status)
			break ;
		if (tk_s->type == HERE_DOC)
			ft_close (&pipex_s->pipe_fd[0]);
		if (tk_s->type == HERE_DOC && tk_s->next && *tk_s->next->value)
		{
			pipex_s->pipe_fd[0] = read_heredoc(tk_s->next);
			minishell()->temp_fd[0] = -2;
			minishell()->temp_fd[1] = -2;
		}
		tk_s = tk_s->next;
	}
}
