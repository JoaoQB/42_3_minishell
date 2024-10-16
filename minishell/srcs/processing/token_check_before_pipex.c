/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_check_before_pipex.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:43:16 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/16 14:58:23 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	token_check_before_pipex(t_token *first)
{
	t_token	*current;
	int		count;

	if (!first)
		return ;
	current = first;
	count = 0;
	while (current)
	{
		if (current->type != EMPTY && current->type != PIPE)
			count++;
		current = current->next;
	}
	if (count == 0)
		minishell()->silence_info = true;
}
