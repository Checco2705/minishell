/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 20:25:00 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/02 20:33:16 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_single_quotes(char *src, int *i, char *dst, int *j)
{
	(*i)++;
	while (src[*i] && src[*i] != '\'')
	{
		dst[(*j)++] = src[(*i)++];
	}
	if (src[*i] == '\'')
		(*i)++;
}

void	process_double_quotes(char *src, int *i, char *dst, int *j)
{
	(*i)++;
	while (src[*i] && src[*i] != '"')
	{
		if (src[*i] == '\\' && src[*i + 1])
			process_escape(src, i, dst, j);
		else if (src[*i] == '$' && src[*i + 1])
			copy_env_value(src, i, dst, j);
		else
			dst[(*j)++] = src[(*i)++];
	}
	if (src[*i] == '"')
		(*i)++;
}

void	process_character(char *src, int *i, char *dst, int *j)
{
	if (src[*i] == '\'')
		process_single_quotes(src, i, dst, j);
	else if (src[*i] == '"')
		process_double_quotes(src, i, dst, j);
	else if (src[*i] == '$' && src[*i + 1])
		copy_env_value(src, i, dst, j);
	else
		dst[(*j)++] = src[(*i)++];
}

char	*process_token_quotes(char *src)
{
	char	*result;
	int		i;
	int		j;

	result = malloc(4096);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (src[i])
		process_character(src, &i, result, &j);
	result[j] = 0;
	return (result);
}
