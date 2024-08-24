/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juka <juka@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:42:25 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/08/24 18:09:57 by juka             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	repl(t_main *main_s, char **envp)
{
	// int	i;

	// i = -1;
	init_main(main_s, envp);
	//while (++i < 5)
	while (1)
	{
		main_s->user_input = readline("minishell-> ");
		process_input(main_s, main_s->user_input);
		//print_tokens(main_s->tokens);
		ft_shell_pipex(main_s);
		free_main_input(main_s);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_main	main_s;

	(void)argv;
	if (argc != 1)
		return (1);
	repl(&main_s, envp);
	cleanup_main(&main_s);
	return (0);
}
