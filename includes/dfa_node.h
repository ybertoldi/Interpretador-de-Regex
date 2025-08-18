#include <stdbool.h>
#include <stdint.h>

#ifndef DFAUTOMATA_H

#define DFAUTOMATA_H
#define MAX_MAP_SIZE 256


#define BUILD_DFA_NODE(s_name)                                                 \
  {.name = s_name, .map = init_dfa_map(), .is_final = false}

#define BUILD_DFA_NODE_FINAL(s_name)                                           \
  {.name = s_name, .map = init_dfa_map(), .is_final = true}



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


DfaMap init_dfa_map(void);
DfaNode *dfa_map_get(DfaMap *map, char key);


DfaNode *dfalloc(char *name, bool isfinal);
void dfa_add_delta(DfaNode *src, char key, DfaNode *target);
void dfa_add_delta_expr(DfaNode *src, const char *expr, DfaNode *target);

bool dfa_node_move(DfaNode **node, char input);

void print_dfa_node(DfaNode *node);
bool dfa_valida_cadeia(DfaNode *estado_inicial, char *cadeia);

#endif
