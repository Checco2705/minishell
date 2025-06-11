/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:24:01 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/06/11 19:40:02 by ffebbrar         ###   ########.fr       */
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
t_token *create_token(const char *val, t_token_type type)
{
    t_token *tok;

    tok = malloc(sizeof(t_token));
    if (!tok)
        return (NULL);
    tok->value = strdup(val);
    tok->type = type;
    tok->next = NULL;
    return (tok);
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
static int handle_redirection_operator(const char *line, int i, t_token **tok)
{
    if (line[i] == '<' && line[i + 1] == '<')
    {
        *tok = create_token("<<", TOKEN_HEREDOC);
        return (2);
    }
    if (line[i] == '>' && line[i + 1] == '>')
    {
        *tok = create_token(">>", TOKEN_APPEND);
        return (2);
    }
    if (line[i] == '<')
    {
        *tok = create_token("<", TOKEN_REDIR_IN);
        return (1);
    }
    if (line[i] == '>')
    {
        *tok = create_token(">", TOKEN_REDIR_OUT);
        return (1);
    }
    return (0);
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
int is_operator(const char *line, int i, t_token **tok)
{
    if (line[i] == '|')
    {
        *tok = create_token("|", TOKEN_PIPE);
        return (1);
    }
    return (handle_redirection_operator(line, i, tok));
}

/*
** Estrae una parola tra virgolette.
** 
** @param line: La stringa da processare
** @param i: Indice corrente nella stringa
** @param out: Puntatore alla stringa estratta
** @return: La lunghezza della parola estratta
**
** La funzione estrae il contenuto tra virgolette singole o doppie,
** copiandolo nella stringa di output. Le virgolette vengono rimosse
** dal risultato. La memoria per la stringa estratta viene allocata
** dinamicamente.
*/
static int extract_quoted_word(const char *line, int i, char **out)
{
    char quote;
    int start;
    int len;

    quote = line[i++];
    start = i;
    while (line[i] && line[i] != quote)
        i++;
    len = i - start;
    *out = strndup(line + start, len);
    if (line[i] == quote)
        i++;
    return (i - start + 1);
}

/*
** Estrae una parola o una stringa tra virgolette.
** 
** @param line: La stringa da processare
** @param i: Indice corrente nella stringa
** @param out: Puntatore alla stringa estratta
** @return: La lunghezza della parola estratta
**
** La funzione estrae una parola dalla stringa, gestendo:
** - Parole tra virgolette singole o doppie
** - Parole normali (fino al prossimo spazio o operatore)
** La memoria per la stringa estratta viene allocata dinamicamente.
*/
int extract_word(const char *line, int i, char **out)
{
    int start;
    int len;

    start = i;
    len = 0;
    if (line[i] == '\'' || line[i] == '"')
        return (extract_quoted_word(line, i, out));
    while (line[i] && line[i] != ' ' && line[i] != '\t' &&
           line[i] != '|' && line[i] != '<' && line[i] != '>' &&
           line[i] != '\'' && line[i] != '"')
        i++;
    len = i - start;
    *out = strndup(line + start, len);
    return (len);
}
