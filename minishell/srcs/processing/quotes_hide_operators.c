/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_hide_operators.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 02:59:39 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/19 05:01:51 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	revert_operators(char *str)
{
	int		i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		if (str[i] < 0)
		{
			if (str[i] == NEG_PIPE)
				str[i] = '|';
			else if (str[i] == NEG_LESS)
				str[i] = '<';
			else if (str[i] == NEG_GREATER)
				str[i] = '>';
			else if (str[i] == NEG_VAR)
				str[i] = '$';
		}
		i++;
	}
}

void quotes_revert_operators(t_token *first)
{
	t_token *current;

	if (!first)
		return ;
	current = first;
	while (current)
	{
		if (current->type == WORD || current->type == QUOTE)
			revert_operators(current->value);
		current = current->next;
	}
}

void	hide_operators(char *str)
{
	int		i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		if (str[i] > 0 && ft_isoperator(str[i]))
		{
			if (str[i] == '|')
				str[i] = NEG_PIPE;
			else if (str[i] == '<')
				str[i] = NEG_LESS;
			else if (str[i] == '>')
				str[i] = NEG_GREATER;
		}
		i++;
	}
}

void	quotes_hide_operators(t_token *first)
{
	t_token	*current;

	if (!first)
		return ;
	current = first;
	while (current)
	{
		if (current->type == WORD || current->type == QUOTE)
			hide_operators(current->value);
		current = current->next;
	}
}
