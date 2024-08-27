/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 18:21:08 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/08/27 11:13:13 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		i++;
	}
	free(array);
}
void	cleanup_main(t_main *main_s)
{
	if (main_s->menv)
		free_double_array(main_s->menv);
	if (main_s->env)
		free_env(main_s->env);
}

void	free_main_input(t_main *main_s)
{
	if (main_s->tokens)
		free_tokens(main_s->tokens);
	if (main_s->user_input)
		free(main_s->user_input);
	if (main_s->input_trim)
		free(main_s->input_trim);
	if (main_s->input_reorg)
		free(main_s->input_reorg);
	if (main_s->cmd)
		free_triple_array(main_s->cmd);
	main_s->size = -1;
	main_s->exe_fd[0] = -1;
	main_s->exe_fd[1] = -1;
	main_s->fd_pipeline[0] = NULL;
	main_s->fd_pipeline[1] = NULL;
	main_s->pid_pipeline = NULL;
	main_s->silence_info = false;
}
