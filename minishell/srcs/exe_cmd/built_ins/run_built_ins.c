/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_built_ins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:47:34 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/07 20:55:36 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	run_export(t_pipex *pipex_s)
{
	t_env	*new;
	int		i;

	if (!pipex_s)
		return ;
	if (!pipex_s->cmd[1])
	{
		print_export(minishell()->export);
		return ;
	}
	i = 1;
	while (pipex_s->cmd[i])
	{
		new = ft_export_new(pipex_s->cmd[i]);
		if (new)
			export_check(new);
		i++;
	}
}

void	run_unset(t_pipex *pipex_s)
{
	t_env	*menv_s;
	t_env	*prev;

	menv_s = minishell()->env;
	if (!pipex_s->cmd[1])
		return ;
	while (menv_s && ft_strcmp(menv_s->var, pipex_s->cmd[1]) != 0)
	{
		prev = menv_s;
		menv_s = menv_s->next;
	}
	if (!menv_s)
		return ;
	prev->next = menv_s->next;
	free(menv_s->value);
	free(menv_s->var_value);
	free(menv_s->var);
	free(menv_s);
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
		// return (printf("%s: %s\n", cmd[0], "too many arguments"), 1);
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
			// printf("%s: %s: %s\n", cmd[0], cmd[1], strerror(errno)), 1);
	ft_setenv("PWD", run_pwd(false), 1);
	free(new_dir);
	return (0);
}

int	run_echo(t_pipex *pipex_s)
{
	int	i;
	int	nl;

	nl = 0;
	i = 1;
	while (pipex_s->cmd[i])
	{
		if (i == 1 && ft_strcmp(pipex_s->cmd[i], "-n") == 0)
		{
			++i;
			nl = 1;
		}
		printf("%s", pipex_s->cmd[i]);
		if (pipex_s->cmd[++i] && *pipex_s->cmd[i])
			printf(" ");
	}
	if (!nl)
		printf("\n");
	return (1);
}
