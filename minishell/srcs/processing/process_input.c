/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 09:36:26 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/07/10 13:45:39 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

const char *token_type_to_string(int type)
{
    switch (type)
    {
        case RED_IN: return "RED_IN";
        case RED_OUT: return "RED_OUT";
        case DELIM: return "DELIM";
        case RED_OUT_APP: return "RED_OUT_APP";
        case PIPE: return "PIPE";
        case QT_SIN: return "QT_SIN";
        case QT_DBL: return "QT_DBL";
        case WORD: return "WORD";
        default: return "UNKNOWN";
    }
}

void print_tokens(t_token *tokens)
{
    t_token *current = tokens;
    while (current != NULL)
    {
        printf("Token: '%s', Type: %s, Index: '%d'\n", current->value,
			token_type_to_string(current->type), current->index);
        current = current->next;
    }
}

char	*reorg_input(t_main *main_s)
{
	// char	*reorg;

	main_s->tokens = tokenize_refine_word(main_s);
	print_tokens(main_s->tokens);
	if (!first_pars(main_s, main_s->tokens))
		return (NULL);
	// tokenize_smarter(main_s->tokens);
	return (NULL);
}

void	process_input(t_main *main_s, char *user_input)
{
	main_s->input_trim = trim_input(main_s, user_input);
	printf("Trim Input: '%s'\n", main_s->input_trim);
	print_tokens(main_s->tokens);
	main_s->input_reorg = reorg_input(main_s);
}
