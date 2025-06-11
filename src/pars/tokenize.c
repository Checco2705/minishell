/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:31:43 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/06/11 19:39:01 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Aggiunge un token alla lista.
** 
** @param head: Puntatore alla testa della lista
** @param tail: Puntatore alla coda della lista
** @param tok: Il token da aggiungere
**
** La funzione aggiunge un token alla lista mantenendo sia il puntatore
** alla testa che alla coda. Se la lista è vuota, il token diventa sia
** testa che coda, altrimenti viene aggiunto in coda.
*/
static void add_token_to_list(t_token **head, t_token **tail, t_token *tok)
{
    if (!*head)
        *head = tok;
    else
        (*tail)->next = tok;
    *tail = tok;
}

/*
** Gestisce l'estrazione e l'aggiunta di una parola.
** 
** @param line: La stringa da processare
** @param i: Indice corrente nella stringa
** @param head: Puntatore alla testa della lista di token
** @param tail: Puntatore alla coda della lista di token
** @return: La lunghezza della parola estratta
**
** La funzione estrae una parola dalla stringa, crea un nuovo token
** e lo aggiunge alla lista. Gestisce anche la memoria allocata
** per la parola estratta.
*/
static int handle_word(const char *line, int i, t_token **head, t_token **tail)
{
    char *word;
    t_token *tok;
    int wlen;

    word = NULL;
    wlen = extract_word(line, i, &word);
    if (wlen > 0 && word && *word)
    {
        tok = create_token(word, TOKEN_WORD);
        add_token_to_list(head, tail, tok);
    }
    free(word);
    return (wlen);
}

/*
** Funzione principale di tokenizzazione.
** 
** @param line: La stringa da tokenizzare
** @return: La lista di token generata
**
** La funzione converte una stringa in una lista di token, riconoscendo:
** - Operatori (|, <, >, <<, >>)
** - Parole (stringhe di caratteri)
** - Spazi e tabulazioni (che vengono ignorati)
** La memoria per i token viene allocata dinamicamente e deve essere
** liberata dal chiamante usando free_tokens.
*/
t_token *tokenize(const char *line)
{
    int i;
    t_token *head;
    t_token *tail;
    t_token *tok;

    i = 0;
    head = NULL;
    tail = NULL;
    while (line[i])
    {
        while (line[i] == ' ' || line[i] == '\t')
            i++;
        if (!line[i])
            break;
        tok = NULL;
        if (is_operator(line, i, &tok))
        {
            add_token_to_list(&head, &tail, tok);
            i += is_operator(line, i, &tok);
            continue;
        }
        i += handle_word(line, i, &head, &tail);
    }
    return (head);
}
