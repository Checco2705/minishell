/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:02:23 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 15:44:49 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>
#include <sys/stat.h>

/*
** Cerca un eseguibile nei percorsi specificati in PATH
** Ritorna il percorso completo se trovato, NULL altrimenti
*/
char	*find_executable(const char *cmd)
{
	char	*path_env;
	char	*path;
	char	*result;

	result = handle_absolute_path(cmd);
	if (result != (char *)-1)
		return (result);
	path_env = ft_getenv("PATH");
	if (!path_env)
		return (NULL);
	path = ft_strdup(path_env);
	if (!path)
		return (NULL);
	return (search_in_path(cmd, path));
}
