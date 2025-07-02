/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:02:23 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/02 12:15:19 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>
#include <sys/stat.h>

/*
** Verifica se un file esiste ed è eseguibile
*/
static int	is_executable(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
	{
		if (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR))
			return (1);
	}
	return (0);
}

/*
** Gestisce il caso di comando con path assoluto o relativo
*/
static char	*handle_absolute_path(const char *cmd)
{
	if (strchr(cmd, '/'))
	{
		if (is_executable(cmd))
			return (strdup(cmd));
		return (NULL);
	}
	return ((char *)-1);
}

/*
** Costruisce il percorso completo e verifica se è eseguibile
*/
static char	*build_and_check_path(const char *dir, const char *cmd)
{
	char	*full_path;

	full_path = malloc(strlen(dir) + strlen(cmd) + 2);
	if (!full_path)
		return (NULL);
	sprintf(full_path, "%s/%s", dir, cmd);
	if (is_executable(full_path))
		return (full_path);
	free(full_path);
	return (NULL);
}

/*
** Cerca l'eseguibile in tutti i percorsi di PATH
*/
static char	*search_in_path(const char *cmd, char *path)
{
	char	*token;
	char	*result;

	token = strtok(path, ":");
	while (token)
	{
		result = build_and_check_path(token, cmd);
		if (result)
		{
			free(path);
			return (result);
		}
		token = strtok(NULL, ":");
	}
	free(path);
	return (NULL);
}

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
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	path = strdup(path_env);
	if (!path)
		return (NULL);
	return (search_in_path(cmd, path));
}
