
#include "mem.h"

typedef struct chain_t {
  POINT pt;
  int len;
  char* code;
} chain_t;

typedef struct chains_t {
  chain_t* ch;
  int nch;
} chains_t;

// 1
//2 0
// 3
static const POINT code_deltas4[4] = { {1, 0}, {0, -1}, { -1, 0}, {0, 1} };
//321
//4 0
//567
static const POINT code_deltas8[8] = { {1, 0}, {1, -1}, {0, -1}, { -1, -1}, { -1, 0}, { -1, 1}, {0, 1}, {1, 1} };

int chain4_ext(const chain_t* ch1, chain_t* ch2)
{
  const char* aa[4][4] = {
    "0", "01", "012", "",
    "", "1", "12", "123",
    "230", "", "2", "23",
    "30", "301", "", "3",
  };
  int i, j, ci, cj, n = ch1->len;
  const char* c4 = ch1->code;
  SAFEFREE(ch2->code);
  ch2->code = (char*)pmalloc(ch1->len * 2 + 8);
  ch2->code[0] = 0;
  ch2->pt = ch1->pt;

  if (0 == n) {
    ch2->code[0] = 1;
    ch2->code[1] = 2;
    ch2->code[2] = 3;
    ch2->code[3] = 0;
    ch2->len = 4;
    return 0;
  }

  for (i = 0; i < n; ++i) {
    j = (i + n - 1) % n;
    ci = c4[i], cj = c4[j];
    strcat(ch2->code, aa[cj][ci]);
  }

  printf("%s\n", ch2->code);
  ch2->len = strlen(ch2->code);

  for (i = 0; i < ch2->len; ++i) {
    ch2->code[i] -= '0';
  }

  return 0;
}
int chains4_ext(const chains_t* chs1, chains_t* chs2)
{
  int i;
  SAFEFREE(chs2->ch);
  chs2->ch = (chain_t*)pmalloc(sizeof(chain_t) * chs1->nch);
  memset(chs2->ch, 0, sizeof(chain_t) * chs1->nch);

  for (i = 0; i < chs1->nch; ++i) {
    chain4_ext(chs1->ch + i, chs2->ch + i);
  }

  chs2->nch = chs1->nch;
  return 0;
}
// 4方向链码面积
int chain4_area(int n, const char* c4)
{
  int i, j, ci, area = 0, y = 0;
  POINT pp;

  for (i = 0; i < n; ++i) {
    j = (i + 1) % n;
    ci = c4[i];
    pp = code_deltas4[ci];
    area += pp.x * (y);
    y += pp.y;
  }

  return area;
}
int chain4_area2(int n, const char* c4)
{
  int aa[4][4] = {
    0, 0, 1, 0,
    0, 0, 0, 1,
    1, 0, 0, 0,
    0, 1, 0, 0,
  };
  int i, j, ci, cj, area = 0, y = 0;
  POINT pp;

  for (i = 0; i < n; ++i) {
    j = (i + 1) % n;
    ci = c4[i], cj = c4[j];
    pp = code_deltas4[ci];
    area += pp.x * (y + (ci == 0)) + aa[ci][cj];
    y += pp.y;
  }

  return area;
}
int chain_to_polygon(chain_t* ch, POINT* pp)
{
  int i, n = 0;

  for (i = 0; i < ch->len; ++i) {
  }

  return 0;
}

int trace_contour1(char* ch, unsigned char* ptr, int* deltas, int nd, int nbd)
{
  int dir = 0, i, j, d, n = 0, nhd = nd / 2, ndmark = nd - 1;
  unsigned char* stop_ptr = ptr;

  for (;;) {
    for (i = 0; i < nd; ++i) {
      j = (i + dir + nhd + 1) & ndmark;
      d = deltas[j];

      if (ptr[d]) {
        printf("%d", j);
        ch[n] = j;
        ptr[0] = -2; //破坏边界
        dir = j;
        ptr += d;
        ++n;
        break;
      }
    }

    if (ptr == stop_ptr) {
      printf("\n");
      return n;
    }
  }

  return 0;
}

