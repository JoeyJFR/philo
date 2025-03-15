/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjiang <zjiang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 08:58:37 by zjiang            #+#    #+#             */
/*   Updated: 2025/03/15 11:29:08 by zjiang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	philo_died(t_philo *philo)
{
	long	elapsed;
	long	time_to_die;

	if (get_bool(&(philo->philo_lock), &(philo->full), philo->table))
		return (false);
	elapsed = get_time(MILLISECOND, philo->table) - get_long(&(philo->philo_lock), &(philo->last_meal_time), philo->table);
	time_to_die = philo->table->time_to_die / 1000;
	if (elapsed > time_to_die)
		return (true);
	return (false);
}

void	*monitor(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	while (!all_philos_running(&(table->table_lock),
			&(table->thread_running_nb), table->philo_nb, table))
		;
	while (!sim_finish(table))
	{
		i = -1;
		while (++i < table->philo_nb && !sim_finish(table))
		{
			if (philo_died(table->philos + i))
			{
				write_status(DIED, table->philos + i, DEBUG_MODE);
				set_bool(&(table->table_lock), &(table->end_simulation), true, table);
			}
		}
	}
	return (NULL);
}
