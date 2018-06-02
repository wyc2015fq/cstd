#ifndef _CMEM_H_
#define _CMEM_H_

#define CC_ALIGN(n, x)   (((n)+((x)-1))&(~((x)-1)))
CC_INLINE int alignSize1(int n, int x) {
  return CC_ALIGN(n, x);
}
CC_INLINE size_t alignSize(size_t sz, int n) {
  return (sz + n-1) & -n;
}
#define _Kb  (1<<10)
#define _Mb  (1<<20)
//int malloc_count = 0;
//int realloc_count = 0;
#define sys_free(p)  sys_free_(p, __FILE__, __LINE__)
#define sys_malloc(newn)  sys_malloc_(newn, __FILE__, __LINE__)
#define sys_realloc(p, newn)  sys_realloc_(p, newn, __FILE__, __LINE__)
CC_INLINE void sys_free_(void* p, const char* file, int line) {
  UNUSED(file);
  UNUSED(line);
  free(p);
}
CC_INLINE void* sys_malloc_(int newn, const char* file, int line) {
  void* p = malloc(newn);
  //++malloc_count;
  UNUSED(file);
  UNUSED(line);
  //printf(" %s %d sys_malloc_err\n", file, line);
  if (NULL==p) {
    printf(" %s %d sys_malloc_err", file, line);
  }
  return p;
}
static int sys_msize(void* p);

CC_INLINE void* sys_realloc_(void* p, int newn, const char* file, int line) {
  //++realloc_count;
  //printf("%s:%d\n", file, line);
  //int oldn = sys_msize(p);
  UNUSED(file);
  UNUSED(line);
  p = realloc(p, newn);
  //memset((char*)p+oldn, 0, newn-oldn);
  return p;
}

typedef struct my_mem_stage_t {
  unsigned int magic0;
  unsigned int n;
  int id;
  int line;
  int add_id;
  const char* file;
  struct my_mem_stage_t* next;
  struct my_mem_stage_t* prev;
  unsigned int magic1;
} my_mem_stage_t;
static int g_mem_check_id = 0;
CC_INLINE my_mem_stage_t* my_mem_stage_opt(my_mem_stage_t* p0_add, my_mem_stage_t* p0_remove)
{
  static my_mem_stage_t* g_mem_stage = NULL;
  static int add_id = 0;
  my_mem_stage_t* p0 = NULL;
  if (p0_add) {
    p0_add->add_id = add_id++;
    p0 = p0_add;
    if (g_mem_stage) {
      g_mem_stage->prev = p0;
      p0->next = g_mem_stage;
    }
    else {
      p0->next = NULL;
    }
    p0->prev = NULL;
    g_mem_stage = p0;
  }
  if (p0_remove) {
    p0 = p0_remove;
    if (p0->prev) {
      p0->prev->next = p0->next;
    }
    else {
      g_mem_stage = p0->next;
    }
    if (p0->next) {
      p0->next->prev = p0->prev;
    }
  }
  return g_mem_stage;
}
CC_INLINE int my_msize(void* p)
{
  int n = 0;
  if (p) {
    my_mem_stage_t* p0 = ((my_mem_stage_t*)p)-1;
    n = p0->n;
  }
  return n;
}
typedef struct {
  void (*_free)(void* p, const char* file, int line);
  int (*_msize)(void* p);
  void* (*_realloc)(void* p, int n, const char* file, int line);
  void* (*_malloc)(int n, const char* file, int line);
} mem_funs_t;

#define MEM_MEMFUN_SET(p, name)  ((p)->_free = name##_free, (p)->_msize = name##_msize, (p)->_realloc = name##_realloc, (p)->_malloc = name##_malloc)
#define MEM_INIT(arr)  mem_init(arr, sizeof(arr), __FILE__, __LINE__)

#if 1
static mem_funs_t* _memfun = NULL;
#else
static mem_funs_t** get_memfun() {
  static mem_funs_t* _memfun = NULL;
  return &_memfun;
}
#endif
#define MALLOC_ALIGN  16
#if 0
#define free(ptr) (my_free(ptr, __FILE__, __LINE__))
//#define free(ptr) ((ptr) ? (my_free((void*)(ptr)), ptr=0) : 0)
#define malloc(n) my_malloc(n, __FILE__, __LINE__)
#define calloc(x, n) my_malloc((x)*(n), __FILE__, __LINE__)
#define realloc(p, n) my_realloc(p, n, __FILE__, __LINE__)
#endif
#if 0
#define free(ptr)   (?)
//#define free(ptr) ((ptr) ? (my_free((void*)(ptr)), ptr=0) : 0)
#define malloc(n)    (?)
#define calloc(x, n)   (?)
#define realloc(p, n) (?)
#endif

#if 0
#define pfree(p)       ((_memfun) ? _memfun->_free(p, __FILE__, __LINE__) : sys_free_(p, __FILE__, __LINE__), p = NULL)
#define pmalloc(n)     ((_memfun) ? _memfun->_malloc(n, __FILE__, __LINE__) : sys_malloc_(n, __FILE__, __LINE__))
#define prealloc(p, n) ((_memfun) ? _memfun->_realloc(p, n, __FILE__, __LINE__) : sys_realloc_(p, n, __FILE__, __LINE__))
#else
#define pfree(p)       pfree_(p, __FILE__, __LINE__)
#define pmalloc(n)     pmalloc_(n, __FILE__, __LINE__)
#define prealloc(p, n) prealloc_(p, (int)(n), __FILE__, __LINE__)
CC_INLINE void pfree_(void* p, const char* file, int line) {
  (_memfun) ? _memfun->_free(p, file, line) : sys_free_(p, file, line);
}
CC_INLINE void* pmalloc_(int n, const char* file, int line) {
  return ((_memfun) ? _memfun->_malloc(n, file, line) : sys_malloc_(n, file, line));
}
CC_INLINE void* prealloc_(void* p, int n, const char* file, int line) {
  return ((_memfun) ? _memfun->_realloc(p, n, file, line) : sys_realloc_(p, n, file, line));
}
#endif
#define pcalloc(p, n) pcalloc_(p, n, __FILE__, __LINE__)
CC_INLINE void* pcalloc_(int n, const char* file, int line) {
  void* p = pmalloc_(n, file, line);
  if (p) {
    memset(p, 0, n);
  }
  return p;
}

#define memclone(pp, p, n)  memclone_(pp, p, n, __FILE__, __LINE__)
#define MEMCLONE(pp, p, n)  memclone_(pp, p, (n)*sizeof(*p), __FILE__, __LINE__)
CC_INLINE int memclone_(void* pp, const void* p, int n, const char* file, int line) {
  void** pp1 = (void**)pp;
  *pp1 = prealloc_(*pp1, n, file, line);
  memcpy(*pp1, p, n);
  return 0;
}
#define MALLOC(_TYPE, _N)             (_TYPE*)pmalloc_((unsigned int)(sizeof(_TYPE)*(_N)), __FILE__, __LINE__)
#define CALLOC(_TYPE, _N)             (_TYPE*)pcalloc_((unsigned int)(sizeof(_TYPE)*(_N)), __FILE__, __LINE__)
#define REALLOC(_TYPE, _PTR, _N)      _PTR=(_TYPE*)prealloc_(_PTR, (unsigned int)(sizeof(_TYPE)*(_N)), __FILE__, __LINE__)
#define REALLOC_(_TYPE, _PTR, _N)      _PTR=(_TYPE*)prealloc_(_PTR, (unsigned int)(sizeof(_TYPE)*(_N)), file, line)
#define MYREALLOC(_PTR, _N)           pprealloc((&(_PTR)), sizeof(*_PTR)*(_N), __FILE__, __LINE__)
#define FREE(ptr)              ((ptr) ? (pfree((void*)(ptr)), ptr=0) : 0), (ptr)=0
#define NFREE(ptr, _N)         FREE(ptr)
//#define GSAFEFREE(_PTR)         if (NULL!=_PTR) { gfree(_PTR); _PTR = NULL; }
//#define SAFEFREE(_PTR)         if (NULL!=_PTR) { pfree(_PTR); _PTR = NULL; }

