
typedef struct arr2_t {
  uint h, w; // dims
  union {
#define TYPEDEF(a, b, c, d)  d c[8/sizeof(d)];
#include "../typedef.txt"
#undef TYPEDEF
  };
} arr2_t;
typedef struct var_t {
  union {
    struct {
      uint tt: 24; // base type
      uint ff: 4; // field 0==real 1==complex
      uint nn: 4; // dims  0,1,2,2,3,4
    };
    uint bb;
  };
  union {
#define TYPEDEF(a, b, c, d)  d c;
#include "../typedef.txt"
#undef TYPEDEF
    arr2_t* a2;
  };
} var_t;

int var_free(var_t* va)
{
  if (va->nn && va->a2) {
    GFREE(va->a2);
    memset(va, 0, sizeof(var_t));
  }

  return 0;
}

#define var_set(va, tt1, xx, x)  ((va)->nn=0, (va)->tt=tt1, (va)->xx=x)

int var_printf(const var_t* va)
{
  int tttt = va->tt;
  const char* fmt[] = {"%3d", "%3d", "%5d", "%5d", "%8d", "%8d", "%9.4f", "%9.4f"};

  if (0 == va->nn) {
    switch (tttt) {
#define TYPEDEF(a, b, c, d)  case (a): printf(fmt[a], va->b); break;
#include "typedef.def"
#undef TYPEDEF

    default:
      break;
    }
  }
  else if (2 == va->nn) {
    int i = 0, j = 0, h1 = va->a2->h, w1 = va->a2->w, n = h1 * w1;

    switch (tttt) {
#define TYPEDEF(a, b, c, d)  case (a): for (i=0; i<h1; ++i) {for (j=0; j<w1; ++j) { printf(fmt[a], va->a2->b[i*w1+j]); printf(",");} printf("\n");} break;
#include "typedef.def"
#undef TYPEDEF

    default:
      break;
    }
  }

  return 0;
}
int var_a2_setsize(var_t* va, int tt, int h, int w)
{
  int i = 0;
  var_free(va);
  va->tt = tt;
  va->nn = 2;

  switch (tt) {
#define TYPEDEF(a, b, c, d)  case (a): va->a2 = (arr2_t*)GMALLOC(uchar, sizeof(arr2_t)+h*w*c); for (; i<h*w; ++i) va->a2->b[i]=0; break;
#include "typedef.def"
#undef TYPEDEF

  default:
    break;
  }

  va->a2->h = h;
  va->a2->w = w;
  return 0;
}

int var_cvt_tt(var_t* va, int tt, const var_t* vb)
{
  int tttt = vb->tt * 100 + tt;
#define VAR_CVT(a, b)  a=b
#include "var_cvt_impl.inl"
#undef VAR_CVT
  return 0;
}
int var_add(var_t* va, const var_t* vb)
{
  int tttt = vb->tt * 100 + va->tt;
#define VAR_CVT(a, b)  a+=b
#include "var_cvt_impl.inl"
#undef VAR_CVT
  return 0;
}
int var_sub(var_t* va, const var_t* vb)
{
  int tttt = vb->tt * 100 + va->tt;
#define VAR_CVT(a, b)  a-=b
#include "var_cvt_impl.inl"
#undef VAR_CVT
  return 0;
}
int var_mul(var_t* va, const var_t* vb)
{
  int tttt = vb->tt * 100 + va->tt;
#define VAR_CVT(a, b)  a*=b
#include "var_cvt_impl.inl"
#undef VAR_CVT
  return 0;
}

int var_div(var_t* va, const var_t* vb)
{
  int tttt = vb->tt * 100 + va->tt;
#define VAR_CVT(a, b)  a/=b
#include "var_cvt_impl.inl"
#undef VAR_CVT
  return 0;
}
