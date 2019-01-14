// ÄÚ´æ³Ø

//#include "mem_c.h"

#include "types_c.h"
#include "bit_c.h"

typedef int (*alloc_fun_t)(void** pp, size_t n);
struct mem_head_t {
  void* x;
  int (*free_)(void** pp);
  size_t size;
  size_t used;
  size_t id;
};

int defult_free(void** pp)
{
  mem_head_t* h = ((mem_head_t*)(*pp)) - 1;
  free(h);
  *pp = NULL;
  return 0;
}
int defult_alloc(void** pp, size_t n)
{
  mem_head_t* h = *pp ? ((mem_head_t*)(*pp)) - 1 : NULL;
  int n1 = n + sizeof(mem_head_t);
  h = (mem_head_t*)realloc(h, n1);
  *pp = h + 1;
  h->x = NULL;
  h->size = n1;
  h->used = n1;
  h->free_ = defult_free;
  return 0;
}

#define MAPSIZE   (32)
struct mem_block_head_t {
  struct mem_block_head_t* next;
  struct mem_block_head_t* prev;
  int unit_size;
  unsigned char map[MAPSIZE];
};
#include "list.h"

static const size_t unit_size[] = {1 << 4, 1 << 5, 1 << 6, 1 << 7, 1 << 8, 1 << 9, 1 << 10};

struct mempool_t {
  mem_block_head_t* blks[countof(unit_size)];
  size_t size_of_unit;
};
size_t nextpow2(size_t v)
{
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}

#define mfree(pp)   (*(pp)) ? (((mem_head_t*)(*(pp)))-1)->free_(pp) : 0
mempool_t mp[1] = {0};
int mempool_free(void** pp)
{
  mem_head_t* h = ((mem_head_t*)(*pp)) - 1;
  uchar* ptr = (uchar*)h;
  if (h->x) {
    mem_block_head_t* p = (mem_block_head_t*)h->x;
    uchar* ptr0 = (uchar*)((p) + 1);
    size_t size = h->size + sizeof(mem_head_t);
    size_t off = ptr - ptr0;
    size_t id = off / size;
    mem_block_head_t* b = p;
    ASSERT(id == h->id);
    ASSERT((off & (size - 1)) == 0);
    b->map[id >> 3] &= ~(1 << (id & 7));
    size_t k = 0;
    int  i;
    for (i = 0; i < MAPSIZE; ++i) {
      k |= b->map[i] != 0;
    }
    if (0 == k) {
      int j = -1;
      for (i = 0; i < countof(unit_size); ++i) {
        if (size == unit_size[i]) {
          j = i;
          break;
        }
      }
      ASSERT(j >= 0);
      LIST_DEL(mp->blks[j], b);
      free(b);
    }
  }
  *pp = NULL;
  return 0;
}

mem_head_t* get_block1(mem_block_head_t** p, size_t unit_size)
{
  unsigned char* ptr = NULL;
  mem_head_t* h = NULL;
  size_t num = (MAPSIZE * 8);
  mem_block_head_t* p1 = *p;
  for (; p1; p1 = p1->next) {
    unsigned char* map = p1->map;
    for (int i = 0; i < num; ++i) {
      if ( !((map[i >> 3] >> (i & 7)) & 1) ) {
        map[i >> 3] |= 1 << (i & 7);
        ptr = (unsigned char*)(p1 + 1);
        ptr += i * unit_size;
        h = (mem_head_t*)ptr;
        h->x = p1;
        h->id = i;
        return h;
      }
    }
  }
  return h;
}
mem_head_t* get_block(mem_block_head_t** p, size_t unit_size)
{
  mem_head_t* h = get_block1(p, unit_size);
  if (h == NULL) {
    size_t num = (MAPSIZE * 8);
    mem_block_head_t* p1 = *p;
    unsigned char* ptr = NULL;
    p1 = (mem_block_head_t*)malloc(sizeof(mem_block_head_t) + num * unit_size);
    memset(p1, 0, sizeof(mem_block_head_t));
    LIST_ADDFRONT((*p), p1);
    p1->map[0] = 1;
    p1->unit_size = unit_size;
    ptr = (unsigned char*)(p1 + 1);
    h = (mem_head_t*)ptr;
    h->x = p1;
    h->id = 0;
  }
  return h;
}
int mempool_alloc(void** pp, size_t n)
{
  if (*pp) {
    mem_head_t* h = ((mem_head_t*)(*pp)) - 1;
    if (n < h->size) {
      h->used = n;
      return 0;
    }
  }
  size_t n1 = n + sizeof(mem_head_t);
  for (int i = 0; i < countof(unit_size); ++i) {
    if (n1 < unit_size[i]) {
      mem_head_t* h = get_block(mp->blks + i, unit_size[i]);
      h->size = unit_size[i] - sizeof(mem_head_t);
      h->used = n;
      h->free_ = mempool_free;
      if (*pp) {
        mem_head_t* h0 = ((mem_head_t*)(*pp)) - 1;
        memcpy(h + 1, h0 + 1, MIN(h0->used, h->used));
        mempool_free(pp);
      }
      *pp = h + 1;
      return 0;
    }
  }
  defult_alloc(pp, n);
  return 0;
}


int test_alloc(alloc_fun_t alloc)
{
  enum {NN = 1000};
  void* p[NN] = {0};
  int i;
  for (i = 0; i < NN; ++i) {
    int sz = rand() % 1000;
    sz = 10;
    alloc(p + i, sz);
  }
  for (int k = 2; k < 10; ++k) {
    for (i = 0; i < NN; i += k) {
      mfree(p + i);
    }
    for (i = 0; i < NN; i += k) {
      int sz = rand() % 1000;
      sz = 10;
      alloc(p + i, sz);
    }
  }
  for (i = 0; i < NN; ++i) {
    mfree(p + i);
  }
  return 0;
}

#include "utime.h"

int test_mempool()
{
  if (1) {
    {
      utime_test a;
      test_alloc(defult_alloc);
    }
    {
      utime_test a;
      test_alloc(mempool_alloc);
    }
  }
  return 0;
}