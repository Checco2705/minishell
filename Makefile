# Makefile per Minishell (Scuola 42)

# Directory sorgenti e oggetti\ nSRCDIR := src

OBJDIR := obj

# Nome eseguibile

NAME := minishell

# Compilatore e flag

CC := gcc
CFLAGS := -Wall -Wextra -Werror -g -I\$(SRCDIR)

# Raccolta dei sorgenti e degli oggetti

SRCS := \$(wildcard \$(SRCDIR)/\*.c)
OBJS := \$(patsubst \$(SRCDIR)/%.c,\$(OBJDIR)/%.o,\$(SRCS))

# Regola di default

all: \$(NAME)

# Collegamento eseguibile

\$(NAME): \$(OBJS)
\$(CC) \$(OBJS) -o \$(NAME)

# Compilazione dei .c in .o nella cartella obj

\$(OBJDIR)/%.o: \$(SRCDIR)/%.c | \$(OBJDIR)
\$(CC) \$(CFLAGS) -c \$< -o \$@

# Creazione della cartella obj se non esiste

\$(OBJDIR):
@mkdir -p \$(OBJDIR)

# Pulisce i file oggetto

clean:
@rm -rf \$(OBJDIR)

# Pulisce oggetti ed eseguibile

fclean: clean
@rm -f \$(NAME)

# Ricompila da zero

re: fclean all

# Obbliga regole senza file

.PHONY: all clean fclean re
