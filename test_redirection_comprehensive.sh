#!/bin/bash

# Test completo per redirection minishell
# Colori per output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${YELLOW}=== TEST COMPLETO REDIRECTION MINISHELL ===${NC}\n"

# Funzione per testare comandi
test_redirection() {
    local test_name="$1"
    local command="$2"
    local expected_result="$3"
    local check_file="$4"
    
    echo -e "${BLUE}Test: $test_name${NC}"
    echo "Comando: $command"
    
    # Esegui il comando
    echo "$command" | ./minishell > /dev/null 2>&1
    local exit_code=$?
    
    # Verifica risultato
    if [ "$check_file" != "" ]; then
        if [ -f "$check_file" ]; then
            echo -e "${GREEN}✓ File $check_file creato${NC}"
            if [ "$expected_result" != "" ]; then
                if grep -q "$expected_result" "$check_file"; then
                    echo -e "${GREEN}✓ Contenuto corretto${NC}"
                else
                    echo -e "${RED}✗ Contenuto errato${NC}"
                    echo "Contenuto trovato: $(cat $check_file)"
                fi
            fi
        else
            echo -e "${RED}✗ File $check_file non creato${NC}"
        fi
    fi
    
    echo "Exit code: $exit_code"
    echo "---"
}

# Pulisci file di test precedenti
rm -f test_*.txt output_*.txt input_*.txt error_*.txt

# Crea file di input per i test
echo "content for input" > input_test.txt
echo "line1" > input_multi.txt
echo "line2" >> input_multi.txt

echo -e "${YELLOW}=== 1. REDIRECTION SEMPLICI ===${NC}"

# Test redirection output semplice
test_redirection "Output semplice" "echo 'test output' > test_output.txt" "test output" "test_output.txt"

# Test redirection input semplice  
test_redirection "Input semplice" "cat < input_test.txt > test_input_result.txt" "content for input" "test_input_result.txt"

# Test redirection append
test_redirection "Append" "echo 'first line' > test_append.txt && echo 'second line' >> test_append.txt" "second line" "test_append.txt"

echo -e "\n${YELLOW}=== 2. REDIRECTION MULTIPLE ===${NC}"

# Test redirection multiple (input + output)
test_redirection "Input + Output" "cat < input_test.txt > test_multi_io.txt" "content for input" "test_multi_io.txt"

# Test redirection multiple dello stesso tipo (dovrebbe usare l'ultima)
test_redirection "Output multipli" "echo 'test' > test_first.txt > test_last.txt" "test" "test_last.txt"

# Test combinazione append e output
test_redirection "Output + Append" "echo 'base' > test_combo.txt && echo 'added' >> test_combo.txt" "added" "test_combo.txt"

echo -e "\n${YELLOW}=== 3. GESTIONE ERRORI ===${NC}"

# Test file inesistente
echo -e "${BLUE}Test: File inesistente${NC}"
echo "cat < file_non_esistente.txt" | ./minishell 2>&1 | grep -q "non esistente"
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Errore file inesistente gestito correttamente${NC}"
else
    echo -e "${RED}✗ Errore file inesistente non gestito${NC}"
fi
echo "---"

# Test permessi negati
echo -e "${BLUE}Test: Permessi negati${NC}"
echo "echo 'test' > /root/no_permission.txt" | ./minishell 2>&1 | grep -q -i "permission\|permess"
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Errore permessi gestito correttamente${NC}"
else
    echo -e "${RED}✗ Errore permessi non gestito${NC}"
fi
echo "---"

# Test fallimenti multipli
echo -e "${BLUE}Test: Fallimenti redirection multiple${NC}"
echo "cat < file_non_esistente.txt > test_fail.txt" | ./minishell 2>&1 >/dev/null
if [ ! -f "test_fail.txt" ]; then
    echo -e "${GREEN}✓ File output non creato quando input fallisce${NC}"
else
    echo -e "${RED}✗ File output creato anche se input fallisce${NC}"
fi
echo "---"

echo -e "\n${YELLOW}=== 4. HEREDOC TESTS ===${NC}"

# Test heredoc semplice
echo -e "${BLUE}Test: Heredoc semplice${NC}"
echo -e "cat << EOF > test_heredoc.txt\nline1\nline2\nEOF" | ./minishell >/dev/null 2>&1
if [ -f "test_heredoc.txt" ] && grep -q "line1" "test_heredoc.txt" && grep -q "line2" "test_heredoc.txt"; then
    echo -e "${GREEN}✓ Heredoc semplice funziona${NC}"
else
    echo -e "${RED}✗ Heredoc semplice non funziona${NC}"
fi
echo "---"

# Test heredoc multipli
echo -e "${BLUE}Test: Heredoc multipli${NC}"
echo -e "cat << EOF1 << EOF2 > test_heredoc_multi.txt\ncontent1\nEOF1\ncontent2\nEOF2" | ./minishell >/dev/null 2>&1
if [ -f "test_heredoc_multi.txt" ]; then
    echo -e "${GREEN}✓ Heredoc multipli gestiti${NC}"
    echo "Contenuto: $(cat test_heredoc_multi.txt)"
else
    echo -e "${RED}✗ Heredoc multipli non funzionano${NC}"
fi
echo "---"

echo -e "\n${YELLOW}=== 5. CASI SPECIALI ===${NC}"

# Test con comandi built-in
test_redirection "Built-in con redirection" "echo 'pwd test' > test_builtin.txt && pwd >> test_builtin.txt" "test" "test_builtin.txt"

# Test con pipeline e redirection
echo -e "${BLUE}Test: Pipeline + Redirection${NC}"
echo "echo 'pipeline test' | cat > test_pipeline.txt" | ./minishell >/dev/null 2>&1
if [ -f "test_pipeline.txt" ] && grep -q "pipeline test" "test_pipeline.txt"; then
    echo -e "${GREEN}✓ Pipeline con redirection funziona${NC}"
else
    echo -e "${RED}✗ Pipeline con redirection non funziona${NC}"
fi
echo "---"

# Test con caratteri speciali nel nome file
test_redirection "Caratteri speciali" "echo 'special chars' > 'test file with spaces.txt'" "special chars" "test file with spaces.txt"

echo -e "\n${YELLOW}=== RIEPILOGO FILES CREATI ===${NC}"
ls -la test_* input_* 2>/dev/null | head -10

echo -e "\n${GREEN}=== TEST COMPLETATI ===${NC}"
echo "Per pulire i file di test: rm -f test_*.txt input_*.txt 'test file with spaces.txt'" 