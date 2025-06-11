#ifndef MINISHELL_H
#define MINISHELL_H

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

/* --- Struct globale unico per stato shell --- */
typedef struct s_shell_state
{
    volatile sig_atomic_t signal;
    int last_status;
}   t_shell_state;

typedef struct s_command {
    char   *path;       // percorso dell'eseguibile (già risolto)
    char  **argv;       // array di argomenti, terminato da NULL
    int     in_fd;      // file descriptor da usare come STDIN (se non c'è, è -1)
    int     out_fd;     // file descriptor da usare come STDOUT (se non c'è, è -1)
    int     is_builtin; // 1 se è built-in, 0 altrimenti
    struct s_command *next; // puntatore al comando successivo (per pipeline)
} t_command;

typedef enum e_token_type {
    TOKEN_WORD,      // parola normale (comando o argomento)
    TOKEN_PIPE,      // |
    TOKEN_REDIR_IN,  // <
    TOKEN_REDIR_OUT, // >
    TOKEN_HEREDOC,   // <<
    TOKEN_APPEND,    // >>
    TOKEN_EOF        // fine input/token list
}   t_token_type;

typedef struct s_token {
    char           *value;      // testo del token
    t_token_type    type;       // tipo di token
    struct s_token *next;       // prossimo token nella lista
}   t_token;

static t_shell_state g_state = {0, 0};

/* --- Funzioni user (Parte esecuzione & segnali) --- */
void setup_signals(void);
void execute_pipeline(t_command *commands);
void cleanup_after_execution(t_command *commands);

/* --- Funzioni Persona 1 (Parte parsing) --- */
extern t_command *parse_input(const char *line);
extern void free_commands(t_command *commands);

/* --- Funzioni di tokenizzazione --- */
t_token *tokenize(const char *input);
void free_tokens(t_token *tokens);

/* --- Funzioni di gestione token --- */
t_token *create_token(const char *value, t_token_type type);
t_token *handle_redirection_operator(const char *input, int *i);
int is_operator(const char *input, int i);
char *extract_quoted_word(const char *input, int *i, char quote);
char *extract_word(const char *input, int *i);

/* --- Funzioni di gestione quote --- */
void handle_quotes(t_token *tokens);

/* --- Funzioni di gestione variabili --- */
void expand_variables(t_token *tokens);
char *expand_string(const char *str);
void copy_env_value(const char *var_name, char *dest, int *j);

/* --- Funzioni di controllo sintassi --- */
int check_syntax_errors(t_token *tokens);

/* --- Funzioni di gestione redirezioni --- */
void handle_redirection(t_command *cmd, t_token *curr);

/* --- Funzioni di costruzione comandi --- */
t_command *build_commands(t_token *tokens);

#endif