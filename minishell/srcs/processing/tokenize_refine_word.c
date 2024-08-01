/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_refine_word.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:13:32 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/08/01 11:50:43 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	append_token_front(t_token **first, t_token *target, t_token *new)
{
	t_token	*current;

	if (!first || !*first || !target || !new)
		return ;
	if (*first == target)
	{
		new->next = target;
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
		current->type = token_assign(current);
		current = current->next;
		i++;
	}
}

static bool	check_token_word(t_main *main_s, t_token *token)
{
	t_token	*current;
	int		i;

	current = token;
	if (!current)
		return (false);
	i = 0;
	while (current->value[i])
	{
		if (ft_isoperator(current->value[i]))
		{
			extract_operator_word(main_s, current);
			return (true);
		}
		i++;
	}
	return (false);
}

t_token	*tokenize_refine_word(t_main *main_s)
{
	t_token	*current;

	current = main_s->tokens;
	while (current)
	{
		if (current->type == WORD || current->type == QUOTE)
		{
			if (check_token_word(main_s, current))
			{
				reassign_tokens(main_s->tokens);
				reindex_tokens(main_s->tokens);
				current = main_s->tokens;
			}
		}
		current = current->next;
	}
	current = main_s->tokens;
	return (current);
}
