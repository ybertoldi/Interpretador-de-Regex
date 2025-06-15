#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

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

    case '|':
      while (napp > 1) {
        *bp++ = '.';
        napp--;
      }
      napp--;
      nalt++;
      break;

    case '+':
    case '*':
      *bp++ = **reg;
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

    if (get_out) 
      break;
  }

  while (napp-- > 1) 
    *bp++ = '.';
  while (nalt-- > 0) 
    *bp++ = '|';

  *bp++ = '\0';
  return buff;
}

