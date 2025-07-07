/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 19:47:36 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/07 15:00:00 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

int	should_skip_quote(char c, int single_q, int double_q)
{
	if ((c == '\'' && !double_q) || (c == '"' && !single_q))
		return (1);
	return (0);
}

void	update_quote_state(char c, int *single_q, int *double_q)
{
	if (c == '\'' && !(*double_q))
	{
		if (*single_q == 0)
			*single_q = 1;
		else
			*single_q = 0;
	}
	else if (c == '"' && !(*single_q))
	{
		if (*double_q == 0)
			*double_q = 1;
		else
			*double_q = 0;
	}
}

char	*expand_string_respecting_single_quotes(const char *str)
{
	char	*result;
	int		si;
	int		di;
	int		single_q;
	int		double_q;

	if (!str)
		return (NULL);
	result = malloc(4096);
	if (!result)
		return (NULL);
	si = 0;
	di = 0;
	single_q = 0;
	double_q = 0;
	while (str[si])
	{
		update_quote_state(str[si], &single_q, &double_q);
		if (should_skip_quote(str[si], single_q, double_q))
			si++;
		else if (should_expand(str, si, single_q, double_q))
			copy_env_value(str, &si, result, &di);
		else
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
			current->value = expand_string_respecting_single_quotes(current->value);
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
