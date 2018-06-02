#include "arith.inl"
#include <string.h>
#include <assert.h>
int test_arith()
{
  const char* str = "afjaelimvasid fqelkfm asdrfadf adsfa e4ad adfasdf jf kty79p5u kyjgsdfgqwerf zdfv";
  char dst[100];
  int len = strlen(str), l2;
  encode(str, len);
  printf("%d\n", code_index);
  l2 = decode(dst, 100);
  dst[l2] = 0;
  assert(len == l2 && 0 == memcmp(str, dst, len));
  system("pause");
  return 0;
}

