/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_split_words.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 17:11:37 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/20 13:29:13 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_token	*ambiguous_redirect(t_token *current)
{
	if (!current)
		return (NULL);
	if (!minishell()->silence_info)
		ft_putendl_fd("minishell: ambiguous redirect", 2);
	// minishell()->silence_info = true;
	ft_free(&current->value);
	current->value = ft_strdup("");
	minishell()->status = 1; //TODO this will prevent pipex to be ran?
	return (current->next);
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

static t_token	*check_for_space(t_token **first, t_token *current)
{
	char	*str;
	int		i;

	if (!first || !current)
		return (NULL);
	else if (!current->value && token_is_redirect(current->prev))
		return (ambiguous_redirect(current));
	else if (!current->value)
		return (current->next);
	else if (!*current->value && token_is_redirect(current->prev))
		return (ambiguous_redirect(current));
	str = current->value;
	i = 0;
	while (str[i])
	{
		if (ft_isquotes(str[i]))
			i += iterate_quotes(&str[i]);
		else if (str[i] == ' ' && token_is_redirect(current->prev))
			return (ambiguous_redirect(current));
		else if (str[i] == ' ')
			return (divide_words(first, current, i));
		i++;
	}
	return (current->next);
}

void	token_split_words(t_token **first)
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
			next = check_for_space(first, current);
		current = next;
	}
}
