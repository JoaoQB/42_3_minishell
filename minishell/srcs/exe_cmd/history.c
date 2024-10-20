/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/20 15:36:26 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_hist	*ft_init_hist_s(void)
{
	t_hist	*hist_s;

	hist_s = (t_hist *)safe_malloc(sizeof(t_hist));
	hist_s->idx = 1;
	hist_s->prev = NULL;
	hist_s->next = NULL;
	hist_s->usr_input = NULL;
	return (hist_s);
}

void	add_to_history(void)
{
	t_hist	*last_s;
	t_hist	*temp_s;

	last_s = minishell()->history;
	while (last_s && last_s->next)
		last_s = last_s->next;
	if (last_s && ft_strcmp(last_s->usr_input, minishell()->user_input) == 0)
		return ;
	add_history(minishell()->user_input);
	if (last_s && last_s->idx == MAX_HIST)
		rt_one_history();
	temp_s = ft_init_hist_s();
	temp_s->prev = last_s;
	temp_s->usr_input = minishell()->user_input;
	minishell()->user_input = NULL;
	if (last_s)
	{
		temp_s->idx = last_s->idx + 1;
		last_s->next = temp_s;
	}
	else
		minishell()->history = temp_s;
}

void	get_history(t_hist *hist_s, int index)
{
	char	*str_token;
	t_hist	*temp_s;

	temp_s = hist_s;
	if (index == -1)
	{
		while (temp_s)
		{
			str_token = temp_s->usr_input;
			if (!str_token)
				break ;
			print_err("%d %s\n", temp_s->idx, str_token);
			temp_s = temp_s->next;
		}
	}
}

void	rt_one_history(void)
{
	t_hist	*temp_s;

	if (!minishell()->history)
		return ;
	temp_s = minishell()->history;
	minishell()->history = temp_s->next;
	ft_free(&temp_s->usr_input);
	free(temp_s);
	temp_s = minishell()->history;
	while (temp_s)
	{
		temp_s->idx -= 1;
		temp_s = temp_s->next;
	}
}

void	free_history(void)
{
	t_hist	*temp_s;

	while (minishell()->history)
	{
		temp_s = minishell()->history;
		minishell()->history = temp_s->next;
		ft_free(&temp_s->usr_input);
		free(temp_s);
	}
	rl_clear_history();
}
