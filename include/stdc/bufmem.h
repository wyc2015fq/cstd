
#ifndef _BUFMEM_H_
#define _BUFMEM_H_

#include "stdc.h"

#define SIZEOFARR(_T, _N)           (sizeof(_T)*(_N))

#ifdef _DEBUG
#define BUFUSEBEGIN(_BUF, _BUFSZ)   { unsigned char* __BUFEND__ = (unsigned char*)(_BUF)+(_BUFSZ), * __BUFBEG__ = (unsigned char*)(_BUF)/**/
#define BUFUSEEND()                 ASSERT(__BUFBEG__<=__BUFEND__);/**/ }
#else
#define BUFUSEBEGIN(_BUF, _BUFSZ)   { unsigned char* __BUFEND__ = (unsigned char*)(_BUF)+(_BUFSZ), * __BUFBEG__ = (unsigned char*)(_BUF)/**/
#define BUFUSEEND()                 /*ASSERT(__BUFBEG__<=__BUFEND__);*/ }
#endif


#define GETBUFBEG()                 (__BUFBEG__)
#define GETBUFLEN()                 (__BUFEND__-__BUFBEG__)
CC_INLINE void ptr2ptr(void* pp, void* p) {
  *(void**)pp = p;
}
#define BUFMALLOC2(_P, _N)          ptr2ptr(&_P, (void*)(__BUFBEG__+=SIZEOFARR(*_P, _N), __BUFBEG__-SIZEOFARR(*_P, _N)))
#define BUFFREE2(_P, _N)            (__BUFBEG__-=SIZEOFARR(*_P, _N), _P = NULL)
#define BUFMALLOC(_T, _N)           (_T*)(__BUFBEG__+=SIZEOFARR(_T, _N), __BUFBEG__-SIZEOFARR(_T, _N))
#define BUFFREE(_T, _N)             (__BUFBEG__-=SIZEOFARR(_T, _N))
#define BUFBOUNDCHECK()             ASSERT(__BUFBEG__<=__BUFEND__)


typedef struct buf_t {
  int len;
  unsigned char* data;
} buf_t;


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
//
#define bfinit(bf, _p, _n)  ((bf)->len=_n, (bf)->data = (unsigned char*)_p)
#define bfsetsize(bf, newlen)  ((bf)->len=newlen, (bf)->data = (unsigned char*)realloc((bf)->data, newlen))
#define bffree(bf)   if ((bf)->data) {(free((bf)->data), (bf)->data=0, (bf)->len=0);}
#define bffrees2(bf, n)   {int _i=0; for (_i=0; _i<(int)(n); ++_i) {bffree((bf+_i));}}
#define bffrees(bf)   bffrees2(bf, countof(bf))
#define bfdata(bf, T)  ((T*)((bf)->data))
#define bfsize(bf, T)  (((bf)->len)/sizeof(T))
#define bfsetsizeT(bf, newlen, T)  bfsetsize(bf, (newlen)*sizeof(T))
#define bfzero(bf)   memset((bf)->data, 0, (bf)->len)
//
#define BF_MEM_INFO_LEN    (int)(sizeof(bf_mem_head_t)+sizeof(bf_mem_tail_t))
#define BFMALLOC(_BF, _P, _N)  bf_malloc(_BF, (void**)&(_P), (_N)*sizeof(*(_P)))
#define BFFREE(_BF, _P)    bf_free(_BF, (void**)&(_P))
#define BF_MALLOC(_P, _N)  BFMALLOC(bf, _P, _N)
#define BF_CALLOC(_P, _N)  BFMALLOC(bf, _P, _N); if (_P) MEMZERO(_P, _N)
#define BF_FREE(_P)    BFFREE(bf, _P)
#define SYSMALLOC(_T, _P, _N)  (_P = (_T*)malloc((_N)*sizeof(*(_P))))
#define SYSFREE(_T, _P, _N)    (free(_P), _P=NULL)
//static int bf_maxlen=0;
//static int bf_malloc_maxlen=0;
CC_INLINE int bf_malloc(buf_t* bf, void* p, int n)
{
  uchar* p0 = 0;
  bf_mem_head_t* head;
  bf_mem_tail_t* tail;
  int len = (n + BF_MEM_INFO_LEN);
  if (len > bf->len) {
    //ASSERT(len <= bf->len && "¿Õ¼ä²»¹»");
    printf("bf_malloc error : Memory space is not enough!!\n");
    *(void**)p = NULL;
  }
  else {
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
    ASSERT((bf->data + bf->len + sizeof(bf_mem_head_t)) == p0 && "ÊÍ·ÅË³Ðò²»¶Ô");
    //ASSERT(head->size == n && "??ï¿½ï¿½?????");
    ASSERT(head->magic == BF_MEM_MAGIC && "??????");
    ASSERT(((bf_mem_tail_t*)(p0 + head->size))->magic == BF_MEM_MAGIC && "ï¿½ï¿½?????");
    bf->len += len;
    *pp = NULL;
  }
  return 0;
}


#endif // _BUFMEM_H_
