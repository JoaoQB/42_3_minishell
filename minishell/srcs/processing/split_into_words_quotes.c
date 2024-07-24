/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_into_words_quotes.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 11:41:53 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/07/24 10:54:05 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"



// // static int	iterate_operator(char *str, int *i, int *s_word, bool *is_word)
// // {
// // 	*is_word = false;
// // 	*s_word = *i;
// // 	if (str[*i] == '<' && str[*i + 1] == '<')
// // 		*i++;
// // 	else if (str[*i] == '>' && str[*i + 1] == '>')
// // 		*i++;

// // }

// static int	iterate_quotes(char *str, int *i, int *s_word, bool *is_word)
// {
// 	char	quote_char;

// 	quote_char = str[*i];
// 	if (!*is_word)
// 	{
// 		*is_word = true;
// 		*s_word = *i;
// 	}
// 	(*i)++;
// 	while (str[*i] && str[*i] != quote_char)
// 		(*i++);
// 	return (*i);
// }

// static void	init_variables(int *i, int *j, int *s_word, bool *is_word)
// {
// 	*i = 0;
// 	*j = 0;
// 	*s_word = 0;
// 	*is_word = false;
// }

// char	**split_into_words_quotes(const char *str)
// {
// 	char **split_strs;
// 	int		i;
// 	int		j;
// 	int		s_word;
// 	bool	is_word;

// 	split_strs = (char **)malloc((count_words(str) + 1) * sizeof(char *));
// 		if (!split_strs || !str)
// 			return (NULL);
// 	init_variables(&i, &j, &s_word, &is_word);
// 	while (str[i])
// 	{
// 		if (ft_isspace(str[i]))
// 			i++;
// 		i_word
// 	}
// }
// char	**split_into_words_quotes(char const *str)
// {
// 	char	**split_strs;
// 	int		i;
// 	int		j;
// 	int		s_word;
// 	bool	is_word;

// 	split_strs = (char **)malloc((count_words(str) + 1) * sizeof(char *));
// 	if (!split_strs || !str)
// 		return (NULL);
// 	while (str[i])
// 	{
// 		if (ft_isquotes(str[i]))
// 			i = iterate_quotes(str, i, &s_word, &is_word);
// 		else if (ft_isoperator(str[i]))
// 		{
// 			if (is_word)
// 				split_strs[j++] = fill_substr(&str[s_word], (i - s_word) + 1, j);
// 			s_word = i;
// 			i = iterate_operator(str, i, &s_word, &is_word);
// 			split_strs[j++] = fill_substr(&str[s_word], (i - s_word) + 1, j);
// 		}
// 		i++;
// 	}
// }
