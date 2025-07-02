/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:27:59 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/01 23:10:49 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Controlla se un nome è un identificatore valido per export
static int	is_valid_identifier(const char *name)
{
	int	i;

	i = 0;
	if (!name || !name[0])
		return (0);
	if (!isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// Gestisce export con formato VAR=value
static int	process_var_with_value(char *arg, char *eq)
{
	*eq = '\0';
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("export: not a valid identifier: ", 2);
		ft_putendl_fd(arg, 2);
		*eq = '=';
		return (1);
	}
	else
	{
		setenv(arg, eq + 1, 1);
	}
	*eq = '=';
	return (0);
}

// Gestisce export con formato VAR (senza valore)
static int	process_var_without_value(char *arg)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("export: not a valid identifier: ", 2);
		ft_putendl_fd(arg, 2);
		return (1);
	}
	return (0);
}

// Elabora un singolo argomento di export
static int	process_export_arg(char *arg)
{
	char	*eq;

	eq = strchr(arg, '=');
	if (eq)
		return (process_var_with_value(arg, eq));
	else
		return (process_var_without_value(arg));
}

// --- CHECCO: inizio funzione ft_export ---
// Aggiunge o aggiorna variabili d'ambiente, stampa errore se nome non valido
int	ft_export(char **args)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!args[1])
		return (ft_env(args));
	while (args[i])
	{
		if (process_export_arg(args[i]))
			status = 1;
		i++;
	}
	return (status);
}
