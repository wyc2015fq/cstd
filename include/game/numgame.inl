#include "cstd.h"
typedef struct numnode_t_ numnode_t;
struct numnode_t_ {
  int x, y;
  numnode_t* next;
  numnode_t* parent;
};
typedef struct {
  int h, w, n;
  int maplen;
  int lv;
  numnode_t** map;
  int* cnt;
} numsovle_t;
int numnode_invcount(const numsovle_t* s, const numnode_t* a1)
{
  int i, j, temp = 0, sum1 = 0;
  uchar* a = (uchar*)(a1 + 1);
  for (i = 0; i < s->n; i++) {
    if (a[i] == 0) {
      int w = i;
      int y = w / s->h;
      int x = w % s->w;
      sum1 += abs(1 + x - s->w) + abs(1 + y - s->h);
    }
    for (j = i + 1; j < s->n; j++) {
      if (a[i] > a[j]) {
        sum1++;
      }
    }
  }
  return sum1;
}
uint numnode_gethash(const numsovle_t* s, const numnode_t* a)
{
  int i;
  uint hash = 0;
  const uchar* data = (const uchar*)(a + 1);
  for (i = 0; i < s->n; ++i) {
    hash *= 11;
    hash += data[i];
  }
  return hash;
}
numnode_t* numnode_find_and_ins(numsovle_t* s, numnode_t* a, int ins)
{
  uint hash = numnode_gethash(s, a);
  int id = hash % s->maplen;
  numnode_t* b = s->map[id];
  for (; b; b = b->next) {
    if (0 == memcmp(a + 1, b + 1, s->n)) {
      return b;
    }
  }
  if (ins) {
    a->next = s->map[id];
    s->map[id] = a;
    s->cnt[id]++;
  }
  return NULL;
}
int numbfs_print(numsovle_t* s, const numnode_t* a)
{
  int i, j;
  for (i = 0; i < s->h; ++i) {
    for (j = 0; j < s->w; ++j) {
      printf("%2d,", ((uchar*)(a + 1))[i * s->w + j]);
    }
    printf("\n");
  }
  printf("\n");
  return 0;
}
static const char dir[4][2] = {{ -1, 0}, {1, 0}, {0, -1}, {0, 1}};
#define bufptr(p, len) ((p += (len)) - (len))
int numnode_move1(int h, int w, numnode_t* a, numnode_t* b, int i)
{
  int x, y, t, n = h * w;
  int sz = sizeof(numnode_t) + n;
  x = a->x + dir[i][0];
  y = a->y + dir[i][1];
  if (0 <= x && x < w && 0 <= y && y < h) {
    uchar* data = (uchar*)(b + 1);
    if (a != b) {
      memcpy(b, a, sz);
    }
    CV_SWAP(data[y * w + x], data[a->y * w + a->x], t);
    b->x = x, b->y = y;
    return 1;
  }
  return 0;
}
int numnode_move(numsovle_t* s, numnode_t** pa, int an, numnode_t** pb, int bn)
{
  int i, x, y, j = 0, k;
  int sz = sizeof(numnode_t) + s->n;
  for (k = 0; k < an && j < bn; ++k) {
    numnode_t* a = pa[k];
    int sa = numnode_invcount(s, a);
    for (i = 0; i < 4 && j < bn; ++i) {
      x = a->x + dir[i][0];
      y = a->y + dir[i][1];
      if (0 <= x && x < s->w && 0 <= y && y < s->h) {
        numnode_t* b = (numnode_t*)malloc(sz);
        numnode_t* c = NULL;
        uchar* data = (uchar*)(b + 1);
        char t;
        memcpy(b, a, sz);
        b->x = x, b->y = y;
        CV_SWAP(data[y * s->w + x], data[a->y * s->w + a->x], t);
        if (!(c = numnode_find_and_ins(s, b, 1))) {
          int sb = numnode_invcount(s, b);
          //numbfs_print(s, b);
          ASSERT(0 == ((sa + sb) & 1));
          pb[j++] = b;
          b->parent = a;
        }
        else {
          FREE(b);
        }
      }
    }
  }
  return j;
}
int numbfs(int h, int w)
{
  numnode_t** pb = NULL;
  int i, j, n = 0, n0 = 0, sz, bn = 100000 * h * w;
  numsovle_t s[1] = {0};
  numnode_t* a = NULL;
  s->h = h, s->w = w, s->n = h * w;
  s->maplen = 10000;
  MYREALLOC(pb, bn);
  MYREALLOC(s->map, s->maplen);
  MYREALLOC(s->cnt, s->maplen);
  MEMSET(s->map, 0, s->maplen);
  MEMSET(s->cnt, 0, s->maplen);
  sz = sizeof(numnode_t) + s->n;
  a = (numnode_t*)malloc(sz);
  for (i = 0; i < s->n; ++i) {
    j = (i + 1) % (s->n);
    ((uchar*)(a + 1))[i] = j;
    if (0 == j) {
      a->y = i / s->w;
      a->x = i % s->w;
    }
  }
  pb[n++] = a;
  numbfs_print(s, a);
  numnode_find_and_ins(s, a, 1);
  for (j = 0; j < 20 && n < bn; ++j) {
    int n1 = n;
    n += numnode_move(s, pb + n0, n - n0, pb + n, bn - n);
    numbfs_print(s, pb[n - 1]);
    if (n1 == n) {
      break;
    }
    n0 = n1;
  }
  if (0) {
    int cnt = 0;
    for (i = 0; i < s->maplen; ++i) {
      cnt += !!s->cnt[i];
    }
    printf("%d\n", cnt);
  }
  FREE(a);
  FREE(pb);
  FREE(s->map);
  return 0;
}
int test_numbfs()
{
  numbfs(4, 4);
  return 0;
}

