#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// aa|b -> aa.b|
// a*ba* -> a*b.a*.j`

char *re2post(char **reg, int level) {
  char *buff = malloc(8000), *bp, *temp, *todelete;
  int napp = 0, nalt = 0;
  bool get_out = false;

  bp = buff;
  for (; **reg; (*reg)++) {
    switch (**reg) {
    case '(':
      (*reg)++;
      while (napp > 1) {
        *bp++ = '.';
        napp--;
      }
      temp = re2post(reg, level + 1);
			todelete = temp;
      while (*temp) {
        *bp++ = *temp++;
      }
      napp++;
			free(todelete);
      break;

    case ')':
      if (level > 0)
        get_out = true;
      break;

    case '+':
    case '*':
      *bp++ = **reg;
      break;

    case '|':
      while (napp > 1) {
        *bp++ = '.';
        napp--;
      }
      napp--;
      nalt++;
      break;

    default:
      if (napp > 1) {
        *bp++ = '.';
        napp--;
      }
      *bp++ = **reg;
      napp++;
      break;
    }

    if (get_out) {
      break;
    }
  }

  while (napp-- > 1) {
    *bp++ = '.';
  }
  while (nalt-- > 0) {
    *bp++ = '|';
  }

  *bp++ = '\0';

  return buff;
}

int main() {
  char *teste = malloc(100);
  printf("valor: ");
  scanf("%s", teste);
  printf("%s\n", re2post(&teste, 0));

  return 0;
}
