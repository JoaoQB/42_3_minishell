/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_pars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 12:23:33 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/19 04:59:55 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	parse_quotes(t_main *main_s, char *input)
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
		return (true);
	else
		syntax_error_msg(main_s);
	return (false);
}

static bool	pars_redirection(t_main *main_s, t_token *current)
{
	t_token	*next;

	if (!current->next)
	{
		syntax_error_msg(main_s);
		return (false);
	}
	next = current->next;
	if (next->type != WORD && next->type != QUOTE)
	{
		syntax_error_msg(main_s);
		return (false);
	}
	return (true);
}

static bool	pars_pipe(t_main *main_s, t_token *current)
{
	t_token	*next;

	if (!current->prev)
	{
		syntax_error_pipe(main_s);
		return (false);
	}
	else if (!current->next)
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
