/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:52:02 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/07/22 14:20:32 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	parse_quotes(t_main *main_s, char *input)
{
	int		i;
	bool	in_sng_qt;
	bool	in_dbl_qt;

	i = 0;
	in_dbl_qt = false;
	in_sng_qt = false;
	while (input[i])
	{
		if (input[i] == '"' && !in_sng_qt)
			in_dbl_qt = !in_dbl_qt;
		else if (input[i] == '\'' && !in_dbl_qt)
			in_sng_qt = !in_sng_qt;
		i++;
	}
	if (!in_sng_qt && !in_dbl_qt)
		return ;
	else
		syntax_error_msg(main_s);
}

bool	find_quotes(t_main *main_s)
{
	char	*input;
	int		i;

	input = main_s->user_input;
	if (!input)
		return (false);
	i = 0;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
			return (true);
		i++;
	}
	return (false);
}
