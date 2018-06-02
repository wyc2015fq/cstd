
#include <ctype.h>

int NAME(char *A, char *B){

  char a = *A;
  char b = *B;

  if (a > 96) a -= 32;
  if (b > 96) b -= 32;

  return (a == b);
}
