/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_struct.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 17:41:15 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/08/27 12:21:52 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_env(t_env *first)
{
	t_env	*temp;

	if (!first)
		return ;
	while (first)
	{
		temp = first;
		first = first->next;
		if (temp->value)
			free (temp->value);
		if (temp->var)
			free (temp->var);
		if (temp->var_value)
			free (temp->var_value);
		free (temp);
	}
}

void	free_tokens(t_token *token)
{
	t_token	*temp;

	if (!token)
		return ;
	while (token)
	{
		temp = token;
		token = token->next;
		if (temp->value)
			free (temp->value);
		free (temp);
	}
}
