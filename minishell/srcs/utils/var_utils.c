/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 09:34:41 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/11 13:05:07 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*extract_inside_quotes(char *str)
{
	char	*new_value;
	int		i;
	int		len;

	i = 1;
	len = 0;
	while (str[i] && str[i] != '"')
	{
		i++;
		len++;
	}
	if (len == 0)
		return (NULL);
	new_value = (char *)malloc(sizeof(char) * len + 1);
	if (!new_value)
		return (NULL);
	ft_strlcpy2(new_value, &str[1], len + 1);
	return (new_value);
}

char	*var_extract_after(t_token *current, int i)
{
	t_token	*new;
	char	*new_value;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = extract_from_i(current->value, i);
	if (!new->value)
	{
		free(new);
		return (NULL);
	}
	current->type = CONC;
	new->type = CONC;
	new->index = -1;
	new->next = NULL;
	new->prev = NULL;
	token_append_after(current, new);
	new_value = extract_before_i(current->value, i);
	if (current->value)
		ft_free(&current->value);
	if (!new_value)
		return (NULL);
	return (new_value);
}

char	*var_extract_before(t_token **first, t_token *current, int i)
{
	t_token	*new;
	char	*new_value;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = extract_before_i(current->value, i);
	if (!new->value)
	{
		free(new);
		return (NULL);
	}
	current->type = CONC;
	new->type = CONC;
	new->index = -1;
	new->next = NULL;
	new->prev = NULL;
	token_append_before(first, current, new);
	new_value = extract_from_i(current->value, i);
	if (current->value)
		ft_free(&current->value);
	if (!new_value)
		return (NULL);
	return (new_value);
}

int	ft_isvarchar(int c)
{
	if (c == '_' || ft_ischar(c) || ft_isdigit(c))
		return (1);
	else
		return (0);
}

int	ft_isvar1stchar(int c)
{
	if (c == '_' || ft_ischar(c))
		return (1);
	else
		return (0);
}
