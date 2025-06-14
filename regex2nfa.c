#include "stdbool.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUTOMATA_H_TESTE
#include "teste/automata.h"

int nome_count = 0;

char *prox_nome() {
  char *nome = malloc(5);
  nome[0] = 'q';
  nome[1] = '0' + nome_count++;
  nome[2] = '\0';
  return nome;
}

char *nome_final(char *nome) {
  char *nomee_final = malloc(10);
  strcpy(nomee_final, nome);
  strcat(nomee_final, "f");
  return nomee_final;
}

typedef struct g_node {
  void *val;
  struct g_node *next;
} GNode;

typedef struct nfauto {
  NfaNode *inicio;
  ll_node *estados_finais;
} NfAutomata;

GNode *gnew(void *val) {
  GNode *node = malloc(sizeof(GNode));
  node->next = NULL;
  node->val = val;
  return node;
}

void gappend(GNode *head, void *val) {
  GNode *cur = head;
  while (cur->next != NULL)
    cur = cur->next;

  cur->next = gnew(val);
}

void gappend_orcreate(GNode **pnode, void *val) {
  if (*(pnode) != NULL)
    gappend(*(pnode), val);
  else
    *(pnode) = gnew(val);
}

NfAutomata stack[8000];
uint32_t sp = 0;

NfaNode *nfalloc(char *name, bool isfinal) {
  NfaNode *n = malloc(sizeof(NfaNode));
  n->name = name;
  n->map = init_nfa_map();
  n->is_final = isfinal;
  return n;
}

void push(NfAutomata n) { stack[sp++] = n; }

NfAutomata pop() { return stack[--sp]; }

NfAutomata novo_automato(char *nome, char c) {
  NfaNode *i = nfalloc(nome, false);
  NfaNode *f = nfalloc(nome_final(nome), true);
  nfa_add_delta(i, c, f);

  ll_node *lista_finais = llnew(f);
  NfAutomata n = {.inicio = i, .estados_finais = lista_finais};
  return n;
}

NfAutomata concat(NfAutomata n1, NfAutomata n2) {
  ll_node *ls = n1.estados_finais;
  while (ls) {
    ls->val->is_final = false;
    nfa_add_delta(ls->val, '\0', n2.inicio);
    ls = ls->next;
  }

  ll_node *dlt = n1.estados_finais;
  n1.estados_finais = llcopy(n2.estados_finais);
  llfree(dlt);
  return n1;
}

int main() {
  NfAutomata n1, n2;
  char *teste = "ab.b.";
  for (; *teste; teste++) {
    switch (*teste) {
    case '.':
      n2 = pop();
      n1 = pop();
      push(concat(n1, n2));
      break;

    default:
      push(novo_automato(prox_nome(), *teste));
      break;
    }
  }

  char *cadeia = malloc(100 * sizeof(char));
  printf("\ndigite uma cadeia na forma abb: ");
  fgets(cadeia, 100, stdin);

  if (nfa_teste(pop().inicio, cadeia))
    printf("\nEstado Aceito!\n");
  else
    printf("\nEstado Invalido\n");
}
