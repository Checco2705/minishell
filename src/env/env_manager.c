/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:15:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 16:14:24 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include <string.h>

extern char	**environ;

char	**get_static_env(void)
{
	if (!g_state.local_env)
		g_state.local_env = copy_environ_vars();
	return (g_state.local_env);
}

void	set_static_env(char **new_env)
{
	if (g_state.local_env)
		free(g_state.local_env);
	g_state.local_env = new_env;
}

char	**copy_environ_vars(void)
{
	char	**new_environ;
	int		count;
	int		i;

	count = 0;
	while (environ[count])
		count++;
	new_environ = malloc(sizeof(char *) * (count + 1));
	if (!new_environ)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_environ[i] = ft_strdup(environ[i]);
		if (!new_environ[i])
		{
			while (--i >= 0)
				free(new_environ[i]);
			free(new_environ);
			return (NULL);
		}
		i++;
	}
	new_environ[i] = NULL;
	return (new_environ);
}

int	find_env_index(const char *name)
{
	int		i;
	int		name_len;
	char	**env;

	env = get_static_env();
	if (!name || !env)
		return (-1);
	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0
			&& env[i][name_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	count_env_vars(void)
{
	int		count;
	char	**env;

	env = get_static_env();
	count = 0;
	if (env)
	{
		while (env[count])
			count++;
	}
	return (count);
}
