/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:45:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 16:08:40 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include <string.h>

void	sort_env_vars(char **env_copy, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strncmp(env_copy[j], env_copy[j + 1],
					ft_strlen(env_copy[j]) + 1) > 0)
			{
				temp = env_copy[j];
				env_copy[j] = env_copy[j + 1];
				env_copy[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

void	print_exported_vars(void)
{
	int		i;
	int		count;
	char	**env_copy;
	char	**our_environ;

	our_environ = ft_get_environ();
	count = 0;
	while (our_environ && our_environ[count])
		count++;
	if (count == 0)
		return ;
	env_copy = malloc(sizeof(char *) * count);
	if (!env_copy)
		return ;
	i = -1;
	while (++i < count)
		env_copy[i] = our_environ[i];
	sort_env_vars(env_copy, count);
	i = -1;
	while (++i < count)
		print_single_var(env_copy[i]);
	free(env_copy);
}
