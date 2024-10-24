/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:51:31 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/22 15:18:12 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*export_append_values(t_env *new, t_env *current)
{
	char	*new_value;
	char	*new_var_value;
	int		len;

	if (!new || !new->value || !current)
		return (NULL);
	if (current->var_value && !new->var_value)
		return (extract_from_i(current->var_value, 0));
	new_var_value = NULL;
	len = ft_strlen(new->var_value) + ft_strlen(current->var_value);
	new_var_value = (char *)safe_malloc(sizeof(char) * (len + 1));
	if (!new_var_value)
		return (NULL);
	new_var_value[0] = '\0';
	if (current->var_value)
		ft_strcat(new_var_value, current->var_value);
	if (new->var_value)
		ft_strcat(new_var_value, new->var_value);
	new_value = env_get_value(new->var, new_var_value);
	ft_free(&current->value);
	current->value = new_value;
	ft_free(&current->var_value);
	current->var_value = new_var_value;
	return (extract_from_i(new_var_value, 0));
}

static void	handle_not_found(t_env *new)
{
	char	*var_value;

	if (!new || !new->value)
		return ;
	var_value = NULL;
	export_set_ordered(&minishell()->export, new);
	if (find_equal(new->value))
	{
		if (new->var_value)
			var_value = extract_from_i(new->var_value, 0);
		ft_setenv(new->var, var_value, 1);
	}
}

static void	handle_found(t_env *new, t_env *current)
{
	char	*var_value;

	if (!new || !new->value || !current)
		return ;
	var_value = NULL;
	if (!find_equal(new->value))
		return ;
	else if (find_plus(new->value))
	{
		var_value = export_append_values(new, current);
		ft_setenv(new->var, var_value, 1);
		return ;
	}
	ft_free(&current->value);
	current->value = extract_from_i(new->value, 0);
	ft_free(&current->var_value);
	if (new->var_value)
	{
		current->var_value = extract_from_i(new->var_value, 0);
		var_value = extract_from_i(new->var_value, 0);
	}
	ft_setenv(new->var, var_value, 1);
}

void	export_check(t_env *new)
{
	t_env	*current;
	bool	found;

	if (!new)
		return ;
	found = false;
	current = minishell()->export;
	while (current)
	{
		if (ft_strcmp(new->var, current->var) == 0)
		{
			found = true;
			break ;
		}
		current = current->next;
	}
	if (found == true)
	{
		handle_found(new, current);
		free_env(&new);
	}
	else
		handle_not_found(new);
}

void	print_export(t_env *first)
{
	t_env	*current;

	if (!first)
		return ;
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
