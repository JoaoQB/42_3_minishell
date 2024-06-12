/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   len_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:36:22 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/06/09 20:50:37 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_words(const char *str)
{
	int		i;
	int		count;
	bool	is_word;

	if (!str)
		return (0);
	i = 0;
	count = 0;
	is_word = false;
	while (str[i])
	{
		if (!ft_isspace(str[i]) && !is_word)
		{
			is_word = true;
			count++;
		}
		else if (ft_isspace(str[i]) && is_word == true)
			is_word = false;
		i++;
	}
	return (count);
}

int	count_arrays(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
