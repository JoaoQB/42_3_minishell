/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:52:02 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/07/23 17:15:03 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static char	*iterate_quotes(char *str, int *count, char *qt_char, bool *i_word)
// {
// 	*qt_char = *str;
// 	if (!*i_word)
// 	{
// 		(*count)++;
// 		*i_word = true;
// 	}
// 	str++;
// 	while (*str != *qt_char && *str)
// 		str++;
// 	return (str);
// 	// 	str++;
// 	// while (*str)
// 	// {
// 	// 	if (*str == ' ')
// 	// 	{
// 	// 		*i_word = false;
// 	// 		return (str);
// 	// 	}
// 	// 	else if (ft_isquotes(str))
// 	// 		return (*(str - 1));
// 	// 	str++;
// 	// }
// 	// return (*(str - 1));
// }

// static void	init_variables(int *count, char *qt_char, bool *i_word)
// {
// 	*count = 0;
// 	*qt_char = '\0';
// 	*i_word = false;
// }

// int	count_words_quotes(const char *str)
// {
// 	int		count;
// 	char	quote_char;
// 	bool	is_word;
// 	// bool	is_quotes;

// 	if (!str || !*str)
// 		return (0);
// 	init_variables(&count, &quote_char, &is_word);
// 	while (*str)
// 	{
// 		if (!ft_isspace(*str) && !!ft_isquotes(*str) && !is_word)
// 		{
// 			is_word = true;
// 			count++;
// 		}
// 		else if (ft_isspace && is_word)
// 			is_word = false;
// 		else if (ft_isquotes(*str))
// 			str = iterate_quotes(str, &count, &quote_char, &is_word);
// 		str++;
// 	}
// 	return (count);
// }

bool	find_quotes(t_main *main_s)
{
	char	*input;
	int		i;

	input = main_s->user_input;
	if (!input)
		return (false);
	i = 0;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
			return (true);
		i++;
	}
	return (false);
}
