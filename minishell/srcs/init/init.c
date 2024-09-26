/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:49:20 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/25 11:54:41 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_main(char **envp)
{
	minishell()->env = get_env(envp);
	// print_env(minishell()->env);
	minishell()->menv = get_array_env();
	// print_menv(minishell()->menv);
	minishell()->export = sort_env();
	minishell()->tokens = NULL;
	minishell()->user_input = NULL;
	minishell()->input_trim = NULL;
	minishell()->input_reorg = NULL;
	minishell()->pipex = NULL;
	minishell()->silence_info = false;
	minishell()->status = 0;
	minishell()->history = NULL;
	minishell()->pipex = ft_init_pipex_s();
}
