/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:43:23 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/02 22:01:48 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>

extern char	**environ;

void	execute_all_commands(t_command *commands, int **pipes,
		pid_t *pids, int num_cmds)
{
	int			i;
	t_command	*current;

	i = 0;
	current = commands;
	while (i < num_cmds && current)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			cleanup_resources(pipes, pids, num_cmds);
			return ;
		}
		if (pids[i] == 0)
		{
			setup_child_pipes(current, pipes, num_cmds, i);
			execute_child(current);
			exit(1);
		}
		i++;
		current = current->next;
	}
}

void	close_all_pipes(int **pipes, int num_cmds)
{
	int	i;

	if (pipes)
	{
		i = 0;
		while (i < num_cmds - 1)
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			i++;
		}
	}
}

void	setup_child_pipes(t_command *cmd, int **pipes, int num_cmds, int i)
{
	int	j;

	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < num_cmds - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	j = 0;
	while (j < num_cmds - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
	if (cmd->in_fd >= 0)
	{
		dup2(cmd->in_fd, STDIN_FILENO);
		close(cmd->in_fd);
	}
	if (cmd->out_fd >= 0)
	{
		dup2(cmd->out_fd, STDOUT_FILENO);
		close(cmd->out_fd);
	}
}

void	wait_for_children(pid_t *pids, int num_cmds)
{
	int	k;
	int	status;

	k = 0;
	while (k < num_cmds)
	{
		waitpid(pids[k], &status, 0);
		if (WIFEXITED(status))
			g_state.last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_state.last_status = 128 + WTERMSIG(status);
		k++;
	}
}

void	cleanup_resources(int **pipes, pid_t *pids, int num_cmds)
{
	int	j;

	if (num_cmds > 1)
	{
		j = 0;
		while (j < num_cmds - 1)
		{
			free(pipes[j]);
			j++;
		}
		free(pipes);
	}
	free(pids);
}
