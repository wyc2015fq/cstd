
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "macro.h"

typedef ushort arrtype;

typedef double scalar_type;
typedef double accum_type;
typedef arrtype* value_type;
typedef arrtype* __instype;
typedef double accum_type;

typedef struct dist_t {
  int id;
  double dist;
} dist_t;


#define ctor(x)  (x)
#define deref(_A, x)  *(_A + x)

typedef double(*distfun_t)(int n, const void* a, const void* b);

typedef struct bbf_nn {  // info on found neighbors (approx k nearest)
  arrtype* p;          // nearest neighbor
  double dist;  // distance from d to query point
}
bbf_nn;

typedef struct bbf_node {  // info on branches not taken
  int node;   // corresponding node
  accum_type dist;  // minimum distance from bounds to query point
}
bbf_node;

#define bbf_less(lhs, rhs)    ((lhs).dist<(rhs).dist)
#define bbf_greater(lhs, rhs) ((lhs).dist>(rhs).dist)

typedef struct KDNODE {
  int dim;          // split dimension;>=0 for nodes, -1 for leaves
  int left;
  int right;        // node indices of left and right branches
  arrtype* value;        // if leaf, value of leaf
  double boundary;    // left if deref(value,dim)<=boundary, otherwise right
}
KDNODE;

typedef struct kdtree_t {
  KDNODE* nodes;  // node storage
  int nodes_size;
  int point_dim;  // dimension of points (the k in kd-tree)
  int root_node;  // index of root node, -1 if empty tree
  arrtype* mat;
  int mat_step;
  bbf_node* tmp_pq;
  int tmp_pq_size;
}
kdtree_t;


void KD_print(kdtree_t* tr, int i, int indent)
{
  int j;
  const KDNODE* n;

  if (i == -1) {
    return ;
  }

  for (j = 0; j < indent; ++j) {
    printf(" ");
  }

  n = tr->nodes + i;

  if (n->dim >= 0) {
    printf("node %d, left %d, right %d, dim %d, boundary %f\n", i, n->left, n->right, n->dim, n->boundary);
    KD_print(tr, n->left, indent + 3);
    KD_print(tr, n->right, indent + 3);
  }
  else {
    printf("leaf %d, value = %f\n", i, *n->value);
  }
}

// for given set of point indices, compute dimension of highest variance
int dimension_of_highest_variance(__instype* first, __instype* last,
    int point_dim)
{
  int j, maxj = -1;
  __instype* k;
  accum_type maxvar;
  assert(last - first > 0);
  maxvar = -FLT_MAX;

  for (j = 0; j < point_dim; ++j) {
#if 1
    accum_type mean = 0;
    accum_type var = 0;

    for (k = first; k < last; ++k) {
      mean += deref(ctor(*k), j);
    }

    mean /= last - first;

    for (k = first; k < last; ++k) {
      accum_type diff = deref(ctor(*k), j) - mean;
      var += diff * diff;
    }

    var /= last - first;
#else

    accum_type mean = 0;
    accum_type var = 0;

    for (k = first; k < last; ++k) {
      accum_type t = deref(ctor(*k), j);
      mean += t;
      var += t * t;
    }

    mean /= last - first;
    var /= last - first;
    var = var - mean * mean;
#endif

    assert(maxj != -1 || var >= maxvar);

    if (var >= maxvar) {
      maxvar = var;
      maxj = j;
    }
  }

  return maxj;
}

__instype _pivot;
int _dim;

int median_pr(const __instype lhs)
{
  return deref(ctor(lhs), _dim) <= deref(ctor(_pivot), _dim);
}

typedef int (*_Pr)(const __instype);
typedef __instype* _BI;

_BI partition(_BI _F, _BI _L, _Pr _P)
{
  __instype t;

  for (; ; ++_F) {
    for (; _F != _L && _P(*_F); ++_F)
      ;

    if (_F == _L) {
      break;
    }

    for (; _F != --_L && !_P(*_L);)
      ;

    if (_F == _L) {
      break;
    }

    CV_SWAP(*_F, *_L, t);
  }

  return (_F);
}

void median_partition_0(__instype* first, __instype* last, __instype* k, int dim)
{
  __instype t;
  __instype* middle;
  int pivot = (last - first) / 2;

  CV_SWAP(first[ pivot ], last[ -1 ], t);
  _pivot = last[ -1 ];
  _dim = dim;
  middle = partition(first, last - 1, median_pr);
  CV_SWAP(*middle, last[ -1 ], t);

  if (middle < k) {
    median_partition_0(middle + 1, last, k, dim);
  }
  else if (middle > k) {
    median_partition_0(first, middle, k, dim);
  }
}

