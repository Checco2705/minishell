/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:42:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 15:45:17 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>
#include <sys/stat.h>

/*
** Verifica se un file esiste ed è eseguibile
*/
int	is_executable(const char *path)
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
** Copia la directory nel percorso completo
*/
static void	copy_directory(char *full_path, const char *dir, int dir_len)
{
	int	i;

	i = 0;
	while (i < dir_len)
	{
		full_path[i] = dir[i];
		i++;
	}
	full_path[i] = '/';
}

/*
** Copia il comando nel percorso completo
*/
static void	copy_command(char *full_path, const char *cmd, int start, int len)
{
	int	j;

	j = 0;
	while (j < len)
	{
		full_path[start + j] = cmd[j];
		j++;
	}
	full_path[start + len] = '\0';
}

/*
** Costruisce il percorso completo concatenando directory e comando
*/
char	*build_path_string(const char *dir, const char *cmd)
{
	char	*full_path;
	int		dir_len;
	int		cmd_len;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL);
	copy_directory(full_path, dir, dir_len);
	copy_command(full_path, cmd, dir_len + 1, cmd_len);
	return (full_path);
}

/*
** Costruisce il percorso completo e verifica se è eseguibile
*/
char	*build_and_check_path(const char *dir, const char *cmd)
{
	char	*full_path;

	full_path = build_path_string(dir, cmd);
	if (!full_path)
		return (NULL);
	if (is_executable(full_path))
		return (full_path);
	free(full_path);
	return (NULL);
}
