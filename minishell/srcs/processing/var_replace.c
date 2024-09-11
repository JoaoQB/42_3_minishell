/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_replace.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:54:05 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/09 15:13:33 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*var_check_env(t_env *env, char *str)
{
	t_env	*current;
	int		len;
	char	*new_value;

	if (!env || !str)
		return (NULL);
	current = env;
	while (current)
	{
		if (!ft_strcmp(current->var, str))
		{
			if (!current->var_value)
				return (NULL);
			len = ft_strlen(current->var_value);
			new_value = (char *)malloc(sizeof(char) * (len + 1));
			if (!new_value)
				return (NULL);
			ft_strlcpy2(new_value, current->var_value, len + 1);
			return (new_value);
		}
		current = current->next;
	}
	return (NULL);
}

static char	*var_change(t_main *main_s, t_token *current)
{
	char	*str;
	char	*var_value;
	char	*new_value;
	int		i;

	if (!main_s || !current || !current->value)
		return (NULL);
	str = current->value;
	i = 0;
	if (str[i + 1] == '\0' || ft_isquotes(str[i + 1]))
	{
		str[i] = NEG_VAR;
		new_value = extract_from_i(str, 0);
		return (new_value);
	}
	else if (str[i + 1] == '?')
	{
		// replace_question_mark(main_s, current); TODO
		new_value = extract_from_i(str, 1);
		return (new_value);
	}
	else if (!ft_isvar1stchar(str[i + 1]))
		return (NULL);
	var_value = extract_from_i(str, 1);
	if (!var_value)
		return (NULL);
	new_value = var_check_env(main_s->env, var_value);
	free (var_value);
	return (new_value);
}

static char	*var_check_after_var(t_token *current)
{
	char *str;
	int		i;

	if (!current || !current->value)
		return (NULL);
	str = current->value;
	i = 1;
	if (str[i] == '\0')
		return (str);
	else if (str[i] == '$' || str[i] == '"')
	{
		return (var_extract_after(current, i));
	}
	else if (!ft_isvar1stchar(str[i]))
	{
		if (str[i + 1] != '\0')
			return (var_extract_after(current, i + 1));
	}
	while(ft_isvarchar(str[i]))
		i++;
	if (str[i] != '\0')
		return(var_extract_after(current, i));
	return (str);
}

t_token	*var_replace(t_main *main_s, t_token *current, int i)
{
	char	*new_value;

	if (!main_s || !main_s->tokens || !current || !current->value)
		return (NULL);
	if (i > 0)
	{
		current->value = var_extract_before(&main_s->tokens, current, i);
		i = 0;
	}
	current->value = var_check_after_var(current);
	new_value = var_change(main_s, current);
	if (current->value)
		ft_free(&current->value);
	if (new_value)
		hide_operators(new_value);
	current->value = new_value;
	if (current->type == CONC)
		return (var_conc_quotes(&main_s->tokens, current));
	if (!current->value)
		return (current->next);
	return (current);
}
