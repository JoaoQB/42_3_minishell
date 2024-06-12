/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:49:20 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/06/11 12:30:31 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_main	*init_main(t_main *main_s, char **envp)
{
	t_main	*main_init;

	main_init = main_s;
	main_init->menv = get_env(envp);
	main_init->tokens = NULL;
	return (main_init);
}

	// if (main_init->menv)
	// {
	// 	printf("menv array created successfully.\n");
	// 	for (int i = 0; main_init->menv[i] != NULL; i++)
	// 	{
	// 		printf("menv[%d]: %s\n", i, main_init->menv[i]);
	// 	}
	// }
	// else
	// 	printf("failed to create menv\n");
