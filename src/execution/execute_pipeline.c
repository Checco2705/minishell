/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:43:23 by ffebbrar          #+#    #+#             */
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

static int	check_command_validity(t_command *commands)
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

static int	handle_single_command(t_command *commands)
{
	pid_t	pid;
	int		check_result;

	check_result = check_command_validity(commands);
	if (check_result != -1)
		return (check_result);
	g_state.executing = 1;
	pid = fork();
	if (pid == 0)
		execute_child(commands);
	else if (pid > 0)
		return (wait_for_single_child(pid));
	else
	{
		g_state.last_status = 1;
		g_state.executing = 0;
	}
	return (g_state.last_status);
}

static int	execute_single_or_pipeline(t_command *commands, int num_cmds)
{
	int	result;

	if (num_cmds == 1)
	{
		result = handle_single_command(commands);
		if (result != -1)
			return (result);
	}
	return (-1);
}

static int	run_pipeline(t_command *commands, int num_cmds)
{
	int		**pipes;
	pid_t	*pids;

	num_cmds = init_pipeline(commands, &pipes, &pids);
	if (num_cmds <= 0)
		return (1);
	g_state.executing = 1;
	execute_all_commands(commands, pipes, pids, num_cmds);
	close_all_pipes(pipes, num_cmds);
	wait_for_children(pids, num_cmds);
	g_state.executing = 0;
	cleanup_resources(pipes, pids, num_cmds);
	return (g_state.last_status);
}

int	execute_pipeline(t_command *commands)
{
	int	num_cmds;
	int	result;

	if (!commands)
	{
		g_state.last_status = 0;
		return (0);
	}
	num_cmds = count_commands(commands);
	result = execute_single_or_pipeline(commands, num_cmds);
	if (result != -1)
		return (result);
	return (run_pipeline(commands, num_cmds));
}
