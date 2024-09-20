/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 15:54:16 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/20 15:24:00 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while ((*s1 || *s2) && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((*(unsigned char *)s1 - *(unsigned char *)s2));
}
