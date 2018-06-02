#ifndef _MP_H_
#define _MP_H_
#include "cstd.h"
#include "str.h"
//typedef unsigned int mpbase_t;
#if 1
typedef unsigned int mpbase_t;
typedef unsigned __int64 mpbase_tt;
#else
typedef unsigned short mpbase_t;
typedef unsigned int mpbase_tt;
#endif
#define MP_MASK     ((mpbase_t)-1)
#define MPBITS      (sizeof(mpbase_t) * 8)
#define HALF_MPBITS (MPBITS / 2)
#define HALF_BASE    (((mpbase_t)1)<<(MPBITS-1))
#define MPL_MASK    ((mpbase_t)(((mpbase_t)1) << HALF_MPBITS) - 1)
//typedef unsigned char mpbase_t;
typedef struct mpz_t {
  int len;
  int flag;
  mpbase_t* data;
} mpz_t;
#define MPZN_N  16
typedef struct mpzn_t {
  int flag;
  mpbase_t data[MPZN_N];
} mpzn_t;
#define MPZSWAPP(a, b)  if (a->len<b->len) {const mpz_t* _tp;SWAP(a, b, _tp);}
int mpz_free(mpz_t* a)
{
  GFREE(a->data);
  a->len = 0;
  return 0;
}
int mpz_frees(mpz_t* a, int n)
{
  int i;
  for (i = 0; i < n; ++i) {
    mpz_free(a + i);
  }
  return 0;
}
int mpz_setsize(mpz_t* a, int n)
{
  GREALLOC(mpbase_t, a->data, n);
  MEMSET(a->data, 0, n);
  a->len = n;
  return 0;
}
int mpz_setzero(mpz_t* a)
{
  a->len = 0;
  return 0;
}
int mpz_setint(mpz_t* a, mpbase_t b)
{
  if (0 == b) {
    a->len = 0;
    return 0;
  }
  mpz_setsize(a, 1);
  a->data[0] = b;
  return 0;
}
int mpz_swap(mpz_t* a, mpz_t* b)
{
  mpz_t t;
  SWAP(*a, *b, t);
  return 0;
}
int mpz_adjustlen(mpz_t* c)
{
  int len = c->len;
  while (len > 0 && 0 == c->data[len - 1]) {
    len--;
  }
  c->len = len;
  return len;
}
int mpz_adjust_free(mpz_t* cc, mpz_t* c)
{
  mpz_adjustlen(cc), mpz_swap(cc, c), mpz_free(cc);
  return 0;
}
mpbase_t mps_cpy(const mpbase_t* a, mpbase_t* c, int n)
{
  memcpy(c, a, n * sizeof(mpbase_t));
  return 0;
}
int mps_cmp(const mpbase_t* a, const mpbase_t* b, int an, int bn)
{
  int i, t;
  for (; 0 == a[an - 1]; --an);
  for (; 0 == b[bn - 1]; --bn);
  t = CC_CMP(an, bn);
  if (0 == t) {
    for (i = an - 1; i >= 0 && 0 == t; --i) {
      t = CC_CMP(a[i], b[i]);
    }
  }
  return t;
}
int mpz_clone(const mpz_t* a, mpz_t* b)
{
  mpz_setsize(b, a->len);
  mps_cpy(a->data, b->data, a->len);
  return 0;
}
int calc_pi()
{
  long a = 10000, b, c = 2800, d, e, f[2801], g;
  for (; b - c;) {
    f[b++] = a / 5;
  }
  for (; d = 0, g = c * 2; c -= 14, printf("%.4d", e + d / a), e = d % a) {
    for (b = c; d += f[b] * a, f[b] = d % --g, d /= g--, --b; d *= b);
  }
  return 0;
}
mpbase_t mps_adds(const mpbase_t* a, mpbase_t b, mpbase_t* c, int n)
{
  int i;
  mpbase_t t = b;
  for (i = 0; i < n && t; ++i) {
    c[i] = a[i] + t;
    t = c[i] < t;
  }
  mps_cpy(a + i, c + i, n - i);
  return (mpbase_t)t;
}
mpbase_t mps_add(const mpbase_t* a, const mpbase_t* b, mpbase_t* c, int n)
{
  int i;
  mpbase_t t = 0, aa;
  for (i = 0; i < n; ++i) {
    aa = a[i] + t;
    c[i] = aa + b[i];
    t = (c[i] < aa) + (aa < t);
  }
  return t;
}
mpbase_t mps_subs(const mpbase_t* a, mpbase_t t, mpbase_t* c, int n)
{
  int i;
  mpbase_t aa;
  for (i = 0; i < n && t; ++i) {
    aa = a[i];
    c[i] = aa - t;
    t = aa < t;
  }
  mps_cpy(a + i, c + i, n - i);
  return t;
}
// d=a-b*c
mpbase_t mps_sub_mul(const mpbase_t* a, const mpbase_t* b, mpbase_t c, mpbase_t* d, int n)
{
  int i;
  mpbase_tt cc = c, cb = 0;
  mpbase_t t = 0, aa;
  for (i = 0; i < n; ++i) {
    cb += cc * b[i];
    aa = a[i];
    t = (mpbase_t)(cb & MP_MASK);
    d[i] = aa - t;
    cb = (cb >> MPBITS) + (d[i] > aa);
  }
  return (mpbase_t)cb;
}
mpbase_t mps_sub(const mpbase_t* a, const mpbase_t* b, mpbase_t* c, int n)
{
  int i;
  mpbase_t t = 0, aa;
  for (i = 0; i < n; ++i) {
    aa = a[i];
    c[i] = aa - b[i] - t;
    t = c[i] > aa || (t && c[i] == aa);
  }
  return t;
}
int mps_absadd(const mpbase_t* a, const mpbase_t* b, mpbase_t* c, int an, int bn)
{
  int n;
  const mpbase_t* tptr;
  mpbase_t t;
  if (an < bn) {
    SWAP(a, b, tptr);
    SWAP(an, bn, n);
  }
  t = mps_add(a, b, c, bn);
  t = mps_adds(a + bn, t, c + bn, an - bn);
  if (t) {
    c[an] = t;
  }
  return 0;
}
int mps_abssub(const mpbase_t* a, const mpbase_t* b, mpbase_t* c, int an, int bn)
{
  int s, n;
  const mpbase_t* tptr;
  mpbase_t t;
  s = mps_cmp(a, b, an, bn);
  if (s < 0) {
    SWAP(a, b, tptr);
    SWAP(an, bn, n);
  }
  t = mps_sub(a, b, c, MIN(an, bn));
  if (an < bn) {
    ASSERT(0 == t);
    SWAP(a, b, tptr);
    SWAP(an, bn, n);
  }
  t = mps_subs(a + bn, t, c + bn, an - bn);
  ASSERT(0 == t);
  return s;
}
mpbase_t mps_muls(const mpbase_t* a, mpbase_t b, mpbase_t* c, int n)
{
  int i;
  mpbase_tt t = 0, bb = b;
  for (i = 0; i < n; ++i) {
    t += bb * a[i];
    c[i] = (mpbase_t)(t & MP_MASK);
    t >>= MPBITS;
  }
  return (mpbase_t)t;
}
// 返回进位
// d = [a, n] * b + c
mpbase_t mps_muls_add(const mpbase_t* a, mpbase_t b, mpbase_t c, mpbase_t* d, int n)
{
  int i;
  mpbase_tt t = c, bb = b;
  for (i = 0; i < n; ++i) {
    t += bb * a[i];
    d[i] = (mpbase_t)(t & MP_MASK);
    t >>= MPBITS;
  }
  return (mpbase_t)t;
}
int mps_mul(const mpbase_t* a, const mpbase_t* b, mpbase_t* c, int an, int bn)
{
  int i, j;
  mpbase_t hh = 0, ll = 0;
  for (i = 0; i < an; ++i) {
    mpbase_tt t = 0, aa = a[i];
    for (hh = 0, j = 0; j < bn; ++j) {
      t += aa * b[j] + c[i + j];
      c[i + j] = (mpbase_t)(t & MP_MASK);
      t >>= MPBITS;
    }
    for (; t; ++j) {
      t += c[i + j];
      c[i + j] = (mpbase_t)(t & MP_MASK);
      t >>= MPBITS;
    }
  }
  return 0;
}
//shift left
// c = a<<b;
// (0 < b < 32)
mpbase_t mps_shl(const mpbase_t* a, mpbase_t b, mpbase_t* c, int n)
{
  int i, r = MPBITS - b;
  mpbase_t carry = 0, t;
  if (0 == b) {
    return mps_cpy(a, c, n);
  }
  for (i = 0; i < n; ++i) {
    t = a[i];
    carry |= t << b;
    c[i] = carry;
    carry = t >> r;
  }
  return carry;
}
mpbase_t mps_shr(const mpbase_t* a, mpbase_t b, mpbase_t* c, int n)
{
  int i, r = MPBITS - b;
  mpbase_t carry = 0, t;
  if (0 == b) {
    return mps_cpy(a, c, n);
  }
  for (i = n - 1; i >= 0; --i) {
    t = a[i];
    carry |= t >> b;
    c[i] = carry;
    carry = t << r;
  }
  return carry;
}
mpbase_t mps_divs(const mpbase_t* a, mpbase_t b, mpbase_t* q, int n)
{
  int i;
  mpbase_tt t1 = 0, q1;
  for (i = n - 1; i >= 0; --i) {
    t1 = (t1 << MPBITS) | a[i];
    q1 = t1 / b;
    t1 -= b * q1;
    q[i] = (mpbase_t)q1;
  }
  return (mpbase_t)t1;
}
// 如果 a==NULL 返回 an
int mps_setstr(mpbase_t* a, const char* s, int l, int base)
{
  int i, len, j = 0;
  int u = (int)log(MP_MASK) / log(base);
  const mpbase_t max_base = MP_MASK / base;
  len = l / u + 1;
  if (a) {
    for (i = 0; i < l;) {
      mpbase_t x = 0, b = 1;
      for (; i < l && b < max_base; ++i, b *= base) {
        x = x * 10 + (s[i] - '0');
      }
      a[j] = mps_muls_add(a, b, x, a, j);
      j += a[j] > 0;
    }
    len = j;
  }
  return len;
}
int mps_tostr(const mpbase_t* a, int an, str_t* s)
{
  mpbase_t r;
  int i, smaxlen;
  int u = (int)log10(MP_MASK);
  mpbase_t base10 = (mpbase_t)pow(10, u);
  char fmt[32] = "%";
  char buf[32];
  int slen;
  mpbase_t* aa = (mpbase_t*)memdup(a, an * sizeof(mpbase_t));
  _snprintf(fmt + 1, 30, "0%dd", u);
  slen = smaxlen = (an + 1) * (u + 1);
  str_setsize(s, smaxlen);
  while (an > 0) {
    r = mps_divs(aa, base10, aa, an);
    for (; 0 == aa[an - 1]; --an);
    _snprintf(buf, 32, fmt, r);
    smaxlen -= u;
    memcpy(s->s + smaxlen, buf, u);
  }
  ASSERT(smaxlen >= 0);
  for (i = smaxlen; i < s->l && s->s[i] == '0'; ++i);
  str_del(s, 0, i);
  GFREE(aa);
  return 0;
}
int mps_print(const mpbase_t* a, int an)
{
  str_t s[1] = {0};
  mps_tostr(a, an, s);
  printf("%s\n", s->s);
  str_free(s);
  return 0;
}
// [q, r] = a / b
int mps_div(const mpbase_t* a, const mpbase_t* b, mpbase_t* q, mpbase_t* r, int an, int bn)
{
  int shift = 0;
  mpbase_t half_base = HALF_BASE;
  int i, k;
  int qn = an - bn + 1;
  mpbase_tt t1 = 0, q1, b1;
  mpbase_t sh = b[ bn - 1 ];
  mpbase_t* aa = GMALLOC(mpbase_t, an + 2);
  mpbase_t* bb = GMALLOC(mpbase_t, bn + 1);
  MEMSET(aa, 0, an + 2);
  MEMSET(bb, 0, bn + 1);
  while (!(sh & half_base)) {
    sh <<= 1;
    shift++;
  }
  aa[an] = mps_shl(a, shift, aa, an);
  mps_shl(b, shift, bb, bn);
  b1 = bb[bn - 1];
  t1 = 0;
  ++b1;
  for (i = an; i >= bn - 1; --i) {
    t1 = (t1 << MPBITS) | aa[i];
    q1 = t1 / b1;
    k = i + 1 - bn;
    if (q1) {
      mps_sub_mul(aa + k, bb, (mpbase_t)q1, aa + k, bn + 1);
    }
    while (mps_cmp(aa + k, bb, bn + 1, bn + 1) >= 0) {
      mps_sub(aa + k, bb, aa + k, bn + 1);
      ++q1;
    }
    t1 = aa[i];
    q[k] = (mpbase_t)q1;
  }
  mps_shr(aa, shift, r, bn);
  GFREE(aa);
  GFREE(bb);
  return 0;
}
mpbase_t mps_mulss(mpbase_t a, mpbase_t b, mpbase_t t, mpbase_t* high)
{
  mpbase_t al, ah, bl, bh, ll, lh, hl, hh;
  al = a & MPL_MASK, ah = a >> HALF_MPBITS;
  bl = b & MPL_MASK, bh = b >> HALF_MPBITS;
  lh = al * bh, hl = ah * bl;
  ll = al * bl + (lh << HALF_MPBITS) + (hl << HALF_MPBITS);
  hh = ah * bh + (lh >> HALF_MPBITS) + (hl >> HALF_MPBITS);
  ll += t;
  hh += ll < t;
  *high = hh;
  return ll;
}
mpbase_t mps_muls2(const mpbase_t* a, mpbase_t b, mpbase_t* c, int n)
{
  int i;
  mpbase_t hh = 0, ll = 0;
  for (i = 0; i < n; ++i) {
    c[i] = mps_mulss(a[i], b, hh, &hh);
  }
  return 0;
}
int mps_mul2(const mpbase_t* a, const mpbase_t* b, mpbase_t* c, int an, int bn)
{
  int i, j;
  mpbase_t hh = 0, ll = 0;
  for (i = 0; i < an; ++i) {
    for (hh = 0, j = 0; j < bn; ++j) {
      ll = mps_mulss(a[i], b[j], hh, &hh);
      c[i + j] += ll;
      hh += c[i + j] < ll;
    }
    for (; hh; ++j) {
      c[i + j] += hh;
      hh = c[i + j] < ll;
    }
  }
  return 0;
}
mpbase_t mps_divss(mpbase_t h, mpbase_t l, mpbase_t b, mpbase_t* q)
{
  mpbase_t bh, bl, qh, t, tt;
  bl = b & MPL_MASK;
  bh = b >> HALF_MPBITS;
  qh = h / bh;
  t = h - qh * bh;
  tt = (t << HALF_MPBITS) | (l >> HALF_MPBITS);
  qh -= tt < qh * bl;
  return 0;
}
int mps_rand(mpbase_t* a, int an)
{
  int i, n;
  unsigned char* ua = (unsigned char*)a;
  n = an * sizeof(mpbase_t);
  for (i = 0; i < n; ++i) {
    ua[i] = rand() & 0xff;
  }
  return 0;
}
//  Input:
//        4n   3n   2n    n    0
//
//                     A1    A0
//                 +-------+--------+
//  a[0..2n-1]     |   a1  | a0     |
//                 +-------+--------+
//
//                     B1    B0
//                 +-------+--------+
//  b[0..2n-1]     |   b1  | b0     |
//                 +-------+--------+
//
//                 T2              T1               T0
//                 +-------+-------+-------+--------+
//  tmp[0..4n-1]   |       |       |       |        |  space for temporaries
//                 +-------+-------+-------+--------+
//
// Output:             P3      P2      P1      P0
//                 +-------+-------+-------+--------+
// c[0..4n-1]      |             a * b              |
//                 +-------+-------+-------+--------+
//
// Let
//     a = a1*B^n + a0
//     b = b1*B^n + b0
//
// then
//    a*b = (a1*b1)*B^2n + (a1*b0)*B^n + (a0*b1)*B^n + (a0*b0)
//
//      = (a0*b0) + (a1*b1)*B^2n + (a1*b1 + a0*b0 + (a1-a0)(b0-b1))*B^n
//
//    4n   3n   2n    n    0
//    +-------+-------+-------+--------+
//    |    a1 * b1    |    a0 * b0     |
//    +-------+-------+-------+--------+
//    +-------+-------+-------+--------+
//  + |  0    |    a1 * b1    |  0     |
//    +-------+-------+-------+--------+
//    +-------+-------+-------+--------+
//  + |  0    |    a0 * b0    |  0     |
//    +-------+-------+-------+--------+
//    +-------+-------+-------+--------+
//  + |  0    |(a1-a0)*(b0-b1)|  0     |
//    +-------+-------+-------+--------+
//   ----------------------------------------
//    +--------------------------------+
//  = |           a   *   b            |
//    +--------------------------------+
//
//  We compute the three partial products a1*b1, a0*b0, and (a1-a0)*(b0-b1)
//  recursively as long as n is even and greater or equal to KARA_LIMIT
//  and by a call of School_Mult otherwise.
#define KARA_LIMIT  16
int mps_karatsuba(const mpbase_t* a, const mpbase_t* b, mpbase_t* c, mpbase_t* tmp, int an, int bn)
{
  int i, n, a_sign, b_sign, an0, an1, bn0, bn1;
  const mpbase_t* A0, *A1, *B0, *B1;
  mpbase_t* P0, *P1, *P2, *P3, *T0, *T2;
  if (an < bn) {
    const mpbase_t* ctptr;
    SWAP(an, bn, n);
    SWAP(a, b, ctptr);
  }
  if (bn <= KARA_LIMIT) {
    MEMSET(c, 0, an + bn);
    return mps_mul(a, b, c, an, bn);
  }
  n = (an + 1) / 2;
  if (bn <= n) {
    for (i = 0; i < an; i += bn) {
      mps_karatsuba(a + i, b, c + i, tmp, MIN(an - i, bn), bn);
    }
    return 0;
  }
  an0 = n, an1 = an - n, bn0 = n, bn1 = bn - n;
  A0 = a, A1 = a + an0, B0 = b, B1 = b + bn0;
  P0 = c, P1 = c + n, P2 = c + 2 * n, P3 = c + 3 * n;
  T0 = tmp, T2 = tmp + 2 * n;
  a_sign = mps_abssub(A0, A1, P2, an0, an1);  // c[2n..3n] := a0 - a1
  b_sign = mps_abssub(B1, B0, P3, bn1, bn0);  // c[3n..4n] := b1 - b0
  mps_karatsuba(P2, P3, T2, T0, n, n);  // tmp[2n..4n] := |a1-a0|*|b0-b1|
  //MEMSET(c, 0, 4*n);
  mps_karatsuba(A0, B0, P0, T0, an0, bn0);  // c[0..2n] := a0 * b0
  mps_karatsuba(A1, B1, P2, T0, an1, bn1);  // c[2n..4n] := a1 * b1
  MEMCPY(T0, P0, 2 * n);
  // now we have
  //        P3   P2   P1   P0
  //    +-------+-------+-------+-------+
  //  c = |   a1*b1   |   a0*b0   |
  //    +-------+-------+-------+-------+
  //
  //            T2   T1   T0
  //    +-------+-------+-------+-------+
  // tmp = ||a1-a0|*|b0-b1||   a0*b0   |
  //    +-------+-------+-------+-------+
  mps_absadd(P1, P2, P1, 3 * n, 2 * n);  // c[1n..3n] += c[2n..4n]
  mps_absadd(P1, T0, P1, 3 * n, 2 * n);  // c[1n..3n] += tmp[0n..2n]
  if (a_sign == b_sign) {
    mps_absadd(P1, T2, P1, 3 * n, 2 * n); // c[1n..3n] += tmp[2n..4n]
  }
  else {
    mps_abssub(P1, T2, P1, 3 * n, 2 * n); // c[1n..3n] -= tmp[2n..4n]
  }
  //printf("%d====================\n", (a_sign == b_sign));
  //mps_print(a, 2 * n); mps_print(b, 2 * n); mps_print(P0, 4 * n);
  //printf("====================\n");
  return 0;
}
int mpzn_cmp(const mpzn_t* a, const mpzn_t* b)
{
  return mps_cmp(a->data, b->data, MPZN_N, MPZN_N);
}
// c=a+b
int mpzn_adds(const mpzn_t* a, mpbase_t b, mpzn_t* c)
{
  mps_adds(a->data, b, c->data, MPZN_N);
  return 0;
}
int mpzn_add(const mpzn_t* a, const mpzn_t* b, mpzn_t* c)
{
  mps_absadd(a->data, b->data, c->data, MPZN_N, MPZN_N);
  return 0;
}
int mpzn_subs(const mpzn_t* a, mpbase_t b, mpzn_t* c)
{
  mpbase_t t;
  t = mps_subs(a->data, b, c->data, MPZN_N);
  ASSERT(0 == t);
  return 0;
}
int mpzn_sub(const mpzn_t* a, const mpzn_t* b, mpzn_t* c)
{
  int s = mpzn_cmp(a, b);
  if (s) {
    mpbase_t t;
    if (s < 0) {
      const mpzn_t* t;
      SWAP(a, b, t);
    }
    t = mps_sub(a->data, b->data, c->data, MPZN_N);
    t = mps_subs(a->data + MPZN_N, t, c->data + MPZN_N, MPZN_N - MPZN_N);
    ASSERT(0 == t);
  }
  return s;
}
mpbase_t mpzn_sub_mul(const mpzn_t* a, const mpzn_t* b, mpbase_t c, mpzn_t* d)
{
  mpzn_t dd[1] = {0};
  mpbase_t t;
  t = mps_sub_mul(a->data, b->data, c, dd->data, MPZN_N);
  return t;
}
int mpzn_muls(const mpzn_t* a, mpbase_t b, mpzn_t* c)
{
  mps_muls(a->data, b, c->data, MPZN_N);
  return 0;
}
int mpzn_mul(const mpzn_t* a, const mpzn_t* b, mpzn_t* c)
{
  mpbase_t cc[2 * MPZN_N] = {0};
  mps_mul(a->data, b->data, cc, MPZN_N, MPZN_N);
  MEMCPY(c->data, cc, MPZN_N);
  return 0;
}
int mpzn_shl(const mpzn_t* a, mpbase_t b, mpzn_t* c)
{
  mpbase_t r;
  int k = b / MPBITS;
  b %= MPBITS;
  r = mps_shl(a->data, b, c->data + k, MPZN_N);
  return r;
}
mpbase_t mpzn_shr(const mpzn_t* a, mpbase_t b, mpzn_t* c)
{
  mpbase_t r;
  r = mps_shr(a->data, b, c->data, MPZN_N);
  return r;
}
mpbase_t mpzn_divs(const mpzn_t* a, mpbase_t b, mpzn_t* q)
{
  mpbase_t r;
  r = mps_divs(a->data, b, q->data, MPZN_N);
  return r;
}
int mpzn_rand(mpzn_t* a, int n)
{
  mps_rand(a->data, MPZN_N);
  return 0;
}
int mpzn_div(const mpzn_t* a, const mpzn_t* b, mpzn_t* q, mpzn_t* r)
{
  mpzn_t qq[1] = {0};
  mpzn_t rr[1] = {0};
  mps_div(a->data, b->data, qq->data, rr->data, MPZN_N, MPZN_N);
  return 0;
}
// c=a%b
int mpzn_mod(const mpzn_t* a, const mpzn_t* b, mpzn_t* c)
{
  mpzn_t qq[1] = {0};
  mpzn_div(a, b, qq, c);
  return 0;
}
int mpzn_setstr(mpzn_t* a, str_t s)
{
  int i, len, j = 0;
  int u = (int)log10(MP_MASK);
  const mpbase_t m10 = MP_MASK / 10;
  len = s.l / u + 1;
  for (i = 0; i < s.l && j < MPZN_N;) {
    mpbase_t x = 0, b = 1;
    for (; i < s.l && b < m10; ++i, b *= 10) {
      x = x * 10 + (s.s[i] - '0');
    }
    a->data[j] = mps_muls_add(a->data, b, x, a->data, j);
    j += a->data[j] > 0;
  }
  return 0;
}
int mpzn_tostr(const mpzn_t* a, str_t* s)
{
  mps_tostr(a->data, MPZN_N, s);
  return 0;
}
int mpz_cmp(const mpz_t* a, const mpz_t* b)
{
  return mps_cmp(a->data, b->data, a->len, b->len);
}
// c=a+b
int mpz_adds(const mpz_t* a, mpbase_t b, mpz_t* c)
{
  mpz_t cc[1] = {0};
  mpz_setsize(cc, a->len + 1);
  cc->data[a->len] = mps_adds(a->data, b, cc->data, a->len);
  mpz_adjust_free(cc, c);
  return 0;
}
int mpz_add(const mpz_t* a, const mpz_t* b, mpz_t* c)
{
  mpz_t cc[1] = {0};
  mpbase_t t = 0;
  MPZSWAPP(a, b);
  mpz_setsize(cc, a->len + 1);
#if 0
  t = mps_add(a->data, b->data, cc->data, b->len);
  cc->data[a->len] = mps_adds(a->data + b->len, t, cc->data + b->len, a->len - b->len);
#else
  mps_absadd(a->data, b->data, cc->data, a->len, b->len);
#endif
  mpz_adjust_free(cc, c);
  return 0;
}
int mpz_subs(const mpz_t* a, mpbase_t b, mpz_t* c)
{
  if (1 == a->len) {
    mpbase_t aa = a->data[0], cc = ABSSUB(aa, b);
    mpz_setint(c, cc);
    return CC_CMP(aa, b);
  }
  else if (0 == a->len) {
    mpz_setint(c, b);
    return CC_CMP(0, b);
  }
  else {
    mpbase_t t;
    mpz_t cc[1] = {0};
    mpz_setsize(cc, a->len);
    t = mps_subs(a->data, b, cc->data, a->len);
    ASSERT(0 == t);
    mpz_adjust_free(cc, c);
  }
  return 0;
}
int mpz_sub(const mpz_t* a, const mpz_t* b, mpz_t* c)
{
  int s = mpz_cmp(a, b);
  if (s) {
    mpbase_t t;
    mpz_t cc[1] = {0};
    if (s < 0) {
      MPZSWAPP(a, b);
    }
    mpz_setsize(cc, a->len);
    t = mps_sub(a->data, b->data, cc->data, b->len);
    t = mps_subs(a->data + b->len, t, cc->data + b->len, a->len - b->len);
    ASSERT(0 == t);
    mpz_adjust_free(cc, c);
  }
  return s;
}
mpbase_t mpz_sub_mul(const mpz_t* a, const mpz_t* b, mpbase_t c, mpz_t* d)
{
  mpz_t dd[1] = {0};
  mpbase_t t;
  mpz_setsize(dd, a->len);
  t = mps_sub_mul(a->data, b->data, c, dd->data, a->len);
  mpz_adjust_free(dd, d);
  return t;
}
int mpz_muls(const mpz_t* a, mpbase_t b, mpz_t* c)
{
  mpz_t cc[1] = {0};
  mpz_setsize(cc, a->len + 1);
  cc->data[a->len] = mps_muls(a->data, b, cc->data, a->len);
  mpz_adjust_free(cc, c);
  return 0;
}
int mpz_mul(const mpz_t* a, const mpz_t* b, mpz_t* c)
{
  mpz_t cc[1] = {0};
  mpz_setsize(cc, a->len + b->len + 1);
  mps_mul(a->data, b->data, cc->data, a->len, b->len);
  mpz_adjust_free(cc, c);
  return 0;
}
int mpz_mul_kara(const mpz_t* a, const mpz_t* b, mpz_t* c)
{
  mpz_t cc[1] = {0};
  mpz_t tt[1] = {0};
  mpz_t aa[1] = {0};
  mpz_t bb[1] = {0};
  int an = next_power(a->len), bn = next_power(b->len);
  int n = an + bn;
  mpz_setsize(aa, an), MEMCPY(aa->data, a->data, a->len);
  mpz_setsize(bb, an), MEMCPY(bb->data, b->data, b->len);
  mpz_setsize(cc, n + 1);
  mpz_setsize(tt, 2 * n);
  mps_karatsuba(aa->data, bb->data, cc->data, tt->data, an, bn);
  mpz_free(tt);
  mpz_free(aa);
  mpz_free(bb);
  mpz_adjust_free(cc, c);
  return 0;
}
int mpz_shl(const mpz_t* a, mpbase_t b, mpz_t* c)
{
  mpz_t cc[1] = {0};
  int k = b / MPBITS;
  b %= MPBITS;
  mpz_setsize(cc, a->len + k + 1);
  cc->data[a->len + k] = mps_shl(a->data, b, cc->data + k, a->len);
  mpz_adjust_free(cc, c);
  return 0;
}
mpbase_t mpz_shr(const mpz_t* a, mpbase_t b, mpz_t* c)
{
  mpz_t cc[1] = {0};
  mpbase_t r;
  mpz_setsize(cc, a->len);
  r = mps_shr(a->data, b, cc->data, a->len);
  mpz_adjust_free(cc, c);
  return r;
}
mpbase_t mpz_divs(const mpz_t* a, mpbase_t b, mpz_t* q)
{
  mpz_t qq[1] = {0};
  mpbase_t r;
  mpz_setsize(qq, a->len);
  r = mps_divs(a->data, b, qq->data, a->len);
  mpz_adjust_free(qq, q);
  return r;
}
int mpz_rand(mpz_t* a, int n)
{
  mpz_setsize(a, n);
  mps_rand(a->data, a->len);
  return 0;
}
int mpz_div(const mpz_t* a, const mpz_t* b, mpz_t* q, mpz_t* r)
{
  mpz_t qq[1] = {0};
  mpz_t rr[1] = {0};
  mpz_setsize(qq, a->len - b->len + 2);
  mpz_setsize(rr, b->len);
  mps_div(a->data, b->data, qq->data, rr->data, a->len, b->len);
  mpz_adjust_free(qq, q);
  mpz_adjust_free(rr, r);
  return 0;
}
// c=a%b
int mpz_mod(const mpz_t* a, const mpz_t* b, mpz_t* c)
{
  mpz_t qq[1] = {0};
  mpz_div(a, b, qq, c);
  mpz_free(qq);
  return 0;
}
int mpz_setstr(mpz_t* a, str_t s)
{
  int i, len, j = 0;
  int u = (int)log10(MP_MASK);
  const mpbase_t m10 = MP_MASK / 10;
  len = s.l / u + 1;
  mpz_setsize(a, len);
  for (i = 0; i < s.l;) {
    mpbase_t x = 0, b = 1;
    for (; i < s.l && b < m10; ++i, b *= 10) {
      x = x * 10 + (s.s[i] - '0');
    }
    a->data[j] = mps_muls_add(a->data, b, x, a->data, j);
    j += a->data[j] > 0;
  }
  a->len = j;
  return 0;
}
int mpz_tostr(const mpz_t* a, str_t* s)
{
#if 0
  mpz_t c[1] = {0};
  mpbase_t r;
  int i, smaxlen;
  int u = (int)log10(MP_MASK);
  mpbase_t base10 = (mpbase_t)pow(10, u);
  char fmt[32] = "%";
  char buf[32];
  mpz_clone(a, c);
  str_setsize(s, 0);
  _snprintf(fmt + 1, 30, "0%dd", u);
  smaxlen = (a->len + 1) * (u + 1);
  str_setsize(s, smaxlen);
  while (c->len > 0) {
    r = mps_divs(c->data, base10, c->data, c->len);
    mpz_adjustlen(c);
    _snprintf(buf, 32, fmt, r);
    smaxlen -= u;
    memcpy(s->s + smaxlen, buf, u);
  }
  ASSERT(smaxlen >= 0);
  for (i = smaxlen; i < s->l && s->s[i] == '0'; ++i);
  str_del(s, 0, i);
  mpz_free(c);
#else
  mps_tostr(a->data, a->len, s);
#endif
  return 0;
}
#endif // _MP_H_

