/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:54:23 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/26 19:10:56 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	export_set_ordered(t_env **first, t_env *new)
{
	t_env	*current;
	t_env	*target;

	if (!*first)
	{
		*first = new;
		return ;
	}
	current = *first;
	target = NULL;
	while (current && ft_strcmp(current->var, new->var) < 0)
	{
		target = current;
		current = current->next;
	}
	if (!target)
	{
		new->next = current;
		*first = new;
	}
	else
	{
		new->next = current;
		target->next = new;
	}
}

char	*env_check_name(char *str)
{
	int		i;
	bool	valid;

	if (!str)
		return (NULL);
	valid = true;
	i = 0;
	if (!ft_isvar1stchar(str[i]))
		valid = false;
	i++;
	while (str[i] && str[i] != '=' && str[i] != '+')
	{
		if (!ft_isvarchar(str[i]))
			valid = false;
		i++;
	}
	if (valid == false)
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(str, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (NULL);
	}
	return (extract_from_i(str, 0));
}

t_env	*ft_export_new(char *str)
{
	t_env	*new;

	if (!str)
		return (NULL);
	new = (t_env *)safe_malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->value = env_check_name(str);
	if (!new->value)
	{
		free(new);
		return (NULL);
	}
	new->var_value = NULL;
	new->var = env_extract_var(new);
	new->next = NULL;
	return (new);
}

t_env	*sort_env()
{
	t_env	*current_env;
	t_env	*first_export;
	t_env	*new_export;

	if (!minishell()->env)
		return (NULL);
	current_env = minishell()->env;
	first_export = NULL;
	while (current_env)
	{
		if (current_env->value)
		{
			new_export = ft_export_new(current_env->value);
			if (!new_export)
			{
				free_env(&first_export);
				return (NULL);
			}
			export_set_ordered(&first_export, new_export);
		}
		current_env = current_env->next;
	}
	return (first_export);
}
