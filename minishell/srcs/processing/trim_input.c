/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:10:33 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/06/12 11:08:50 by jqueijo-         ###   ########.fr       */
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

	trimmed = (char *)malloc((total_len + j) * sizeof(char));
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

char	*trim_input(t_main	*main_s, char *user_input)
{
	char	**words;
	int		i;
	int		total_len;

	words = split_into_words(user_input);
	if (!words)
		return (NULL);
	main_s->tokens = tokenize_input(words);
	total_len = 0;
	i = -1;
	while (words[++i])
		total_len += ft_strlen(words[i]);
	if (total_len == 0)
	{
		if (words)
			free_double_array(words);
		return (ft_strdup(""));
	}
	return (alloc_and_concat(words, total_len, i));
}
