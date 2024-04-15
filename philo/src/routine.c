/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: splattje <splattje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:29:27 by splattje          #+#    #+#             */
/*   Updated: 2024/04/08 11:17:36 by splattje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief set start time
 * @param data pointer to program stuct (t_data)
*/
static t_data	*set_start(t_data *data)
{
	int	index;

	if (!data->start_time)
	{
		data->start_time = get_current_time();
		index = -1;
		while (++index < data->philo_num)
		{
			pthread_mutex_lock(&data->philos[index].lock);
			data->philos[index].last_meal_at = data->start_time;
			data->philos[index].start_time = data->start_time;
			data->philos[index].status = 1;
			pthread_mutex_unlock(&data->philos[index].lock);
		}
		return (data);
	}
	return (NULL);
}

/**
 * @brief check if program is finiched
 * @param data pointer to the progam struct (t_data)
 * @return 1 if need to finish, 0 if continue
*/
static int	check_finished(t_data **data, int finished, int number)
{
	int	index;

	if ((*data)->dead || (*data)->finished == (*data)->philo_num)
	{
		index = 0;
		while (index < (*data)->philo_num)
		{
			pthread_mutex_lock(&(*data)->philos[index].lock);
			(*data)->philos[index].status = -1;
			pthread_mutex_unlock(&(*data)->philos[index].lock);
			index++;
		}
		if (finished == 0)
		{
			pthread_mutex_lock(&(*data)->write);
			printf("%zu %d died\n", get_current_time() - (*data)->start_time,
				(*data)->philos[number].id);
			pthread_mutex_unlock(&(*data)->write);
		}
		return (1);
	}
	return (0);
}

/**
 * @brief checks if a philospher has died
 * @param data pointer to the program struct (t_data)
*/
static void	check_death(t_data **data, int index)
{
	while (1)
	{
		pthread_mutex_lock(&(*data)->philos[index].lock);
		if ((*data)->philos[index].status == -1)
			(*data)->dead = 1;
		pthread_mutex_unlock(&(*data)->philos[index].lock);
		if ((*data)->dead || (*data)->finished == (*data)->philo_num)
			if (check_finished(data, (*data)->finished, index))
				break ;
		pthread_mutex_lock(&(*data)->philos[index].lock);
		if ((get_current_time() - (*data)->philos[index].last_meal_at)
			>= (*data)->death_time)
		{
			(*data)->dead = 1;
			pthread_mutex_unlock(&(*data)->philos[index].lock);
			check_finished(data, (*data)->finished, index);
			break ;
		}
		if (!(*data)->dead
			&& (*data)->philos[index].eat_count >= (*data)->meals_nr)
			(*data)->finished++;
		pthread_mutex_unlock(&(*data)->philos[index].lock);
		finished_eating(&(*data), &index);
		index++;
	}
}

/**
 * @brief the thead that checks if the simulation needs to stop
 * @param args void pointer to the program struct (t_data)
 * @return null when done
*/
static void	*checker_routine(void *args)
{
	t_data	*data;

	data = (t_data *)args;
	while (1)
	{
		pthread_mutex_lock(&data->lock);
		if (data->start == 1)
		{
			pthread_mutex_unlock(&data->lock);
			break ;
		}
		pthread_mutex_unlock(&data->lock);
		usleep(500);
	}
	data = set_start(data);
	if (data != NULL)
		check_death(&data, 0);
	return (NULL);
}

/**
 * @brief sets the threads for the simulation
 * @param data pointer to the program struct (t_data)
 * @return 1 on error, 0 on success
*/
int	set_threads(t_data *data)
{
	int	index;

	if (pthread_create(data->tid, NULL, checker_routine, data) != 0)
		return (write_error("thread create failed", data));
	index = -1;
	while (++index < data->philo_num)
	{
		if (pthread_create(data->philos[index].ptid, NULL, philo_routine,
				&data->philos[index]) != 0)
			return (write_error("philo thread create failed", data));
	}
	pthread_mutex_lock(&data->lock);
	data->start = 1;
	pthread_mutex_unlock(&data->lock);
	if (pthread_join(*data->tid, NULL) != 0)
		return (write_error("thread_join", data));
	index = -1;
	while (++index < data->philo_num)
		pthread_join(*data->philos[index].ptid, NULL);
	return (0);
}