CC_INLINE void pprealloc(void* ptr, int n, const char* file, int line) {
  void** pptr = (void**)ptr;
  *pptr=prealloc_(*pptr, n, file, line);
}

#define myReAlloc(ptr, sz) realloc_push(&ptr, 0, sz, sizeof(*ptr), 0, __FILE__, __LINE__)
#define myMemFree(ptr) ((ptr) ? (pfree(ptr), ptr = NULL) : 0)
#define myMemAlloc(sz) pmalloc(sz)
#define myAlloc(ptr, oldsz, newsz) realloc_push(&ptr, &oldsz, newsz, sizeof(*ptr), 0, __FILE__, __LINE__)
#define myAllocPush(ptr, oldsz, addsz) realloc_push(&ptr, &oldsz, oldsz+addsz, sizeof(*ptr), oldsz, __FILE__, __LINE__)
#define myFree(ptr, oldsz) {myMemFree(ptr); oldsz = 0; }

#define VEC_FREES(_P, _N, OBJ_FREE) { int i; for (i=0;i<_N; ++i) {OBJ_FREE((_P+i));} }
#define VEC_SETSIZE(_P, _N, _NEWN, OBJ_FREE) { \
  if (_NEWN<_N) { \
    int i; \
    for (i = _NEWN; i < _N; ++i) { \
      OBJ_FREE(_P + i); \
    } \
  } else if (_NEWN>_N) { \
    MYREALLOC(_P, _NEWN); \
    memset(_P+_N, 0, (_NEWN-_N)*sizeof(*_P)); \
  } \
  _N = _NEWN; \
}

#define VEC_FUN_DEF(name, freefun, setsizefun, OBJ_FREE) \
CC_INLINE int freefun(name* iv) {  VEC_FREES(iv->v, iv->n, OBJ_FREE); FREE(iv->v); iv->v=NULL, iv->n=0; return 0;} \
CC_INLINE int setsizefun(name* iv, int n) {  VEC_SETSIZE(iv->v, iv->n, n, OBJ_FREE);  return 0; }

#define VEC_DEF(name, type, freefun, setsizefun, OBJ_FREE)  typedef struct name { type* v; int n; } name; VEC_FUN_DEF(name, freefun, setsizefun, OBJ_FREE)

#define VEC_DEF_EASY(type, OBJ_FREE)  VEC_DEF(v##type, type, v##type##_free, v##type##_setsize, OBJ_FREE)

VEC_DEF_EASY(s32, CC_NOP)
VEC_DEF_EASY(f64, CC_NOP)
VEC_DEF_EASY(f32, CC_NOP)
//VEC_DEF(vf64_t, double, vf64_free, vf64_setsize, CC_NOP);
//VEC_DEF(vf32_t, float, vf32_free, vf32_setsize, CC_NOP);
//VEC_DEF(vs32_t, int, vs32_free, vs32_setsize, CC_NOP);
typedef vs32 vint;
typedef vf32 vfloat;
typedef vf64 vdouble;

CC_INLINE void my_free(void* p, const char* file, int line)
{
  if (p) {
    my_mem_stage_t* p0 = ((my_mem_stage_t*)p)-1;
    //my_mem_stage_t* p1 = ;
    ASSERT(p0->magic0 == 0x11223344 && p0->magic1 == 0x55667788 && "头部越界");
    ASSERT(((my_mem_stage_t*)((char*)p + p0->n))->magic0 == 0x11223344 && ((my_mem_stage_t*)((char*)p + p0->n))->magic1 == 0x55667788 && "尾部越界");
    my_mem_stage_opt(0, p0);
    sys_free_(p0, file, line);
  }
}
CC_INLINE void* my_malloc(int newn, const char* file, int line)
{
  int n = newn<(1<<22) ? (int)next_power(newn) : newn;
  int sz = n + 2 * sizeof(my_mem_stage_t);
  my_mem_stage_t* p0;
  my_mem_stage_t* p1;
  char* p = NULL;
  p = (char*)sys_malloc_(sz, (char*)file, line);
  p0 = (my_mem_stage_t*)p;
  p1 = (my_mem_stage_t*)(p + sz - sizeof(my_mem_stage_t));
  memset(p, 0, sz);
  p1->n = p0->n = n;
  p1->file = p0->file = (const char*)file;
  p1->line = p0->line = line;
  p1->magic0 = p0->magic0 = 0x11223344;
  p1->magic1 = p0->magic1 = 0x55667788;
  p1->id = p0->id = g_mem_check_id;
  if (1470==g_mem_check_id) {
    int asdf=0;
  }
  ++g_mem_check_id;
  my_mem_stage_opt(p0, 0);
  return p0 + 1;
}
CC_INLINE void* my_realloc(void* p, int newn, const char* file, int line)
{
  char* newp = NULL;
  int n = p ? my_msize(p) : 0;
  if (newn > n) {
    newn = MAX(newn, 16);
    while(newn<n) {
      newn = newn*3/2;
    }
    newp = (char*)my_malloc(newn, file, line);
    if (p) {
      memcpy(newp, p, n);
      my_free(p, file, line);
    }
    p = newp;
  }
  return p;
}
CC_INLINE void* my_calloc(int n, const char* file, int line) {
  void* p = my_malloc(n, file, line);
  if (p) {
    memset(p, 0, n);
  }
  return p;
}
static mem_funs_t** my_memfun() {
  static mem_funs_t _my_memfun[1] = {0};
  MEM_MEMFUN_SET(_my_memfun, my);
  _memfun = _my_memfun;
  return &_memfun;
}
/*
static void test_debug_new() {
char* p = malloc(2);
MEM_CHECK_BEGIN();
p = malloc(2);
p[0] = 'A';
p[1] = 'B';
MEM_CHECK_END();
free(p);
}
MEM_CHECK_BEGIN();
MEM_CHECK_END();
*/
#define MEM_CHECK_BEGIN  { mem_check_begin(); {
#define MEM_CHECK_END    } mem_check_end(); }
CC_INLINE int mem_check_begin()
{
  g_mem_check_id = 1;
  my_memfun();
  return 0;
}
CC_INLINE int mem_check_end()
{
  my_mem_stage_t* p = my_mem_stage_opt(0, 0);
  int n=0, i;
  // 输出内存泄漏报告，显示在 Output 窗口的 Debug 页中
  for (; p; p = p->next, ++n) {
    if (p->id) {
      char* p0 = (char*)(p+1);
      int m = MIN(p->n, 20);
      printf("%s(%d) : (size=%d, id=%d) ", p->file, p->line, p->n, p->id);
      for (i=0; i < m; ++i) {
        printf("%02x ", p0[i]);
      }
      printf("\n");
    }
  }
  g_mem_check_id = 0;
  return 0;
}
CC_INLINE void* realloc_push(void* ptr, int* poldsz, int newsz, int elemsz, int retoff, const char* file, int line)
{
  int oldsz = poldsz ? *poldsz : 0;
  void** pptr = (void**)ptr;
  if (newsz > oldsz) {
    *pptr = prealloc_(*pptr, newsz * elemsz, file, line);
    memset((char*)(*pptr) + oldsz * elemsz, 0, (newsz - oldsz)*elemsz);
  }
  if (poldsz) {
    *poldsz = newsz;
  }
  return (char*)(*pptr) + (retoff * elemsz);
}
#define MULTIALLOC2(buf, p1, n1, p2, n2)   multialloc(&buf, &p1, (n1)*sizeof(*p1), &p2, (n2)*sizeof(*p2), NULL)
#define MULTIALLOC3(buf, p1, n1, p2, n2, p3, n3)   multialloc(&buf, &p1, (n1)*sizeof(*p1), &p2, (n2)*sizeof(*p2), &p3, (n3)*sizeof(*p3), NULL)
#define MULTIALLOC4(buf, p1, n1, p2, n2, p3, n3, p4, n4)   multialloc(&buf, &p1, (n1)*sizeof(*p1), &p2, (n2)*sizeof(*p2), &p3, (n3)*sizeof(*p3), &p4, (n4)*sizeof(*p4), NULL)
#define MULTIALLOC5(buf, p1, n1, p2, n2, p3, n3, p4, n4, p5, n5)   multialloc(&buf, &p1, (n1)*sizeof(*p1), &p2, (n2)*sizeof(*p2), &p3, (n3)*sizeof(*p3), &p4, (n4)*sizeof(*p4), &p5, (n5)*sizeof(*p5), NULL)
CC_INLINE void* multialloc(void* p, ...) {
  int n = 0, n1=0;
  void** pp = (void**)p;
  char* pc = (char*)*pp;
  void** pp1;
  va_list arglist;
  {
    va_start(arglist, p);
    for (;(pp1 = (void**)va_arg(arglist, char*))!=NULL;) {
      n1 = va_arg(arglist, int);
      n += n1;
    }
    va_end(arglist);
  }
  MYREALLOC(pc, n);
  *pp = pc;
  {
    va_start(arglist, p);
    for (;(pp1 = va_arg(arglist, void**))!=NULL;) {
      n1 = va_arg(arglist, int);
      *pp1 = pc;
      pc += n1;
    }
    va_end(arglist);
  }
  return *pp;
}

