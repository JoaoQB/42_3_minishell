/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   len_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:36:22 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/07/29 13:53:53 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static const char	*iterate_quotes(const char *str, int *count, bool *is_word)
{
	char	quote_char;

	quote_char = *str;
	if (!*is_word)
	{
		*is_word = true;
		(*count)++;
	}
	str++;
	while(*str && *str != quote_char)
		str++;
	return (str);
}

int count_words(const char *str)
{
	int		count;
	bool	is_word;

	if (!str || !*str)
		return (0);
	count = 0;
	is_word = false;
	while(*str)
	{
		if (ft_isquotes(*str))
			str = iterate_quotes(str, &count, &is_word);
		else if (!ft_isspace(*str) && !is_word)
		{
			count++;
			is_word = true;
		}
		else if (ft_isspace(*str) && is_word)
			is_word = false;
		str++;
	}
	return (count);
}

int	count_arrays(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
