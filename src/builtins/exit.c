/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:17:03 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 14:47:46 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

// Controlla se una stringa rappresenta un numero valido (con possibile segno)
static int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **args)
{
	int	code;

	code = 0;
	if (args[1] && args[2])
	{
		ft_fprintf(2, "exit: troppi argomenti\n");
		return (1);
	}
	if (args[1])
	{
		if (!is_valid_number(args[1]))
		{
			ft_fprintf(2, "exit: %s: è necessario un argomento numerico\n",
				args[1]);
			exit(2);
		}
		code = ft_atoi(args[1]);
		code = code & 0xFF;
	}
	exit(code);
}