#define BF_MEM_MAGIC  (0x55667788)
#define ALIGN_TO(_X, ALIGN) ( ((size_t)(_X) + ALIGN - 1) & ~(size_t)(ALIGN - 1) )
typedef struct bf_mem_tail_t {
  uint magic;
  //int line;
  //char file[256];
} bf_mem_tail_t;
typedef struct bf_mem_head_t {
  int id;
  int size;
  int used;
  const char* file;
  int line;
  struct bf_mem_head_t* next, *prev;
  bf_mem_tail_t* tail;
  uint magic;
} bf_mem_head_t;

#define BF_MEM_INFO_LEN    (int)(sizeof(bf_mem_head_t)+sizeof(bf_mem_tail_t))
#define BFMALLOC(_BF, _P, _N)  bf_malloc(_BF, (void**)&(_P), (_N)*sizeof(*(_P)))
#define BFFREE(_BF, _P)    bf_free(_BF, (void**)&(_P))
#define BF_MALLOC(_P, _N)  BFMALLOC(bf, _P, _N)
#define BF_CALLOC(_P, _N)  BFMALLOC(bf, _P, _N); if (_P) MEMZERO(_P, _N)
#define BF_FREE(_P)    BFFREE(bf, _P)
#define SYSMALLOC(_T, _P, _N)  (_P = (_T*)pmalloc((_N)*sizeof(*(_P))))
#define SYSFREE(_T, _P, _N)    (pfree(_P), _P=NULL)
//static int bf_maxlen=0;
//static int bf_malloc_maxlen=0;
CC_INLINE int bf_malloc(buf_t* bf, void* p, int n)
{
  uchar* p0 = 0;
  bf_mem_head_t* head;
  bf_mem_tail_t* tail;
  int len = (n + BF_MEM_INFO_LEN);
  if (len > bf->len) {
    //ASSERT(len <= bf->len && "空间不够");
    printf("bf_malloc error : Memory space is not enough!!\n");
    *(void**)p = NULL;
  } else {
    //bf_maxlen = MAX(bf_maxlen, bf->len);
    //bf_malloc_maxlen = MAX(bf_maxlen, bf->len-len);
    bf->len -= len;
    p0 = bf->data + bf->len;
    head = (bf_mem_head_t*)p0;
    tail = (bf_mem_tail_t*)(p0 + len - sizeof(bf_mem_tail_t));
    head->magic = BF_MEM_MAGIC, head->size = n;
    tail->magic = BF_MEM_MAGIC;
    *(void**)p = p0 + sizeof(bf_mem_head_t);
  }
  return 0;
}
CC_INLINE int bf_free(buf_t* bf, void* p)
{
  void** pp = (void**)p;
  uchar* p0 = (uchar*)(*pp);
  if (p0) {
    bf_mem_head_t* head = (bf_mem_head_t*)(p0 - sizeof(bf_mem_head_t));
    //bf_mem_tail_t* tail = (bf_mem_tail_t*)(p0 + head->size);
    int len = head->size + BF_MEM_INFO_LEN;
    ASSERT((bf->data + bf->len + sizeof(bf_mem_head_t)) == p0 && "释放顺序不对");
    //ASSERT(head->size == n && "??锟斤拷?????");
    ASSERT(head->magic == BF_MEM_MAGIC && "??????");
    ASSERT(((bf_mem_tail_t*)(p0 + head->size))->magic == BF_MEM_MAGIC && "锟斤拷?????");
    bf->len += len;
    *pp = NULL;
  }
  return 0;
}

typedef struct mem_t {
  int len;
  int cnt;
  int used;
  bf_mem_head_t* head;
  char* buf;
} mem_t;
#define GETMEM()  get_mem()
CC_INLINE struct mem_t* get_mem() {
  static struct mem_t gmem[1] = {0, 0, 0, NULL, NULL};
  return gmem;
}

CC_INLINE int memblock_init(void* p, int n, int used, bf_mem_head_t* prev, bf_mem_head_t* next, const char* file, int line) {
  char* buf = (char*)p;
  bf_mem_head_t* head = (bf_mem_head_t*)p;
  bf_mem_tail_t* tail = ((bf_mem_tail_t*)(buf + n))-1;
  //ASSERT(buf>=mem->buf && (buf+n)<=(mem->buf+mem->len));
  //memset(p, 0, n);
  head->size = n - BF_MEM_INFO_LEN;
  tail->magic = head->magic = BF_MEM_MAGIC;
  if (next) {
    next->prev = head;
  }
  if (prev) {
    prev->next = head;
  }
  head->next = next;
  head->prev = prev;
  head->used = used;
  head->file = file;
  head->line = line;
  head->tail = tail;
  return 0;
}

#define MEM_BLOCK_IS_GOOD(_head, n)  (0==(_head)->used && (_head)->size >= n)

