/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:51:15 by fandre-b          #+#    #+#             */
/*   Updated: 2024/09/19 15:27:29 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *run_pwd(bool print)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (perror("getcwd() error"), NULL); //TODO Handle error n
	if (print == true)
		printf("%s\n", cwd);
	return (ft_strdup((char *)cwd));
}

int run_cd(t_pipex *pipex_s)
{
	t_main	*main_s;
	char	*new_dir;
	char	**cmd;

	main_s = pipex_s->main_s;
	cmd = pipex_s->cmd;
	if (cmd[1] && cmd[2])
		return (printf("%s: %s\n", cmd[0], "too many arguments"), 1); //TODO Handle error s
    if (!cmd[1] || strcmp(cmd[1], "~") == 0)
        new_dir = strdup(ft_getenv(main_s, "HOME"));
    else if (strcmp(cmd[1], "-") == 0)
	{
        new_dir = strdup(ft_getenv(main_s, "OLDPWD"));
        printf("%s\n", new_dir);
	}
    else
		new_dir = ft_strdup(cmd[1]);
	ft_setenv(main_s, "OLDPWD", run_pwd(false), 1);
    if (!(chdir(new_dir) == 0))
        return (free(new_dir), printf("%s: %s: %s\n", cmd[0], cmd[1], strerror(errno)), 1);//TODO Handle error s
    ft_setenv(main_s, "PWD", run_pwd(false), 1);
	free(new_dir);
	return (0);
}

int run_echo(t_pipex *pipex_s)
{
	int i;
	int nl;
	char *input;

	nl = 0;
	i = 1;
	(void) input;
	while (pipex_s->cmd[i])
	{
		if (i == 2 && ft_strcmp(pipex_s->cmd[i], "-n") == 0 && ++i)
			nl = 1;
		printf("%s", pipex_s->cmd[i]);
		if (pipex_s->cmd[++i])
			printf(" ");
	}
	if (!nl)
		printf("\n");
	return (1);
}

int	edge_cases(t_pipex *pipex_s)
{
	if (!pipex_s || !pipex_s->cmd[0])
		return (0);
	if (ft_strcmp(pipex_s->cmd[0], "history") == 0)
		get_history(pipex_s->main_s->history, -1);
	else if (ft_strcmp(pipex_s->cmd[0], "pwd") == 0)
		free(run_pwd(true));
	else if (ft_strcmp(pipex_s->cmd[0], "echo") == 0)
		run_echo(pipex_s);
	else if (ft_strcmp(pipex_s->cmd[0], "env") == 0)
		my_print_env(pipex_s->main_s);
	else
		return (0);
	return (1);
}

void run_export(t_pipex *pipex_s)
{
	char *prefix;
	char *sufix;
	int	i;

	if (!pipex_s->cmd[1])
		return ;
	i = 0;
	while ((pipex_s->cmd[1])[i] &&(pipex_s->cmd[1])[i] != '=')
		i++;
	prefix = ft_strnjoin(NULL, pipex_s->cmd[1], i++);
	sufix = ft_strnjoin(NULL, &pipex_s->cmd[1][i], -1);
	i = 0;
	if (sufix)
		ft_setenv(pipex_s->main_s, prefix, sufix, 1);
}

void run_unset(t_pipex *pipex_s)
{
	t_env *menv_s;
	t_env *prev;

	menv_s = pipex_s->main_s->env;
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


int special_edge_cases(t_pipex *pipex_s)
{
	if (!pipex_s || !pipex_s->cmd[0])
		return (0);
	if (ft_strcmp(pipex_s->cmd[0], "cd") == 0)
		pipex_s->status = run_cd(pipex_s);
	else if (ft_strcmp(pipex_s->cmd[0], "exit") == 0)
		ft_exit(pipex_s);
	else if (ft_strcmp(pipex_s->cmd[0], "unset") == 0)
		run_unset(pipex_s);
	else if (ft_strcmp(pipex_s->cmd[0], "export") == 0)
		run_export(pipex_s);
	else
		return (0);
	return (1);
}
