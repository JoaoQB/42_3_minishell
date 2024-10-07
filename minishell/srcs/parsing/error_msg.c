/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 10:31:51 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/07 15:57:04 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	syntax_error_pipe(void)
{
	ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
	minishell()->status = 2;
	minishell()->silence_info = true;
}

void	syntax_error_msg(void)
{
	ft_putendl_fd("minishell: syntax error", 2);
	minishell()->status = 2;
	minishell()->silence_info = true;
}
