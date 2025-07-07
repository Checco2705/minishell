/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 14:30:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/07 15:00:00 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <errno.h>

static int	process_heredoc_input(char *line, char *delimiter, int fd,
	int has_quotes)
{
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
		&& ft_strlen(line) == ft_strlen(delimiter))
	{
		free(line);
		return (1);
	}
	if (expand_heredoc_line(line, fd, has_quotes) == -1)
	{
		free(line);
		return (-1);
	}
	free(line);
	return (0);
}

static int	read_heredoc_loop(char *clean_delimiter, int fd, int has_quotes)
{
	char	*line;
	int		result;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_fprintf(2, "minishell: warning: here-document delimited "
				"by end-of-file (wanted `%s')\n", clean_delimiter);
			break ;
		}
		result = process_heredoc_input(line, clean_delimiter, fd, has_quotes);
		if (result != 0)
			break ;
	}
	if (result == -1)
		return (-1);
	return (0);
}

int	read_heredoc_content(char *delimiter, int fd)
{
	char	*clean_delimiter;
	int		has_quotes;
	int		result;

	clean_delimiter = process_token_quotes(delimiter);
	if (!clean_delimiter)
		return (-1);
	has_quotes = (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '"'));
	result = read_heredoc_loop(clean_delimiter, fd, has_quotes);
	free(clean_delimiter);
	return (result);
}

int	handle_single_heredoc(t_command *cmd, t_token *curr)
{
	char	*filename;
	int		fd;

	filename = create_temp_filename();
	if (!filename)
		return (-1);
	if (setup_heredoc_file(filename, curr->next->value) == -1)
	{
		unlink(filename);
		free(filename);
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	unlink(filename);
	free(filename);
	if (fd == -1)
		return (-1);
	if (cmd->in_fd >= 0)
		close(cmd->in_fd);
	cmd->in_fd = fd;
	return (0);
}

int	handle_multiple_heredocs(t_command *cmd, t_token *start_token)
{
	int	count;

	count = count_heredocs(start_token);
	if (count <= 1)
		return (handle_single_heredoc(cmd, start_token));
	while (start_token && start_token->type != TOKEN_PIPE)
	{
		if (start_token->type == TOKEN_HEREDOC)
		{
			if (handle_single_heredoc(cmd, start_token) == -1)
				return (-1);
		}
		start_token = start_token->next;
	}
	return (0);
}
