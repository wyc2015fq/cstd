// 矩阵乘法-分块计算
//将整个矩阵分解为这样的小块，每次完成一对小块的计算，以提高Cache的命中率。
//图中n=N/m
//计算次序为A11*B11, A11*B12,…, A11*B1n,，由于反复使用A11，因此可以提高Cache的命中率。
//C=A*B  --- C(i,j)等于A的第i行乘以第j列
#include "cstd.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
typedef double float_t;
typedef struct ARG {
  float_t* A;
  int ax, ay;
  float_t* B;
  int bx, by;
  float_t* C;
  int cx, cy;
  int m;
  int n;
} ARG;
void ClearMatrix(float_t* m, int n)
{
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      m[i * n + j] = 0.0;
    }
  }
}
// 普通矩阵相乘
void GeneralMul(float_t* A, float_t* B, float_t* C, int n)
{
  int i, j, k;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      float_t* p = C + i * n + j;
      for (k = 0; k < n; k++) {
        *p += A[i * n + k] * B[k * n + j];
      }
    }
  }
}
DWORD WINAPI Mul_Fun(LPVOID arg)
{
  struct ARG* p = (struct ARG*)arg;
  float_t* A = p->A;
  float_t* B = p->B;
  float_t* C = p->C;
  int m = p->m;
  int n = p->n;
  int i, j, k;
  for (i = 0; i < m; i++) {
    for (j = 0; j < m; j++) {
      float_t* t = C + (i + p->cx) * n + p->cy + j;
      for (k = 0; k < m; k++) {
        *t += A[(p->ax + i) * n + p->ay + k] * B[(p->bx + k) * n + p->by + j];
      }
    }
  }
  return 0;
}
// 矩阵分块计算
void BlockCacul(float_t* A, float_t* B, float_t* C, int n, int thread_num, int m)
{
  //m = static_cast<int>(sqrt(m));
  ARG* args = MALLOC(ARG, thread_num);
  HANDLE* handle = MALLOC(HANDLE, thread_num);
  int t = 0;
  int i, j, k, ii=0;
  for (i = 0; i < thread_num; i++) {
    args[i].A = A;
    args[i].B = B;
    args[i].C = C;
    args[i].m = m;
    args[i].n = n;
  }
  //分成n/m x n/m块
  //A i行j列
  for (i = 0; i < n; i += m) {
    for (j = 0; j < n; j += m) {
      //B j行k列
      for (k = 0; k < n; k += m) {
        args[t].ax = i;
        args[t].ay = j;
        args[t].bx = j;
        args[t].by = k;
        args[t].cx = i;
        args[t].cy = k;
        if (t < thread_num) {
          handle[t] = CreateThread(NULL, 0, Mul_Fun, (LPVOID)(&args[t]), 0, 0 );
          //Mul_Fun(&args[t]);
          t++;
        }
        if (t == thread_num) {
          for (ii=0; ii<t; ii++) WaitForMultipleObjects(thread_num, &handle[ii],TRUE,INFINITE);
          t = 0;
        }
      }
    }
  }
}
// 生成n*n矩阵
void GenerateMatrix(float_t* p, int n)
{
  int i;
  srand(time(NULL) + rand());
  for (i = 0; i < n * n; i++) {
    *p = (float_t)(rand()) / ((float_t)(rand()) + (float_t)(0.55));
    p++;
  }
}
// 两个矩阵的误差
float_t diff(float_t* C1, float_t* C0, int n)
{
  float_t rst = 0.0;
  float_t t;
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      t = C1[i * n + j] - C0[i * n + j];
      if (t < 0) {
        t = -t;
      }
      rst += t;
    }
  }
  return rst;
}
void PrintMatrix(float_t* p, int n)
{
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      printf("%.2f\t", p[i * n + j]);
    }
    printf("\n");
  }
  printf("\n");
}
int test_matmul()
{
  int n = 4;
  int thread_num = 4;
  int m = 1024;
  float_t* A = MALLOC(float_t, n * n);
  float_t* B = MALLOC(float_t, n * n);
  float_t* C = MALLOC(float_t, n * n);
  float_t* C0 = MALLOC(float_t, n * n);
  clock_t start;
  float_t time_used, dd;
  GenerateMatrix(A, n);
  GenerateMatrix(B, n);
  ClearMatrix(C0, n);
  start = clock();
  GeneralMul(A, B, C0, n);
  time_used = (float_t)(clock() - start) / CLOCKS_PER_SEC * 1000;
  printf("General:   time = %f\n", time_used);
  ClearMatrix(C, n);
  start = clock();
  BlockCacul(A, B, C, n, thread_num, m);
  time_used = (float_t)(clock() - start) / CLOCKS_PER_SEC * 1000;
  printf("Block:  time = %f\n", time_used);
  dd = diff(C0, C, n);
  printf("Difference of two result: %f\n", dd);
  free(A);
  free(B);
  free(C);
  free(C0);
  return 0;
}

