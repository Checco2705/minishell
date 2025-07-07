/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:43:23 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 15:28:11 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" 
#include <unistd.h>
#include <limits.h>

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

// Funzione per pulire completamente readline
void	cleanup_readline_and_exit(void)
{
	rl_clear_history();
}

// Inizializza le variabili d'ambiente essenziali
static void	init_essential_vars(void)
{
	char	cwd[PATH_MAX];
	char	*pwd_value;

	pwd_value = ft_getenv("PWD");
	if (!pwd_value)
	{
		if (getcwd(cwd, PATH_MAX))
			ft_setenv("PWD", cwd, 1);
	}
	if (!ft_getenv("SHLVL"))
		ft_setenv("SHLVL", "1", 1);
}

static void	handle_eof(int is_interactive)
{
	if (is_interactive)
		write(1, "exit\n", 5);
}

static void	process_command_line(char *line)
{
	t_command	*commands;

	if (*line)
		add_history(line);
	g_state.signal = 0;
	commands = parse_input(line);
	if (!commands)
		return ;
	g_state.last_status = execute_pipeline(commands);
	cleanup_after_execution(commands);
}

int	main(void)
{
	char		*line;
	int			is_interactive;

	setup_signals();
	init_essential_vars();
	is_interactive = isatty(STDIN_FILENO);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			handle_eof(is_interactive);
			break ;
		}
		process_command_line(line);
		free(line);
	}
	cleanup_readline_and_exit();
	return (g_state.last_status);
}
