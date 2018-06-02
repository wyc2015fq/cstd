#include "cstd.h"
#include "hmm.inl"
int test_hmm1()
{
  float a[3][3] = {{0.5, 0.2, 0.3}, {0.3, 0.5, 0.2}, {0.2, 0.3, 0.5}};
  float b[3][2] = {{0.5, 0.5}, {0.4, 0.6}, {0.7, 0.3}};
  float result[4][3];
  int list[4] = {0, 1, 0, 1};
  int max[4][3];
  float tmp;
  int i, j, k, count = 1, max_node;
  float max_v;
  //step1:Initialization
  result[0][0] = 0.2 * 0.5;
  result[0][1] = 0.4 * 0.4;
  result[0][2] = 0.4 * 0.7;
  //step2:πÈƒ…‘ÀÀ„
  for (i = 1; i <= 3; i++) {
    for (j = 0; j <= 2; j++) {
      tmp = result[i - 1][0] * a[0][j];
      max[i][j] = 0;
      for (k = 1; k <= 2; k++) {
        if (result[i - 1][k] * a[k][j] > tmp) {
          tmp = result[i - 1][k] * a[k][j];
          max[i][j] = k;
        }
        result[i][j] = tmp * b[j][list[count]];
      }
    }
    count += 1;
  }
  max_v = result[3][0];
  max_node = 0;
  for (k = 1; k <= 2; k++) {
    if (result[3][k] > max_v) {
      max_v = result[3][k];
      max_node = k;
    }
  }
  //step3:÷’Ω·
  for (i = 0; i <= 3; i++) {
    for (j = 0; j <= 2; j++) {
      printf("%d %d %f\n", i + 1, j + 1, result[i][j]);
    }
  }
  printf("Pmax=%f\n", max_v);
  printf("step4:%d \n", max_node + 1);
  //step4:ªÿÀ›
  for (k = 3; k >= 1; k--) {
    printf("step%d:%d \n", k, max[k][max_node] + 1);
    max_node = max[k][max_node];
  }
  return 0;
}
int test_hmm()
{
  int* hmm = 0;
  enum { N = 3, M = 2, T = 4 };
  double* pi = 0, *A = 0, *B = 0;
  double a[N * N] = {0.5, 0.2, 0.3,
      0.3, 0.5, 0.2,
      0.2, 0.3, 0.5
                    };
  double b[N * M] = {0.5, 0.5,
      0.4, 0.6,
      0.7, 0.3
                    };
  double p[N] = {0.2, 0.4, 0.4};
  double alpha[T * N] = {0};
  double beta[T * N] = {0};
  double delta[T * N] = {0};
  int psi[T * N] = {0};
  int q[T] = {0};
  int O[T] = {0, 1, 0, 1};
  double prob1 = 0;
  double prob2 = 0;
  double prob3 = 0;
  test_hmm1();
  hmm_resize(&hmm, N, M, &pi, &A, &B);
  MEMCPY(A, a, N * N);
  MEMCPY(B, b, N * M);
  MEMCPY(pi, p, N);
  hmm_forward(hmm, T, O, alpha, &prob1);
  hmm_backward(hmm, T, O, beta, &prob2);
  hmm_Viterbi(hmm, T, O, delta, psi, q, &prob3);
  return 0;
}

