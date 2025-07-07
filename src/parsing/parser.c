/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 11:58:21 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/02 21:10:15 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Funzione principale di parsing.
** 
** @param line: La stringa da parsare
** @return: La lista di comandi generata, o NULL in caso di errore
**
** La funzione esegue il parsing completo dell'input:
** 1. Tokenizza la stringa
** 2. Gestisce le virgolette
** 3. Espande le variabili
** 4. Controlla la sintassi
** 5. Costruisce i comandi
*/
t_command	*parse_input(const char *line)
{
	t_token		*tokens;
	t_command	*commands;

	if (!line || !*line)
		return (NULL);
	tokens = tokenize(line);
	if (!tokens)
		return (NULL);
	expand_variables(&tokens);
	handle_quotes(tokens);
	if (check_syntax_errors(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	commands = build_commands(tokens);
	free_tokens(tokens);
	return (commands);
}
