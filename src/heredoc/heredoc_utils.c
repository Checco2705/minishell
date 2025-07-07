/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:00:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/07 15:00:00 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <errno.h>

int	write_expanded_line(char *line, int fd)
{
	char	*expanded;

	expanded = expand_string(line);
	if (!expanded)
		return (-1);
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
	return (0);
}

int	count_heredocs(t_token *start)
{
	t_token	*current;
	int		count;

	count = 0;
	current = start;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_HEREDOC)
			count++;
		current = current->next;
	}
	return (count);
}

int	expand_heredoc_line(char *line, int fd, int has_quotes)
{
	if (has_quotes)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		return (0);
	}
	return (write_expanded_line(line, fd));
}

int	setup_heredoc_file(char *filename, char *delimiter)
{
	int	fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		return (-1);
	if (read_heredoc_content(delimiter, fd) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
