#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NFA_NODE_TESTE             // habilita o nfa_teste(), que valida uma cadeia mostrando o passo a passo 
#define NFA_AUTOMATA_TESTE         // habilita os nomes nos estados, para visualizacao

#include "includes/nfa_automata.h" // header com AFNDs e suas operacoes (concat, uniao, etc.)
#include "includes/nfa_node.h"     // header com os nós (estados) dos AFNDs. Funcoes de navegacão, validação, delta...
#include "includes/regex2post.h"   // header para conversão do regex comum à notação postscript (polonesa inversa)

NfAutomata stack[8000];            // pilha de automatos
uint32_t sp = 0;                   // tamanho da pilha

void push(NfAutomata n) { stack[sp++] = n; }
NfAutomata pop() { return stack[--sp]; }

int main() {
  char *cadeia = malloc(100 * sizeof(char));
  printf("\ndigite uma expressao regular: ");
  scanf("%s", cadeia);

  char *cadeia_post = re2post(&cadeia, 0);
  printf("notacao polonesa inversa: %s\n", cadeia_post);

  NfAutomata n1, n2;
  for (; *cadeia_post; cadeia_post++) {
    switch (*cadeia_post) {

    case '+':
      n1 = pop();
      n2 = automata_copy(n1);
      push(concat(n2, estrela(n1)));
      break;

    case '*':
      n1 = pop();
      push(estrela(n1));
      break;

    case '|':
      n2 = pop();
      n1 = pop();
      push(uniao(n1, n2));
      break;

    case '.':
      n2 = pop();
      n1 = pop();
      push(concat(n1, n2));
      break;

    default:                                            //qualquer caracter comum
      push(novo_automato(prox_nome(), *cadeia_post));
      break;
    }
  }

  NfAutomata automato = pop(); // ao final da leitura, o unico automato que sobra na pilha eh o correto para a regex

  while (true) {
    printf("\ndigite uma cadeia na forma na forma da expressao regular (q para "
           "sair): ");
    scanf("%s", cadeia);
    if (strcmp(cadeia, "q") == 0) {
      break;
    }
    if (nfa_teste(automato.inicio, cadeia))
      printf("\nEstado Aceito!\n");
    else
      printf("\nEstado Invalido\n");
  }
}
