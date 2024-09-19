/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_into_words.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:28:52 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/19 04:59:55 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Don't forget to free double array after calling split_into_words(). */
static char	*fill_substr(const char *src, size_t len)
{
	char	*dest;

	if (!src)
		return (NULL);
	dest =(char *)safe_malloc(sizeof(char) *(len + 1));
	ft_strlcpy2(dest, src, len + 1);
	return (dest);
}

static char	**ft_free_array(char **strs, int count)
{
	while (--count >= 0)
		free(*(strs + count));
	free(strs);
	return (NULL);
}

static int	iterate_i(const char *s, int i)
{
	char	quote_char;

	while (s[i] && !ft_isspace(s[i]))
	{
		if (ft_isquotes(s[i]))
		{
			quote_char = s[i];
			i++;
			while (s[i] && s[i] != quote_char)
				i++;
		}
		i++;
	}
	return (i);
}

char	**split_into_words(char const *s)
{
	char	**split_strs;
	int		i;
	int		j;
	int		i_word;

	split_strs =(char **)safe_malloc((count_words(s) + 1) * sizeof(char *));
	j = 0;
	i = 0;
	while (*(s + i))
	{
		if (ft_isspace(s[i]))
			i++;
		i_word = i;
		i = iterate_i(s, i);
		if (i_word < i)
		{
			*(split_strs + j++) = fill_substr(&s[i_word],(i - i_word));
			if (!(*(split_strs + j - 1)))
				return (ft_free_array(split_strs, j));
		}
	}
	*(split_strs + j) = NULL;
	return (split_strs);
}
