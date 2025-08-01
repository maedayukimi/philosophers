/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 17:34:22 by mawako            #+#    #+#             */
/*   Updated: 2025/07/31 19:27:37 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <stdbool.h>
# include <limits.h>

# define MAX_PHILOS     250

typedef struct s_philo	t_philo;

typedef struct s_table
{
	time_t			start_time;
	unsigned int	nb_philos;
	pthread_t		is_dead;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				must_eat_count;
	bool			sim_stop;
	pthread_mutex_t	sim_stop_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*fork_locks;
	t_philo			**philos;
}	t_table;

typedef struct s_philo
{
	pthread_t		thread;
	unsigned int	id;
	unsigned int	times_ate;
	unsigned int	fork[2];
	pthread_mutex_t	meal_time_lock;
	time_t			last_meal;
	t_table			*table;
}	t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING,
	SLEEPING,
	THINKING,
	FORK_L,
	FORK_R
}	t_status;

int			main(int ac, char **av);
void		release_all(t_table *env);
bool		check_args(int argc, char **argv);
t_table		*setup_simulation(int ac, char **av, int idx);
void		*philosopher(void *arg);
void		log_status(t_philo *philo, bool is_dead, t_status flag);
void		*monitor(void *arg);
bool		is_simulation_stopped(t_table *env);
time_t		get_current_ms(void);
void		ph_delay(t_table *env, time_t wait_ms);
void		sim_start_delay(time_t start);
void		clear_mutexes(t_table *env);
void		help_msg(void);
void		exit_error(char *msg, t_table *env);
int			ph_atoi(char *s);

#endif
