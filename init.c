/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjiang <zjiang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 10:18:41 by zjiang            #+#    #+#             */
/*   Updated: 2025/03/15 15:30:08 by zjiang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_philo *philo, t_fork *forks, int pos)
{
	int	philo_nb;

	philo_nb = philo->table->philo_nb;
	if (philo->philo_id % 2 == 0)
	{
		philo->first_fork = forks + pos;
		philo->second_fork = forks + (pos + 1) % philo_nb;
	}
	else
	{
		philo->first_fork = forks + (pos + 1) % philo_nb;
		philo->second_fork = forks + pos;
	}
}

static void	philo_init(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->philo_nb)
	{
		philo = table->philos + i;
		philo->philo_id = i + 1;
		philo->full = false;
		philo->meals_count = 0;
		philo->table = table;
		assign_forks(philo, table->forks, i);
		mutex_handle_guard(&(philo->philo_lock), INIT, table);
		philo->created = true;
	}
}

static void	set_philo_false(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->philo_nb)
	{
		philo = table->philos + i;
		philo->created = false;
	}
}

static void	set_fork_false(t_table *table)
{
	int		i;
	t_fork	*fork;

	i = -1;
	while (++i < table->philo_nb)
	{
		fork = table->forks + i;
		fork->created = false;
		fork->taken = false;
	}
}

void	table_init(t_table *table)
{
	int	i;

	table->philos = NULL;
	table->forks = NULL;
	table->end_simulation = false;
	table->all_philos_ready = false;
	table->thread_running_nb = 0;
	table->table_lock_created = false;
	table->write_lock_created = false;
	mutex_handle_guard(&(table->table_lock), INIT, table);
	table->table_lock_created = true;
	mutex_handle_guard(&(table->write_lock), INIT, table);
	table->write_lock_created = true;
	table->philos = malloc_guard(sizeof(t_philo) * table->philo_nb, table);
	set_philo_false(table);
	table->forks = malloc_guard(sizeof(t_fork) * table->philo_nb, table);
	set_fork_false(table);
	i = -1;
	while (++i < table->philo_nb)
	{
		mutex_handle_guard(&(table->forks[i].fork_lock), INIT, table);
		table->forks[i].created = true;
		table->forks[i].fork_id = i;
	}
	philo_init(table);
}
