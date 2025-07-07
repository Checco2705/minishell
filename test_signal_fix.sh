#!/bin/bash

echo "=== TEST CORREZIONE SEGNALI MINISHELL ==="
echo ""
echo "Per testare la correzione del doppio prompt:"
echo ""
echo "1. Avvia la minishell: ./minishell"
echo "2. Digita: cat"
echo "3. Premi CTRL+C"
echo "4. Dovrai vedere solo UN prompt 'minishell$' invece di due"
echo ""
echo "Altri test da fare:"
echo "- 'grep pattern' poi CTRL+C"
echo "- 'sleep 10' poi CTRL+C"  
echo "- 'yes' poi CTRL+C"
echo ""
echo "In tutti i casi dovresti vedere solo un prompt dopo CTRL+C"
echo ""
echo "Premi ENTER per avviare la minishell..."
read
./minishell 