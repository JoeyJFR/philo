/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjiang <zjiang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:14:17 by zjiang            #+#    #+#             */
/*   Updated: 2025/03/15 11:58:35 by zjiang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(t_time time_code, t_table *table)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
	{
		clean_table(table);
		perror_exit("gettimeofday failure.\n", 1);
	}
	if (time_code == MILLISECOND)
		return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	else if (time_code == SECOND)
		return (tv.tv_sec + tv.tv_usec / 1000000);
	else if (time_code == MICROSECOND)
		return (tv.tv_sec * 1000000 + tv.tv_usec);
	else
	{
		clean_table(table);
		perror_exit("wrong time code.\n", 1);
	}
	return (2);
}

void	precise_usleep(long usec, t_table *table)
{
	long	start;
	long	elapsed;
	long	remain;

	start = get_time(MICROSECOND, table);
	while (get_time(MICROSECOND, table) - start < usec)
	{
		if (sim_finish(table))
			break ;
		elapsed = get_time(MICROSECOND, table) - start;
		remain = usec - elapsed;
		if (remain > 1000)
			usleep(remain / 2);
		else
		{
			while (get_time(MICROSECOND, table) - start < usec)
				;
		}
	}
}

static void	write_status_debug(t_status status, t_philo *philo, long elapsed)
{
	if (status == TAKE_FIRST_FORK && !sim_finish(philo->table))
		printf("%-6ld %d has taken 1st fork. fork_id: %d\n",
			elapsed, philo->philo_id, philo->first_fork->fork_id);
	else if (status == TAKE_SECOND_FORK && !sim_finish(philo->table))
		printf("%-6ld %d has taken 2nd fork. fork_id: %d\n",
			elapsed, philo->philo_id, philo->second_fork->fork_id);
	else if (status == EATING && !sim_finish(philo->table))
		printf("%-6ld %d is eating. meals_count: %d\n",
			elapsed, philo->philo_id, philo->meals_count);
	else if (status == SLEEPING && !sim_finish(philo->table))
		printf("%-6ld %d is sleeping.\n", elapsed, philo->philo_id);
	else if (status == THINKING && !sim_finish(philo->table))
		printf("%-6ld %d is thinking.\n", elapsed, philo->philo_id);
	else if (status == DIED && !sim_finish(philo->table))
		printf("%-6ld %d is died.\n", elapsed, philo->philo_id);
}

void	write_status(t_status status, t_philo *philo, bool debug)
{
	long	elapsed;

	if (philo->full)
		return ;
	elapsed = get_time(MILLISECOND, philo->table)
		- philo->table->start_simulation;
	mutex_handle_guard(&(philo->table->write_lock), LOCK, philo->table);
	if (debug)
		write_status_debug(status, philo, elapsed);
	else
	{
		if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK)
			&& !sim_finish(philo->table))
			printf("%-6ld %d has taken a fork\n", elapsed, philo->philo_id);
		else if (status == EATING && !sim_finish(philo->table))
			printf("%-6ld %d is eating\n", elapsed, philo->philo_id);
		else if (status == SLEEPING && !sim_finish(philo->table))
			printf("%-6ld %d is sleeping\n", elapsed, philo->philo_id);
		else if (status == THINKING && !sim_finish(philo->table))
			printf("%-6ld %d is thinking\n", elapsed, philo->philo_id);
		else if (status == DIED && !sim_finish(philo->table))
			printf("%-6ld %d is died\n", elapsed, philo->philo_id);
	}
	mutex_handle_guard(&(philo->table->write_lock), UNLOCK, philo->table);
}

void	free_table(t_table *table)
{
	t_philo	*philo;
	t_fork	*fork;
	int		i;

	i = -1;
	while (++i < table->philo_nb)
	{
		philo = table->philos + i;
		fork = table->forks + i;
		mutex_handle_guard(&(philo->philo_lock), DESTROY, table);
		mutex_handle_guard(&(fork->fork_lock), DESTROY, table);
	}
	mutex_handle_guard(&(table->table_lock), DESTROY, table);
	mutex_handle_guard(&(table->write_lock), DESTROY, table);
	if (table->forks)
	{
		free(table->forks);
		table->forks = NULL;
	}
	if (table->philos)
	{
		free(table->philos);
		table->philos = NULL;
	}
}
