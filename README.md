# Minishell

## Descrizione del progetto
Minishell è un progetto della scuola 42 che consiste nel creare una shell semplice, cioè un programma che permette di interagire con il computer tramite comandi testuali, proprio come Bash. L'obiettivo è imparare a gestire processi, file descriptor e tutte le problematiche tipiche di una shell.

## Obiettivi principali
- Scrivere una shell in linguaggio C.
- Rispettare le regole di stile della scuola (Norm).
- Ogni file può avere massimo 5 funzioni, ogni funzione massimo 25 righe.
- Nessun crash o comportamento indefinito (es: segmentation fault, bus error, double free, ecc.).
- Nessun memory leak: la memoria allocata deve essere liberata (tranne per la funzione readline, che può avere memory leak non correggibili).
- Usare al massimo una variabile globale (solo per il numero del segnale ricevuto).
- Consegnare un Makefile con le regole: all, clean, fclean, re.

## Funzionalità richieste
La shell deve:
- Mostrare un prompt in attesa di un comando.
- Salvare la cronologia dei comandi (history).
- Cercare ed eseguire i comandi (usando la variabile d'ambiente PATH o un percorso assoluto/relativo).
- Gestire le virgolette singole (') e doppie (").
- Gestire le redirezioni:
  - `<`  per reindirizzare l'input
  - `>`  per reindirizzare l'output
  - `<<` per l'heredoc (input fino a un delimitatore)
  - `>>` per l'output in modalità append
- Gestire le pipe (`|`), collegando l'output di un comando all'input del successivo.
- Gestire le variabili d'ambiente (`$VAR`).
- Gestire `$?` (exit status dell'ultimo comando eseguito).
- Gestire i segnali:
  - ctrl-C: mostra un nuovo prompt
  - ctrl-D: esce dalla shell
  - ctrl-\: non fa nulla

## Comandi built-in da implementare
La shell deve implementare i seguenti comandi interni (built-in):
- `echo` (con opzione -n)
- `cd` (solo con percorso relativo o assoluto)
- `pwd` (senza opzioni)
- `export` (senza opzioni)
- `unset` (senza opzioni)
- `env` (senza opzioni o argomenti)
- `exit` (senza opzioni)

## Parte bonus (facoltativa)
Se la parte obbligatoria è perfetta, puoi implementare anche:
- Operatori logici `&&` e `||` con parentesi per le priorità
- Wildcard `*` funzionante nella directory corrente

---

## Divisione dei compiti per due persone

### Fase 1: Pianificazione e struttura
- **Entrambi:**
  - Leggete insieme il subject e chiarite ogni dubbio.
  - Decidete la struttura delle cartelle e dei file (ad esempio: src/, include/, builtins/, parser/, ecc.).
  - Create insieme il Makefile base.

### Fase 2: Suddivisione delle macro-aree

#### Persona 1: Gestione input, parsing e built-in
- **Prompt e input utente:**
  - Mostrare il prompt e leggere la linea di comando (usando readline).
  - Gestire la cronologia dei comandi (history).
- **Parsing della linea di comando:**
  - Gestire le virgolette singole e doppie.
  - Separare i comandi, gestire le pipe e le redirezioni.
  - Gestire le variabili d'ambiente ($VAR) e $? .
- **Implementazione dei built-in:**
  - echo, cd, pwd, export, unset, env, exit.

#### Persona 2: Esecuzione, processi e gestione avanzata
- **Esecuzione dei comandi:**
  - Cercare ed eseguire i comandi (PATH, execve).
  - Gestire la creazione di processi (fork, execve, wait).
- **Redirezioni e pipe:**
  - Implementare le redirezioni (<, >, <<, >>).
  - Implementare le pipe (|).
- **Gestione dei segnali:**
  - Gestire ctrl-C, ctrl-D, ctrl-\ come richiesto.
  - Gestire la variabile globale per i segnali.

### Fase 3: Integrazione e test
- **Entrambi:**
  - Unire le parti sviluppate separatamente.
  - Testare tutte le funzionalità insieme.
  - Correggere bug e memory leak.
  - Confrontare il comportamento con Bash.

### Fase 4: Bonus (se la parte obbligatoria è perfetta)
- **Entrambi:**
  - &&, ||, parentesi e wildcard *.

## Consigli pratici
- Fate spesso il punto della situazione insieme.
- Usate Git per lavorare in parallelo e risolvere conflitti.
- Commentate il codice per aiutarvi a vicenda.
- Se uno dei due ha difficoltà, aiutatelo: l'obiettivo è imparare entrambi!
