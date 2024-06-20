/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reorg_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:11:50 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/06/20 11:12:36 by jqueijo-         ###   ########.fr       */
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

t_token	*ft_token_new_late(char *string, int len)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = (char *)malloc((len + 1) * sizeof(char));
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	ft_strlcpy(new_token->value, string, len + 1);
	new_token->type = token_assign(new_token);
	new_token->index = -1;
	new_token->next = NULL;
	return (new_token);
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

void	reorg_word(t_main *main_s, t_token *current)
{
	int		i;

	i = 0;
	while (current->value[i] && !ft_isoperator(current->value[i]))
		i++;
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
