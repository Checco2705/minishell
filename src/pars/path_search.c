/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_search.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:45:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 15:45:45 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>

/*
** Gestisce il caso di comando con path assoluto o relativo
*/
char	*handle_absolute_path(const char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (is_executable(cmd))
			return (ft_strdup(cmd));
		return (NULL);
	}
	return ((char *)-1);
}

/*
** Cerca l'eseguibile in tutti i percorsi di PATH
*/
char	*search_in_path(const char *cmd, char *path)
{
	char	*token;
	char	*result;
	char	*path_ptr;

	path_ptr = path;
	token = get_next_path_token(&path_ptr);
	while (token != NULL)
	{
		result = build_and_check_path(token, cmd);
		if (result)
		{
			free(path);
			return (result);
		}
		token = get_next_path_token(&path_ptr);
	}
	free(path);
	return (NULL);
}
