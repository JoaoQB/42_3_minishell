/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:44:33 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/16 12:12:45 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**get_array_env(void)
{
	t_env	*env;
	char	**menvp;
	int		i;

	i = 0;
	env = minishell()->env;
	while (env)
	{
		i++;
		env = env->next;
	}
	menvp = (char **) safe_malloc (sizeof (char *) * (i + 1));
	i = 0;
	env = minishell()->env;
	while (env)
	{
		menvp[i++] = ft_strdup(env->value);
		env = env->next;
	}
	// printf("menvp[0]: %s\n", menvp[i - 1]);
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

char	*ft_getenv(char *var_name)
{
	t_env	*menv_s;

	if (!var_name)
		return (NULL);
	menv_s = minishell()->env;
	if (!menv_s)
		return (NULL);
	while (menv_s && ft_strcmp(menv_s->var, var_name) != 0)
		menv_s = menv_s->next;
	if (menv_s)
		return (menv_s->var_value);
	return (NULL);
}

void	ft_setenv(char *var_nm, char *var_vl, int ovwr)
{
	t_env	*menv_s;
	t_env	*new;
	char	*value;

	menv_s = minishell()->env;
	if (!menv_s)
		return ;
	while (menv_s && ft_strcmp(menv_s->var, var_nm) != 0)
		menv_s = menv_s->next;
	value = env_get_value(var_nm, var_vl);
	if (!menv_s)
	{
		new = ft_export_new(value);
		append_env_back(&minishell()->env, new);
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
}

void	my_print_env(void)
{
	t_env	*menv_s;

	menv_s = minishell()->env;
	while (menv_s)
	{
		if (menv_s->var)
		{
			ft_putstr_fd(menv_s->var, 1);
			ft_putstr_fd("=", 1);
			if (menv_s->var_value)
				ft_putstr_fd(menv_s->var_value, 1);
			ft_putstr_fd("\n", 1);
		}
		menv_s = menv_s->next;
	}
}
