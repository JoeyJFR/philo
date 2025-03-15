/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjiang <zjiang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:20:06 by zjiang            #+#    #+#             */
/*   Updated: 2025/03/15 10:18:44 by zjiang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_all_philos(t_table *table)
{
	while (!get_bool(&(table->table_lock), &(table->all_philos_ready), table))
		;
}

bool	all_philos_running(t_mtx *lock, long *threads,
			long philo_nb, t_table *table)
{
	bool	ret;

	ret = false;
	mutex_handle_guard(lock, LOCK, table);
	if (*threads == philo_nb)
		ret = true;
	mutex_handle_guard(lock, UNLOCK, table);
	return (ret);
}

void	increase_long(t_mtx *lock, long *value, t_table *table)
{
	mutex_handle_guard(lock, LOCK, table);
	(*value)++;
	mutex_handle_guard(lock, UNLOCK, table);
}
