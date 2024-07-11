/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:49:20 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/07/10 13:35:10 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_main	*init_main(t_main *main_s, char **envp)
{
	t_main	*main_init;

	main_init = main_s;
	if (!main_init->menv)
		main_init->menv = get_env(envp);
	main_init->tokens = NULL;
	main_init->user_input = NULL;
	main_init->input_trim = NULL;
	main_init->input_reorg = NULL;
	main_init->cmd = NULL;
	main_init->size = -1;
	main_init->exe_fd[0] = -1;
	main_init->exe_fd[1] = -1;
	main_init->fd_pipeline[0] = NULL;
	main_init->fd_pipeline[1] = NULL;
	main_init->pid_pipeline = NULL;
	main_init->silence_info = false;
	return (main_init);
}
