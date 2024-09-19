/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_struct.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 17:41:15 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/19 05:01:51 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_env(t_env **first)
{
	t_env	*current;
	t_env	*temp;
	t_env	*next;

	if (!first || !*first)
		return ;
	current = *first;
	while (current)
	{
		next = current->next;
		temp = current;
		if (temp->value)
			ft_free(&temp->value);
		if (temp->var)
			ft_free(&temp->var);
		if (temp->var_value)
			ft_free(&temp->var_value);
		free(temp);
		current = next;
	}
	*first = NULL;
}

void	free_tokens(t_token **first)
{
	t_token	*current;
	t_token	*temp;
	t_token	*next;

	if (!first || !*first)
		return ;
	current = *first;
	while (current)
	{
		next = current->next;
		temp = current;
		if (temp->value)
			ft_free(&temp->value);
		free(temp);
		current = next;
	}
	*first = NULL;
}
