#ifndef _SEQPARTITION_H_
#define _SEQPARTITION_H_


#include "stdc/stddef_c.h"
#include "stdc/mem_c.h"
#include "stdc/algo.h"

#define REAL_TYPE double

// out == 1
// out == 0 ??????? double threshold=0.3;
// ?????.w = MIN((r1?????-r2?????), (r2?????-r1?????))
// ?????? (area(r1) + area(r2))/2
#define IS_RECT_EQUAL_COVER_RETE12(r1, r2, THRESHOLD, OUT) \
  do { \
    int dx, dy; \
    if ((dx = (MIN(((r1).x+(r1).w-(r2).x), ((r2).x+(r2).w-(r1).x)))) < 0 \
    || (dy = (MIN(((r1).y+(r1).h-(r2).y), ((r2).y+(r2).h-(r1).y)))) < 0) {\
      (OUT) = 0; \
    } else { \
      (OUT) = (2*dx*dy) > (THRESHOLD * ((r1).w*(r1).h + (r2).w*(r2).h)); \
    } \
  } while(0)
// line[a0, a1] ???? line[b0, b1]
#define IS_LINE_INCLUDE(a0, a1, b0, b1)  ((a0)<=(b0) && (a1)>=(b1))
// r1 ???? r2
#define IS_RECT_INCLUDE(r1, r2) \
  ((IS_LINE_INCLUDE((r1).x, (r1).x+(r1).w, (r2).x, (r2).x+(r2).w) && \
      IS_LINE_INCLUDE((r1).y, (r1).y+(r1).h, (r2).y, (r2).y+(r2).h) ) \
      && ((r1).w < ((r2).w*2)))
#define IS_RECT_EQUAL_COVER_RETE(r1, r2, THRESHOLD, OUT) \
  do { \
    int dx, dy, covarea, avgarea; \
    (OUT) = IS_RECT_INCLUDE(r1, r2) || IS_RECT_INCLUDE(r2, r1); \
    if (OUT) { break; } \
    dx = MIN(((r1).x+(r1).w-(r2).x), ((r2).x+(r2).w-(r1).x)); \
    dy = MIN(((r1).y+(r1).h-(r2).y), ((r2).y+(r2).h-(r1).y)); \
    covarea=dx*dy; \
    if (dx < 0 || dy < 0 ) { (OUT) = 0; break; } \
    avgarea=((r1).w*(r1).h + (r2).w*(r2).h)/2; \
    (OUT) = (covarea >= (THRESHOLD)*avgarea); \
  } while(0)

#define SEQPARTITION(total, seq, labels, is_equal, class_cnt, TYPE)           \
  do {                                                                          \
    typedef struct CvPTreeNode {                                                \
      struct CvPTreeNode* parent;                                               \
      TYPE* element;                                                            \
      int rank;                                                                 \
    }                                                                           \
    CvPTreeNode;                                                                \
    int i, j, class_idx = 0;                                                    \
    CvPTreeNode* nodes;                                                         \
    nodes = malloc(sizeof(CvPTreeNode) * total);                                \
    for( i = 0; i < total; i++ ) {                                              \
      CvPTreeNode node = { 0, 0, 0 };                                           \
      node.element = (TYPE*)(seq + i);                                          \
      nodes[i] = node;                                                          \
    }                                                                           \
    /* The main O(N^2) pass. Merge connected components. */                     \
    for( i = 0; i < total; i++ ) {                                              \
      CvPTreeNode* node = nodes + i;                                            \
      CvPTreeNode* root = node;                                                 \
      /* find root */                                                           \
      while( root->parent ) {                                                   \
        root = root->parent;                                                    \
      }                                                                         \
      for( j = 0; j < total; j++ ) {                                            \
        CvPTreeNode* node2 = nodes + j;                                         \
        if( node2->element && node2 != node ) {                                 \
          int out;                                                              \
          is_equal(node->element, node2->element, out);                         \
          if (out) {                                                            \
            CvPTreeNode* root2 = node2;                                         \
            /* unite both trees */                                              \
            while( root2->parent ) {                                            \
              root2 = root2->parent;                                            \
            }                                                                   \
            if( root2 != root ) {                                               \
              if( root->rank > root2->rank ) {                                  \
                root2->parent = root;                                           \
              } else {                                                          \
                root->parent = root2;                                           \
                root2->rank += root->rank == root2->rank;                       \
                root = root2;                                                   \
              }                                                                 \
              /* compress path from node2 to the root */                        \
              while( node2->parent ) {                                          \
                CvPTreeNode* temp = node2;                                      \
                node2 = node2->parent;                                          \
                temp->parent = root;                                            \
              }                                                                 \
              /* compress path from node to the root */                         \
              node2 = node;                                                     \
              while( node2->parent ) {                                          \
                CvPTreeNode* temp = node2;                                      \
                node2 = node2->parent;                                          \
                temp->parent = root;                                            \
              }                                                                 \
            }                                                                   \
          }                                                                     \
        }                                                                       \
      }                                                                         \
    }                                                                           \
    /* Final O(N) pass (Enumerate classes) */                                   \
    /* Reuse reader one more time */                                            \
    for( i = 0; i < total; i++ ) {                                              \
      CvPTreeNode* node = nodes + i;                                            \
      int idx = -1;                                                             \
      if( node->element ) {                                                     \
        while( node->parent ) {                                                 \
          node = node->parent;                                                  \
        }                                                                       \
        if( node->rank >= 0 ) {                                                 \
          node->rank = ~class_idx++;                                            \
        }                                                                       \
        idx = ~node->rank;                                                      \
      }                                                                         \
      labels[i] = idx;                                                          \
    }                                                                           \
    free(nodes);                                                                \
    class_cnt = class_idx;                                                      \
  } while(0)
