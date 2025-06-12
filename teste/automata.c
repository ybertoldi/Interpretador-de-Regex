#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "automata.h"

// MAPAS E LISTA
DfaMap init_dfa_map() {
  DfaMap new;
  new.size = 0;
  return new;
}

DfaNode *dfa_map_get(DfaMap *map, char key) {
  for (int i = 0; i < map->size; i++) {
    if (map->key[i] == key)
      return map->val[i];
  }
  return NULL;
}

NfaMap init_nfa_map() {
  NfaMap new;
  new.size = 0;
  new.epsilon = NULL;
  return new;
}

ll_node *llnew(NfaNode *val) {
  ll_node *new = malloc(sizeof(ll_node));
  new->val = val;
  new->next = NULL;
  return new;
}

void llappend(ll_node *head, NfaNode *val) {
  ll_node *cur = head;
  while (cur->next != NULL)
    cur = cur->next;

  cur->next = llnew(val);
}

void llappend_orcreate(ll_node **pnode, NfaNode *val) {
  if (*(pnode) != NULL)
    llappend(*(pnode), val);
  else
    *(pnode) = llnew(val);
}

void llprint(ll_node *head) {
  printf("{");
  while (head) {
    printf("%s%s", head->val->name, (head->next == NULL) ? "}\n" : ", ");
    head = head->next;
  }
}

ll_node *nfa_map_get(NfaMap *map, char key) {
  for (int i = 0; i < map->size; i++) {
    if (map->key[i] == key)
      return map->val[i];
  }
  return NULL;
}
void dfa_add_delta(DfaNode *node, char key, DfaNode *value) {
  DfaMap *map = &(node->map);

  if (map->size >= MAX_MAP_SIZE) {
    fprintf(stderr, "ERROR - dfa_add_delta: mapa cheio\n");
    exit(EXIT_FAILURE);
  }
  map->key[map->size] = key;
  map->val[map->size] = value;
  map->size++;
}

bool dfa_node_move(DfaNode **node, char input) {
  DfaMap *map = &(*node)->map;
  DfaNode *mapped_node = dfa_map_get(map, input);
  if (mapped_node) {
    *node = mapped_node;
    return true;
  }
  return false;
}

void print_dfa_node(DfaNode *node) {
  printf("%s %s |", node->name, (node->is_final) ? " (final) " : "         ");

  for (int i = 0; i < node->map.size; i++) {
    printf(" '%c' : %s  |", node->map.key[i], node->map.val[i]->name);
  }
  printf("\n");
}

bool dfa_valida_cadeia(DfaNode *estado_inicial, char *cadeia) {
  bool char_invalido = false;
  DfaNode *est_atual = estado_inicial;
  while (*cadeia != '\0' && *cadeia != '\n') {
    if (!dfa_node_move(&est_atual, *(cadeia++))) {
      char_invalido = true;
      break;
    }
  }

  if (est_atual->is_final && !char_invalido)
    return true;
  return false;
}

ll_node *nfa_node_get(NfaNode *node, char key) {
  return nfa_map_get(&node->map, key);
}

void nfa_add_delta(NfaNode *node, char key, NfaNode *value) {
  NfaMap *map = &(node->map);

  if (map->size >= MAX_MAP_SIZE) {
    fprintf(stderr, "ERROR - nfa_add_delta: mapa cheio\n");
    exit(EXIT_FAILURE);
  }

  // caso o valor seja epsilon
  if (key == '\0') {
    llappend_orcreate(&map->epsilon, value);
    return;
  }

  // caso o valor ja esteja registrado
  for (int i = 0; i < map->size; i++) {
    if (map->key[i] == key) {
      llappend(map->val[i], value);
      return;
    }
  }

  map->key[map->size] = key;
  map->val[map->size] = llnew(value);
  map->size++;
}

void print_nfa_node(NfaNode *node) {
  printf("%s %s |", node->name, (node->is_final) ? " (final) " : "         ");

  for (int i = 0; i < node->map.size; i++) {
    printf(" '%c' : {", node->map.key[i]);

    ll_node *cur = node->map.val[i];
    while (cur != NULL) {
      printf("%s%s", cur->val->name, (cur->next != NULL) ? ", " : "");
      cur = cur->next;
    }
    printf("} | ");
  }

  printf("  eps : {");
  ll_node *cur_e = node->map.epsilon;
  while (cur_e != NULL) {
    printf("%s%s", cur_e->val->name, (cur_e->next != NULL) ? ", " : "");
    cur_e = cur_e->next;
  }
  printf("} |\n");
}

void nfa_move(ll_node *estados_atuais, ll_node **novos_estados, char input) {
  ll_node *nova_lista = NULL, *transicoes, *e_transicoes, *cur;

  while (estados_atuais != NULL) {
    cur = estados_atuais;
    estados_atuais = estados_atuais->next;

    transicoes = nfa_node_get(cur->val, input);
    e_transicoes = cur->val->map.epsilon;

    while (transicoes != NULL) {
      llappend_orcreate(&nova_lista, transicoes->val);
      transicoes = transicoes->next;
    }

    while (e_transicoes != NULL) {
      llappend_orcreate(&nova_lista, e_transicoes->val);
      e_transicoes = e_transicoes->next;
    }

    free(cur);
  }

  *novos_estados = nova_lista;
}
bool nfa_valida_cadeia(NfaNode *estado_inicial, char *cadeia) {
  bool erro_leitura = false;
  ll_node *nodes = llnew(estado_inicial);
  ll_node *new_nodes = NULL;

  while (*cadeia != '\0' && *cadeia != '\n') {
    nfa_move(nodes, &new_nodes, *(cadeia++));
    if (new_nodes == NULL) {
      erro_leitura = true;
      break;
    }
    nodes = new_nodes;
    new_nodes = NULL;
  }

  bool aceito = false;
  while (nodes != NULL) {
    if (nodes->val->is_final) {
      aceito = true;
      break;
    }
    nodes = nodes->next;
  }

  return aceito;
}

// FUNCOES DE TESTE
bool nfa_teste(NfaNode *estado_inicial, char *cadeia) {
  bool erro_leitura = false;
  ll_node *nodes = llnew(estado_inicial);
  ll_node *new_nodes = NULL;

  printf("estado inicial: ");
  llprint(nodes);

  while (*cadeia != '\0' && *cadeia != '\n') {
    nfa_move(nodes, &new_nodes, *(cadeia));
    if (new_nodes == NULL) {
      erro_leitura = true;
      break;
    }
    nodes = new_nodes;
    new_nodes = NULL;
    printf("%c -> ", *(cadeia++));
    llprint(nodes);
  }

  bool aceito = false;
  while (nodes != NULL) {
    if (nodes->val->is_final) {
      aceito = true;
      break;
    }
    nodes = nodes->next;
  }

  return aceito;
}
