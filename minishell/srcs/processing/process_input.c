/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 09:36:26 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/06/12 11:12:46 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// const char *token_type_to_string(int type)
// {
//     switch (type)
//     {
//         case RED_IN: return "RED_IN";
//         case RED_OUT: return "RED_OUT";
//         case DELIM: return "DELIM";
//         case RED_OUT_APP: return "RED_OUT_APP";
//         case PIPE: return "PIPE";
//         case EXP: return "EXP";
//         case QT_SIN: return "QT_SIN";
//         case QT_DBL: return "QT_DBL";
//         case WORD: return "WORD";
//         default: return "UNKNOWN";
//     }
// }

// void print_tokens(t_token *tokens)
// {
//     t_token *current = tokens;
//     while (current != NULL)
//     {
//         printf("Token: '%s', Type: %s\n", current->value,
// 			token_type_to_string(current->type));
//         current = current->next;
//     }
// }

void	process_input(t_main *main_s, char *user_input)
{
	// if (main_s->input_trim)
	main_s->input_trim = trim_input(main_s, user_input);
	printf("Trim Input: '%s'\n", main_s->input_trim);
	print_tokens(main_s->tokens);
}
