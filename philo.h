/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjiang <zjiang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:03:37 by zjiang            #+#    #+#             */
/*   Updated: 2025/03/15 15:30:06 by zjiang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/time.h>
# include <errno.h>
# include <stdbool.h>

# define DEBUG_MODE 0

typedef pthread_mutex_t	t_mtx;
typedef struct s_table	t_table;

typedef enum e_ope
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH
}	t_ope;

typedef enum e_time
{
	SECOND,
	MILLISECOND,
	MICROSECOND
}	t_time;

typedef enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED
}	t_status;

typedef struct s_fork
{
	t_mtx	fork_lock;
	int		fork_id;
	bool	created;
	bool	taken;
}	t_fork;

typedef struct s_philo
{
	int			philo_id;
	int			meals_count;
	bool		full;
	long		last_meal_time;
	t_fork		*first_fork;
	t_fork		*second_fork;
	pthread_t	thread_id;
	t_table		*table;
	t_mtx		philo_lock;
	bool		created;
}	t_philo;

struct s_table
{
	long		philo_nb;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		meals_to_meat;
	long		start_simulation;
	long		thread_running_nb;
	bool		end_simulation;
	bool		all_philos_ready;
	t_fork		*forks;
	t_philo		*philos;
	t_mtx		table_lock;
	bool		table_lock_created;
	t_mtx		write_lock;
	bool		write_lock_created;
	pthread_t	monitor;
};

/* utils */
int		ft_strlen(char *s);
int		perror_exit(char *s, int i);
void	clean_table(t_table *table);
long	get_time(t_time time_code, t_table *table);
void	precise_usleep(long usec, t_table *table);
void	write_status(t_status status, t_philo *philo, bool debug);
void	free_table(t_table *table);
int		check_forks(t_philo *philo);

/* parse input*/
void	parse_av(int ac, char *av[]);
void	parse_input(t_table *table, char **av);

/* guarding*/
void	*malloc_guard(size_t bytes, t_table *table);
void	mutex_handle_guard(t_mtx *lock, t_ope ope, t_table *table);
void	thread_handle_guard(pthread_t *thread,
			void *(*f)(void *), void *table, t_ope ope);
/* init */
void	table_init(t_table *table);

/* getter setter */
void	set_bool(t_mtx *mutex, bool *dest, bool value, t_table *table);
bool	get_bool(t_mtx *mutex, bool *value, t_table *table);
void	set_long(t_mtx *mutex, long *dest, long value, t_table *table);
long	get_long(t_mtx *mutex, long *value, t_table *table);
bool	sim_finish(t_table *table);

/* sync */
void	wait_all_philos(t_table *table);
void	increase_long(t_mtx *lock, long *value, t_table *table);
bool	all_philos_running(t_mtx *lock, long *threads,
			long philo_nb, t_table *table);
void	philo_wait(t_philo *philo);

/* dinner */
void	dinner_start(t_table *table);
void	think(t_philo *philo);

/* monitor */
void	*monitor(void *arg);

#endif
