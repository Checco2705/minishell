/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:24:01 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/06/11 19:37:57 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Gestisce una stringa tra virgolette singole (').
** 
** @param str: La stringa da processare
** @param i: Puntatore all'indice corrente nella stringa
** @param result: Buffer dove salvare il risultato
** @param j: Puntatore all'indice corrente nel buffer risultato
** @return: 1 se la gestione è avvenuta con successo
**
** La funzione copia il contenuto tra virgolette singole nel buffer risultato
** senza espandere variabili o caratteri speciali. Le virgolette singole
** vengono rimosse dal risultato finale.
*/
static int handle_single_quotes(const char *str, int *i, char *result, int *j)
{
    (*i)++;
    while (str[*i] && str[*i] != '\'')
        result[(*j)++] = str[(*i)++];
    if (str[*i] == '\'')
        (*i)++;
    return (1);
}

/*
** Gestisce una stringa tra virgolette doppie (").
** 
** @param str: La stringa da processare
** @param i: Puntatore all'indice corrente nella stringa
** @param result: Buffer dove salvare il risultato
** @param j: Puntatore all'indice corrente nel buffer risultato
** @return: 1 se la gestione è avvenuta con successo
**
** La funzione copia il contenuto tra virgolette doppie nel buffer risultato,
** permettendo l'espansione delle variabili d'ambiente ($VAR). Le virgolette
** doppie vengono rimosse dal risultato finale.
*/
static int handle_double_quotes(const char *str, int *i, char *result, int *j)
{
    (*i)++;
    while (str[*i] && str[*i] != '"')
    {
        if (str[*i] == '$' && str[*i + 1])
        {
            if (copy_env_value(str, i, result, j))
                continue;
        }
        result[(*j)++] = str[(*i)++];
    }
    if (str[*i] == '"')
        (*i)++;
    return (1);
}

/*
** Gestisce le virgolette in un token.
** 
** @param token: Il token da processare
**
** La funzione processa il valore del token, gestendo sia virgolette singole
** che doppie. Crea un nuovo buffer per il risultato, processa il token
** carattere per carattere e sostituisce il valore originale con il risultato
** processato. Le virgolette vengono rimosse e le variabili d'ambiente vengono
** espanse solo all'interno delle virgolette doppie.
*/
void handle_quotes(t_token *token)
{
    char *result;
    int i;
    int j;

    if (!token || !token->value || token->type != TOKEN_WORD)
        return;
    result = malloc(4096);
    i = 0;
    j = 0;
    while (token->value[i])
    {
        if (token->value[i] == '\'')
            handle_single_quotes(token->value, &i, result, &j);
        else if (token->value[i] == '"')
            handle_double_quotes(token->value, &i, result, &j);
        else
            result[j++] = token->value[i++];
    }
    result[j] = 0;
    free(token->value);
    token->value = strdup(result);
    free(result);
}
