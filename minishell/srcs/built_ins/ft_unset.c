/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 18:16:32 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/22 15:18:19 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	env_unset(char *env_var)
{
	t_env	*menv_s;
	t_env	*prev;

	menv_s = minishell()->env;
	prev = NULL;
	while (menv_s && ft_strcmp(menv_s->var, env_var) != 0)
	{
		prev = menv_s;
		menv_s = menv_s->next;
	}
	if (!menv_s)
		return ;
	if (prev)
		prev->next = menv_s->next;
	else
		minishell()->env = menv_s->next;
	ft_free(&menv_s->value);
	ft_free(&menv_s->var_value);
	ft_free(&menv_s->var);
	free(menv_s);
}

void	export_unset(char *export_var)
{
	t_env	*current;
	t_env	*prev;

	current = minishell()->export;
	prev = NULL;
	while (current && ft_strcmp(export_var, current->var) != 0)
	{
		prev = current;
		current = current->next;
	}
	if (!current)
		return ;
	if (prev)
		prev->next = current->next;
	else
		minishell()->export = current->next;
	ft_free(&current->value);
	ft_free(&current->var);
	ft_free(&current->var_value);
	free(current);
}
