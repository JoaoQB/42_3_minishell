/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_smarter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 13:28:20 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/08/27 10:31:47 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*ft_token_new_late(char *string, int len)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = (char *)malloc((len + 1) * sizeof(char));
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	ft_strlcpy(new_token->value, string, len + 1);
	new_token->type = token_assign(new_token);
	new_token->index = -1;
	new_token->next = NULL;
	new_token->prev = NULL;
	// new_token->cmd = NULL;
	return (new_token);
}

int	count_cmd_size(t_token *first)
{
	int		cmd_size;
	t_token	*current;

	if (!first)
		return (0);
	cmd_size = 0;
	current = first;
	while (current)
	{
		if (current->type == CMD)
			cmd_size++;
		current = current->next;
	}
	return (cmd_size);
}

static void	tokenize_args(t_token *current, int type)
{
	if (current && current->type == WORD)
	{
		current->type = type;
		current = current->next;
		while (current && current->type != PIPE)
		{
			if (current->type == RED_IN || current->type == RED_OUT
				|| current->type == RED_OUT_APP)
				current->next->type = PATH;
			else if (current->type == HERE_DOC)
				current->next->type = DELIM;
			else if (current->type == WORD)
				current->type = ARG;
			current = current->next;
		}
	}
}

void	tokenize_smarter(t_token *first)
{
	t_token	*current;
	bool	no_cmd;

	if (!first)
		return ;
	no_cmd = true;
	current = first;
	while (current)
	{
		if (no_cmd == true && (current->type == WORD))
		{
			tokenize_args(current, CMD);
			no_cmd = false;
		}
		else if (current->type == PIPE)
			no_cmd = true;
		else if ((current->type == RED_IN || current->type == RED_OUT
				|| current->type == RED_OUT_APP)
			&& current->next && current->next->type == WORD)
			current->next->type = PATH;
		else if (current->type == HERE_DOC
			&& current->next && current->next->type == WORD)
			current->next->type = DELIM;
		current = current->next;
	}
}
