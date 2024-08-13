/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 17:14:31 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/08/13 18:09:50 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// char	**get_env(char **envp)
// {
// 	char	**menv;
// 	int		env_count;
// 	int		i;

// 	env_count = count_arrays(envp);
// 	menv = calloc(env_count + 1, sizeof(char *));
// 	if (!menv)
// 		return (NULL);
// 	i = 0;
// 	while (i < env_count)
// 	{
// 		menv[i] = ft_strdup(envp[i]);
// 		if (!menv[i])
// 		{
// 			while (i-- > 0)
// 				free(menv[i]);
// 			free(menv);
// 			return (NULL);
// 		}
// 		i++;
// 	}
// 	for (i = 0; menv[i] != NULL; i++)
// 		printf("menv[%d]: %s\n", i, menv[i]);
// 	return (menv);
// }

static t_env	*get_last_env(t_env *first)
{
	t_env	*last;
	if (!first)
		return (NULL);
	while (first->next != NULL)
		first = first->next;
	last = first;
	return (last);
}

static void	append_env_back(t_env *first, t_env *new_env)
{
	t_env	*last;
	last = get_last_env(first);
	if (!last)
		return ;
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
	ft_strlcpy(new_env->value, envp[i], env_len + 1);
	new_env->index = i;
	new_env->next = NULL;
	new_env->var = NULL;
	new_env->var_value = NULL;
	// new_env->var = extract_var(new_env->value); //TODO
	// new_env->var_value = extract_var_value(new_env->value); //TODO
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
		if (i == 0)
			first = new_env;
		else
			append_env_back(first, new_env);
		i++;
	}
	return (first);
}
