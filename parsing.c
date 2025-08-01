/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 17:28:05 by mawako            #+#    #+#             */
/*   Updated: 2025/07/31 19:23:34 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	ph_isnum(char *s)
{
	while (*s)
	{
		if (!(*s >= '0' && *s <= '9'))
			return (false);
		s++;
	}
	return (true);
}

int	ph_atoi(char *s)
{
	unsigned long long	num;

	num = 0;
	while (*s && (*s >= '0' && *s <= '9'))
	{
		num = num * 10 + (*s - '0');
		s++;
	}
	if (num > INT_MAX)
		return (-1);
	return (num);
}

bool	check_args(int argc, char **argv)
{
	int	i;
	int	val;

	i = 1;
	while (i < argc)
	{
		if (!ph_isnum(argv[i]))
			exit_error("invalid arguments", NULL);
		val = ph_atoi(argv[i]);
		if (i == 1)
		{
			if (val <= 0 || val > MAX_PHILOS)
				exit_error("invalid arguments", NULL);
		}
		else if (val == -1)
			exit_error("invalid arguments", NULL);
		i++;
	}
	return (true);
}
