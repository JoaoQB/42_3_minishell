/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 14:56:03 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/18 17:11:24 by jqueijo-         ###   ########.fr       */
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
		pipex->main_s->status = 2;
	}
	else if (flag == 2)
	{
		ft_putstr_fd("exit\nminishell: too many arguments\n", 2);
		pipex->main_s->status = 1;
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
	t_main	*main_s;

	main_s = pipex->main_s;
	if (!pipex || !pipex->cmd)
		return ;
	cmd = pipex->cmd;
	status = main_s->status;
	if (!is_valid_status(cmd[1]))
		invalid_exit(pipex, 1);
	else if (cmd[1])
	{
		if (cmd[2])
			return (invalid_exit(pipex, 2));
		main_s->status = ft_atoi(cmd[1]);
		if (main_s->status < 0 || main_s->status > 255)
			main_s->status = (main_s->status % 256 + 256) % 256;
	}
	status = main_s->status;
	free_main_input(main_s);
	cleanup_main(main_s);
	exit(status);
}

void	ft_exit_pid(t_pipex *pipex)
{
	int		status;
	t_main	*main_s;

	main_s = pipex->main_s;
	if (!pipex || !pipex->cmd)
		return ;
	status = pipex->main_s->status;
	free_main_input(main_s);
	cleanup_main(main_s);
	exit(status);
}
