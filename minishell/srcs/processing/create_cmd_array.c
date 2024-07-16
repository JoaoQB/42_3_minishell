/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:11:02 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/07/16 17:06:39 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	fill_cmd_args(t_token *current, char **cmd_args)
{
	int	i;

	i = 0;
	cmd_args[i] = ft_strdup(current->value);
	i++;
	current = current->next;
	while (current && current->type != CMD)
	{
		if (current->type == ARG)
		{
			cmd_args[i] = ft_strdup(current->value);
			i++;
		}
		current = current->next;
	}
	cmd_args[i] = NULL;
}

static int	count_arg_size(t_token *target_cmd)
{
	t_token	*current;
	int		arg_size;

	current = target_cmd->next;
	arg_size = 1;
	while (current && current->type != CMD)
	{
		if (current->type == ARG)
			arg_size++;
		current = current->next;
	}
	return (arg_size);
}

static bool	alloc_cmd_args(t_main *main_s, char ***cmd_array)
{
	t_token	*current;
	int		arg_size;
	int		cmd_index;

	current = main_s->tokens;
	cmd_index = 0;
	while (cmd_index < main_s->size)
	{
		while (current && current->type != CMD)
			current = current->next;
		arg_size = count_arg_size(current);
		cmd_array[cmd_index] = (char **)malloc(sizeof(char *) * (arg_size + 1));
		if (!cmd_array[cmd_index])
		{
			free_triple_array(cmd_array);
			return (false);
		}
		fill_cmd_args(current, cmd_array[cmd_index]);
		current = current->next;
		cmd_index++;
	}
	cmd_array[cmd_index] = NULL;
	return (true);
}

char	***create_cmd_array(t_main *main_s)
{
	char	***cmd_array;
	int		cmd_size;

	cmd_size = main_s->size;
	if (cmd_size <= 0)
		return (NULL);
	cmd_array = (char ***)malloc(sizeof(char **) * (cmd_size + 1));
	if (!cmd_array)
		return (NULL);
	if (!alloc_cmd_args(main_s, cmd_array))
		return (NULL);
	return (cmd_array);
}