// given point indices and dimension, find index of median; (almost) modifies [first,last)
// such that points_in[first,median]<=point[median], points_in(median,last)> point[median].
// implemented as partial quicksort; expected linear perf.
__instype* median_partition(__instype* first, __instype* last, int dim)
{
  __instype* k;
  assert(last - first > 0);
  k = first + (last - first) / 2;
  median_partition_0(first, last, k, dim);
  return k;
}

// insert given points into the tree; return created node
int KD_insert(kdtree_t* tr, __instype* first, __instype* last)
{
  if (first == last) {
    return -1;
  }
  else {
    int dim;
    __instype* median;
    __instype* split;
    dim = dimension_of_highest_variance(first, last, tr->point_dim);
    median = median_partition(first, last, dim);

    split = median;

    for (; split != last && deref(ctor(*split), dim) ==
        deref(ctor(*median), dim); ++split)
      ;

    if (split == last) {   // leaf
      int nexti = -1;

      for (--split; split >= first; --split) {
        int i = tr->nodes_size;
        KDNODE* n = (tr->nodes + tr->nodes_size++);
        n->dim = -1;
        n->value = ctor(*split);
        n->left = -1;
        n->right = nexti;
        nexti = i;
      }

      return nexti;
    }
    else {   // node
      int i = tr->nodes_size;
      // note that recursive insert may invalidate this ref
      KDNODE* n = (tr->nodes + tr->nodes_size++);

      n->value = NULL;
      n->dim = dim;
      n->boundary = deref(ctor(*median), dim);

      tr->nodes[ i ].left = KD_insert(tr, first, split);
      tr->nodes[ i ].right = KD_insert(tr, split, last);

      return i;
    }
  }
}

typedef double __desctype;

// called for branches not taken, as bbf walks to leaf;
// construct bbf_node given minimum distance to bounds of alternate branch
void pq_alternate(kdtree_t* tr, int alt_n, scalar_type dist)
{
  bbf_node t;

  if (alt_n == -1) {
    return ;
  }

  // add bbf_node for alternate branch in priority queue
  //pq.push_back(bbf_node(alt_n, dist));
  tr->tmp_pq[ tr->tmp_pq_size ].node = alt_n;
  tr->tmp_pq[ tr->tmp_pq_size ].dist = dist;
  tr->tmp_pq_size++;
  PUSH_HEAP(tr->tmp_pq_size, tr->tmp_pq, bbf_greater, t);
}

// called by bbf to walk to leaf;
// takes one step down the tree towards query point d
int bbf_branch(kdtree_t* tr, int i, const arrtype* d)
{
  const KDNODE* n = tr->nodes + i;

  // push bbf_node with bounds of alternate branch, then branch
  if (d[ n->dim ] <= n->boundary) {   // left
    pq_alternate(tr, n->right, n->boundary - d[ n->dim ]);
    return n->left;
  }
  else {     // right
    pq_alternate(tr, n->left, d[ n->dim ] - n->boundary);
    return n->right;
  }
}

// 在特征树结构中找[desc_rows, dim, desc, desc_step]中每个行特征的k个最近的id(放在results中)
// 和相应的距离(放在dist中)
// results 是[desc_rows×k] 的矩阵
// dist 是[desc_rows×k] 的矩阵
// emax 越大速度越慢，精度越高，反之速度越快，精度越低（一般取10）
// finds (with high probability) the k nearest neighbors of d,
// searching at most emax leaves/bins.
// ret_nn_pq is an array containing the (at most) k nearest neighbors
// (see bbf_nn structure def above).
int KD_find_nn(kdtree_t* tr, const arrtype* d, int k, int emax, distfun_t disfun, dist_t* out)
{
  bbf_nn* nn_pq = 0;
  int nn_pq_size = 0;
  int i;
  bbf_node t;
  assert(k > 0);

  if (tr->root_node == -1) {
    return 0;
  }

  nn_pq = MALLOC(bbf_nn, k);

  assert((int) nn_pq_size <= k);

  // add root_node to bbf_node priority queue;
  // iterate while queue non-empty and emax> 0
  tr->tmp_pq[ 0 ].node = tr->root_node;
  tr->tmp_pq[ 0 ].dist = 0;
  tr->tmp_pq_size = 1;

  while (tr->tmp_pq_size && emax > 0) {
    int i;
    bbf_node bbf;
    // from node nearest query point d, run to leaf
    POP_HEAP(tr->tmp_pq_size, tr->tmp_pq, bbf_greater, t);
    bbf = tr->tmp_pq[ --tr->tmp_pq_size ];

    for (i = bbf.node; i != -1 && tr->nodes[ i ].dim >= 0; i = bbf_branch(tr, i, d))
      ;

    if (i != -1) {
      // add points in leaf/bin to ret_nn_pq
      do {
        // called per candidate nearest neighbor; constructs new bbf_nn for
        // candidate and adds it to priority queue of all candidates; if
        // queue len exceeds k, drops the point furthest from query point d.
        // bbf_new_nn(nn_pq, k, d, tr->nodes[i].value);
        arrtype* p = tr->nodes[ i ].value;
        bbf_nn nn_tmp;
        bbf_nn nn;
        nn.p = p;
        nn.dist = disfun(tr->point_dim, d, p);

        if ((int) nn_pq_size < k) {
          nn_pq[ nn_pq_size++ ] = (nn);
          PUSH_HEAP(nn_pq_size, nn_pq, bbf_less, nn_tmp);
        }
        else if (nn_pq[ 0 ].dist > nn.dist) {
          POP_HEAP(nn_pq_size, nn_pq, bbf_less, nn_tmp);
          nn_pq[ nn_pq_size - 1 ] = nn;
          PUSH_HEAP(nn_pq_size, nn_pq, bbf_less, nn_tmp);
        }

        assert(nn_pq_size < 2 || nn_pq[ 0 ].dist >= nn_pq[ 1 ].dist);
      }
      while (-1 != (i = tr->nodes[ i ].right));

      --emax;
    }
  }

  tr->tmp_pq_size = 0;

  for (i = 0; i < nn_pq_size; ++i) {
    out[i].id = (nn_pq[ i ].p - tr->mat) / tr->mat_step;
    out[i].dist = nn_pq[ i ].dist;
  }


  FREE(nn_pq);
  return nn_pq_size;
}

