/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:11:02 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/20 15:25:35 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_token	*get_last_token(t_token *first)
{
	t_token	*last;

	if (!first)
		return (NULL);
	while (first->next != NULL)
		first = first->next;
	last = first;
	return (last);
}

static void	append_token_back(t_token *first, t_token *new_node)
{
	t_token	*last;

	last = get_last_token(first);
	if (!last)
	{
		first = new_node;
		return ;
	}
	last->next = new_node;
	new_node->prev = last;
}

int	token_assign(char *str)
{
	if (!str)
		return (-1);
	else if (str[0] == '<' && str[1] == '\0')
		return (RED_IN);
	else if (str[0] == '>' && str[1] == '\0')
		return (RED_OUT);
	else if (str[0] == '<' && str[1] == '<' && str[2] == '\0')
		return (HERE_DOC);
	else if (str[0] == '>' && str[1] == '>' && str[2] == '\0')
		return (RED_OUT_APP);
	else if (str[0] == '|' && str[1] == '\0')
		return (PIPE);
	else if (find_quotes(str))
		return (QUOTE);
	else
		return (WORD);
}

static t_token	*ft_token_new(char *str)
{
	t_token	*new_token;
	int		word_len;

	new_token =(t_token *)safe_malloc(sizeof(t_token));
	word_len = ft_strlen(str);
	new_token->value =(char *)safe_malloc((word_len + 1) * sizeof(char));
	ft_strlcpy(new_token->value, str, word_len + 1);
	if (new_token->value)
		new_token->type = token_assign(new_token->value);
	new_token->index = -1;
	new_token->prev = NULL;
	new_token->next = NULL;
	return (new_token);
}

t_token	*input_tokenize(char **words)
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
		new_token = ft_token_new(words[i]);
		if (!new_token)
		{
			free_tokens(&first);
			free_double_array(words);
			return (NULL);
		}
		if (i == 0)
			first = new_token;
		else
			append_token_back(first, new_token);
		i++;
	}
	reindex_tokens(first);
	return (first);
}
