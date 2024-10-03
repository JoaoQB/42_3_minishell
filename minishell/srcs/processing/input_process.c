/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 09:36:26 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/03 12:34:26 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	token_flag_empty(t_token *first)
{
	t_token	*current;

	if (!first)
		return ;
	current = first;
	while (current)
	{
		if (current->value && !*current->value)
			current->type = EMPTY;
		current = current->next;
	}
}

static void	concatenate_tokens(char *result, t_token *first)
{
	t_token	*current;
	int		len;
	int		value_len;

	current = first;
	*result = '\0';
	while (current)
	{
		if (current->value)
		{
			len = ft_strlen(result);
			value_len = ft_strlen(current->value);
			ft_strlcpy(result + len, current->value, value_len + 1);
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
	result = (char *)safe_malloc(sizeof(char) *(total_len + 1));
	concatenate_tokens(result, first);
	return (result);
}

char	*input_and_tokens_reorg()
{
	char	*input_reorg;

	if (minishell()->silence_info)
		return (NULL);
	minishell()->tokens = token_separate_operator();
	if (!first_pars(minishell()->tokens))
		return (NULL);
	tilde_replace(minishell()->tokens);
	quotes_hide_operators(minishell()->tokens);
	var_swap(&minishell()->tokens);
	token_split_words(&minishell()->tokens);
	quotes_remover(minishell()->tokens);
	quotes_revert_operators(minishell()->tokens);
	clean_tokens(&minishell()->tokens);
	if (!minishell()->tokens)
	{
		minishell()->silence_info = true;
		return (NULL);
	}
	tokenize_smarter(minishell()->tokens);
	token_flag_empty(minishell()->tokens);
	input_reorg = concat_tokens_to_char(minishell()->tokens);
	if (!input_reorg)
		return (NULL);
	return (input_reorg);
}

void	input_process(char *user_input)
{
	minishell()->input_trim = input_trim_and_tokenize(user_input);
	if (!minishell()->input_trim || !*minishell()->input_trim)
		minishell()->silence_info = true;
	minishell()->input_reorg = input_and_tokens_reorg();
}
