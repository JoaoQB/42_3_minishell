/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_built_ins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:47:34 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/16 11:41:47 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	run_export(t_pipex *pipex_s)
{
	t_env	*new;
	int		i;
	int		exit_status;

	exit_status = 0;
	if (!pipex_s)
		return (1);
	if (!pipex_s->cmd[1]) //do i need to protect $SHELL?
	{
		print_export(minishell()->export);
		return (exit_status);
	}
	i = 1;
	while (pipex_s->cmd[i])
	{
		new = ft_export_new(pipex_s->cmd[i]);
		if (new)
			export_check(new);
		else
			exit_status = 1;
		i++;
	}
	return (exit_status);
}

void	run_unset(t_pipex *pipex_s)
{
	int		i;

	i = 1;
	if (!pipex_s->cmd[1] || ft_strcmp(pipex_s->cmd[1], "SHELL") == 0)
		return ;
	while (pipex_s->cmd[i])
	{
		env_unset(pipex_s->cmd[i]);
		export_unset(pipex_s->cmd[i]);
		i++;
	}
}

//TODO Handle error n
char	*run_pwd(bool print)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (perror("getcwd() error"), NULL);
	if (print == true)
		printf("%s\n", cwd);
	return (ft_strdup((char *)cwd));
}

//TODO Handle error s
int	run_cd(t_pipex *pipex_s)
{
	char	*new_dir;
	char	**cmd;

	cmd = pipex_s->cmd;
	if (cmd[1] && cmd[2])
		return (print_err("%s: %s\n", cmd[0], "too many arguments"), 1);
	if (!cmd[1] || strcmp(cmd[1], "~") == 0)
		new_dir = ft_strdup(ft_getenv("HOME"));
	else if (strcmp(cmd[1], "-") == 0)
	{
		new_dir = ft_strdup(ft_getenv("OLDPWD"));
		if (new_dir)
			printf("%s\n", new_dir);
	}
	else
		new_dir = ft_strdup(cmd[1]);
	ft_setenv("OLDPWD", run_pwd(false), 1);
	if (new_dir && !(chdir(new_dir) == 0))
		return (free(new_dir),
			print_err("%s: %s: %s\n", cmd[0], cmd[1], strerror(errno)), 1);
	ft_setenv("PWD", run_pwd(false), 1);
	free(new_dir);
	return (0);
}

void	run_echo(t_pipex *pipex_s)
{
	int	i;
	int	nl;

	if (!pipex_s->cmd || pipex_s->pipe_fd[1] == -1)
		return ;
	nl = 1;
	i = 1;
	while (pipex_s->cmd[i])
	{
		if (i == 1 && ft_strcmp(pipex_s->cmd[i], "-n") == 0)
		{
			i++;
			nl = 0;
			if (!pipex_s->cmd[i])
				return ;
		}
		ft_putstr_fd(pipex_s->cmd[i], 1);
		if (pipex_s->cmd[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (nl)
		ft_putstr_fd("\n", 1);
}
