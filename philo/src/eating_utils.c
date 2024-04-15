/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: splattje <splattje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 16:06:56 by splattje          #+#    #+#             */
/*   Updated: 2024/04/09 11:59:37 by splattje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	unlock_mutexes(int lf, int rf, int w, t_philo **philo)
{
	if (rf)
		pthread_mutex_unlock((*philo)->r_fork);
	if (lf)
		pthread_mutex_unlock((*philo)->l_fork);
	if (w)
		pthread_mutex_unlock((*philo)->write);
}

/**
 * @brief checks if the waiting for the forks took to long and the philo died
 * @param philo pointer to the philo struct (t_philo)
 * @param option flag of how many mutexes are locked
 * @param even flag if philo id is even
 * @return 1 if philo died, 0 if not
*/
static int	if_waited_to_long(t_philo **philo)
{
	pthread_mutex_lock(&(*philo)->lock);
	if ((*philo)->status == -1)
	{
		pthread_mutex_unlock(&(*philo)->lock);
		return (1);
	}
	pthread_mutex_unlock(&(*philo)->lock);
	if (((get_current_time() - (*philo)->last_meal_at)) > (*philo)->time_to_die)
	{
		pthread_mutex_lock(&(*philo)->lock);
		(*philo)->status = -1;
		pthread_mutex_unlock(&(*philo)->lock);
		return (1);
	}
	return (0);
}

int	eating(t_philo **philo)
{
	pthread_mutex_lock(&(*philo)->lock);
	if (((*philo)->status != -1))
	{
		(*philo)->status = 3;
		(*philo)->last_meal_at = get_current_time();
		pthread_mutex_unlock(&(*philo)->lock);
		pthread_mutex_lock((*philo)->write);
		if (if_waited_to_long(&(*philo)))
			return (unlock_mutexes(1, 1, 1, &(*philo)), 1);
		if (what_to_print((*philo)->status, &(*philo)))
			return (unlock_mutexes(1, 1, 1, &(*philo)), 1);
		pthread_mutex_unlock((*philo)->write);
		ft_usleep_sleep((*philo)->time_to_eat, *philo);
		pthread_mutex_lock(&(*philo)->lock);
		(*philo)->eat_count++;
		if ((*philo)->status != -1)
			(*philo)->status = 5;
		pthread_mutex_unlock(&(*philo)->lock);
	}
	else
		pthread_mutex_unlock(&(*philo)->lock);
	return (unlock_mutexes(1, 1, 0, &(*philo)), 0);
}
