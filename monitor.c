/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 14:25:00 by mawako            #+#    #+#             */
/*   Updated: 2025/07/31 18:29:49 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	update_simulation_flag(t_table *env, bool is_stop)
{
	pthread_mutex_lock(&env->sim_stop_lock);
	env->sim_stop = is_stop;
	pthread_mutex_unlock(&env->sim_stop_lock);
}

bool	is_simulation_stopped(t_table *env)
{
	bool	is_stop;

	pthread_mutex_lock(&env->sim_stop_lock);
	is_stop = env->sim_stop;
	pthread_mutex_unlock(&env->sim_stop_lock);
	return (is_stop);
}

static bool	check_philo_death(t_philo *philo)
{
	time_t	now;

	now = get_current_ms();
	if ((now - philo->last_meal) >= philo->table->time_to_die)
	{
		update_simulation_flag(philo->table, true);
		log_status(philo, true, DIED);
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (true);
	}
	return (false);
}

static bool	termination_condition(t_table *env)
{
	unsigned int	i;
	bool			all_full;

	i = 0;
	all_full = true;
	while (i < env->nb_philos)
	{
		pthread_mutex_lock(&env->philos[i]->meal_time_lock);
		if (check_philo_death(env->philos[i]))
			return (true);
		if (env->must_eat_count != -1
			&& env->philos[i]->times_ate < (unsigned int)env->must_eat_count)
			all_full = false;
		pthread_mutex_unlock(&env->philos[i]->meal_time_lock);
		i++;
	}
	if (env->must_eat_count != -1 && all_full)
	{
		update_simulation_flag(env, true);
		return (true);
	}
	return (false);
}

void	*monitor(void *arg)
{
	t_table	*env;

	env = (t_table *)arg;
	sim_start_delay(env->start_time);
	while (true)
	{
		if (termination_condition(env))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
