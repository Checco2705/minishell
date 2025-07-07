/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:27:59 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 14:42:15 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include <string.h>

int	is_valid_identifier(const char *name)
{
	int	i;

	i = 0;
	if (!name || !name[0])
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	process_var_with_value(char *arg, char *eq)
{
	*eq = '\0';
	if (is_valid_identifier(arg))
	{
		ft_setenv(arg, eq + 1, 1);
		*eq = '=';
		return (0);
	}
	else
	{
		write(2, "minishell: export: `", 20);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 26);
		*eq = '=';
		return (1);
	}
}

static int	process_var_without_value(char *arg)
{
	if (is_valid_identifier(arg))
	{
		if (!ft_getenv(arg))
			ft_setenv(arg, "\001", 1);
		return (0);
	}
	else
	{
		write(2, "minishell: export: `", 20);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 26);
		return (1);
	}
}

static int	process_export_arg(char *arg)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
		return (process_var_with_value(arg, eq));
	else
		return (process_var_without_value(arg));
}

int	ft_export(char **args)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!args[1])
	{
		print_exported_vars();
		return (0);
	}
	while (args[i])
	{
		if (process_export_arg(args[i]))
			status = 1;
		i++;
	}
	return (status);
}
