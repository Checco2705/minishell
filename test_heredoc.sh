#!/bin/bash

# Test script per gli heredoc del minishell
# Basato su test_hederoc.txt

# Colori per output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Contatori
TOTAL_TESTS=0
PASSED_TESTS=0

# Funzione per stampare header
print_header() {
    echo -e "${BLUE}============================================${NC}"
    echo -e "${BLUE}    TEST HEREDOC MINISHELL${NC}"
    echo -e "${BLUE}============================================${NC}"
}

# Funzione per test
run_test() {
    local test_name="$1"
    local test_command="$2"
    local expected_behavior="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "\n${YELLOW}=== $test_name ===${NC}"
    echo -e "${BLUE}Comando:${NC} $test_command"
    echo -e "${BLUE}Comportamento atteso:${NC} $expected_behavior"
    echo -e "${BLUE}Output:${NC}"
    
    # Esegui il test
    if eval "$test_command"; then
        echo -e "${GREEN}✓ Test completato${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ Test fallito (exit code: $?)${NC}"
    fi
}

# Funzione per test interattivo
run_interactive_test() {
    local test_name="$1"
    local input_lines="$2"
    local expected_behavior="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "\n${YELLOW}=== $test_name ===${NC}"
    echo -e "${BLUE}Input:${NC}"
    echo "$input_lines"
    echo -e "${BLUE}Comportamento atteso:${NC} $expected_behavior"
    echo -e "${BLUE}Output:${NC}"
    
    # Esegui il test interattivo
    if echo -e "$input_lines" | ./minishell; then
        echo -e "${GREEN}✓ Test completato${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ Test fallito (exit code: $?)${NC}"
    fi
}

# Funzione per test con timeout
run_test_with_timeout() {
    local test_name="$1"
    local input_lines="$2"
    local expected_behavior="$3"
    local timeout_seconds="${4:-10}"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "\n${YELLOW}=== $test_name ===${NC}"
    echo -e "${BLUE}Input:${NC}"
    echo "$input_lines"
    echo -e "${BLUE}Comportamento atteso:${NC} $expected_behavior"
    echo -e "${BLUE}Output:${NC}"
    
    # Esegui il test con timeout
    if timeout "$timeout_seconds" bash -c "echo -e '$input_lines' | ./minishell"; then
        echo -e "${GREEN}✓ Test completato${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        local exit_code=$?
        if [ $exit_code -eq 124 ]; then
            echo -e "${RED}✗ Test fallito (timeout dopo $timeout_seconds secondi)${NC}"
        else
            echo -e "${RED}✗ Test fallito (exit code: $exit_code)${NC}"
        fi
    fi
}

# Inizio test
print_header

# Verifica che minishell esista
if [ ! -f "./minishell" ]; then
    echo -e "${RED}Errore: minishell non trovato. Compila prima con 'make'${NC}"
    exit 1
fi

echo -e "${BLUE}Versione minishell:${NC} $(./minishell --version 2>/dev/null || echo 'N/A')"

# Test 1: Heredoc semplice
run_test_with_timeout \
    "Test 1: Heredoc Semplice" \
    "cat << EOF\nhello\nworld\nEOF\nexit" \
    "Dovrebbe stampare 'hello' e 'world'"

# Test 2: Heredoc con espansione variabili
run_test_with_timeout \
    "Test 2: Heredoc con Espansione Variabili" \
    "cat << EOF\nHello \$USER\nCurrent directory: \$PWD\nEOF\nexit" \
    "Dovrebbe espandere le variabili USER e PWD"

# Test 3: Heredoc con delimiter quotato (no espansione)
run_test_with_timeout \
    "Test 3: Heredoc con Delimiter Quotato" \
    "cat << 'EOF'\nHello \$USER\nThis \$PWD will not expand\nEOF\nexit" \
    "Le variabili NON dovrebbero essere espanse"

# Test 4: Heredoc con doppie quote
run_test_with_timeout \
    "Test 4: Heredoc con Doppie Quote" \
    'cat << "EOF"\nHello $USER\nThis $PWD will not expand\nEOF\nexit' \
    "Le variabili NON dovrebbero essere espanse"

# Test 5: Heredoc multipli
run_test_with_timeout \
    "Test 5: Heredoc Multipli" \
    "cat << EOF1 << EOF2\ncontent for EOF2\nEOF2\ncontent for EOF1\nEOF1\nexit" \
    "Dovrebbe processare EOF2 per primo, poi EOF1"

# Test 6: Heredoc con pipe
run_test_with_timeout \
    "Test 6: Heredoc con Pipe" \
    "cat << EOF | grep hello\nhello world\ngoodbye world\nEOF\nexit" \
    "Dovrebbe filtrare solo le linee con 'hello'"

# Test 7: Heredoc con redirezione output
run_test_with_timeout \
    "Test 7: Heredoc con Redirezione Output" \
    "cat << EOF > test_output.txt\ncontent here\nmore content\nEOF\ncat test_output.txt\nrm -f test_output.txt\nexit" \
    "Dovrebbe scrivere nel file e poi mostrare il contenuto"

# Test 8: Heredoc con variabile speciale $?
run_test_with_timeout \
    "Test 8: Heredoc con Variabile \$?" \
    "true\ncat << EOF\nLast exit status: \$?\nEOF\nexit" \
    "Dovrebbe mostrare exit status 0"

# Test 9: Heredoc con caratteri speciali
run_test_with_timeout \
    "Test 9: Heredoc con Caratteri Speciali" \
    "cat << EOF\nSpecial chars: ! @ # \$ % ^ & * ( ) - + = [ ] { } | \\ : ; \" ' < > , . ? /\nEOF\nexit" \
    "Dovrebbe gestire correttamente i caratteri speciali"

# Test 10: Heredoc vuoto
run_test_with_timeout \
    "Test 10: Heredoc Vuoto" \
    "cat << EOF\nEOF\nexit" \
    "Dovrebbe gestire heredoc vuoto senza errori"

# Test 11: Heredoc con delimitatore lungo
run_test_with_timeout \
    "Test 11: Heredoc con Delimitatore Lungo" \
    "cat << VERY_LONG_DELIMITER_NAME\nContent with long delimiter\nVERY_LONG_DELIMITER_NAME\nexit" \
    "Dovrebbe gestire delimitatori lunghi"

# Test 12: Heredoc con spazi nel delimitatore (non dovrebbe funzionare)
run_test_with_timeout \
    "Test 12: Heredoc con Spazi (Test Errore)" \
    "cat << 'EOF WITH SPACES'\nThis should not work\nEOF WITH SPACES\nexit" \
    "Dovrebbe gestire l'errore di sintassi"

# Riepilogo
echo -e "\n${BLUE}============================================${NC}"
echo -e "${BLUE}    RIEPILOGO TEST${NC}"
echo -e "${BLUE}============================================${NC}"
echo -e "${BLUE}Test totali:${NC} $TOTAL_TESTS"
echo -e "${GREEN}Test passati:${NC} $PASSED_TESTS"
echo -e "${RED}Test falliti:${NC} $((TOTAL_TESTS - PASSED_TESTS))"

if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
    echo -e "\n${GREEN}🎉 Tutti i test sono passati!${NC}"
    exit 0
else
    echo -e "\n${YELLOW}⚠️  Alcuni test sono falliti. Controlla l'output sopra.${NC}"
    exit 1
fi 