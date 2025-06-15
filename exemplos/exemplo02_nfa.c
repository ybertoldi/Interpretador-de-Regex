#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NFA_NODE_TESTE
#include "../includes/nfa_node.h"

int main() {
  char *cadeia = malloc(100 * sizeof(char));
  NfaNode q0 = BUILD_NFA_NODE("q0");
  NfaNode q1 = BUILD_NFA_NODE("q1");
  NfaNode q2 = BUILD_NFA_NODE_FINAL("q2");
  NfaNode q3 = BUILD_NFA_NODE("q3");
  NfaNode q4 = BUILD_NFA_NODE_FINAL("q4");

  nfa_add_delta(&q0, '\0', &q0); // d(q0, eps) = {q0}
  nfa_add_delta(&q0, '1', &q3);  // d(q0, '1') = {q3}
  nfa_add_delta(&q0, '0', &q1);  // d(q0, '0') = {q1}

  nfa_add_delta(&q1, '0', &q2); // d(q1, '0') = {q2}
  nfa_add_delta(&q3, '1', &q4); // d(q3, '1') = {q4}

  print_nfa_node(&q0);
  print_nfa_node(&q1);
  print_nfa_node(&q2);
  print_nfa_node(&q3);
  print_nfa_node(&q4);

  printf("\ndigite uma cadeia na forma (1|0)*(11|00): ");
  fgets(cadeia, 100, stdin);

  // faz o mesmo que nfa_valida_cadeia, mas mostrando o passo-a-passo
  if (nfa_teste(&q0, cadeia))
    printf("\nEstado Aceito!\n");
  else
    printf("\nEstado Invalido\n");
}
