/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: splattje <splattje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:56:27 by splattje          #+#    #+#             */
/*   Updated: 2024/04/08 09:18:29 by splattje         ###   ########.fr       */
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

/**
 * @brief the locking and unlocking for eating for even id philos
 * @param philo pointer to the philo struct (t_philo)
 * @return 0 on success 1 if philo died while waiting for forks
*/
static int	is_even(t_philo **philo)
{
	pthread_mutex_lock((*philo)->r_fork);
	if (if_waited_to_long(&(*philo)))
		return (unlock_mutexes(0, 1, 0, &(*philo)), 1);
	pthread_mutex_lock((*philo)->write);
	if (if_waited_to_long(&(*philo)))
		return (unlock_mutexes(0, 1, 1, &(*philo)), 1);
	pthread_mutex_lock(&(*philo)->lock);
	(*philo)->status = 4;
	pthread_mutex_unlock(&(*philo)->lock);
	if (what_to_print((*philo)->status, &(*philo)))
		return (unlock_mutexes(0, 1, 1, &(*philo)), 1);
	pthread_mutex_unlock((*philo)->write);
	pthread_mutex_lock((*philo)->l_fork);
	if (if_waited_to_long(&(*philo)))
		return (unlock_mutexes(1, 1, 0, &(*philo)), 1);
	if (eating(&(*philo)))
		return (1);
	return (0);
}

/**
 * @brief the locking and unlocking for eating for odd id philos
 * @param philo pointer to the philo struct (t_philo)
 * @return 0 on success 1 if philo died while waiting for forks
*/
static int	is_odd(t_philo **philo)
{
	pthread_mutex_lock((*philo)->l_fork);
	if (if_waited_to_long(&(*philo)))
		return (unlock_mutexes(1, 0, 0, &(*philo)), 1);
	pthread_mutex_lock((*philo)->write);
	if (if_waited_to_long(&(*philo)))
		return (unlock_mutexes(1, 0, 1, &(*philo)), 1);
	pthread_mutex_lock(&(*philo)->lock);
	(*philo)->status = 4;
	pthread_mutex_unlock(&(*philo)->lock);
	if (what_to_print((*philo)->status, &(*philo)))
		return (unlock_mutexes(1, 0, 1, &(*philo)), 1);
	pthread_mutex_unlock((*philo)->write);
	if ((*philo)->r_fork == NULL)
		return (unlock_mutexes(1, 0, 0, &(*philo)), 1);
	pthread_mutex_lock((*philo)->r_fork);
	if (if_waited_to_long(&(*philo)))
		return (unlock_mutexes(1, 1, 0, &(*philo)), 1);
	if (eating(&(*philo)))
		return (1);
	return (0);
}

/**
 * @brief set the eating option based if odd or even
 * @param philo pointer to the philo struct (t_pilo)
 * @return 1 if philo died, 0 on success
*/
int	is_eating(t_philo **philo)
{
	if (((*philo)->id % 2) == 0)
		return (is_even(&(*philo)));
	else
		return (is_odd(&(*philo)));
}
