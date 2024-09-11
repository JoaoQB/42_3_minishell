/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 15:54:16 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/11 13:40:52 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	ft_strlcpy2(char *dst, const char *src, size_t size)
{
	size_t	i;

	if (!dst || !src || size == 0)
		return (0);
	i = 0;
	while (src[i] && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while ((*s1 || *s2) && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((*(unsigned char *)s1 - *(unsigned char *)s2));
}
