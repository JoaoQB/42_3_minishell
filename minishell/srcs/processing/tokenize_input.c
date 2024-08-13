/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:11:02 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/08/13 18:59:07 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	append_token(t_token **first, t_token *new_node)
{
	t_token	*last;

	if (*first == NULL)
	{
		*first = new_node;
		return ;
	}
	last = *first;
	while (last->next != NULL)
		last = last->next;
	last->next = new_node;
}

int	token_assign(t_token *token)
{
	if (!token)
		return (-1);
	else if (!ft_strncmp(token->value, "<", 2) && token->value[1] == '\0')
		return (RED_IN);
	else if (!ft_strncmp(token->value, ">", 2) && token->value[1] == '\0')
		return (RED_OUT);
	else if (!ft_strncmp(token->value, "<<", 3) && token->value[2] == '\0')
		return (HERE_DOC);
	else if (!ft_strncmp(token->value, ">>", 3) && token->value[2] == '\0')
		return (RED_OUT_APP);
	else if (!ft_strncmp(token->value, "|", 2) && token->value[1] == '\0')
		return (PIPE);
	else if (find_quotes(token->value))
		return (QUOTE);
	else
		return (WORD);
}

static t_token	*ft_token_new(char **words, int i)
{
	t_token	*new_token;
	int		word_len;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	word_len = ft_strlen(words[i]);
	new_token->value = (char *)malloc((word_len + 1) * sizeof(char));
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	ft_strlcpy(new_token->value, words[i], word_len + 1);
	new_token->type = token_assign(new_token);
	new_token->index = i;
	new_token->next = NULL;
	new_token->cmd = NULL;
	return (new_token);
}

t_token	*tokenize_input(char **words)
{
	t_token	*new_token;
	t_token	*first;
	int		word_count;
	int		i;

	first = NULL;
	word_count = count_arrays(words);
	i = 0;
	while (i < word_count)
	{
		new_token = ft_token_new(words, i);
		if (!new_token)
		{
			if (first)
				free_tokens(first);
			free_double_array(words);
			return (NULL);
		}
		append_token(&first, new_token);
		i++;
	}
	return (first);
}
