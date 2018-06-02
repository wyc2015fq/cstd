#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "macro.h"
#include "sort.h"


#include "kdtree.inl"

// compute euclidean distance between two points
double dist_euclidean(int point_dim, const arrtype* d, const arrtype* p)
{
  int j;
  double dist = 0;

  for (j = 0; j < point_dim; ++j) {
    double diff = d[j] - p[j];
    dist += diff * diff;
  }

  return sqrt(dist);
}

// compute euclidean distance between two points
double dist_chi(int point_dim, const arrtype* d, const arrtype* p)
{
  int j;
  double dist = 0;

  for (j = 0; j < point_dim; ++j) {
    double diff = d[j] - p[j];
    double sum = d[j] + p[j];

    if (sum) {
      dist += diff * diff / sum;
    }
  }

  return sqrt(dist);
}

// compute euclidean distance between two points
double dist_abs(int point_dim, const arrtype* d, const arrtype* p)
{
  int j;
  double dist = 0;

  for (j = 0; j < point_dim; ++j) {
    double diff = d[j] > p[j] ? (d[j] - p[j]) : (p[j] - d[j]);
    dist += diff;
  }

  return dist;
}

int test_kdtree()
{
  kdtree_t tr[1] = {0};
  int i, h = 1000, w = 200;
  int m = 1, k = 1, emax = 10;
  arrtype* A = MALLOC(arrtype, h * w);
  arrtype* B = MALLOC(arrtype, m * w);
  dist_t* results = MALLOC(dist_t, m * k);
  dist_t* results2 = MALLOC(dist_t, m * k);
  int re[] = {91, 50, 94, 46, 63, 29, 93, 50, 35, 90};
  distfun_t disfun = dist_chi;
  srand((0));
  //srand(time(0));
  FILL(h * w, A, (rand() % 1000));
  {
    utime_start(u1);
    kd_init(tr, h, w, A, w);
    printf("%f\n", utime_elapsed(u1));
  }

  for (i = 0; i < 10; ++i) {
    double time1, time2, tt1;
    FILL(m * w, B, (rand() % 100));
    MEMSET(results, 0, m * k);
    MEMSET(results2, 0, m * k);
    {
      utime_start(u1);
      KD_find_nn(tr, B, k, 100, disfun, results);
      time1 = utime_elapsed(u1);
    }
    {
      utime_start(u2);
      knn_find(h, w, A, w, B, k, disfun, results2);
      time2 = utime_elapsed(u2);
    }

    tt1 = (results[0].dist - results2[0].dist) * 200. / (results[0].dist + results2[0].dist);
    printf("%3d %3d %5.2f %7.6f\n", results[0].id, results2[0].id, tt1, time2 / time1);
  }

  kd_free(tr);
  FREE(A);
  FREE(B);
  FREE(results);
  FREE(results2);
  return 0;
}
