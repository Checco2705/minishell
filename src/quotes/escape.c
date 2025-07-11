/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escape.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:15:01 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/01 19:47:04 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Processa un carattere di escape.
** 
** @param str: La stringa da processare
** @param i: Puntatore all'indice corrente nella stringa
** @param result: Buffer dove salvare il risultato
** @param j: Puntatore all'indice corrente nel buffer risultato
** @return: 1 se l'escape è stato processato, 0 altrimenti
*/
int	process_escape(const char *str, int *i, char *result, int *j)
{
	char	c;

	(*i)++;
	c = str[*i];
	if (!c)
		return (0);
	if (c == '"' || c == '\\' || c == '$')
		result[(*j)++] = c;
	else if (c == 'n')
		result[(*j)++] = '\n';
	else if (c == 't')
		result[(*j)++] = '\t';
	else
	{
		result[(*j)++] = '\\';
		result[(*j)++] = c;
	}
	(*i)++;
	return (1);
}
