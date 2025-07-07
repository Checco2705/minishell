#!/bin/bash

# Script di test per verificare il comportamento delle quote e l'espansione delle variabili

echo "=== TEST QUOTE E ESPANSIONE VARIABILI ==="
echo

# Compila la minishell
echo "Compilando minishell..."
make -C . all
echo

# Configura l'ambiente
export TEST_VAR="hello world"
export USER="test_user"
export HOME="/home/test"

# Test cases
echo "=== Test 1: Double quotes - dovrebbero espandere le variabili ==="
echo 'Input: echo "Hello \$USER, welcome to \$HOME"'
echo "Hello $USER, welcome to $HOME" | ./minishell

echo
echo "=== Test 2: Single quotes - NON dovrebbero espandere le variabili ==="
echo 'Input: echo '\''Hello \$USER, welcome to \$HOME'\'''
echo 'Hello $USER, welcome to $HOME' | ./minishell

echo
echo "=== Test 3: Misto single e double quotes ==="
echo 'Input: echo "Hello \$USER" '\''and \$HOME'\'''
echo "Hello $USER" 'and $HOME' | ./minishell

echo
echo "=== Test 4: Variabile con spazi in double quotes ==="
echo 'Input: echo "Value is: \$TEST_VAR"'
echo "Value is: $TEST_VAR" | ./minishell

echo
echo "=== Test 5: Variabile con spazi in single quotes ==="
echo 'Input: echo '\''Value is: \$TEST_VAR'\'''
echo 'Value is: $TEST_VAR' | ./minishell

echo
echo "=== Test 6: Exit status in double quotes ==="
echo 'Input: echo "Exit status: \$?"'
echo "Exit status: $?" | ./minishell

echo
echo "=== Test 7: Exit status in single quotes ==="
echo 'Input: echo '\''Exit status: \$?'\'''
echo 'Exit status: $?' | ./minishell

echo
echo "=== Test 8: Escape nella double quote ==="
echo 'Input: echo "Hello \"world\" with \$USER"'
echo "Hello \"world\" with $USER" | ./minishell

echo
echo "=== Test 9: Concatenazione di quote ==="
echo 'Input: echo "Hello "\$USER'\'' world'\'''
echo "Hello "$USER' world' | ./minishell

echo
echo "=== Test 10: Variabile non esistente ==="
echo 'Input: echo "Undefined: \$UNDEFINED_VAR"'
echo "Undefined: $UNDEFINED_VAR" | ./minishell

echo
echo "=== Test con minishell interattiva ==="
echo "Avviando minishell interattiva per test manuali..."
echo "Puoi testare manualmente i seguenti comandi:"
echo 'echo "Hello $USER"'
echo 'echo '\''Hello $USER'\'''
echo 'echo "Exit: $?"'
echo 'echo '\''Exit: $?'\'''
echo "exit per uscire"
echo

./minishell 