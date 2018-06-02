//Unicode,GBƒÈÂë,×Ö,»­Êý,±Ê»­Âë,
typedef struct hzbh_t {
  unsigned short gb;
  unsigned short uni;
  char chr[4];
  int bh;
  char aa[60];
} hzbh_t;
static hzbh_t g_hzbh[] = {
#define HZBHDEF(a, b, c, d, e) {0x##a, 0x##b, #c, d, #e},
#include "hzbh.txt"
#undef HZBHDEF
};
hzbh_t* gethzbh(int ch)
{
  int i;
  for (i = 0; i < countof(g_hzbh); ++i) {
    if (ch == g_hzbh[i].gb) {
      return g_hzbh + i;
    }
  }
  return 0;
}

