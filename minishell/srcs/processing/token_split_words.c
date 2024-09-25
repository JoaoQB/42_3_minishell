/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_split_words.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 17:11:37 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/25 14:30:32 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ambiguous_redirect(t_main *main_s)
{
	ft_putendl_fd("minishell:ambiguous redirect", 2);
	main_s->silence_info = true;
}

static t_token	*divide_words(t_token **first, t_token *current, int i)
{
	char	*str;
	t_token	*next;

	if (!first || !current)
		return (NULL);
	next = current->next;
	if (!current->value)
		return (next);
	str = current->value;
	if (i > 0)
	{
		token_extract_before(first, current, i);
		str = current->value;
		i = 0;
	}
	while (str[i] == ' ')
		i++;
	if (str[i] != '\0')
	{
		token_extract_after(current, i);
		next = current->next;
	}
	delete_token(first, current);
	return (next);
}

static t_token	*check_for_space(t_main *main_s, t_token **first, t_token *current)
{
	char	*str;
	int		i;

	if (!first || !current)
		return (NULL);
	else if (!current->value)
		return (current->next);
	str = current->value;
	i = 0;
	if (!*str && token_is_redirect(current->prev))
		ambiguous_redirect(main_s);
	while (str[i])
	{
		if (ft_isquotes(str[i]))
			i += iterate_quotes(&str[i]);
		else if (str[i] == ' ' && token_is_redirect(current->prev))
		{
			ambiguous_redirect(main_s);
			return (current->next);
		}
		else if (str[i] == ' ')
			return (divide_words(first, current, i));
		i++;
	}
	return (current->next);
}

void	token_split_words(t_main *main_s, t_token **first)
{
	t_token	*current;
	t_token	*next;

	if (!first || !*first)
		return ;
	current = *first;
	while (current)
	{
		next = current->next;
		if (current->type == WORD || current->type == QUOTE)
			next = check_for_space(main_s, first, current);
		current = next;
	}
}
