/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_remove_quotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:18:48 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/19 04:59:55 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*var_conc_quotes(t_token **first, t_token *target)
{
	t_token	*current;

	if (!first || !first)
		return (NULL);
	current = *first;
	while (current)
	{
		if (current->type == CONC)
			return (var_conc(current, first));
		current = current->next;
	}
	return (target);
}

static char	*var_delete_var_quotes(t_token *current)
{
	char	*new_value;
	int		len;

	if (!current || !current->value)
		return (NULL);
	if (ft_strlen(current->value) == 3)
	{
		ft_free(&current->value);
		return (NULL);
	}
	len = ft_strlen(current->value) - 1;
	new_value =(char *)safe_malloc(sizeof(char) *(len + 1));
	ft_strlcpy2(new_value, &current->value[1], len + 1);
	return (new_value);
}

static char	*var_extract_after_quotes(t_token *current)
{
	char	*str;
	int		i;

	if (!current || !current->value)
		return (NULL);
	str = current->value;
	i = 1;
	i += iterate_quotes(&str[i]) + 1;
	if (str[i] == '\0')
		return (str);
	return (var_extract_after(current, i));
}

t_token	*var_remove_quotes(t_token **first, t_token *current, int i)
{
	if (!first || !*first || !current || !current->value)
		return (NULL);
	if (i > 0)
	{
		current->value = var_extract_before(first, current, i);
		i = 0;
	}
	current->value = var_extract_after_quotes(current);
	current->value = var_delete_var_quotes(current);
	if (current->value)
		hide_operators(current->value);
	if (current->type == CONC)
		return (var_conc_quotes(first, current));
	if (!current->value)
		return (current->next);
	return (current);
}
