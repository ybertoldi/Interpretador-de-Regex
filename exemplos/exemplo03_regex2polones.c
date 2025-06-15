#include <stdio.h>
#include <stdlib.h>

#include "../includes/regex2post.h"

int main() {
  char *teste = malloc(100);
  printf("valor: ");
  scanf("%s", teste);
  printf("%s\n", re2post(&teste, 0));

  return 0;
}
