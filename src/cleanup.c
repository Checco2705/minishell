/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:43:23 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/02 22:07:10 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

static void	cleanup_command_fds(t_command *cmd)
{
	if (cmd->in_fd >= 0)
	{
		close(cmd->in_fd);
		cmd->in_fd = -1;
	}
	if (cmd->out_fd >= 0)
	{
		close(cmd->out_fd);
		cmd->out_fd = -1;
	}
}

static void	cleanup_command_argv(t_command *cmd)
{
	int	i;

	if (cmd->argv)
	{
		i = 0;
		while (cmd->argv[i])
		{
			free(cmd->argv[i]);
			i++;
		}
		free(cmd->argv);
	}
}

void	cleanup_after_execution(t_command *commands)
{
	t_command	*current;
	t_command	*next;

	current = commands;
	while (current)
	{
		next = current->next;
		cleanup_command_fds(current);
		cleanup_command_argv(current);
		if (current->path)
			free(current->path);
		free(current);
		current = next;
	}
}
