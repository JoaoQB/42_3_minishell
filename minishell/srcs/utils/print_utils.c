/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:06:07 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/07 15:46:26 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_menv(char **menv)
{
	int	i;

	if (!menv)
		return ;
	i = 0;
	while (menv[i])
	{
		printf("my array env: %s\n", menv[i]);
		i++;
	}
}

void	print_env(t_env *env)
{
	t_env	*current;

	if (!env)
		return ;
	current = env;
	while (current != NULL)
	{
		if (current->value)
			printf("\nmy env list env: %s\n", current->value);
		if (current->var)
			printf("env_var: %s\n", current->var);
		if (current->var_value)
			printf("env_var_value: %s\n", current->var_value);
		current = current->next;
	}
}

void	print_tokens(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
	{
		printf("no tokens\n");
		return ;
	}
	current = tokens;
	while (current != NULL)
	{
		if (current->value)
			printf("Token: '%s', Type: %d\n", current->value, current->type);
		else
			printf("Token: 'no value', Type: %d\n", current->type);
		current = current->next;
	}
}
