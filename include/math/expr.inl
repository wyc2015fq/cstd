
#ifndef _EXPR_INL_
#define _EXPR_INL_

#include "cstd.h"

template <typename T, int N>
struct expr {
  typedef expr<T, N> self;
  enum {size=N};
  struct exprnode_t {
    union { double num; const T* obj; };
    ExprType t;
    void init(ExprType x) {t = x;}
    void init(double x) {num = x; t = CC_EXPR_NUM;}
    void init(const T* x) {obj = x; t = CC_EXPR_OBJ;}
  };
  struct expr_t {
    exprnode_t ops[N];
    int nops;
  };
  expr_t* s;
  ~expr() {
    if (s) FREE(s);}
  //expr() : s(NULL) {}
  expr(ExprType t, const T& a, double b) : s(NULL) {
    s = MALLOC(expr_t, 1);
    s->nops=0;
    s->ops[s->nops++].init(&a);
    s->ops[s->nops++].init(b);
    s->ops[s->nops++].init(t);
  }
  expr(ExprType t, const T& a, const T& b) : s(NULL) {
    s = MALLOC(expr_t, 1);
    s->nops=0;
    s->ops[s->nops++].init(&a);
    s->ops[s->nops++].init(&b);
    s->ops[s->nops++].init(t);
  }
  expr(ExprType t, self& a, const T& b) : s(NULL) {
    T_SWAP(expr_t*, s, a.s);
    ASSERT(s->nops<size);
    s->ops[s->nops++].init(&b);
    s->ops[s->nops++].init(t);
  }
  expr(ExprType t, self& a, double b) : s(NULL) {
    T_SWAP(expr_t*, s, a.s);
    ASSERT(s->nops<size);
    s->ops[s->nops++].init(b);
    s->ops[s->nops++].init(t);
  }
  expr(ExprType t, self& a, self& b) : s(NULL) {
    T_SWAP(expr_t*, s, a.s);
    ASSERT(s->nops<size);
    for (int i=0; i<b.s->nops; ++i) {
      s->ops[s->nops++] = b.s->ops[i];
    }
    s->ops[s->nops++].init(t);
  }
  typedef void (*bin_opt_t)(ExprType t, const T* a, const T* b, double d, T* c);
  void calc(T* o, bin_opt_t bin_opt) const {
    if (s) {
      int i=0, j=0, nst=0;
      ImgExpr::exprnode_t st[size];
      ImgExpr::exprnode_t *ops = s->ops;
      T* ims[size+1] = {o};
      T tmp[size] = {0};
      for (j=0; j<size; ++j) {
        ims[j] = tmp+j;
      }
      ims[j++] = o;
      const T *a, *b;
      T *c;
      double d;
      for (i=0; i<s->nops; ++i) {
        ExprType t = ops[i].t;
        switch (t) {
        case CC_EXPR_SUB:
        case CC_EXPR_ADD:
        case CC_EXPR_MUL:
          ASSERT(CC_EXPR_OBJ==st[nst-1].t);
          ASSERT(CC_EXPR_OBJ==st[nst-2].t);
          b = st[--nst].obj;
          a = st[--nst].obj;
          if (a==o || (unsigned)(a-tmp)<size) {
            ims[j++] = (T*)a; }
          if (b==o || (unsigned)(b-tmp)<size) {
            ims[j++] = (T*)b; }
          c = ims[--j];
          bin_opt(t, a, b, d, c);
          st[nst++].init(c);
          break;
        case CC_EXPR_SUBS:
        case CC_EXPR_ADDS:
        case CC_EXPR_MULS:
          ASSERT(CC_EXPR_NUM==st[nst-1].t);
          ASSERT(CC_EXPR_OBJ==st[nst-2].t);
          d = st[--nst].num;
          a = st[--nst].obj;
          if (a==o || (unsigned)(a-tmp)<size) {
            ims[j++] = (T*)a; }
          c = ims[--j];
          bin_opt(t, a, b, d, c);
          st[nst++].init(c);
          break;
        case CC_EXPR_NUM: st[nst++].init(ops[i].num); break;
        case CC_EXPR_OBJ: st[nst++].init(ops[i].obj); break;
        default: ASSERT(0); break;
        }
      }
      ASSERT(1==nst);
      if (o!=st[0].obj) {
        bin_opt(CC_EXPR_COPY, st[0].obj, NULL, 0, o);
      }
      for (i=0; i<size; ++i) {
        bin_opt(CC_EXPR_FREE, NULL, NULL, 0, tmp+i);
      }
    }
    return ;
  }
};

//#include "DijkstraEvalute.inl"

#endif // _EXPR_INL_
