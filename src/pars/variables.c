/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 19:47:36 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/04 14:42:19 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

int	copy_env_value(const char *src, int *si, char *dst, int *di)
{
	int	var_start;
	int	var_len;
	int	chars_written;

	if (src[*si] == '$' && src[*si + 1] == '?')
	{
		*si += 2;
		return (handle_exit_status(dst, di));
	}
	if (src[*si] == '$' && (ft_isalpha(src[*si + 1]) || src[*si + 1] == '_'))
	{
		(*si)++;
		var_start = *si;
		var_len = get_var_length(src, var_start);
		*si += var_len;
		chars_written = handle_env_var(src, var_start, var_len, dst + *di);
		*di += chars_written;
		return (1);
	}
	dst[(*di)++] = src[(*si)++];
	return (0);
}

char	*expand_string(const char *str)
{
	char	*result;
	int		si;
	int		di;
	char	in_quotes;

	if (!str)
		return (NULL);
	result = malloc(4096);
	if (!result)
		return (NULL);
	si = 0;
	di = 0;
	in_quotes = 0;
	while (str[si])
	{
		if ((str[si] == '\'' || str[si] == '"') && !in_quotes)
			in_quotes = str[si];
		else if (str[si] == in_quotes)
			in_quotes = 0;
		if (in_quotes == '\''
			|| !copy_env_value(str, &si, result, &di))
			result[di++] = str[si++];
	}
	result[di] = '\0';
	return (result);
}

static void	remove_empty_token(t_token **tokens, t_token *prev,
	t_token **current)
{
	t_token	*to_remove;

	to_remove = *current;
	if (prev)
		prev->next = (*current)->next;
	else
		*tokens = (*current)->next;
	*current = (*current)->next;
	free(to_remove->value);
	free(to_remove);
}

void	expand_variables(t_token **tokens)
{
	t_token	*current;
	t_token	*prev;
	char	*old_value;

	current = *tokens;
	prev = NULL;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			old_value = current->value;
			current->value = expand_string(current->value);
			if (old_value)
				free(old_value);
			if (current->value && ft_strlen(current->value) == 0)
			{
				remove_empty_token(tokens, prev, &current);
				continue ;
			}
		}
		prev = current;
		current = current->next;
	}
}
