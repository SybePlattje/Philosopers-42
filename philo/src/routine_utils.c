/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: splattje <splattje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 09:59:23 by splattje          #+#    #+#             */
/*   Updated: 2024/04/08 11:39:01 by splattje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	finished_eating(t_data **data, int *index)
{
	int	i;

	if (!(*data)->dead && *index == (*data)->philo_num - 1)
	{
		if ((*data)->finished != (*data)->philo_num)
			(*data)->finished = 0;
		*index = -1;
	}
	if ((*data)->finished == (*data)->philo_num)
	{
		i = 0;
		while (i < (*data)->philo_num)
		{
			pthread_mutex_lock(&(*data)->philos[i].lock);
			(*data)->philos[i].status = -1;
			pthread_mutex_unlock(&(*data)->philos[i].lock);
			i++;
		}
	}
}
