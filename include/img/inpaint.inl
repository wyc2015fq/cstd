
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "macro.h"
#include "xtypes.h"
#include "macro.h"
#include "filters.h"

#define CC_INPAINT_NS      0
#define CC_INPAINT_TELEA   1

typedef FPOINT FPOINT;

#undef X_MAT_ELEM_PTR_FAST
#define X_MAT_ELEM_PTR_FAST( mat, row, col, pix_size )  \
  ((mat).data.ptr + (size_t)(mat).step*(row) + (pix_size)*(col))

float
min4(float a, float b, float c, float d)
{
  a = MIN(a, b);
  c = MIN(c, d);
  return MIN(a, c);
}

#define X_MAT_ELEM(img,img_step,y,x)  *(img+(y)*(img_step)+(x))

#define CC_MAT_3COLOR_ELEM(img,img_step,y,x,c) X_MAT_ELEM(img,img_step,y,(x)*3+(c))

#define KNOWN  0  //known outside narrow band
#define BAND   1  //narrow band (known)
#define INSIDE 2  //unknown
#define CHANGE 3  //servise

typedef struct CvHeapElem {
  float T;
  int i, j;
  struct CvHeapElem* prev;
  struct CvHeapElem* next;
}
CvHeapElem;

typedef struct CvPriorityQueueFloat {
  CvHeapElem* mem, *empty, *head, *tail;
  int num, in;
}
CvPriorityQueueFloat;

BOOL CvPriorityQueueFloatInit(CvPriorityQueueFloat* pq,
    int f_rows, int f_cols, const unsigned char* f, int f_step)
{
  int i, j;
  pq->num = pq->in = 0;
  pq->mem = pq->empty = pq->head = pq->tail = NULL;

  for (i = pq->num = 0; i < f_rows; i++) {
    for (j = 0; j < f_cols; j++) {
      pq->num += (*(f + i * f_step + j)) != 0;
    }
  }

  if (pq->num <= 0) {
    return FALSE;
  }

  pq->mem = (CvHeapElem*) cvAlloc((pq->num + 2) * sizeof(CvHeapElem));

  if (pq->mem == NULL) {
    return FALSE;
  }

  pq->head = pq->mem;
  pq->head->i = pq->head->j = -1;
  pq->head->prev = NULL;
  pq->head->next = pq->mem + 1;
  pq->head->T = -FLT_MAX;
  pq->empty = pq->mem + 1;

  for (i = 1; i <= pq->num; i++) {
    pq->mem[ i ].prev = pq->mem + i - 1;
    pq->mem[ i ].next = pq->mem + i + 1;
    pq->mem[ i ].i = pq->mem[ i ].i = -1;
    pq->mem[ i ].T = FLT_MAX;
  }

  pq->tail = pq->mem + i;
  pq->tail->i = pq->tail->j = -1;
  pq->tail->prev = pq->mem + i - 1;
  pq->tail->next = NULL;
  pq->tail->T = FLT_MAX;
  return TRUE;
}

BOOL CvPriorityQueueFloatPush(CvPriorityQueueFloat* pq, int i, int j, float T)
{
  CvHeapElem* tmp = pq->empty, *add
      = pq->empty;

  if (pq->empty == pq->tail) {
    return FALSE;
  }

  while (tmp->prev->T > T) {
    tmp = tmp->prev;
  }

  if (tmp != pq->empty) {
    add
    ->prev->next = add
        ->next;
    add
    ->next->prev = add
        ->prev;
    pq->empty = add
        ->next;
    add
    ->prev = tmp->prev;
    add
    ->next = tmp;
    add
    ->prev->next = add
        ;
    add
    ->next->prev = add
        ;
  }
  else {
    pq->empty = pq->empty->next;
  }

  add
  ->i = i;
  add
  ->j = j;
  add
  ->T = T;
  pq->in++;
  //      printf("push i %3d  j %3d  T %12.4e  in %4d\n",i,j,T,in);
  return TRUE;
}

BOOL CvPriorityQueueFloatAdd(CvPriorityQueueFloat* pq, int f_rows, int f_cols, const unsigned char* f, int f_step)
{
  int i, j;

  for (i = 0; i < f_rows; i++) {
    for (j = 0; j < f_cols; j++) {
      if ((*(f + i * f_step + j)) != 0) {
        if (!CvPriorityQueueFloatPush(pq, i, j, 0)) {
          return FALSE;
        }
      }
    }
  }

  return TRUE;
}

BOOL CvPriorityQueueFloatPop(CvPriorityQueueFloat* pq, int* i, int* j)
{
  CvHeapElem* tmp = pq->head->next;

  if (pq->empty == tmp) {
    return FALSE;
  }

  *i = tmp->i;
  *j = tmp->j;
  tmp->prev->next = tmp->next;
  tmp->next->prev = tmp->prev;
  tmp->prev = pq->empty->prev;
  tmp->next = pq->empty;
  tmp->prev->next = tmp;
  tmp->next->prev = tmp;
  pq->empty = tmp;
  pq->in--;
  //      printf("pop  i %3d  j %3d  T %12.4e  in %4d\n",tmp->i,tmp->j,tmp->T,in);
  return TRUE;
}