CC_INLINE bf_mem_head_t* mem_find_first_free_block(int n) {
  bf_mem_head_t* head = GETMEM()->head;
  for (;head; head = head->next) {
    if (MEM_BLOCK_IS_GOOD(head, n)) {
      return head;
    }
  }
  return NULL;
}
CC_INLINE int mem_get_len(int n) {
  enum {nn_minlen = (1<<10)-1};
  int n2 = (MAX(n, BF_MEM_INFO_LEN)+nn_minlen)&(~nn_minlen);
  int len = (n2);
  return len;
}

CC_INLINE void mem_list() {
  bf_mem_head_t* head = GETMEM()->head;
  if (0) {
    int size = 0, id = 0;
    for (;head; head = head->next) {
      bf_mem_tail_t* tail = (bf_mem_tail_t*)((char*)head + head->size + BF_MEM_INFO_LEN)-1;
      //printf("size = %d used = %d %s %d\n", head->size, head->used, head->file, head->line);
      head->id = id++;
      if (head->magic != BF_MEM_MAGIC ) {
        ASSERT(head->magic == BF_MEM_MAGIC && "head bound err");
      }
      
      if (tail->magic != BF_MEM_MAGIC) {
        ASSERT(tail->magic == BF_MEM_MAGIC && "tail bound err");
      }
      size += head->size + BF_MEM_INFO_LEN;
    }
    ASSERT(size==GETMEM()->len);
    //printf("\n");
  }
  return ;
}
CC_INLINE void* mem_malloc(int n, const char* file, int line)
{
  bf_mem_head_t* head;
  if (n<=0) {
    return NULL;
  }
  head = mem_find_first_free_block(n);

  if (head) {
    int len = mem_get_len(n);
    int size = len + BF_MEM_INFO_LEN;
    int allsize = head->size + BF_MEM_INFO_LEN;
    if (head->size > size) {
      bf_mem_head_t h = *head;
      bf_mem_head_t* next = NULL;
      next = (bf_mem_head_t*)((char*)(head) + len + BF_MEM_INFO_LEN);
      memblock_init(head, size, n, h.prev, next, file, line);
      memblock_init(next, allsize-size, 0, head, h.next, file, line);
    }
    head->used = n;
    GETMEM()->used += len;
    GETMEM()->cnt++;
  } else {
    mem_list();
    ASSERT(0 && "mem_malloc fail");
  }
  mem_list();
  return head+1;
}
CC_INLINE void* mem_calloc(int n, const char* file, int line) {
  void* p = mem_malloc(n, file, line);
  memset(p, 0, n);
  return p;
}

CC_INLINE int mem_msize(void* p) {
  int size = 0;
  if (p) {
    uchar* p0 = (uchar*)(p);
    bf_mem_head_t* head = ((bf_mem_head_t*)p0)-1;
    size = head->size;
  }
  return size;
}
CC_INLINE void mem_free(void* p, const char* file, int line)
{
  if (p) {
    uchar* p0 = (uchar*)(p);
    bf_mem_head_t* head = ((bf_mem_head_t*)p0)-1;
    if (head->used>0) {
      bf_mem_tail_t* tail = (bf_mem_tail_t*)(p0 + head->size);
      bf_mem_head_t* prev = head->prev;
      bf_mem_head_t* next = head->next;
      char* p1 = (char*)head;
      int len = head->size + BF_MEM_INFO_LEN;
      GETMEM()->used -= len;
      mem_list();
      if (tail) {
        ASSERT(head->magic == BF_MEM_MAGIC && "head bound err");
        ASSERT(tail->magic == BF_MEM_MAGIC && "tail bound err");
      }
      if (head->next && 0==head->next->used) {
        next = head->next->next;
        len += head->next->size + BF_MEM_INFO_LEN;
      }
      if (head->prev && 0==head->prev->used) {
        prev = head->prev->prev;
        p1 -= head->prev->size + BF_MEM_INFO_LEN;
        len += head->prev->size + BF_MEM_INFO_LEN;
      }
      memblock_init(p1, len, 0, prev, next, file, line);
      GETMEM()->cnt--;
    }
  }
}

CC_INLINE void* mem_realloc(void* p, int n, const char* file, int line) {
  uchar* p0 = (uchar*)(p);
  if (n<=0) {
    mem_free(p, file, line);
    return NULL;
  }
  if (NULL==p) {
    p = mem_malloc(n, file, line);
  } else {
    bf_mem_head_t* head = ((bf_mem_head_t*)p0)-1;
    //char* buf = (char*)p0;
    if (n > head->size) {
      int n2 = n - head->size - BF_MEM_INFO_LEN;
      if (0 && head->next && MEM_BLOCK_IS_GOOD(head->next, n2)) {
        int len = mem_get_len(n);
        bf_mem_head_t* head_prev = head->prev;
        bf_mem_head_t* next_next = head->next->next;
        bf_mem_head_t* next = (bf_mem_head_t*)((char*)(head) + len + BF_MEM_INFO_LEN);
        int size = head->size + head->next->size + BF_MEM_INFO_LEN + BF_MEM_INFO_LEN;
        memblock_init(head, len, n, head_prev, next, file, line);
        memblock_init(next, size-len, 0, head, next_next, file, line);
      } else {
        void* p2 = mem_malloc(n, file, line);
        memcpy(p2, p, head->used);
        mem_free(p, file, line);
        p = p2;
      }
    } else {
      head->used = n;
    }
  }
  mem_list();
  return p;
}
CC_INLINE void* mem_realloc2(void* p, int n, const char* file, int line) {
  //uchar* p0 = (uchar*)(p);
  if (NULL==p) {
    p = mem_malloc(n, file, line);
  } else {
    //bf_mem_head_t* head = ((bf_mem_head_t*)p0)-1;
    void* p2 = mem_malloc(n, file, line);
    //memcpy(p2, p, head->used);
    mem_free(p, file, line);
    p = p2;
  }
  return p;
}

CC_INLINE int mem_init(void* p, int n, const char* file, int line) {
  static mem_funs_t mem_memfun = {0};
  if (NULL==mem_memfun._free) {
    MEM_MEMFUN_SET(&mem_memfun, mem);
    _memfun = &mem_memfun;
  } else {
    ASSERT(GETMEM()->cnt==0);
  }
  GETMEM()->cnt = 0;
  GETMEM()->used = 0;
  GETMEM()->buf = (char*)p;
  GETMEM()->head = (bf_mem_head_t*)p;
  GETMEM()->len = n;
  memblock_init(p, n, 0, NULL, NULL, file, line);
  return 0;
}

CC_INLINE void memset2(void* adr, int val, int count)
{
  int i, n, v;
  unsigned int* p;
  unsigned short* q;
  p = (unsigned int*)adr;
  v = val | (val << 16);
  n = count >> 3;
  for (i = 0; i < n; i++) {
    p[0] = v;
    p[1] = v;
    p[2] = v;
    p[3] = v;
    p += 4;
  }
  q = (unsigned short*) p;
  n = count & 7;
  for (i = 0; i < n; i++) {
    *q++ = val;
  }
}
CC_INLINE void memset4(void* adr, int val, int count)
{
  int i, n, v;
  unsigned int* p;
  p = (unsigned int*)adr;
  v = val;
  n = count >> 2;
  for (i = 0; i < n; i++) {
    p[0] = v;
    p[1] = v;
    p[2] = v;
    p[3] = v;
    p += 4;
  }
  n = count & 3;
  for (i = 0; i < n; i++) {
    *p++ = val;
  }
}
/* count must be a multiple of 4 and >= 4 */
CC_INLINE void memset3(void* adr, int r, int v, int b, long count)
{
  long i, n;
  register long v1, v2, v3, *pt = (long*)(adr);
  unsigned char* p, R = (unsigned char)r, V = (unsigned char)v, B = (unsigned char)b;
  p = (unsigned char*)adr;
  *p++ = R;
  *p++ = V;
  *p++ = B;
  *p++ = R;
  *p++ = V;
  *p++ = B;
  *p++ = R;
  *p++ = V;
  *p++ = B;
  *p++ = R;
  *p++ = V;
  *p++ = B;
  v1 = *pt++;
  v2 = *pt++;
  v3 = *pt++;
  n = count >> 2;
  for (i = 1; i < n; i++) {
    pt[0] = v1;
    pt[1] = v2;
    pt[2] = v3;
    pt+=3;
  }
}

