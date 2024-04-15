/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: splattje <splattje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 08:48:52 by splattje          #+#    #+#             */
/*   Updated: 2024/04/03 12:16:12 by splattje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	main(int argc, char **argv)
{
	t_data	*data;

	if (!(argc == 5 || argc == 6))
		return (write_error("Not the right amount of arguments\n", NULL), 1);
	data = malloc (sizeof(t_data));
	if (data == NULL)
		return (1);
	if (initialize_program(argv, argc == 6, data))
	{
		free(data);
		return (1);
	}
	set_threads(data);
	destroy_philo_mutex(data->philos, data->philo_num);
	destroy_mutexes(data, data->philo_num, 2);
	free_data(data);
	return (0);
}
