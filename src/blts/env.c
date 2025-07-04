/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:08:01 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 15:38:12 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <string.h>

static int	should_skip_var(char *env_var, int is_terminal)
{
	if (!is_terminal
		&& (ft_strncmp(env_var, "COLUMNS=", 8) == 0
			|| ft_strncmp(env_var, "LINES=", 6) == 0))
		return (1);
	return (0);
}

static int	is_valid_env_var(char *env_var)
{
	char	*eq_pos;

	eq_pos = ft_strchr(env_var, '=');
	if (eq_pos && eq_pos[1] != '\001')
		return (1);
	return (0);
}

static void	print_env_vars(char **our_environ, int is_terminal)
{
	int	i;

	i = 0;
	while (our_environ && our_environ[i])
	{
		if (is_valid_env_var(our_environ[i]))
		{
			if (!should_skip_var(our_environ[i], is_terminal))
				ft_putendl_fd(our_environ[i], 1);
		}
		i++;
	}
}

int	ft_env(char **args)
{
	int		is_terminal;
	char	**our_environ;

	(void)args;
	is_terminal = isatty(STDOUT_FILENO);
	our_environ = ft_get_environ();
	print_env_vars(our_environ, is_terminal);
	return (0);
}
