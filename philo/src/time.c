/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: splattje <splattje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:09:38 by splattje          #+#    #+#             */
/*   Updated: 2024/04/08 11:32:29 by splattje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief gives the time of daty in milisecondes
 * @return returns the time of day in miliseconds 
*/
size_t	get_current_time(void)
{
	struct timeval	timeval;

	if (gettimeofday(&timeval, NULL) == -1)
		write_error("gettimeofday() error\n", NULL);
	return (timeval.tv_sec * 1000 + timeval.tv_usec / 1000);
}

/**
 * @param miniseconds the amount of time to wait
 * @brief sleep for x milisecondes
 * @return returns 0 when done waiting
*/
int	ft_usleep(size_t miliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < miliseconds)
		usleep(500);
	return (0);
}

/**
 * @param miniseconds the amount of time to wait
 * @param philo pointer to the philo struct (t_philo)
 * @brief sleep for x milisecondes and checks if philo has died
 * @return returns 0 when done waiting, 1 if the philo died
*/
int	ft_usleep_sleep(size_t miliseconds, t_philo *philo)
{
	size_t	start;

	start = get_current_time();
	pthread_mutex_lock(&philo->lock);
	if (philo->status == -1)
	{
		pthread_mutex_unlock(&philo->lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->lock);
	while ((get_current_time() - start) < miliseconds)
	{
		usleep(500);
		if (((get_current_time() - philo->last_meal_at)) > philo->time_to_die)
			return (1);
	}
	return (0);
}
