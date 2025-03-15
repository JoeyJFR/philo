/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjiang <zjiang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:26:18 by zjiang            #+#    #+#             */
/*   Updated: 2025/03/14 14:11:45 by zjiang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static long	ft_atol(char *s)
{
	long	x;
	int		i;

	x = 0;
	i = 0;
	while (s[i])
	{
		x = x * 10 + s[i] - '0';
		i++;
	}
	if (x > INT_MAX)
		perror_exit("input beyond the INT_MAX.\n", 1);
	return (x);
}

void	parse_input(t_table *table, char **av)
{
	table->philo_nb = ft_atol(av[1]);
	if (table->philo_nb == 0)
		perror_exit("there is no philo.\n", 1);
	table->time_to_die = ft_atol(av[2]) * 1000;
	table->time_to_eat = ft_atol(av[3]) * 1000;
	table->time_to_sleep = ft_atol(av[4]) * 1000;
	if (table->time_to_die < 60000
		|| table->time_to_eat < 60000 || table->time_to_sleep < 60000)
		perror_exit("timestamp less than 60ms.\n", 1);
	if (av[5])
		table->meals_to_meat = ft_atol(av[5]);
	else
		table->meals_to_meat = -1;
}

static void	check_syntax(char *s)
{
	int	j;

	j = 0;
	if (!s[j])
		perror_exit("empty arg.\n", 1);
	while (s[j])
	{
		if (s[j] >= '0' && s[j] <= '9')
			j++;
		else
			perror_exit("invalid arg, "
				"arg should be just integer.\n", 1);
	}
	if (s[0] == '0' && s[1])
		perror_exit("arg begin with 0.\n", 1);
	if (ft_strlen(s) > 10)
		perror_exit("input beyond the INT_MAX.\n", 1);
}

void	parse_av(int ac, char *av[])
{
	int	i;

	i = 0;
	if (ac < 5 || ac > 6)
		perror_exit("wrong input.\n"
			"Need: \"./philo\" \"no. of philos\" \"time to die\""
			" \"time to eat\" \"time to sleep\""
			" \"[no. of times each philo must eat]\"\n", 1);
	while (++i < ac)
		check_syntax(av[i]);
}
