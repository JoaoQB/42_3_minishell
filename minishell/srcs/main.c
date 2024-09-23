/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:42:25 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/21 09:13:03 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t g_signal; // Definition

static void	repl(t_main *main_s, char **envp)
{
	// rl_catch_signals = 0;
	init_main(main_s, envp);
	while (1)
	{
		main_s->user_input = readline("minishell-> ");
		if (!main_s->user_input)
			break ;
		input_process(main_s, main_s->user_input);
		//print_tokens(main_s->tokens);
		ft_shell_pipex(main_s);
		free_main_input(main_s);
	}
}

char *alt_get_path()
{
	pid_t pid;
	int *piper;
	piper = pipe;
	pid = fork();
	if (pid == -1)
		return(perror("fork"), NULL);
	else if (pid == 0)
	{
		dup2(piper[1], STDOUT_FILENO);
		close_all_fd(piper[1]);
		if (execve("/bin/ps", exec_args, NULL) == -1)
			return (-1);
	}
}

pid_t ft_getpid()
{
	pid_t pid;
	int *piper;
	char *exec_args[6];
	char *path;

	piper = pipe;
	pid = fork();
	if (pid == -1)
		perror("fork");
	else if (pid == 0)
	{
		exec_args[0] = ;
		exec_args[1] = "ps";
		exec_args[2] = "-o";
		exec_args[3] = "ppid=";
		exec_args[4] = "-p";
		exec_args[6] = NULL;
		exec_args[5] = ft_itoa(pid);
		dup2(piper[1], STDOUT_FILENO);
		close_all_fd(piper[1]);
		if (execve("/bin/ps", exec_args, NULL) == -1)
			return (-1);
	}
	while 
}

int	main(int argc, char **argv, char **envp)
{
	t_main	main_s;
	pid_t main_pid;

	g_signal = 0;
	(void)argv;
	if (argc != 1)
		return (1);
	main_pid = ft_getpid();
	printf("main pid: %d", main_pid);
	main_pid = get_pid();
	printf("main pid: %d", main_pid);
	// if (set_sig_handlers(SIGINT, handle_sigint, ctx) != 0)
	if (set_sig_handlers(SIGINT, handle_sigint) != 0)
        return (1);
	if (set_sig_handlers(SIGQUIT, handle_sigquit) != 0)
        return (1);
	repl(&main_s, envp);
	cleanup_main(&main_s);
	return (0);
}

int set_signals(t_main	*main_s)
{
	// struct union	sigval value;

	// value.sival_ptr = main_s;
	// if (set_sig_handlers(SIGINT, handle_sigint, ctx) != 0)
	if (set_sig_handlers(SIGINT, handle_sigint, main_s) != 0)
		return (1);
	if (set_sig_handlers(SIGQUIT, handle_sigquit, main_s) != 0)
		return (1);
	// if (set_sig_handlers(SIGQUIT, handle_sigquit, main_s) != 0) //TODO sigusr
	return (1);
}

// int virtual_main(t_main	*main_s, char **envp)
// {
// 	g_signal = 0;
// 	init_main(main_s, envp);
// 	set_signals(main_s);
// 	main_s->pid = fork();
// 	if (main_s->pid == -1)
// 		return (perror("fork failed"), 1); //TODO Handle error s
// 	else if (main_s->pid == 0)
// 		repl(main_s);
// 	return (0);
// }
// int	main(int argc, char **argv, char **envp)
// {
// 	t_main	main_s;

// 	(void)argv;
// 	if (argc != 1)
// 		return (1);
// 	virtual_main(&main_s, envp);
// 	cleanup_main(&main_s);
// 	return (0);
// }
