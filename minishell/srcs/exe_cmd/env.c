/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:44:33 by fandre-b          #+#    #+#             */
/*   Updated: 2024/09/25 11:51:41 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char **get_array_env(t_main *main_s)
{
	t_env *env;
	char **menvp;
	int	i;

	i = 0;
	env = main_s->env;
	while (env)
	{
		i++;
		env = env->next;
	}
	menvp = (char **) safe_malloc (sizeof (char *) * (i + 1));
	i = 0;
	env = main_s->env;
	while (env)
	{
		menvp[i++] = ft_strdup(env->value);
		env = env->next;
	}
	menvp[i] = NULL;
	return (menvp);
}

char	*env_get_value(char *var_name, char *var_value)
{
	char	*new_value;
	int		len;

	if (!var_name)
		return (NULL);
	len = ft_strlen(var_name) + 1;
	if (var_value && *var_value)
		len += ft_strlen(var_value);
	new_value = (char *)safe_malloc(sizeof(char) * (len + 1));
	if (new_value)
	{
		ft_strlcpy(new_value, var_name, ft_strlen(var_name) + 1);
		ft_strcat(new_value, "=");
		if (var_value && *var_value)
			ft_strcat(new_value, var_value);
	}
	return (new_value);
}

char	*ft_getenv(t_main *main_s, char *var_name)
{
	t_env *menv_s;

    if (!main_s || !var_name)
		return (NULL);
	menv_s = main_s->env;
	if (!menv_s)
		return (NULL);
	while (menv_s && ft_strcmp(menv_s->var, var_name) != 0)
		menv_s = menv_s->next;
	if (menv_s)
		return (menv_s->var_value);
	return (NULL);
}

void	ft_setenv(t_main *main_s, char *var_nm, char *var_vl, int ovwr)
{
	t_env	*menv_s;
	t_env	*new;
	char	*value;

	menv_s = main_s->env;
	if (!menv_s)
		return;
	while (menv_s && ft_strcmp(menv_s->var, var_nm) != 0)
		menv_s = menv_s->next;
	value = env_get_value(var_nm, var_vl);
	if (!menv_s)
	{
		new = ft_export_new(value);
		append_env_back(&main_s->env, new);
		free(value);
		free(var_vl);
	}
	else if (ovwr == 1)
	{
		ft_free(&menv_s->value);
		menv_s->value = value;
		ft_free(&menv_s->var_value);
		menv_s->var_value = var_vl;
	}
	else
		ft_free(&var_vl);
}

t_env	*new_menv_s(void)
{//deprecated
	t_env *menv_s;

	menv_s = (t_env *)safe_malloc(sizeof(t_env));
	if (!menv_s)
		return (NULL);
	menv_s->var = NULL;
	menv_s->var_value = NULL;
	menv_s->value = NULL;
	// menv_s->index = 1;
	menv_s->next = NULL;
	return (menv_s);
}

// void export_env(t_main *main_s)
// {//deprecated
// 	char	**envp;
// 	int		i;
// 	t_env	*menv_s;

// 	if (main_s->env)
// 		return ;
// 	main_s->env = new_menv_s();
// 	menv_s = main_s->env;
// 	envp = main_s->menv;
//     while (*envp != NULL)
//     {
// 		i = 0;
// 		while ((*envp)[i] &&(*envp)[i] != '=')
// 			i++;
// 		menv_s->var = ft_strnjoin(NULL, *envp, i++);
// 		menv_s->var_value = ft_strnjoin(NULL, *envp + i, -1);
// 		if (*(++envp) == NULL)
// 			break ;
// 		menv_s->next = new_menv_s();
//     	menv_s->next->index = menv_s->next->index + 1;
// 		menv_s = menv_s->next;
//     }
// }

void	my_print_env(t_main *main_s)
{
	t_env *menv_s;

	menv_s = main_s->env;
	while (menv_s)
	{
		if(menv_s->var)
		{
			ft_putstr_fd(menv_s->var, 1);
			ft_putstr_fd("=", 1);
			if (menv_s->var_value)
				ft_putstr_fd(menv_s->var_value, 1);
			ft_putstr_fd("\n", 1);
		}
		// printf("%s=%s\n", menv_s->var, menv_s->var_value);
		menv_s = menv_s->next;
	}
}
