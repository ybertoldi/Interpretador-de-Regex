#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "automata.h"

int main() {
  char *cadeia = malloc(100 * sizeof(char));
  DfaNode q0 = BUILD_DFA_NODE("q0");
  DfaNode q1 = BUILD_DFA_NODE("q1");
  DfaNode q2 = BUILD_DFA_NODE("q2");
  DfaNode q3 = BUILD_DFA_NODE_FINAL("q3");

  dfa_add_delta(&q0, 'b', &q1); // S(q0,'b') = {q1}
  dfa_add_delta(&q0, 'a', &q0); // S(q0,'a') = {q0}

  dfa_add_delta(&q1, 'b', &q2); // S(q1,'b') = {q2}
  dfa_add_delta(&q1, 'a', &q0); // S(q1,'a') = {q0}

  dfa_add_delta(&q2, 'b', &q2); // S(q2,'b') = {q2}
  dfa_add_delta(&q2, 'a', &q3); // S(q2,'a') = {q3}

  printf("Automato atual:\n");
  print_dfa_node(&q0);
  print_dfa_node(&q1);
  print_dfa_node(&q2);
  print_dfa_node(&q3);

  printf("\ndigite uma cadeia na forma (a*)b(b+)a: ");
  fgets(cadeia, 100, stdin);

  if (dfa_valida_cadeia(&q0, cadeia))
    printf("Cadeia Valida\n");
  else
    printf("Cadeia Invalida\n");

  // atualiza automato
  dfa_add_delta(&q3, 'a', &q0); // S(q3,'a') = {q0}
  dfa_add_delta(&q3, 'b', &q1); // S(q3,'b') = {q1}

  printf("\nNovo automato:\n");
  print_dfa_node(&q0);
  print_dfa_node(&q1);
  print_dfa_node(&q2);
  print_dfa_node(&q3);

  printf("\nagora digite uma cadeia na forma ((a*)b(b+)a)*: ");
  fgets(cadeia, 100, stdin);

  if (dfa_valida_cadeia(&q0, cadeia))
    printf("Cadeia Valida\n");
  else
    printf("Cadeia Invalida\n");

  free(cadeia);
  return 0;
}
