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

/* 
 * mapeia todos os caracteres informados na expressao expr 
 * do automato s para o automato t.
 *
 * exemplos de expressoes:
 * "[A-Z]", "[A-Za-z]", "[A-Z]^JKL", "-*+/", "-[0-9]"
 */
void dfa_add_delta_expr(DfaNode *s, const char *expr, DfaNode *t){
  int i, to_addlen, l;
  char *invchars;
  bool az, AZ, nums;

  to_addlen = strchr(expr, '[') ? (expr - strchr(expr, '[')) : strlen(expr);
  i = -1;
  while (++i < to_addlen)
    dfa_add_delta(s, expr[i], t);

  if (!expr[i])
    return;

  az = AZ = nums = false;
  if (expr[i] == '[') {
    i++;
    while (expr[i] != ']') {
      if (strncmp(&expr[i], "a-z", 3) == 0) {
        az = true;
        i += 3;
      } else if (strncmp(&expr[i], "A-Z", 3) == 0) {
        AZ = true;
        i += 3;
      } else if (strncmp(&expr[i], "0-9", 3) == 0) {
        nums = true;
        i += 3;
      } else {
        fprintf(stderr, "cadeia invalida: %s\n", &expr[i]);
        exit(EXIT_FAILURE);
      }
    }
  }
  i++;
  
  invchars = NULL;
  if (expr[i] == '^') {
    i++;
    l = strlen(&expr[i]);
    invchars = malloc(l+1);
    strcpy(invchars, &expr[i]);
  } 

  if (az) {
    for (i = 0; i < 26; i++)
      if (!invchars || !strchr(invchars, i + 'a'))
        dfa_add_delta(s, i + 'a', t);
  }
  if (AZ) {
    for (i = 0; i < 26; i++)
      if (!invchars || !strchr(invchars, i + 'A'))
        dfa_add_delta(s, i + 'A', t);
  }
  if (nums) {
    for (i = 0; i < 10; i++)
      if (!invchars || !strchr(invchars, i + '0'))
        dfa_add_delta(s, i + '0', t);
  }

  free(invchars);
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

