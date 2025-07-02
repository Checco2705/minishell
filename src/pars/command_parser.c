/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 20:30:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/02 20:21:17 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*process_redirection(t_command *cmd, t_token *curr)
{
	if (handle_redirection(cmd, curr) == -1)
	{
		cmd->redir_error = 1;
		return (NULL);
	}
	curr = curr->next;
	if (curr)
		curr = curr->next;
	return (curr);
}

static t_token	*extract_arguments(t_token *curr, int *argc, char **argv,
	t_command *cmd)
{
	*argc = 0;
	while (curr && curr->type != TOKEN_PIPE)
	{
		if (curr->type == TOKEN_WORD)
		{
			argv[(*argc)++] = strdup(curr->value);
			curr = curr->next;
		}
		else if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_APPEND)
		{
			curr = process_redirection(cmd, curr);
			if (!curr)
				break ;
		}
		else
			curr = curr->next;
	}
	argv[*argc] = NULL;
	return (curr);
}

static t_command	*init_command(t_token *curr)
{
	t_command	*cmd;
	char		*local_argv[256];
	int			local_argc;

	cmd = create_command();
	if (!cmd)
		return (NULL);
	extract_arguments(curr, &local_argc, local_argv, cmd);
	return (finalize_command(cmd, local_argc, local_argv));
}

t_command	*build_commands(t_token *tokens)
{
	t_command	*head;
	t_command	*tail;
	t_command	*cmd;
	t_token		*curr;

	head = NULL;
	tail = NULL;
	curr = tokens;
	while (curr)
	{
		cmd = init_command(curr);
		if (cmd)
			add_command_to_list(&head, &tail, cmd);
		curr = find_next_command(curr);
	}
	return (head);
}