BOOL CvPriorityQueueFloatPopT(CvPriorityQueueFloat* pq, int* i, int* j, float* T)
{
  CvHeapElem* tmp = pq->head->next;

  if (pq->empty == tmp) {
    return FALSE;
  }

  *i = tmp->i;
  *j = tmp->j;
  *T = tmp->T;
  tmp->prev->next = tmp->next;
  tmp->next->prev = tmp->prev;
  tmp->prev = pq->empty->prev;
  tmp->next = pq->empty;
  tmp->prev->next = tmp;
  tmp->next->prev = tmp;
  pq->empty = tmp;
  pq->in--;
  //      printf("pop  i %3d  j %3d  T %12.4e  in %4d\n",tmp->i,tmp->j,tmp->T,in);
  return TRUE;
}

void CvPriorityQueueFloatUnInit(CvPriorityQueueFloat* pq)
{
  cvFree(&(pq->mem));
}

#define VectorScalMult(v1, v2) ((v1).x*(v2).x+(v1).y*(v2).y)
#define VectorLength(v1)       ((v1).x*(v1).x+(v1).y*(v1).y)

///////////////////////////////////////////////////////////////////////////////////////////
//HEAP::iterator Heap_Iterator;
//HEAP Heap;

float FastMarching_solve(int i1, int j1, int i2, int j2, const unsigned char* f, int f_step, const float* t, int t_step)
{
  double sol, a11, a22, m12;
  a11 = *(t + i1 * t_step + j1);
  a22 = *(t + i2 * t_step + j2);
  m12 = MIN(a11, a22);

  if (*(f + i1 * f_step + j1) != INSIDE)
    if (*(f + i2 * f_step + j2) != INSIDE)
      if (fabs(a11 - a22) >= 1.0) {
        sol = 1 + m12;
      }
      else {
        sol = (a11 + a22 + sqrt((double)(2 - (a11 - a22) * (a11 - a22)))) * 0.5;
      }
    else {
      sol = 1 + a11;
    }
  else if (*(f + i2 * f_step + j2) != INSIDE) {
    sol = 1 + a22;
  }
  else {
    sol = 1 + m12;
  }

  return (float) sol;
}

/////////////////////////////////////////////////////////////////////////////////////
static void
icvCalcFMM(int f_rows, int f_cols, unsigned char* f, int f_step, float* t, int t_step, CvPriorityQueueFloat* Heap, BOOL negate)
{
  int i, j, ii = 0, jj = 0, q;
  float dist;

  while (CvPriorityQueueFloatPop(Heap, &ii, &jj)) {
    unsigned known = (negate) ? CHANGE : KNOWN;
    *(f + ii * f_step + jj) = (unsigned char) known;

    for (q = 0; q < 4; q++) {
      i = 0;
      j = 0;

      if (q == 0) {
        i = ii - 1;
        j = jj;
      }
      else if (q == 1) {
        i = ii;
        j = jj - 1;
      }
      else if (q == 2) {
        i = ii + 1;
        j = jj;
      }
      else {
        i = ii;
        j = jj + 1;
      }

      if ((i <= 0) || (j <= 0) || (i > f_rows) || (j > f_cols)) {
        continue;
      }

      if (*(f + i * f_step + j) == INSIDE) {
        dist = min4(FastMarching_solve(i - 1, j, i, j - 1, f, f_step, t, t_step),
            FastMarching_solve(i + 1, j, i, j - 1, f, f_step, t, t_step),
            FastMarching_solve(i - 1, j, i, j + 1, f, f_step, t, t_step),
            FastMarching_solve(i + 1, j, i, j + 1, f, f_step, t, t_step));
        *(t + i * t_step + j) = dist;
        *(f + i * f_step + j) = BAND;
        CvPriorityQueueFloatPush(Heap, i, j, dist);
      }
    }
  }

  if (negate) {
    for (i = 0; i < f_rows; i++) {
      for (j = 0; j < f_cols; j++) {
        if ((*f + i + j) == CHANGE) {
          *(f + i * f_step + j) = KNOWN;
          *(t + i * t_step + j) = -(*(t + i * t_step + j));
        }
      }
    }
  }
}

