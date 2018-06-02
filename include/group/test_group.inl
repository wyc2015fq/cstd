
#include "cstd.h"
//#include "group1.inl"
#include "math/rand.inl"

// ¥Ú”°÷√ªª
int print_permutation(const uint* p, int n) {
  int i;
  printf("{");
  for (i=0; i<n; ++i) {
    if (i) {
      printf(",");
    }
    printf("%d", p[i]);
  }
  printf("}");
  return 0;
}
// ¥Ú”°¬÷ªª
int print_rotation(const uint* p, int n) {
  int i, start = 1;
  for (i=0; i<n; ++i) {
    if (-1==p[i]) {
      if (!start) {
        start = 1;
        printf(")");
      }
    } else {
      if (start) {
        printf("(");
      } else {
        printf(",");
      }
      start = 0;
      printf("%d", p[i]);
    }
  }
  return 0;
}

// ≈–∂œ «∑Ò «÷√ªª
int is_permutation_group(const uint* p, int n) {
  uchar* cnt = 0, i, ret = 1;
  for (i=0; i<n; ++i) {
    if (p[i]>=n) {
      return 0;
    }
  }
  MYREALLOC(cnt, n);
  MEMSET(cnt, 0, n);
  for (i=0; i<n; ++i) {
    if (cnt[p[i]]==0) {
      cnt[p[i]]++;
    } else {
      ret = 0;
      break;
    }
  }
  if (ret) {
    for (i=0; i<n; ++i) {
      if (cnt[i]!=1) {
        ret = 0;
        break;
      }
    }
  }
  FREE(cnt);
  return ret;
}

// ÷√ªª◊™¬÷ªª
int permutation_to_rotation(const uint* p, int n, uint* r) {
  uchar* cnt = 0, i, j, k = 0;
  MYREALLOC(cnt, n);
  MEMSET(cnt, 0, n);
  for (i=0; i<n; ++i) {
    if (!cnt[i]) {
      cnt[i] = 1;
      r[k++] = i;
      for (j = i; (j=p[j])!=i; ) {
        r[k++] = j;
        cnt[j] = 1;
      }
      r[k++] = -1;
    }
  }
  FREE(cnt);
  for (j=k; j<2*n; ) {
    r[j++] = -1;
  }
  return k;
}
// ÷√ªª◊™∂‘ªª
int permutation_to_trade(const uint* p, int n, uint* r) {
  ASSERT(0);
  return 0;
}
int permutation_is_identyty(const uint* p, int n) {
  int i;
  for (i=0;i<n;++i) {
    if (p[i]!=i) {
      return 0;
    }
  }
  return 1;
}
// ÷√ªªµƒƒÊ
int permutation_inv(const uint* p, int n, uint* p2) {
  int i;
  for (i=0; i<n; ++i) {
    p2[p[i]] = i;
  }
  return 0;
}
// ÷√ªª≥À∑® p1*p2=p3
int permutation_mul(const uint* p1, const uint* p2, int n, uint* p3) {
  int i;
  for (i=0; i<n; ++i) {
    p3[i] = p2[p1[i]];
  }
  return 0;
}
int permutation_set_identyty(uint* p, int n) {
  int i;
  for (i=0; i<n; ++i) {
    p[i] = i;
  }
  return 0;
}
int permutation_pow(const uint* p, int n, int k, uint* p1) {
  int i;
  if (k>0) {
    MEMCPY(p1, p, n);
    for (i=1; i<k; ++i) {
      permutation_mul(p1, p, n, p1);
    }
  } else {
    permutation_set_identyty(p1, n);
  }
  return 0;
}
int permutation_rank(const uint* p, int n) {
  int rank = 1;
  uint* p1 = NULL;
  MYREALLOC(p1, n);
  MEMCPY(p1, p, n);
  while(!permutation_is_identyty(p1, n)) {
    ++rank;
    permutation_mul(p1, p, n, p1);
  }
  FREE(p1);
  return rank;
}
#include "permutation_group.inl"
int test_group() {
  uint p[6] = {0};
  uint p2[countof(p)] = {0};
  uint r[countof(p)*2] = {0};
  int i;
  rand_permutation(NULL, p, countof(p));
  print_permutation(p, countof(p));
  printf("\n");
  if (1) {
    rand_permutation(NULL, p, countof(p));
    print_permutation(p, countof(p));
    permutation_to_rotation(p, countof(p), r);
    print_rotation(r, countof(r));
    printf("\n");
  }
  if (1) {
    for (i=0; i<10000; ++i) {
      rand_permutation(NULL, p, countof(p));
      ASSERT(is_permutation_group(p, countof(p)));
      permutation_inv(p, countof(p), p2);
      permutation_to_rotation(p, countof(p), r);
      if (0) {
        print_permutation(p, countof(p));
        printf("\n inv = ");
        print_permutation(p2, countof(p2));
        printf("\n");
      }
      if (0) {
        print_permutation(p, countof(p));
        printf("\n = ");
        print_rotation(r, countof(r));
        printf("\n");
      }
      if (0) {
        int rank = permutation_rank(p, countof(p));
        print_permutation(p, countof(p));
        printf(" rank = %d\n", rank);
      }
    }
  }
  return 0;
}
