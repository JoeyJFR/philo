/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guarding.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjiang <zjiang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 10:23:45 by zjiang            #+#    #+#             */
/*   Updated: 2025/03/15 11:28:36 by zjiang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*malloc_guard(size_t bytes, t_table *table)
{
	void	*ret;

	ret = malloc(bytes);
	if (!ret)
	{
		clean_table(table);
		perror_exit("malloc.\n", 1);
	}
	return (ret);
}

static void	mutex_error_handler(int status, t_ope ope, t_table *table)
{
	if (status == 0)
		return ;
	clean_table(table);
	if (status == EINVAL && (ope == LOCK || ope == UNLOCK || ope == DESTROY))
		perror_exit("invalid mutex.\n", 1);
	else if (status == EINVAL && ope == INIT)
		perror_exit("invalid attr.\n", 1);
	else if (status == EDEADLK)
		perror_exit("deadlock would occur.\n", 1);
	else if (status == EPERM)
		perror_exit("the current thread does not hold a lock on mutex.\n", 1);
	else if (status == ENOMEM)
		perror_exit("no enough memory to create new mutex.\n", 1);
	else if (status == EBUSY)
		perror_exit("mutex is locked.\n", 1);
}

void	mutex_handle_guard(t_mtx *lock, t_ope ope, t_table *table)
{
	if (ope == LOCK)
		mutex_error_handler(pthread_mutex_lock(lock), ope, table);
	else if (ope == UNLOCK)
		mutex_error_handler(pthread_mutex_unlock(lock), ope, table);
	else if (ope == INIT)
		mutex_error_handler(pthread_mutex_init(lock, NULL), ope, table);
	else if (ope == DESTROY)
		mutex_error_handler(pthread_mutex_destroy(lock), ope, table);
	else
	{
		clean_table(table);
		perror_exit("wronge ope code.\n", 1);
	}
}

static void	thread_error_handler(int status, t_ope ope, t_table *table)
{
	if (status == 0)
		return ;
	clean_table(table);
	if (status == EINVAL && ope == CREATE)
		perror_exit("invalid attr.\n", 1);
	else if (status == EINVAL && (ope == JOIN || ope == DETACH))
		perror_exit("not joinable.\n", 1);
	else if (status == EAGAIN)
		perror_exit("no resources to create new thread.\n", 1);
	else if (status == EPERM)
		perror_exit("no permission.\n", 1);
	else if (status == ESRCH)
		perror_exit("could not find the thread.\n", 1);
	else if (status == EDEADLK)
		perror_exit("a deadlock was detected.\n", 1);
}

void	thread_handle_guard(pthread_t *thread,
		void *(*f)(void *), void *philo, t_ope ope)
{
	if (ope == CREATE)
		thread_error_handler(pthread_create(thread, NULL, f, philo),
			ope, ((t_philo *)philo)->table);
	else if (ope == JOIN)
		thread_error_handler(pthread_join(*thread, NULL),
			ope, ((t_philo *)philo)->table);
	else if (ope == DETACH)
		thread_error_handler(pthread_detach(*thread),
			ope, ((t_philo *)philo)->table);
	else
	{
		clean_table(((t_philo *)philo)->table);
		perror_exit("wronge ope code.\n", 1);
	}
}
