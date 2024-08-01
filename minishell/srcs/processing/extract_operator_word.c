/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_operator_word.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:11:50 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/08/01 14:14:13 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	check_after_operator(char *string)
{
	if (!string)
		return (false);
	else if (!ft_strncmp(string, "<<", 2) && string[2] != '\0')
		return (true);
	else if (!ft_strncmp(string, ">>", 2) && string[2] != '\0')
		return (true);
	else if (string[1] != '\0' && (ft_strncmp(string, ">>", 2)
			&& ft_strncmp(string, "<<", 2)))
		return (true);
	return (false);
}

static char	*remove_before_operator(t_token *current, int len)
{
	char	*new_string;
	int		total_len;

	total_len = ft_strlen(current->value);
	new_string = (char *)malloc(sizeof(char) * (total_len - len + 1));
	if (!new_string)
		return (NULL);
	ft_strlcpy(new_string, &current->value[len], total_len - len + 1);
	if (current->value)
		free(current->value);
	return (new_string);
}

static void	extract_before_operator(t_main *main_s, t_token *current, int len)
{
	t_token	*new;

	new = ft_token_new_late(current->value, len);
	if (!new)
		return ;
	append_token_front(&main_s->tokens, current, new);
	current->value = remove_before_operator(current, len);
}

static int	iterate_quotes(char *str)
{
	int		i;
	char	qt_char;

	if (!str)
		return (0);
	i = 0;
	qt_char = str[i++];
	while (str[i] != qt_char && str[i])
		i++;
	return (i);
}

void	extract_operator_word(t_main *main_s, t_token *current)
{
	int		i;

	i = 0;
	while (current->value[i] && !ft_isoperator(current->value[i]))
	{
		if (ft_isquotes(current->value[i]))
			i += iterate_quotes(&current->value[i]);
		i++;
	}
	if (i != 0)
		extract_before_operator(main_s, current, i);
	i = 0;
	if (check_after_operator(current->value))
	{
		if (!ft_strncmp(current->value, "<<", 2)
			|| !ft_strncmp(current->value, ">>", 2))
			extract_before_operator(main_s, current, 2);
		else
			extract_before_operator(main_s, current, 1);
	}
}
