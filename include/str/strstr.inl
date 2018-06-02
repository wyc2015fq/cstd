#ifndef _STRSTR_INL_
#define _STRSTR_INL_
#define OUTPUT return
#define ASIZE 256
#define XSIZE 256
//一个字符8位 最大256种
#define MAX_CHAR_SIZE 256
// 字符串匹配 Brute Force(BF或蛮力搜索) 算法
// http://www-igm.univ-mlv.fr/~lecroq/string/node3.html
// 时间复杂度是O(mn)
// 首先将匹配串和模式串左对齐，然后从左向右一个一个进行比较，如果不成功则模式串向右移动一个单位。
CC_INLINE int strstr_BF(const char* y, int n, const char* x, int m)
{
  int i, j;
  // Searching
  for (j = 0; j <= n - m; ++j) {
    for (i = 0; i < m && x[i] == y[i + j]; ++i);
    if (i >= m) {
      return (j);
    }
  }
  return -1;
}
#define REHASH(a, b, h) ((((h) - (a)*d) << 1) + (b))
// Karp-Rabin algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node5.html
CC_INLINE int strstr_KR(const char* y, int n, const char* x, int m)
{
  int d, hx, hy, i, j;
  // Preprocessing
  // computes d = 2^(m-1) with the left-shift operator
  for (d = i = 1; i < m; ++i) {
    d = (d << 1);
  }
  for (hy = hx = i = 0; i < m; ++i) {
    hx = ((hx << 1) + x[i]);
    hy = ((hy << 1) + y[i]);
  }
  /* Searching */
  j = 0;
  while (j <= n - m) {
    if (hx == hy && memcmp(x, y + j, m) == 0) {
      return (j);
    }
    hy = REHASH(y[j], y[j + m], hy);
    ++j;
  }
  return -1;
}
CC_INLINE void preMp(const char* x, int m, int mpNext[])
{
  int i, j;
  i = 0;
  j = mpNext[0] = -1;
  while (i < m) {
    while (j > -1 && x[i] != x[j]) {
      j = mpNext[j];
    }
    mpNext[++i] = ++j;
  }
}
// Morris-Pratt algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node7.html
CC_INLINE int strstr_MP(const char* y, int n, const char* x, int m)
{
  int i, j, mpNext[XSIZE];
  /* Preprocessing */
  preMp(x, m, mpNext);
  /* Searching */
  i = j = 0;
  while (j < n) {
    while (i > -1 && x[i] != y[j]) {
      i = mpNext[i];
    }
    i++;
    j++;
    if (i >= m) {
      return (j - i);
      //i = mpNext[i];
    }
  }
  return -1;
}
// Knuth-Morris-Pratt algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node8.html#SECTION0080
CC_INLINE void preKmp(const char* x, int m, int kmpNext[])
{
  int i, j;
  i = 0;
  j = kmpNext[0] = -1;
  while (i < m) {
    while (j > -1 && x[i] != x[j]) {
      j = kmpNext[j];
    }
    i++;
    j++;
    if (x[i] == x[j]) {
      kmpNext[i] = kmpNext[j];
    }
    else {
      kmpNext[i] = j;
    }
  }
}
CC_INLINE int strstr_KMP(const char* y, int n, const char* x, int m)
{
  int i, j, kmpNext[XSIZE];
  /* Preprocessing */
  preKmp(x, m, kmpNext);
  /* Searching */
  i = j = 0;
  while (j < n) {
    while (i > -1 && x[i] != y[j]) {
      i = kmpNext[i];
    }
    i++;
    j++;
    if (i >= m) {
      return (j - i);
      //i = kmpNext[i];
    }
  }
  return -1;
}
// Colussi algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node10.html#SECTION00100
CC_INLINE int preCOLUSSI(const char* x, int m, int h[], int next[], int shift[])
{
  int i, k, nd, q, r = 0, s;
  int hmax[XSIZE], kmin[XSIZE], nhd0[XSIZE], rmin[XSIZE];
  /* Computation of hmax */
  i = k = 1;
  do {
    while (x[i] == x[i - k]) {
      i++;
    }
    hmax[k] = i;
    q = k + 1;
    while (hmax[q - k] + k < i) {
      hmax[q] = hmax[q - k] + k;
      q++;
    }
    k = q;
    if (k == i + 1) {
      i = k;
    }
  }
  while (k <= m);
  /* Computation of kmin */
  memset(kmin, 0, m * sizeof(int));
  for (i = m; i >= 1; --i)
    if (hmax[i] < m) {
      kmin[hmax[i]] = i;
    }
  /* Computation of rmin */
  for (i = m - 1; i >= 0; --i) {
    if (hmax[i + 1] == m) {
      r = i + 1;
    }
    if (kmin[i] == 0) {
      rmin[i] = r;
    }
    else {
      rmin[i] = 0;
    }
  }
  /* Computation of h */
  s = -1;
  r = m;
  for (i = 0; i < m; ++i)
    if (kmin[i] == 0) {
      h[--r] = i;
    }
    else {
      h[++s] = i;
    }
  nd = s;
  /* Computation of shift */
  for (i = 0; i <= nd; ++i) {
    shift[i] = kmin[h[i]];
  }
  for (i = nd + 1; i < m; ++i) {
    shift[i] = rmin[h[i]];
  }
  shift[m] = rmin[0];
  /* Computation of nhd0 */
  s = 0;
  for (i = 0; i < m; ++i) {
    nhd0[i] = s;
    if (kmin[i] > 0) {
      ++s;
    }
  }
  /* Computation of next */
  for (i = 0; i <= nd; ++i) {
    next[i] = nhd0[h[i] - kmin[h[i]]];
  }
  for (i = nd + 1; i < m; ++i) {
    next[i] = nhd0[m - rmin[h[i]]];
  }
  next[m] = nhd0[m - rmin[h[m - 1]]];
  return(nd);
}
CC_INLINE int strstr_COLUSSI(char* y, int n, char* x, int m)
{
  int i, j, last, nd, h[XSIZE], next[XSIZE], shift[XSIZE];
  /* Processing */
  nd = preCOLUSSI(x, m, h, next, shift);
  /* Searching */
  i = j = 0;
  last = -1;
  while (j <= n - m) {
    while (i < m && last < j + h[i] &&
        x[h[i]] == y[j + h[i]]) {
      i++;
    }
    if (i >= m || last >= j + h[i]) {
      return (j);
      i = m;
    }
    if (i > nd) {
      last = j + m - 1;
    }
    j += shift[i];
    i = next[i];
  }
  return -1;
}
// Galil-Giancarlo algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node11.html#SECTION00110
CC_INLINE int strstr_GG(char* y, int n, char* x, int m)
{
  int i, j, k, ell, last, nd;
  int h[XSIZE], next[XSIZE], shift[XSIZE];
  char heavy;
  for (ell = 0; x[ell] == x[ell + 1]; ell++);
  if (ell == m - 1) {
    // Searching for a power of a single character
    for (j = ell = 0; j < n; ++j) {
      if (x[0] == y[j]) {
        ++ell;
        if (ell >= m) {
          return (j - m + 1);
        }
      }
      else {
        ell = 0;
      }
    }
  }
  else {
    // Preprocessing
    nd = preCOLUSSI(x, m, h, next, shift);
    // Searching
    i = j = heavy = 0;
    last = -1;
    while (j <= n - m) {
      if (heavy && i == 0) {
        k = last - j + 1;
        while (x[0] == y[j + k]) {
          k++;
        }
        if (k <= ell || x[ell + 1] != y[j + k]) {
          i = 0;
          j += (k + 1);
          last = j - 1;
        }
        else {
          i = 1;
          last = j + k;
          j = last - (ell + 1);
        }
        heavy = 0;
      }
      else {
        while (i < m && last < j + h[i] && x[h[i]] == y[j + h[i]]) {
          ++i;
        }
        if (i >= m || last >= j + h[i]) {
          return(j);
          i = m;
        }
        if (i > nd) {
          last = j + m - 1;
        }
        j += shift[i];
        i = next[i];
      }
      heavy = (j > last ? 0 : 1);
    }
  }
  return -1;
}
// Apostolico-Crochemore algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node12.html
CC_INLINE int strstr_AXAMAC(char* y, int n, char* x, int m)
{
  int i, j, k, ell, kmpNext[XSIZE];
  /* Preprocessing */
  preKmp(x, m, kmpNext);
  for (ell = 1; x[ell - 1] == x[ell]; ell++);
  if (ell == m) {
    ell = 0;
  }
  /* Searching */
  i = ell;
  j = k = 0;
  while (j <= n - m) {
    while (i < m && x[i] == y[i + j]) {
      ++i;
    }
    if (i >= m) {
      while (k < ell && x[k] == y[j + k]) {
        ++k;
      }
      if (k >= ell) {
        return(j);
      }
    }
    j += (i - kmpNext[i]);
    if (i == ell) {
      k = MAX(0, k - 1);
    }
    else {
      if (kmpNext[i] <= ell) {
        k = MAX(0, kmpNext[i]);
        i = ell;
      }
      else {
        k = ell;
        i = kmpNext[i];
      }
    }
  }
  return -1;
}
// Not So Naive algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node13.html#SECTION00130
CC_INLINE int strstr_NSN(char* y, int n, char* x, int m)
{
  int j, k, ell;
  /* Preprocessing */
  if (x[0] == x[1]) {
    k = 2;
    ell = 1;
  }
  else {
    k = 1;
    ell = 2;
  }
  /* Searching */
  j = 0;
  while (j <= n - m) {
    if (x[1] != y[j + 1]) {
      j += k;
    }
    else {
      if (memcmp(x + 2, y + j + 2, m - 2) == 0 &&
          x[0] == y[j]) {
        OUTPUT(j);
      }
      j += ell;
    }
  }
  return 0;
}
// 单模式匹配算法 Boyer-Moore
// http://www-igm.univ-mlv.fr/~lecroq/string/node14.html
// 原始论文：R.S.Boyer, J.S.Moore, A fast string searching algorithm , Communications of the ACM,20(10):762-772 ,1977
CC_INLINE int preBmBc(const char* x, int m, int* bmBc)
{
  int i;
  for (i = 0; i < ASIZE; ++i) {
    bmBc[i] = m;
  }
  for (i = 0; i < m - 1; ++i) {
    bmBc[x[i]] = m - i - 1;
  }
  return 0;
}
CC_INLINE int suffixes(const char* x, int m, int* suff)
{
  int f, g, i;
  suff[m - 1] = m;
  g = m - 1;
  for (i = m - 2; i >= 0; --i) {
    if (i > g && suff[i + m - 1 - f] < i - g) {
      suff[i] = suff[i + m - 1 - f];
    }
    else {
      if (i < g) {
        g = i;
      }
      f = i;
      while (g >= 0 && x[g] == x[g + m - 1 - f]) {
        --g;
      }
      suff[i] = f - g;
    }
  }
  return 0;
}
CC_INLINE int preBmGs(const char* x, int m, int bmGs[])
{
  int i, j, suff[XSIZE];
  suffixes(x, m, suff);
  for (i = 0; i < m; ++i) {
    bmGs[i] = m;
  }
  j = 0;
  for (i = m - 1; i >= 0; --i) {
    if (suff[i] == i + 1) {
      for (; j < m - 1 - i; ++j) {
        if (bmGs[j] == m) {
          bmGs[j] = m - 1 - i;
        }
      }
    }
  }
  for (i = 0; i <= m - 2; ++i) {
    bmGs[m - 1 - suff[i]] = m - 1 - i;
  }
  return 0;
}
CC_INLINE int strstr_BM(const char* y, int n, const char* x, int m)
{
  int i, j, bmGs[XSIZE], bmBc[ASIZE];
  /* Preprocessing */
  preBmGs(x, m, bmGs);
  preBmBc(x, m, bmBc);
  /* Searching */
  j = 0;
  while (j <= n - m) {
    for (i = m - 1; i >= 0 && x[i] == y[i + j]; --i);
    if (i < 0) {
      return (j);
      j += bmGs[0];
    }
    else {
      j += MAX(bmGs[i], bmBc[y[i + j]] - m + 1 + i);
    }
  }
  return -1;
}
// Turbo-BM algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node15.html#SECTION00150
CC_INLINE int strstr_TBM(char* y, int n, char* x, int m)
{
  int bcShift, i, j, shift, u, v, turboShift, bmGs[XSIZE], bmBc[ASIZE];
  /* Preprocessing */
  preBmGs(x, m, bmGs);
  preBmBc(x, m, bmBc);
  /* Searching */
  j = u = 0;
  shift = m;
  while (j <= n - m) {
    i = m - 1;
    while (i >= 0 && x[i] == y[i + j]) {
      --i;
      if (u != 0 && i == m - 1 - shift) {
        i -= u;
      }
    }
    if (i < 0) {
      OUTPUT(j);
      shift = bmGs[0];
      u = m - shift;
    }
    else {
      v = m - 1 - i;
      turboShift = u - v;
      bcShift = bmBc[y[i + j]] - m + 1 + i;
      shift = MAX(turboShift, bcShift);
      shift = MAX(shift, bmGs[i]);
      if (shift == bmGs[i]) {
        u = MIN(m - shift, v);
      }
      else {
        if (turboShift < bcShift) {
          shift = MAX(shift, u + 1);
        }
        u = 0;
      }
    }
    j += shift;
  }
  return -1;
}
// Apostolico-Giancarlo algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node16.html#SECTION00160
// Reverse Colussi algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node17.html#SECTION00170
CC_INLINE int preRc(char* x, int m, int h[], int rcBc[ASIZE][XSIZE], int rcGs[])
{
  int a, i, j, k, q, r, s,
      hmin[XSIZE], kmin[XSIZE], link[XSIZE],
      locc[ASIZE], rmin[XSIZE];
  /* Computation of link and locc */
  for (a = 0; a < ASIZE; ++a) {
    locc[a] = -1;
  }
  link[0] = -1;
  for (i = 0; i < m - 1; ++i) {
    link[i + 1] = locc[x[i]];
    locc[x[i]] = i;
  }
  /* Computation of rcBc */
  for (a = 0; a < ASIZE; ++a) {
    for (s = 1; s <= m; ++s) {
      i = locc[a];
      j = link[m - s];
      while (i - j != s && j >= 0)
        if (i - j > s) {
          i = link[i + 1];
        }
        else {
          j = link[j + 1];
        }
      while (i - j > s) {
        i = link[i + 1];
      }
      rcBc[a][s] = m - i - 1;
    }
  }
  /* Computation of hmin */
  k = 1;
  i = m - 1;
  while (k <= m) {
    while (i - k >= 0 && x[i - k] == x[i]) {
      --i;
    }
    hmin[k] = i;
    q = k + 1;
    while (hmin[q - k] - (q - k) > i) {
      hmin[q] = hmin[q - k];
      ++q;
    }
    i += (q - k);
    k = q;
    if (i == m) {
      i = m - 1;
    }
  }
  /* Computation of kmin */
  memset(kmin, 0, m * sizeof(int));
  for (k = m; k > 0; --k) {
    kmin[hmin[k]] = k;
  }
  /* Computation of rmin */
  for (i = m - 1; i >= 0; --i) {
    if (hmin[i + 1] == i) {
      r = i + 1;
    }
    rmin[i] = r;
  }
  /* Computation of rcGs */
  i = 1;
  for (k = 1; k <= m; ++k) {
    if (hmin[k] != m - 1 && kmin[hmin[k]] == k) {
      h[i] = hmin[k];
      rcGs[i++] = k;
    }
  }
  i = m - 1;
  for (j = m - 2; j >= 0; --j) {
    if (kmin[j] == 0) {
      h[i] = j;
      rcGs[i--] = rmin[j];
    }
  }
  rcGs[m] = rmin[0];
  return 0;
}
CC_INLINE int strstr_RC(char* y, int n, char* x, int m)
{
  int i, j, s, rcBc[ASIZE][XSIZE], rcGs[XSIZE], h[XSIZE];
  /* Preprocessing */
  preRc(x, m, h, rcBc, rcGs);
  /* Searching */
  j = 0;
  s = m;
  while (j <= n - m) {
    while (j <= n - m && x[m - 1] != y[j + m - 1]) {
      s = rcBc[y[j + m - 1]][s];
      j += s;
    }
    for (i = 1; i < m && x[h[i]] == y[j + h[i]]; ++i);
    if (i >= m) {
      OUTPUT(j);
    }
    s = rcGs[i];
    j += s;
  }
  return -1;
}
// Horspool algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node18.html#SECTION00180
CC_INLINE int strstr_HORSPOOL(char* y, int n, char* x, int m)
{
  int j, bmBc[ASIZE];
  char c;
  /* Preprocessing */
  preBmBc(x, m, bmBc);
  /* Searching */
  j = 0;
  while (j <= n - m) {
    c = y[j + m - 1];
    if (x[m - 1] == c && memcmp(x, y + j, m - 1) == 0) {
      OUTPUT(j);
    }
    j += bmBc[c];
  }
  return -1;
}
// Quick Search algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node19.html
CC_INLINE int preQsBc(const char* x, int m, int* qsBc)
{
  int i;
  for (i = 0; i < ASIZE; ++i) {
    qsBc[i] = m + 1;
  }
  for (i = 0; i < m; ++i) {
    qsBc[(unsigned char)x[i]] = m - i;
  }
  return 0;
}
CC_INLINE int strstr_QS(const char* y, int n, const char* x, int m)
{
  int j = 0, qsBc[ASIZE];
  // Preprocessing
  preQsBc(x, m, qsBc);
  while (j <= n - m) {
    if (memcmp(x, y + j, m) == 0) {
      return j;
    }
    j += qsBc[(unsigned char)y[j + m]]; /* shift */
  }
  return -1;
}
//单模式字符串匹配算法---Tuned Boyer-Moore algorithm实现
// http://www-igm.univ-mlv.fr/~lecroq/string/tunedbm.html
// The function preBmBc is given chapter Boyer-Moore algorithm.
// 单模式匹配算法中最快的一种匹配算法，在BM算法之上进行了改进
// 主要思想就是：因为匹配时最耗时的就是检查模式串的字符跟主串的字符是否匹配，
// 因此减少字符匹配的次数是这个算法的主要思想
// 在实际比较字符之前，进行多次滑动，同样运用“坏字符”规则，这里的预处理与Sunday算法有点区别，
// 遇到不匹配的字符的时候，检查模式子串最后一个字符在主串中的位置
// 然后比较是否相同，不同的话就移动模式串中最后一个字符 所需滑动的距离 继续寻找它在主串的位置
CC_INLINE int strstr_TUNEDBM(char* y, int n, char* x, int m)
{
  int j, k, shift, bmBc[ASIZE];
  // Preprocessing
  preBmBc(x, m, bmBc);
  shift = bmBc[x[m - 1]];
  bmBc[x[m - 1]] = 0;
  memset(y + n, x[m - 1], m);
  /* Searching */
  j = 0;
  while (j < n) {
    k = bmBc[y[j + m - 1]];
    while (k != 0) {
      j += k;
      k = bmBc[y[j + m - 1]];
      j += k;
      k = bmBc[y[j + m - 1]];
      j += k;
      k = bmBc[y[j + m - 1]];
    }
    if (memcmp(x, y + j, m - 1) == 0 && j < n) {
      return (j);
    }
    j += shift; /* shift */
  }
  return -1;
}
// Zhu-Takaoka algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node20.html#SECTION00200
CC_INLINE void preZtBc(char* x, int m, int ztBc[ASIZE][ASIZE])
{
  int i, j;
  for (i = 0; i < ASIZE; ++i)
    for (j = 0; j < ASIZE; ++j) {
      ztBc[i][j] = m;
    }
  for (i = 0; i < ASIZE; ++i) {
    ztBc[i][x[0]] = m - 1;
  }
  for (i = 1; i < m - 1; ++i) {
    ztBc[x[i - 1]][x[i]] = m - 1 - i;
  }
}
CC_INLINE int strstr_ZT(char* y, int n, char* x, int m)
{
  int i, j, ztBc[ASIZE][ASIZE], bmGs[XSIZE];
  /* Preprocessing */
  preZtBc(x, m, ztBc);
  preBmGs(x, m, bmGs);
  /* Searching */
  j = 0;
  while (j <= n - m) {
    i = m - 1;
    while (i < m && x[i] == y[i + j]) {
      --i;
    }
    if (i < 0) {
      OUTPUT(j);
      j += bmGs[0];
    }
    else
      j += MAX(bmGs[i],
          ztBc[y[j + m - 2]][y[j + m - 1]]);
  }
  return -1;
}
// Berry-Ravindran algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/berryravindran.html#SECTION00205
CC_INLINE void preBrBc(char* x, int m, int brBc[ASIZE][ASIZE])
{
  int a, b, i;
  for (a = 0; a < ASIZE; ++a)
    for (b = 0; b < ASIZE; ++b) {
      brBc[a][b] = m + 2;
    }
  for (a = 0; a < ASIZE; ++a) {
    brBc[a][x[0]] = m + 1;
  }
  for (i = 0; i < m - 1; ++i) {
    brBc[x[i]][x[i + 1]] = m - i;
  }
  for (a = 0; a < ASIZE; ++a) {
    brBc[x[m - 1]][a] = 1;
  }
}
CC_INLINE int strstr_BR(char* y, int n, char* x, int m)
{
  int j, brBc[ASIZE][ASIZE];
  /* Preprocessing */
  preBrBc(x, m, brBc);
  /* Searching */
  y[n + 1] = '\0';
  j = 0;
  while (j <= n - m) {
    if (memcmp(x, y + j, m) == 0) {
      OUTPUT(j);
    }
    j += brBc[y[j + m]][y[j + m + 1]];
  }
  return -1;
}
// Smith algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node21.html#SECTION00210
CC_INLINE int strstr_SMITH(char* y, int n, char* x, int m)
{
  int j, bmBc[ASIZE], qsBc[ASIZE];
  /* Preprocessing */
  preBmBc(x, m, bmBc);
  preQsBc(x, m, qsBc);
  /* Searching */
  j = 0;
  while (j <= n - m) {
    if (memcmp(x, y + j, m) == 0) {
      OUTPUT(j);
    }
    j += MAX(bmBc[y[j + m - 1]], qsBc[y[j + m]]);
  }
  return -1;
}
// http://www-igm.univ-mlv.fr/~lecroq/string/node22.html#SECTION00220
// Raita algorithm
CC_INLINE int strstr_RAITA(char* y, int n, char* x, int m)
{
  int j, bmBc[ASIZE];
  char c, firstCh, *secondCh, middleCh, lastCh;
  /* Preprocessing */
  preBmBc(x, m, bmBc);
  firstCh = x[0];
  secondCh = x + 1;
  middleCh = x[m / 2];
  lastCh = x[m - 1];
  /* Searching */
  j = 0;
  while (j <= n - m) {
    c = y[j + m - 1];
    if (lastCh == c && middleCh == y[j + m / 2] &&
        firstCh == y[j] &&
        memcmp(secondCh, y + j + 1, m - 2) == 0) {
      OUTPUT(j);
    }
    j += bmBc[c];
  }
  return -1;
}
// Reverse Factor algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node23.html#SECTION00230
// Backward Nondeterministic Dawg Matching algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/bndm.html#SECTION00300
// Galil-Seiferas algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node25.html#SECTION00250
// Two Way algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node26.html#SECTION00260
/* Computing of the maximal suffix for <= */
CC_INLINE int maxSuf(char* x, int m, int* p)
{
  int ms, j, k;
  char a, b;
  ms = -1;
  j = 0;
  k = *p = 1;
  while (j + k < m) {
    a = x[j + k];
    b = x[ms + k];
    if (a < b) {
      j += k;
      k = 1;
      *p = j - ms;
    }
    else if (a == b)
      if (k != *p) {
        ++k;
      }
      else {
        j += *p;
        k = 1;
      }
    else { /* a > b */
      ms = j;
      j = ms + 1;
      k = *p = 1;
    }
  }
  return(ms);
}
/* Computing of the maximal suffix for >= */
CC_INLINE int maxSufTilde(char* x, int m, int* p)
{
  int ms, j, k;
  char a, b;
  ms = -1;
  j = 0;
  k = *p = 1;
  while (j + k < m) {
    a = x[j + k];
    b = x[ms + k];
    if (a > b) {
      j += k;
      k = 1;
      *p = j - ms;
    }
    else if (a == b)
      if (k != *p) {
        ++k;
      }
      else {
        j += *p;
        k = 1;
      }
    else { /* a < b */
      ms = j;
      j = ms + 1;
      k = *p = 1;
    }
  }
  return(ms);
}
/* Two Way string matching algorithm. */
CC_INLINE int strstr_TW(char* y, int n, char* x, int m)
{
  int i, j, ell, memory, p, per, q;
  /* Preprocessing */
  i = maxSuf(x, m, &p);
  j = maxSufTilde(x, m, &q);
  if (i > j) {
    ell = i;
    per = p;
  }
  else {
    ell = j;
    per = q;
  }
  /* Searching */
  if (memcmp(x, x + per, ell + 1) == 0) {
    j = 0;
    memory = -1;
    while (j <= n - m) {
      i = MAX(ell, memory) + 1;
      while (i < m && x[i] == y[i + j]) {
        ++i;
      }
      if (i >= m) {
        i = ell;
        while (i > memory && x[i] == y[i + j]) {
          --i;
        }
        if (i <= memory) {
          OUTPUT(j);
        }
        j += per;
        memory = m - per - 1;
      }
      else {
        j += (i - ell);
        memory = -1;
      }
    }
  }
  else {
    per = MAX(ell + 1, m - ell - 1) + 1;
    j = 0;
    while (j <= n - m) {
      i = ell + 1;
      while (i < m && x[i] == y[i + j]) {
        ++i;
      }
      if (i >= m) {
        i = ell;
        while (i >= 0 && x[i] == y[i + j]) {
          --i;
        }
        if (i < 0) {
          OUTPUT(j);
        }
        j += per;
      }
      else {
        j += (i - ell);
      }
    }
  }
  return -1;
}
//String Matching on Ordered Alphabets
// http://www-igm.univ-mlv.fr/~lecroq/string/node27.html#SECTION00270
/* Compute the next maximal suffix. */
CC_INLINE void nextMaximalSuffix(char* x, int m, int* i, int* j, int* k, int* p)
{
  char a, b;
  while (*j + *k < m) {
    a = x[*i + *k];
    b = x[*j + *k];
    if (a == b) {
      if (*k == *p) {
        (*j) += *p;
        *k = 1;
      }
      else {
        ++(*k);
      }
    }
    else {
      if (a > b) {
        (*j) += *k;
        *k = 1;
        *p = *j - *i;
      }
      else {
        *i = *j;
        ++(*j);
        *k = *p = 1;
      }
    }
  }
}
/* String matching on ordered alphabets algorithm. */
CC_INLINE int strstr_SMOA(char* y, int n, char* x, int m)
{
  int i, ip, j, jp, k, p;
  /* Searching */
  ip = -1;
  i = j = jp = 0;
  k = p = 1;
  while (j <= n - m) {
    while (i + j < n && i < m && x[i] == y[i + j]) {
      ++i;
    }
    if (i == 0) {
      ++j;
      ip = -1;
      jp = 0;
      k = p = 1;
    }
    else {
      if (i >= m) {
        OUTPUT(j);
      }
      nextMaximalSuffix(y + j, i + 1, &ip, &jp, &k, &p);
      if (ip < 0 ||
          (ip < p &&
              memcmp(y + j, y + j + p, ip + 1) == 0)) {
        j += p;
        i -= p;
        if (i < 0) {
          i = 0;
        }
        if (jp - ip > p) {
          jp -= p;
        }
        else {
          ip = -1;
          jp = 0;
          k = p = 1;
        }
      }
      else {
        j += (MAX(ip + 1,
            MIN(i - ip - 1, jp + 1)) + 1);
        i = jp = 0;
        ip = -1;
        k = p = 1;
      }
    }
  }
  return -1;
}
// Optimal Mismatch algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node28.html#SECTION00280
typedef struct patternScanOrder {
  int loc;
  char c;
} pattern;
static int freq[ASIZE];
typedef int (* pcmp_t)(const void*, const void*);
/* Construct an ordered pattern from a string. */
CC_INLINE int orderPattern(const char* x, int m, pcmp_t pcmp, pattern* pat)
{
  int i;
  for (i = 0; i <= m; ++i) {
    pat[i].loc = i;
    pat[i].c = x[i];
  }
  qsort(pat, m, sizeof(pattern), pcmp);
  return 0;
}
// Optimal Mismatch pattern comparison function
CC_INLINE int optimalPcmp(const pattern* pat1, const pattern* pat2)
{
  float fx;
  fx = (float)freq[pat1->c] - freq[pat2->c];
  return(fx ? (fx > 0 ? 1 : -1) : (pat2->loc - pat1->loc));
}
// Find the next leftward matching shift for the first ploc pattern elements after a current shift or lshift
CC_INLINE int matchShift(const char* x, int m, int ploc, int lshift, pattern* pat)
{
  int i, j;
  for (; lshift < m; ++lshift) {
    i = ploc;
    while (--i >= 0) {
      if ((j = (pat[i].loc - lshift)) < 0) {
        continue;
      }
      if (pat[i].c != x[j]) {
        break;
      }
    }
    if (i < 0) {
      break;
    }
  }
  return(lshift);
}
// Constructs the good-suffix shift table from an ordered string
CC_INLINE void preAdaptedGs(const char* x, int m, int adaptedGs[], pattern* pat)
{
  int lshift, i, ploc;
  adaptedGs[0] = lshift = 1;
  for (ploc = 1; ploc <= m; ++ploc) {
    lshift = matchShift(x, m, ploc, lshift, pat);
    adaptedGs[ploc] = lshift;
  }
  for (ploc = 0; ploc <= m; ++ploc) {
    lshift = adaptedGs[ploc];
    while (lshift < m) {
      i = pat[ploc].loc - lshift;
      if (i < 0 || pat[ploc].c != x[i]) {
        break;
      }
      ++lshift;
      lshift = matchShift(x, m, ploc, lshift, pat);
    }
    adaptedGs[ploc] = lshift;
  }
}
/* Optimal Mismatch string matching algorithm. */
CC_INLINE int strstr_OM(char* y, int n, char* x, int m)
{
  int i, j, adaptedGs[XSIZE], qsBc[ASIZE];
  pattern pat[XSIZE];
  /* Preprocessing */
  orderPattern(x, m, (pcmp_t)optimalPcmp, pat);
  preQsBc(x, m, qsBc);
  preAdaptedGs(x, m, adaptedGs, pat);
  /* Searching */
  j = 0;
  while (j <= n - m) {
    i = 0;
    while (i < m && pat[i].c == y[j + pat[i].loc]) {
      ++i;
    }
    if (i >= m) {
      OUTPUT(j);
    }
    j += MAX(adaptedGs[i], qsBc[y[j + m]]);
  }
  return -1;
}
// Maximal Shift algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node29.html#SECTION00290
static int minShift[XSIZE];
/* Computation of the MinShift table values. */
CC_INLINE void computeMinShift(const char* x, int m)
{
  int i, j;
  for (i = 0; i < m; ++i) {
    for (j = i - 1; j >= 0; --j) {
      if (x[i] == x[j]) {
        break;
      }
    }
    minShift[i] = i - j;
  }
}
/* Maximal Shift pattern comparison function. */
CC_INLINE int maxShiftPcmp(const pattern* pat1, const pattern* pat2)
{
  int dsh;
  dsh = minShift[pat2->loc] - minShift[pat1->loc];
  return(dsh ? dsh : (pat2->loc - pat1->loc));
}
/* Maximal Shift string matching algorithm. */
CC_INLINE int strstr_MS(const char* y, int n, const char* x, int m)
{
  int i, j, qsBc[ASIZE], adaptedGs[XSIZE];
  pattern pat[XSIZE];
  /* Preprocessing */
  computeMinShift(x , m);
  orderPattern(x, m, (pcmp_t)maxShiftPcmp, pat);
  preQsBc(x, m, qsBc);
  preAdaptedGs(x, m, adaptedGs, pat);
  /* Searching */
  j = 0;
  while (j <= n - m) {
    i = 0;
    while (i < m && pat[i].c == y[j + pat[i].loc]) {
      ++i;
    }
    if (i >= m) {
      OUTPUT(j);
    }
    j += MAX(adaptedGs[i], qsBc[y[j + m]]);
  }
  return -1;
}
// KMP Skip Search algorithm
// http://www-igm.univ-mlv.fr/~lecroq/string/node32.html#SECTION00320
CC_INLINE int attempt(char* y, char* x, int m, int start, int wall)
{
  int k;
  k = wall - start;
  while (k < m && x[k] == y[k + start]) {
    ++k;
  }
  return(k);
}
CC_INLINE int strstr_KMPSKIP(char* y, int n, char* x, int m)
{
  int i, j, k, kmpStart, per, start, wall;
  int kmpNext[XSIZE], list[XSIZE], mpNext[XSIZE], z[ASIZE];
  /* Preprocessing */
  preMp(x, m, mpNext);
  preKmp(x, m, kmpNext);
  memset(z, -1, ASIZE * sizeof(int));
  memset(list, -1, m * sizeof(int));
  z[x[0]] = 0;
  for (i = 1; i < m; ++i) {
    list[i] = z[x[i]];
    z[x[i]] = i;
  }
  /* Searching */
  wall = 0;
  per = m - kmpNext[m];
  i = j = -1;
  do {
    j += m;
  }
  while (j < n && z[y[j]] < 0);
  if (j >= n) {
    return -1;
  }
  i = z[y[j]];
  start = j - i;
  while (start <= n - m) {
    if (start > wall) {
      wall = start;
    }
    k = attempt(y, x, m, start, wall);
    wall = start + k;
    if (k == m) {
      OUTPUT(start);
      i -= per;
    }
    else {
      i = list[i];
    }
    if (i < 0) {
      do {
        j += m;
      }
      while (j < n && z[y[j]] < 0);
      if (j >= n) {
        return -1;
      }
      i = z[y[j]];
    }
    kmpStart = start + k - kmpNext[k];
    k = kmpNext[k];
    start = j - i;
    while (start < kmpStart ||
        (kmpStart < start && start < wall)) {
      if (start < kmpStart) {
        i = list[i];
        if (i < 0) {
          do {
            j += m;
          }
          while (j < n && z[y[j]] < 0);
          if (j >= n) {
            return -1;
          }
          i = z[y[j]];
        }
        start = j - i;
      }
      else {
        kmpStart += (k - mpNext[k]);
        k = mpNext[k];
      }
    }
  }
  return -1;
}
// 字符串匹配---sunday算法实现
//
// 适用于：模式串较短的情况，最坏时间复杂性为O(N*M)，不过一般没这么坏
//
// Sunday算法其实思想跟BM算法很相似，只不过Sunday算法是从前往后匹配，
// 在匹配失败时关注的是文本串中参加匹配的最末位字符的下一位字符。
// 如果该字符没有在匹配串中出现则直接跳过，即移动步长= 匹配串长度+ 1；
// 否则，同BM算法一样其移动步长=匹配串中最右端的该字符到末尾的距离+1。
//
// 遇到不匹配的看大串中匹配范围之外的右侧第一个字符在小串中的最右位置
// 另外：采用BM/KMP的预处理的做法，事先计算好移动步长，等到遇到不匹配的值直接使用
// 设定每个字符最右移动步长，保存每个字符的移动步长
// 如果大串中匹配字符的右侧一个字符没在子串中，大串移动步长=整个串的距离+1
// 如果大串中匹配范围内的右侧一个字符在子串中，大串移动距离=子串长度-这个字符在子串中的位置
CC_INLINE int* setCharStep(const char* subStr, int subStrLen, int* charStep)
{
  int i;
  for (i = 0; i < MAX_CHAR_SIZE; i++) {
    charStep[i] = subStrLen + 1;
  }
  //从左向右扫描一遍 保存子串中每个字符所需移动步长
  for (i = 0; i < subStrLen; i++) {
    charStep[(unsigned char)subStr[i]] = subStrLen - i;
  }
  return charStep;
}
CC_INLINE int strstr_sunday_find(const char* mainStr, int mainStrLen, const char* subStr, int subStrLen, int* charStep)
{
  int main_i = 0;
  int sub_j = 0;
  while (main_i < mainStrLen) {
    //保存大串每次开始匹配的起始位置，便于移动指针
    int tem = main_i;
    while (sub_j < subStrLen) {
      if (mainStr[main_i] == subStr[sub_j]) {
        main_i++;
        sub_j++;
        continue;
      }
      else {
        unsigned char firstRightChar;
        //如果匹配范围外已经找不到右侧第一个字符，则匹配失败
        if (tem + subStrLen > mainStrLen) {
          return -1;
        }
        //否则 移动步长 重新匹配
        firstRightChar = (unsigned char)mainStr[tem + subStrLen];
        main_i = tem + charStep[firstRightChar];
        sub_j = 0;
        break;//退出本次失败匹配 重新一轮匹配
      }
    }
    if (sub_j == subStrLen) {
      return main_i - subStrLen;
    }
  }
  return -1;
}
// 算法核心思想，从左向右匹配，遇到不匹配的看大串中匹配范围之外的右侧第一个字符在小串中的最右位置
// 根据事先计算好的移动步长移动大串指针，直到匹配
CC_INLINE int strstr_sunday(const char* mainStr, int mainStrLen, const char* subStr, int subStrLen)
{
  int charStep[MAX_CHAR_SIZE] = {0};
  setCharStep(subStr, subStrLen, charStep);
  return strstr_sunday_find(mainStr, mainStrLen, subStr, subStrLen, charStep);
}
#undef OUTPUT
#ifdef TEST
#include "cmath.h"
int test_strstr()
{
  char* mainStr;
  char subStr[256];
  const char* strmap;
  int i, pos, pos0, maxlen = 10000000;
  int mainStrLen, subStrLen;
  uint32 seed = 10;
  strmap = "abcdefghijklmnopqrstuvwxyz0123456789";
  strmap = "abcdefghijklmnopqrstuvwxyz";
  mainStr = (char*)malloc(maxlen);
  for (i = 0; i < 100; ++i) {
    randstrnum(&seed, subStr, 80);
    randstr(&seed, mainStr, maxlen - 10, strmap);
    subStrLen = strlen(subStr);
    mainStrLen = strlen(mainStr);
    pos0 = -1;
    if (mainStrLen > subStrLen && (rand() % 10) > 2) {
      pos0 = (mainStrLen - subStrLen) / 2;
      memcpy(mainStr + pos0, subStr, subStrLen);
    }
    if (1) {
      utime_start(_start_time);
      pos = strstr_BF(mainStr, mainStrLen, subStr, subStrLen);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(pos == pos0);
    }
    if (0) {
      utime_start(_start_time);
      pos = strstr_MP(mainStr, mainStrLen, subStr, subStrLen);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(pos == pos0);
    }
    if (0) {
      utime_start(_start_time);
      pos = strstr_KMP(mainStr, mainStrLen, subStr, subStrLen);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(pos == pos0);
    }
    if (0) {
      utime_start(_start_time);
      pos = strstr_COLUSSI(mainStr, mainStrLen, subStr, subStrLen);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(pos == pos0);
    }
    if (0) {
      utime_start(_start_time);
      pos = strstr_GG(mainStr, mainStrLen, subStr, subStrLen);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(pos == pos0);
    }
    if (0) {
      utime_start(_start_time);
      pos = strstr_AXAMAC(mainStr, mainStrLen, subStr, subStrLen);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(pos == pos0);
    }
    if (0) {
      utime_start(_start_time);
      pos = strstr_NSN(mainStr, mainStrLen, subStr, subStrLen);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(pos == pos0);
    }
    if (1) {
      utime_start(_start_time);
      pos = strstr_TBM(mainStr, mainStrLen, subStr, subStrLen);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(pos == pos0);
    }
    if (1) {
      utime_start(_start_time);
      pos = strstr_HORSPOOL(mainStr, mainStrLen, subStr, subStrLen);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(pos == pos0);
    }
    if (1) {
      utime_start(_start_time);
      pos = strstr_KMPSKIP(mainStr, mainStrLen, subStr, subStrLen);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(pos == pos0);
    }
    if (1) {
      utime_start(_start_time);
      pos = strstr_KR(mainStr, mainStrLen, subStr, subStrLen);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(pos == pos0);
    }
    if (1) {
      utime_start(_start_time);
      pos = strstr_sunday(mainStr, mainStrLen, subStr, subStrLen);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(pos == pos0);
    }
    if (1) {
      utime_start(_start_time);
      pos = strstr_BM(mainStr, mainStrLen, subStr, subStrLen);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(pos == pos0);
    }
    if (0) {
      utime_start(_start_time);
      pos = strstr_TUNEDBM(mainStr, mainStrLen, subStr, subStrLen);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(pos == pos0);
    }
    printf("\n");
  }
  free(mainStr);
  return 0;
}
#endif // TEST
#endif // _STRSTR_INL_

