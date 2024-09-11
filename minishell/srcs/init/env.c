/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 17:14:31 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/11 13:56:21 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*extract_var_value(char *str)
{
	int		i;
	char	*var_value;

	if (!str)
		return (NULL);
	i = 1;
	if (str[i] == '"')
		return (extract_inside_quotes(&str[i]));
	while (str[i])
		i++;
	if (i == 1)
		return (NULL);
	var_value = (char *)malloc(sizeof(char) * i);
	if (!var_value)
		return (NULL);
	ft_strlcpy2(var_value, str + 1, i);
	return (var_value);
}

char	*extract_var(t_env	*new_env)
{
	int		i;
	char	*var;
	char	*str;

	str = new_env->value;
	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	var = (char *)malloc(sizeof(char) * (i + 1));
	if (!var)
		return (NULL);
	ft_strlcpy2(var, str, i + 1);
	if (str[i] == '=')
		new_env->var_value = extract_var_value(&str[i]);
	return (var);
}

static void	append_env_back(t_env **first, t_env *new_env)
{
	t_env	*last;

	if (!*first)
	{
		*first = new_env;
		return ;
	}
	last = *first;
	while (last->next != NULL)
		last = last->next;
	last->next = new_env;
}

static t_env	*ft_new_env(char **envp, int i)
{
	t_env	*new_env;
	int		env_len;

	new_env = (t_env *)malloc(sizeof(t_env));
	if (!new_env)
		return (NULL);
	env_len = ft_strlen(envp[i]);
	new_env->value = (char *)malloc((env_len + 1) * sizeof(char));
	if (!new_env->value)
	{
		free(new_env);
		return (NULL);
	}
	ft_strlcpy2(new_env->value, envp[i], env_len + 1);
	new_env->index = i;
	new_env->next = NULL;
	new_env->var = NULL;
	new_env->var_value = NULL;
	new_env->var = extract_var(new_env);
	return (new_env);
}

t_env	*get_env(char **envp)
{
	t_env	*first;
	t_env	*new_env;
	int		i;

	if (!envp)
		return (NULL);
	first = NULL;
	i = 0;
	while (envp[i])
	{
		new_env = ft_new_env(envp, i);
		if (!new_env)
		{
			if (first)
				free_env(first);
			return (NULL);
		}
		append_env_back(&first, new_env);
		i++;
	}
	return (first);
}
