/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: splattje <splattje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:05:22 by splattje          #+#    #+#             */
/*   Updated: 2024/04/15 09:34:42 by splattje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	set_routine(t_philo **philo)
{
	if (((*philo)->id % 2) == 0)
	{
		if (is_eating(&(*philo)))
			return (1);
		if (is_sleeping(&(*philo)))
			return (1);
	}
	else
	{
		if (is_sleeping(&(*philo)))
			return (1);
		if (is_eating(&(*philo)))
			return (1);
	}
	return (0);
}

static int	thinking(t_philo **philo)
{
	long	time;
	size_t	time2;

	pthread_mutex_lock((*philo)->write);
	time2 = get_current_time() - (*philo)->start_time;
	printf("%zu %d is thinking\n", time2, (*philo)->id);
	pthread_mutex_unlock((*philo)->write);
	time = (*philo)->time_to_die - ((*philo)->time_to_eat * 2)
		- (*philo)->time_to_sleep;
	if (time < 0)
		time = 0;
	if (ft_usleep_sleep((size_t)time, *philo))
		return (1);
	return (0);
}

static int	check_status(t_philo **philo)
{
	pthread_mutex_lock(&(*philo)->lock);
	if ((*philo)->status != -1)
	{
		pthread_mutex_unlock(&(*philo)->lock);
		if (thinking(&(*philo)))
			return (1);
	}
	else
		pthread_mutex_unlock(&(*philo)->lock);
	return (0);
}

static void	set_start(t_philo **philo)
{
	pthread_mutex_lock(&(*philo)->lock);
	if ((*philo)->status == 1)
	{
		(*philo)->status = 5;
		pthread_mutex_unlock(&(*philo)->lock);
	}
	else
		pthread_mutex_unlock(&(*philo)->lock);
}

/**
 * @brief the simutaion each philo is doing
 * @param args void pointer to the philo (t_philo)
 * @return NULL when done, or needs to stop
*/
void	*philo_routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (1)
	{
		pthread_mutex_lock(&philo->lock);
		if (philo->status == -1)
		{
			pthread_mutex_unlock(&philo->lock);
			break ;
		}
		else if (philo->status == 5)
		{
			pthread_mutex_unlock(&philo->lock);
			if (set_routine(&philo))
				break ;
			if (check_status(&philo))
				break ;
		}
		else
			pthread_mutex_unlock(&philo->lock);
		set_start(&philo);
	}
	return (NULL);
}
