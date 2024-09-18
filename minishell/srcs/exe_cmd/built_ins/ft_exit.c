/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 14:56:03 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/18 17:45:13 by fandre-b         ###   ########.fr       */
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
		ft_putstr_fd("exit\nminishell: ", 2);
		ft_putstr_fd(cmd[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		pipex->status = 2;
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

void	ft_exit(t_pipex *pipex_s)
{
	int		status;
	char	**cmd;
	t_main	*main_s;

	main_s = pipex_s->main_s;
	if (!pipex_s || !pipex_s->cmd)
		return ;
	cmd = pipex_s->cmd;
	status = pipex_s->status;
	if (!is_valid_status(cmd[1]))
		invalid_exit(pipex_s, 1);
	else if (cmd[1])
	{
		if (cmd[2])
			return (invalid_exit(pipex_s, 2));
		status = ft_atoi(cmd[1]);
		if (status < 0 || status > 255)
			status = (status % 256 + 256) % 256;
	}
	pipex_s->status = status;
	free_main_input(main_s);
	cleanup_main(main_s);
	exit(status);
}

void	ft_exit_pid(t_pipex *pipex_s)
{
	int		status;
	t_main	*main_s;

	main_s = pipex_s->main_s;
	if (!pipex_s || !pipex_s->cmd)
		return ;
	status = pipex_s->status;
	free_main_input(main_s);
	cleanup_main(main_s);
	exit(status);
}
