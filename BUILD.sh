mkdir execs

gcc -o execs/ex1Dfa exemplos/exemplo01_dfa.c includes/dfa_node.c 
gcc -o execs/ex2Nfa exemplos/exemplo02_nfa.c includes/nfa_node.c
gcc -o execs/ex3Re2Post exemplos/exemplo03_regex2polones.c includes/regex2post.c
gcc -o execs/programa_principal main.c includes/nfa_automata.c includes/nfa_node.c includes/regex2post.c