static void
icvTeleaInpaintFMM(int f_rows, int f_cols, unsigned char* f, int f_step,
    int t_rows, int t_cols, float* t, int t_step,
    unsigned char* out, int out_step, int out_cn,
    int range, CvPriorityQueueFloat* Heap)
{
  int i = 0, j = 0, ii = 0, jj = 0, k, l, q, color = 0;
  float dist;

  if (out_cn == 3) {
    while (CvPriorityQueueFloatPop(Heap, &ii, &jj)) {
      *(f + f_step * ii + jj) = KNOWN;

      for (q = 0; q < 4; q++) {
        if (q == 0) {
          i = ii - 1;
          j = jj;
        }
        else if (q == 1) {
          i = ii;
          j = jj - 1;
        }
        else if (q == 2) {
          i = ii + 1;
          j = jj;
        }
        else if (q == 3) {
          i = ii;
          j = jj + 1;
        }

        if ((i <= 1) || (j <= 1) || (i > t_rows - 1) || (j > t_cols - 1)) {
          continue;
        }

        if (*(f + f_step * i + j) == INSIDE) {
          dist = min4(FastMarching_solve(i - 1, j, i, j - 1, f, f_step, t, t_step),
              FastMarching_solve(i + 1, j, i, j - 1, f, f_step, t, t_step),
              FastMarching_solve(i - 1, j, i, j + 1, f, f_step, t, t_step),
              FastMarching_solve(i + 1, j, i, j + 1, f, f_step, t, t_step));
          *(t + t_step * i + j) = dist;

          for (color = 0; color <= 2; color++) {
            FPOINT gradI, gradT, r;
            float Ia = 0, Jx = 0, Jy = 0, s = 1.0e-20f, w, dst, lev, dir, sat;

            if (*(f + f_step * i + j + 1) != INSIDE) {
              if (*(f + f_step * i + j - 1) != INSIDE) {
                gradT.x = (float)((*(t + t_step * i + j + 1) - * (t + t_step * i + j - 1))) * 0.5f;
              }
              else {
                gradT.x = (float)((*(t + t_step * i + j + 1) - * (t + t_step * i + j)));
              }
            }
            else {
              if (*(f + f_step * i + j - 1) != INSIDE) {
                gradT.x = (float)((*(t + t_step * i + j) - * (t + t_step * i + j - 1)));
              }
              else {
                gradT.x = 0;
              }
            }

            if (*(f + f_step * (i + 1) + j) != INSIDE) {
              if (*(f + f_step * (i - 1) + j) != INSIDE) {
                gradT.y = (float)((*(t + t_step * (i + 1) + j) - * (t + t_step * (i - 1) + j))) * 0.5f;
              }
              else {
                gradT.y = (float)((*(t + t_step * (i + 1) + j) - * (t + t_step * i + j)));
              }
            }
            else {
              if (*(f + f_step * (i - 1) + j) != INSIDE) {
                gradT.y = (float)((*(t + t_step * i + j) - * (t + t_step * (i - 1) + j)));
              }
              else {
                gradT.y = 0;
              }
            }

            for (k = i - range; k <= i + range; k++) {
              int km = k - 1 + (k == 1), kp = k - 1 - (k == t_rows - 2);

              for (l = j - range; l <= j + range; l++) {
                int lm = l - 1 + (l == 1), lp = l - 1 - (l == t_cols - 2);

                if (k > 0 && l > 0 && k < t_rows - 1 && l < t_cols - 1) {
                  if ((*(f + f_step * k + l) != INSIDE) &&
                      ((l - j) * (l - j) + (k - i) * (k - i) <= range * range)) {
                    r.y = (float)(i - k);
                    r.x = (float)(j - l);

                    dst = (float)(1. / (VectorLength(r) * sqrt((double) VectorLength(r))));
                    lev = (float)(1. / (1 + fabs(*(t + t_step * k + l) - * (t + t_step * i + j))));

                    dir = VectorScalMult(r, gradT);

                    if (fabs(dir) <= 0.01) {
                      dir = 0.000001f;
                    }

                    w = (float) fabs(dst * lev * dir);

                    if (*(f + f_step * k + l + 1) != INSIDE) {
                      if (*(f + f_step * k + l - 1) != INSIDE) {
                        gradI.x = (float)((CC_MAT_3COLOR_ELEM(out, out_step, km, lp + 1, color) - CC_MAT_3COLOR_ELEM(out, out_step, km, lm - 1, color))) * 2.0f;
                      }
                      else {
                        gradI.x = (float)((CC_MAT_3COLOR_ELEM(out, out_step, km, lp + 1, color) - CC_MAT_3COLOR_ELEM(out, out_step, km, lm, color)));
                      }
                    }
                    else {
                      if (*(f + f_step * k + l - 1) != INSIDE) {
                        gradI.x = (float)((CC_MAT_3COLOR_ELEM(out, out_step, km, lp, color) - CC_MAT_3COLOR_ELEM(out, out_step, km, lm - 1, color)));
                      }
                      else {
                        gradI.x = 0;
                      }
                    }

                    if (*(f + f_step * (k + 1) + l) != INSIDE) {
                      if (*(f + f_step * (k - 1) + l) != INSIDE) {
                        gradI.y = (float)((CC_MAT_3COLOR_ELEM(out, out_step, kp + 1, lm, color) - CC_MAT_3COLOR_ELEM(out, out_step, km - 1, lm, color))) * 2.0f;
                      }
                      else {
                        gradI.y = (float)((CC_MAT_3COLOR_ELEM(out, out_step, kp + 1, lm, color) - CC_MAT_3COLOR_ELEM(out, out_step, km, lm, color)));
                      }
                    }
                    else {
                      if (*(f + f_step * (k - 1) + l) != INSIDE) {
                        gradI.y = (float)((CC_MAT_3COLOR_ELEM(out, out_step, kp, lm, color) - CC_MAT_3COLOR_ELEM(out, out_step, km - 1, lm, color)));
                      }
                      else {
                        gradI.y = 0;
                      }
                    }

                    Ia += (float) w * (float)(CC_MAT_3COLOR_ELEM(out, out_step, km, lm, color));
                    Jx -= (float) w * (float)(gradI.x * r.x);
                    Jy -= (float) w * (float)(gradI.y * r.y);
                    s += w;
                  }
                }
              }
            }

            sat = (float)((Ia / s + (Jx + Jy) / (sqrt(Jx * Jx + Jy * Jy) + 1.0e-20f) + 0.5f));
            {
              int isat = cvRound(sat);
              CC_MAT_3COLOR_ELEM(out, out_step, i - 1, j - 1, color) = CC_CAST_8U(isat);
            }
          }

          *(f + f_step * i + j) = BAND;
          CvPriorityQueueFloatPush(Heap, i, j, dist);
        }
      }
    }

  }
  else if (out_cn == 1) {

    while (CvPriorityQueueFloatPop(Heap, &ii, &jj)) {

      *(f + f_step * ii + jj) = KNOWN;

      for (q = 0; q < 4; q++) {
        if (q == 0) {
          i = ii - 1;
          j = jj;
        }
        else if (q == 1) {
          i = ii;
          j = jj - 1;
        }
        else if (q == 2) {
          i = ii + 1;
          j = jj;
        }
        else if (q == 3) {
          i = ii;
          j = jj + 1;
        }

        if ((i <= 1) || (j <= 1) || (i > t_rows - 1) || (j > t_cols - 1)) {
          continue;
        }

        if (*(f + f_step * i + j) == INSIDE) {
          dist = min4(FastMarching_solve(i - 1, j, i, j - 1, f, f_step, t, t_step),
              FastMarching_solve(i + 1, j, i, j - 1, f, f_step, t, t_step),
              FastMarching_solve(i - 1, j, i, j + 1, f, f_step, t, t_step),
              FastMarching_solve(i + 1, j, i, j + 1, f, f_step, t, t_step));
          *(t + t_step * i + j) = dist;

          for (color = 0; color <= 0; color++) {
            FPOINT gradI, gradT, r;
            float Ia = 0, Jx = 0, Jy = 0, s = 1.0e-20f, w, dst, lev, dir, sat;

            if (*(f + f_step * i + j + 1) != INSIDE) {
              if (*(f + f_step * i + j - 1) != INSIDE) {
                gradT.x = (float)((*(t + t_step * i + j + 1) - * (t + t_step * i + j - 1))) * 0.5f;
              }
              else {
                gradT.x = (float)((*(t + t_step * i + j + 1) - * (t + t_step * i + j)));
              }
            }
            else {
              if (*(f + f_step * i + j - 1) != INSIDE) {
                gradT.x = (float)((*(t + t_step * i + j) - * (t + t_step * i + j - 1)));
              }
              else {
                gradT.x = 0;
              }
            }

            if (*(f + f_step * (i + 1) + j) != INSIDE) {
              if (*(f + f_step * (i - 1) + j) != INSIDE) {
                gradT.y = (float)((*(t + t_step * (i + 1) + j) - * (t + t_step * (i - 1) + j))) * 0.5f;
              }
              else {
                gradT.y = (float)((*(t + t_step * (i + 1) + j) - * (t + t_step * i + j)));
              }
            }
            else {
              if (*(f + f_step * (i - 1) + j) != INSIDE) {
                gradT.y = (float)((*(t + t_step * i + j) - * (t + t_step * (i - 1) + j)));
              }
              else {
                gradT.y = 0;
              }
            }

            for (k = i - range; k <= i + range; k++) {
              int km = k - 1 + (k == 1), kp = k - 1 - (k == t_rows - 2);

              for (l = j - range; l <= j + range; l++) {
                int lm = l - 1 + (l == 1), lp = l - 1 - (l == t_cols - 2);

                if (k > 0 && l > 0 && k < t_rows - 1 && l < t_cols - 1) {
                  if ((*(f + f_step * k + l) != INSIDE) &&
                      ((l - j) * (l - j) + (k - i) * (k - i) <= range * range)) {
                    r.y = (float)(i - k);
                    r.x = (float)(j - l);

                    dst = (float)(1. / (VectorLength(r) * sqrt(VectorLength(r))));
                    lev = (float)(1. / (1 + fabs(*(t + t_step * k + l) - * (t + t_step * i + j))));

                    dir = VectorScalMult(r, gradT);

                    if (fabs(dir) <= 0.01) {
                      dir = 0.000001f;
                    }

                    w = (float) fabs(dst * lev * dir);

                    if (*(f + f_step * k + l + 1) != INSIDE) {
                      if (*(f + f_step * k + l - 1) != INSIDE) {
                        gradI.x = (float)((X_MAT_ELEM(out, out_step, km, lp + 1) - X_MAT_ELEM(out, out_step, km, lm - 1))) * 2.0f;
                      }
                      else {
                        gradI.x = (float)((X_MAT_ELEM(out, out_step, km, lp + 1) - X_MAT_ELEM(out, out_step, km, lm)));
                      }
                    }
                    else {
                      if (*(f + f_step * k + l - 1) != INSIDE) {
                        gradI.x = (float)((X_MAT_ELEM(out, out_step, km, lp) - X_MAT_ELEM(out, out_step, km, lm - 1)));
                      }
                      else {
                        gradI.x = 0;
                      }
                    }

                    if (*(f + f_step * (k + 1) + l) != INSIDE) {
                      if (*(f + f_step * (k - 1) + l) != INSIDE) {
                        gradI.y = (float)((X_MAT_ELEM(out, out_step, kp + 1, lm) - X_MAT_ELEM(out, out_step, km - 1, lm))) * 2.0f;
                      }
                      else {
                        gradI.y = (float)((X_MAT_ELEM(out, out_step, kp + 1, lm) - X_MAT_ELEM(out, out_step, km, lm)));
                      }
                    }
                    else {
                      if (*(f + f_step * (k - 1) + l) != INSIDE) {
                        gradI.y = (float)((X_MAT_ELEM(out, out_step, kp, lm) - X_MAT_ELEM(out, out_step, km - 1, lm)));
                      }
                      else {
                        gradI.y = 0;
                      }
                    }

                    Ia += (float) w * (float)(X_MAT_ELEM(out, out_step, km, lm));
                    Jx -= (float) w * (float)(gradI.x * r.x);
                    Jy -= (float) w * (float)(gradI.y * r.y);
                    s += w;
                  }
                }
              }
            }

            sat = (float)((Ia / s + (Jx + Jy) / (sqrt(Jx * Jx + Jy * Jy) + 1.0e-20f) + 0.5f));
            {
              int isat = cvRound(sat);
              X_MAT_ELEM(out, out_step, i - 1, j - 1) = CC_CAST_8U(isat);
            }
          }

          *(f + f_step * i + j) = BAND;
          CvPriorityQueueFloatPush(Heap, i, j, dist);
        }
      }
    }
  }
}

