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

ll_node *llcopy(ll_node *l) {
  ll_node *nl = NULL;
  while (l) {
    llappend_orcreate(&nl, l->val);
    l = l->next;
  }
  return nl;
}

void llappend(ll_node *head, NfaNode *val) {
  ll_node *cur = head, *toappend;

  while (cur) {
    if (cur->val == val)
      return;
    if (cur->next == NULL) 
      toappend = cur;

    cur = cur->next;
  }
  toappend->next = llnew(val);
}

void llcat(ll_node *head, ll_node *new) {
  ll_node *cur = head;
  while (cur->next != NULL) {
    cur = cur->next;
  }
  cur->next = new;
}

void llappend_orcreate(ll_node **pnode, NfaNode *val) {
  if (*(pnode) != NULL)
    llappend(*(pnode), val);
  else
    *(pnode) = llnew(val);
}

bool llcontains(ll_node *l, NfaNode *val) {
  ll_node *cur = l;
  while (cur) {
    if (cur->val == val) {
      return true;
    }
    cur = cur->next;
  }
  return false;
}

void llfree(ll_node *l) {
  ll_node *dl;
  while (l) {
    dl = l;
    l = l->next;
    free(dl);
  }
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

void get_full_epilson(ll_node *estados) {
  ll_node *tovisit, *visited = NULL, *res = NULL, *estados_copy, *dlt1, *dlt2;
  estados_copy = llcopy(estados);
  dlt2 = estados_copy;

  while (estados_copy) {
    tovisit = llnew(estados_copy->val);
    dlt1 = tovisit;

    while (tovisit) {
      NfaNode *cur = tovisit->val;
      llappend(estados, cur);

      ll_node *cur_epilson = cur->map.epsilon;
      while (cur_epilson) {
        if (!llcontains(visited, cur_epilson->val)) {
          llappend(tovisit, cur_epilson->val);
          llappend_orcreate(&visited, cur_epilson->val);
        }
        cur_epilson = cur_epilson->next;
      }

      tovisit = tovisit->next;
    }
    estados_copy = estados_copy->next;
    llfree(dlt1);
  }
  llfree(dlt2);
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

  get_full_epilson(nova_lista);
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


//TODO: Arrumar este problema:
//
//digite uma cadeia na forma abb: abbb
// estado inicial: {q0}
// a -> {q0f, q1}
// b -> {q1, q1f, q2}
// b -> {q1f, q2, q2f}
// b -> {q2, q2f}
//
// Estado Aceito!
//
//
// q0f deve ser substituido imediatamente por q1 ao ler 'a' e por ai vai...
