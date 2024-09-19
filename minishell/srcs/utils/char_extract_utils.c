/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_extract_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 11:46:57 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/19 04:59:55 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	iterate_quotes(char *str)
{
	int		i;
	char	qt_char;

	if (!str)
		return (0);
	i = 0;
	qt_char = str[i++];
	while (str[i] != qt_char && str[i])
		i++;
	return (i);
}

char	*extract_from_i(char *str, int i)
{
	char	*new_value;
	int		len;
	if (!str)
		return (NULL);
	len = ft_strlen(str) - i;
	new_value =(char *)safe_malloc(sizeof(char) *(len + 1));
	ft_strlcpy2(new_value, &str[i], len + 1);
	return (new_value);
}

char	*extract_before_i(char *str, int i)
{
	char	*new_value;

	if (!str)
		return (NULL);
	new_value =(char *)safe_malloc(sizeof(char) *(i + 1));
	ft_strlcpy2(new_value, str, i + 1);
	return (new_value);
}
