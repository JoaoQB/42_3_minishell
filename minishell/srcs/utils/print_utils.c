/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:06:07 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/08/27 10:05:18 by jqueijo-         ###   ########.fr       */
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
            case QUOTE: type_str = "QUOTE"; break;
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
        printf("Token: '%s', Type: %s, %d, Index: '%d'\n", current->value, type_str, current->type, current->index);
        current = current->next;
    }
}

void	print_cmd_array(char ***cmd)
{
	int	i;
	int	j;

	if (!cmd)
	{
		printf("Command array is NULL\n");
		return ;
	}
	i = 0;
	while (cmd[i] != NULL)
	{
		printf("Command %d\n", i);
		j = 0;
		while (cmd[i][j] != NULL)
		{
			if (j == 0)
				printf("CMD: '%s'\n", cmd[i][j]);
			else if (j > 0)
				printf("ARG: '%s'\n", cmd[i][j]);
			j++;
		}
		i++;
	}
}