static void
icvNSInpaintFMM(int f_rows, int f_cols, unsigned char* f, int f_step,
    int t_rows, int t_cols, float* t, int t_step,
    unsigned char* out, int out_step, int out_cn,
    int range, CvPriorityQueueFloat* Heap)
{
  int i = 0, j = 0, ii = 0, jj = 0, k, l, q, color = 0;
  float dist;

  if (out_cn == 3) {
    while (CvPriorityQueueFloatPop(Heap, &ii, &jj)) {
      *(f + f_step * ii + jj) = KNOWN;

      for (q = 0; q < 4; q++) {
        if (q == 0) {
          i = ii - 1;
          j = jj;
        }
        else if (q == 1) {
          i = ii;
          j = jj - 1;
        }
        else if (q == 2) {
          i = ii + 1;
          j = jj;
        }
        else if (q == 3) {
          i = ii;
          j = jj + 1;
        }

        if ((i <= 1) || (j <= 1) || (i > t_rows - 1) || (j > t_cols - 1)) {
          continue;
        }

        if (*(f + f_step * i + j) == INSIDE) {
          dist = min4(FastMarching_solve(i - 1, j, i, j - 1, f, f_step, t, t_step),
              FastMarching_solve(i + 1, j, i, j - 1, f, f_step, t, t_step),
              FastMarching_solve(i - 1, j, i, j + 1, f, f_step, t, t_step),
              FastMarching_solve(i + 1, j, i, j + 1, f, f_step, t, t_step));
          *(t + t_step * i + j) = dist;

          for (color = 0; color <= 2; color++) {
            FPOINT gradI, r;
            float Ia = 0, s = 1.0e-20f, w, dst, dir;

            for (k = i - range; k <= i + range; k++) {
              int km = k - 1 + (k == 1), kp = k - 1 - (k == f_rows - 2);

              for (l = j - range; l <= j + range; l++) {
                int lm = l - 1 + (l == 1), lp = l - 1 - (l == f_cols - 2);

                if (k > 0 && l > 0 && k < f_rows - 1 && l < f_cols - 1) {
                  if ((*(f + f_step * k + l) != INSIDE) &&
                      ((l - j) * (l - j) + (k - i) * (k - i) <= range * range)) {
                    r.y = (float)(k - i);
                    r.x = (float)(l - j);

                    dst = 1 / (VectorLength(r) * VectorLength(r) + 1);

                    if (*(f + f_step * (k + 1) + l) != INSIDE) {
                      if (*(f + f_step * (k - 1) + l) != INSIDE) {
                        gradI.x = (float)(abs(CC_MAT_3COLOR_ELEM(out, out_step, kp + 1, lm, color) - CC_MAT_3COLOR_ELEM(out, out_step, kp, lm, color)) +
                            abs(CC_MAT_3COLOR_ELEM(out, out_step, kp, lm, color) - CC_MAT_3COLOR_ELEM(out, out_step, km - 1, lm, color)));
                      }
                      else {
                        gradI.x = (float)(abs(CC_MAT_3COLOR_ELEM(out, out_step, kp + 1, lm, color) - CC_MAT_3COLOR_ELEM(out, out_step, kp, lm, color))) * 2.0f;
                      }
                    }
                    else {
                      if (*(f + f_step * (k - 1) + l) != INSIDE) {
                        gradI.x = (float)(abs(CC_MAT_3COLOR_ELEM(out, out_step, kp, lm, color) - CC_MAT_3COLOR_ELEM(out, out_step, km - 1, lm, color))) * 2.0f;
                      }
                      else {
                        gradI.x = 0;
                      }
                    }

                    if (*(f + f_step * k + l + 1) != INSIDE) {
                      if (*(f + f_step * k + l - 1) != INSIDE) {
                        gradI.y = (float)(abs(CC_MAT_3COLOR_ELEM(out, out_step, km, lp + 1, color) - CC_MAT_3COLOR_ELEM(out, out_step, km, lm, color)) +
                            abs(CC_MAT_3COLOR_ELEM(out, out_step, km, lm, color) - CC_MAT_3COLOR_ELEM(out, out_step, km, lm - 1, color)));
                      }
                      else {
                        gradI.y = (float)(abs(CC_MAT_3COLOR_ELEM(out, out_step, km, lp + 1, color) - CC_MAT_3COLOR_ELEM(out, out_step, km, lm, color))) * 2.0f;
                      }
                    }
                    else {
                      if (*(f + f_step * k + l - 1) != INSIDE) {
                        gradI.y = (float)(abs(CC_MAT_3COLOR_ELEM(out, out_step, km, lm, color) - CC_MAT_3COLOR_ELEM(out, out_step, km, lm - 1, color))) * 2.0f;
                      }
                      else {
                        gradI.y = 0;
                      }
                    }

                    gradI.x = -gradI.x;
                    dir = VectorScalMult(r, gradI);

                    if (fabs(dir) <= 0.01) {
                      dir = 0.000001f;
                    }
                    else {
                      dir = (float) fabs(VectorScalMult(r, gradI) / sqrt(VectorLength(r) * VectorLength(gradI)));
                    }

                    w = dst * dir;
                    Ia += (float) w * (float)(CC_MAT_3COLOR_ELEM(out, out_step, km, lm, color));
                    s += w;
                  }
                }
              }
            }

            {
              int out_val = cvRound((double) Ia / s);
              CC_MAT_3COLOR_ELEM(out, out_step, i - 1, j - 1, color) = CC_CAST_8U(out_val);
            }
          }

          *(f + f_step * i + j) = BAND;
          CvPriorityQueueFloatPush(Heap, i, j, dist);
        }
      }
    }
  }
  else if (out_cn == 1) {
    while (CvPriorityQueueFloatPop(Heap, &ii, &jj)) {
      *(f + f_step * ii + jj) = KNOWN;

      for (q = 0; q < 4; q++) {
        if (q == 0) {
          i = ii - 1;
          j = jj;
        }
        else if (q == 1) {
          i = ii;
          j = jj - 1;
        }
        else if (q == 2) {
          i = ii + 1;
          j = jj;
        }
        else if (q == 3) {
          i = ii;
          j = jj + 1;
        }

        if ((i <= 1) || (j <= 1) || (i > t_rows - 1) || (j > t_cols - 1)) {
          continue;
        }

        if (*(f + f_step * i + j) == INSIDE) {
          dist = min4(FastMarching_solve(i - 1, j, i, j - 1, f, f_step, t, t_step),
              FastMarching_solve(i + 1, j, i, j - 1, f, f_step, t, t_step),
              FastMarching_solve(i - 1, j, i, j + 1, f, f_step, t, t_step),
              FastMarching_solve(i + 1, j, i, j + 1, f, f_step, t, t_step));
          *(t + t_step * i + j) = dist;

          {
            FPOINT gradI, r;
            float Ia = 0, s = 1.0e-20f, w, dst, dir;

            for (k = i - range; k <= i + range; k++) {
              int km = k - 1 + (k == 1), kp = k - 1 - (k == t_rows - 2);

              for (l = j - range; l <= j + range; l++) {
                int lm = l - 1 + (l == 1), lp = l - 1 - (l == t_cols - 2);

                if (k > 0 && l > 0 && k < t_rows - 1 && l < t_cols - 1) {
                  if ((*(f + f_step * k + l) != INSIDE) &&
                      ((l - j) * (l - j) + (k - i) * (k - i) <= range * range)) {
                    r.y = (float)(i - k);
                    r.x = (float)(j - l);

                    dst = 1 / (VectorLength(r) * VectorLength(r) + 1);

                    if (*(f + f_step * (k + 1) + l) != INSIDE) {
                      if (*(f + f_step * (k - 1) + l) != INSIDE) {
                        gradI.x = (float)(abs(X_MAT_ELEM(out, out_step, kp + 1, lm) - X_MAT_ELEM(out, out_step, kp, lm)) +
                            abs(X_MAT_ELEM(out, out_step, kp, lm) - X_MAT_ELEM(out, out_step, km - 1, lm)));
                      }
                      else {
                        gradI.x = (float)(abs(X_MAT_ELEM(out, out_step, kp + 1, lm) - X_MAT_ELEM(out, out_step, kp, lm))) * 2.0f;
                      }
                    }
                    else {
                      if (*(f + f_step * (k - 1) + l) != INSIDE) {
                        gradI.x = (float)(abs(X_MAT_ELEM(out, out_step, kp, lm) - X_MAT_ELEM(out, out_step, km - 1, lm))) * 2.0f;
                      }
                      else {
                        gradI.x = 0;
                      }
                    }

                    if (*(f + f_step * k + l + 1) != INSIDE) {
                      if (*(f + f_step * k + l - 1) != INSIDE) {
                        gradI.y = (float)(abs(X_MAT_ELEM(out, out_step, km, lp + 1) - X_MAT_ELEM(out, out_step, km, lm)) +
                            abs(X_MAT_ELEM(out, out_step, km, lm) - X_MAT_ELEM(out, out_step, km, lm - 1)));
                      }
                      else {
                        gradI.y = (float)(abs(X_MAT_ELEM(out, out_step, km, lp + 1) - X_MAT_ELEM(out, out_step, km, lm))) * 2.0f;
                      }
                    }
                    else {
                      if (*(f + f_step * k + l - 1) != INSIDE) {
                        gradI.y = (float)(abs(X_MAT_ELEM(out, out_step, km, lm) - X_MAT_ELEM(out, out_step, km, lm - 1))) * 2.0f;
                      }
                      else {
                        gradI.y = 0;
                      }
                    }

                    gradI.x = -gradI.x;
                    dir = VectorScalMult(r, gradI);

                    if (fabs(dir) <= 0.01) {
                      dir = 0.000001f;
                    }
                    else {
                      dir = (float) fabs(VectorScalMult(r, gradI) / sqrt(VectorLength(r) * VectorLength(gradI)));
                    }

                    w = dst * dir;
                    Ia += (float) w * (float)(X_MAT_ELEM(out, out_step, km, lm));
                    s += w;
                  }
                }
              }
            }

            {
              int out_val = cvRound((double) Ia / s);
              X_MAT_ELEM(out, out_step, i - 1, j - 1) = CC_CAST_8U(out_val);
            }
          }

          *(f + f_step * i + j) = BAND;
          CvPriorityQueueFloatPush(Heap, i, j, dist);
        }
      }
    }

  }
}

