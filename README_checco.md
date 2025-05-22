# Compiti Checco: Input, Parsing e Built-in

## 1. Prompt e input utente
- Mostrare il prompt ogni volta che la shell è pronta a ricevere un comando.
- Usare la funzione readline per leggere la linea di comando inserita dall'utente.
- Gestire la cronologia dei comandi (history), permettendo all'utente di scorrere i comandi precedenti con le frecce.

## 2. Parsing della linea di comando
- Analizzare la stringa inserita dall'utente e suddividerla in comandi e argomenti.
- Gestire le virgolette singole (') e doppie ("):
  - Le virgolette singole racchiudono testo che non deve essere interpretato (nessuna espansione di variabili o caratteri speciali).
  - Le virgolette doppie permettono l'espansione delle variabili ($VAR) ma non dei caratteri speciali.
- Gestire i caratteri di escape (\) se richiesto dal subject.
- Riconoscere e separare le pipe (|) e le redirezioni (<, >, <<, >>) nella linea di comando.
- Gestire l'espansione delle variabili d'ambiente ($VAR) e del valore di $? (exit status dell'ultimo comando).
- Gestire errori di sintassi (ad esempio virgolette non chiuse o redirezioni malformate).

## 3. Implementazione dei comandi built-in
- Implementare i seguenti comandi interni:
  - `echo` (gestire l'opzione -n per non andare a capo)
  - `cd` (cambiare directory solo con percorso relativo o assoluto, gestire errori se la directory non esiste)
  - `pwd` (stampare la directory corrente)
  - `export` (aggiungere/modificare variabili d'ambiente)
  - `unset` (rimuovere variabili d'ambiente)
  - `env` (stampare tutte le variabili d'ambiente)
  - `exit` (chiudere la shell, gestire eventuali argomenti)
- Ogni comando deve essere gestito come funzione separata.
- Gestire i casi limite e gli errori per ogni comando (ad esempio: export senza argomenti, unset di variabili non esistenti, ecc.).

## 4. Comunicazione con la parte di esecuzione
- Preparare le strutture dati che rappresentano i comandi e i loro argomenti, pronte per essere eseguite dalla Persona 2.
- Passare correttamente le informazioni sulle redirezioni e sulle pipe.

## 5. Test e collaborazione
- Testare ogni parte sviluppata confrontando il comportamento con Bash.
- Scrivere commenti chiari nel codice per aiutare l'altra persona.
- Collaborare per integrare il parsing con l'esecuzione vera e propria.

## Suggerimenti
- Lavora a piccoli passi: prima il prompt, poi il parsing semplice, poi aggiungi le funzionalità avanzate.
- Usa funzioni di supporto per non superare il limite di 25 righe per funzione.
- Se hai dubbi su come Bash si comporta, prova direttamente da terminale. 