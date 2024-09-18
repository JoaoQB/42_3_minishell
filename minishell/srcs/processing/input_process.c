/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 09:36:26 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/18 17:58:36 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	concatenate_tokens(char *result, t_token *first)
{
	t_token	*current;
	int		len;

	current = first;
	*result = '\0';
	while (current)
	{
		if (current->value)
		{
			len = ft_strlen(result);
			ft_strlcpy2(result + len, current->value, ft_strlen(current->value) + 1);
			len += ft_strlen(current->value);
			if (current->next)
			{
				result[len] = ' ';
				len++;
				result[len] = '\0';
			}
		}
		current = current->next;
	}
}

char	*concat_tokens_to_char(t_token *first)
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
		if (current->value)
			total_len += ft_strlen(current->value) + 1;
		current = current->next;
	}
	if (total_len == 0)
		return (NULL);
	result = (char *)safe_malloc(sizeof(char) * (total_len + 1));
	concatenate_tokens(result, first);
	return (result);
}

char	*input_and_tokens_reorg(t_main *main_s)
{
	char	*input_reorg;

	if (main_s->silence_info)
		return (NULL);
	main_s->tokens = token_separate_operator(main_s);
	if (!first_pars(main_s, main_s->tokens))
		return (NULL);
	tilde_replace(main_s, main_s->tokens);
	quotes_hide_operators(main_s->tokens);
	var_swap(main_s, &main_s->tokens);
	token_split_words(&main_s->tokens);
	quotes_remover(main_s->tokens);
	quotes_revert_operators(main_s->tokens);
	clean_tokens(&main_s->tokens);
	if (!main_s->tokens)
	{
		main_s->silence_info = true;
		return (NULL);
	}
	tokenize_smarter(main_s->tokens);
	input_reorg = concat_tokens_to_char(main_s->tokens);
	if (!input_reorg)
		return (NULL);
	return (input_reorg);
}

void	input_process(t_main *main_s, char *user_input)
{
	main_s->input_trim = input_trim_and_tokenize(main_s, user_input);
	if (!main_s->input_trim || !*main_s->input_trim)
		main_s->silence_info = true;
	main_s->input_reorg = input_and_tokens_reorg(main_s);
}
