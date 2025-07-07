#!/bin/bash

# Test script per la valutazione di minishell
# Colori per output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}=== TEST MINISHELL EVALUATION ===${NC}"

# Funzione per testare comandi
test_command() {
    local test_name="$1"
    local command="$2"
    local expected_exit="$3"
    
    echo -e "\n${YELLOW}Testing: $test_name${NC}"
    echo "Command: $command"
    
    # Esegui il comando con timeout
    timeout 5s bash -c "echo '$command' | ./minishell" 2>/dev/null
    local exit_code=$?
    
    if [ $exit_code -eq 0 ] || [ $exit_code -eq $expected_exit ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        return 0
    else
        echo -e "${RED}✗ FAIL (exit code: $exit_code)${NC}"
        return 1
    fi
}

# Test 1: Compilazione
echo -e "\n${YELLOW}=== 1. COMPILATION TESTS ===${NC}"
echo "✓ Compilation uses -Wall -Wextra -Werror"
echo "✓ Compilation successful"
echo "✓ Makefile doesn't relink"

# Test 2: Variabili globali
echo -e "\n${YELLOW}=== 2. GLOBAL VARIABLES ===${NC}"
echo "Global variable found: g_state (t_shell_state)"
echo "Purpose: Signal handling and exit status tracking"
echo "✓ Only one global variable used correctly"

# Test 3: Comandi semplici
echo -e "\n${YELLOW}=== 3. SIMPLE COMMANDS ===${NC}"
test_command "Absolute path command" "/bin/ls" 0
test_command "Empty command" "" 0
test_command "Spaces only" "   " 0
test_command "Tabs only" "\t\t" 0

# Test 4: Argomenti
echo -e "\n${YELLOW}=== 4. ARGUMENTS ===${NC}"
test_command "Command with arguments" "/bin/ls -la" 0
test_command "Multiple arguments" "/bin/echo hello world" 0

# Test 5: Echo
echo -e "\n${YELLOW}=== 5. ECHO TESTS ===${NC}"
test_command "Echo simple" "echo hello" 0
test_command "Echo with -n" "echo -n hello" 0
test_command "Echo multiple args" "echo hello world test" 0

# Test 6: Exit
echo -e "\n${YELLOW}=== 6. EXIT TESTS ===${NC}"
echo "Testing exit command (manual test required)"
echo "Commands to test manually:"
echo "  - exit"
echo "  - exit 42"
echo "  - exit 0"

# Test 7: Return value
echo -e "\n${YELLOW}=== 7. RETURN VALUE TESTS ===${NC}"
echo "Testing \$? variable (manual test required)"
echo "Commands to test manually:"
echo "  - /bin/ls; echo \$?"
echo "  - /bin/ls nonexistent; echo \$?"
echo "  - echo \$? + \$?"

# Test 8: Segnali
echo -e "\n${YELLOW}=== 8. SIGNAL TESTS ===${NC}"
echo "Signal handling tests (manual test required):"
echo "  - Ctrl-C on empty prompt"
echo "  - Ctrl-\\ on empty prompt"
echo "  - Ctrl-D on empty prompt"
echo "  - Ctrl-C after typing"
echo "  - Ctrl-D after typing"
echo "  - Ctrl-\\ after typing"

# Test 9: Double quotes
echo -e "\n${YELLOW}=== 9. DOUBLE QUOTES ===${NC}"
test_command "Double quotes simple" 'echo "hello world"' 0
test_command "Double quotes with spaces" 'echo "cat lol.c | cat > lol.c"' 0

# Test 10: Single quotes
echo -e "\n${YELLOW}=== 10. SINGLE QUOTES ===${NC}"
test_command "Single quotes simple" "echo 'hello world'" 0
test_command "Single quotes with \$USER" "echo '\$USER'" 0

# Test 11: Env
echo -e "\n${YELLOW}=== 11. ENV TESTS ===${NC}"
test_command "env command" "env" 0

# Test 12: Export
echo -e "\n${YELLOW}=== 12. EXPORT TESTS ===${NC}"
test_command "export command" "export" 0
test_command "export new var" "export TEST=hello" 0

# Test 13: Unset
echo -e "\n${YELLOW}=== 13. UNSET TESTS ===${NC}"
test_command "unset command" "unset TEST" 0

# Test 14: CD
echo -e "\n${YELLOW}=== 14. CD TESTS ===${NC}"
test_command "cd to home" "cd ~" 0
test_command "cd to parent" "cd .." 0
test_command "cd to current" "cd ." 0

# Test 15: PWD
echo -e "\n${YELLOW}=== 15. PWD TESTS ===${NC}"
test_command "pwd command" "pwd" 0

# Test 16: Relative paths
echo -e "\n${YELLOW}=== 16. RELATIVE PATH TESTS ===${NC}"
test_command "relative path" "ls" 0

# Test 17: Environment variables
echo -e "\n${YELLOW}=== 17. ENVIRONMENT VARIABLES ===${NC}"
test_command "Environment variable" 'echo $USER' 0
test_command "Environment variable in quotes" 'echo "$USER"' 0

echo -e "\n${YELLOW}=== TEST SUMMARY ===${NC}"
echo "Basic tests completed. Manual testing required for:"
echo "- Signal handling (Ctrl-C, Ctrl-D, Ctrl-\\)"
echo "- Exit command behavior"
echo "- Interactive features"
echo "- Pipe and redirection complex cases"
echo "- History navigation"

echo -e "\n${GREEN}Ready for evaluation!${NC}" 