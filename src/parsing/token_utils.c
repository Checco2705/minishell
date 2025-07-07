/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:24:01 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/05 14:20:49 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Crea un nuovo token e lo aggiunge in coda alla lista.
** 
** @param val: Il valore del token
** @param type: Il tipo del token
** @return: Il nuovo token creato, o NULL in caso di errore
**
** La funzione alloca memoria per un nuovo token, copia il valore
** e imposta il tipo. Il token viene inizializzato con next = NULL.
** In caso di errore di allocazione, restituisce NULL.
*/
t_token	*create_token(const char *val, t_token_type type)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->value = ft_strdup(val);
	tok->type = type;
	tok->has_quotes = 0;
	tok->next = NULL;
	return (tok);
}

static t_token	*handle_double_operators(const char *input, int *i)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		*i += 2;
		return (create_token("<<", TOKEN_HEREDOC));
	}
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		*i += 2;
		return (create_token(">>", TOKEN_APPEND));
	}
	return (NULL);
}

static t_token	*handle_single_operators(const char *input, int *i)
{
	if (input[*i] == '<')
	{
		*i += 1;
		return (create_token("<", TOKEN_REDIR_IN));
	}
	if (input[*i] == '>')
	{
		*i += 1;
		return (create_token(">", TOKEN_REDIR_OUT));
	}
	if (input[*i] == '|')
	{
		*i += 1;
		return (create_token("|", TOKEN_PIPE));
	}
	return (NULL);
}

/*
** Gestisce gli operatori di redirezione (<, >, <<, >>).
** 
** @param line: La stringa da processare
** @param i: Indice corrente nella stringa
** @param tok: Puntatore al token da creare
** @return: La lunghezza dell'operatore riconosciuto (1 o 2)
**
** La funzione riconosce gli operatori di redirezione e crea
** il token appropriato. Restituisce la lunghezza dell'operatore
** per permettere l'avanzamento corretto nell'input.
*/
t_token	*handle_redirection_operator(const char *input, int *i)
{
	t_token	*tok;

	tok = handle_double_operators(input, i);
	if (tok)
		return (tok);
	return (handle_single_operators(input, i));
}

/*
** Riconosce e gestisce operatori speciali (|, <, >, <<, >>).
** 
** @param line: La stringa da processare
** @param i: Indice corrente nella stringa
** @param tok: Puntatore al token da creare
** @return: La lunghezza dell'operatore riconosciuto (1 o 2)
**
** La funzione riconosce tutti gli operatori speciali della shell:
** - | (pipe)
** - < (input redirection)
** - > (output redirection)
** - << (heredoc)
** - >> (append)
** Restituisce la lunghezza dell'operatore per l'avanzamento.
*/
int	is_operator(const char *input, int i)
{
	if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		return (1);
	return (0);
}
