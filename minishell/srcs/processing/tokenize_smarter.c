/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_smarter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 13:28:20 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/07/11 13:00:04 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	tokenize_args(t_token *current, int type)
{
	if (current && current->type == WORD)
	{
		current->type = type;
		current = current->next;
		while (current && current->type == WORD)
		{
			current->type = ARG;
			current = current->next;
		}
	}
}

void	tokenize_smarter(t_token *first)
{
	t_token	*current;

	current = first;
	while (current)
	{
		if (current == first && current->type == WORD)
			tokenize_args(current, CMD);
		else if (current->type == PIPE)
			tokenize_args(current->next, CMD);
		else if (current->type == RED_IN || current->type == RED_OUT
			|| current->type == RED_OUT_APP)
		{
			if (current->next && current->next->type == WORD)
				current->next->type = PATH;
		}
		current = current ->next;
	}
}
