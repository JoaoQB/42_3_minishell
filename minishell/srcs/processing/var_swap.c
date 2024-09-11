/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_swap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 15:34:27 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/11 13:07:35 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_token	*var_check_quote(t_main *main_s, t_token **first, t_token *current)
{
	char	*str;
	int		i;
	bool	is_quote;

	if (!main_s || !first || !*first || !current || !current->value)
		return (NULL);
	str = current->value;
	if (!str)
		return (current->next);
	i = 0;
	is_quote = false;
	while(str[i])
	{
		if (str[i] == '"')
			is_quote = !is_quote;
		else if (str[i] == '\'' && !is_quote)
			i += iterate_quotes(&str[i]);
		else if (str[i] == '$' && !is_quote && ft_isquotes(str[i + 1]))
			return (var_remove_quotes(first, current, i));
		else if (str[i] == '$')
			return (var_replace(main_s, current, i));
		i++;
	}
	if (current->type == CONC)
		return (var_conc_quotes(&main_s->tokens, current));
	return (current->next);
}

void	var_swap(t_main *main_s, t_token **first)
{
	t_token	*current;
	t_token	*next;

	if (!first)
		return ;
	current = *first;
	while(current)
	{
		next = current->next;
		if (current->type == HERE_DOC)
		{
			current = next;
			next = current->next;
		}
		else if (current->type == WORD || current->type == QUOTE || current->type == CONC)
			next = var_check_quote(main_s, first, current);
		current = next;
	}
	return ;
}