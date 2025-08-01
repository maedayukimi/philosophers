/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:03:35 by mawako            #+#    #+#             */
/*   Updated: 2025/07/27 14:52:26 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	get_current_ms(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec * 1000) + (now.tv_usec / 1000));
}

void	ph_delay(t_table *env, time_t wait_ms)
{
	time_t	end;

	end = get_current_ms() + wait_ms;
	while (get_current_ms() < end)
	{
		if (is_simulation_stopped(env))
			break ;
		usleep(100);
	}
}

void	sim_start_delay(time_t start)
{
	while (get_current_ms() < start)
		continue ;
}
