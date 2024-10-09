/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 18:21:08 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/09 17:33:34 by fandre-b         ###   ########.fr       */
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

void	close_all_fd(t_pipex *pipex_s)
{
	t_pipex *save;

	save = NULL;
	while (pipex_s->prev)
		pipex_s = pipex_s->prev;
	while (pipex_s->next)
	{
		if (pipex_s != save)
		{
			ft_close(pipex_s->pipe_fd[0]);
			ft_close(pipex_s->pipe_fd[1]);
		}
		pipex_s = pipex_s->next;
	}
}

void	free_pipex_node(t_pipex *pipex_s)
{
	ft_close(pipex_s->pipe_fd[0]);
	ft_close(pipex_s->pipe_fd[1]);
	if (pipex_s == minishell()->pipex)
	{
		if (pipex_s->next)
		{
			pipex_s->next->prev = NULL;
			minishell()->pipex = pipex_s->next;
		}
		else
			minishell()->pipex = NULL;
	}
	else if (pipex_s->prev)
	{
		pipex_s->prev->next = pipex_s->next;
		if (pipex_s->next)
			pipex_s->next->prev = pipex_s->prev;
	}
	free_double_array(pipex_s->cmd);
	free(pipex_s->path);
	free(pipex_s);
}


void free_pipex_s(void)
{
	t_pipex *temp;

	close_all_fd(minishell()->pipex);
	while (minishell()->pipex)
	{
		temp = minishell()->pipex;
		minishell()->pipex = temp->next;
		free_double_array(temp->cmd);
		free(temp->path);
		free(temp);
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
		free_history();
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
		free_pipex_s();
	minishell()->tokens = NULL;
	minishell()->user_input = NULL;
	minishell()->input_trim = NULL;
	minishell()->input_reorg = NULL;
	minishell()->pipex = NULL;
	minishell()->silence_info = false;
}
