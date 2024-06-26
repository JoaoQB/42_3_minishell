/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_pars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 12:23:33 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/06/26 11:54:53 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	pars_redin(t_main *main_s, t_token *current)
{
	t_token	*next;

	if (!current->next)
	{
		synstax_error_msg(main_s);
		return (false);
	}
}

/* Checking if after pipe comes nothing or other pipe */
static bool	pars_pipe(t_main *main_s, t_token *current)
{
	t_token	*next;

	if (!current->next)
	{
		// opens here_doc line to check for rest of input.
		// pipe_doc(current); TODO
		syntar_error_pipe(main_s);
		return (false);
	}
	else
		next = current->next;
	if (next->type == PIPE)
	{
		syntax_error_pipe(main_s);
		return (false);
	}
	return (true);
}

bool	first_pars(t_main *main_s, t_token *first)
{
	t_token	*current;

	current = first;
	while (current)
	{
		if (current->type == PIPE)
			if (!pars_pipe(main_s, current))
				return (false);
		else if (current->type == RED_IN)
			if (!pars_redin(main_s, current))
				return (false);
	}
}
