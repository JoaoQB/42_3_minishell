/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:49:20 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/08/27 11:09:31 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_main(t_main *main_s, char **envp)
{
	main_s->env = get_env(envp);
	main_s->menv = (envp);
	// print_env(main_s->env);
	main_s->tokens = NULL;
	main_s->user_input = NULL;
	main_s->input_trim = NULL;
	main_s->input_reorg = NULL;
	main_s->cmd = NULL;
	main_s->size = -1;
	main_s->exe_fd[0] = -1;
	main_s->exe_fd[1] = -1;
	main_s->fd_pipeline[0] = NULL;
	main_s->fd_pipeline[1] = NULL;
	main_s->pid_pipeline = NULL;
	main_s->silence_info = false;
	// main_s->next = NULL;
}
