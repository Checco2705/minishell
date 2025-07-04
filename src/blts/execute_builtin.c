/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 23:18:19 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 13:16:44 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_builtin	*get_builtins(void)
{
	static t_builtin	builtins[] = {
	{"echo", ft_echo},
	{"cd", ft_cd},
	{"pwd", ft_pwd},
	{"export", ft_export},
	{"unset", ft_unset},
	{"env", ft_env},
	{"exit", ft_exit},
	{NULL, NULL}
	};

	return (builtins);
}

static void	save_std_fds(int *saved_stdin, int *saved_stdout, t_command *cmd)
{
	if (cmd->in_fd >= 0)
	{
		*saved_stdin = dup(STDIN_FILENO);
		dup2(cmd->in_fd, STDIN_FILENO);
		close(cmd->in_fd);
	}
	if (cmd->out_fd >= 0)
	{
		*saved_stdout = dup(STDOUT_FILENO);
		dup2(cmd->out_fd, STDOUT_FILENO);
		close(cmd->out_fd);
	}
}

static void	restore_std_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin >= 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout >= 0)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

static int	run_builtin_command(const char *cmd_name, char **argv)
{
	int			i;
	int			result;
	t_builtin	*builtins;

	i = 0;
	result = 1;
	builtins = get_builtins();
	while (builtins[i].name)
	{
		if (ft_strncmp(cmd_name, builtins[i].name,
				ft_strlen(builtins[i].name) + 1) == 0)
			return (builtins[i].func(argv));
		i++;
	}
	return (result);
}

void	execute_builtin(t_command *cmd)
{
	const char	*cmd_name;
	int			saved_stdin;
	int			saved_stdout;

	saved_stdin = -1;
	saved_stdout = -1;
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
	save_std_fds(&saved_stdin, &saved_stdout, cmd);
	cmd_name = cmd->argv[0];
	g_state.last_status = run_builtin_command(cmd_name, cmd->argv);
	restore_std_fds(saved_stdin, saved_stdout);
}
