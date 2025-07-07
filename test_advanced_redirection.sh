#!/bin/bash

# Test avanzati per redirection - casi edge e specifici per la valutazione
echo "=== TEST AVANZATI REDIRECTION - CASI EDGE ==="

# Pulisci file precedenti
rm -f adv_*.txt

echo "1. Test: Repeat multiple times with different commands and arguments"
echo "   (Il comportamento: esegue comandi con diversi argomenti e redirection diverse)"

# Test 1: Stesso comando con argomenti diversi e redirection diverse
echo "echo 'first test' > adv_test1.txt" | ./minishell
echo "echo 'second test with args' arg1 arg2 > adv_test2.txt" | ./minishell  
echo "echo 'third test' >> adv_test1.txt" | ./minishell

echo "✓ Comando ripetuto con argomenti diversi:"
echo "File 1: $(cat adv_test1.txt)"
echo "File 2: $(cat adv_test2.txt)"
echo

echo "2. Test: Change > with >> (append invece di overwrite)"
echo "echo 'base content' > adv_change.txt" | ./minishell
echo "echo 'appended content' >> adv_change.txt" | ./minishell
echo "✓ Cambio da > a >>:"
echo "Contenuto finale: $(cat adv_change.txt)"
echo

echo "3. Test: Check if multiple tries of the same redirection fail"
echo "   (Testa comportamento quando redirection multiple falliscono)"

# Crea un file read-only per testare fallimenti
echo "test content" > adv_readonly.txt
chmod 444 adv_readonly.txt

# Test fallimento scrittura su file read-only
echo "echo 'fail test' > adv_readonly.txt > adv_backup.txt" | ./minishell 2>/dev/null
if [ ! -f "adv_backup.txt" ]; then
    echo "✓ Redirection multiple fermate al primo fallimento"
else
    echo "✗ Redirection continue anche dopo fallimento"
fi

chmod 644 adv_readonly.txt  # ripristina permessi
echo

echo "4. Test: Complex heredoc scenarios"
echo "   (Test << redirection - non deve aggiornare history)"

# Test heredoc con variabili
export TEST_VAR="variabile_test"
echo -e "cat << 'EOF' > adv_heredoc_quoted.txt\n\$TEST_VAR should not expand\nline2\nEOF" | ./minishell 2>/dev/null

echo -e "cat << EOF > adv_heredoc_unquoted.txt\n\$TEST_VAR should expand to: $TEST_VAR\nline2\nEOF" | ./minishell 2>/dev/null

echo "✓ Heredoc con delimitatori quotati (no espansione):"
if [ -f "adv_heredoc_quoted.txt" ]; then
    echo "Contenuto: $(cat adv_heredoc_quoted.txt)"
else
    echo "File non creato"
fi

echo "✓ Heredoc con delimitatori non quotati (con espansione):"
if [ -f "adv_heredoc_unquoted.txt" ]; then
    echo "Contenuto: $(cat adv_heredoc_unquoted.txt)"
else
    echo "File non creato"
fi
echo

echo "5. Test: Redirection precedence e ordine"
echo "   (Test ordine di elaborazione delle redirection multiple)"

# Test: input poi output
echo "input_data" > adv_input.txt
echo "cat < adv_input.txt > adv_output_order1.txt > adv_output_order2.txt" | ./minishell

echo "✓ Test ordine redirection (dovrebbe usare ultimo output):"
echo "File 1 (dovrebbe essere vuoto): '$(cat adv_output_order1.txt 2>/dev/null || echo "FILE_VUOTO")'"
echo "File 2 (dovrebbe avere contenuto): '$(cat adv_output_order2.txt 2>/dev/null || echo "NESSUN_CONTENUTO")'"
echo

echo "6. Test: Mixed redirection types"
echo "   (Combinazioni diverse di redirection)"

# Test: input + output + append su file diversi
echo "base_data" > adv_input_mixed.txt
echo "existing_content" > adv_append_target.txt

echo "cat < adv_input_mixed.txt > adv_mixed_out.txt && echo 'extra' >> adv_append_target.txt" | ./minishell

echo "✓ Redirection miste:"
echo "Output normale: $(cat adv_mixed_out.txt)"  
echo "File con append: $(cat adv_append_target.txt)"
echo

echo "7. Test: Error handling e file descriptor management"
echo "   (Gestione corretta chiusura FD anche in caso di errore)"

# Test che simula leak di file descriptor
for i in {1..5}; do
    echo "cat < /dev/null > adv_fd_test_$i.txt" | ./minishell 2>/dev/null
done

echo "✓ Test gestione file descriptor (dovrebbero essere tutti creati):"
ls adv_fd_test_*.txt 2>/dev/null | wc -l | tr -d ' ' | xargs echo "File creati:"
echo

echo "8. Test: Syntax edge cases"
echo "   (Casi limite di sintassi)"

# Test spazi attorno agli operatori
echo "echo 'spaces test'    >    adv_spaces.txt" | ./minishell
echo "echo 'tabs test' 	>	adv_tabs.txt" | ./minishell

echo "✓ Test spazi/tab attorno operatori:"
echo "Con spazi: $(cat adv_spaces.txt 2>/dev/null || echo "FALLITO")"
echo "Con tab: $(cat adv_tabs.txt 2>/dev/null || echo "FALLITO")"
echo

echo "=== RIEPILOGO FILES CREATI ==="
ls -la adv_*.txt 2>/dev/null | head -15

echo
echo "=== CLEANUP ==="
echo "Per pulire: rm -f adv_*.txt"
echo "chmod 644 adv_readonly.txt 2>/dev/null; rm -f adv_readonly.txt" 