int trace_contour(char* ch, char* ptr, const int* deltas0, int nd)
{
  int nbd = 2;
  int dir = 0, i, n = 0, nhd = nd / 2, ndmark = nd - 1;
  int deltas[16];
  int s, s_end;
  char* i0, *i1, *i3, *i4 = 0;
  s_end = s = ptr[0] ? nhd : 0;

  if (0 == ptr[0]) {
    int adsf = 0;
  }

  ptr -= 0 == ptr[0];
  i0 = ptr;

  for (i = 0; i < 2 * nd; ++i) {
    deltas[i] = deltas0[i & ndmark];
  }

  do {
    s = (s - 1) & ndmark;
    i1 = i0 + deltas[s];

    if (*i1 != 0) {
      break;
    }
  }
  while (s != s_end);

  if (s == s_end) {
    // single pixel domain
    *i0 = (char)(nbd | -128);
    return 0;
  }

  i3 = i0;

  for (;;) {
    s_end = s;

    for (;;) {
      i4 = i3 + deltas[++s];

      if (*i4 != 0) {
        break;
      }
    }

    s &= ndmark;

    // check "right" bound
    if ((unsigned)(s - 1) < (unsigned) s_end) {
      *i3 = (char)(nbd | -128);
    }
    else if (*i3 == 1) {
      *i3 = nbd;
    }

    printf("%d", s);
    ch[n++] = s;

    if (i4 == i0 && i3 == i1) {
      break;
    }

    i3 = i4;
    s = (s + nhd) & ndmark;
  }

  printf("\n");

  return n;
}

int freeman(int h, int w, const unsigned char* A, int al, chains_t* chs, int nd)
{
  int i, j, h2 = h + 2, w2 = w + 2;
  int deltas[8] = {0};
  char* buf;
  const char* A0;
  char* B;
  char* B0;
  const POINT* code_deltas = 8 == nd ? code_deltas8 : code_deltas4;
  B = (char*)pmalloc(h2 * w2);
  buf = (char*)pmalloc(h2 * w2);
  memset(B, 0, w2);
  memset(B + (h2 - 1)*w2, 0, w2);

  for (i = 0; i < nd; ++i) {
    deltas[i] = code_deltas[i].y * w2 + code_deltas[i].x;
  }

  for (i = 0; i < h; ++i) {
    A0 = (const char*)A + i * al;
    B0 = B + (i + 1) * w2 + 1;
    B0[-1] = B0[w] = 0;

    for (j = 0; j < w; ++j) {
      B0[j] = A0[j] > 0;
    }
  }

  for (i = 0; i < h; ++i) {
    B0 = B + (i + 1) * w2 + 1;

    for (j = 0; j < w; ++j) {
      // if not external contour
      if (B0[j] == B0[j - 1]) {
        continue;
      }

      if (!(B0[j - 1] == 0 && B0[j] == 1)) {
        /* check hole */
        if (B0[j] != 0 || B0[j - 1] < 1) {
          continue;
        }

        //is_hole = 1;
      }

      if (1) {
        chain_t* pch;
        int n = trace_contour(buf, B0 + j, deltas, nd);
        chs->ch = (chain_t*)my_memins(chs->ch, chs->nch, chs->nch, 1, sizeof(chain_t));
        pch = chs->ch + chs->nch;
        chs->nch++;
        pch->code = (char*)memdup(buf, n);
        pch->len = n, pch->pt.x = j - !B0[j], pch->pt.y = i;
      }
    }
  }

  pfree(buf);
  pfree(B);
  return 0;
}

#define POINTCOMPARE(p1, p2)  (p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x))

