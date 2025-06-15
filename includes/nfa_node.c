#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "nfa_node.h"

NfaMap init_nfa_map() {
  NfaMap new;
  new.size = 0;
  new.epsilon = NULL;
  return new;
}

ll_node *nfa_map_get(NfaMap *map, char key) {
  for (int i = 0; i < map->size; i++) {
    if (map->key[i] == key)
      return map->val[i];
  }
  return NULL;
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

ll_node *nfa_node_get(NfaNode *node, char key) {
  return nfa_map_get(&node->map, key);
}

NfaNode *nfalloc(char *name, bool isfinal) {
  NfaNode *n = malloc(sizeof(NfaNode));
  n->name = name;
  n->map = init_nfa_map();
  n->is_final = isfinal;
  return n;
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

// TODO: optimizar e remover
ll_node *nfa_pega_transicoes(NfaNode *node, char input) {
  ll_node *tovisit, *eps, *visited = NULL, *ret = NULL, *dlt;
  NfaNode *cur;

  tovisit = llcopy(nfa_node_get(node, input));
  eps = node->map.epsilon;
  while (eps) {
    llappend_orcreate(&tovisit, eps->val);
    eps = eps->next;
  }
  dlt = tovisit;

  while (tovisit) {
    cur = tovisit->val;
    if (cur->map.size > 0 || cur->is_final)
      llappend_orcreate(&ret, cur);
    llappend_orcreate(&tovisit, cur);

    eps = cur->map.epsilon;
    while (eps) {
      if (!llcontains(visited, eps->val))
        llappend(tovisit, eps->val);
      eps = eps->next;
    }
    tovisit = tovisit->next;
  }

  llfree(dlt);
  return ret;
}

void nfa_move(ll_node *estados_atuais, ll_node **novos_estados, char input) {
  ll_node *nova_lista = NULL, *transicoes, *e_transicoes, *cur;

  while (estados_atuais != NULL) {
    cur = estados_atuais;
    estados_atuais = estados_atuais->next;

    transicoes = nfa_pega_transicoes(cur->val, input);
    while (transicoes) {
      llappend_orcreate(&nova_lista, transicoes->val);
      transicoes = transicoes->next;
    }

    free(transicoes);
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
  ll_node *nodes;
  ll_node *new_nodes = NULL;

  nodes = nfa_pega_transicoes(estado_inicial, '\0');
  if (estado_inicial->map.size > 0 || estado_inicial->is_final) {
    llappend_orcreate(&nodes, estado_inicial);
  }

  printf("estado inicial: ");
  llprint(nodes);

  while (*cadeia != '\0' && *cadeia != '\n') {
    nfa_move(nodes, &new_nodes, *(cadeia));
    if (new_nodes == NULL) {
      erro_leitura = true;
      nodes = NULL;
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

