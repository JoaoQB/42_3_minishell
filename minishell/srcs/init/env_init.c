/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 17:14:31 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/24 12:43:23 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*env_extract_var_value(char *str)
{
	int		i;
	char	*var_value;

	if (!str)
		return (NULL);
	i = 0;
	if (str[i] == '"')
		return (extract_inside_quotes(&str[i]));
	while (str[i])
		i++;
	var_value = extract_before_i(str, i);
	return (var_value);
}

char	*env_extract_var(t_env	*new_env)
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
	var = extract_before_i(str, i);
	if (str[i] == '=')
		i++;
	if (str[i] != '\0')
		new_env->var_value = env_extract_var_value(&str[i]);
	return (var);
}

void	append_env_back(t_env **first, t_env *new_env)
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

	if (!envp)
		return (NULL);
	new_env = (t_env *)safe_malloc(sizeof(t_env));
	if (!new_env)
		return (NULL);
	env_len = ft_strlen(envp[i]);
	new_env->value =(char *)safe_malloc((env_len + 1) * sizeof(char));
	if (!new_env->value)
	{
		free (new_env);
		return (NULL);
	}
	ft_strlcpy(new_env->value, envp[i], env_len + 1);
	new_env->var_value = NULL;
	new_env->var = env_extract_var(new_env);
	new_env->next = NULL;
	return (new_env);
}

t_env	*get_env(char **envp)
{
	t_env	*first;
	t_env	*new_env;
	int		i;

	first = NULL;
	i = 0;
	while (envp[i])
	{
		new_env = ft_new_env(envp, i);
		if (!new_env)
		{
			if (first)
				free_env(&first);
			return (NULL);
		}
		append_env_back(&first, new_env);
		i++;
	}
	return (first);
}
