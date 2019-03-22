
#ifndef _STDC_MEM_C_H_
#define _STDC_MEM_C_H_

#include "stddef_c.h"
#include "inttypes_c.h"
#include <stdarg.h>


/////////////////////////////////////////////////////////

static void* matcpy(void* dst, int dl, const void* src, int sl, int h, int w)
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
static void* matmove(void* dst, int dl, const void* src, int sl, int h, int w)
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

static int memcpy2da(void* B, int bl, const void* A, int al, int h, int w)
{
  int i;
  for (i = 0; i < h; ++i) {
    memcpy(((uchar*)B + i * bl), ((const uchar*)A + i * al), w);
  }
  return 0;
}
static int copy2d(int h, int w, const void* A, int al, void* B, int bl)
{
  int i;
  for (i = 0; i < h; ++i) {
    memcpy(((uchar*)B + i * bl), ((const uchar*)A + i * al), w);
  }
  return 0;
}
static void* memcpy2d(void* dst, int dl, const void* src, int sl, int h, int w)
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
static int mem_swap(int n, void* src, void* dst)
{
  int i, t0, t1, t2, t3;
  int* s = (int*)src;
  int* d = (int*)dst;
  char* cs = (char*)src;
  char* cd = (char*)dst;
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
static int matflip(int h, int w, void* src, int srcstep)
{
  int i;
  char* cs = (char*)src;
  char* cd = (char*)src + (h - 1) * srcstep;
  for (i = 0; i < h / 2; ++i, cs += srcstep, cd -= srcstep) {
    mem_swap(w, cs, cd);
  }
  return 0;
}
static void* memflip(void* dst, int dl, const void* src, int sl, int h, int w)
{
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  if (d == s && dl == sl) {
    matflip(h, w, dst, dl);
  } else {
    d += (h - 1) * dl;
    for (; h--; s += sl, d -= dl) {
      memcpy(d, s, w);
    }
  }
  return 0;
}
static void memset2(void* adr, int val, int count)
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
  q = (unsigned short*)p;
  n = count & 7;
  for (i = 0; i < n; i++) {
    *q++ = val;
  }
}
static void memset4(void* adr, int val, int count)
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
static void memset3(void* adr, int r, int v, int b, long count)
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
    pt += 3;
  }
}

#define MEMSET(_F, _V, _N)     memset(_F, _V, sizeof(*(_F))*(_N))
#define MEMZERO(_F, _N)        memset(_F, 0, sizeof(*(_F))*(_N))
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////

typedef struct buf_t {
  int len;
  unsigned char* data;
} buf_t;


