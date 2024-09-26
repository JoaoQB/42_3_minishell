/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 18:21:08 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/25 11:54:12 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_free(char **str)
{
	if (str && *str)
	{
		free(*str);
		*str = NULL;
	}
}

// TODO DELETE
void	free_triple_array(char ***array)
{
	int	i;

	i = 0;
	if (array != NULL)
	{
		while (array[i])
		{
			free_double_array(array[i]);
			i++;
		}
		free(array);
	}
}

void	free_double_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array[i]);
	array[i] = NULL;
	free(array);
	array = NULL;
}

void	cleanup_main()
{
	if (minishell()->env)
		free_env(&minishell()->env);
	if (minishell()->menv)
		free_double_array(minishell()->menv);
	if (minishell()->export)
		free_env(&minishell()->export);
	if (minishell()->history)
		free_history(minishell()->history);
	minishell()->history = NULL;
}

void	free_main_input()
{
	if (minishell()->tokens)
		free_tokens(&minishell()->tokens);
	if (minishell()->user_input)
		ft_free(&minishell()->user_input);
	if (minishell()->input_trim)
		ft_free(&minishell()->input_trim);
	if (minishell()->input_reorg)
		ft_free(&minishell()->input_reorg);
	if (minishell()->pipex)
		free_pipex_s(&minishell()->pipex);
	minishell()->tokens = NULL;
	minishell()->user_input = NULL;
	minishell()->input_trim = NULL;
	minishell()->input_reorg = NULL;
	minishell()->pipex = NULL;
	minishell()->silence_info = false;
}
