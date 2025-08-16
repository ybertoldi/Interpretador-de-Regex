#include <stdbool.h>

#ifndef NFA_AUTOMATA_H
#define NFA_AUTOMATA_H
#include "nfa_node.h"
typedef struct {
  NfaNode *key[8000];
  NfaNode *val[8000];
  int size;
} AuMap;

typedef struct nfauto {
  NfaNode *inicio;
  ll_node *estados_finais;
} NfAutomata;

NfAutomata novo_automato(char *nome, char c);
NfAutomata concat(NfAutomata n1, NfAutomata n2);
NfAutomata uniao(NfAutomata n1, NfAutomata n2);
NfAutomata estrela(NfAutomata n1);
NfAutomata automata_copy(NfAutomata n);

#ifdef NFA_AUTOMATA_TESTE
char *prox_nome();
char *nome_final(char *nome);
#endif

#endif
