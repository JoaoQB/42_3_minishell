/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_x_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:52:02 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/19 15:48:28 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	find_equal(char *str)
{
	int	i;

	if (!str)
		return (false);
	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (true);
		i++;
	}
	return (false);
}

bool	find_quotes(char *str)
{
	int		i;

	if (!str)
		return (false);
	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			return (true);
		i++;
	}
	return (false);
}
