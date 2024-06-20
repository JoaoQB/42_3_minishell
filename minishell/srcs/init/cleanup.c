/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 18:21:08 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/06/17 13:04:43 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tokens(t_token *token)
{
	t_token	*temp;

	if (!token)
		return ;
	while (token)
	{
		temp = token;
		token = token->next;
		if (temp->value)
			free (temp->value);
		free (temp);
	}
}

void	free_main_input(t_main *main_s)
{
	if (main_s)
	{
		if (main_s->user_input)
			free(main_s->user_input);
		if (main_s->input_trim)
			free(main_s->input_trim);
	}
	if (main_s->tokens)
		free_tokens(main_s->tokens);
	// free (main_s);
}

void	free_double_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	cleanup_main(t_main *main_s)
{
	if (main_s)
	{
		if (main_s->menv)
			free_double_array(main_s->menv);
		// free(main_s->user_input);
		// free(main_s->input_trim);
	}
	free (main_s);
}
