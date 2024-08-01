/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_x.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 09:55:50 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/08/01 11:33:14 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_isquotes(int c)
{
	if (c == '"' || c == '\'')
		return (1);
	else
		return (0);
}

int	ft_isoperator(int c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	else
		return (0);
}

int	ft_isspace(int c)
{
	if ((c >= '\t' && c <= '\r') || c == ' ')
		return (1);
	else
		return (0);
}