#define BF_MEM_MAGIC  (0x55667788)
#define ALIGN_TO(_X, ALIGN) ( ((size_t)(_X) + ALIGN - 1) & ~(size_t)(ALIGN - 1) )
typedef struct bf_mem_tail_t {
  uint32_t magic;
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
  uint32_t magic;
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
static int bf_malloc(buf_t* bf, void* p, int n)
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
static int bf_free(buf_t* bf, void* p)
{
  void** pp = (void**)p;
  uchar* p0 = (uchar*)(*pp);
  if (p0) {
    bf_mem_head_t* head = (bf_mem_head_t*)(p0 - sizeof(bf_mem_head_t));
    //bf_mem_tail_t* tail = (bf_mem_tail_t*)(p0 + head->size);
    int len = head->size + BF_MEM_INFO_LEN;
    //ASSERT((bf->data + bf->len + sizeof(bf_mem_head_t)) == p0 && "释放顺序不对");
    //ASSERT(head->size == n && "??锟斤拷?????");
    ASSERT(head->magic == BF_MEM_MAGIC && "?");
    ASSERT(((bf_mem_tail_t*)(p0 + head->size))->magic == BF_MEM_MAGIC && "锟斤拷?????");
    bf->len += len;
    *pp = NULL;
  }
  return 0;
}

/////////////////////////////////////////////////////////
enum MemType { CPUMEM, GPUMEM };
struct mem_t {
  MemType type_;
  void* (*realloc_)(void* p, size_t n);
  void(*free_)(void* p);
};
static void* cpu_realloc(void* p, size_t n) { return realloc(p, n); }
static void cpu_free(void* p) { free(p); }
static void* copy_cpu2cpu(void* dst, const void* src, size_t n) { return memcpy(dst, src, n); }
typedef void* (*mem_copy_t)(void* dst, const void* src, size_t n);
static mem_copy_t mem_copy[2][2] = { copy_cpu2cpu , NULL, NULL, NULL };
static mem_t cpu_mem[1] = { CPUMEM, cpu_realloc , cpu_free };
static mem_t cpu_mem_nul[1] = { CPUMEM, NULL , NULL };
static void* mem_realloc(void* p, size_t newn, mem_t* newmem, size_t oldn, mem_t* oldmem)
{
  void* newp = NULL;
  newmem = newmem ? newmem : cpu_mem;
  oldmem = oldmem ? oldmem : cpu_mem;
  if (newmem == oldmem) {
    newp = newmem->realloc_(p, newn);
  } else {
    newp = newmem->realloc_(NULL, newn);
    mem_copy[newmem->type_][oldmem->type_](newp, p, oldn);
    if (oldmem->free_) { oldmem->free_(p); }
  }
  return newp;
}
/////////////////////////////////////////////////////////

#define MULTIALLOC2(buf, p1, n1, p2, n2)   multialloc(&buf, &p1, (n1)*sizeof(*p1), &p2, (n2)*sizeof(*p2), NULL)
#define MULTIALLOC3(buf, p1, n1, p2, n2, p3, n3)   multialloc(&buf, &p1, (n1)*sizeof(*p1), &p2, (n2)*sizeof(*p2), &p3, (n3)*sizeof(*p3), NULL)
#define MULTIALLOC4(buf, p1, n1, p2, n2, p3, n3, p4, n4)   multialloc(&buf, &p1, (n1)*sizeof(*p1), &p2, (n2)*sizeof(*p2), &p3, (n3)*sizeof(*p3), &p4, (n4)*sizeof(*p4), NULL)
#define MULTIALLOC5(buf, p1, n1, p2, n2, p3, n3, p4, n4, p5, n5)   multialloc(&buf, &p1, (n1)*sizeof(*p1), &p2, (n2)*sizeof(*p2), &p3, (n3)*sizeof(*p3), &p4, (n4)*sizeof(*p4), &p5, (n5)*sizeof(*p5), NULL)
CC_INLINE void* multialloc(void* p, ...) {
  int n = 0, n1 = 0;
  void** pp = (void**)p;
  char* pc = (char*)*pp;
  void** pp1;
  va_list arglist;
  {
    va_start(arglist, p);
    for (; (pp1 = (void**)va_arg(arglist, char*)) != NULL;) {
      n1 = va_arg(arglist, int);
      n += n1;
    }
    va_end(arglist);
  }
  MYREALLOC(pc, n);
  *pp = pc;
  {
    va_start(arglist, p);
    for (; (pp1 = va_arg(arglist, void**)) != NULL;) {
      n1 = va_arg(arglist, int);
      *pp1 = pc;
      pc += n1;
    }
    va_end(arglist);
  }
  return *pp;
}
/////////////////////////////////////////////////////////

static int fillImage(int h, int w, uchar* dst, int dststep, int cn, COLOR color) {
  int i, j;
  uchar* c = (uchar*)(&color);
  uchar r = c[0];
  uchar g = c[1];
  uchar b = c[2];
  uchar a = c[3];
  switch (cn) {
  case 1:
    r = (r + (g << 1) + b) >> 2;
    for (i = 0; i < h; ++i) {
      uchar* d = dst + dststep*i;
      for (j = 0; j < w; ++j) {
        d[j] = r;
      }
    }
    break;
  case 3:
    for (i = 0; i < h; ++i) {
      uchar* d = dst + dststep*i;
      for (j = 0; j < w; ++j, d += cn) {
        d[0] = r;
        d[1] = g;
        d[2] = b;
      }
    }
    break;
  case 4:
    for (i = 0; i < h; ++i) {
      uchar* d = dst + dststep*i;
      for (j = 0; j < w; ++j, d += cn) {
        d[0] = r;
        d[1] = g;
        d[2] = b;
        d[3] = a;
      }
    }
    break;
  default:
    assert(0);
    break;
  }
  return 0;
}


static void* blend(void* dst, const void* src, int a, size_t size ) {
  uchar* tptr = (uchar*)dst;
  const uchar* tsrc = (const uchar*)src;
  for (size_t i = 0; i < size; ++i) {
    int _cb = tptr[i];
    int cb = tsrc[i];
    _cb += ((cb - _cb)*a + 127) >> 8;
    tptr[i] = (uchar)_cb;
  }
  return dst;
}

static void* set_line(void* dst, const void* color, int pix_size, size_t pix_num) {
  uchar* hline_min_ptr = (uchar*)dst;
  uchar* hline_end_ptr = (uchar*)dst + (pix_num)*(pix_size);
  uchar* hline_ptr = (uchar*)dst;
  if (pix_size == 1) {
    memset(hline_min_ptr, *(uchar*)color, hline_end_ptr - hline_min_ptr);
  }
  else//if (pix_size != 1)
  {
    if (hline_min_ptr < hline_end_ptr)
    {
      memcpy(hline_ptr, color, pix_size);
      hline_ptr += pix_size;
    }//end if (hline_min_ptr < hline_end_ptr)
    size_t sizeToCopy = pix_size;
    while (hline_ptr < hline_end_ptr) {
      memcpy(hline_ptr, hline_min_ptr, sizeToCopy);
      hline_ptr += sizeToCopy;
      sizeToCopy = MIN(2 * sizeToCopy, static_cast<size_t>(hline_end_ptr - hline_ptr));
    }//end while(hline_ptr < hline_end_ptr)
  }//end if (pix_size != 1)
  return dst;
}
static void* blend_line(void* dst, const void* color, int pix_size, size_t pix_num) {
  int a = ((uchar*)color)[3];
  if (255 == a) return set_line(dst, color, pix_size, pix_num);
  int cb = ((uchar*)color)[0], cg = ((uchar*)color)[1], cr = ((uchar*)color)[2], ca = ((uchar*)color)[3];
  uchar* tptr = (uchar*)dst;
  int _cb, _cg, _cr, _ca;
  size_t i;
  switch (pix_size) {
  case 1:
    for (i = 0; i < pix_num; ++i) {
      int _cb = tptr[i];
      _cb += ((cb - _cb)*a + 127) >> 8;
      tptr[i] = (uchar)_cb;
    }
    break;
  case 3:
    for (i = 0; i < pix_num; ++i, tptr += 3) {
      _cb = tptr[0];
      _cg = tptr[1];
      _cr = tptr[2];
      _cb += ((cb - _cb)*a + 127) >> 8;
      _cg += ((cg - _cg)*a + 127) >> 8;
      _cr += ((cr - _cr)*a + 127) >> 8;
      tptr[0] = (uchar)_cb;
      tptr[1] = (uchar)_cg;
      tptr[2] = (uchar)_cr;
    }
    break;
  case 4:
    for (i = 0; i < pix_num; ++i, tptr += 3) {
      _cb = tptr[0];
      _cg = tptr[1];
      _cr = tptr[2];
      _ca = tptr[3];
      _cb += ((cb - _cb)*a + 127) >> 8;
      _cg += ((cg - _cg)*a + 127) >> 8;
      _cr += ((cr - _cr)*a + 127) >> 8;
      _ca += ((ca - _ca)*a + 127) >> 8;
      tptr[0] = (uchar)_cb;
      tptr[1] = (uchar)_cg;
      tptr[2] = (uchar)_cr;
      tptr[3] = (uchar)_ca;
    }
    break;
  default:
    assert(0);
    break;
  }
  return dst;
}
static int draw_font_bitmap(int h, int w, uchar* dst, int dststep, int cn, const uchar* bit, int bitstep, COLOR color) {
  int i, j;
  uchar* c = (uchar*)(&color);
  uchar r = c[0];
  uchar g = c[1];
  uchar b = c[2];
  uchar a = c[3];
  switch (cn) {
  case 1:
    r = (r + (g << 1) + b) >> 2;
    for (i = 0; i < h; ++i) {
      uchar* d = dst + dststep*i;
      const uchar* s = bit + bitstep*i;
      for (j = 0; j < w; ++j) {
        int ss = s[j], s1 = 256 - ss;
        d[j] = (ss*r + d[j] * s1) >> 8;
      }
    }
    break;
  case 3:
    for (i = 0; i < h; ++i) {
      uchar* d = dst + dststep*i;
      const uchar* s = bit + bitstep*i;
      for (j = 0; j < w; ++j, d += cn) {
        int ss = (s[j] * a) >> 8, s1 = 256 - ss;
        d[0] = (ss * r + d[0] * s1) >> 8;
        d[1] = (ss * g + d[1] * s1) >> 8;
        d[2] = (ss * b + d[2] * s1) >> 8;
      }
    }
    break;
  case 4:
    for (i = 0; i < h; ++i) {
      uchar* d = dst + dststep*i;
      const uchar* s = bit + bitstep*i;
      for (j = 0; j < w; ++j, d += cn) {
        int ss = (s[j] * a) >> 8, s1 = 256 - ss;
        d[0] = (ss * r + d[0] * s1) >> 8;
        d[1] = (ss * g + d[1] * s1) >> 8;
        d[2] = (ss * b + d[2] * s1) >> 8;
        d[3] = (ss * b + d[3] * s1) >> 8;
      }
    }
    break;
  default:
    assert(0);
    break;
  }
  return 0;
}

#endif // _STDC_MEM_C_H_
