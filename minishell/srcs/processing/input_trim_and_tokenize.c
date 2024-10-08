/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_trim_and_tokenize.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 11:39:05 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/07 17:23:36 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	concatenate_word_space(const char *word, char *dest, int *pos)
{
	int	i;

	if (!word || !dest)
		return ;
	i = 0;
	while (word[i])
	{
		dest[*pos] = word[i];
		i++;
		(*pos)++;
	}
	dest[*pos] = ' ';
	(*pos)++;
}

static char	*alloc_and_concat(char **words, int total_len, int j)
{
	char	*trimmed;
	int		current_pos;
	int		i;

	trimmed = (char *)safe_malloc((total_len + j) * sizeof(char));
	if (!trimmed)
		return (NULL);
	current_pos = 0;
	i = -1;
	while (words[++i])
		concatenate_word_space(words[i], trimmed, &current_pos);
	trimmed[current_pos - 1] = '\0';
	free_double_array(words);
	return (trimmed);
}

char	*input_trim_and_tokenize(char *user_input)
{
	char	**words;
	int		i;
	int		total_len;

	if (!user_input || !parse_quotes(user_input))
		return (NULL);
	words = split_into_words(user_input);
	if (!words)
		return (NULL);
	minishell()->tokens = input_tokenize(words);
	total_len = 0;
	i = -1;
	while (words[++i])
		total_len += ft_strlen(words[i]);
	if (total_len == 0)
	{
		if (words)
			free_double_array(words);
		return (NULL);
	}
	return (alloc_and_concat(words, total_len, i));
}
