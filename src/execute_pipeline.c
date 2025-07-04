/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:43:23 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 13:03:08 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>

extern char	**environ;

static int	handle_single_command(t_command *commands)
{
	if (commands->redir_error)
		return (1);
	if (commands->is_builtin)
	{
		execute_builtin(commands);
		return (g_state.last_status);
	}
	if (!commands->argv || !commands->argv[0]
		|| ft_strlen(commands->argv[0]) == 0)
	{
		g_state.last_status = 0;
		return (0);
	}
	return (-1);
}

int	execute_pipeline(t_command *commands)
{
	int			**pipes;
	pid_t		*pids;
	int			num_cmds;
	int			result;

	if (!commands)
	{
		g_state.last_status = 0;
		return (0);
	}
	num_cmds = count_commands(commands);
	if (num_cmds == 1)
	{
		result = handle_single_command(commands);
		if (result != -1)
			return (result);
	}
	num_cmds = init_pipeline(commands, &pipes, &pids);
	if (num_cmds <= 0)
		return (1);
	execute_all_commands(commands, pipes, pids, num_cmds);
	close_all_pipes(pipes, num_cmds);
	wait_for_children(pids, num_cmds);
	cleanup_resources(pipes, pids, num_cmds);
	return (g_state.last_status);
}

int	count_commands(t_command *commands)
{
	int			count;
	t_command	*cmd;

	count = 0;
	cmd = commands;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

int	**create_pipes(int num_cmds)
{
	int	**pipes;
	int	i;

	if (num_cmds <= 1)
		return (NULL);
	pipes = malloc(sizeof(int *) * (num_cmds - 1));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < num_cmds - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) < 0)
		{
			perror("pipe");
			while (--i >= 0)
				free(pipes[i]);
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

int	init_pipeline(t_command *commands, int ***pipes, pid_t **pids)
{
	int	num_cmds;

	num_cmds = count_commands(commands);
	if (num_cmds == 0)
		return (0);
	*pipes = create_pipes(num_cmds);
	if (num_cmds > 1 && !*pipes)
		return (0);
	*pids = malloc(sizeof(pid_t) * num_cmds);
	if (!*pids)
	{
		cleanup_resources(*pipes, NULL, num_cmds);
		return (0);
	}
	return (num_cmds);
}
