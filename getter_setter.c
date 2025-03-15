/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter_setter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjiang <zjiang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:49:52 by zjiang            #+#    #+#             */
/*   Updated: 2025/03/14 14:09:50 by zjiang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_bool(t_mtx *mutex, bool *dest, bool value, t_table *table)
{
	mutex_handle_guard(mutex, LOCK, table);
	*dest = value;
	mutex_handle_guard(mutex, UNLOCK, table);
}

bool	get_bool(t_mtx *mutex, bool *value, t_table *table)
{
	bool	ret;

	mutex_handle_guard(mutex, LOCK, table);
	ret = *value;
	mutex_handle_guard(mutex, UNLOCK, table);
	return (ret);
}

void	set_long(t_mtx *mutex, long *dest, long value, t_table *table)
{
	mutex_handle_guard(mutex, LOCK, table);
	*dest = value;
	mutex_handle_guard(mutex, UNLOCK, table);
}

long	get_long(t_mtx *mutex, long *value, t_table *table)
{
	long	ret;

	mutex_handle_guard(mutex, LOCK, table);
	ret = *value;
	mutex_handle_guard(mutex, UNLOCK, table);
	return (ret);
}

bool	sim_finish(t_table *table)
{
	return (get_bool(&(table->table_lock), &(table->end_simulation), table));
}
