/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_remover.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 12:02:51 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/02 13:06:14 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	copy_without_quotes(char *src, char *dest)
{
	int		i;
	int		j;
	char	qt_char;

	if (!src || !dest)
		return ;
	i = 0;
	j = 0;
	while (src[i])
	{
		if (ft_isquotes(src[i]))
		{
			qt_char = src[i];
			i++;
			while (src[i] && src[i] != qt_char)
				dest[j++] = src[i++];
			if (src[i] == qt_char)
				i++;
		}
		else
			dest[j++] = src[i++];
	}
	dest[j] = '\0';
}

static int	n_quotes(char *str)
{
	int		i;
	int		count;

	if (!str)
		return (0);
	i = 0;
	count = 0;
	while (str[i])
	{
		if (ft_isquotes(str[i]))
		{
			count += 2;
			i += iterate_quotes(&str[i]);
		}
		i++;
	}
	return (count);
}

static char	*remove_quotes(t_token *token)
{
	char	*result;
	char	*input;
	int		new_len;

	if (!token || !token->value)
		return (NULL);
	input = token->value;
	new_len = (ft_strlen(input) - n_quotes(input));
	if (new_len <= 0)
		return (ft_strdup(""));
	result = (char *)safe_malloc(sizeof(char) *(new_len + 1));
	copy_without_quotes(input, result);
	return (result);
}

void	quotes_remover(t_token *first)
{
	t_token	*current;
	char	*new_value;

	if (!first)
		return ;
	current = first;
	while (current)
	{
		if (current->type == QUOTE)
		{
			new_value = remove_quotes(current);
			ft_free(&current->value);
			if (new_value)
				current->value = new_value;
			if (current->prev->type == HERE_DOC)
				current->type = DELIM_QUOTE;
			else
				current->type = WORD;
		}
		current = current->next;
	}
	return ;
}
