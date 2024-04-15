/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: splattje <splattje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 08:43:34 by splattje          #+#    #+#             */
/*   Updated: 2024/04/08 09:55:17 by splattje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	memset_program(t_data **data)
{
	(*data)->start = 0;
	(*data)->dead = 0;
	(*data)->finished = 0;
	(*data)->start_time = 0;
}

void	memset_philo(t_philo *philo, t_data **data, int index)
{
	philo->last_meal_at = 0;
	philo->start_time = 0;
	if (index == (*data)->philo_num - 1)
	{
		if ((*data)->philo_num - 1 != 0)
			(*data)->philos[index].r_fork = &(*data)->forks[0];
		else
			(*data)->philos[index].r_fork = NULL;
	}
	else
		(*data)->philos[index].r_fork = &(*data)->forks[index + 1];
}

static void	printing(size_t time, int id, char *string)
{
	printf("%zu %d %s\n", time, id, string);
}

int	what_to_print(int status, t_philo **philo)
{
	size_t	time;

	time = get_current_time() - (*philo)->start_time;
	if (status == 3)
	{
		if ((*philo)->status != -1)
			return (printing(time, (*philo)->id, "is eating"), 0);
		else
			return (1);
	}
	else if (status == 4)
	{
		pthread_mutex_lock(&(*philo)->lock);
		if ((*philo)->status != -1)
		{
			pthread_mutex_unlock(&(*philo)->lock);
			return (printing(time, (*philo)->id, "has taken a fork"), 0);
		}
		else
		{
			pthread_mutex_unlock(&(*philo)->lock);
			return (1);
		}
	}
	return (0);
}

/**
 * @brief parses string to int
 * @param nptr the string to parse
 * @param nr pointer to the converted number
 * @return 1 on overflow, 0 on success
*/
int	ft_atoi(char *nptr, int *nr)
{
	int		sign;
	long	number;

	number = 0;
	sign = 1;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		number = number * 10 + (*nptr - '0');
		if (number * sign > INT_MAX || number * sign < INT_MIN)
			return (1);
		nptr++;
	}
	if (number * sign < 1)
		return (1);
	*nr = (int)(number * sign);
	return (0);
}
