/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:00:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/07 15:00:00 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>

extern char	**environ;

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

static int	create_single_pipe(int **pipes, int i)
{
	pipes[i] = malloc(sizeof(int) * 2);
	if (!pipes[i] || pipe(pipes[i]) < 0)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}

static void	cleanup_pipes_on_error(int **pipes, int count)
{
	while (--count >= 0)
		free(pipes[count]);
	free(pipes);
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
		if (create_single_pipe(pipes, i) == -1)
		{
			cleanup_pipes_on_error(pipes, i);
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
