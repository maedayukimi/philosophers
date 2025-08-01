/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 17:32:42 by mawako            #+#    #+#             */
/*   Updated: 2025/07/31 19:20:42 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	release_all(t_table *env)
{
	unsigned int	j;

	j = 0;
	if (!env)
		return ;
	while (j < env->nb_philos)
	{
		pthread_mutex_destroy(&env->fork_locks[j]);
		pthread_mutex_destroy(&env->philos[j]->meal_time_lock);
		free(env->philos[j]);
		j++;
	}
	free(env->fork_locks);
	free(env->philos);
	pthread_mutex_destroy(&env->write_lock);
	pthread_mutex_destroy(&env->sim_stop_lock);
	free(env);
}

static bool	launch_threads(t_table *env)
{
	unsigned int	j;

	j = 0;
	env->start_time = get_current_ms() + (env->nb_philos * 20);
	while (j < env->nb_philos)
	{
		if (pthread_create(&env->philos[j]->thread, NULL,
				&philosopher, env->philos[j]) != 0)
			exit_error("Failed to create pthread", env);
		j++;
	}
	if (env->nb_philos > 1)
	{
		if (pthread_create(&env->is_dead, NULL,
				&monitor, env) != 0)
			exit_error("Failed to create pthread", env);
	}
	return (true);
}

static void	shutdown_simulation(t_table *env)
{
	unsigned int	j;

	j = 0;
	while (j < env->nb_philos)
	{
		pthread_join(env->philos[j]->thread, NULL);
		j++;
	}
	if (env->nb_philos > 1)
		pthread_join(env->is_dead, NULL);
	release_all(env);
}

int	main(int ac, char **av)
{
	t_table	*env;

	if (ac < 5 || ac > 6)
		help_msg();
	if (ac == 6 && av[5][0] == '0')
		exit_error("must eat is 0", NULL);
	if (!check_args(ac, av))
		exit_error("Bad arguments", NULL);
	env = setup_simulation(ac, av, 1);
	if (!env)
		exit_error("A system failure for the masses", NULL);
	if (!launch_threads(env))
		exit_error("Antimatter for the master plan", env);
	shutdown_simulation(env);
	return (0);
}
