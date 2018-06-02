
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <float.h>



#ifndef MALLOC
#define MALLOC(TYPE, size)(TYPE*)pmalloc(sizeof(TYPE) *(size_t)(size))
#endif

#ifndef MAX
#define MAX(a, b)  ((a) > (b) ? (a) : (b))
#endif

/********************************************************************************
                        shen-castan算子的功能函数
********************************************************************************/

#define ISEF1D(_x, _y, _A, _width, _cn, b, b1, b2)                             \
  do {                                                                           \
    int _I;                                                                      \
    *(_A) = b1 * *(_x);                                                          \
    /* 计算相关成员参数 */                                                       \
    for (_I=1; _I<(_width); ++_I)                                                \
      *(_A+_I) = b1 * *(_x+_I*(_cn)) + b * *(_A+_I-1);                           \
    /* 计算ISEF滤波后图像的边缘情况 */                                           \
    *(_y+(_width-1)*(_cn)) = *(_A+_width-1);                                     \
    for (_I=0; _I<(_width)-1; ++_I)                                              \
      *(_y+_I*(_cn)) = *(_A+_I);                                                 \
    *(_A+_width-1) = b2 * *(_x+(_width-1)*(_cn));                                \
    /* 计算非相关成员参数 */                                                     \
    for (_I=(_width)-2; _I>=0; --_I)                                             \
      *(_A+_I) = b2 * *(_x+_I*(_cn)) + b * *(_A+_I+1);                           \
    for (_I=0; _I<(_width)-1; ++_I)                                              \
      *(_y+_I*(_cn)) += *(_A+_I+1);                                              \
  } while(0)

/******************************************************************************
  功能：用ISEF滤波器对图像进行滤波
  输入：x, y  - 存储图像的缓存区域
        nrows - 图像的行数
        ncols - 图像的列数
******************************************************************************/
void compute_ISEF(const float* x, float* y, int nrows, int ncols, int step, float b)
{
  int row, col;
  float b1 = (1.0f - b) / (1.0f + b);
  float b2 = b * b1;
  float* A = MALLOC(float, nrows > ncols ? nrows : ncols);
  float* z = MALLOC(float, nrows * ncols);

  /* 首先在垂直方向滤波 */
  for (col = 0; col < ncols; col += 1) {
    ISEF1D((x + col), (z + col), A, nrows, step, b, b1, b2);
  }

  /* 在水平方向滤波 */
  for (row = 0; row < nrows * ncols; row += ncols) {
    ISEF1D((z + row), (y + row), A, ncols, 1, b, b1, b2);
  }

  /* 释放内存 */
  pfree(A);
  pfree(z);
}

/******************************************************************************
  功能：计算像素点的梯度值
  输入：bli，orig-存储图像的缓冲区
******************************************************************************/
float compute_adaptive_gradient(const unsigned char* bli, const float* orig, int half_size, int step, int step2)
{
  register int i, j;
  float sum_on, sum_off;
  float avg_on, avg_off;
  int num_on, num_off;

  sum_on = sum_off = 0.0f;
  num_on = num_off = 0;

  for (i = (-half_size); i <= (half_size); i++) {
    const unsigned char* _bli = bli + i * step2;
    const float* _orig = orig + i * step;

    for (j = (-half_size); j <= (half_size); j++) {
      if (_bli[ j ]) {
        sum_on += _orig[ j ];
        num_on++;
      }
      else {
        sum_off += _orig[ j ];
        num_off++;
      }
    }
  }

  avg_off = (num_off) ? sum_off / num_off : 0;
  avg_on = (num_on) ? sum_on / num_on : 0;
  return (avg_off - avg_on);
}

/******************************************************************************
  功能：确定梯度的高、低门限值
  输入：nr-图像的行数
        nc-图像的列数
  输出：high_thresh - 高、低门限值
******************************************************************************/
double estimate_thresh(float* lap, int sz, float ratio)
{
  float vmax = -FLT_MAX, vmin = FLT_MAX, scale, x;
  int j, k, hist[ 256 ], count;
  memset(hist, 0, 256 * sizeof(int));

  for (j = 0; j < sz; j++) {
    x = lap[ j ];

    if (vmin > x) {
      vmin = x;
    }

    if (vmax < x) {
      vmax = x;
    }
  }

  scale = 256.0f / ((vmax - vmin) + 1.f);

  for (j = 0; j < sz; j++) {
    x = lap[ j ];
    k = (int)((x - vmin) * scale);
    ++(hist[ k ]);
  }

  k = 255;
  j = (int)(ratio * sz);
  count = hist[ 255 ];

  while (count < j) {
    k--;

    if (k < 0) {
      break;
    }

    count += hist[ k ];
  }

  return (double) k / scale + vmin;
}

