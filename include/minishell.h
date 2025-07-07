/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffebbrar <ffebbrar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 19:47:36 by ffebbrar          #+#    #+#             */
/*   Updated: 2025/07/05 14:30:13 by ffebbrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/wait.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include "libft.h"
# include "../printf/ft_printf.h"

typedef struct s_shell_state
{
	volatile sig_atomic_t	signal;
	int						last_status;
	char					**local_env;
	volatile sig_atomic_t	executing;
}	t_shell_state;

typedef struct s_builtin
{
	const char	*name;
	int			(*func)(char **);
}	t_builtin;

typedef struct s_command
{
	char				*path;
	char				**argv;
	int					in_fd;
	int					out_fd;
	int					is_builtin;
	int					redir_error;
	struct s_command	*next;
}	t_command;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				has_quotes;
	struct s_token	*next;
}	t_token;

extern t_shell_state	g_state;

int			wait_for_single_child(pid_t pid);
/* --- Funzioni user (Parte esecuzione & segnali) --- */
void		setup_signals(void);
int			execute_pipeline(t_command *commands);
int			count_commands(t_command *commands);
void		cleanup_after_execution(t_command *commands);

/* --- Funzioni Persona 1 (Parte parsing) --- */
t_command	*parse_input(const char *line);
void		free_commands(t_command *commands);

/* --- Funzioni di tokenizzazione --- */
t_token		*tokenize(const char *input);
void		free_tokens(t_token *tokens);

/* --- Funzioni di gestione token --- */
t_token		*create_token(const char *value, t_token_type type);
t_token		*handle_redirection_operator(const char *input, int *i);
int			is_operator(const char *input, int i);
char		*extract_quoted_word(const char *input, int *i, char quote);
char		*extract_word(const char *input, int *i);

/* --- Funzioni di gestione quote --- */
void		handle_quotes(t_token *token);
int			process_escape(const char *str, int *i, char *result, int *j);
void		process_single_quotes(char *src, int *i, char *dst, int *j);
void		process_double_quotes(char *src, int *i, char *dst, int *j);
void		process_character(char *src, int *i, char *dst, int *j);
char		*process_token_quotes(char *src);

/* --- Funzioni di gestione variabili --- */
int			copy_env_value(const char *src, int *si, char *dst, int *di);
void		expand_variables(t_token **tokens);
char		*expand_string(const char *str);

/* --- Funzioni utility per variabili --- */
int			handle_exit_status(char *dst, int *di);
void		copy_env_value_to_dst(char *val, char *dst, int *di);
int			get_var_length(const char *src, int start);
int			handle_env_var(const char *src, int var_start, int var_len,
				char *dst);

/* --- Funzioni di controllo sintassi --- */
int			check_syntax_errors(t_token *tokens);

/* --- Funzioni di gestione redirezioni --- */
int			handle_redirection(t_command *cmd, t_token *curr);

/* --- Funzioni di gestione heredoc --- */
char		*create_temp_filename(void);
int			handle_multiple_heredocs(t_command *cmd, t_token *start_token);
int			handle_single_heredoc(t_command *cmd, t_token *curr);
int			count_heredocs(t_token *start);
int			write_expanded_line(char *line, int fd);
int			expand_heredoc_line(char *line, int fd, int has_quotes);
int			setup_heredoc_file(char *filename, char *delimiter);
int			read_heredoc_content(char *delimiter, int fd);

/* --- Funzioni di costruzione comandi --- */
t_command	*build_commands(t_token *tokens);
int			is_builtin_command(const char *cmd_name);
t_command	*create_command(void);
t_command	*finalize_command(t_command *cmd, int argc, char **argv);
void		add_command_to_list(t_command **head, t_command **tail,
				t_command *cmd);
t_token		*find_next_command(t_token *curr);

/* --- Funzioni di risoluzione percorsi --- */
char		*find_executable(const char *cmd);
char		*get_next_path_token(char **path_ptr);
int			is_executable(const char *path);
char		*build_path_string(const char *dir, const char *cmd);
char		*build_and_check_path(const char *dir, const char *cmd);
char		*handle_absolute_path(const char *cmd);
char		*search_in_path(const char *cmd, char *path);

/* --- Funzioni built-in --- */
int			ft_echo(char **args);
int			ft_cd(char **args);
int			ft_pwd(char **args);
int			ft_export(char **args);
int			ft_unset(char **args);
int			ft_env(char **args);
int			ft_exit(char **args);

// Environment management
int			ft_setenv(const char *name, const char *value, int overwrite);
int			ft_unsetenv(const char *name);
char		*ft_getenv(const char *name);
char		**ft_get_environ(void);
int			is_valid_identifier(const char *name);

/* --- Environment helper functions --- */
char		**copy_environ_vars(void);
char		**get_static_env(void);
void		set_static_env(char **new_env);
char		**get_our_environ(void);
void		set_our_environ(char **new_env);
char		*create_env_string(const char *name, const char *value);
int			find_env_index(const char *name);
int			count_env_vars(void);
int			remove_single_var(void);

t_builtin	*get_builtins(void);

/* --- Funzioni export --- */
void		print_exported_vars(void);
int			compare_env_vars(const void *a, const void *b);
void		print_var_without_value(char *env_var, char *eq_pos);
void		print_var_with_empty_value(char *env_var, char *eq_pos);
void		print_var_with_value(char *env_var, char *eq_pos);
void		print_single_var(char *env_var);

/* --- Funzioni di pipeline --- */
int			init_pipeline(t_command *commands, int ***pipes, pid_t **pids);
void		cleanup_resources(int **pipes, pid_t *pids, int num_cmds);
void		setup_child_pipes(t_command *cmd, int **pipes, int num_cmds, int i);
void		execute_child(t_command *cmd);
void		wait_for_children(pid_t *pids, int num_cmds);
int			**create_pipes(int num_cmds);
void		execute_all_commands(t_command *commands, int **pipes,
				pid_t *pids, int num_cmds);
void		close_all_pipes(int **pipes, int num_cmds);

void		execute_builtin(t_command *cmd);

#endif