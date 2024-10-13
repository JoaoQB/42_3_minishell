/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:49:20 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/07 15:40:16 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_main(char **envp)
{
	minishell()->env = get_env(envp);
	minishell()->menv = get_array_env();
	minishell()->export = sort_env();
	minishell()->tokens = NULL;
	minishell()->user_input = NULL;
	minishell()->input_trim = NULL;
	minishell()->input_reorg = NULL;
	minishell()->pipex = NULL;
	minishell()->silence_info = false;
	minishell()->status = 0;
	minishell()->history = NULL;
	// minishell()->pipex = ft_init_pipex_s();
}
