
#include "minishell.h"  // contiene t_shell_state g_state e prototipi

/*
  Esegue una lista di comandi collegati da pipe e redirezioni.
  commands è la testa di una lista collegata (linked list) di t_command.
*/
void execute_pipeline(t_command *commands)
{
    int      num_cmds = 0;
    t_command *cmd;
    int      **pipes = NULL;   // array di pipe [num_cmds-1][2]
    pid_t    *pids  = NULL;
    int       i;

    // 1) Contiamo quanti comandi ci sono nella lista
    cmd = commands;
    while (cmd) {
        num_cmds++;
        cmd = cmd->next;
    }
    if (num_cmds == 0)
        return; // niente da fare

    // 2) Se c’è più di un comando, creiamo le pipe
    if (num_cmds > 1) {
        pipes = malloc(sizeof(int *) * (num_cmds - 1));
        for (i = 0; i < num_cmds - 1; ++i) {
            pipes[i] = malloc(sizeof(int) * 2);
            if (pipe(pipes[i]) < 0) {
                perror("pipe");
                // In un caso reale bisognerebbe liberare tutto e uscire
                return;
            }
        }
    }

    // 3) Array di PID per aspettare i figli dopo
    pids = malloc(sizeof(pid_t) * num_cmds);

    // 4) Per ogni comando, facciamo fork + setup dei fd
    cmd = commands;
    i = 0;
    while (cmd) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            // Qui possiamo decidere di abortire tutto, ma per semplicità torniamo
            return;
        }

        if (pid == 0) {
            // ---> Processo Figlio <---

            // A) Se non è il primo comando, leggo dalla pipe precedente:
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            // B) Se non è l’ultimo comando, scrivo sulla pipe successiva:
            if (i < num_cmds - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // C) Redirezioni su file (se Persona 1 ha impostato in_fd/out_fd):
            if (cmd->in_fd >= 0) {
                dup2(cmd->in_fd, STDIN_FILENO);
            }
            if (cmd->out_fd >= 0) {
                dup2(cmd->out_fd, STDOUT_FILENO);
            }

            // D) Chiudo TUTTI i descrittori di pipe (nessun processo deve tenerli aperti):
            for (int j = 0; j < num_cmds - 1; ++j) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // E) Se è un built-in, lo eseguo direttamente (senza execve):
            if (cmd->is_builtin) {
                // ESEMPIO: se è `echo`, `cd`, ecc.
                execute_builtin(cmd);
                exit(g_state.last_status);
            }

            // F) Altrimenti, eseguo con execve il programma esterno:
            execve(cmd->path, cmd->argv, environ);
            // Se arrivo qui, execve ha fallito:
            perror("execve");
            exit(EXIT_FAILURE);
        }

        // ---> Processo Padre <---
        pids[i] = pid;

        // Subito dopo il fork chiudo le estremità di pipe:
        // Se c’è la pipe precedente, chiudo la sua parte di lettura (i-1][0])
        if (i > 0) {
            close(pipes[i - 1][0]);
            close(pipes[i - 1][1]);
        }

        cmd = cmd->next;
        i++;
    }

    // 5) Nel padre: dopo l’ultimo fork, chiudo eventuali pipe rimaste aperte
    if (num_cmds > 1) {
        // La pipe num_cmds-2 non è stata chiusa dentro il ciclo, quindi:
        close(pipes[num_cmds - 2][0]);
        close(pipes[num_cmds - 2][1]);
    }

    // 6) Aspetto tutti i figli e raccolgo exit status
    for (int k = 0; k < num_cmds; ++k) {
        int status;
        waitpid(pids[k], &status, 0);
        if (WIFEXITED(status)) {
            g_state.last_status = WEXITSTATUS(status);
        }
        else if (WIFSIGNALED(status)) {
            g_state.last_status = 128 + WTERMSIG(status);
        }
    }

    // 7) Libero memoria delle pipe e dell’array di pid
    if (num_cmds > 1) {
        for (int j = 0; j < num_cmds - 1; ++j)
            free(pipes[j]);
        free(pipes);
    }
    free(pids);
}
