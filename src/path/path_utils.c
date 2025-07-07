/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:40:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 15:43:46 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Trova il prossimo token separato da ':'
*/
char	*get_next_path_token(char **path_ptr)
{
	char	*start;
	char	*end;

	if (!*path_ptr || !**path_ptr)
		return (NULL);
	start = *path_ptr;
	end = start;
	while (*end && *end != ':')
		end++;
	if (*end == ':')
	{
		*end = '\0';
		*path_ptr = end + 1;
	}
	else
		*path_ptr = end;
	return (start);
}
