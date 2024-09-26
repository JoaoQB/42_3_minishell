/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_separate_operator.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:13:32 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/25 12:08:11 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	check_after_operator(char *string)
{
	if (!string)
		return (false);
	else if (!ft_strncmp(string, "<<", 2) && string[2] != '\0')
		return (true);
	else if (!ft_strncmp(string, ">>", 2) && string[2] != '\0')
		return (true);
	else if (string[1] != '\0' && (ft_strncmp(string, ">>", 2)
			&& ft_strncmp(string, "<<", 2)))
		return (true);
	return (false);
}

static void	extract_operator(t_token *current, int i)
{
	char	*str;

	if (!current || !current->value)
		return ;
	str = current->value;
	if (i > 0)
	{
		token_extract_before(&minishell()->tokens, current, i);
		str = current->value;
		i = 0;
	}
	if (check_after_operator(str))
	{
		if (!ft_strncmp(current->value, "<<", 2)
			|| !ft_strncmp(current->value, ">>", 2))
			token_extract_before(&minishell()->tokens, current, 2);
		else
			token_extract_before(&minishell()->tokens, current, 1);
	}
}

static bool	check_token_word(t_token *current)
{
	char	*str;
	int		i;

	if (!current || !current->value)
		return (false);
	str = current->value;
	i = 0;
	while (str[i])
	{
		if (ft_isquotes(str[i]))
			i += iterate_quotes(&str[i]);
		else if (ft_isoperator(str[i]))
		{
			extract_operator(current, i);
			return (true);
		}
		i++;
	}
	return (false);
}

t_token	*token_separate_operator()
{
	t_token	*current;
	t_token	*next;

	current = minishell()->tokens;
	while (current)
	{
		next = current->next;
		if (current->type == WORD || current->type == QUOTE)
		{
			if (check_token_word(current))
			{
				reassign_tokens(minishell()->tokens);
				reindex_tokens(minishell()->tokens);
				next = minishell()->tokens;
			}
		}
		current = next;
	}
	current = minishell()->tokens;
	return (current);
}
