/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:42:25 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/06/12 10:47:36 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	repl(t_main *main_s, char **envp)
{
	int	i;

	i = -1;
	main_s = init_main(main_s, envp);
	while (++i < 5)
	// while (1)
	{
		main_s->user_input = readline("minishell-> ");
		printf("User Input: '%s'\n", main_s->user_input);
		process_input(main_s, main_s->user_input);
		free_main_input(main_s);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_main	*main_s;

	(void)argv;
	if (argc != 1)
		return (1);
	main_s = malloc(sizeof(t_main));
	if (!main_s)
		return (1);
	// main_s = init_main(main_s, envp);
	repl(main_s, envp);
	cleanup_main(main_s);
	return (0);
}
