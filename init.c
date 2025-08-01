/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 17:40:12 by mawako            #+#    #+#             */
/*   Updated: 2025/07/31 19:22:51 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static pthread_mutex_t	*create_forks(t_table *env)
{
	unsigned int	j;
	pthread_mutex_t	*fork_array;

	fork_array = malloc(sizeof(pthread_mutex_t) * env->nb_philos);
	if (!fork_array)
		exit_error("Failed to malloc", env);
	j = 0;
	while (j < env->nb_philos)
	{
		if (pthread_mutex_init(&fork_array[j], NULL) != 0)
			exit_error("Failed to init", env);
		j++;
	}
	return (fork_array);
}

static void	assign_fork(t_philo *philo)
{
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->table->nb_philos;
		philo->fork[1] = philo->id;
	}
	else
	{
		philo->fork[0] = philo->id;
		philo->fork[1] = (philo->id + 1) % philo->table->nb_philos;
	}
}

static t_philo	**create_philos(t_table *env)
{
	unsigned int	j;
	t_philo			**list;

	list = malloc(sizeof(t_philo *) * env->nb_philos);
	if (!list)
		exit_error("Failed to malloc", env);
	j = 0;
	while (j < env->nb_philos)
	{
		list[j] = malloc(sizeof(t_philo));
		if (!list[j])
			exit_error("Failed to malloc", env);
		if (pthread_mutex_init(&list[j]->meal_time_lock, NULL) != 0)
			exit_error("Failed to init", env);
		list[j]->table = env;
		list[j]->id = j;
		list[j]->times_ate = 0;
		assign_fork(list[j]);
		j++;
	}
	return (list);
}

static bool	init_mutexes(t_table *env)
{
	env->fork_locks = create_forks(env);
	if (!env->fork_locks)
		return (false);
	if (pthread_mutex_init(&env->sim_stop_lock, NULL) != 0)
		exit_error("Failed to init", env);
	if (pthread_mutex_init(&env->write_lock, NULL) != 0)
		exit_error("Failed to init", env);
	return (true);
}

t_table	*setup_simulation(int ac, char **av, int idx)
{
	t_table	*env;

	env = malloc(sizeof(t_table));
	if (!env)
		exit_error("Failed to malloc", NULL);
	env->nb_philos = ph_atoi(av[idx++]);
	env->time_to_die = ph_atoi(av[idx++]);
	env->time_to_eat = ph_atoi(av[idx++]);
	env->time_to_sleep = ph_atoi(av[idx++]);
	env->must_eat_count = -1;
	if (ac == 6)
		env->must_eat_count = ph_atoi(av[idx]);
	env->philos = create_philos(env);
	if (!env->philos)
		return (NULL);
	if (!init_mutexes(env))
		return (NULL);
	env->sim_stop = false;
	return (env);
}
