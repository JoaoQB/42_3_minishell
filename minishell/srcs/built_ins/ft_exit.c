/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 14:56:03 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/23 11:26:46 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	int			i;
	long long	status;

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
	errno = 0;
	status = ft_atoi_lng(str);
	if (errno == ERANGE || status > LLONG_MAX || status < LLONG_MIN)
		return (false);
	return (true);
}

void	ft_exit_builtins(t_pipex *pipex)
{
	long long	long_status;
	int			status;
	char		**cmd;

	if (!pipex || !pipex->cmd)
		return ;
	cmd = pipex->cmd;
	status = 0;
	if (cmd[1] && !is_valid_status(cmd[1]))
		invalid_exit(pipex, 1);
	else if (cmd[1])
	{
		if (cmd[2])
			return (invalid_exit(pipex, 2));
		long_status = ft_atoi_lng(cmd[1]);
		if (long_status < 0 || long_status > 255)
			long_status = (long_status % 256 + 256) % 256;
		pipex->status = long_status;
	}
	status = pipex->status;
	ft_exit(status);
}

void	ft_exit(int status)
{
	free_main_input();
	cleanup_main();
	exit(status);
}

void	ft_exit_pid(t_pipex *pipex)
{
	int		status;

	if (!pipex)
		return ;
	status = minishell()->status;
	if (status == 0)
		status = pipex->status;
	ft_exit(status);
}
