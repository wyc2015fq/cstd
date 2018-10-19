
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "popcount.h"

double dist(const double* arrayA, const double* arrayB, int n) {
  double s = 0;
  for (int i = 0; i < n; ++i) {
    double a = arrayA[i];
    double b = arrayB[i];
    double d = a - b;
    s += d * d;
  }
  s /= n;
  s = sqrt(s);
  s = 1. / (1. + s);
  return s;
}

double meandev(const double* arrayA, int n, double& stdev) {
  double sum = 0;
  for (int i = 0; i < n; ++i) {
    double a = arrayA[i];
    sum += a;
  }

  double mean = sum / n; //¾ùÖµ 
  double accum = 0.0;
  for (int i = 0; i < n; ++i) {
    double a = arrayA[i];
    accum += (a - mean)*(a - mean);
  }

  stdev = sqrt(accum / (n - 1)); //·½²î 
  return mean;
}
double dist_diff(const double* arrayA, const double* arrayB, int n) {
  double s = 0;
  double meanA, devA;
  double meanB, devB;
  meanA = meandev(arrayA, n, devA);
  meanB = meandev(arrayA, n, devB);
  for (int i = 0; i < n-1; ++i) {
    double a = arrayA[i + 1] - arrayA[i];
    double b = arrayB[i + 1] - arrayB[i];
    double d = a - b;
    s += d * d;
  }
  s /= n;
  s = sqrt(s);
  s = 1. / (1. + s);
  return s;
}

double mycosine(const double* arrayA, const double* arrayB, int length) {
  if (!arrayA || !arrayB) return 0;
  double sumarrayA = 0, sumarrayB = 0;
  double cosine = 0;
  for (int i = 0; i < length; i++) {
    double a = arrayA[i];
    double b = arrayB[i];
    sumarrayA += a * a;
    sumarrayB += b * b;
    cosine += a * b;
  }
  if ((sumarrayA - 0 < 0.0001) || (sumarrayB - 0 < 0.0001)) {
    return 0;
  }
  cosine /= sqrt(sumarrayA*sumarrayB);
  //  cout<<sumarrayA<<' '<<sumarrayB<<' '<<cosine<<endl;
  return cosine;
}

double mycosine_diff(const double* arrayA, const double* arrayB, int length) {
  if (!arrayA || !arrayB) return 0;
  double sumarrayA = 0, sumarrayB = 0;
  double cosine = 0;
  for (int i = 0; i < length-1; i++) {
    double a = arrayA[i + 1] - arrayA[i];
    double b = arrayB[i + 1] - arrayB[i];
    sumarrayA += a * a;
    sumarrayB += b * b;
    cosine += a * b;
  }
  if ((sumarrayA - 0 < 0.0001) || (sumarrayB - 0 < 0.0001)) {
    return 0;
  }
  cosine /= sqrt(sumarrayA*sumarrayB);
  //  cout<<sumarrayA<<' '<<sumarrayB<<' '<<cosine<<endl;
  return cosine;
}

double sub_match(const double* a, int na, const double* b, int nb, int mincount, int maxount, int ma, int mb, int& maska, int& maskb) {
  double fa[100];
  double fb[100];
  double mindis = 0;
  for (int i = 1; i < (1 << na); ++i) {
    if ((i&ma)!=ma) continue;
    int k = 0, count = popcount(i);
    if (mincount <= count && count) {
      double c = pow(count, 0.01);
      for (int j = 0; j < na; ++j) {
        if (i & (1 << j)) {
          fa[k++] = a[j];
        }
      }
      for (int m = 1; m < (1 << nb); ++m) {
        if ((m&mb)!=mb) continue;
        int k = 0, countb = popcount(m);
        if (countb == count) {
          for (int j = 0; j < nb; ++j) {
            if (m & (1 << j)) {
              fb[k++] = b[j];
            }
          }
          double dis = mycosine_diff(fa, fb, count)*c;
          if (dis > mindis) {
            mindis = dis;
            maska = i;
            maskb = m;
          }
        }
      }
    }
  }
  return mindis;
}

#define countof(arr)  sizeof(arr)/sizeof(arr[0])

int printAllCombination(const char* s, int mincount, int maxount, int ma) {
  int len = strlen(s);
  int num = 0;

  for (int i = 1; i < 1 << len; ++i) {
    int count = popcount(i);
    //printf("%d\n", count);
    if (!(i&ma)) continue;
    if (mincount <= count && count < maxount) {
      for (int j = 0; j < len; ++j) {
        char c = (i & (1 << j)) ? s[j] : ' ';
        printf("%c", c);
      }
      printf("\n");
      ++num;
    }
  }
  return num;
}

int print_mask_arr(const double* arr, int len, int i) {
  for (int j = 0; j < len; ++j) {
    char c = (i & (1 << j)) ? '1' : '0';
    printf("%c", c);
  }
  printf(" ");
  for (int j = 0; j < len; ++j) {
    if (i & (1 << j)) {
      printf("%5.1lf ", arr[j]);
    }
  }
  printf("\n");
  return 0;
}

int test_sub_match() {
  double a[] = { 0, 47, 73, 99, 143, 187, 231 };
  double b[] = { 0, 73, 99, 143, 187, 231 };
  printAllCombination("12345", 3, 4, 1<<1);
  int ma, mb;
  double dis = sub_match(a, countof(a), b, countof(b), 3, 100, -1, 1<<3, ma, mb);
  print_mask_arr(a, countof(a), ma);
  print_mask_arr(b, countof(b), mb);
  return 0;
}

