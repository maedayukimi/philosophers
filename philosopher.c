/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 14:36:29 by mawako            #+#    #+#             */
/*   Updated: 2025/07/31 18:39:20 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*solo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	log_status(philo, false, FORK_L);
	ph_delay(philo->table, philo->table->time_to_die);
	log_status(philo, false, DIED);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	return (NULL);
}

static void	custom_think(t_philo *philo, bool first)
{
	time_t	delay;

	pthread_mutex_lock(&philo->meal_time_lock);
	delay = philo->table->time_to_die
		- (get_current_ms() - philo->last_meal)
		- philo->table->time_to_eat;
	if (delay < 0)
		delay = 0;
	pthread_mutex_unlock(&philo->meal_time_lock);
	if (delay == 0 && first)
		delay = 1;
	if (delay > 600)
		delay = 200;
	if (!first)
		log_status(philo, false, THINKING);
	ph_delay(philo->table, delay);
}

static void	do_eat_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	log_status(philo, false, FORK_L);
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[1]]);
	log_status(philo, false, FORK_R);
	log_status(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = get_current_ms();
	pthread_mutex_unlock(&philo->meal_time_lock);
	ph_delay(philo->table, philo->table->time_to_eat);
	if (!is_simulation_stopped(philo->table))
	{
		pthread_mutex_lock(&philo->meal_time_lock);
		philo->times_ate++;
		pthread_mutex_unlock(&philo->meal_time_lock);
	}
	log_status(philo, false, SLEEPING);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[1]]);
	ph_delay(philo->table, philo->table->time_to_sleep);
}

void	*philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = philo->table->start_time;
	pthread_mutex_unlock(&philo->meal_time_lock);
	sim_start_delay(philo->table->start_time);
	if (philo->table->time_to_die == 0)
		return (NULL);
	if (philo->table->nb_philos == 1)
		return (solo_routine(philo));
	if (philo->id % 2)
		custom_think(philo, true);
	while (!is_simulation_stopped(philo->table))
	{
		do_eat_sleep(philo);
		custom_think(philo, false);
	}
	return (NULL);
}