#define MEMSET(_F, _V, _N)     memset(_F, _V, sizeof(*(_F))*(_N))
#define MEMZERO(_F, _N)        memset(_F, 0, sizeof(*(_F))*(_N))
#define MEMCPY(_P, _F, _N)     memcpy(_P, _F, sizeof(*(_F))*(_N))
#ifndef MEMMOVE
#define MEMMOVE(_P, _F, _N)    memmove(_P, _F, sizeof(*(_F))*(_N))
#endif
#define SAFEFCLOSE(_PF)        if (NULL!=_PF) { fclose(_PF); _PF = NULL; }
#define MALLOCSET(_TYPE, _N, _V) (_TYPE*)memset(pmalloc(_N*sizeof(_TYPE)), _V, _N*sizeof(_TYPE))

#define MEM_GET16B(p, i, x) x = (i+=2, (((uchar*)p)[i-2]<<8) | (((uchar*)p)[i-1]))
#define MEM_GET16L(p, i, x) x = (i+=2, (((uchar*)p)[i-1]<<8) | (((uchar*)p)[i-2]))
#define MEM_GET32B(p, i, x) x = (i+=4, (((uchar*)p)[i-4]<<24) | (((uchar*)p)[i-3]<<16) | (((uchar*)p)[i-2]<<8) | (((uchar*)p)[i-1]))
#define MEM_GET32L(p, i, x) x = (i+=4, (((uchar*)p)[i-1]<<24) | (((uchar*)p)[i-2]<<16) | (((uchar*)p)[i-3]<<8) | (((uchar*)p)[i-4]))

#define MEM_SET16B(p, i, x) (((uchar*)p)[i+0] = ((x)>>8), ((uchar*)p)[i+1] = (x)&0xff, i+=2)
#define MEM_SET16L(p, i, x) (((uchar*)p)[i+1] = ((x)>>8), ((uchar*)p)[i+0] = (x)&0xff, i+=2)
#define MEM_SET32B(p, i, x) (((uchar*)p)[i+0] = ((x)>>24), ((uchar*)p)[i+1] = ((x)>>16), ((uchar*)p)[i+2] = ((x)>>8), ((uchar*)p)[i+3] = (x), i+=4)
#define MEM_SET32L(p, i, x) (((uchar*)p)[i+3] = ((x)>>24), ((uchar*)p)[i+2] = ((x)>>16), ((uchar*)p)[i+1] = ((x)>>8), ((uchar*)p)[i+0] = (x), i+=4)
#define MEM_SETMEM(p, i, x, n) (memcpy((uchar*)p + i, x, n), i+=n)

typedef union {
  uint32 u;
  int32 s;
  float f;
} inttofloat;
CC_INLINE uint32 IR(float x)
{
  inttofloat tmp;
  tmp.f = x;
  return tmp.u;
}
CC_INLINE float FR_u32(uint32 x)
{
  inttofloat tmp;
  tmp.u = x;
  return tmp.f;
}
CC_INLINE float FR_s32(int32 x)
{
  inttofloat tmp;
  tmp.s = x;
  return tmp.f;
}
CC_INLINE void byterev(void* v, int n)
{
  char* s = (char*)v;
  char* e = s + n - 1, t;
  for (; s < e; ++s, --e) {
    t = *s;
    *s = *e;
    *e = t;
  }
}
//#undef bswap_16
//#undef bswap_32
//#define bswap_16(X) ((((X)&0xFF) << 8) | (((X)&0xFF00) >> 8))
//#define bswap_32(X) ( (((X)&0x000000FF)<<24) | (((X)&0xFF000000) >> 24) | (((X)&0x0000FF00) << 8) | (((X) &0x00FF0000) >> 8))
#define BYTESWAP_16(X)  byteswap_16(&(X))
#define BYTESWAP_32(X)  byteswap_32(&(X))
#define BYTEORDERL(X)  if ('b'==ENDIANNESS) {BYTESWAP(X);}
#define BYTEORDERB(X)  if ('b'!=ENDIANNESS) {BYTESWAP(X);}
#define BYTEORDER16L(X)  if ('b'==ENDIANNESS) {byteswap_16(&X);}
#define BYTEORDER16B(X)  if ('b'!=ENDIANNESS) {byteswap_16(&X);}
#define BYTEORDER32L(X)  if ('b'==ENDIANNESS) {byteswap_32(&X);}
#define BYTEORDER32B(X)  if ('b'!=ENDIANNESS) {byteswap_32(&X);}

#if defined(_WIN32) && (_MSC_VER < 1300)
CC_INLINE uint16 _byteswap_ushort(const uint16 i)
{
    uint16 j;
    j =  (i << 8);
    j += (i >> 8);
    return j;
}

CC_INLINE uint32 _byteswap_ulong(const uint32 i)
{
    uint32 j;
    j =  (i << 24);
    j += (i <<  8) & 0x00FF0000;
    j += (i >>  8) & 0x0000FF00;
    j += (i >> 24);
    return j;
}

CC_INLINE uint64 _byteswap_uint64(uint64 const i)
{
    uint64 j;
    j =  (i << 56);
    j += (i << 40) & 0x00FF000000000000;
    j += (i << 24) & 0x0000FF0000000000;
    j += (i <<  8) & 0x000000FF00000000;
    j += (i >>  8) & 0x00000000FF000000;
    j += (i >> 24) & 0x0000000000FF0000;
    j += (i >> 40) & 0x000000000000FF00;
    j += (i >> 56);
    return j;
}
#endif

CC_INLINE void byteswap_16(void* x)
{
  uchar* ux = (uchar*)x, t;
  CC_SWAP(ux[0], ux[1], t);
}
CC_INLINE void byteswap_32(void* x)
{
  uchar* ux = (uchar*)x, t;
  CC_SWAP(ux[0], ux[3], t);
  CC_SWAP(ux[1], ux[2], t);
}
#define BYTESWAP(t)   byteswap(sizeof(t), &t)
CC_INLINE void byteswap(int size, void* x)
{
  uchar* ux = (uchar*)x, t;
  int i;
  switch (size) {
  case 1:
    break;
  case 2:
    CC_SWAP(ux[0], ux[1], t);
    break;
  case 3:
    CC_SWAP(ux[0], ux[2], t);
    break;
  case 4:
    CC_SWAP(ux[0], ux[3], t);
    CC_SWAP(ux[1], ux[2], t);
    break;
  default:
    for (i = 0; i < size / 2; ++i) {
      CC_SWAP(ux[i], ux[size - 1 - i], t);
    }
    break;
  }
}

