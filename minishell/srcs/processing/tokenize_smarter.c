/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_smarter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 13:28:20 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/10 13:46:16 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	tokenize_args(t_token *current, int type)
{
	if (!current || !current->type)
		return ;
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
	t_token	*next;
	bool	no_cmd;

	if (!first)
		return ;
	no_cmd = true;
	current = first;
	while (current)
	{
		next = current->next;
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
		current = next;
	}
}
