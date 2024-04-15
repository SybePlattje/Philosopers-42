/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_and_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: splattje <splattje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:42:15 by splattje          #+#    #+#             */
/*   Updated: 2024/04/03 12:16:01 by splattje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief frees the heap allocated memory in each philosopher
 * @param data pointer to the program struct (t_data)
 * @param amound amout of philosophers
*/
void	free_philos(t_data *data, int amound)
{
	int	index;

	index = -1;
	if (data->philos != NULL)
	{
		while (++index < amound)
			if (data->philos[index].ptid != NULL)
				free((data->philos[index]).ptid);
		free(data->philos);
	}
}

/**
 * @brief frees the heap allcated memory in the main data struct
 * @param data pointer to the program strcut (t_data)
*/
void	free_data(t_data *data)
{
	if (data->tid != NULL)
		free(data->tid);
	free_philos(data, data->philo_num);
	if (data->forks != NULL)
		free(data->forks);
	free(data);
}

/**
 * @brief destroys the mutexes in each philosopher
 * @param philo pointer to the philo struct (t_philo)
 * @param number amound of philosers 
*/
void	destroy_philo_mutex(t_philo *philo, int number)
{
	int	index;

	index = -1;
	while (++index < number)
		pthread_mutex_destroy(&philo[index].lock);
}

/**
 * @brief destroys the mutexes in the main program struct
 * @param data pointer to the main program struct (t_data)
 * @param number amount of forks created
 * @param extra check if extra mutexes are created
*/
void	destroy_mutexes(t_data *data, int number, int extra)
{
	int	index;

	index = -1;
	while (++index < number)
		pthread_mutex_destroy(&data->forks[index]);
	if (extra >= 1)
		pthread_mutex_destroy(&data->lock);
	if (extra >= 2)
		pthread_mutex_destroy(&data->write);
}

/**
 * @brief writes the error message and if data isn't null it frees the data
 * @param string the string to print
 * @param data pointer to the program struct (t_data)
 * @return 1 when done
*/
int	write_error(char *string, t_data *data)
{
	size_t	index;

	index = -1;
	while (string[++index] != '\0')
		write(2, &string[index], 1);
	if (data != NULL)
	{
		destroy_philo_mutex(data->philos, data->philo_num);
		destroy_mutexes(data, data->philo_num, 2);
		free_data(data);
	}
	return (1);
}
