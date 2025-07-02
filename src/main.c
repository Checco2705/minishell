/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:43:23 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/02 21:43:23 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" 

// Funzione per pulire completamente readline
void	cleanup_readline_and_exit(void)
{
	rl_clear_history();
	rl_cleanup_after_signal();
	rl_reset_terminal(NULL);
	rl_set_prompt("");
}

int	main(void)
{
	char		*line;
	t_command	*commands;

	setup_signals();
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		g_state.signal = 0;
		commands = parse_input(line);
		free(line);
		if (!commands)
			continue ;
		execute_pipeline(commands);
		cleanup_after_execution(commands);
	}
	cleanup_readline_and_exit();
	return (g_state.last_status);
}
