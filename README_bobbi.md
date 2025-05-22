# Compiti Bobbi: Esecuzione, Processi e Gestione Avanzata

## 1. Esecuzione dei comandi
- Ricevere dalla Persona 1 la struttura dati con i comandi e i loro argomenti.
- Cercare il percorso dell'eseguibile usando la variabile d'ambiente PATH (se il comando non è built-in).
- Usare le funzioni fork ed execve per creare nuovi processi e lanciare i comandi esterni.
- Gestire la terminazione dei processi figli con wait e waitpid.
- Gestire l'exit status e comunicarlo alla Persona 1 per l'espansione di $?.

## 2. Redirezioni e pipe
- Implementare la gestione delle redirezioni:
  - `<`  reindirizza l'input da un file
  - `>`  reindirizza l'output verso un file (sovrascrive)
  - `>>` reindirizza l'output verso un file (in modalità append)
  - `<<` heredoc: legge l'input fino a un delimitatore
- Implementare le pipe (`|`): collegare l'output di un comando all'input del successivo creando le pipe necessarie e gestendo i file descriptor.
- Gestire la chiusura corretta dei file descriptor per evitare memory leak e comportamenti strani.

## 3. Gestione dei segnali
- Gestire i segnali ctrl-C, ctrl-D, ctrl-\ come richiesto dal subject:
  - ctrl-C: mostra un nuovo prompt senza chiudere la shell
  - ctrl-D: esce dalla shell
  - ctrl-\: non fa nulla
- Usare una sola variabile globale per memorizzare il numero del segnale ricevuto (come richiesto dal subject).
- Assicurarsi che la gestione dei segnali non interferisca con le strutture dati principali.

## 4. Gestione della memoria
- Liberare tutta la memoria allocata dinamicamente dopo l'esecuzione di ogni comando.
- Verificare che non ci siano memory leak (tranne quelli causati da readline, che sono accettati).

## 5. Integrazione e collaborazione
- Collaborare con la Persona 1 per ricevere i comandi già "parsati" e pronti all'esecuzione.
- Restituire l'exit status e gli eventuali errori alla Persona 1.
- Testare ogni funzionalità confrontando il comportamento con Bash.
- Scrivere commenti chiari nel codice per aiutare l'altra persona.

## Suggerimenti
- Procedi per passi: prima esecuzione semplice, poi aggiungi pipe e redirezioni.
- Usa funzioni di supporto per non superare il limite di 25 righe per funzione.
- Se hai dubbi su come Bash si comporta, prova direttamente da terminale. 