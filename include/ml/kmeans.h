
#ifndef _KMEANS_H_
#define _KMEANS_H_

#include <float.h>
#include "distance.h"
#include "macro.h"


#define VEC_ADD(dims, c, s)                                                   \
  do {                                                                          \
    int j;                                                                      \
    for( j = 0; j <= dims - 4; j += 4 ) {                                       \
      *(c+j  ) += *(s+j  );                                                     \
      *(c+j+1) += *(s+j+1);                                                     \
      *(c+j+2) += *(s+j+2);                                                     \
      *(c+j+3) += *(s+j+3);                                                     \
    }                                                                           \
    for( ; j < dims; ++j ) {                                                    \
      *(c+j  ) += *(s+j  );                                                     \
    }                                                                           \
  } while(0)

// [行, 列, 指针, 行步进]
// [sample_count, dims, samples, samples_step]
// [cluster_count, dims, centers, centers_step]
// [cluster_count, dims, old_centers, old_centers_step]
// [cluster_count, counters]
// [sample_count, labels]
#define KMEANS2(TYPE, TYPE2, sample_count, dims, samples, samples_step,       \
    cluster_count, _centers, centers_step, _old_centers, old_centers_step,       \
    counters, labels, epsilon, max_iter )                                        \
do {                                                                          \
  int i, j, k, iter;                                                          \
  TYPE* s = 0;                                                                \
  TYPE2* c = 0;                                                               \
  TYPE2* c_o = 0;                                                             \
  TYPE2* temp = 0;                                                            \
  TYPE2* centers = _centers;                                                  \
  TYPE2* old_centers = _old_centers;                                          \
  CvRNG rng = (-1);                                                           \
  TYPE max_dist;                                                              \
  \
  /* init centers */                                                          \
  for( i = 0; i < sample_count; i++ )                                         \
    labels[i] = cvRandInt(rng) % cluster_count;                               \
  \
  max_dist = (TYPE)(epsilon*2);                                               \
  \
  for( iter = 0; iter < (max_iter); iter++ ) {                                \
    /* computer centers */                                                    \
    SKIP_FILL( cluster_count, dims, centers, centers_step, 1, 0);             \
    VEC_FILL( cluster_count, counters, 0);                                    \
    \
    for( i = 0; i < sample_count; i++ ) {                                     \
      s = samples + i*samples_step;                                           \
      k = labels[i];                                                          \
      c = centers + k*centers_step;                                           \
      VEC_ADD(dims, c, s);                                                    \
      counters[k]++;                                                          \
    }                                                                         \
    \
    if( iter > 0 )                                                            \
      max_dist = 0;                                                           \
    \
    for( k = 0; k < cluster_count; k++ ) {                                    \
      c = (TYPE2*)(centers + k*centers_step);                                 \
      if( counters[k] != 0 ) {                                                \
        double scale = 1./counters[k];                                        \
        for( j = 0; j < dims; j++ )                                           \
          c[j] *= scale;                                                      \
      } else {                                                                \
        i = cvRandInt(rng) % sample_count;                                    \
        s = (TYPE*)(samples + i*samples_step);                                \
        for( j = 0; j < dims; j++ )                                           \
          c[j] = s[j];                                                        \
      }                                                                       \
      \
      if( iter > 0 ) {                                                        \
        TYPE dist = 0;                                                        \
        c_o = (TYPE2*)(old_centers + k*old_centers_step);                     \
        EUCLID_DISTOR2(dims, c, c_o, dist, TYPE2);                            \
        if( max_dist < dist )                                                 \
          max_dist = dist;                                                    \
      }                                                                       \
    }                                                                         \
    \
    /* assign labels */                                                       \
    for( i = 0; i < sample_count; i++ ) {                                     \
      TYPE* s = (TYPE*)(samples + i*samples_step);                            \
      int k_best = 0;                                                         \
      TYPE min_dist = DBL_MAX;                                                \
      \
      for( k = 0; k < cluster_count; k++ ) {                                  \
        TYPE2 dist = 0;                                                       \
        c = (TYPE2*)(centers + k*centers_step);                               \
        EUCLID_DISTOR2(dims, c, s, dist, TYPE2);                              \
        if( min_dist > dist ) {                                               \
          min_dist = dist;                                                    \
          k_best = k;                                                         \
        }                                                                     \
      }                                                                       \
      \
      labels[i] = k_best;                                                     \
    }                                                                         \
    \
    if( max_dist < epsilon )                                                  \
      break;                                                                  \
    \
    CV_SWAP( centers, old_centers, temp );                                    \
  }                                                                           \
  \
  /* empty clusters */                                                        \
  VEC_FILL( cluster_count, counters, 0);                                      \
  for( i = 0; i < sample_count; i++ )                                         \
    counters[labels[i]]++;                                                    \
  \
  /* ensure that we do not have empty clusters */                             \
  for( k = 0; k < cluster_count; k++ ) {                                      \
    if( counters[k] == 0 ) {                                                  \
      for(;;) {                                                               \
        i = cvRandInt( rng) % sample_count;                                   \
        j = labels[i];                                                        \
        if( counters[j] > 1 ) {                                               \
          labels[i] = k;                                                      \
          counters[j]--;                                                      \
          counters[k]++;                                                      \
          break;                                                              \
        }                                                                     \
      }                                                                       \
    }                                                                         \
  }                                                                           \
} while(0)

#endif // _KMEANS_H_