int knn_find(int h, int n, arrtype* A, int al, arrtype* d, int k, distfun_t disfun, dist_t* out)
{
  dist_t* nn = MALLOC(dist_t, h);
  int i;

  for (i = 0; i < h; ++i, A += al) {
    nn[i].id = i;
    nn[i].dist = disfun(n, A, d);
  }

#define DISLESS(a, b)   ((a).dist<(b).dist)
  QSORT(nn, nn + h, DISLESS, dist_t);
#undef DISLESS
  k = MIN(h, k);

  for (i = 0; i < k; ++i) {
    out[i] = nn[i];
  }

  FREE(nn);
  return k;
}

// 创建特征树结构
// [desc_rows, desc_cols, desc, desc_step] 特征矩阵
int kd_init(kdtree_t* tr, int desc_rows, int desc_cols, arrtype* desc, int desc_step)
{
  __instype* first;
  __instype* last;
  int i;
  first = MALLOC(arrtype*, desc_rows);
  last = first + desc_rows;

  for (i = 0; i < desc_rows; ++i) {
    first[ i ] = desc + i * desc_step;
  }

  tr->mat = desc;
  tr->mat_step = desc_step;
  tr->nodes_size = 0;
  tr->point_dim = desc_cols;
  tr->nodes = MALLOC(KDNODE, desc_rows * 2);
  tr->tmp_pq = MALLOC(bbf_node, desc_cols);
  tr->tmp_pq_size = 0;
  tr->root_node = KD_insert(tr, first, last);
  FREE(first);
  return 0;
}

// 释放特征树结构
int kd_free(kdtree_t* tr)
{
  FREE(tr->nodes);
  FREE(tr->tmp_pq);
  return 0;
}

int find_ortho_range(KDNODE* nodes, int i, scalar_type* bounds_min,
    scalar_type* bounds_max,
    arrtype** inbounds,
    int inbounds_size,
    int inbounds_maxsize)
{
  KDNODE* n = nodes;

  if (i == -1) {
    return inbounds_size;
  }

  if (n->dim >= 0) {   // node
    if (bounds_min[ n->dim ] <= n->boundary) {
      find_ortho_range(nodes, n->left, bounds_min, bounds_max, inbounds, inbounds_size, inbounds_maxsize);
    }

    if (bounds_max[ n->dim ] > n->boundary) {
      find_ortho_range(nodes, n->right, bounds_min, bounds_max, inbounds, inbounds_size, inbounds_maxsize);
    }
  }
  else {   // leaf
    do {
      if (inbounds_size < inbounds_maxsize) {
        inbounds[ inbounds_size++ ] = (n->value);
      }
      else {
        return inbounds_size;
      }
    }
    while (-1 != (i = n->right));
  }

  return inbounds_size;
}


// run to leaf; linear search for p;
// if found, remove paths to empty leaves on unwind
int KD_remove(KDNODE* nodes, int* i, arrtype* p)
{
  int r;
  KDNODE* n;

  if (*i == -1) {
    return 0;
  }

  n = nodes + *i;

  if (n->dim >= 0) {   // node
    if (deref(p, n->dim) <= n->boundary) {     // left
      r = KD_remove(nodes, &n->left, p);
    }
    else {   // right
      r = KD_remove(nodes, &n->right, p);
    }

    // if terminal, remove this node
    if (n->left == -1 && n->right == -1) {
      *i = -1;
    }

    return r;
  }
  else {   // leaf
    if (n->value == p) {
      *i = n->right;
      return 1;
    }
    else {
      return KD_remove(nodes, &n->right, p);
    }
  }
}
