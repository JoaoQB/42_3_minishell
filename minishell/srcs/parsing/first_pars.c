/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_pars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 12:23:33 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/07/15 11:50:56 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	pars_redirection(t_main *main_s, t_token *current)
{
	t_token	*next;

	if (!current->next)
	{
		syntax_error_msg(main_s);
		return (false);
	}
	next = current->next;
	if (next->type != WORD && next->type != QT_SIN && next->type != QT_DBL)
	{
		syntax_error_msg(main_s);
		return (false);
	}
	return (true);
}

static bool	pars_pipe(t_main *main_s, t_token *current)
{
	t_token	*next;

	if (!current->next)
	{
		syntax_error_pipe(main_s);
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
		{
			if (!pars_pipe(main_s, current))
				return (false);
		}
		else if (current->type == RED_IN || current->type == RED_OUT
			|| current->type == RED_OUT_APP || current->type == HERE_DOC)
		{
			if (!pars_redirection(main_s, current))
				return (false);
		}
		current = current->next;
	}
	return (true);
}
