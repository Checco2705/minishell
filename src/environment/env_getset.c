/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_getset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:25:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 15:35:16 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

extern char	**environ;

char	*ft_getenv(const char *name)
{
	int		index;
	int		name_len;
	char	**env;

	env = get_our_environ();
	if (!name || !env)
		return (NULL);
	name_len = ft_strlen(name);
	index = 0;
	while (env[index])
	{
		if (ft_strncmp(env[index], name, name_len) == 0
			&& env[index][name_len] == '=')
			return (env[index] + name_len + 1);
		index++;
	}
	return (NULL);
}

char	**ft_get_environ(void)
{
	return (get_our_environ());
}

int	remove_single_var(void)
{
	char	**env;

	env = get_our_environ();
	if (env[0])
		free(env[0]);
	env[0] = NULL;
	return (0);
}

char	**get_our_environ(void)
{
	return (get_static_env());
}

void	set_our_environ(char **new_env)
{
	set_static_env(new_env);
}
