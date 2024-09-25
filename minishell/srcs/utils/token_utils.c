/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 17:20:48 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/25 12:24:30 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	token_extract_before(t_token **first, t_token *current, int i)
{
	t_token	*new;
	char	*new_value;

	new = (t_token *)safe_malloc(sizeof(t_token));
	new->value = extract_before_i(current->value, i);
	if (!new->value)
	{
		free(new);
		return ;
	}
	new->type = token_assign(new->value);
	new->index = -1;
	new->next = NULL;
	new->prev = NULL;
	token_append_before(first, current, new);
	new_value = extract_from_i(current->value, i);
	if (current->value)
		ft_free(&current->value);
	current->value = new_value;
}

void	token_append_after(t_token *target, t_token *new)
{
	if (!target || !new)
		return ;
	if (target->next)
	{
		new->next = target->next;
		target->next->prev = new;
		new->prev = target;
		target->next = new;
		return ;
	}
	target->next = new;
	new->prev = target;
}

void	token_append_before(t_token **first, t_token *target, t_token *new)
{
	t_token	*current;

	if (!first || !*first || !target || !new)
		return ;
	if (*first == target)
	{
		new->next = target;
		target->prev = new;
		*first = new;
		return ;
	}
	current = *first;
	while (current->next != target && current->next)
		current = current->next;
	if (current->next == target)
	{
		new->next = target;
		current->next = new;
		new->prev = current;
		target->prev = new;
	}
}

void	reindex_tokens(t_token *first)
{
	t_token	*current;
	int		i;

	i = 0;
	current = first;
	while (current)
	{
		current->index = i;
		current = current->next;
		i++;
	}
}

void	reassign_tokens(t_token *first)
{
	t_token	*current;
	int		i;

	i = 0;
	current = first;
	while (current)
	{
		current->type = token_assign(current->value);
		current = current->next;
		i++;
	}
}
