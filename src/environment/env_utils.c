/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:20:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 16:07:59 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

extern char	**environ;

static void	update_existing_var(int index, char *new_var, int overwrite)
{
	char	**env;

	env = get_our_environ();
	if (overwrite)
	{
		free(env[index]);
		env[index] = new_var;
	}
	else
		free(new_var);
}

static void	add_new_var(char *new_var, int count)
{
	char	**new_environ;
	char	**env;
	int		i;

	env = get_our_environ();
	new_environ = malloc(sizeof(char *) * (count + 2));
	if (!new_environ)
	{
		free(new_var);
		return ;
	}
	i = 0;
	while (i < count)
	{
		new_environ[i] = env[i];
		i++;
	}
	new_environ[i] = new_var;
	new_environ[i + 1] = NULL;
	set_our_environ(new_environ);
}

int	ft_setenv(const char *name, const char *value, int overwrite)
{
	char	*new_var;
	int		index;
	int		count;

	if (!name || !value || ft_strchr(name, '='))
		return (-1);
	new_var = create_env_string(name, value);
	if (!new_var)
		return (-1);
	index = find_env_index(name);
	if (index >= 0)
	{
		update_existing_var(index, new_var, overwrite);
		return (0);
	}
	count = count_env_vars();
	add_new_var(new_var, count);
	return (0);
}

static void	remove_var_from_array(int index, int count)
{
	char	**new_environ;
	char	**env;
	int		i;
	int		j;

	env = get_our_environ();
	new_environ = malloc(sizeof(char *) * count);
	if (!new_environ)
		return ;
	i = 0;
	j = 0;
	while (i < count)
	{
		if (i != index)
			new_environ[j++] = env[i];
		else
			free(env[i]);
		i++;
	}
	new_environ[j] = NULL;
	set_our_environ(new_environ);
}

int	ft_unsetenv(const char *name)
{
	int		index;
	int		count;
	char	**env;

	if (!name || ft_strchr(name, '='))
		return (-1);
	index = find_env_index(name);
	if (index < 0)
		return (0);
	count = count_env_vars();
	if (count <= 1)
	{
		env = get_our_environ();
		if (env[0])
			free(env[0]);
		env[0] = NULL;
		return (0);
	}
	remove_var_from_array(index, count);
	return (0);
}
