/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:43:23 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/02 22:05:27 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

extern char	**environ;

static int	check_file_permissions(char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			fprintf(stderr, "minishell: %s: È una directory\n", path);
			return (126);
		}
		if (!(st.st_mode & S_IXUSR))
		{
			fprintf(stderr, "minishell: %s: Permesso negato\n", path);
			return (126);
		}
		return (0);
	}
	fprintf(stderr, "minishell: %s: File o directory non esistente\n", path);
	return (127);
}

static void	execute_absolute_path(t_command *cmd)
{
	int	status;

	status = check_file_permissions(cmd->argv[0]);
	if (status != 0)
		exit(status);
	execve(cmd->argv[0], cmd->argv, environ);
	perror("execve");
	exit(126);
}

void	execute_child(t_command *cmd)
{
	if (cmd->redir_error)
		exit(1);
	if (cmd->is_builtin)
	{
		execute_builtin(cmd);
		exit(g_state.last_status);
	}
	if (!cmd->argv || !cmd->argv[0] || strlen(cmd->argv[0]) == 0)
		exit(0);
	if (strchr(cmd->argv[0], '/'))
	{
		execute_absolute_path(cmd);
		return ;
	}
	if (!cmd->path && cmd->argv && cmd->argv[0])
		cmd->path = find_executable(cmd->argv[0]);
	if (cmd->path)
	{
		execve(cmd->path, cmd->argv, environ);
		perror("execve");
		exit(126);
	}
	fprintf(stderr, "minishell: %s: command not found\n", cmd->argv[0]);
	exit(127);
}
