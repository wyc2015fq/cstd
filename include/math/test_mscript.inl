#include "math/mscript.inl"
#include "fileio.h"
int test_mscript()
{
  buf_t bf[1] = {0};
  mscript_engine_t me[1] = {0};
  int i = 0, tok;
  str_t s[1] = {0};
  str_t s1[1] = {0};
  buf_load("test_mscript.txt", bf);
  mscript_init(me, bf->data, bf->len);
  str_setstr2(s, " ");
  for (;;) {
    const char* tokstr;
    tok = mscript_next_token(me, s1);
    str_setstr1(s, *s1);
    tokstr = mscript_token2str(tok);
    printf("%s %s\n", tokstr, s->s);
    if (TOK_EOF == tok) {
      break;
    }
  }
  str_free(s);
  bffree(bf);
  return 0;
}

