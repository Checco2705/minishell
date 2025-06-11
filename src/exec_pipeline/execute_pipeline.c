#include "minishell.h"  // contiene t_shell_state g_state e prototipi

/*
  Esegue una lista di comandi collegati da pipe e redirezioni.
  commands è la testa di una lista collegata (linked list) di t_command.
*/
void execute_pipeline(t_command *commands)
{
    int **pipes = NULL;        // Array di pipe, ogni pipe ha 2 fd [read, write]
    pid_t *pids = NULL;        // Array per tenere traccia dei PID dei processi figli
    int num_cmds = init_pipeline(commands, &pipes, &pids);  // Inizializza pipe e pids, ritorna numero comandi
    
    if (!num_cmds)             // Se non ci sono comandi, esci
        return;

    t_command *cmd = commands; // Puntatore al primo comando
    for (int i = 0; cmd; i++) {  // Per ogni comando nella lista
        pid_t pid = fork();     // Crea un nuovo processo
        if (pid < 0) {          // Se fork fallisce
            perror("fork");     // Stampa errore
            cleanup_resources(pipes, pids, num_cmds);  // Libera risorse
            return;             // Esci
        }

        if (pid == 0) {         // Se siamo nel processo figlio
            setup_child_fds(cmd, pipes, num_cmds, i);  // Configura file descriptor
            execute_child(cmd);  // Esegui il comando
        }

        pids[i] = pid;          // Salva il PID del figlio
        close_pipe_ends(pipes, num_cmds, i);  // Chiudi le pipe non più necessarie
        cmd = cmd->next;        // Passa al prossimo comando
    }

    close_last_pipe(pipes, num_cmds);  // Chiudi l'ultima pipe rimasta aperta
    wait_for_children(pids, num_cmds); // Aspetta che tutti i figli terminino
    cleanup_resources(pipes, pids, num_cmds);  // Libera tutta la memoria
}


/*
  Pulisce tutte le risorse dopo l'esecuzione dei comandi.
  commands: lista dei comandi da pulire
  La funzione si assicura che tutti i file descriptor siano chiusi
  e che tutta la memoria allocata sia liberata.
*/
void cleanup_after_execution(t_command *commands)
{
    t_command *current;
    t_command *next;

    current = commands;
    while (current) {
        // Salva il prossimo comando prima di liberare il corrente
        next = current->next;

        // Chiudi i file descriptor se sono aperti
        if (current->in_fd >= 0) {
            close(current->in_fd);
            current->in_fd = -1;
        }
        if (current->out_fd >= 0) {
            close(current->out_fd);
            current->out_fd = -1;
        }

        // Libera l'array degli argomenti
        if (current->argv) {
            for (int i = 0; current->argv[i]; i++)
                free(current->argv[i]);
            free(current->argv);
        }

        // Libera il percorso del comando
        if (current->path)
            free(current->path);

        // Libera il comando corrente
        free(current);

        // Passa al prossimo comando
        current = next;
    }
}
