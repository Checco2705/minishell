#ifdef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

/* --- Struct globale unico per stato shell --- */
typedef struct s_shell_state
{
    volatile sig_atomic_t signal;
    int last_status;
}   t_shell_state;

static t_shell_state g_state = {0, 0};

/* --- Funzioni user (Parte esecuzione & segnali) --- */
void setup_signals(void);
void execute_pipeline(t_command *commands);
void cleanup_after_execution(t_command *commands);

/* --- Funzioni Persona 1 (Parte parsing) --- */
extern t_command *parse_input(const char *line);
extern void free_commands(t_command *commands);

#endif