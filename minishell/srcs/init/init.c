/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:49:20 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/24 15:50:09 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_main(t_main *main_s, char **envp)
{
	main_s->env = get_env(envp);
	// print_env(main_s->env);
	main_s->menv = get_array_env(main_s);
	// print_menv(main_s->menv);
	main_s->export = sort_env(main_s);
	// main_s->export = NULL;
	main_s->tokens = NULL;
	main_s->user_input = NULL;
	main_s->input_trim = NULL;
	main_s->input_reorg = NULL;
	main_s->pipex = NULL;
	main_s->silence_info = false;
	main_s->status = 0;
	main_s->history = NULL;
	main_s->pipex = ft_init_pipex_s(main_s);
}
