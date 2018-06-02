
#include "cstd.h"
#include "cmath.h"


int matprinti(int n, int m, const int* x, int step, int cn) {
  int i, j;
  for (i=0; i<n; ++i) {
    const int* x1 = x + i*step;
    for (j=0; j<m; ++j) {
      printf("%3d, ", x1[j*cn]);
    }
    printf("\n");
  }
  return 0;
}

int check(int n, const int* man, const int* woman, const int* match)  {
  int i, j, a, b, c;
  int* wm = MALLOC(int, n*n+n); // wm[i][j]女孩i对男孩j的排名
  int* choose = wm+n*n;
  for (i=0; i<n; ++i) {
    choose[match[i]] = i;
    for (j=0; j<n; ++j) {
      wm[i*n+woman[i*n+j]] = j;
    }
  }
  for (i=0; i<n; ++i) {
    a = match[i];
    for (j=0; j<n && (b=man[i*n+j])!=a; ++j) {
      c = choose[b]; // 女孩b的男友c
      if (wm[b*n+c]>wm[b*n+i]) {
        printf("不稳定\n");
        if (1) {
          printf("wm[] = \n");
          matprinti(n, n, wm, n, 1);
          printf("man[] = \n");
          matprinti(n, n, man, n, 1);
          printf("woman[] = \n");
          matprinti(n, n, woman, n, 1);
          printf("match[] = \n");
          matprinti(n, 1, match, 1, 1);
          printf("choose[] = \n");
          matprinti(n, 1, choose, 1, 1);
          printf("\n");
        }
        FREE(wm);
        return 1; //不稳定
      }
    }
  }
  FREE(wm);
  return 0;
}

void GaleShapley(int n, const int* man, const int* woman, int* match) {
  int* wm = MALLOC(int, n*n+2*n); // wm[i][j]女孩i对男孩j的排名
  int* choose = wm+n*n; // choose[i] 女孩i的当前男友
  int* manindex = choose+n; // [i] 男孩i被多少个女孩拒绝过
  int i, j;
  int w, m;
  int bSingle = 0; // 是否所有男友都有了女友
  for (i=0; i<n; ++i) {
    match[i] = -1;
    choose[i] = -1;
    manindex[i] = 0; // 从最喜欢的女孩开始选
    for (j=0; j<n; ++j) {
      wm[i*n+woman[i*n+j]] = j;
    }
  }
  while (!bSingle) {
    bSingle = 1;
    for (i=0; i<n; ++i) {
      if (match[i]!=-1) {
        continue;
      }
      bSingle = 0;
      j = manindex[i]++;
      w = man[i*n+j]; // 男孩i第j喜欢的女孩w
      m = choose[w];
      if ((m==-1)||(wm[w*n+i] < wm[w*n+m])) {
        match[i] = w;
        choose[w] = i;
        if (m!=-1) {
          match[m] = -1;
        }
      }
    }
  }
  if (check(n, man, woman, match)) {
  }
  FREE(wm);
  return ;
}

int test_galeshapley() {
  if (0) {
    enum { N = 4 };
    int man[N*N] = {
      2,3,1,0,
        2,1,3,0,
        0,2,3,1,
        1,3,2,0,
    }; // 男孩喜欢的女孩列表
    int woman[N*N] = {
      0,3,2,1,
        0,1,2,3,
        0,2,3,1,
        1,0,3,2,
    };
    int match[N]; //男孩的女友
    GaleShapley(N, man, woman, match);
  }
  {
    int i, j;
    int n = 4000;
    int* man = MALLOC(int, 2*n*n+n);
    int* woman = man + n*n;
    int* match = woman + n*n;
    
    for (j=0; j<100; ++j) {
      for (i=0; i<n; ++i) {
        rand_permutation(man + i*n, n);
        rand_permutation(woman + i*n, n);
      }
      {utime_start(_start_time);
      GaleShapley(n, man, woman, match);
      printf("%f\n", utime_elapsed(_start_time));}
    }
    FREE(man);
  }
  return 0;
}