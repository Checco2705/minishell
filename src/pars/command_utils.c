/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 20:30:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/02 20:19:43 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin_command(const char *cmd_name)
{
	t_builtin	*builtins;
	int			i;

	if (!cmd_name)
		return (0);
	builtins = get_builtins();
	i = 0;
	while (builtins[i].name)
	{
		if (strcmp(cmd_name, builtins[i].name) == 0)
			return (1);
		i++;
	}
	return (0);
}

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = calloc(1, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->in_fd = -1;
	cmd->out_fd = -1;
	cmd->redir_error = 0;
	return (cmd);
}

t_command	*finalize_command(t_command *cmd, int argc, char **argv)
{
	int	i;

	cmd->argv = malloc((argc + 1) * sizeof(char *));
	if (!cmd->argv)
		return (NULL);
	i = 0;
	while (i < argc)
	{
		cmd->argv[i] = argv[i];
		i++;
	}
	cmd->argv[argc] = NULL;
	if (argc == 0 && !cmd->redir_error)
	{
		free(cmd->argv);
		free(cmd);
		return (NULL);
	}
	if (cmd->argv[0])
		cmd->is_builtin = is_builtin_command(cmd->argv[0]);
	return (cmd);
}

void	add_command_to_list(t_command **head, t_command **tail, t_command *cmd)
{
	if (!*head)
		*head = cmd;
	else
		(*tail)->next = cmd;
	*tail = cmd;
}

t_token	*find_next_command(t_token *curr)
{
	while (curr && curr->type != TOKEN_PIPE)
	{
		if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_APPEND)
		{
			curr = curr->next;
			if (curr)
				curr = curr->next;
		}
		else
			curr = curr->next;
	}
	if (curr && curr->type == TOKEN_PIPE)
		curr = curr->next;
	return (curr);
}
