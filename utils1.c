/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjiang <zjiang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:38:43 by zjiang            #+#    #+#             */
/*   Updated: 2025/03/15 11:29:13 by zjiang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	perror_exit(char *s, int i)
{
	printf("Error: ");
	printf("%s", s);
	exit (i);
}

void	destroy_all(t_table *table)
{
	int		i;
	t_philo	*philo;
	t_fork	*fork;

	i = -1;
	while (++i < table->philo_nb)
	{
		if (table->forks)
		{
			fork = table->forks + i;
			if (fork->created == true)
				pthread_mutex_destroy(&(fork->fork_lock));
		}
		if (table->philos)
		{
			philo = table->philos + i;
			if (philo->created == true)
				pthread_mutex_destroy(&(philo->philo_lock));
		}
	}
	if (table->table_lock_created == true)
		pthread_mutex_destroy(&(table->table_lock));
	if (table->write_lock_created == true)
		pthread_mutex_destroy(&(table->write_lock));
}

void	clean_table(t_table *table)
{
	destroy_all(table);
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
