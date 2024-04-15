/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleeping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: splattje <splattje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 08:52:14 by splattje          #+#    #+#             */
/*   Updated: 2024/04/08 11:38:04 by splattje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	unlock_lock(t_philo **philo)
{
	pthread_mutex_unlock(&((*philo)->lock));
}

int	is_sleeping(t_philo **philo)
{
	size_t	time;

	pthread_mutex_lock(&((*philo)->lock));
	if ((*philo)->status == -1)
		return (unlock_lock(&(*philo)), 1);
	if ((*philo)->status != -1)
		(*philo)->status = 2;
	pthread_mutex_unlock(&((*philo)->lock));
	time = get_current_time() - (*philo)->start_time;
	pthread_mutex_lock((*philo)->write);
	printf("%zu %d is sleeping\n", time, (*philo)->id);
	pthread_mutex_unlock((*philo)->write);
	if (ft_usleep_sleep((*philo)->time_to_sleep, (*philo)))
	{
		pthread_mutex_lock(&((*philo)->lock));
		(*philo)->status = -1;
		return (unlock_lock(&(*philo)), 1);
	}
	else
	{
		pthread_mutex_lock(&((*philo)->lock));
		if ((*philo)->status != -1)
			(*philo)->status = 5;
		return (unlock_lock(&(*philo)), 0);
	}
}