// 转换为线段表
// 1. ptStart      起始点
// 2. ChainCode    链码表
// 3. pLineTable   线段表
int ToLineTable(chain_t* ch, POINT* pLineTable)
{
  int i, j, n = 0;
  POINT ptCurrent = { 0, 0};
  POINT* ptr = pLineTable;
  int nTable[8][8] = {
    { 0, 2, 2, 2, 2, 0, 0, 0 },
    { 0, 2, 2, 2, 2, 3, 0, 0 },
    { 0, 2, 2, 2, 2, 3, 3, 0 },
    { 0, 2, 2, 2, 2, 3, 3, 3 },
    { 1, 0, 0, 0, 0, 1, 1, 1 },
    { 1, 3, 0, 0, 0, 1, 1, 1 },
    { 1, 3, 3, 0, 0, 1, 1, 1 },
    { 1, 3, 3, 3, 0, 1, 1, 1 }
  };
  // 清空线段表
  ptCurrent = ch->pt;

  if (0 == ch->len) {
    // 独立点
    pLineTable[n++] = ptCurrent;
    pLineTable[n++] = ptCurrent;
    return n;
  }
  else {
    // 查表法转换
    for (i = 0; i < ch->len; ++i) {
      int ii, jj;
      j = (i == ch->len - 1) ? 0 : i + 1;
      ii = ch->code[i];
      jj = ch->code[j];
      ptCurrent.x += code_deltas8[ii].x;
      ptCurrent.y += code_deltas8[ii].y;

      switch (nTable[ii][jj]) {
      case 0:
        break;

      case 1:
      case 2:
        pLineTable[n++] = ptCurrent;
        break;

      case 3:
        pLineTable[n++] = ptCurrent;
        pLineTable[n++] = ptCurrent;
        break;
      }
    }

    // 排序
    QSORT(pLineTable, pLineTable + n, POINTCOMPARE, POINT);
  }

  return n;
}
// 轮廓矩形边界
// 1. LineTable 线段表
bool ContourRect(int n, const POINT* LineTable, RECT* pRect)
{
  int i;

  if (0 == n) {
    pRect->left = 0;
    pRect->top = 0;
    pRect->right = 0;
    pRect->bottom = 0;
  }
  else {
    pRect->left = LineTable[0].x;
    pRect->top = LineTable[0].y;
    pRect->right = LineTable[0].x;
    pRect->bottom = LineTable[0].y;

    for (i = 0; i != n; i += 2) {
      if (pRect->left > LineTable[i].x) {
        pRect->left = LineTable[i].x;
      }

      if (pRect->top > LineTable[i].y) {
        pRect->top = LineTable[i].y;
      }

      if (pRect->right < LineTable[i + 1].x) {
        pRect->right = LineTable[i + 1].x;
      }

      if (pRect->bottom < LineTable[i].y) {
        pRect->bottom = LineTable[i].y;
      }
    }
  }

  return true;
}
// 轮廓重心
// 1. LineTable 线段表
POINT ContourBarycenter(int n, const POINT* LineTable)
{
  long lLength = 0;
  long lArea = 0;
  int i;
  POINT pBarycenter;
  pBarycenter.x = 0;
  pBarycenter.y = 0;

  for (i = 0; i != n; i += 2) {
    lLength = (LineTable[i + 1].x - LineTable[i].x + 1);
    pBarycenter.x += (lLength * (LineTable[i + 1].x + LineTable[i].x) / 2);
    pBarycenter.y += (lLength * LineTable[i].y);
    lArea += lLength;
  }

  pBarycenter.x /= lArea;
  pBarycenter.y /= lArea;
  return pBarycenter;
}
// 轮廓长度
// 1. ChainCode 链码表
double ContourLength(int n, const char* ChainCode)
{
  double dLength = 0;
  int i, nOddCode = 0;

  for (i = 0; i != n; i++) {
    if (ChainCode[i] & 0x01) {
      nOddCode++;
    }
  }

  dLength = n - nOddCode + nOddCode * sqrt(2.0);
  return dLength;
}
// 基于链码轮廓的面积计算
double ContourArea(int n, const char* cc)
{
  double lArea = 0;
  int i, y = 0;

  for (i = 0; i < n; +i) {
    POINT dp = code_deltas8[cc[i]];
    lArea += dp.x * (y + dp.y / 2.);
    y += dp.y;
  }

  return 0;
}
// 轮廓面积
// 1. LineTable 线段表
long LineTableContourArea(int n, const POINT* LineTable)
{
  long lArea = 0;
  int i;

  for (i = 0; i != n; i += 2) {
    lArea += (LineTable[(i + 1)].x - LineTable[i].x + 1);
  }

  return lArea;
}

int chain_free(chain_t* cc)
{
  pfree(cc->code);
  memset(cc, 0, sizeof(chain_t));
  return 0;
}
int free_chains(chains_t* chs)
{
  int i;

  for (i = 0; i < chs->nch; ++i) {
    pfree(chs->ch[i].code);
  }

  pfree(chs->ch);
  memset(chs, 0, sizeof(chains_t));
  return 0;
}
