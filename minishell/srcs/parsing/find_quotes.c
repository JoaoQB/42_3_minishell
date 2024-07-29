/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:52:02 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/07/29 14:18:32 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
