#include "redft.inl"
#include "renft.inl"
int test_redfa()
{
  dfa_t dfa[1] = {0};
  enum {nn = 20, mm = 5};
  uchar* buf = 0;
  int* id = 0;
  int i, j;
  buf = MALLOC(uchar, nn * mm);
  id = MALLOC(int, nn);
  redfa_setsize(dfa, nn * mm);
  if (0) {
    redfa_clear(dfa);
    for (i = 0; i < nn; ++i) {
      randstr(buf + i * mm, mm, MAPALPHA0);
      id[i] = redfa_comp(dfa, buf + i * mm, mm);
    }
    for (i = 0; i < nn; ++i) {
      ASSERT(id[i] == redfa_match(dfa, buf + i * mm, mm, 0, 0));
    }
  }
  if (1) {
    uchar buf2[256];
    for (i = 0; i < nn; ++i) {
      redfa_clear(dfa);
      randstr(buf + i * mm, mm, MAPALPHA0);
      randstr(buf2, 256, MAPALPHA0);
      memcpy(buf2 + (mm + 1) * 2, buf + i * mm, mm);
      for (j = 0; j < 5; ++j) {
        buf2[j * (mm + 1) + mm] = '|';
      }
      buf2[(mm + 1) * 5 - 1] = 0;
      id[i] = redfa_comp(dfa, buf2, (mm + 1) * 5 - 1);
      ASSERT(id[i] == redfa_match(dfa, buf + i * mm, mm, 0, 0));
    }
  }
  redfa_free(dfa);
  FREE(buf);
  FREE(id);
  return 0;
}

