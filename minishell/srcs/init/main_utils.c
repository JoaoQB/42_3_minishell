/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fandre-b <fandre-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:43:44 by fandre-b          #+#    #+#             */
/*   Updated: 2024/10/22 15:25:01 by fandre-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	critical_error(char *err_print)
{
	perror(err_print);
	ft_exit(1);
}

void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = (void *) malloc(size);
	if (ptr == NULL)
		critical_error("malloc failed");
	return (ptr);
}
