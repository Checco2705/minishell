/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:08:01 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/01 23:06:49 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <string.h>

extern char	**environ;

int	ft_env(char **args)
{
	int	i;

	(void)args;
	i = 0;
	while (environ && environ[i])
	{
		if (strncmp(environ[i], "COLUMNS=", 8) != 0
			&& strncmp(environ[i], "LINES=", 6) != 0)
		{
			ft_putendl_fd(environ[i], 1);
		}
		i++;
	}
	return (0);
}
