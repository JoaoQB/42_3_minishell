/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_struct2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:25:14 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/16 10:38:28 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	read_heredoc(t_token *tokens_s)
{
	char	*input;
	char	*delim;
	char	*expanded_input;
	int		piper[2];

	set_signals(SIGHD);
	delim = tokens_s->value;
	if (pipe(piper) == -1)
		return (perror("failed pipe"), 0); //TODO alredy exists put it into function
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
	ft_close(piper[1]);
	return (piper[0]);
}
