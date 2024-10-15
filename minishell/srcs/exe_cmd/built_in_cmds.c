/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/15 20:33:08 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	edge_cases(t_pipex *pipex_s)
{
	if (!pipex_s || !pipex_s->cmd || !pipex_s->cmd[0])
		return (0);
	if (pipex_s->status)
		return (0);
	if (ft_strcmp(pipex_s->cmd[0], "history") == 0)
		get_history(minishell()->history, -1);
	else if (ft_strcmp(pipex_s->cmd[0], "pwd") == 0)
		free(run_pwd(true));
	else if (ft_strcmp(pipex_s->cmd[0], "echo") == 0)
		run_echo(pipex_s);
	else if (ft_strcmp(pipex_s->cmd[0], "env") == 0)
		my_print_env();
	else
		return (0);
	return (1);
}

int	special_edge_cases(t_pipex *pipex_s)
{
	if (!pipex_s || !pipex_s->cmd || !pipex_s->cmd[0])
		return (0);
	if (pipex_s->status)
		return (0);
	if (ft_strcmp(pipex_s->cmd[0], "cd") == 0)
		pipex_s->status = run_cd(pipex_s);
	else if (ft_strcmp(pipex_s->cmd[0], "exit") == 0)
		ft_exit(pipex_s);
	else if (ft_strcmp(pipex_s->cmd[0], "unset") == 0)
		run_unset(pipex_s);
	else if (ft_strcmp(pipex_s->cmd[0], "export") == 0)
		pipex_s->status = run_export(pipex_s);
	else
		return (0);
	return (1);
}
