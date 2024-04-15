/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: splattje <splattje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 08:51:40 by splattje          #+#    #+#             */
/*   Updated: 2024/04/08 09:55:39 by splattje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief parses times from arguments
 * @param argv the 2D array of the strings needed for parsing
 * @param flag check if optional argument was givven
 * @return the parsed numbers on success, NULL on error or overflow
*/
static t_token	*parse_input(char **argv, int flag)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	if (ft_atoi(argv[1], &token->amount_of_philo))
		return (free(token), NULL);
	if (ft_atoi(argv[2], &token->time_to_die))
		return (free(token), NULL);
	if (ft_atoi(argv[3], &token->time_to_eat))
		return (free(token), NULL);
	if (ft_atoi(argv[4], &token->time_to_sleep))
		return (free(token), NULL);
	if (flag)
	{
		if (ft_atoi(argv[5], &token->times_eaten))
			return (free(token), NULL);
	}
	else
		token->times_eaten = INT_MAX;
	return (token);
}

/** 
 * @brief creates the forks needed for the program
 * @param data pointer to the program struct (t_data)
 * @param amount the amount of forks needed
 * @return returns -1 on success, the index of on witch fork we failed on
 * creating error
*/
static int	set_forks(t_data *data, int amount)
{
	int	index;

	index = -1;
	while (++index < amount)
	{
		if (pthread_mutex_init(&data->forks[index], NULL) != 0)
			return (index);
	}
	return (-1);
}

/**
 * @brief creates the program struct
 * @param token the parsed numbers struct (t_token)
 * @param data pointer to the program struct (t_data)
 * @return 1 on success, 0 on error
*/
static int	program(t_token *token, t_data *data)
{
	int		forks;

	memset_program(&data);
	data->tid = malloc(sizeof(pthread_t));
	if (data->tid == NULL)
		return (free_data(data), 0);
	data->philo_num = token->amount_of_philo;
	data->meals_nr = token->times_eaten;
	data->philos = malloc(sizeof(t_philo) * token->amount_of_philo);
	if (data->philos == NULL)
		return (free_data(data), 0);
	data->death_time = token->time_to_die;
	data->eat_time = token->time_to_eat;
	data->sleep_time = token->time_to_sleep;
	data->forks = malloc(sizeof(pthread_mutex_t) * token->amount_of_philo);
	if (data->forks == NULL)
		return (free_data(data), 0);
	forks = set_forks(data, data->philo_num);
	if (forks != -1)
		return (destroy_mutexes(data, forks, 0), free_data(data), 0);
	if (pthread_mutex_init(&data->lock, NULL) != 0)
		return (destroy_mutexes(data, data->philo_num, 0), free_data(data), 0);
	if (pthread_mutex_init(&data->write, NULL) != 0)
		return (destroy_mutexes(data, data->philo_num, 1), free_data(data), 0);
	return (1);
}

/**
 * @brief creates the philo's structs
 * @param data pointer to the program struct (t_data)
 * @return 1 on success, 0 on failure
*/
static int	set_philos(t_data **data)
{
	int		index;

	index = -1;
	while (++index < (*data)->philo_num)
	{
		memset_philo(&(*data)->philos[index], &(*data), index);
		(*data)->philos[index].ptid = malloc(sizeof(pthread_t));
		if ((*data)->philos[index].ptid == NULL)
			return (0);
		(*data)->philos[index].id = index + 1;
		(*data)->philos[index].time_to_eat = (*data)->eat_time;
		(*data)->philos[index].time_to_die = (*data)->death_time;
		(*data)->philos[index].time_to_sleep = (*data)->sleep_time;
		if (pthread_mutex_init(&((*data)->philos[index]).lock, NULL) != 0)
			return (destroy_philo_mutex((*data)->philos, index),
				free((*data)->philos[index].ptid), 0);
		(*data)->philos[index].l_fork = &(*data)->forks[index];
		(*data)->philos[index].write = &(*data)->write;
	}
	return (1);
}

/**
 * @brief initializes the date for the start
 * @param argv 2D array of strings from the argucments
 * @param is_optional flag if the optional arugment is given
 * @param data pointer to the program struct (t_data)
 * @return returns 1 on error, 0 success
*/
int	initialize_program(char **argv, int is_optional, t_data *data)
{
	t_token	*token;

	token = parse_input(argv, is_optional);
	if (token == NULL)
		return (1);
	if (program(token, data) == 0)
		return (1);
	free(token);
	if (set_philos(&data) == 0)
		return (destroy_mutexes(data, data->philo_num, 2), free_data(data), 1);
	return (0);
}
