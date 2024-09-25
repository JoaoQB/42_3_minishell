/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jqueijo- <jqueijo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:11:25 by jqueijo-          #+#    #+#             */
/*   Updated: 2024/09/25 12:24:17 by jqueijo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	digit_count(int n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		count++;
	while (n != 0)
	{
		n /= 10;
		count ++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		count;
	int		sign;

	sign = 1;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	count = digit_count(n);
	result = (char *)safe_malloc(sizeof(char) *(count + 1));
	if (!result)
		return (NULL);
	result[count] = '\0';
	if (n < 0)
	{
		sign = -1;
		n = -n;
	}
	while (count > 0)
	{
		result[--count] = (n % 10) + '0';
		n /= 10;
	}
	if (sign == -1)
		result[0] = '-';
	return (result);
}

double	ft_atoi_dbl(const char *nptr)
{
	double	res;
	int		sign;
	int		i;

	res = 0;
	i = 0;
	sign = 1;
	while ((nptr[i] >= '\t' && nptr[i] <= '\r') || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
		if (nptr[i++] == '-')
			sign = -1;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = (res * 10) +(nptr[i] - '0');
		i++;
	}
	return (res * sign);
}

int	ft_atoi(const char *nptr)
{
	int	res;
	int	sign;
	int	i;

	res = 0;
	i = 0;
	sign = 1;
	while ((nptr[i] >= '\t' && nptr[i] <= '\r') || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
		if (nptr[i++] == '-')
			sign = -1;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = (res * 10) + nptr[i] - '0';
		i++;
	}
	return (res * sign);
}