#define bfinit(bf, _p, _n)  ((bf)->len=_n, (bf)->data = (unsigned char*)_p)
#define bfsetsize(bf, newlen)  ((bf)->len=newlen, (bf)->data = (unsigned char*)prealloc((bf)->data, newlen))
#define bffree(bf)   if ((bf)->data) {(pfree((bf)->data), (bf)->data=0, (bf)->len=0);}
#define bffrees2(bf, n)   {int _i=0; for (_i=0; _i<(int)(n); ++_i) {bffree((bf+_i));}}
#define bffrees(bf)   bffrees2(bf, countof(bf))
#define bfdata(bf, T)  ((T*)((bf)->data))
#define bfsize(bf, T)  (((bf)->len)/sizeof(T))
#define bfsetsizeT(bf, newlen, T)  bfsetsize(bf, (newlen)*sizeof(T))
#define bfzero(bf)   memset((bf)->data, 0, (bf)->len)
CC_INLINE int bfinsert(buf_t* bf, int bypos, const void* data, int datalen)
{
  int oldlen = bf->len;
  int newlen = MAX(oldlen, bypos) + datalen;
  bfsetsize(bf, newlen);
  if (bypos < oldlen) {
    memmove(bf->data + bypos + datalen, bf->data + bypos, oldlen - bypos);
  }
  else {
    memset(bf->data + oldlen, 0, bypos - oldlen);
  }
  memcpy(bf->data + bypos, data, datalen);
  return 0;
}
CC_INLINE void* matcpy(void* dst, int dl, const void* src, int sl, int h, int w)
{
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  if (dl == sl && dl == w) {
    return memcpy(dst, src, w * h);
  }
  for (; h--; s += sl, d += dl) {
    memcpy(d, s, w);
  }
  return dst;
}
CC_INLINE void* matmove(void* dst, int dl, const void* src, int sl, int h, int w)
{
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  if (dl == sl && dl == w) {
    return memmove(dst, src, w * h);
  }
  for (; h--; s += sl, d += dl) {
    memmove(d, s, w);
  }
  return dst;
}
// 把 i~n 替换成p0 l0
CC_INLINE int memrep(const void* p, int l, int i, int n, const void* p0, int l0, void* p1, int l1)
{
  const char* s = (const char*)p;
  const char* s0 = (const char*)p0;
  char* s1 = (char*)p1;
  int l2 = l + l0 - n;
  ASSERT(l2 <= l1);
  if ((l + l0 - n) <= l1) {
    if (s1 != s) {
      memcpy(s1, s, i);
    }
    memmove(s1 + i + l0, s + i + n, l - (i + n));
    if (s0) {
      memcpy(s1 + i, s0, l0);
    }
  }
  return l2;
}
//#define my_realloc(p, n, newn) prealloc(p, newn)
#define MEMINST(P, N, I, F, M)  meminst(P, N, I, F, M, sizeof(*(P)))
CC_INLINE int meminst(void* p, int n, int i, const void* f, int m, int elemsize)
{
  memmove((char*)p + (i + m)*elemsize, (char*)p + i*elemsize, (n - i)*elemsize);
  if (f) {
    memcpy((char*)p + i*elemsize, f, m*elemsize);
  }
  return m + n;
}
// del m byte from p+i
#define MEMDEL(P, N, I, M)  memdel(P, N, I, M, sizeof(*(P)) )
CC_INLINE int memdel(void* p, int n, int i, int m, int elemsize)
{
  memcpy((char*)p + i*elemsize, (char*)p + (i + m)*elemsize, (n - i - m)*elemsize);
  return n - m;
}
#define MEMINS(_P, _N, _POS, _NEWN)  my_memins((void**)&(_P), _N, _POS, _NEWN, sizeof(*(_P)))
#define MEMPUSH(_P, _N)  MEMINS(_P, _N, _N, 1)
CC_INLINE void* my_memins(void** p, int n, int pos, int m, int elemsize)
{
  n *= elemsize, pos *= elemsize, m *= elemsize;
  prealloc_(p, MAX(n, pos) + m, __FILE__, __LINE__);
  memmove((char*)p + pos + m, (char*)p + pos, n - pos);
  return p;
}
#define SZMIN(sz1, sz2)  iSIZE(MIN(sz1.cx, sz2.cx), MIN(sz1.cy, sz2.cy))
CC_INLINE void* mat_realloc(void* p, int h, int w, int new_h, int new_w)
{
  char* newp;
  int newn = new_w * new_h;
  int min_h = MIN(h, new_h), min_w = MIN(w, new_w);
  newp = (char*)pmalloc(newn);
  memset(newp, 0, newn);
  if (p) {
    matcpy(newp, new_w, p, w, min_h, min_w);
    pfree(p);
  }
  return newp;
}
CC_INLINE void* mat_inscol(void* p, int h, int w, int pos, int n)
{
  int new_w = MAX(w, pos) + n;
  p = mat_realloc(p, h, w, h, new_w);
  if (w > pos) {
    matmove((char*)p + pos + n, new_w, (char*)p + pos, new_w, h, w - pos);
  }
  return p;
}
CC_INLINE void* mat_insrow(void* p, int h, int w, int pos, int n)
{
  int new_h = MAX(h, pos) + n;
  p = mat_realloc(p, h, w, new_h, w);
  if (h > pos) {
    matmove((char*)p + (pos + n)*w, w, (char*)p + pos * w, w, h - pos, w);
  }
  return p;
}
CC_INLINE int memcpy2da(void* B, int bl, const void* A, int al, int h, int w)
{
  int i;
  for (i = 0; i < h; ++i) {
    memcpy(((uchar*)B + i * bl), ((const uchar*)A + i * al), w);
  }
  return 0;
}
CC_INLINE int copy2d(int h, int w, const void* A, int al, void* B, int bl)
{
  int i;
  for (i = 0; i < h; ++i) {
    memcpy(((uchar*)B + i * bl), ((const uchar*)A + i * al), w);
  }
  return 0;
}
CC_INLINE void* memcpy2d(void* dst, int dl, const void* src, int sl, int h, int w)
{
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  if (dl == sl && dl == w) {
    return memcpy(dst, src, h * w);
  }
  for (; h--; s += sl, d += dl) {
    memcpy(d, s, w);
  }
  return dst;
}
CC_INLINE void* memcpy_mask(void* dst, const void* src, const uchar* mask, int n, int c) {
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  if (NULL==mask) {
    return memcpy(dst, src, n*c);
  }
#define MEMCPY_MASK_CASE(_C, X) case _C: for (; n--; ++mask, s += c, d += c) { if (*mask) {X;} } break
  switch (c) {
    MEMCPY_MASK_CASE(1, *d=*s);
    MEMCPY_MASK_CASE(2, *(short*)d=*(const short*)s );
    MEMCPY_MASK_CASE(3, (d[0]=s[0], d[1]=s[1], d[2]=s[2]) );
    MEMCPY_MASK_CASE(4, *(int*)d=*(const int*)s );
    MEMCPY_MASK_CASE(8, *(int64*)d=*(const int64*)s );
  default:
    for (; n--; ++mask, s += c, d += c) {
      if (*mask) {
        memcpy(d, s, c);
      }
    }
    break;
  }
#undef MEMCPY_MASK_CASE
  return dst;
}
CC_INLINE void* memcpy2dmask(void* dst, int dl, const void* src, int sl, const uchar* mask, int ml, int h, int w, int c) {
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  if (NULL==mask) {
    return memcpy2d(dst, dl, src, sl, h, w*c);
  }
  if (dl == sl && dl == w*c && ml == w) {
    return memcpy_mask(dst, src, mask, h * w, c);
  }
  for (; h--; s += sl, d += dl, mask += ml) {
    memcpy_mask(d, s, mask, w, c);
  }
  return dst;
}
CC_INLINE int mem_swap(int n, void* src, void* dst)
{
  int i, t0, t1, t2, t3;
  int* s = (int*) src;
  int* d = (int*) dst;
  char* cs = (char*) src;
  char* cd = (char*) dst;
  char ct;
  for (i = 0; i < n - 15; i += 16, s += 4, d += 4) {
    CC_SWAP(s[0], d[0], t0);
    CC_SWAP(s[1], d[1], t1);
    CC_SWAP(s[2], d[2], t2);
    CC_SWAP(s[3], d[3], t3);
  }
  for (; i < n; ++i) {
    CC_SWAP(cs[i], cd[i], ct);
  }
  return 0;
}
// 上下颠倒
CC_INLINE int matflip(int h, int w, void* src, int srcstep)
{
  int i;
  char* cs = (char*) src;
  char* cd = (char*) src + (h - 1) * srcstep;
  for (i = 0; i < h / 2; ++i, cs += srcstep, cd -= srcstep) {
    mem_swap(w, cs, cd);
  }
  return 0;
}
CC_INLINE void* memflip(void* dst, int dl, const void* src, int sl, int h, int w)
{
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  if (d==s && dl==sl) {
    matflip(h, w, dst, dl);
  } else {
    d += (h - 1) * dl;
    for (; h--; s += sl, d -= dl) {
      memcpy(d, s, w);
    }
  }
  return 0;
}
#define MEMDUP(_T, _P, _N)  ((_T*)memdup(_P, (_N)*sizeof(_T)))
CC_INLINE void* memdup1(const void* pByte, uint32 dwSize)
{
  char* ret = (char*)pmalloc(dwSize + 1);
  memcpy(ret, pByte, dwSize);
  ret[dwSize] = 0;
  return ret;
}
CC_INLINE void* memdup(const void* pSrc, int Length)
{
  void* pNew = pmalloc(Length);
  memmove(pNew, pSrc, Length);
  return pNew;
}
#define MEMDUP2D(_T, A, al, h, w)  (_T*)memdup2d(A, (al)*sizeof(_T), h, (w)*sizeof(_T))
CC_INLINE void* memdup2d(const void* A, int al, int h, int w)
{
  void* pNew = pmalloc(h * w);
  memcpy2d(pNew, w, A, al, h, w);
  return pNew;
}
CC_INLINE int memmap(int n, const uchar* src, const void* pmap, int mapcn, void* dst, int dstcn, int cn)
{
  int i, j;
  uchar* ud = (uchar*)dst;
  const uchar* um = (const uchar*)pmap;
  cn = BOUND(cn, 0, MIN(mapcn, dstcn));
  for (i = 0; i < n; ++i) {
    j = src[i];
    memcpy(ud + i * dstcn, um + j * mapcn, cn);
  }
  return 0;
}
CC_INLINE int mem_insert(void* pp, int* n, int pos, const void* q, int m, int elemsize)
{
  void** p = (void**)pp;
  char* pc = 0;
  pprealloc(p, (*n + m) * elemsize, __FILE__, __LINE__);
  pc = (char*)(*p);
  memmove(pc + (pos + m)*elemsize, pc + pos * elemsize, (*n - pos)*elemsize);
  if (q) {
    memcpy(pc + pos * elemsize, q, m * elemsize);
  }
  *n += m;
  return 0;
}
CC_INLINE int mem_push_back(void* pp, int* n, const void* q, int m, int elemsize)
{
  return mem_insert(pp, n, *n, q, m, elemsize);
}
typedef uint32 COLOR;
typedef uint64 COLOR64;
#define FILL2D_IMPL_BEGIN()   for (y = 0; y < h; ++y) { uchar* pix = ptr + y*al;for (x=0; x<w; ++x, pix+=ai) {
#define FILL2D_IMPL_END()     }}
CC_INLINE int fill2d(int h, int w, void* A, int al, int ai, COLOR color)
{
  int x, y;
  uchar* ptr = (uchar*)A;
  uchar* pclr = (uchar*)&color;
  int _a = pclr[3];
  if (255 == _a) {
    for (x = 0; x < w; ++x) {
      memcpy(ptr + (x)*ai, &color, ai);
    }
    for (y = 1; y < h; ++y) {
      memcpy(ptr + y * al, A, w * ai);
    }
  }
  else {
    switch (ai) {
    case 1:
      FILL2D_IMPL_BEGIN();
      pix[0] += ((pclr[0] - pix[0]) * _a) >> 8;
      FILL2D_IMPL_END();
      break;
    case 2:
      FILL2D_IMPL_BEGIN();
      pix[0] += ((pclr[0] - pix[0]) * _a) >> 8;
      pix[1] += ((pclr[1] - pix[1]) * _a) >> 8;
      FILL2D_IMPL_END();
      break;
    case 3:
    case 4:
      FILL2D_IMPL_BEGIN();
      pix[0] += ((pclr[0] - pix[0]) * _a) >> 8;
      pix[1] += ((pclr[1] - pix[1]) * _a) >> 8;
      pix[2] += ((pclr[2] - pix[2]) * _a) >> 8;
      FILL2D_IMPL_END();
      break;
    }
  }
  return 0;
}
#undef FILL2D_IMPL_BEGIN
#undef FILL2D_IMPL_END
#define REVCOPY(_B, _A, _N)   { int _i=0; for (; _i<(_N); ++_i) {(_B)[(_N)-1-_i]=(_A)[_i]; } }
//#define ARRREV(_N, _A, t)   { int _i=0; for (; _i<(_N)/2; ++_i) { CC_SWAP((_A)[(_N)-_i-1], (_A)[_i], t); } }
#define ARRREV(_N, _A, t)   {int _i=0; for (;_i<(_N)/2; ++_i) {t=(_A)[_i], (_A)[_i]=(_A)[_N-1-_i], (_A)[_N-1-_i]=t;} }
CC_INLINE int memrev(void* A, int elemsize, int n)
{
  switch (elemsize) {
#define MEMREVCPY_CASE(cn)  case cn: {typedef struct {uchar t[cn];} arrtype; arrtype t, * a0=(arrtype*)A; ARRREV(n, a0, t); } break
    MEMREVCPY_CASE(1);
    MEMREVCPY_CASE(2);
    MEMREVCPY_CASE(3);
    MEMREVCPY_CASE(4);
    MEMREVCPY_CASE(5);
    MEMREVCPY_CASE(6);
    MEMREVCPY_CASE(7);
    MEMREVCPY_CASE(8);
    MEMREVCPY_CASE(9);
    MEMREVCPY_CASE(10);
    MEMREVCPY_CASE(11);
    MEMREVCPY_CASE(12);
    MEMREVCPY_CASE(13);
    MEMREVCPY_CASE(14);
    MEMREVCPY_CASE(15);
    MEMREVCPY_CASE(16);
    MEMREVCPY_CASE(17);
    MEMREVCPY_CASE(18);
    MEMREVCPY_CASE(19);
    MEMREVCPY_CASE(20);
    MEMREVCPY_CASE(21);
    MEMREVCPY_CASE(22);
    MEMREVCPY_CASE(23);
    MEMREVCPY_CASE(24);
    MEMREVCPY_CASE(25);
    MEMREVCPY_CASE(26);
    MEMREVCPY_CASE(27);
    MEMREVCPY_CASE(28);
    MEMREVCPY_CASE(29);
    MEMREVCPY_CASE(30);
    MEMREVCPY_CASE(31);
    MEMREVCPY_CASE(32);
    MEMREVCPY_CASE(33);
    MEMREVCPY_CASE(34);
    MEMREVCPY_CASE(35);
    MEMREVCPY_CASE(36);
    MEMREVCPY_CASE(37);
    MEMREVCPY_CASE(38);
    MEMREVCPY_CASE(39);
  default:
    ASSERT(0);
    break;
  }
#undef MEMREVCPY_CASE
  return 0;
}
CC_INLINE int memrevcpy(void* B, const void* A, int elemsize, int n)
{
  switch (elemsize) {
#define MEMREVCPY_CASE(cn)  case cn: {typedef struct {uchar t[cn];} arrtype; const arrtype* a0=(const arrtype*)A; arrtype* b0=(arrtype*)B; REVCOPY(b0, a0, n); } break
    MEMREVCPY_CASE(1);
    MEMREVCPY_CASE(2);
    MEMREVCPY_CASE(3);
    MEMREVCPY_CASE(4);
    MEMREVCPY_CASE(5);
    MEMREVCPY_CASE(6);
    MEMREVCPY_CASE(7);
    MEMREVCPY_CASE(8);
    MEMREVCPY_CASE(9);
    MEMREVCPY_CASE(10);
    MEMREVCPY_CASE(11);
    MEMREVCPY_CASE(12);
    MEMREVCPY_CASE(13);
    MEMREVCPY_CASE(14);
    MEMREVCPY_CASE(15);
    MEMREVCPY_CASE(16);
    MEMREVCPY_CASE(17);
    MEMREVCPY_CASE(18);
    MEMREVCPY_CASE(19);
    MEMREVCPY_CASE(20);
    MEMREVCPY_CASE(21);
    MEMREVCPY_CASE(22);
    MEMREVCPY_CASE(23);
    MEMREVCPY_CASE(24);
    MEMREVCPY_CASE(25);
    MEMREVCPY_CASE(26);
    MEMREVCPY_CASE(27);
    MEMREVCPY_CASE(28);
    MEMREVCPY_CASE(29);
    MEMREVCPY_CASE(30);
    MEMREVCPY_CASE(31);
    MEMREVCPY_CASE(32);
    MEMREVCPY_CASE(33);
    MEMREVCPY_CASE(34);
    MEMREVCPY_CASE(35);
    MEMREVCPY_CASE(36);
    MEMREVCPY_CASE(37);
    MEMREVCPY_CASE(38);
    MEMREVCPY_CASE(39);
  default:
    ASSERT(0);
    break;
  }
#undef MEMREVCPY_CASE
  return 0;
}
CC_INLINE int data2men1(FILE* fpout, const void* ca, int len, int addr, int isdat)
{
  int i;
  const char* HEXFOMAT = isdat ? "0x%02x%02x%02x%02x\r\n" : "0x%02x%02x%02x%02x,\r\n";
  if (isdat) {
    fprintf(fpout, "1651 1 %x 0 %x\r\n", addr, (len + 3) / 4);
  }
  for (i = 0; i < len - 3; i += 4) {
    unsigned char* r_data = (unsigned char*) ca + i;
    fprintf(fpout, HEXFOMAT, r_data[ 3 ], r_data[ 2 ], r_data[ 1 ], r_data[ 0 ]);
  }
  if (i < len) {
    unsigned char buf[ 4 ] = { 0 };
    for (; i < len; ++i) {
      buf[ i ] = *((unsigned char*) ca + i);
    }
    fprintf(fpout, HEXFOMAT, buf[ 3 ], buf[ 2 ], buf[ 1 ], buf[ 0 ]);
  }
  return 0;
}

