/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:12:45 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/10/22 15:14:45 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*join_string(char *before, char *var, char *after)
{
	int		len;
	char	*new;

	if (!before && !var && !after)
		return (NULL);
	len = ft_strlen(before) + ft_strlen(var) + ft_strlen(after);
	if (len == 0)
		return (NULL);
	new = (char *)safe_malloc(sizeof(char) * len + 1);
	if (!new)
		return (NULL);
	new[0] = '\0';
	if (before)
		ft_strcat(new, before);
	if (var)
		ft_strcat(new, var);
	if (after)
		ft_strcat(new, after);
	return (new);
}

static char	*get_var(char *str)
{
	char	*var_value;
	char	*var;
	int		i;

	if (!str)
		return (NULL);
	i = 1;
	if (str[i] == '\0' || str[i] == '$'
		|| str[i] == ' ' || ft_isquotes(str[i]))
		return (extract_before_i(str, 1));
	else if (str[i] == '?')
		return (var_replace_qstnmrk());
	else if (!ft_isvar1stchar(str[i]))
		return (NULL);
	while (ft_isvarchar(str[i]))
		i++;
	var_value = extract_before_i(&str[1], i - 1);
	if (!var_value)
		return (NULL);
	var = var_check_env(minishell()->env, var_value);
	ft_free(&var_value);
	return (var);
}

static char	*check_after_var(char *str)
{
	int	i;

	if (!str)
		return (NULL);
	i = 1;
	if (str[i] == '\0')
		return (NULL);
	else if (str[i] == '$' || str[i] == ' ' || ft_isquotes(str[i]))
		return (extract_from_i(str, i));
	else if (!ft_isvar1stchar(str[i]))
	{
		if (str[i + 1] != '\0')
			return (extract_from_i(str, i + 1));
		return (NULL);
	}
	while (ft_isvarchar(str[i]))
		i++;
	if (str[i] != '\0')
		return (extract_from_i(str, i));
	return (NULL);
}

static char	*str_expand(t_token *delim, char *str, int i)
{
	char	*var;
	char	*before;
	char	*after;
	char	*new;

	if (!str)
		return (NULL);
	before = NULL;
	if (i > 0)
		before = extract_before_i(str, i);
	after = check_after_var(&str[i]);
	if (after)
		after = heredoc_expand(delim, after);
	var = get_var(&str[i]);
	new = join_string(before, var, after);
	ft_free(&str);
	ft_free(&before);
	ft_free(&var);
	ft_free(&after);
	return (new);
}

char	*heredoc_expand(t_token *delim, char *str)
{
	char	*new_input;
	int		i;

	if (!str)
		return (NULL);
	new_input = NULL;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && delim->type != DELIM_QUOTE)
			return (str_expand(delim, str, i));
		i++;
	}
	new_input = extract_from_i(str, 0);
	ft_free(&str);
	return (new_input);
}
