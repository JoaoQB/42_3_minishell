/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   len_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:36:22 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/22 15:00:28 by fandre-b         ###   ########.fr       */
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

char	*ft_strnjoin(char *old_str, char *str_add, int size)
{
	int		len;
	int		i;
	char	*new_str;

	if (!str_add || !str_add[0] || size == 0)
		return (old_str);
	len = 0;
	while (old_str && old_str[len])
		len++;
	i = 0;
	while (str_add && str_add[i])
		i++;
	if (i < size || size == -1)
		size = i;
	new_str = (char *) safe_malloc(size + len + 1);
	i = -1;
	while (old_str && ++i < len)
		new_str[i] = old_str[i];
	i = -1;
	while (str_add && ++i < size)
		new_str[len + i] = str_add[i];
	new_str[len + i] = '\0';
	return (free(old_str), new_str);
}