CC_INLINE void sendbuf_delall(sendbuf_t** ss) {
  while (*ss != NULL) {
    sendbuf_t* next = (*ss)->next;
    pfree(*ss);
    *ss = next;
  }
}
CC_INLINE int sendbuf_addbuf(sendbuf_t** ss, const void* data, int n) {
  sendbuf_t* new_sb = (sendbuf_t*)pmalloc(n + sizeof(sendbuf_t));
  if (new_sb) {
    // 加到最后
    new_sb->len = n;
    new_sb->next = NULL;
    memcpy(new_sb->data, data, n);
    if (*ss) {
      sendbuf_t* sb = *ss;
      while(sb->next) {
        sb = sb->next;
      }
      sb->next = new_sb;
    } else {
      *ss = new_sb;
    }
    return 1;
  }
  return 0;
}


/******************************** Memory storage ****************************************/

#define CC_STORAGE_MAGIC_VAL    0x42890000

typedef struct mem_block_t
{
    int signature;
    struct mem_block_t *next;
    struct mem_block_t *prev;
    int block_size;
    int free_space;
}
mem_block_t;

#define CC_IS_STORAGE(storage)  \
    ((storage) != NULL &&       \
    (((CvMemStorage*)(storage))->signature & CC_MAGIC_MASK) == CC_STORAGE_MAGIC_VAL)

