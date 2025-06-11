/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:24:01 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/06/11 19:54:51 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Inizializza un nuovo comando con i suoi argomenti.
** 
** @param curr: Il token corrente da cui iniziare l'estrazione
** @param argc: Puntatore al contatore degli argomenti
** @param argv: Array dove salvare gli argomenti
** @return: Il comando inizializzato
**
** La funzione:
** - Alloca memoria per il comando
** - Inizializza i file descriptor a -1
** - Estrae gli argomenti dai token fino al prossimo pipe
** - Gestisce le redirezioni
** - Costruisce l'array degli argomenti
*/
static t_command *init_command(t_token *curr, int *argc, char **argv)
{
    t_command *cmd;

    cmd = calloc(1, sizeof(t_command));
    cmd->in_fd = -1;
    cmd->out_fd = -1;
    *argc = 0;
    while (curr && curr->type != TOKEN_PIPE)
    {
        if (curr->type == TOKEN_WORD)
            argv[(*argc)++] = strdup(curr->value);
        else if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_APPEND)
            handle_redirection(cmd, curr);
        curr = curr->next;
    }
    argv[*argc] = NULL;
    cmd->argv = malloc((*argc + 1) * sizeof(char*));
    for (int i = 0; i <= *argc; i++)
        cmd->argv[i] = argv[i];
    return (cmd);
}

/*
** Costruisce una lista di comandi (pipeline) dai token.
** 
** @param tokens: La lista di token da cui costruire i comandi
** @return: La testa della lista dei comandi
**
** La funzione:
** - Crea una lista di comandi separati da pipe
** - Per ogni comando:
**   - Inizializza il comando con i suoi argomenti
**   - Gestisce le redirezioni
**   - Collega i comandi in pipeline
*/
t_command *build_commands(t_token *tokens)
{
    t_command *head;
    t_command *tail;
    t_token *curr;
    t_command *cmd;
    int argc;
    char *argv[256];

    head = NULL;
    tail = NULL;
    curr = tokens;
    while (curr)
    {
        cmd = init_command(curr, &argc, argv);
        if (!head)
            head = cmd;
        else
            tail->next = cmd;
        tail = cmd;
        while (curr && curr->type != TOKEN_PIPE)
            curr = curr->next;
        if (curr)
            curr = curr->next;
    }
    return (head);
}
