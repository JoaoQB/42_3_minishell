/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 09:36:26 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/07/14 16:25:08 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void print_tokens(t_token *tokens)
{
    t_token *current = tokens;
    while (current != NULL)
    {
        const char *type_str;
        switch (current->type)
        {
            case RED_IN: type_str = "RED_IN"; break;
            case RED_OUT: type_str = "RED_OUT"; break;
            case HERE_DOC: type_str = "HERE_DOC"; break;
            case RED_OUT_APP: type_str = "RED_OUT_APP"; break;
            case PIPE: type_str = "PIPE"; break;
            case QT_SIN: type_str = "QT_SIN"; break;
            case QT_DBL: type_str = "QT_DBL"; break;
            case WORD: type_str = "WORD"; break;
            case CMD: type_str = "CMD"; break;
            case ARG: type_str = "ARG"; break;
            case PATH: type_str = "PATH"; break;
            case DELIM: type_str = "DELIM"; break;
            // Add new token types here
            // case NEW_TYPE_1: type_str = "NEW_TYPE_1"; break;
            // case NEW_TYPE_2: type_str = "NEW_TYPE_2"; break;
            default: type_str = "UNKNOWN";
        }
        printf("Token: '%s', Type: %s, Index: '%d'\n", current->value, type_str, current->index);
        current = current->next;
    }
}

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

	main_s->tokens = tokenize_refine_word(main_s);
	printf("Words reorganized tokens:\n");
	print_tokens(main_s->tokens);
	if (!first_pars(main_s, main_s->tokens))
		return (NULL);
	tokenize_smarter(main_s->tokens);
	printf("Smarter Tokens:\n");
	print_tokens(main_s->tokens);
	input_reorg = concat_tokens(main_s->tokens);
	if (!input_reorg)
		return (NULL);
	return (input_reorg);
}

void	process_input(t_main *main_s, char *user_input)
{
	main_s->input_trim = trim_input(main_s, user_input);
	printf("Trim Input: '%s'\nTrimmed tokens:\n", main_s->input_trim);
	print_tokens(main_s->tokens);
	main_s->input_reorg = reorg_input(main_s);
	printf("Reorganized input: '%s'\n", main_s->input_reorg);
}
