/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:09:06 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/25 15:20:39 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	token_is_redirect(t_token *current)
{
	if (!current)
		return (false);
	else if (!current->type)
		return (false);
	else if (current->type == RED_IN || current->type == RED_OUT
		|| current->type == RED_OUT_APP)
		return (true);
	return (false);
}

void	delete_token(t_token **first, t_token *current)
{
	t_token	*to_free;

	if (!current)
		return ;
	to_free = current;
	if (current->prev)
		current->prev->next = current->next;
	else
		*first = current->next;
	if (current->next)
		current->next->prev = current->prev;
	ft_free(&to_free->value);
	free(to_free);
}

void	clean_tokens(t_token **first)
{
	t_token	*current;
	t_token	*next;

	if (!first || !*first)
		return ;
	current = *first;
	while (current)
	{
		next = current->next;
		if (!current->value)
			current->value = ft_strdup("");
		else if (current->type == CONC
			|| current->type == VAR || current->type == QUOTE)
			current->type = WORD;
		current = next;
	}
}

void	token_extract_after(t_token *current, int i)
{
	t_token	*new;
	char	*new_value;

	new = (t_token *)safe_malloc(sizeof(t_token));
	new->value = extract_from_i(current->value, i);
	if (!new->value)
	{
		free(new);
		return ;
	}
	new->type = token_assign(new->value);
	new->index = -1;
	new->next = NULL;
	new->prev = NULL;
	token_append_after(current, new);
	new_value = extract_before_i(current->value, i);
	if (current->value)
		ft_free(&current->value);
	current->value = new_value;
}
