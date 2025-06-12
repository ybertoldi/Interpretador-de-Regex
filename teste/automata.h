#include <stdint.h>

#ifndef AUTOMATA_H

#define AUTOMATA_H
#define MAX_MAP_SIZE 256

#define BUILD_DFA_NODE(s_name)                                                 \
  {.name = s_name, .map = init_dfa_map(), .is_final = false}

#define BUILD_DFA_NODE_FINAL(s_name)                                           \
  {.name = s_name, .map = init_dfa_map(), .is_final = true}

#define BUILD_NFA_NODE(s_name)                                                 \
  {.name = s_name, .map = init_nfa_map(), .is_final = false}

#define BUILD_NFA_NODE_FINAL(s_name)                                           \
  {.name = s_name, .map = init_nfa_map(), .is_final = true}

// AUTOMATOS DETERMINISTICOS
typedef struct dfa_node_map {
  char key[MAX_MAP_SIZE];
  struct dfa_node *val[MAX_MAP_SIZE];
  uint32_t size;
} DfaMap;

typedef struct dfa_node {
  char *name;
  struct dfa_node_map map;
  bool is_final;
} DfaNode;

// AUTOMATOS NAO DETERMINISTICOS
typedef struct ll_node {
  struct nfa_node *val;
  struct ll_node *next;
} ll_node;

typedef struct nfa_node_map {
  char key[MAX_MAP_SIZE];
  ll_node *val[MAX_MAP_SIZE];
  ll_node *epsilon;
  uint32_t size;
} NfaMap;

typedef struct nfa_node {
  char *name;
  NfaMap map;
  bool is_final;
} NfaNode;

// MAPAS e LISTA-LIGADA
DfaMap init_dfa_map(void);
DfaNode *dfa_map_get(DfaMap *map, char key);

NfaMap init_nfa_map();
ll_node *nfa_map_get(NfaMap *map, char key);
ll_node *llnew(NfaNode *val);
void llappend(ll_node *head, NfaNode *val);
void llappend_orcreate(ll_node **pnode, NfaNode *val);
void llprint(ll_node *head);

// funcoes DFA
void dfa_add_delta(DfaNode *node, char key, DfaNode *value);
bool nfa_node_move(DfaNode **node, char input);
void print_dfa_node(DfaNode *node);
bool dfa_valida_cadeia(DfaNode *estado_inicial, char *cadeia);

// funcoes NFA
ll_node *nfa_node_get(NfaNode *node, char key);
void nfa_add_delta(NfaNode *node, char key, NfaNode *value);
void print_nfa_node(NfaNode *node);
void nfa_move(ll_node *estados_atuais, ll_node **novos_estados, char input);
bool nfa_valida_cadeia(NfaNode *estado_inicial, char *cadeia);

#ifdef AUTOMATA_H_TESTE
bool nfa_teste(NfaNode *estado_inicial, char *cadeia);

#endif

#endif
