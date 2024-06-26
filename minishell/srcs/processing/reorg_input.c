/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reorg_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 12:10:40 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/06/26 11:56:24 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*reorg_input(t_main *main_s)
{
	// char	*reorg;

	main_s->tokens = tokenize_refine_word(main_s);
	first_pars(main_s, main_s->tokens);
	// tokenize_smarter(main_s->tokens);
	return (NULL);
}
