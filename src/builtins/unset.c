/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:37:50 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 14:20:33 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>

// --- CHECCO: inizio funzione ft_unset ---
// Rimuove variabili d'ambiente, stampa errore se nome non valido
int	ft_unset(char **args)
{
	int	i;
	int	status;

	status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			write(2, "minishell: unset: `", 19);
			write(2, args[i], ft_strlen(args[i]));
			write(2, "': not a valid identifier\n", 26);
			status = 1;
		}
		else
			ft_unsetenv(args[i]);
		i++;
	}
	return (status);
}
// --- CHECCO: fine funzione ft_unset ---
