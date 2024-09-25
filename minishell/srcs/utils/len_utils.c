/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   len_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:36:22 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/25 12:25:47 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static const char	*iterate_qt_str(const char *str, int *count, bool *is_w)
{
	char	quote_char;

	quote_char = *str;
	if (!*is_w)
	{
		*is_w = true;
		(*count)++;
	}
	str++;
	while (*str && *str != quote_char)
		str++;
	return (str);
}

int	count_words(const char *str)
{
	int		count;
	bool	is_word;

	if (!str || !*str)
		return (0);
	count = 0;
	is_word = false;
	while (*str)
	{
		if (ft_isquotes(*str))
			str = iterate_qt_str(str, &count, &is_word);
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

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}
