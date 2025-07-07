/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:50:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 13:04:30 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <string.h>

void	print_var_without_value(char *env_var, char *eq_pos)
{
	*eq_pos = '\0';
	ft_putendl_fd(env_var, 1);
	*eq_pos = '=';
}

void	print_var_with_empty_value(char *env_var, char *eq_pos)
{
	*eq_pos = '\0';
	ft_putstr_fd(env_var, 1);
	ft_putstr_fd("=\"\"", 1);
	*eq_pos = '=';
	ft_putchar_fd('\n', 1);
}

void	print_var_with_value(char *env_var, char *eq_pos)
{
	*eq_pos = '\0';
	ft_putstr_fd(env_var, 1);
	ft_putstr_fd("=\"", 1);
	ft_putstr_fd(eq_pos + 1, 1);
	ft_putstr_fd("\"", 1);
	*eq_pos = '=';
	ft_putchar_fd('\n', 1);
}

void	print_single_var(char *env_var)
{
	char	*eq_pos;

	ft_putstr_fd("declare -x ", 1);
	eq_pos = ft_strchr(env_var, '=');
	if (eq_pos)
	{
		if (eq_pos[1] == '\001')
			print_var_without_value(env_var, eq_pos);
		else if (eq_pos[1] == '\0')
			print_var_with_empty_value(env_var, eq_pos);
		else
			print_var_with_value(env_var, eq_pos);
	}
	else
		ft_putendl_fd(env_var, 1);
}
