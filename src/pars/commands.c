/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:24:01 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/02 11:55:37 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Verifica se un comando è built-in.
** 
** @param cmd_name: Il nome del comando da verificare
** @return: 1 se è built-in, 0 altrimenti
*/
static int is_builtin_command(const char *cmd_name)
{
    const char *builtins[] = {
        "echo", "cd", "pwd", "export", "unset", "env", "exit", NULL
    };
    int i;

    i = 0;
    while (builtins[i])
    {
        if (strcmp(cmd_name, builtins[i]) == 0)
            return (1);
        i++;
    }
    return (0);
}

/*
** Inizializza un nuovo comando base.
** 
** @return: Il comando inizializzato
*/
static t_command *create_command(void)
{
    t_command *cmd;

    cmd = calloc(1, sizeof(t_command));
    cmd->in_fd = -1;
    cmd->out_fd = -1;
    cmd->redir_error = 0;
    return (cmd);
}

/*
** Processa un token di redirezione.
** 
** @param cmd: Il comando corrente
** @param curr: Puntatore al token corrente
** @return: Il token successivo dopo la redirezione
*/
static t_token *process_redirection(t_command *cmd, t_token *curr)
{
    if (handle_redirection(cmd, curr) == -1)
    {
        cmd->redir_error = 1;
        return (NULL);
    }
    curr = curr->next; // Salta il token della redirezione
    if (curr)
        curr = curr->next; // Salta il nome del file
    return (curr);
}

/*
** Estrae gli argomenti dai token.
** 
** @param curr: Il token corrente
** @param argc: Puntatore al contatore degli argomenti
** @param argv: Array dove salvare gli argomenti
** @param cmd: Il comando corrente
** @return: Il token successivo dopo l'estrazione
*/
static t_token *extract_arguments(t_token *curr, int *argc, char **argv, t_command *cmd)
{
    *argc = 0;
    while (curr && curr->type != TOKEN_PIPE)
    {
        if (curr->type == TOKEN_WORD)
        {
            argv[(*argc)++] = strdup(curr->value);
            curr = curr->next;
        }
        else if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_APPEND)
        {
            curr = process_redirection(cmd, curr);
            if (!curr)
                break;
        }
        else
            curr = curr->next;
    }
    argv[*argc] = NULL;
    return (curr);
}

/*
** Finalizza il comando con gli argomenti estratti.
** 
** @param cmd: Il comando da finalizzare
** @param argc: Numero di argomenti
** @param argv: Array degli argomenti
** @return: Il comando finalizzato o NULL se vuoto
*/
static t_command *finalize_command(t_command *cmd, int argc, char **argv)
{
    cmd->argv = malloc((argc + 1) * sizeof(char*));
    for (int i = 0; i < argc; i++)
        cmd->argv[i] = argv[i];
    cmd->argv[argc] = NULL;
    
    if (argc == 0 && !cmd->redir_error)
    {
        free(cmd->argv);
        free(cmd);
        return (NULL);
    }
    
    if (cmd->argv[0])
        cmd->is_builtin = is_builtin_command(cmd->argv[0]);
    return (cmd);
}

/*
** Inizializza un nuovo comando con i suoi argomenti.
** 
** @param curr: Il token corrente da cui iniziare l'estrazione
** @param argc: Puntatore al contatore degli argomenti
** @param argv: Array dove salvare gli argomenti
** @return: Il comando inizializzato
*/
static t_command *init_command(t_token *curr, int *argc, char **argv)
{
    t_command *cmd;

    cmd = create_command();
    extract_arguments(curr, argc, argv, cmd);
    return (finalize_command(cmd, *argc, argv));
}

/*
** Trova il prossimo token dopo il comando corrente.
** 
** @param curr: Il token corrente
** @return: Il token successivo
*/
static t_token *find_next_command(t_token *curr)
{
    while (curr && curr->type != TOKEN_PIPE)
    {
        if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_APPEND)
        {
            curr = curr->next;
            if (curr)
                curr = curr->next;
        }
        else
            curr = curr->next;
    }
    if (curr && curr->type == TOKEN_PIPE)
        curr = curr->next;
    return (curr);
}

/*
** Aggiunge un comando alla lista.
** 
** @param head: Puntatore alla testa della lista
** @param tail: Puntatore alla coda della lista
** @param cmd: Il comando da aggiungere
*/
static void add_command_to_list(t_command **head, t_command **tail, t_command *cmd)
{
    if (!*head)
        *head = cmd;
    else
        (*tail)->next = cmd;
    *tail = cmd;
}

/*
** Costruisce una lista di comandi (pipeline) dai token.
** 
** @param tokens: La lista di token da cui costruire i comandi
** @return: La testa della lista dei comandi
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
        if (cmd)
            add_command_to_list(&head, &tail, cmd);
        curr = find_next_command(curr);
    }
    return (head);
}
