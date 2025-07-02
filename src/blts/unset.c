/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:37:50 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/01 19:39:19 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

// --- CHECCO: inizio funzione ft_unset ---
// Rimuove variabili d'ambiente, stampa errore se nome non valido
int	ft_unset(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!args[i][0])
		{
			ft_putstr_fd("unset: not a valid identifier: ", 2);
			ft_putendl_fd(args[i], 2);
			return (1);
		}
		unsetenv(args[i]);
		i++;
	}
	return (0);
}
// --- CHECCO: fine funzione ft_unset ---
