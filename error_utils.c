/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:50:31 by mawako            #+#    #+#             */
/*   Updated: 2025/07/31 19:32:19 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	error_putendl(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
		i++;
	write(2, s, i);
	write(2, "\n", 1);
}

void	help_msg(void)
{
	error_putendl("This is Philosophers");
	error_putendl("Usage: ./philo number_of_philo time_die"
	  " time_eat time_sleep [must_eat]");
	error_putendl("Example:");
	error_putendl("./philo 2 300 200 50");
	exit(EXIT_FAILURE);
}

void	exit_error(char *msg, t_table *env)
{
	if (env)
		release_all(env);
	error_putendl(msg);
	exit(EXIT_FAILURE);
}