//#define CC_IS_MAT(mat)  (mat)
#define IMFUN_BEGIN(b, im)  {img_t im_tmp[1] = {0}; if (b) {imclone2(im, im_tmp); im = im_tmp; }

#define IMFUN_END()  imfree(im_tmp); }

#define cRound(x)   ROUND(x)
#define cFloor(x)   FLOOR(x)
#define cCeil(x)    CEIL(x)
#define cAlloc(x)   pmalloc(x)
#define cFree(x)    (pfree(*(x)), *(x)=NULL)

#define MEM_BLOCK_ALLOC(T, storage, block_size, size) (T*)mem_block_alloc(&(storage), block_size, sizeof(T)*(size), __FILE__, __LINE__)
static void*
mem_block_alloc( mem_block_t** storage, int block_size, int size, const char* file, int line )
{
    uchar* ptr = NULL;
    if (!(*storage) || (*storage)->free_space < size) {
      mem_block_t* new_block;
      if( block_size <= 0 )
          block_size = CC_STORAGE_BLOCK_SIZE;

      block_size = MAX(block_size, size) + sizeof(mem_block_t);
      block_size = cvAlign( block_size, CC_STRUCT_ALIGN );

      new_block = (mem_block_t*)pmalloc_( block_size, file, line );
      memset( new_block, 0, block_size);
      //LIST_ADDFRONT((*storage), new_block);
      new_block->signature = CC_STORAGE_MAGIC_VAL;
      new_block->block_size = block_size;
      new_block->free_space = block_size;
      new_block->next = (*storage);
      new_block->prev = NULL;
      if (*storage) {
        (*storage)->prev = new_block;
      }
      (*storage) = new_block;
    }
    ptr = (uchar*)((*storage)+1);
    (*storage)->free_space -= size;
    ptr += (*storage)->free_space;
    return ptr;
}

#define MEM_BLOCK_FREEALL(storage) mem_block_freeall(&storage, __FILE__, __LINE__)
static void
mem_block_freeall( mem_block_t** storage, const char* file, int line )
{
    for( ; (*storage); )
    {
        mem_block_t* next = (*storage)->next;
        pfree_( (*storage), file, line );
        (*storage) = next;
    }
}


#endif // _CMEM_H_
