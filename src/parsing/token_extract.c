/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extract.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 00:00:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 15:07:51 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_escape_in_quotes(const char *input, int *i, int *len)
{
	if (input[*i] == '\\' && input[*i + 1])
	{
		(*i) += 2;
		*len += 2;
	}
	else
	{
		(*i)++;
		(*len)++;
	}
}

char	*extract_quoted_word(const char *input, int *i, char quote)
{
	int		start;
	int		len;
	char	*out;

	start = *i + 1;
	len = 0;
	(*i)++;
	while (input[*i] && input[*i] != quote)
	{
		if (quote == '"')
			handle_escape_in_quotes(input, i, &len);
		else
		{
			(*i)++;
			len++;
		}
	}
	out = ft_substr(input, start, len);
	if (input[*i] == quote)
		(*i)++;
	return (out);
}

static void	handle_quoted_section(const char *input, int *i)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
	{
		if (quote == '"' && input[*i] == '\\' && input[*i + 1])
			(*i) += 2;
		else
			(*i)++;
	}
	if (input[*i] == quote)
		(*i)++;
}

static int	is_word_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}

char	*extract_word(const char *input, int *i)
{
	int		start;
	int		len;
	char	*out;

	start = *i;
	while (input[*i] && !is_word_delimiter(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
			handle_quoted_section(input, i);
		else
			(*i)++;
	}
	len = *i - start;
	out = ft_substr(input, start, len);
	return (out);
}
