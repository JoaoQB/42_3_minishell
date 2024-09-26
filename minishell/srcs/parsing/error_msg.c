/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 10:31:51 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/26 12:02:29 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	syntax_error_pipe()
{
	ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
	minishell()->status = 2;
	minishell()->silence_info = true;
}

void	syntax_error_msg()
{
	ft_putendl_fd("minishell: syntax error", 2);
	minishell()->status = 2;
	minishell()->silence_info = true;
}
