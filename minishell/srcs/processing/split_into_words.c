/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_into_words.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:28:52 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/06/10 16:02:39 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Don't forget to free double array after calling this function. */

static char	*fill_substr(const char *src, size_t len)
{
	char	*dest;

	if (!src)
		return (NULL);
	dest = (char *)malloc(sizeof(char) * (len));
	if (!dest)
		return (NULL);
	ft_strlcpy(dest, src, len);
	return (dest);
}

static char	**ft_free(char **strs, int count)
{
	while (--count >= 0)
		free(*(strs + count));
	free(strs);
	return (NULL);
}

static int	iterate_i(const char *s, int i)
{
	while (s[i] && !ft_isspace(s[i]))
		i++;
	return (i);
}

char	**split_into_words(char const *s)
{
	char	**split_strs;
	int		i;
	int		j;
	int		i_word;

	split_strs = (char **)malloc((count_words(s) + 1) * sizeof(char *));
	if (!split_strs || !s)
		return (NULL);
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
			*(split_strs + j++) = fill_substr(&s[i_word], (i - i_word) + 1);
			if (!(*(split_strs + j - 1)))
				return (ft_free(split_strs, j));
		}
	}
	*(split_strs + j) = NULL;
	return (split_strs);
}
