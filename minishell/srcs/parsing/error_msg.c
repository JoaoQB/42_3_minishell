/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 10:31:51 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/07/10 13:23:18 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	syntax_error_pipe(t_main *main_s)
{
	write(2, "Syntax error near unexpected token `|'\n", 40);
	main_s->silence_info = true;
}

void	syntax_error_msg(t_main *main_s)
{
	write(2, "Syntax error\n", 14);
	main_s->silence_info = true;
}
