/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 17:34:15 by mawako            #+#    #+#             */
/*   Updated: 2025/07/27 14:51:39 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	display_status(t_philo *philo, const char *message)
{
	printf("%ld %d %s\n", get_current_ms() - philo->table->start_time,
		philo->id + 1, message);
}

void	log_status(t_philo *philo, bool is_dead, t_status flag)
{
	pthread_mutex_lock(&philo->table->write_lock);
	if (is_simulation_stopped(philo->table) && !is_dead)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (flag == DIED)
		display_status(philo, "died");
	else if (flag == EATING)
		display_status(philo, "is eating");
	else if (flag == SLEEPING)
		display_status(philo, "is sleeping");
	else if (flag == THINKING)
		display_status(philo, "is thinking");
	else if (flag == FORK_L || flag == FORK_R)
		display_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->table->write_lock);
}
