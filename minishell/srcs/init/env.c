/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 17:14:31 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/06/08 18:29:56 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**get_env(char **envp)
{
	char	**menv;
	int		env_count;
	int		i;

	env_count = count_arrays(envp);
	menv = calloc(env_count + 1, sizeof(char *));
	if (!menv)
		return (NULL);
	i = 0;
	while (i < env_count)
	{
		menv[i] = ft_strdup(envp[i]);
		if (!menv[i])
		{
			while (i-- > 0)
				free(menv[i]);
			free(menv);
			return (NULL);
		}
		i++;
	}
	return (menv);
}
