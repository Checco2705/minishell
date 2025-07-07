# Architettura del Progetto Minishell

## Panoramica

Il progetto è stato riorganizzato in una struttura modulare per migliorare la manutenibilità e la comprensibilità del codice. Ogni modulo ha una responsabilità specifica e ben definita.

## Struttura delle Directory

```
src/
├── core/               # Funzioni principali e inizializzazione
├── parsing/            # Tokenizzazione e parsing dei comandi
├── heredoc/            # Gestione specifica degli heredoc
├── redirection/        # Gestione delle redirezioni (esclusi heredoc)
├── execution/          # Esecuzione delle pipeline e comandi
├── builtins/           # Built-in commands della shell
├── environment/        # Gestione variabili d'ambiente
├── path/               # Risoluzione dei percorsi
├── variables/          # Espansione delle variabili
├── quotes/             # Gestione delle quote
└── utils/              # Utilities generiche
```

## Moduli Dettagliati

### 📦 core/
**Responsabilità:** Gestione principale del programma
- `main.c` - Punto di ingresso principale
- `signals.c` - Gestione dei segnali (SIGINT, SIGQUIT)
- `cleanup.c` - Pulizia delle risorse

### 📦 parsing/
**Responsabilità:** Analisi e tokenizzazione dell'input
- `tokenize.c` - Tokenizzazione dell'input utente
- `token_utils.c` - Utilities per la gestione dei token
- `token_extract.c` - Estrazione di parole e operatori
- `parser.c` - Parser principale
- `command_parser.c` - Costruzione dei comandi dai token
- `syntax.c` - Controllo della sintassi

### 📦 heredoc/
**Responsabilità:** Gestione completa degli heredoc
- `heredoc.c` - Implementazione completa degli heredoc
  - Gestione heredoc singoli e multipli
  - Supporto per delimitatori quotati
  - Espansione variabili condizionale
  - Gestione file temporanei

### 📦 redirection/
**Responsabilità:** Gestione delle redirezioni standard
- `redirection.c` - Redirezioni input/output (`<`, `>`, `>>`)
- Gestione errori di file
- Apertura e chiusura file descriptor

### 📦 execution/
**Responsabilità:** Esecuzione dei comandi
- `execute_pipeline.c` - Esecuzione delle pipeline
- `execute_child.c` - Esecuzione processi figli
- `execute_utils.c` - Utilities per l'esecuzione

### 📦 builtins/
**Responsabilità:** Implementazione dei built-in commands
- `echo.c` - Comando echo
- `cd.c` - Comando cd
- `pwd.c` - Comando pwd
- `export.c` - Comando export
- `export_utils.c` - Utilities per export
- `export_print.c` - Stampa variabili esportate
- `unset.c` - Comando unset
- `env.c` - Comando env
- `exit.c` - Comando exit
- `execute_builtin.c` - Dispatcher dei built-in

### 📦 environment/
**Responsabilità:** Gestione delle variabili d'ambiente
- `env_manager.c` - Gestione principale dell'ambiente
- `env_getset.c` - Get/set variabili d'ambiente
- `env_utils.c` - Utilities per l'ambiente
- `env_string.c` - Manipolazione stringhe d'ambiente

### 📦 path/
**Responsabilità:** Risoluzione dei percorsi
- `path_search.c` - Ricerca eseguibili nel PATH
- `path_builder.c` - Costruzione percorsi
- `path_resolution.c` - Risoluzione percorsi
- `path_utils.c` - Utilities per i percorsi

### 📦 variables/
**Responsabilità:** Espansione delle variabili
- `variables.c` - Espansione delle variabili (`$VAR`)
- `var_utils.c` - Utilities per le variabili

### 📦 quotes/
**Responsabilità:** Gestione delle quote
- `quotes.c` - Gestione quote singole e doppie
- `quote_utils.c` - Utilities per le quote
- `escape.c` - Gestione caratteri di escape

### 📦 utils/
**Responsabilità:** Utilities generiche
- `memory.c` - Gestione della memoria
- `command_utils.c` - Utilities per i comandi

## Vantaggi della Nuova Architettura

### 🎯 Modularità
- Ogni modulo ha una responsabilità specifica
- Facile identificazione della posizione del codice
- Riduzione delle dipendenze tra moduli

### 🔧 Manutenibilità
- Modifiche localizzate in moduli specifici
- Debugging più semplice
- Test unitari per modulo

### 📚 Comprensibilità
- Struttura logica e intuitiva
- Documentazione chiara delle responsabilità
- Facile onboarding per nuovi sviluppatori

### 🚀 Scalabilità
- Facile aggiunta di nuove funzionalità
- Possibilità di refactoring graduali
- Supporto per team di sviluppo più grandi

## Flusso di Esecuzione

1. **Input** (`core/main.c`) - Riceve l'input dell'utente
2. **Parsing** (`parsing/`) - Tokenizza e analizza l'input
3. **Espansione** (`variables/`, `quotes/`) - Espande variabili e gestisce quote
4. **Costruzione** (`parsing/command_parser.c`) - Costruisce i comandi
5. **Redirezioni** (`redirection/`, `heredoc/`) - Gestisce redirezioni e heredoc
6. **Esecuzione** (`execution/`) - Esegue i comandi o built-in
7. **Cleanup** (`core/cleanup.c`) - Pulisce le risorse

## Build System

Il Makefile utilizza `find` per scoprire automaticamente tutti i file `.c` nella struttura, quindi non richiede modifiche quando si aggiungono nuovi file.

```bash
# Compilazione
make

# Pulizia
make clean

# Ricompilazione completa
make re
```

## Note per gli Sviluppatori

- **Aggiunta di nuove funzionalità:** Identificare il modulo appropriato o crearne uno nuovo
- **Debugging:** Utilizzare la struttura modulare per isolare i problemi
- **Testing:** Testare ogni modulo indipendentemente quando possibile
- **Documentazione:** Mantenere aggiornata la documentazione dei moduli 