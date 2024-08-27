/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 09:36:26 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/08/27 10:05:00 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	concatenate_tokens(char *result, t_token *first)
{
	t_token	*current;

	current = first;
	*result = '\0';
	while (current)
	{
		ft_strcat(result, current->value);
		if (current->next)
			ft_strcat(result, " ");
		current = current->next;
	}
}

char	*concat_tokens(t_token *first)
{
	char	*result;
	int		total_len;
	t_token	*current;

	if (!first)
		return (NULL);
	current = first;
	total_len = 0;
	while (current)
	{
		total_len += ft_strlen(current->value) + 1;
		current = current->next;
	}
	result = (char *)malloc(sizeof(char) * total_len);
	if (!result)
		return (NULL);
	concatenate_tokens(result, first);
	return (result);
}

char	*reorg_input(t_main *main_s)
{
	char	*input_reorg;

	if (main_s->silence_info)
		return (NULL);
	main_s->tokens = tokenize_refine_word(main_s);
	if (!first_pars(main_s, main_s->tokens))
		return (NULL);
	// var_swap(main_s->tokens); //TODO
	quotes_remover(main_s->tokens);
	tokenize_smarter(main_s->tokens);
	input_reorg = concat_tokens(main_s->tokens);
	if (!input_reorg)
		return (NULL);
	return (input_reorg);
}

void	process_input(t_main *main_s, char *user_input)
{
	main_s->input_trim = trim_input(main_s, user_input);
	if (main_s->silence_info)
		return ;
	main_s->input_reorg = reorg_input(main_s);
	if (main_s->silence_info)
		return ;
	main_s->size = count_cmd_size(main_s->tokens);
	main_s->cmd = create_cmd_array(main_s);
}
