/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjiang <zjiang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:13:33 by zjiang            #+#    #+#             */
/*   Updated: 2025/03/15 12:19:42 by zjiang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*philo_1(void *arg)
{
	t_philo	*philo;
	t_table	*table;

	table = (t_table *)arg;
	philo = table->philos;
	wait_all_philos(table);
	set_long(&(philo->philo_lock), &(philo->last_meal_time),
		get_time(MILLISECOND, table), table);
	increase_long(&(table->table_lock), &(table->thread_running_nb), table);
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	while (!sim_finish(table))
		usleep(100);
	return (NULL);
}

void	think(t_philo *philo)
{
	long	t_eat;
	long	t_sleep;
	long	t_think;

	write_status(THINKING, philo, DEBUG_MODE);
	if (philo->table->philo_nb % 2 == 0)
		return ;
	t_eat = philo->table->time_to_eat;
	t_sleep = philo->table->time_to_sleep;
	t_think = t_eat * 2 - t_sleep;
	if (t_think < 0)
		t_think = 0;
	precise_usleep(t_think * 0.5, philo->table);
}

static void	eat(t_philo *philo)
{
	mutex_handle_guard(&(philo->first_fork->fork_lock), LOCK, philo->table);
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	mutex_handle_guard(&(philo->second_fork->fork_lock), LOCK, philo->table);
	write_status(TAKE_SECOND_FORK, philo, DEBUG_MODE);
	set_long(&(philo->philo_lock), &(philo->last_meal_time),
		get_time(MILLISECOND, philo->table), philo->table);
	philo->meals_count++;
	write_status(EATING, philo, DEBUG_MODE);
	precise_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->meals_to_meat > 0
		&& philo->meals_count == philo->table->meals_to_meat)
		set_bool(&(philo->philo_lock), &(philo->full), true, philo->table);
	mutex_handle_guard(&(philo->first_fork->fork_lock), UNLOCK, philo->table);
	mutex_handle_guard(&(philo->second_fork->fork_lock), UNLOCK, philo->table);
}

static void	*dinner_sim(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_all_philos(philo->table);
	set_long(&(philo->philo_lock), &(philo->last_meal_time),
		get_time(MILLISECOND, philo->table), philo->table);
	increase_long(&(philo->table->table_lock),
		&(philo->table->thread_running_nb), philo->table);
	philo_wait(philo);
	while (!sim_finish(philo->table))
	{
		if (philo->full)
			break ;
		eat(philo);
		write_status(SLEEPING, philo, DEBUG_MODE);
		precise_usleep(philo->table->time_to_sleep, philo->table);
		think(philo);
	}
	return (NULL);
}

void	dinner_start(t_table *table)
{
	int	i;

	if (table->meals_to_meat == 0)
		return ;
	else if (table->philo_nb == 1)
		thread_handle_guard(&(table->philos[0].thread_id),
			philo_1, table, CREATE);
	else
	{
		i = -1;
		while (++i < table->philo_nb)
			thread_handle_guard(&(table->philos[i].thread_id),
				dinner_sim, &(table->philos[i]), CREATE);
	}
	thread_handle_guard(&(table->monitor), monitor, table, CREATE);
	table->start_simulation = get_time(MILLISECOND, table);
	set_bool(&(table->table_lock), &(table->all_philos_ready), true, table);
	i = -1;
	while (++i < table->philo_nb)
		thread_handle_guard(&(table->philos[i].thread_id),
			NULL, &(table->philos[i]), JOIN);
	set_bool(&(table->table_lock), &(table->end_simulation), true, table);
	thread_handle_guard(&(table->monitor), NULL, table, JOIN);
}
