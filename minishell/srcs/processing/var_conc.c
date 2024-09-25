/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_conc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 12:39:06 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/25 12:22:31 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	delete_conc_tokens(t_token **first)
{
	t_token	*current;
	t_token	*next;

	if (!first || !*first)
		return ;
	current = *first;
	while (current)
	{
		next = current->next;
		if (current->type == CONC)
		{
			if (current->prev)
				current->prev->next = current->next;
			else
				*first = current->next;
			if (current->next)
				current->next->prev = current->prev;
			ft_free(&current->value);
			free(current);
		}
		current = next;
	}
}

static void	copy_values(char *result, t_token *target)
{
	t_token	*current;
	int		len;
	int		value_len;

	current = target;
	*result = '\0';
	len = 0;
	while (current && current->type == CONC)
	{
		if (current->value)
		{
			value_len = ft_strlen(current->value);
			ft_strlcpy(result + len, current->value, value_len + 1);
			len += ft_strlen(current->value);
		}
		current = current->next;
	}
}

static char	*conc_values(t_token *target)
{
	t_token	*current;
	char	*result;
	int		total_len;

	if (!target)
		return (NULL);
	current = target;
	total_len = 0;
	while (current && current->type == CONC)
	{
		if (current->value)
			total_len += ft_strlen(current->value);
		current = current->next;
	}
	if (total_len == 0)
		return (NULL);
	result = (char *)safe_malloc(sizeof(char) *(total_len + 1));
	copy_values(result, target);
	return (result);
}

t_token	*var_conc(t_token *target, t_token **first)
{
	t_token	*new;

	if (!target || !first)
		return (NULL);
	new = (t_token *)safe_malloc(sizeof(t_token));
	new->index = -1;
	new->value = NULL;
	new->next = NULL;
	new->prev = NULL;
	new->value = conc_values(target);
	if (new->value)
		new->type = token_assign(new->value);
	token_append_before(first, target, new);
	delete_conc_tokens(first);
	reindex_tokens(*first);
	return (new);
}
