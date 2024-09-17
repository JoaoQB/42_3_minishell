/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 14:56:03 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/17 12:57:17 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	invalid_exit(t_pipex *pipex, int *status, int flag)
{
	char	**cmd;

	if (!pipex || !pipex->cmd)
		return ;
	cmd = pipex->cmd;
	if (flag == 1)
	{
		ft_putstr_fd("exit\nminishell: ", 2);
		ft_putstr_fd(cmd[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		*status = 2;
	}
	else if (flag == 2)
	{
		ft_putstr_fd("exit\nminishell: too many arguments\n", 2);
		pipex->status = 1;
	}
}

static bool	is_valid_status(char *str)
{
	int		i;
	double	status;

	if (!str)
		return (true);
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while(str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	status = ft_atoi_dbl(str);
	if (status > INT_MAX || status < INT_MIN)
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
	printf("status: %d\n", pipex->status);
	if (!is_valid_status(cmd[1]))
		invalid_exit(pipex, &status, 1);
	else if (cmd[1])
	{
		if (cmd[2])
		{
			invalid_exit(pipex, &status, 2);
			return ;
		}
		status = ft_atoi(cmd[1]);
		if (status < 0 || status > 255)
			status = (status % 256 + 256) % 256;
	}
	free_main_input(pipex->main_s);
	cleanup_main(pipex->main_s);
	exit(status);
}