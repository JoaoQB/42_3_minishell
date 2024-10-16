/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 14:56:03 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/16 15:36:53 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	invalid_exit(t_pipex *pipex, int flag)
{
	char	**cmd;

	if (!pipex || !pipex->cmd)
		return ;
	cmd = pipex->cmd;
	if (flag == 1)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		pipex->status = 2;
	}
	else if (flag == 2)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		pipex->status = 1;
	}
}

static bool	is_valid_status(char *str)
{
	int		i;

	if (!str)
		return (true);
	i = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	while (str[i] && str[i] == ' ')
		i++;
	if (str[i] && !ft_isdigit(str[i]))
		return (false);
	return (true);
}

void	ft_exit(t_pipex *pipex)
{
	int		status;
	char	**cmd;

	if (!pipex || !pipex->cmd)
		return ;
	cmd = pipex->cmd;
	status = pipex->status;
	if (!is_valid_status(cmd[1]))
		invalid_exit(pipex, 1);
	else if (cmd[1])
	{
		if (cmd[2])
			return (invalid_exit(pipex, 2));
		pipex->status = ft_atoi(cmd[1]);
		if (pipex->status < 0 || pipex->status > 255)
			pipex->status = (pipex->status % 256 + 256) % 256;
	}
	status = pipex->status;
	free_main_input();
	cleanup_main();
	exit(status);
}

void	ft_exit_pid(t_pipex *pipex)
{
	int		status;

	if (!pipex || !pipex->cmd)
		return ;
	status = pipex->status;
	free_main_input();
	cleanup_main();
	exit(status);
}