/******************************************************************************
  功能：连接离散的边缘点
  输入：i,j-对应点的坐标
        level-
******************************************************************************/
int mark_connected(unsigned char* edges, int step)
{
  int notChainEnd;

  /* 当前点被访问过则返回 */
  if (*edges) {
    return 0;
  }

  *edges = 2; //((lap[i][j]) > param->low_thresh) ? 1 : 255;

  notChainEnd = 0;

  notChainEnd |= mark_connected(edges + 1, step);
  notChainEnd |= mark_connected(edges - 1, step);
  notChainEnd |= mark_connected(edges + step + 1, step);
  notChainEnd |= mark_connected(edges + step , step);
  notChainEnd |= mark_connected(edges + step - 1, step);
  notChainEnd |= mark_connected(edges - step - 1, step);
  notChainEnd |= mark_connected(edges - step , step);
  notChainEnd |= mark_connected(edges - step + 1, step);

  /* 边缘细化 */
  //int thinFactor = param->thinFactor;
  //if (notChainEnd && thinFactor && (level > 0) && (level%thinFactor)  ) {
  //  edges[i][j] = 255;
  //}

  return 1;
}

#define OUTLINE 1

/******************************************************************************
  功能：用shen-castan算子提取图像的边缘
  输入：im  - 指向存储有原图像的结构体指针
        res - 存储边缘提取后处理结果的结构体的指针
        b   - 平滑因子 0 < b < 1，控制边缘数量，越大边缘越少
******************************************************************************/
int shen_castan(int h, int w, const unsigned char* src, int al, unsigned char* edges, int bl, float b)
{
  int i, j;
  float* orig, *smoothed;
  unsigned char* bli;
  int nrows = h, ncols = w;
  int src_step = ncols;
  int dst_step = src_step;
  int beg = OUTLINE * ncols + OUTLINE;
  float ratio = 0.99f;
  const int window_size = 3, do_hysteresis = 0;
  double high_thresh, low_thresh; /* 边缘处理的高低门限 */

  int bufsize = nrows * ncols;
  /* 把输入图像转换为浮点型 */
  orig = MALLOC(float, 2 * bufsize);
  bli = MALLOC(unsigned char, bufsize);

  if (!orig || !bli) {
    return 0;
  }

  memset(bli, 0, sizeof(unsigned char) * bufsize);
  smoothed = orig + bufsize;

  for (i = 0; i < nrows; i++) {
    const unsigned char* _src = src + i * src_step;
    float* _orig = orig + i * ncols;

    for (j = 0; j < ncols; j++) {
      _orig[ j ] = (float)(_src[ j ]);
    }
  }

  /* 用ISEF滤波器对图像进行滤波 */
  //smoothed = f2d(nrows, ncols);
  compute_ISEF(orig, smoothed, nrows, ncols, ncols, b);

  /* 计算近似的拉普拉斯滤波图像 */
  /* smoothed>orig的像素值赋为1，其余的赋为0 */
  for (i = 0; i < bufsize; ++i) {
    bli[ i ] = (smoothed[ i ] > orig[ i ]);
  }

  /* 通过拉普拉斯滤波图像和梯度门限进行边缘检测，提取零交叉点 */
  /* 填充图像边缘 */
  memset(edges, 1, ncols);
  memset(edges + dst_step * (nrows - 1), 1, ncols);

  for (i = 1; i < nrows - 1; i++) {
    int pos = ncols * i;
    unsigned char* _bli = bli + pos;
    unsigned char* _edges = edges + dst_step * i;
    float* _orig = orig + pos;
    float* _smoothed = smoothed + pos;
    _edges[ 0 ] = _edges[ ncols - 1 ] = 1;

    for (j = 0; j < ncols; j++) {
      /* 如果像素是拉普拉斯滤波图像的零交叉点则进行下一步检验 */
      int is_candidate_edge;
#define FDFDF(_A, _B, _k, _N)  (0 == *(_A + _k) ? (*(_B+(_k)) > *(_B-(_k))) : (_N) )

      /* 搜索零交叉点 */
#define IS_CANDIDATE_EDGE(_A, _B, _step) (1 == *(_A) ?   \
    FDFDF(_A, _B, _step, FDFDF(_A, _B, 1, FDFDF(_A, _B, -_step, FDFDF(_A, _B, -1, 0)))) : 0)

      is_candidate_edge = IS_CANDIDATE_EDGE(_bli + j, _smoothed + j, ncols);
#undef FDFDF
#undef IS_CANDIDATE_EDGE

      if (is_candidate_edge) {
        /* 进行梯度门限判决 */
        _orig[ j ] = compute_adaptive_gradient(_bli + j, _smoothed + j, window_size / 2, ncols, ncols);
        _edges[ j ] = 0;
      }
      else {
        _orig[ j ] = 0.0;
        _edges[ j ] = 1;
      }
    }
  }

  high_thresh = estimate_thresh(orig, nrows * ncols, ratio);

  if (do_hysteresis) {
    low_thresh = (high_thresh) / 2;
  }
  else {
    low_thresh = high_thresh;
  }

  /* 通过梯度门限，选取边缘点，去除伪边缘点 */
  for (i = 0; i < nrows; i++) {
    unsigned char* _edges = edges + dst_step * i;
    float* _orig = orig + ncols * i;

    for (j = 0; j < ncols; j++) {
      if ((_orig[ j ]) > high_thresh) {
        mark_connected(_edges + j, ncols);
      }
    }
  }

  for (i = 0; i < nrows; i++) {
    unsigned char* _edges = edges + dst_step * i;

    for (j = 0; j < ncols; j++) {
      _edges[ j ] = (unsigned char) - (_edges[ j ] >> 1);
    }
  }

  pfree(bli);
  pfree(orig);
  return 1;
}
