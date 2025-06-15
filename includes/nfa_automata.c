#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "nfa_automata.h"


/* FUNCOES APENAS PARA VISUALIZAR O AUTOMATO */
static int nome_count = 0;

char *prox_nome() {
  char *nome = malloc(20);
  sprintf(nome, "q%d", nome_count++);
  return nome;
}

char *nome_final(char *nome) {
  char *nome_final = malloc(20);
  sprintf(nome_final, "%sf", nome);
  return nome_final;
}
/* ------------------------------------------ */


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

NfAutomata uniao(NfAutomata n1, NfAutomata n2) {
  NfAutomata ret;

  NfaNode *novo = nfalloc(prox_nome(), false);
  nfa_add_delta(novo, '\0', n1.inicio);
  nfa_add_delta(novo, '\0', n2.inicio);
  ret.inicio = novo;

  ll_node *novos_finais = llcopy(n1.estados_finais);
  llcat(novos_finais, llcopy(n2.estados_finais));
  ret.estados_finais = novos_finais;

  llfree(n1.estados_finais);
  llfree(n2.estados_finais);
  return ret;
}

NfAutomata estrela(NfAutomata n1) {
  ll_node *l = n1.estados_finais;
  while (l) {
    nfa_add_delta(l->val, '\0', n1.inicio);
    l = l->next;
  }
  n1.inicio->is_final = true;
  llappend(n1.estados_finais, n1.inicio);
  return n1;
}

void aumap_add(AuMap *copied_nodes, NfaNode *node, NfaNode *node_copy) {
  copied_nodes->key[copied_nodes->size] = node;
  copied_nodes->val[copied_nodes->size] = node_copy;
  copied_nodes->size++;
}
NfaNode *aumap_get(AuMap *copied_nodes, NfaNode *key) {
  for (int i = 0; i < copied_nodes->size; i++) {
    if (copied_nodes->key[i] == key)
      return copied_nodes->val[i];
  }
  return NULL;
}

NfaNode *automata_copy_aux(NfaNode *node, AuMap *copied_nodes) {
  NfaNode *node_copy = nfalloc(prox_nome(), node->is_final);
  aumap_add(copied_nodes, node, node_copy);

  // faz copia do mapa
  for (int i = 0; i < node->map.size; i++) {
    char inpt = node->map.key[i];
    ll_node *links = node->map.val[i];

    while (links) {
      NfaNode *copied_link = aumap_get(copied_nodes, links->val);

      if (copied_link)
        nfa_add_delta(node_copy, inpt, copied_link);
      else
        nfa_add_delta(node_copy, inpt,
                      automata_copy_aux(links->val, copied_nodes));
      links = links->next;
    }
  }

  // faz copia do epsilon
  ll_node *eps = node->map.epsilon;
  while (eps) {
    NfaNode *copied = aumap_get(copied_nodes, eps->val);

    if (copied)
      nfa_add_delta(node_copy, '\0', copied);
    else
      nfa_add_delta(node_copy, '\0', automata_copy_aux(eps->val, copied_nodes));

    eps = eps->next;
  }

  return node_copy;
}

NfAutomata AutomataCopy(NfAutomata n) {
  AuMap copied_nodes;
  copied_nodes.size = 0;

  NfaNode *novo_grafo = automata_copy_aux(n.inicio, &copied_nodes);
  ll_node *novalista;
  ll_node *lista_copiar = n.estados_finais;
  while (lista_copiar) {
    llappend_orcreate(&novalista, aumap_get(&copied_nodes, lista_copiar->val));
    lista_copiar = lista_copiar->next;
  }

  NfAutomata novo;
  novo.inicio = novo_grafo;
  novo.estados_finais = novalista;

  return novo;
}
