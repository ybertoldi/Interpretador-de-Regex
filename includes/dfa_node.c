#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dfa_node.h"

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

