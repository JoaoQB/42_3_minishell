/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_replace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 02:28:16 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/18 17:58:36 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *change_tilde(t_main *main_s)
{
	t_env	*current_env;
	char	*new_value;
	int		new_len;

	if (!main_s || !main_s->env)
		return (NULL);
	current_env = main_s->env;
	while (current_env)
	{
		if (!ft_strncmp("HOME\0", current_env->var, 5))
		{
			if (!current_env->var_value)
				return (NULL);
			new_len = ft_strlen(current_env->var_value);
			new_value = (char *)safe_malloc(sizeof(char) * (new_len + 1));
			ft_strlcpy2(new_value, current_env->var_value, new_len + 1);
			return (new_value);
		}
		current_env = current_env->next;
	}
	return (NULL);
}

static char	*change_tilde_before(t_main *main_s, t_token *current)
{
	char	*tilde_value;
	char	*new_value;
	int		current_len;
	int		new_len;

	if (!main_s || !main_s->env || !current || !current->value)
		return (NULL);
	tilde_value = change_tilde(main_s);
	if (!tilde_value)
		return (NULL);
	current_len = ft_strlen(current->value) - 1;
	new_len = ft_strlen(tilde_value);
	new_value = (char *)safe_malloc(sizeof(char) * (new_len + current_len + 1));
	ft_strlcpy2(new_value, tilde_value, new_len + 1);
	ft_strlcpy2(&new_value[new_len], &current->value[1], current_len + 1);
	ft_free(&tilde_value);
	return (new_value);
}

static void	replace_tilde(t_main *main_s, t_token *current)
{
	char	*str;
	char	*new_value;
	int		i;

	if (!current || !current->value)
		return ;
	str = current->value;
	i = 0;
	if (str[i] == '~' && str[i + 1] == '\0')
	{
		ft_free(&current->value);
		current->value = change_tilde(main_s);
		return ;
	}
	else if (str[i] == '~' && str[i + 1] == '/')
	{
		new_value = change_tilde_before(main_s, current);
		if (new_value)
		{
			ft_free(&current->value);
			current->value = new_value;
		}
		return ;
	}
}

void	tilde_replace(t_main *main_s, t_token *first)
{
	t_token	*current;
	t_token	*next;

	if (!first)
		return ;
	current = first;
	while (current)
	{
		next = current->next;
		if (current->type == WORD || current->type == QUOTE)
			replace_tilde(main_s, current);
		current = next;
	}
}
