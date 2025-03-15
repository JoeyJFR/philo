/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjiang <zjiang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 14:12:39 by zjiang            #+#    #+#             */
/*   Updated: 2025/03/15 15:30:07 by zjiang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static	void	release_fork(t_fork	*fork, t_philo *philo)
{
	mutex_handle_guard(&(fork->fork_lock), UNLOCK, philo->table);
}

int	check_forks(t_philo *philo)
{
	mutex_handle_guard(&(philo->first_fork->fork_lock), LOCK, philo->table);
	if (philo->first_fork->taken == false)
	{
		write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
		philo->first_fork->taken = true;
	}
	else
	{
		release_fork(philo->first_fork, philo);
		return (1);
	}
	mutex_handle_guard(&(philo->second_fork->fork_lock), LOCK, philo->table);
	if (philo->second_fork->taken == false)
	{
		write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
		philo->first_fork->taken = true;
	}
	else
	{
		release_fork(philo->first_fork, philo);
		release_fork(philo->second_fork, philo);
		return (1);
	}
	return (0);
}