#define SET_BORDER1_C1(image_rows,image_cols,image,image_step,value) {\
    int i,j;\
    for(j=0; j<image_cols; j++) {\
      X_MAT_ELEM(image,image_step,0,j) = value;\
    }\
    for (i=1; i<image_rows-1; i++) {\
      X_MAT_ELEM(image,image_step,i,0) = X_MAT_ELEM(image,image_step,i,image_cols-1) = value;\
    }\
    for(j=0; j<image_cols; j++) {\
      X_MAT_ELEM(image,image_step,erows-1,j) = value;\
    }\
  }

#define COPY_MASK_BORDER1_C1(src_rows, src_cols, src, src_step, dst, dst_step) {\
    int i,j;\
    for (i=0; i<src_rows; i++) {\
      for(j=0; j<src_cols; j++) {\
        if (X_MAT_ELEM(src,src_step,i,j)!=0)\
          X_MAT_ELEM(dst,dst_step,i+1,j+1) = INSIDE;\
      }\
    }\
  }



// All the input and output images must have the same size
// Only 8-bit 1-channel and 3-channel input/output images are supported
// The mask must be 8-bit 1-channel image
void
cvInpaint2(int input_img_rows, int input_img_cols,
    const unsigned char* input_img, int input_img_step,
    const unsigned char* inpaint_mask, int inpaint_mask_step,
    unsigned char* output_img, int output_img_step,
    int imgcn, double inpaintRange, int flags)
{
  unsigned char* mask = 0, *band = 0, *f = 0, *out = 0, *mask1 = 0;
  float* t = 0;
  CvPriorityQueueFloat Heap, Out;
  //IplConvKernel *el_cross = 0, *el_range = 0;
  // CvMat input_hdr, mask_hdr, output_hdr;
  // CvMat* input_img, *inpaint_mask, *output_img;
  int range = cvRound(inpaintRange);
  int erows, ecols, esize;

  __CC_BEGIN__;

  range = MAX(range, 1);
  range = MIN(range, 100);

  erows = input_img_rows + 2;
  ecols = input_img_cols + 2;
  esize = erows * ecols;

  f = MALLOC(unsigned char, esize);
  t = MALLOC(float, esize);
  band = MALLOC(unsigned char, esize);
  mask = MALLOC(unsigned char, esize);
  mask1 = mask + ecols + 1;

  MEMCPY2D(input_img_rows, input_img_cols * imgcn, input_img, input_img_step, output_img, output_img_step);

  FILL(esize, mask, KNOWN);
  COPY_MASK_BORDER1_C1(input_img_rows, input_img_cols, inpaint_mask, inpaint_mask_step, mask, ecols);
  SET_BORDER1_C1(erows, ecols, mask, ecols, 0);
  FILL(esize, f, KNOWN);
  FILL(esize, t, 1.0e6f);
  {

    SKIP_FILTER(input_img_rows, input_img_cols, mask1, ecols, 1, band, ecols, 1, DILATE_CROSS_3x3);
#if 0

    el_cross = cvCreateStructuringElementEx(3, 3, 1, 1, CC_SHAPE_CROSS, NULL);
    cvDilate(mask, band, el_cross, 1);     // image with narrow band
    cvReleaseStructuringElement(&el_cross);
#endif

  }

  if (!CvPriorityQueueFloatInit(&Heap, erows, ecols, band, ecols)) {
    EXIT;
  }

  //cvSub(band,mask,band,NULL);
  FOR_EACH_OP_1D2A(esize, band, 1, mask, 1, -=);
  SET_BORDER1_C1(erows, ecols, band, ecols, 0);

  if (!CvPriorityQueueFloatAdd(&Heap, erows, ecols, band, ecols)) {
    EXIT;
  }

#define IF_SET_BAND(_A, _B)  if (_B) { _A = BAND; }

  FOR_EACH_1D2A(esize, f, 1, band, 1, IF_SET_BAND);
#undef IF_SET_BAND
#define IF_SET_INSIDE(_A, _B)  if (_B) { _A = INSIDE; }

  FOR_EACH_1D2A(esize, f, 1, mask, 1, IF_SET_INSIDE);
#undef IF_SET_INSIDE
#define IF_SET_0(_A, _B)  if (_B) { _A = 0; }

  FOR_EACH_1D2A(esize, f, 1, band, 1, IF_SET_0);
#undef IF_SET_0
  //cvSet(f,cvScalar(BAND,0,0,0),band);
  //cvSet(f,cvScalar(INSIDE,0,0,0),mask);
  //cvSet(t,cvScalar(0,0,0,0),band);

  if (flags == CC_INPAINT_TELEA) {
    unsigned char* out1;
    out = MALLOC(unsigned char, esize);
    memset(out, 0, esize);
    out1 = out + ecols + 1;
    //el_range = cvCreateStructuringElementEx(2*range+1,2*range+1, range,range,CC_SHAPE_RECT,NULL);
    //cvDilate(mask,out,el_range,1);
    SKIP_FILTER(input_img_rows, input_img_cols, mask1, ecols, 1, out1, ecols, 1, DILATE_RECT_3x3);
    //cvSub(out,mask,out,NULL);
    FOR_EACH_OP_1D2A(esize, out, 1, mask, 1, -=);

    if (!CvPriorityQueueFloatInit(&Out, erows, ecols, out, ecols)) {
      EXIT;
    }

    if (!CvPriorityQueueFloatAdd(&Out, erows, ecols, band, ecols)) {
      EXIT;
    }

    //cvSub(out,band,out,NULL);
    FOR_EACH1D_OP(esize, out, 1, band, 1, out, 1, -);
    SET_BORDER1_C1(erows, ecols, out, ecols, 0);
    //icvCalcFMM(out,t,Out,TRUE);
    icvCalcFMM(erows, ecols, out, ecols, t, ecols, &Out, TRUE);

    //icvTeleaInpaintFMM(mask,t,output_img,range,Heap);
    icvTeleaInpaintFMM(erows, ecols, mask, ecols, erows, ecols, t, ecols,
        output_img, output_img_step, imgcn, range, &Heap);
  }
  else {
    //icvNSInpaintFMM(mask,t,output_img,range,Heap);
    icvNSInpaintFMM(erows, ecols, mask, ecols, erows, ecols, t, ecols,
        output_img, output_img_step, imgcn, range, &Heap);
  }

  __END__;

  CvPriorityQueueFloatUnInit(&Out);
  CvPriorityQueueFloatUnInit(&Heap);
  //cvReleaseStructuringElement(&el_range);
  FREE(out);
  FREE(mask);
  FREE(band);
  FREE(t);
  FREE(f);
}

