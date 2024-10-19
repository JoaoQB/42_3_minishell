/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_struct2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:25:14 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/18 22:07:31 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_child(int *piper, t_token *tokens_s)
{
	char *input;
	
	set_signals(SIGHD);
	ft_close(&piper[0]);
	while (1)
	{
		input = readline("> ");
		if (!input)
		{
			ft_putstr_fd("warning: here-document delimited", 2);
			ft_putstr_fd(" by end-of-file (wanted ", 2);
			ft_putstr_fd(tokens_s->value, 2);
			ft_putstr_fd(")\n", 2);
		}
		else if (ft_strcmp(input, tokens_s->value) == 0)
			free(input);
		if (!input || ft_strcmp(input, tokens_s->value) == 0)
			break ;
		input = heredoc_expand(tokens_s, input);
		write(piper[1], input, ft_strlen(input));
		write(piper[1], "\n", 1);
		ft_free(&input);
	}
	ft_close(&piper[1]);
	ft_exit(0);
}

int	read_heredoc(t_token *tokens_s)
{
	int		piper[2];
	pid_t	pid;
	int		status;
	
	set_sig_handlers(SIGINT, SIG_IGN);
	if (pipe(piper) == -1)
		return (perror("failed pipe"), 0); //TODO alredy exists put it into function
	pid = fork();
	if (pid == -1)
		return (perror("failed fork"), 0); //TODO
	else if (pid == 0) //child
		heredoc_child(piper, tokens_s);
	else
	{
		waitpid(pid, &status, 0);
		if(WIFEXITED(status) && WEXITSTATUS(status) != 0)
			minishell()->status = 130;
		status = 0;
		ft_close(&piper[1]);
	}
	return (piper[0]);
}