typedef struct CvPTreeNode {
  struct CvPTreeNode* parent;
  char* element;
  int rank;
}
CvPTreeNode;
typedef struct seqpartition_info_t {
  int total;
  int element_size;
  int class_cnt;
  int* labels;
  char* seq_center;
  CvPTreeNode* center;
  CvPTreeNode* nodes;
}
seqpartition_info_t;
#define SEQPARTITIONSIZE(_ELEMENT_SIZE, _TOTAL) (sizeof(seqpartition_info_t) + (2*sizeof(CvPTreeNode)+sizeof(int)+_ELEMENT_SIZE) * _TOTAL)
static seqpartition_info_t* seqpartition_create(size_t total, const void* seq, int element_size, void* buf, int* pbuflen)
{
  seqpartition_info_t* si = NULL;
  int buflen = SEQPARTITIONSIZE(element_size, total);
  if (NULL != pbuflen) {
    *pbuflen = buflen;
  }
  if (NULL != buf) {
    BUFUSEBEGIN(buf, buflen);
    int i;
    CvPTreeNode zeronode = {0};
    si = BUFMALLOC(seqpartition_info_t, 1);
    si->seq_center = BUFMALLOC(char, total * element_size);
    si->nodes = BUFMALLOC(CvPTreeNode, total);
    si->center = BUFMALLOC(CvPTreeNode, total);
    si->labels = BUFMALLOC(int, total);
    si->total = total;
    si->element_size = element_size;
    for (i = 0; i < total; i++) {
      si->nodes[ i ] = si->center[ i ] = zeronode;
      si->nodes[ i ].element = ((char*) seq + i * element_size);
      si->center[ i ].element = ((char*)(si->seq_center) + i * element_size);
    }
    BUFUSEEND();
  }
  return si;
}
static int seqpartition_process(seqpartition_info_t* si, int (*is_equal)(void*, void*, void*), void* user_data)
{
  int i, j, class_idx = 0, total;
  int* labels;
  CvPTreeNode* nodes;
  nodes = si->nodes;
  total = si->total;
  labels = si->labels;
  /* The main O(N^2) pass. Merge connected components. */
  for (i = 0; i < total; i++) {
    CvPTreeNode* node = nodes + i;
    CvPTreeNode* root = node;
    /* find root */
    while (root->parent) {
      root = root->parent;
    }
    for (j = 0; j < total; j++) {
      CvPTreeNode* node2 = nodes + j;
      if (node2->element && node2 != node) {
        int out;
        out = is_equal(node->element, node2->element, user_data);
        if (out) {
          CvPTreeNode* root2 = node2;
          /* unite both trees */
          while (root2->parent) {
            root2 = root2->parent;
          }
          if (root2 != root) {
            if (root->rank > root2->rank) {
              root2->parent = root;
            }
            else {
              root->parent = root2;
              root2->rank += root->rank == root2->rank;
              root = root2;
            }
            /* compress path from node2 to the root */
            while (node2->parent) {
              CvPTreeNode* temp = node2;
              node2 = node2->parent;
              temp->parent = root;
            }
            /* compress path from node to the root */
            node2 = node;
            while (node2->parent) {
              CvPTreeNode* temp = node2;
              node2 = node2->parent;
              temp->parent = root;
            }
          }
        }
      }
    }
  }
  /* Final O(N) pass (Enumerate classes) */
  /* Reuse reader one more time */
  for (i = 0; i < total; i++) {
    CvPTreeNode* node = nodes + i;
    int idx = -1;
    if (node->element) {
      while (node->parent) {
        node = node->parent;
      }
      if (node->rank >= 0) {
        node->rank = ~class_idx++;
      }
      idx = ~node->rank;
    }
    labels[ i ] = idx;
  }
  si->class_cnt = class_idx;
  return 0;
}
// 给 xrect 赋值
#define MAKE_XRECT(X, Y, CX, CY, OUT) { (OUT).x = (X); (OUT).y = (Y); (OUT).w = (CX); (OUT).h = (CY); }
// a+=b
#define RECT_ADD(a, b)  { (a).x+=(b).x; (a).y+=(b).y; (a).w+=(b).w; (a).h+=(b).h; }
#define XRECT_ADD(a, b)  { (a).x+=(b).x; (a).y+=(b).y; (a).w+=(b).w; (a).h+=(b).h; (a).score+=(b).score; (a).count+=(b).count; }
// a/=b
#define RECT_MUL(a, b)  { (a).x=(int)((a).x*(b)); (a).y=(int)((a).y*(b)); (a).w=(int)((a).w*(b)); (a).h=(int)((a).h*(b)); }
#define RECT_DIV(a, b)  { (a).x/=(b); (a).y/=(b); (a).w/=(b); (a).h/=(b); }
#define XRECT_DIV(a, b)  { (a).x/=(b); (a).y/=(b); (a).w/=(b); (a).h/=(b); (a).score/=(b); (a).count*=(b); }
int is_rect_equal_3(XRECT* r1, XRECT* r2, REAL_TYPE* thd)
{
  int out;
  IS_RECT_EQUAL_COVER_RETE((*r1), (*r2), (*thd), out);
  return out;
}
int seqpartition_center_xrect(seqpartition_info_t* si)
{
  int i;
  CvPTreeNode* center = si->center;
  CvPTreeNode* nodes = si->nodes;
  int* labels = si->labels;
  int class_cnt = si->class_cnt;
  int total = si->total;
  for (i = 0; i < class_cnt; ++i) {
    XRECT zeronode = {0};
    *((XRECT*)(center[ i ].element)) = zeronode;
  }
  for (i = 0; i < total; ++i) {
    int idx = labels[ i ];
    XRECT_ADD(*((XRECT*)(center[ idx ].element)), *((XRECT*)(nodes[ i ].element)));
  }
  //printf("%d\n", class_cnt);
  for (i = 0; i < class_cnt; ++i) {
    RECT_DIV(*((XRECT*)(center[ i ].element)), (*((XRECT*)(center[ i ].element))).count);
  }
  return 0;
}
int is_int_equal_3(int* a, int* b, REAL_TYPE* thd)
{
  return abs(*a - *b) < *thd;
}
typedef struct int_count_t {
  int i;
  int count;
} int_count_t;
int int_partition(int n, const int* B, REAL_TYPE thd, int mincnt, int sort, int maxclass, int* out0, buf_t* bf)
{
  int total = n, i, class_cnt = 0;
  //char buf[SEQPARTITIONSIZE(sizeof(XRECT), 100)];
  //CvPTreeNode* center;
  seqpartition_info_t* si;
  int_count_t* out = 0;
  int buflen = bf->len;
	UNUSED(mincnt);
  BFMALLOC(bf, out, n);
  si = seqpartition_create(total, B, sizeof(int), bf->data, &buflen);
  if (NULL == si) {
    return 0;
  }
  seqpartition_process(si, (int (*)(void*, void*, void*))is_int_equal_3, &thd);
  {
    CvPTreeNode* center = si->center;
    CvPTreeNode* nodes = si->nodes;
    int* labels = si->labels;
    int total = si->total;
    class_cnt = si->class_cnt;
    for (i = 0; i < class_cnt; ++i) {
      out[i].i = 0;
      out[i].count = 0;
    }
    for (i = 0; i < total; ++i) {
      int idx = labels[ i ];
      out[ idx ].i += *((int*)(nodes[ i ].element));
      out[ idx ].count += 1;
    }
    for (i = 0; i < class_cnt; ++i) {
      out[ i ].i /= out[i].count;
    }
    if (1) {
#define INT_COUNT_LESS(x, y)  ((x).count > (y).count)
      INSERT_SORT(out, (out + class_cnt-1), INT_COUNT_LESS, int_count_t);
#undef INT_COUNT_LESS
    }
  }
  //center = si->center;
  class_cnt = MIN(maxclass, class_cnt);
  for (i = 0; i < class_cnt; ++i) {
    out0[ i ] = out[i].i;
  }
  BFFREE(bf, out);
  return class_cnt;
}
// 矩形框聚类
// rc0 矩形框头指针
// B 矩形框尾指针
// thd 聚类用的最小覆盖率值，两个框的覆盖率值大于thd就会被分为一类
// mincnt 最小聚类数，每一类所包含的框的最小个数，小于这个数的类将被抛弃
// sort 是否排序，sort非零则按每一类所包含的框的个数，由大到小的顺序排列
int rect_partition(XRECT* rc0, XRECT* B, REAL_TYPE thd, int mincnt, int sort, void* buf, int* pbuflen)
{
  int class_cnt = 0;
  XRECT* B_end = B;
	UNUSED(sort);
  {
    //mincnt = (mincnt*(B-rc0)+50)/100;
    //printf("%d\n", mincnt);
  }
#if 0
  {
#define IS_RECT_EQUAL_3(r1, r2, OUT)  IS_RECT_EQUAL_COVER_RETE(r1, r2, thd, OUT)
    RECT_PARTITION(XRECT*, rc0, B, IS_RECT_EQUAL_3, mincnt, XRECT, XRECT_ADD, XRECT_DIV, B);
    RECT_PARTITION(XRECT*, rc0, B, IS_RECT_EQUAL_3, 0, XRECT, XRECT_ADD, XRECT_DIV, B);
#undef IS_RECT_EQUAL_3
  }
#else
#if 0
  {
#define IS_RECT_EQUAL_3(r1, r2, OUT)  IS_RECT_EQUAL_COVER_RETE((*r1), (*r2), thd, OUT)
    int total = (B - rc0), i, j;
    int* labels = MALLOC(int, sizeof(int) * total);
    XRECT* B2;
    memset(labels, 0, sizeof(int) * total);
    SEQPARTITION(total, rc0, labels, IS_RECT_EQUAL_3, class_cnt, XRECT);
    B2 = MALLOC(XRECT, sizeof(XRECT) * class_cnt);
    memset(B2, 0, sizeof(XRECT) * class_cnt);
    for (i = 0; i < total; ++i) {
      int idx = labels[ i ];
      XRECT_ADD(B2[ idx ], rc0[ i ]);
    }
    //printf("%d\n", class_cnt);
    for (i = 0, j = 0; i < class_cnt; ++i) {
      //printf("%d\n", B2[i].count);
      if (B2[ i ].count > mincnt) {
        RECT_DIV(B2[ i ], B2[ i ].count);
        rc0[ j++ ] = B2[ i ];
      }
    }
    B = rc0 + j;
    SAFEFREE(B2);
    SAFEFREE(labels);
#undef IS_RECT_EQUAL_3
  }
#else
  {
    size_t total = (B - rc0);
    int i, j;
    //char buf[SEQPARTITIONSIZE(sizeof(XRECT), 100)];
    CvPTreeNode* center;
    seqpartition_info_t* si;
    si = seqpartition_create(total, rc0, sizeof(XRECT), buf, pbuflen);
    if (NULL == si) {
      return 0;
    }
    seqpartition_process(si, (int (*)(void*, void*, void*))is_rect_equal_3, &thd);
    seqpartition_center_xrect(si);
    center = si->center;
    for (i = 0, j = 0; i < si->class_cnt; ++i) {
      XRECT* pr = (XRECT*)(center[ i ].element);
      if (pr->count > mincnt) {
        rc0[ j++ ] = *pr;
      }
    }
    B = rc0 + j;
    class_cnt = j;
  }
#endif
#endif
  if (1) {
    //*B=*rc0;rc0->w--;rc0->h--;++B;
#define XRECT_LESS(x, y)  ((x).w > (y).w)
    INSERT_SORT(rc0, (B - 1), XRECT_LESS, XRECT);
#undef XRECT_LESS
  }
  if (0) {
    int i, n = B - rc0;
    for (i = 1; i < n; ++i) {
      if (rc0[0].w < rc0[i].w) {
        rc0[0] = rc0[i];
      }
    }
  }
  {
    XRECT* B1 = B;
    for (; B1 != B_end; ++B1) {
      MAKE_XRECT(0, 0, 0, 0, *B1);
    }
  }
  return class_cnt;
}
#endif // _SEQPARTITION_H_

