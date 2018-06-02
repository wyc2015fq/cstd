#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MIN(a, b) ((a)<(b)?(a):(b))
#define MAX(a, b) ((a)>(b)?(a):(b))
#define SWAP(a, b, t) (t=a,a=b,b=t)
#if 0
#define BIT   unsigned int
#define BITMAX  (10000)
#define BITADD(H, L, A, B, U)  (L=(A+B+U)%BITMAX, H=L<A)
#define BITSUB(H, L, A, B, U)  (L=(A+B+U)%BITMAX, H=L>A)
#define BITMUL(H, L, A, B, U)  (L+=(A*B), H+=L/BITMAX, L%=BITMAX)
#define STR2BIT_L (4)
#define STR2BIT(B, S, L, M, N)  {int I=0;B=0;for (; I<L; ++I) {B=B*N+M[((S)[I])];} }
#else
#define DBIT   32
#define HBIT   (DBIT>>1)
#define BIT   unsigned int
#define WBIT   unsigned __int64
#define BITMAX  (1<<DBIT)
#define BITMASK (~((BIT)0))
#define BITADD(H, L, A, B, U)  {BIT T=A+B+U; H=T<A; L=T;}
#define BITSUB(H, L, A, B, U)  {BIT T=A-B-U; H=T>A; L=T;}
#define BITHI(A)  ((A)>>HBIT)
#define BITLO(A)  ((A)&(BITMASK>>HBIT))
//B输出,S字符串,M映射,N进制
#define STR2BIT_L (8)
#define STR2BIT(B, S, L, M, N)  {int I=0;B=0;for (; I<L; ++I) {B=B*N+M[((S)[I])];} }
BIT karatsuba(BIT* h, BIT* l, BIT a, BIT b)
{
  BIT ah = BITHI(a), al = BITLO(a);
  BIT bh = BITHI(b), bl = BITLO(b);
  BIT ch = ah * bh, cl = al * bl, tt, u;
  tt = ch + cl - (ah - al) * (bh - bl);
  BITADD(*h, *l, cl, (tt << HBIT), *l);
  BITADD(u, *h, ch, (tt >> HBIT), *h);
  return u;
}
BIT karatsuba2(BIT* h, BIT* l, BIT a, BIT b)
{
  BIT ah = BITHI(a), al = BITLO(a);
  BIT bh = BITHI(b), bl = BITLO(b);
  BIT ch = ah * bh, cl = al * bl, tt, u;
  tt = ch + cl - (ah - al) * (bh - bl);
  BITSUB(*h, *l, *l, cl, (tt << HBIT));
  BITSUB(u, *h, *h, ch, (tt >> HBIT));
  return u;
}
//hl=a*r+q
// 输入hla 求rq
BIT karaDiv(BIT h, BIT l, BIT a, BIT* r, BIT* q)
{
#if 1
  BIT ah = BITHI(a), al = BITLO(a);
  BIT ch, cl, hl, u = 0;
  ch = h / (ah + !!al);
  h -= (ch * ah);
  hl = (h << HBIT) | (l >> HBIT);
  u = hl - ch * al;
  h = u >> HBIT;
  l = (u << HBIT) | BITLO(l);
  hl = (h << HBIT) | (l >> HBIT);
  cl = (hl) / (ah + !!al);
  u = hl - cl * ah;
  h = u >> HBIT;
  l = (u << HBIT) | BITLO(l);
  l -= cl * al;
  assert(h == 0);
  if (l >= a) {
    l -= a, ++cl;
    if (0 == cl) {
      ++ch;
    }
  }
  assert(l <= a);
  *r = (ch << 16) + cl;
  *q = l;
#else
  unsigned __int64 hl = h;
  hl = hl << 32 | l;
  *r = hl / a, *q = hl % a;
#endif
  return 0;
}
//#define BITMUL(H, L, A, B, U)  (karatsuba(&(H), &(L), A, B))
#define BITMUL(H, L, A, B, U)  {WBIT tt=A; tt*=B, H=(BIT)(tt>>DBIT), L+=(BIT)tt;}
//A*B+C
#define BITOP(H, L, OP)  {WBIT tt=(WBIT)OP; H=(BIT)(tt>>DBIT), L=(BIT)tt;}
#define BITMULADD(H, L, A, B, C)  {WBIT tt=A; tt=tt*B+C, H=(BIT)(tt>>DBIT), L=(BIT)tt;}
#define BITMULSUB(H, L, A, B, C)  {WBIT tt=A; tt=tt*B-C, H=(BIT)(tt>>DBIT), L=(BIT)tt;}
#endif
#define FILL(N, A, X) {int I=0; for (; I<(N); ++I) (A)[I]=X; }
#define COPY(N, A, B) {int I=0; for (; I<(N); ++I) (A)[I]=(B)[I]; }
#define MKWBIT(H, L)  (((H)<<32)|(L))
typedef struct BIGU {
  int l;
  BIT* s;
} BIGU;
#define CMP(a, b) (((a) > (b)) - ((a) < (b)))
void* clone(void* a, int an, int grow)
{
  void* b = malloc(an + grow);
  assert(grow >= 0);
  memset(b, 0, an + grow);
  if (a) {
    memcpy(b, a, an);
  }
  return b;
}
#define bigu_clone(a, an, grow)  ((BIT*)clone(a, (an)*sizeof(BIT), (grow)*sizeof(BIT)))
int bigu_cmp_s(int an, BIT* a, int bn, BIT* b)
{
  int i, ret = CMP(an, bn);
  for (i = an; ret == 0 && i-- > 0;) {
    ret = CMP(a[i], b[i]);
  }
  return ret;
}
// c=a+b+u
int bigu_add_s(int an, BIT* a, int bn, BIT* b, BIT u, BIT* y, int* signy)
{
  int i, yn;
  if (an < bn) {
    BIT* t;
    SWAP(a, b, t);
    SWAP(an, bn, i);
  }
  for (i = 0; i < bn; i++) {
    //BITOP(u, y[i], a[i]+b[i]+u);
    BITADD(u, y[i], a[i], b[i], u);
  }
  for (; i < an; i++) {
    //BITOP(u, y[i], a[i]+u);
    BITADD(u, y[i], a[i], 0, u);
  }
  y[i] = u;
  yn = an + !!u;
  *signy = yn > 0;
  return yn;
}
int bigu_abssub_s(int an, BIT* a, int bn, BIT* b, BIT u, BIT* y)
{
  int i, yn = an;
  for (i = 0; i < bn; i++) {
    BITSUB(u, y[i], a[i], b[i], u);
    //BITOP(u, y[i], a[i]-b[i]-!!u);
  }
  for (; i < an; i++) {
    BITSUB(u, y[i], a[i], 0, u);
    //BITOP(u, y[i], a[i]-!!u);
  }
  assert(0 == u);
  yn = an;
  while (!y[yn - 1]) {
    --yn;
  }
  return yn;
}
// c=a-b+u
int bigu_sub_s(int an, BIT* a, int bn, BIT* b, BIT u, BIT* y, int* signy)
{
  int i, yn = an;
  *signy = bigu_cmp_s(an, a, bn, b);
  if (*signy < 0) {
    BIT* t;
    SWAP(a, b, t);
    SWAP(an, bn, i);
  }
  u = 0;
  for (i = 0; i < bn; i++) {
    BITSUB(u, y[i], a[i], b[i], u);
    //BITOP(u, y[i], a[i]-b[i]-!!u);
  }
  for (; i < an; i++) {
    BITSUB(u, y[i], a[i], 0, u);
    //BITOP(u, y[i], a[i]-!!u);
  }
  assert(0 == u);
  yn = an;
  while (!y[yn - 1]) {
    --yn;
  }
  return yn;
}
#define SHL(H, L, A, I)  (L|=(A<<I), H|=(A>>(DBIT-I)))
int bigu_shl(int an, BIT* a, BIT* b, int shift)
{
  int s = shift % DBIT;
  int i, j = shift / DBIT;
  if (s > 0) {
    for (i = 0; i < an; ++i) {
      SHL(b[j + 1], b[j], a[i], s);
    }
  }
  else {
    --j;
    s += 32;
    i = -j;
    b[0] = a[i - 1] >> (DBIT - s);
    for (; i < an; ++i) {
      SHL(b[j + 1], b[j], a[i], s);
    }
  }
  return 0;
}
#define BITDIV(H, L, D, Q, R) {WBIT T=((WBIT)H<<DBIT)|L; WBIT QQ=(BIT)(T/(D)), RR=L-(D)*QQ; Q=(BIT)QQ, R=(BIT)RR;}
int div_word(int an, BIT* a, BIT b, BIT* q, BIT* r)
{
  int i = an;
  *r = 0;
  for (; i-- > 0;) {
    BITDIV(*r, a[i], b, q[i], *r);
  }
  return an - !q[an - 1];
}
//a*b+u
int mul_word(int an, BIT* a, BIT b, BIT u, BIT* c)
{
  int i = 0;
  for (; i < an; ++i) {
    BITMULADD(u, c[i], b, a[i], u);
  }
  c[an] = u;
  return an + !!c[an];
}
int BIGU2str(char* s, int l, int an, BIT* a)
{
  int i, k = 8, qn = an;
  char* ss = s + l - 1;
  static const char* nu = "0123456789";
  BIT b = (BIT)pow(10, k), r = 0;
  BIT* q = (BIT*)malloc(qn * sizeof(BIT));
  memcpy(q, a, an * sizeof(BIT));
  *ss = 0;
  while (qn) {
    qn = div_word(qn, q, b, q, &r);
    if (qn) {
      for (i = 0; i < k; ++i) {
        *--ss;
        assert(ss >= s);
        *ss = nu[r % 10];
        r /= 10;
      }
    }
    else {
      for (i = 0; r; ++i) {
        *--ss;
        assert(ss >= s);
        *ss = nu[r % 10];
        r /= 10;
      }
    }
  }
  strcpy(s, ss);
  free(q);
  return s + l - 1 - ss;
}
int str2BIGU(const char* s, int l, BIT* a)
{
  BIT b, p;
  char map[128];
  const char* nu = "0123456789";
  const char* lo = "abcdef";
  const char* hi = "ABCDEF";
  int i, an;
  memset(map, 0, 128);
  for (i = 0; i < 10; ++i) {
    map[nu[i]] = i;
  }
  for (i = 0; i < 6; ++i) {
    map[lo[i]] = i + 10;
    map[hi[i]] = i + 10;
  }
  an = 2 + l / STR2BIT_L;
  if (a) {
    an = 0;
    for (i = 0; i < l; i += STR2BIT_L) {
      int k = MIN(l, i + STR2BIT_L) - i;
      STR2BIT(b, s + i, k, map, 10);
      p = (BIT)pow(10, k);
      an = mul_word(an, a, p, b, a);
    }
  }
  return an;
}
//y=a*b
int bigu_mul_s(int an, BIT* a, int bn, BIT* b, BIT* y)
{
  int i, j, yn = an + bn;
  BIT bj, u;
  if (an < bn) {
    BIT* t;
    SWAP(a, b, t);
    SWAP(an, bn, i);
  }
  FILL(yn, y, 0);
  if (1 == bn) {
    return mul_word(an, a, b[0], 0, y);
  }
  for (j = 0; j < bn; ++j) {
    BIT* yj = y + j;
    bj = b[j], u = 0;
    for (i = 0; i < an; ++i) {
      BITOP(u, yj[i], bj * a[i] + u + yj[i]);
    }
    BITOP(u, yj[an], u + yj[an]);
    assert(0 == u);
  }
  while (yn > 0 && 0 == y[yn - 1]) {
    --yn;
  }
  return yn;
}
//a*b = (a1*b1)*B^2n + (a1*b0)*B^n + (a0*b1)*B^n + (a0*b0)
//  = (a0*b0) + (a1*b1)*B^2n + (a1*b1 + a0*b0 + (a1-a0)(b0-b1))*B^n
int bigu_axpy(int an, BIT* a, int bn, BIT* b, int cn, BIT* c, int signa, int signc, BIT* y, int* psigny)
{
  int yn = MAX(an + bn, cn) + 1;
  if (NULL == y) {
    return yn;
  }
  yn = bigu_mul_s(an, a, bn, b, y);
  if (signc == signa) {
    yn = bigu_add_s(yn, y, cn, c, 0, y, psigny);
  }
  else {
    yn = bigu_sub_s(yn, y, cn, c, 0, y, psigny);
  }
  *psigny *= signa;
  return yn;
}
int bigu_mulsub_n(int an, BIT* a, int bn, BIT* b, BIT q, BIT* y)
{
  int i = 0;
  BIT h = 0, l = 0, u = 0;
  assert(bn < an);
  for (; i < bn; ++i) {
    WBIT tt = (WBIT)b[i] * q + h;
    h = (BIT)(tt >> 32), l = (BIT)tt;
    BITSUB(u, y[i], a[i], l, u);
  }
  BITSUB(u, y[i], a[i], h, u);
  ++i;
  for (; i < an;) {
    BITSUB(u, y[i], a[i], h, u);
  }
  return 0;
}
int bigu_cmp_shift(int n, BIT* a, BIT* b, int k)
{
  int i = n, kk = 32 - k;
  BIT u = 0, bi;
  if (0 == k) {
    return bigu_cmp_s(n + (a[n] > 0), a, n, b);
  }
  for (; i-- > 0;) {
    bi = u | (b[i] >> kk);
    if (a[i + 1] != bi) {
      return CMP(a[i + 1], bi);
    }
    u = b[i] << k;
  }
  if (a[0] != u) {
    return CMP(a[0], u);
  }
  return 0;
}
int bigu_sub_mul(int n, BIT* a, BIT* b, BIT q)
{
  int i;
  BIT h = 0, l = 0, u = 0;
  if (q) {
    WBIT tt;
    for (i = 0; i < n; ++i) {
      tt = (WBIT)b[i] * q + h;
      h = (BIT)(tt >> 32), l = (BIT)tt;
      BITSUB(u, a[i], a[i], l, u);
    }
  }
  else {
    for (i = 0; i < n; ++i) {
      BITSUB(u, a[i], a[i], b[i], u);
    }
  }
  BITSUB(u, a[i], a[i], h, u);
  return 0;
}
int bigu_sub_shift(int n, BIT* a, BIT* b, int k)
{
  int i, kk = 32 - k;
  BIT u = 0, bi, uu = 0;
  if (0 == k) {
    for (i = 0; i < n; ++i) {
      BITSUB(uu, a[i], a[i], b[i], uu);
    }
  }
  else {
    for (i = 0; i < n; ++i) {
      bi = u | (b[i] << k);
      BITSUB(uu, a[i], a[i], bi, uu);
      u = b[i] >> kk;
    }
  }
  bi = u;
  BITSUB(uu, a[i], a[i], bi, uu);
  return 0;
}
int bigu_lsh(int an, BIT* a, BIT* b, int k)
{
  int i, r = 32 - k;
  if (0 == k) {
    COPY(an, b, a);
  }
  else {
    BIT u = 0, ai;
    for (i = 0; i < an; ++i) {
      ai = a[i];
      b[i] = u | (ai << k);
      u = ai >> r;
    }
    if (u) {
      b[an++] = u;
    }
  }
  return an;
}
int bigu_rsh(int an, BIT* a, BIT* b, int k)
{
  int i, r = 32 - k;
  if (0 == k) {
    COPY(an, b, a);
  }
  else {
    BIT u = a[0] >> k, ai;
    for (i = 1; i < an; ++i) {
      ai = a[i];
      b[i - 1] = u | (ai << r);
      u = ai >> k;
    }
    if (u) {
      b[i - 1] = u;
      ++i;
    }
    an = i - 1;
  }
  return an;
}
BIT bigu_div_sub(int n, BIT* a, BIT* b)
{
  BIT q0 = 0, qq = 0, rr;
  WBIT d = (WBIT)b[n - 1] + 1;
  //{int j=0; utime_start(_start_time);
  for (; 1;) {
    BITDIV(a[n], a[n - 1], d, qq, rr);
    //qq=trydiv6(n, a, b);
    if (0 == qq) {
      break;
    }
    q0 += qq;
    bigu_sub_mul(n, a, b, qq);
    //printf("qq=%u\n", qq);
    //++j;
  }
  //printf("bbb=%2d %f\n", j, utime_elapsed(_start_time));}
  return q0;
}
BIT bigu_div_shift(int n, BIT* a, BIT* b)
{
  int i = 32;
  BIT q0 = 0;
  q0 = bigu_div_sub(n, a, b);
  //{int j=0; utime_start(_start_time);
  for (; i-- > 0;) {
    if (bigu_cmp_shift(n, a, b, i) >= 0) {
      bigu_sub_shift(n, a, b, i);
      q0 += (1 << i);
      //++j;
    }
  }
  //printf("aaa=%2d %f\n", j, utime_elapsed(_start_time));}
  return q0;
}
#define bigu_mod(AN, A, BN, B, R) bigu_div(AN, A, BN, B, 0, 0, R)
int bigu_div(int an, BIT* a, int bn, BIT* b, int* pqn, BIT* q, BIT* r)
{
  int i, j = an - bn, qn = an - bn + 1, rn = bn;
  assert(j >= 0);
  r[bn - 1] = 0;
  for (i = 0; i < bn - 1; ++i) {
    r[i] = a[j + i + 1];
  }
  if (q) {
    for (; j >= 0; --j) {
      for (i = bn; i-- > 0;) {
        r[i + 1] = r[i];
      }
      r[0] = a[j];
      q[j] = bigu_div_shift(bn, r, b);
    }
    while (qn > 0 && 0 == q[qn - 1]) {
      --qn;
    }
    if (pqn) {
      *pqn = qn;
    }
  }
  else {
    for (; j >= 0; --j) {
      for (i = bn; i-- > 0;) {
        r[i + 1] = r[i];
      }
      r[0] = a[j];
      bigu_div_shift(bn, r, b);
    }
  }
  while (rn > 0 && 0 == r[rn - 1]) {
    --rn;
  }
  return rn;
}
//a[an] 必须==0;
int bigu_div2(int an, BIT* a, int bn, BIT* b, int* pqn, BIT* q)
{
  int j = an - bn, qn = an - bn + 1, rn = bn;
  assert(j >= 0);
  if (q) {
    for (; j >= 0; --j) {
      q[j] = bigu_div_shift(bn, a + j, b);
    }
    while (qn > 0 && 0 == q[qn - 1]) {
      --qn;
    }
    if (pqn) {
      *pqn = qn;
    }
  }
  else {
    for (; j >= 0; --j) {
      bigu_div_shift(bn, a + j, b);
    }
  }
  while (rn > 0 && 0 == a[rn - 1]) {
    --rn;
  }
  return rn;
}
int count0bit(BIT sh)
{
  int k = 0;
  while ((sh & (1 << 31)) == 0) {
    sh <<= 1;
    k++;
  }
  return k;
}
int bigu_div3(int an, BIT* a, int bn, BIT* b, int* pqn, BIT* q, BIT* r)
{
  int j = an - bn, rn = bn;
  int k = count0bit(b[bn - 1]);
  assert(k < 32);
  a = bigu_clone(a, an, 2);
  b = bigu_clone(b, bn, 1);
  an = bigu_lsh(an, a, a, k);
  bn = bigu_lsh(bn, b, b, k);
  rn = bigu_div2(an, a, bn, b, pqn, q);
  if (r) {
    rn = bigu_rsh(rn, a, r, k);
  }
  free(a);
  free(b);
  return rn;
}
#define bigu_sqr(AN, A, X) bigu_mul_s(AN, A, AN, A, X)
#define bigu_copy(AN, A, B) (memcpy(B, A, sizeof(A[0])*AN), AN)
#define bigu_new(N)  bigu_clone(0, N, 0)
// x=a^n
int bigu_pow(int an, BIT* a, int n, BIT* x)
{
  int xn = 1, tn;
  int i = 32; //count0bit(a);
  BIT* t = bigu_new(an * n);
  x[0] = 1;
  for (; i-- > 0;) {
    tn = bigu_sqr(xn, x, t);
    if (n & (1 << i)) {
      xn = bigu_mul_s(tn, t, an, a, x);
    }
    else {
      xn = bigu_copy(tn, t, x);
    }
  }
  free(t);
  return xn;
}
//x*x+r=a r<2*x+1
int bigu_sqrt(int an, BIT* a, BIT* x)
{
  int xn = an, qn, rn, tn, tsign;
  BIT* q = bigu_new(an);
  BIT* r = bigu_new(an);
  BIT* t = bigu_new(an);
  int i = 0;
#if 0
  int k = (an * 32 - count0bit(a[an - 1])) / 2 - 1;
  xn = bigu_rsh(an - k / 32, a + k / 32, x, k % 32);
#else
  int k = (an * 32 - count0bit(a[an - 1])) / 2 + 1;
  xn = k / 32 + 1;
  x[xn - 1] = 1 << (k % 32);
#endif
  for (; 1;) {
    rn = bigu_div3(an, a, xn, x, &qn, q, r);
    tn = bigu_add_s(xn, x, qn, q, 0, t, &tsign);
    tn = bigu_rsh(tn, t, t, 1);
    ++i;
    if (bigu_cmp_s(tn, t, xn, x) >= 0) {
      printf(" %3d ", i);
      break;
    }
    COPY(tn, x, t);
    xn = tn;
  }
  free(q);
  free(r);
  free(t);
  return xn;
}
int bigu_gcd(int an, BIT* a, int bn, BIT* b, BIT* r)
{
  int i;
  BIT* t;
  for (; bn;) {
    an = bigu_div2(an, a, bn, b, 0, 0);
    SWAP(an, bn, i);
    SWAP(a, b, t);
  }
  COPY(an, r, b);
  return an;
}
int bigu_gcd2(int an, BIT* a, int bn, BIT* b, BIT* r)
{
  a = bigu_clone(a, an, 1);
  b = bigu_clone(b, bn, 1);
  an = bigu_gcd(an, a, bn, b, r);
  free(a);
  free(b);
  return an;
}
typedef struct BIGS {
  int n, s;
  BIT* p;
} BIGS;
//扩展欧几里德算法求 gcd(a,b)=a*x+b*y=z整数解
int bigu_xgcd(int an, BIT* a, int bn, BIT* b, int* pxn, BIT* x, int* pyn, BIT* y, int* pzn, BIT* z)
{
  int i, qn;
  BIT* q, *tt;
  BIT* u1, *u2, *v1, *v2, *t1, *t2;
  int u1n, u2n, v1n, v2n, t1n, t2n;
  int u1s, u2s, v1s, v2s, t1s, t2s;
  q = bigu_new(an - bn + 1);
  u1 = bigu_new(an + 1);
  u1n = 1;
  u1s = 1;
  u1[0] = 1;
  u2 = bigu_new(an + 1);
  u2n = 0;
  u2s = 0;
  v1 = bigu_new(an + 1);
  v1n = 0;
  v1s = 0;
  v2 = bigu_new(an + 1);
  v2n = 1;
  v2s = 1;
  v2[0] = 1;
  t1 = bigu_new(an + 1);
  t1n = 0;
  t1s = 0;
  t2 = bigu_new(an + 1);
  t2n = 0;
  t1s = 0;
  for (; bn;) {
    an = bigu_div2(an, a, bn, b, &qn, q);
    t1n = bigu_axpy(v1n, v1, qn, q, u1n, u1, -v1s, u1s, t1, &t1s);
    t2n = bigu_axpy(v2n, v2, qn, q, u2n, u2, -v2s, u2s, t2, &t2s);
    SWAP(an, bn, i);
    SWAP(a, b, tt);
  }
  COPY(an, z, a);
  return an;
#undef SET
  return 0;
}
int fact2(BIT a)
{
  int k = 0;
  for (; 0 == (a & (1 << k)); ++k);
  return k;
}
int bigu_fact2(int an, BIT* a)
{
  int i = 0, k = 0;
  for (; i < an && 0 == a[i]; ++i);
  k = i * DBIT;
  if (i < an) {
    k += fact2(a[i]);
  }
  return k;
}
int bigu_bingcd(int an, BIT* a, int bn, BIT* b)
{
  int i, t, sig, ka, kb;
  for (; ;) {
    t = bigu_cmp_s(an, a, bn, b);
    if (0 == t) {
      break;
    }
    ka = bigu_fact2(an, a);
    kb = bigu_fact2(bn, b);
    i += MIN(ka, kb);
    an = bigu_rsh(an, a, a, ka);
    bn = bigu_rsh(bn, b, b, kb);
    t = bigu_cmp_s(an, a, bn, b);
    if (0 == t) {
      break;
    }
    if (t > 0) {
      an = bigu_sub_s(an, a, bn, b, 0, a, &sig);
    }
    else {
      bn = bigu_sub_s(bn, b, an, a, 0, b, &sig);
    }
  }
  return an;
}

