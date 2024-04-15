/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: splattje <splattje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 10:58:32 by splattje          #+#    #+#             */
/*   Updated: 2024/04/08 11:18:01 by splattje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <limits.h>

typedef struct s_token
{
	int	amount_of_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	times_eaten;
}	t_token;

typedef struct s_philo
{
	pthread_t		*ptid;
	int				id;
	int				eat_count;
	int				status;
	size_t			time_to_eat;
	size_t			time_to_die;
	size_t			last_meal_at;
	size_t			time_to_sleep;
	size_t			start_time;
	pthread_mutex_t	lock;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*write;
}	t_philo;

typedef struct s_data
{
	pthread_t		*tid;
	int				start;
	int				philo_num;
	int				meals_nr;
	int				dead;
	int				finished;
	t_philo			*philos;
	size_t			death_time;
	size_t			eat_time;
	size_t			sleep_time;
	size_t			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;
}	t_data;

// initializer
int		initialize_program(char **argv, int is_optional, t_data *data);

// time
size_t	get_current_time(void);
int		ft_usleep(size_t miliseconds);
int		ft_usleep_sleep(size_t miliseconds, t_philo *philo);

// utils
int		ft_atoi(char *nptr, int *nr);
int		what_to_print(int status, t_philo **philo);
void	memset_program(t_data **data);
void	memset_philo(t_philo *philo, t_data **data, int index);

// routine
int		set_threads(t_data *data);

//philo_routine
void	*philo_routine(void *args);

// error_and_free
int		write_error(char *string, t_data *data);
void	destroy_mutexes(t_data *data, int number, int extra);
void	destroy_philo_mutex(t_philo *philo, int number);
void	free_data(t_data *data);

// eating
int		is_eating(t_philo **philo);

// eating_util
int		eating(t_philo **philo);

// sleeping
int		is_sleeping(t_philo **philo);

//routine_utils
void	finished_eating(t_data **data, int *index);
#endif