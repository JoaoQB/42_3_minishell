/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:51:31 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/20 16:03:19 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	handle_not_found(t_main *main_s, t_env *new)
{
	char	*var_value;

	if (!main_s || !new || !new->value)
		return ;
	var_value = NULL;
	export_set_ordered(&main_s->export, new);
	if (find_equal(new->value))
	{
		if (new->var_value)
			var_value = extract_from_i(new->var_value, 0);
		ft_setenv(main_s, new->var, var_value, 1);
	}
}

static void	handle_found(t_main *main_s, t_env *new, t_env *current)
{
	char	*var_value;

	if (!main_s || !new || !new->value || !current)
		return ;
	if (!find_equal(new->value))
		return ;
	var_value = NULL;
	ft_free(&current->value);
	if (new->value)
		current->value = extract_from_i(new->value, 0);
	ft_free(&current->var_value);
	if (new->var_value)
	{
		current->var_value = extract_from_i(new->var_value, 0);
		var_value = extract_from_i(new->var_value, 0);
	}
	ft_setenv(main_s, new->var, var_value, 1);
}

void	export_check(t_main *main_s, t_env *new)
{
	t_env	*current;
	bool	found;

	if (!main_s || !new)
		return ;
	found = false;
	current = main_s->export;
	while (current)
	{
		if (ft_strcmp(new->var, current->var) == 0)
		{
			found = true;
			break;
		}
		current = current->next;
	}
	if (found == true)
	{
		handle_found(main_s, new, current);
		free_env(&new);
	}
	else
		handle_not_found(main_s, new);
}

void	print_export(t_env *first)
{
	t_env	*current;

	if (!first)
		return;
	current = first;
	while (current)
	{
		if (current->var)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(current->var, 1);
			if (find_equal(current->value))
			{
				ft_putstr_fd("=\"", 1);
				if (current->var_value)
					ft_putstr_fd(current->var_value, 1);
				ft_putstr_fd("\"", 1);
			}
			ft_putstr_fd("\n", 1);
		}
		current = current->next;
	}
}
