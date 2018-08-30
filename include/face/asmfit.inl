
#ifndef _ASMFIT_INL_
#define _ASMFIT_INL_
//#include <stdlib.h>
//#include <assert.h>
#include "cstd.h"
// 以前是定位到双眼，再把图片对齐到双眼坐标上。
// 双眼定位经常抖动，导致面具图片也抖动。
// 现在是定位到脸上101个关键轮廓点（包括双眼，鼻子嘴巴，脸轮轮廓等等），贴面具就更加精确。
#define MAXSCANFBUFSIZE (1<<12)

#define HEADSIZE 76

typedef struct asmfit_param_t {
  int height, width, datastep;
  const unsigned char* data; /*???????????????????????*/
  IPOINT leye, reye; // ??????????????????锟斤拷?????
  const void* asm_mode; // asm???????
  // 
  // 
  FPOINT* shape_result; //? Shape_result_len/2????x??????Shape_result_len/2????y????
  int shape_result_len; // ??????????????
  int* contours_ending_points; //?????????????????????[ContoursEndingPointsNum]
  int contours_ending_points_len;
  // ????????????????????, Shape_result????????????????buf?锟斤拷?
  int buflen;// ??????????????锟斤拷 ??asmfit_memsize??????????
  char* buf; // ?????????????(??????????锟斤拷???>=buflen)
} asmfit_param_t;

typedef struct asm_t {
  //目前训练样本采用高度400宽度为320的图像
  int max_iter; // = 5;//搜索时的最大迭代次数(对于整体)
  int nPelsSearch; // = 3;//profile模型的中心沿着profile方向的正方向和负方向的移动量(对于整体)
  int nQualifyingDisp; // = 95;//用于判断是否收敛的参数
  float coef_lim; // = 1.8;//为形状参数的限制系数,正常应该为1.8
  int train_image_height, train_image_width; // 训练样本高度和宽度
  int NbOfPoints; //标点个数
  int contours_ending_points_len;
  int TrnPntsAbove, TrnPntsBelow; //profile方向向上和向下搜索点数
  int NumPyramidLevels; //图像金字塔分层数
  int if_filter; //图像金字塔分层时是否采用高斯滤波
  int shapes_eigen_values_len;
  int* contours_ending_points; //每个器官的最后一个标点索引[contours_ending_points_len]
  float* MeanShape; //平均形状,首行为x次行为y [2*NbOfPoints]
  float* shapes_eigen_values; //pca形状特征值[shapes_eigen_values_len]
  float* ShapesEigenVectors; //pca形状特征向量[(NbOfPoints * 2)*shapes_eigen_values_len]
  float* meanProfiles; //平均归一化profile向量[(NbOfPoints * NumPyramidLevels)*(TrnPntsAbove + TrnPntsBelow + 1)]
  float* covProfiles_inv; //profile协方差矩阵[( s->NbOfPoints * s->NumPyramidLevels ) * ( k * k )] k= (TrnPntsAbove + TrnPntsBelow + 1)
} asm_t;

static int GetBeforeAfterPts(int index, const int* _contours_ending_points,
    int contours_ending_points_len, int* pBefore, int* pAfter)
{
  //函数功能：获得当前标点索引index的前后标点索引
  //参数：    index为当前标点索引
  //          contours_ending_points 存储每个器官的最后一个标点索引，器官顺序为左眉、左眼、右眉、右眼、鼻子、外嘴、内嘴、脸颊+下巴,是从保存的训练数据中读取的
  //返回值:   pt1和pt2为当前标点索引index的前后标点索引

  int i, end = contours_ending_points_len, first, last;
  int pt1 = -1, pt2 = -1;
#if 1

  if (10 == index) {
    int aa = 0;
  }

  for (i = 0; i < end && index >= _contours_ending_points[ i ]; ++i) {}

  first = (0 == i) ? 0 : (_contours_ending_points[ i - 1 ]);
  last = _contours_ending_points[ i ] - 1;

  if (index == first) {
    pt1 = last;
    pt2 = index + 1;
  }
  else if (index == last) {
    pt1 = index - 1;
    pt2 = first;
  }
  else {
    pt1 = index - 1;
    pt2 = index + 1;
  }

#else

  if (index == 1) {   //如果当前点为第一个点
    pt1 = contours_ending_points(1);   //则将最后一个点的坐标索引赋予当前点的前一点（循环结构，...5 1 2 3 4 5 1 2 3...)，contours_ending_points为预标定点的个数
    pt2 = 2;
  }
  else if (index == contours_ending_points(1)) {       //如果当前点为最后一个点
    pt1 = contours_ending_points(1) - 1;
    pt2 = 1;
  }
  else if (index == contours_ending_points(end)) {
    pt1 = contours_ending_points(end) - 1;
    pt2 = contours_ending_points(end - 1) + 1;
  }
  else if (ismember(index - 1, contours_ending_points, contours_ending_points_len)) {       //ismember(a,b)检查a中元素是否是b的成员
    tmp = find(index - 1 == contours_ending_points);
    pt1 = contours_ending_points(tmp + 1);
    pt2 = index + 1;
  }
  else if (ismember(index, contours_ending_points)) {
    pt1 = index - 1;
    tmp = find(index == contours_ending_points);
    pt2 = contours_ending_points(tmp - 1) + 1;
  }
  else {
    pt1 = index - 1;
    pt2 = index + 1;
  }

#endif
  *pBefore = pt1;
  *pAfter = pt2;
  return 0;
}

// 1 5 8 5 1
/* 1/16[1    4    6    4    1]       */
/* ...| x0 | x1 | x2 | x3 | x4 |...  */
#define  PD_FILTER( x0, x1, x2, x3, x4 ) ((x2)*6+((x1)+(x3))*4+(x0)+(x4))
//#define SET_ROW_PD_FILTER(_B, _j, _b, x0, x1, x2, x3, x4) (_b = rowbuf + (_j), _B[_j] =PD_FILTER( _b[x0], _b[x1], _b[x2], _b[x3], _b[x4])>>8)
#define SET_ROW_PD_FILTER(_B, _j, _b, x0, x1, x2, x3, x4) (_b = rowbuf + (_j), _B[_j] =_b[x2]>>4)
#define SET_COL_PD_FILTER(_B, _j, _b, x0, x1, x2, x3, x4) (_b = A + (cn2*_j), _B[_j] =PD_FILTER( _b[x0], _b[x1], _b[x2], _b[x3], _b[x4]))

CC_INLINE int PyramidDown(int h, int w, unsigned char* B, int bl, const unsigned char* A, int al, int cn, int if_filter, void* buf)
{
  int i, j;
  int cn1 = cn;
  int cn2 = cn * 2;
  int al1 = al;
  int al2 = al * 2;
  const unsigned char* a = NULL;
  ushort* b = NULL;
  ushort* rowbuf = NULL;
  void* mybuf = NULL;
  ASSERT(bl > (w * cn / 2));

  if (!if_filter) {
    int alcn = al + cn;

    for (i = 0; i < h; ++i, A += al2, B += bl) {
      for (j = 0; j < w; ++j) {
        a = A + j * cn2;
#if 0

        B[ j ] = (a[ 0 ] + a[ cn ] + a[ al ] + a[ alcn ]) >> 2;
#else

        B[ j ] = (a[ 0 ]);
#endif

      }
    }

    return 0;
  }

  if (buf) {
    rowbuf = (ushort*)buf;
  }
  else {
    rowbuf = (ushort*)malloc(sizeof(ushort) * w);
    mybuf = rowbuf;
  }

  //0 line and h-1 line
  for (i = 0; i < h; ++i, A += al2, B += bl) {
    b = rowbuf;

    if (i > 1 && i < h - 2) {
      for (j = 0; j < w; ++j) {
        SET_COL_PD_FILTER(b, j, a, -al2, -al1, 0, al1, al2);
      }
    }
    else if (0 == i) {
      for (j = 0; j < w; ++j) {
        SET_COL_PD_FILTER(b, j, a, 0, 0, 0, al1, al2);
      }
    }
    else if (1 == i) {
      for (j = 0; j < w; ++j) {
        SET_COL_PD_FILTER(b, j, a, -al1, -al1, 0, al1, al2);
      }
    }
    else if ((h - 2) == i) {
      for (j = 0; j < w; ++j) {
        SET_COL_PD_FILTER(b, j, a, -al2, -al1, 0, al1, al1);
      }
    }
    else if ((h - 1) == i) {
      for (j = 0; j < w; ++j) {
        SET_COL_PD_FILTER(b, j, a, -al2, -al1, 0, 0, 0);
      }
    }

    SET_ROW_PD_FILTER(B, 0, b, 0, 0, 0, cn1, cn2);
    SET_ROW_PD_FILTER(B, 1, b, -cn1, -cn1, 0, cn1, cn2);
    SET_ROW_PD_FILTER(B, w - 2, b, -cn2, -cn1, 0, cn1, cn1);
    SET_ROW_PD_FILTER(B, w - 1, b, -cn2, -cn1, 0, 0, 0);

    for (j = 2; j < w - 2; ++j) {
      SET_ROW_PD_FILTER(B, j, b, -cn2, -cn1, 0, cn1, cn2);
    }
  }

  if (mybuf) {
    free(mybuf);
  }
  return 0;
}

// 计算用于规一化的仿射变换参数
CC_INLINE MATRIX3X2 get_std1(IPOINT s1, IPOINT e1, IPOINT s2, IPOINT e2)
{
  MATRIX3X2 m;
  double c = 0, s = 0;
  double dd = POINT_DIST(s1, e1) / (POINT_DIST(s2, e2));
  ANGLE_SIN_COS(e1.x - s1.x, e1.y - s1.y, e2.x - s2.x, e2.y - s2.y, s, c);
  c *= dd;
  s *= dd;
  MATRIX3X2_GET_STD_TOINT(m, c, s, s1, s2);
  return m;
}

CC_INLINE int CPM_FaceStd1(int ah, int aw, const unsigned char* A, int al, int ai,
    int bh, int bw, unsigned char* B, int bl, int bi,
    IPOINT leye, IPOINT reye,
    float lx, float ly, float rx, float ry)
{
  IPOINT s2, e2;
  MATRIX3X2 m;
  s2.x = (int)(bw * lx);
  s2.y = (int)(bh * ly);
  e2.x = (int)(bw * rx);
  e2.y = (int)(bh * ry);
  m = get_std1(leye, reye, s2, e2);
  IMAFFINE(ah, aw, A, al, ai, bh, bw, B, bl, bi, m, INTER_BILINEARITY);
  return 0;
}

CC_INLINE int affine_point(int n, const FPOINT* rpt, FPOINT* pt, IPOINT s1, IPOINT e1, IPOINT s2, IPOINT e2)
{
  int i;
  float x, y;
  MATRIX3X2 m;
  m = get_std1(s1, e1, s2, e2);

  for (i = 0; i < n; ++i) {
    x = rpt[ i ].x;
    y = rpt[ i ].y;
    MATRIX3X2_MUL_POINT(m, x, y, pt[ i ].x, pt[ i ].y);
    pt[ i ].x /= CC_SHIFT1;
    pt[ i ].y /= CC_SHIFT1;
  }

  return 0;
}
#undef DOT2
#define DOT2(_X0, _Y0, _X1, _Y1)  ((_X0)*(_X1)+(_Y0)*(_Y1))

CC_INLINE int facestd2(int ah, int aw, const unsigned char* A, int al, int ai,
    int bh, int bw, unsigned char* B, int bl, int bi,
    IPOINT leye, IPOINT reye, float rot0)
{
  float rot = (rot0) * (float) M_PI / 180.f;
  float lx = 0.25f, ly = 0.25f, rx = 0.75f, ry = 0.25f;
  float cx = 0.5f, cy = 0.5f, s;
  float sinr = (float) sin(rot), cosr = (float) cos(rot);
  lx -= cx;
  ly -= cy;
  rx -= cx;
  ry -= cy;
  lx = DOT2(lx, ly, cosr, sinr);
  ly = DOT2(lx, ly, -sinr, cosr);
  rx = DOT2(rx, ry, cosr, sinr);
  ry = DOT2(rx, ry, -sinr, cosr);
  lx += cx;
  ly += cy;
  rx += cx;
  ry += cy;
  s = 0.5f / (float) sqrt((lx - rx) * (lx - rx) + (ly - ry) * (ly - ry));
  lx *= s;
  ly *= s;
  rx *= s;
  ry *= s;
  //printf("%f : %13.9f, %13.9f, %13.9f, %13.9f\n", rot0, lx, ly, rx, ry);
  return CPM_FaceStd1(ah, aw, A, al, ai, bh, bw, B, bl, bi, leye, reye, lx, ly, rx, ry);
}

static int GetImagePyramid(int ah, int aw, const unsigned char* Img, int al, int NumPyramidLevels, int if_filter,
    int h, int w, unsigned char* ImagePyramid, IPOINT leye, IPOINT reye,
    float lx, float ly, float rx, float ry)
{
  //函数功能：获得金字塔分层图像
  //参数：   Img为待分层的灰度图像
  //         NumPyramidLevels为图像金字塔分层数
  //         if_filter 为图像金字塔分层时是否采用高斯滤波,1为是，其它数字为否
  //返回值:  ImagePyramid存储分层后的各层图像
  unsigned char* out;
  int i;
  out = ImagePyramid;

  if (h > 0 && w > 0 && leye.x > 0) {
    CPM_FaceStd1(ah, aw, Img, al, 1, h, w, out, w, 1,
        leye, reye, lx, ly, rx, ry);
  }
  else {
    SKIP_COPY(h, w, Img, al, 1, out, w, 1);
  }

  for (i = 1; i < NumPyramidLevels; ++i) {
    PyramidDown(h / 2, w / 2, out + h * w, w / 2, out, w, 1, if_filter, 0);
    out += h * w;
    h /= 2;
    w /= 2;
    //imwrite("./asdf.bmp", h, w, out, w, 1);
#if 0
    {
      char buf[ 256 ];
      _sntprintf(buf, 256, "Pyr%d.bmp", i);
      imwrite(buf, h, w, out, w, 1);
    }
#endif

  }

  return 0;
}

CC_INLINE int GetProfStepSize(const FPOINT* _sub_shape, int NbOfPoints, int ind_markpoint,
    const int* contours_ending_points, int contours_ending_points_len,
    float* pDeltaX, float* pDeltaY)
{
  //函数功能：计算DeltaX和DeltaY
  //参数:    sub_shape为当前金字塔层子图像对应的标点形状坐标
  //         ind_markpoint为当前的标点索引
  //         contours_ending_points存储每个器官的最后一个标点索引，器官顺序为左眉、左眼、右眉、右眼、鼻子、外嘴、内嘴、脸颊+下巴
  //返回值:  DeltaX和DeltaY为profile方向标点坐标的单位移动长度
  double temp_x, temp_y, d_x, d_y;
  double d0_x, d0_y, p_x, p_y;
  double d1_x, d1_y, q_x, q_y;
  double DeltaX, DeltaY;
  double AbsDeltaX, AbsDeltaY;
  int pt1, pt2;
  ASSERT(ind_markpoint < NbOfPoints);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //计算标点ind_markpoint的前一标点pt1和后一标点pt2 (索引)//
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  GetBeforeAfterPts(ind_markpoint, contours_ending_points, contours_ending_points_len, &pt1, &pt2);

  //////////////////////////////////////////////////
  // 求中线(DeltaX和DeltaY) //
  //////////////////////////////////////////////////
  temp_x = _sub_shape[(ind_markpoint)].x - _sub_shape[(pt1)].x;
  temp_y = _sub_shape[(ind_markpoint)].y - _sub_shape[(pt1)].y;
  d0_x = temp_x / (sqrt(temp_x * temp_x + temp_y * temp_y));
  d0_y = temp_y / (sqrt(temp_x * temp_x + temp_y * temp_y));
  p_x = d0_y; //旋转90度
  p_y = -d0_x;

  temp_x = _sub_shape[(pt2)].x - _sub_shape[(ind_markpoint)].x;
  temp_y = _sub_shape[(pt2)].y - _sub_shape[(ind_markpoint)].y;
  d1_x = temp_x / (sqrt(temp_x * temp_x + temp_y * temp_y));
  d1_y = temp_y / (sqrt(temp_x * temp_x + temp_y * temp_y));
  q_x = d1_y; ////旋转90度
  q_y = -d1_x;

  temp_x = p_x + q_x;
  temp_y = p_y + q_y;
  d_x = temp_x / (sqrt(temp_x * temp_x + temp_y * temp_y));
  d_y = temp_y / (sqrt(temp_x * temp_x + temp_y * temp_y));

  if (fabs(d_x) < 1e-10 && fabs(d_y) < 1e-10) {       //后面归一化DeltaX和DeltaY是为了防止分母为0
    d_x = d0_x;
    d_y = d0_y;
  }

  //以下计算并归一化DeltaX和DeltaY
  DeltaX = d_x;
  DeltaY = d_y;

  AbsDeltaX = fabs(DeltaX);
  AbsDeltaY = fabs(DeltaY);

  if (AbsDeltaX >= AbsDeltaY) {
    DeltaY = DeltaY / AbsDeltaX;
    DeltaX = DeltaX / AbsDeltaX;
  }
  else {
    DeltaX = DeltaX / AbsDeltaY;
    DeltaY = DeltaY / AbsDeltaY;
  }

  *pDeltaX = (float) DeltaX;
  *pDeltaY = (float) DeltaY;
  return 0;
}


typedef int int32;
CC_INLINE int CalcProfileGrad1D_for_fit(int nSamplePoints, const FPOINT* _sub_shape, int NbOfPoints, int ind_markpoint,
    const int* contours_ending_points, int contours_ending_points_len,
    int height, int width, const unsigned char* _image_curLevel, int nProfWidth,
    float* gradient_vector, float* pDeltaX, float* pDeltaY)
{
  //函数功能:计算1D profile梯度向量并返回计算得到的DeltaX和DeltaY
  //参数:    nSamplePoints为profile方向向上或向下采样点个数,
  //         sub_shape为当前金字塔层子图像对应的标点形状坐标
  //         ind_markpoint为当前的标点索引
  //         contours_ending_points存储每个器官的最后一个标点索引，器官顺序为左眉、左眼、右眉、右眼、鼻子、外嘴、内嘴、脸颊+下巴
  //         image_curLevel 当前样本图像的当前金字塔层子图像
  //         nProfWidth为profile宽度即2*nSamplePoints +1
  //返回值:  gradient_vector 为未归一化的profile梯度向量
  //         DeltaX和DeltaY为profile方向标点坐标的单位移动长度
#ifndef ROUND
//#undef ROUND
#define ROUND(_X) (int)(_X+0.5f)
#endif

  float x, y;
  float DeltaX, DeltaY;
  int temp_x, temp_y;
  int PrevPels, tempPels;
  int i;
  //[height,width] = size(image_curLevel);

  ////////////////////////////////////////
  //计算DeltaX和DeltaY //
  ////////////////////////////////////////
  GetProfStepSize(_sub_shape, NbOfPoints, ind_markpoint,
      contours_ending_points, contours_ending_points_len, &DeltaX, &DeltaY);

  //////////////////////////////////////////////////////
  //计算profile梯度向量 //
  //////////////////////////////////////////////////////
  x = _sub_shape[(ind_markpoint)].x;
  y = _sub_shape[(ind_markpoint)].y;

  temp_x = ROUND(x + (-nSamplePoints - 1) * DeltaX);
  temp_y = ROUND(y + (-nSamplePoints - 1) * DeltaY);

  if (temp_y < 1) {
    temp_y = 1;
  }

  if (temp_x < 1) {
    temp_x = 1;
  }

  if (temp_y > height) {
    temp_y = height;
  }

  if (temp_x > width) {
    temp_x = width ;
  }

  PrevPels = (int32) _image_curLevel[(temp_y - 1) * width + (temp_x - 1)];

  for (i = -nSamplePoints; i <= nSamplePoints; ++i) {
    temp_x = ROUND(x + i * DeltaX);
    temp_y = ROUND(y + i * DeltaY);

    if (temp_y < 1) {
      temp_y = 1;
    }

    if (temp_x < 1) {
      temp_x = 1;
    }

    if (temp_y > height) {
      temp_y = height;
    }

    if (temp_x > width) {
      temp_x = width ;
    }

    tempPels = (int32) _image_curLevel[(temp_y - 1) * width + (temp_x - 1)];

    gradient_vector[ i + nSamplePoints ] = (float)(tempPels - PrevPels);
    PrevPels = tempPels;
  }


  *pDeltaX = DeltaX;
  *pDeltaY = DeltaY;
  return 0;
}

CC_INLINE int AlignTransformation1(const FPOINT* _ref, const FPOINT* _SubShape, int NbOfPoints, float* a, float* b, float* tx, float* ty)
//函数功能:求对齐参数(SubShape向ref对齐)
//参数:形状SubShape和ref
//     NbOfPoints为标点个数
//返回值:a,b,tx,ty
{
  int i;
  double Params[ 4 ];
  double X1, Y1, X2, Y2, Z, C1, C2, W;
  double x1, y1, x2, y2;
  X1 = 0;
  Y1 = 0;
  X2 = 0;
  Y2 = 0;
  Z = 0;
  C1 = 0;
  C2 = 0;
  W = (double) NbOfPoints;

  for (i = 0; i < NbOfPoints; ++i) {
    x1 = _ref[(i)].x;
    y1 = _ref[(i)].y;
    x2 = _SubShape[(i)].x;
    y2 = _SubShape[(i)].y;

    Z = Z + (x2 * x2 + y2 * y2);
    X1 = X1 + x1;
    Y1 = Y1 + y1;
    X2 = X2 + x2;
    Y2 = Y2 + y2;
    C1 = C1 + (x1 * x2 + y1 * y2);
    C2 = C2 + (y1 * x2 - x1 * y2);
  }

#if 0
  //SolnA*Params=SolnB
  SolnA = [ X2 - Y2 W 0;
      Y2 X2 0 W;
      Z 0 X2 Y2;
      0 Z - Y2 X2 ];
  SolnB = [ X1; Y1; C1; C2 ];

  Params = SolnA\SolnB;
#else

  {
#define SOLVE_4X4(a0,a1,a2,a3,b0,b1,b2,b3,c0,c1,c2,c3,d0,d1,d2,d3) \
  -(+(b2)*(c0)*(a3)*(d1)-(a0)*(b1)*(c2)*(d3)+(a2)*(c1)*(b3)*(d0)+(b1)*(a2)*(c0)*(d3) \
      +(a0)*(b1)*(c3)*(d2)+(c2)*(b1)*(a3)*(d0)+(c2)*(a0)*(b3)*(d1)-(a0)*(b3)*(c1)*(d2) \
      -(b3)*(c0)*(a2)*(d1)-(a3)*(c1)*(b2)*(d0)-(b1)*(a3)*(c0)*(d2)+(a0)*(b2)*(c1)*(d3) \
      -(c3)*(b1)*(a2)*(d0)-(c3)*(a0)*(b2)*(d1)-(b0)*(c2)*(a3)*(d1)+(b0)*(a3)*(c1)*(d2) \
      -(b0)*(a2)*(c1)*(d3)+(b0)*(c3)*(a2)*(d1)+(a1)*(b0)*(c2)*(d3)-(a1)*(b0)*(c3)*(d2) \
      +(a1)*(b3)*(c0)*(d2)-(a1)*(b2)*(c0)*(d3)+(a1)*(c3)*(b2)*(d0)-(a1)*(c2)*(b3)*(d0))

    double dd;
    dd = SOLVE_4X4(X2, Y2, Z, 0, -Y2, X2, 0, Z, W, 0, X2, -Y2, 0, W, Y2, X2);

    if (0. == dd) {
      return 0;
    }

    // d[0], d[1], d[2]
    Params[ 0 ] = SOLVE_4X4(X1, Y1, C1, C2, -Y2, X2, 0, Z, W, 0, X2, -Y2, 0, W, Y2, X2) / dd;
    Params[ 1 ] = SOLVE_4X4(X2, Y2, Z, 0, X1, Y1, C1, C2, W, 0, X2, -Y2, 0, W, Y2, X2) / dd;
    Params[ 2 ] = SOLVE_4X4(X2, Y2, Z, 0, -Y2, X2, 0, Z, X1, Y1, C1, C2, 0, W, Y2, X2) / dd;
    Params[ 3 ] = SOLVE_4X4(X2, Y2, Z, 0, -Y2, X2, 0, Z, W, 0, X2, -Y2, X1, Y1, C1, C2) / dd;
  }
#endif

  *a = (float) Params[ 0 ];
  *b = (float) Params[ 1 ];
  *tx = (float) Params[ 2 ];
  *ty = (float) Params[ 3 ];
  return 1;
}


//马氏距离
// A[1×h]
// B[h×w]
// C[w×1]
// A×B×C
CC_INLINE float MaDist(int h, int w, const float* A, const float* B, int bl, const float* C)
{
  int i, j;
  float d = 0.f;

  for (i = 0; i < h; ++i, B += bl) {
    float bc = 0.f;

    for (j = 0; j < w; ++j) {
      bc += B[ j ] * C[ j ];
    }

    d += A[ i ] * bc;
  }

  return d;
}

CC_INLINE int GetSuggestedShape(const FPOINT* _sub_shape, int height, int width, const unsigned char* image_curLevel,
    const int* contours_ending_points, int contours_ending_points_len,
    int nSamplePoints, int NbOfPoints, int nPelsSearch,
    int nProfWidth_model, const float* meanProfiles,
    const float* covProfiles_inv, int ind_Level,
    FPOINT* _SuggestedShape, int* pnGoodLandmarks
                               )
{
  //函数功能:获得SuggestedShape形状以及收敛点的个数
  //参数:    sub_shape为当前金字塔层子图像对应的标点形状坐标
  //         image_curLevel为当前金字塔层子图像
  //         contours_ending_points存储每个器官的最后一个标点索引，器官顺序为左眉、左眼、右眉、右眼、鼻子、外嘴、内嘴、脸颊+下巴,是从保存的训练数据中读取的
  //         nSamplePoints为搜索时profile方向向上或向下采样点个数
  //         NbOfPoints标点个数,是从保存的训练数据中读取的
  //         nProfWidth_model为从训练中得到的平均profile的宽度
  //         meanProfiles存储每一金字塔层的每一标点对应于所有训练样本图像的平均profile梯度,是从保存的训练数据中读取的
  //         covProfiles_inv存储每一金字塔层的每一标点对应于所有训练样本图像的profile梯度的协方差矩阵的逆矩阵,是从保存的训练数据中读取的
  //         ind_Level金字塔层数索引
  //         nPelsSearch为profile模型的中心沿着profile方向的正方向和负方向的移动量
  //返回值:  SuggestedShape为所有标点沿着profile方向移动后的形状(坐标)
  //         nGoodLandmarks为收敛的坐标点数
  char buf[1 << 9];
  int buflen = 1 << 9;
  int nGoodLandmarks = 0;
  const float* ModelProfVector;
  const float* ModelCov_inv;
  int ixBest;
  float BestFit;
  float DeltaX, DeltaY;
  int i, ind_markpoint, ix;
  float* gradient_vector;
  float* sub_gradient_vector;
  int gradient_vector_len = nPelsSearch * 2 + nProfWidth_model;
  BUFUSEBEGIN(buf, buflen);
  ASSERT((gradient_vector_len + nProfWidth_model) < (buflen));
  gradient_vector = BUFMALLOC(float, gradient_vector_len);
  sub_gradient_vector = BUFMALLOC(float, nProfWidth_model);

  // SuggestedShape = zeros(2,NbOfPoints);
  //FILL( NbOfPoints, _SuggestedShape, 0.f );
  for (i = 0; i < NbOfPoints; ++i) {
    _SuggestedShape[i].x = 0.f;
    _SuggestedShape[i].y = 0.f;
  }

  for (ind_markpoint = 0; ind_markpoint < NbOfPoints; ++ind_markpoint) {
    ModelProfVector = meanProfiles + (ind_markpoint * nProfWidth_model);   //转为列向量
    ModelCov_inv = covProfiles_inv + (ind_markpoint * nProfWidth_model * nProfWidth_model);   //训练得到的profile协方差,尺度为nProfWidth_model*nProfWidth_model

    ixBest = 0;
    BestFit = (float)(1.7976931348623158e+30);      //初始设为无穷大即可
    CalcProfileGrad1D_for_fit(nSamplePoints, _sub_shape, NbOfPoints, ind_markpoint,
        contours_ending_points, contours_ending_points_len,
        height, width, image_curLevel, 2 * nSamplePoints + 1,
        gradient_vector, &DeltaX, &DeltaY);

    if (ind_markpoint == 100) {
      int aaa = 0;
    }

    for (ix = -nPelsSearch; ix <= nPelsSearch; ++ix) {
      float MahDistance_Fit;
      float sum_abs = 0.f;
      int ind_sub_grad = ix + nPelsSearch;
      memcpy(sub_gradient_vector, gradient_vector + ind_sub_grad, sizeof(float) * nProfWidth_model);

      //(ind_sub_grad:nProfWidth_model+ind_sub_grad-1);
      //if length(find(sub_gradient_vector)~=0) ~= 0
      // sub_gradient_vector = sub_gradient_vector/(sum(abs(sub_gradient_vector)));
      //梯度向量的归一化
      for (i = 0; i < nProfWidth_model; ++i) {
        sum_abs += (float) fabs(sub_gradient_vector[ i ]);
      }

      // sub_gradient_vector-ModelProfVector
      for (i = 0; i < nProfWidth_model; ++i) {
        sub_gradient_vector[ i ] = sub_gradient_vector[ i ] / sum_abs - ModelProfVector[ i ];
      }

      // MahDistance_Fit = (sub_gradient_vector)' * ModelCov_inv * (sub_gradient_vector);//马氏距离
      MahDistance_Fit = MaDist(nProfWidth_model, nProfWidth_model, sub_gradient_vector,
          ModelCov_inv, nProfWidth_model, sub_gradient_vector);

      if (MahDistance_Fit < BestFit) {
        ixBest = ix;
        BestFit = MahDistance_Fit;
      }
    }

    _SuggestedShape[(ind_markpoint)].x = _sub_shape[(ind_markpoint)].x + ixBest * DeltaX;
    _SuggestedShape[(ind_markpoint)].y = _sub_shape[(ind_markpoint)].y + ixBest * DeltaY;

    if (abs(ixBest) <= nPelsSearch / 2) {
      nGoodLandmarks = nGoodLandmarks + 1;
    }

    *pnGoodLandmarks = nGoodLandmarks;

  }

  //FREE( gradient_vector );
  //FREE( sub_gradient_vector );
  BUFUSEEND();
  return 0;
}

//函数功能：根据对齐参数a,b,tx,ty将SubShape向某个形状对齐
//返回值:  NewSubShape为新的形状
CC_INLINE int TransformPose(float a, float b, float tx, float ty, const FPOINT* _SubShape, int NbOfPoints, FPOINT* _NewSubShape)
{
  //函数功能：根据对齐参数a,b,tx,ty将SubShape向某个形状对齐
  //返回值:  NewSubShape为新的形状
  int i;
  double c00, c01, c10, c11;

  c00 = a;
  c01 = -b;
  c10 = b;
  c11 = a;

  for (i = 0; i < NbOfPoints; ++i) {
    double x, y;
    x = _SubShape[(i)].x;
    y = _SubShape[(i)].y;
    _NewSubShape[(i)].x = c00 * x + c01 * y + tx;
    _NewSubShape[(i)].y = c10 * x + c11 * y + ty;
  }

  return 0;
}

// A[h×w]
// B[w×1]
// C[h×1]
// D[h×1]
// D = A * B + C
CC_INLINE int mat_mul_vec(int h, int w, const float* A, int al, const float* B, const float* C, float* D)
{
  int i, j;

  for (i = 0; i < h; ++i, A += al) {
    double d = 0.f;

    for (j = 0; j < w; ++j) {
      d += A[ j ] * B[ j ];
    }

    D[ i ] = (float)(C[ i ] + d);
  }

  return 0;
}

CC_INLINE int findResultShape_1DP(const asm_t* s, unsigned char** ImagePyramid, FPOINT* shape_result, char* buf, int buflen)
{
  float* MeanShape = s->MeanShape;
  float* meanProfiles = s->meanProfiles;
  float* covProfiles_inv = s->covProfiles_inv;
  float* ShapesEigenVectors = s->ShapesEigenVectors;
  float* shapes_eigen_values = s->shapes_eigen_values;
  float* StartShape = s->MeanShape;
  int* contours_ending_points = s->contours_ending_points;
  float coef_lim_temp = s->coef_lim;
  int NbOfPoints = s->NbOfPoints;
  int max_iter = s->max_iter;
  int contours_ending_points_len = s->contours_ending_points_len;
  int TrnPntsAbove = s->TrnPntsAbove;
  int TrnPntsBelow = s->TrnPntsBelow;
  int shapes_eigen_values_len = s->shapes_eigen_values_len;
  int nPelsSearch = s->nPelsSearch;
  int nQualifyingDisp = s->nQualifyingDisp;
  int train_image_height = s->train_image_height;
  int train_image_width = s->train_image_width;
  //函数功能：用1D profile进行搜索
  //参数： I为待搜索(样本)图像
  // if_filter图像金字塔分层时是否采用高斯滤波，是从保存的训练数据中读取的
  // if_Prof_Bilinear为计算profile方向上的坐标像素时是否采用双线性插值，是从保存的训练数据中读取的
  // NumPyramidLevels为金字塔分层数,是从保存的训练数据中读取的
  // StartShape为初始形状
  // MeanShape为平均形状
  // NbOfPoints标点个数,是从保存的训练数据中读取的
  // max_iter搜索时的最大迭代次数
  // contours_ending_points存储每个器官的最后一个标点索引，器官顺序为左眉、左眼、右眉、右眼、鼻子、外嘴、内嘴、脸颊+下巴,是从保存的训练数据中读取的
  // TrnPntsAbove为训练时1D profile正方向点数
  // TrnPntsBelow为训练时1D profile负方向点数
  // nPelsSearch为profile模型的中心沿着profile方向的正方向和负方向的移动量
  // meanProfiles存储每一金字塔层的每一标点对应于所有训练样本图像的平均profile梯度,是从保存的训练数据中读取的
  // covProfiles_inv 存储每一金字塔层的每一标点对应于所有训练样本图像的profile梯度的协方差矩阵的逆矩阵,是从保存的训练数据中读取的
  // ShapesEigenVectors为被截断的形状特征向量,是从保存的训练数据中读取的
  // shapes_eigen_values为被截断的形状特征值,是从保存的训练数据中读取的
  // nQualifyingDisp为用于判断是否收敛的参数
  // coef_lim_temp为形状参数的限制系数
  // train_image_height和train_image_width为训练图像宽度和高度
  //返回值: shape_result为最后的形状结果(坐标)

  int i, j, ind_Level;
  int nProfWidth_model = TrnPntsAbove + TrnPntsBelow + 1; //profile(法线)方向宽度
  int nSamplePoints, converge_num;
  int h = train_image_height;
  int w = train_image_width;

  BUFUSEBEGIN(buf, buflen);
  float* sub_MeanShape_ascol = BUFMALLOC(float, 2 * NbOfPoints);
  float* xxx_ascol = BUFMALLOC(float, 2 * NbOfPoints);
  float* sp = BUFMALLOC(float, shapes_eigen_values_len);
  FPOINT* _xxx = BUFMALLOC(FPOINT, NbOfPoints);
  FPOINT* yyy = BUFMALLOC(FPOINT, NbOfPoints);
  FPOINT* tempShape = BUFMALLOC(FPOINT, NbOfPoints);
  FPOINT* SuggestedShape = BUFMALLOC(FPOINT, NbOfPoints);

  nSamplePoints = (nProfWidth_model - 1) / 2 + nPelsSearch;   //搜索时profile(法线)单方向采样点数
  converge_num = (int) floor(nQualifyingDisp * NbOfPoints / 100);     //收敛条件
  // if ndims(I)==3, I=rgb2gray(I); end

  //MeanShape_ascol = MeanShape;//写成向量的形式,x0,y0,x1,y1,...,xn,yn
  //sub_shape      = StartShape/(2^(s->NumPyramidLevels-1));//获得当前金字塔层子图像对应的标点形状坐标
  {
    int k = 1 << (s->NumPyramidLevels - 1);

    for (i = 0; i < NbOfPoints; ++i) {
      shape_result[i].x = StartShape[ 0 + 2 * i ] / k;
      shape_result[i].y = StartShape[ 1 + 2 * i ] / k;
    }
  }

  //xxx = zeros(2,NbOfPoints);//临时形状初始化
  for (ind_Level = s->NumPyramidLevels - 1; ind_Level >= 0; --ind_Level) {   //对于当前搜索样本的每一金子塔层
    int iter;
    int nGoodLandmarks;
    float coef_lim;
    int k = 1 << (ind_Level);
    float* _meanProfiles = meanProfiles + (s->NumPyramidLevels - ind_Level - 1) * NbOfPoints * nProfWidth_model;
    float* _covProfiles_inv = covProfiles_inv + (s->NumPyramidLevels - ind_Level - 1) * NbOfPoints * nProfWidth_model * nProfWidth_model;
    //sub_MeanShape_ascol = MeanShape_ascol/(2^(ind_Level-1));//当前层平均形状
    //sub_StartShape = StartShape/(2^(ind_Level-1));//当前层初始形状
    unsigned char* image_curLevel;
    image_curLevel = ImagePyramid[ ind_Level ]; //当前层图像

    for (i = 0; i < NbOfPoints * 2; ++i) {
      sub_MeanShape_ascol[ i ] = MeanShape[ i ] / k;
      //sub_StartShape[i] = StartShape[i]/k;
    }

    //sp = zeros(length(shapes_eigen_values),1);//初始化形状参数
    FILL(shapes_eigen_values_len, sp, 0.f);
    iter = 1;
    nGoodLandmarks = 0;

    while (iter <= max_iter && nGoodLandmarks <= converge_num) {
      //int ind_temp;
      float a=0, b=0, tx=0, ty=0;
      GetSuggestedShape(shape_result, h / k, w / k, image_curLevel,
          contours_ending_points, contours_ending_points_len,
          nSamplePoints, NbOfPoints, nPelsSearch, nProfWidth_model,
          _meanProfiles, _covProfiles_inv,
          ind_Level, SuggestedShape, &nGoodLandmarks);

      if (iter == max_iter) {
        coef_lim = 3.f;
      }
      else {
        coef_lim = coef_lim_temp;
      }

      // xxx_ascol = sub_MeanShape_ascol + ShapesEigenVectors*sp;
#if 0
      mat_mul_vec(NbOfPoints * 2, shapes_eigen_values_len, ShapesEigenVectors, shapes_eigen_values_len,
          sp, sub_MeanShape_ascol, xxx_ascol);
#else
      {
        int i, j;

        for (i = 0; i < NbOfPoints * 2; ++i) {
          double d = 0.f;

          for (j = 0; j < shapes_eigen_values_len; ++j) {
            d += ShapesEigenVectors[ i * shapes_eigen_values_len + j ] * sp[ j ];
          }

          xxx_ascol[ i ] = (float)(sub_MeanShape_ascol[ i ] + d);
        }
      }

#endif

      for (i = 0; i < NbOfPoints; ++i) {
        _xxx[(i)].x = xxx_ascol[ 0 + 2 * i ];
        _xxx[(i)].y = xxx_ascol[ 1 + 2 * i ];
      }

      AlignTransformation1(_xxx, SuggestedShape, NbOfPoints, &a, &b, &tx, &ty);   //SuggestedShape向xxx对齐的参数
      TransformPose(a, b, tx, ty, SuggestedShape, NbOfPoints, yyy);   //SuggestedShape使用4个参数向xxx对齐,对齐后的形状为yyy

      for (i = 0; i < NbOfPoints; ++i) {
        xxx_ascol[ 0 + 2 * i ] = yyy[ i ].x - sub_MeanShape_ascol[ 0 + 2 * i ];
        xxx_ascol[ 1 + 2 * i ] = yyy[ i ].y - sub_MeanShape_ascol[ 1 + 2 * i ];
      }

      //sp = ShapesEigenVectors'*(yyy(:) - sub_MeanShape_ascol);//实际上sp = inv(ShapesEigenVectors'*ShapesEigenVectors)*ShapesEigenVectors'*(yyy(:) - sub_MeanShape_ascol)只是inv(ShapesEigenVectors'*ShapesEigenVectors)为E矩阵(主对角元素为1，其余元素为0)
      for (j = 0; j < shapes_eigen_values_len; ++j) {
        float d = 0.f;

        for (i = 0; i < NbOfPoints * 2; ++i) {
          d += ShapesEigenVectors[ i * shapes_eigen_values_len + j ] * xxx_ascol[ i ];
        }

        sp[ j ] = d;
      }

      // LIM=coef_lim*sqrt(fabs(shapes_eigen_values));//限制形状参数
      for (i = 0; i < shapes_eigen_values_len; ++i) {
        float LIM = coef_lim * (float) sqrt(fabs(shapes_eigen_values[ i ]));

        if (sp[ i ] > LIM) {
          sp[ i ] = LIM;
        }

        if (sp[ i ] < -LIM) {
          sp[ i ] = -LIM;
        }
      }

      // xxx_ascol = sub_MeanShape_ascol + ShapesEigenVectors*sp;
      for (i = 0; i < NbOfPoints * 2; ++i) {
        float d = 0.f;

        for (j = 0; j < shapes_eigen_values_len; ++j) {
          d += ShapesEigenVectors[ i * shapes_eigen_values_len + j ] * sp[ j ];
        }

        xxx_ascol[ i ] = sub_MeanShape_ascol[ i ] + d;
      }

      //ind_temp = 0;

      for (i = 0; i < NbOfPoints; ++i) {
        tempShape[ i ].x = xxx_ascol[ 0 + 2 * i ];
        tempShape[ i ].y = xxx_ascol[ 1 + 2 * i ];
      }

      AlignTransformation1(SuggestedShape, _xxx, NbOfPoints, &a, &b, &tx, &ty);   //xxx向SuggestedShape对齐的参数
      TransformPose(a, b, tx, ty, tempShape, NbOfPoints, shape_result);

      iter = iter + 1;
    }

    if (ind_Level == 0) {
      break;
    }

    for (i = 0; i < NbOfPoints; ++i) {
      shape_result[ i ].x *=  2;
      shape_result[ i ].y *=  2;
    }
  }

  //BUFFREE2( yyy, 2 * NbOfPoints );
  //BUFFREE2( _xxx, 2 * NbOfPoints );
  //BUFFREE2( sub_MeanShape_ascol, 2 * NbOfPoints );
  //BUFFREE(sub_StartShape);
  //BUFFREE2( SuggestedShape, 2 * NbOfPoints );
  //BUFFREE2( xxx_ascol, 2 * NbOfPoints );
  //BUFFREE2( tempShape, 2 * NbOfPoints );
  //BUFFREE2( xxx_ascol, 2 * NbOfPoints );
  //BUFFREE2( sp, shapes_eigen_values_len );

  BUFUSEEND();

  return 0;
}

static int asm_fix_inl(asm_t* s, const void* buf)
{
  int k, len = 1<<20;
  char* cbuf = (char*)buf;
  *s = *(asm_t*)buf;
  cbuf += HEADSIZE;
  BUFUSEBEGIN(cbuf, len);
  k = (s->TrnPntsAbove + s->TrnPntsBelow + 1);
  BUFMALLOC2(s->contours_ending_points, s->contours_ending_points_len);
  BUFMALLOC2(s->MeanShape, s->NbOfPoints*2);
  BUFMALLOC2(s->shapes_eigen_values, s->shapes_eigen_values_len);
  BUFMALLOC2(s->ShapesEigenVectors, s->NbOfPoints * 2 * s->shapes_eigen_values_len);
  BUFMALLOC2(s->meanProfiles, (s->NbOfPoints * s->NumPyramidLevels) * k);
  BUFMALLOC2(s->covProfiles_inv, (s->NbOfPoints * s->NumPyramidLevels) * (k * k));
  BUFUSEEND();
#if 0
  {
    int i;
    printf("test GetBeforeAfterPts\n");

    for (i = 0; i < s->NbOfPoints; ++i) {
      int Before, After;
      GetBeforeAfterPts(i, s->contours_ending_points, s->contours_ending_points_len, &Before, &After);
      printf("%3d %3d %3d\n", Before, i, After);
    }

    return 0;
  }
#endif
  return 0;
}

static int* scanf_imat(FILE* pf, int* prow, int* pcol)
{
  int i, n, _row = 0, _col = 0;
  int* imat;
  char buf[ MAXSCANFBUFSIZE ];

  while (!strchr(fgets(buf, MAXSCANFBUFSIZE, pf), '"'))
    ;

  fscanf(pf, "%d %d", &_row, &_col);
  n = _row * _col;
  imat = MALLOC(int, n);

  for (i = 0; i < n; ++i) {
    fscanf(pf, "%d", (imat + i));
  }

  if (prow) {
    *prow = _row;
  }

  if (pcol) {
    *pcol = _col;
  }

  return imat;
}

static float* scanf_fmat(FILE* pf, int* prow, int* pcol)
{
  int i, n, _row = 0, _col = 0;
  float* fmat;
  char buf[ MAXSCANFBUFSIZE ];

  while (!strchr(fgets(buf, MAXSCANFBUFSIZE, pf), '"'))
    ;

  fscanf(pf, "%d %d", &_row, &_col);
  n = _row * _col;
  fmat = MALLOC(float, n);

  for (i = 0; i < n; ++i) {
    float f = 0.;
    fscanf(pf, "%f", &f);
    fmat[ i ] = f;
  }

  if (prow) {
    *prow = _row;
  }

  if (pcol) {
    *pcol = _col;
  }

  return fmat;
}

CC_INLINE void asm_free(asm_t** pasm)
{
  if (pasm && *pasm) {
    asm_t* s = *pasm;
    FREE(s->contours_ending_points);
    FREE(s->MeanShape);
    FREE(s->shapes_eigen_values);
    FREE(s->ShapesEigenVectors);
    FREE(s->meanProfiles);
    FREE(s->covProfiles_inv);
    FREE(*pasm);
  }
}

CC_INLINE void asm_set_defalt(asm_t* s)
{
  ASSERT(NULL != s);
  s->max_iter = 5; //搜索时的最大迭代次数(对于整体)
  s->nPelsSearch = 3; //profile模型的中心沿着profile方向的正方向和负方向的移动量(对于整体)
  s->nQualifyingDisp = 95; //用于判断是否收敛的参数
  s->coef_lim = 1.8f; //为形状参数的限制系数,正常应该为1.8
}

CC_INLINE int save_inl(FILE* pf, void* p, int len)
{
  int* pi = (int*)p;
  int i, leni = len / 4;
  ASSERT(0 == len % 4);

  for (i = 0; i < leni; ++i) {
    fprintf(pf, "0x%08x,\r\n", pi[i]);
  }

  return 0;
}

#define SAVE_INL(_F, _P, _N) save_inl(_F, (_P), (_N) * sizeof(*(_P)))

static int asm_save_inl(asm_t* s, const char* txtfilename)
{
  FILE* pf = NULL;
  pf = fopen(txtfilename, "wb");

  if (pf) {
    int k = (s->TrnPntsAbove + s->TrnPntsBelow + 1);
    SAVE_INL(pf, s, 1);
    SAVE_INL(pf, s->contours_ending_points, s->contours_ending_points_len);
    SAVE_INL(pf, s->MeanShape, s->NbOfPoints*2);
    SAVE_INL(pf, s->shapes_eigen_values, s->shapes_eigen_values_len);
    SAVE_INL(pf, s->ShapesEigenVectors, s->NbOfPoints * 2 * s->shapes_eigen_values_len);
    SAVE_INL(pf, s->meanProfiles, (s->NbOfPoints * s->NumPyramidLevels) * k);
    SAVE_INL(pf, s->covProfiles_inv, (s->NbOfPoints * s->NumPyramidLevels) * (k * k));
    fclose(pf);
  }

  return 0;
}

CC_INLINE asm_t* asm_load_txt(const char* txtfilename)
{
  FILE* pf = NULL;
  asm_t* s = NULL;
  pf = fopen(txtfilename, "rb");

  if (pf) {
    int* imat = NULL;
    float* fmat = NULL;
    int row, col;
    s = MALLOC(asm_t, 1);
    ASSERT(NULL != s && "s = MALLOC(asm_t, 1)");
    asm_set_defalt(s);
    {
      //"训练样本高度和宽度(train_image_height和train_image_width)"
      imat = scanf_imat(pf, &row, &col);
      ASSERT(1 == row);
      ASSERT(2 == col);
      s->train_image_height = imat[ 0 ];
      s->train_image_width = imat[ 1 ];
      FREE(imat);
    }
    {
      //"标点个数(NbOfPoints)"
      imat = scanf_imat(pf, &row, &col);
      ASSERT(1 == row);
      ASSERT(1 == col);
      s->NbOfPoints = imat[ 0 ];
      FREE(imat);
    }
    {
      //"每个器官的最后一个标点索引(contours_ending_points)"
      s->contours_ending_points = scanf_imat(pf, &row, &col);
      ASSERT(1 == row);
      s->contours_ending_points_len = col;
    }
    {
      //"profile方向向上和向下搜索点数(TrnPntsAbove和TrnPntsBelow)"
      imat = scanf_imat(pf, &row, &col);
      ASSERT(1 == row);
      ASSERT(2 == col);
      s->TrnPntsAbove = imat[ 0 ];
      s->TrnPntsBelow = imat[ 1 ];
      FREE(imat);
    }
    {
      //"图像金字塔分层数(NumPyramidLevels)"
      imat = scanf_imat(pf, &row, &col);
      ASSERT(1 == row);
      ASSERT(1 == col);
      s->NumPyramidLevels = imat[ 0 ];
      FREE(imat);
    }
    {
      //"图像金字塔分层时是否采用高斯滤波(if_filter)"
      imat = scanf_imat(pf, &row, &col);
      ASSERT(1 == row);
      ASSERT(1 == col);
      s->if_filter = imat[ 0 ];
      FREE(imat);
    }
    {
      //"平均形状,首行为x次行为y(MeanShape)"
      int i;
      float* MeanShape = scanf_fmat(pf, &row, &col);
      s->MeanShape = MALLOC(float, 2 * s->NbOfPoints);
      ASSERT(2 == row);
      ASSERT(s->NbOfPoints == col);

      for (i = 0; i < s->NbOfPoints; ++i) {
        s->MeanShape[ 0 + 2 * i ] = MeanShape[ i ];
        s->MeanShape[ 1 + 2 * i ] = MeanShape[ s->NbOfPoints + i ];
      }

      FREE(MeanShape);
    }
    {
      //"pca形状特征值(shapes_eigen_values)"
      s->shapes_eigen_values = scanf_fmat(pf, &row, &col);
      ASSERT(1 == row);
      s->shapes_eigen_values_len = col;
    }
    {
      //"pca形状特征向量(ShapesEigenVectors)"
      s->ShapesEigenVectors = scanf_fmat(pf, &row, &col);
      ASSERT((s->NbOfPoints * 2) == row);
      ASSERT((s->shapes_eigen_values_len) == col);
    }
    {
      //"平均归一化profile向量(meanProfiles)"
      s->meanProfiles = scanf_fmat(pf, &row, &col);
      ASSERT((s->NbOfPoints * s->NumPyramidLevels) == row);
      ASSERT((s->TrnPntsAbove + s->TrnPntsBelow + 1) == col);
    }
    {
      //"profile协方差矩阵(covProfiles_inv)"
      int k = (s->TrnPntsAbove + s->TrnPntsBelow + 1);
      s->covProfiles_inv = scanf_fmat(pf, &row, &col);
      ASSERT((s->NbOfPoints * s->NumPyramidLevels) == row);
      ASSERT((k * k) == col);
    }
    fclose(pf);
  }

  asm_save_inl(s, "./asmTrainData_all.inl");
  asm_free(&s);
  return s;
}

static int asmfit_memsize(void* p0)
{
  asmfit_param_t* p = (asmfit_param_t*)p0;
  asm_t s[1] = {0};
  asm_fix_inl(s, p->asm_mode);
  ASSERT(NULL != p->asm_mode);
  p->buflen = ARRSIZE(float, 6 * 2 * s->NbOfPoints + s->shapes_eigen_values_len)
      + ARRSIZE(unsigned char*, s->NumPyramidLevels)
      + (4 * s->train_image_height * s->train_image_width) / 3;
  p->shape_result_len = s->NbOfPoints;
  p->contours_ending_points = s->contours_ending_points;
  p->contours_ending_points_len = s->contours_ending_points_len;
  return 0;
}

static int asmfit_process(void* p0)
{
  asmfit_param_t* p = (asmfit_param_t*)p0;
  asm_t s[1] = {0};
  int asmlen = 0;
  int h = p->height;
  int w = p->width;
  const unsigned char* I = p->data;
  int al = p->datastep;
  IPOINT leye = p->leye;
  IPOINT reye = p->reye;
  int ret;
  asm_fix_inl(s, p->asm_mode);

  //printf("ptr size = %d\n", sizeof(void*));
  //asm_load_txt("./asmTrainData_all.txt");
  BUFUSEBEGIN(p->buf, p->buflen);
#if 1
#if 1
  unsigned char* ImagePyramid[23];
  unsigned char* ImagePyramidBuf = NULL;
  float lx, ly, rx, ry;
  int buflen = (6 * 2 * s->NbOfPoints + s->shapes_eigen_values_len) * sizeof(float);
  char* buf = BUFMALLOC(char, buflen);
  lx = 120 / 320.f;
  ly = 0.5f;
  rx = 200 / 320.f;
  ry = 0.5f;
  ImagePyramidBuf = BUFMALLOC(unsigned char, (4 * s->train_image_height * s->train_image_width)/3);
  {
    int i, h, w;
    h = s->train_image_height, w = s->train_image_width;
    ImagePyramid[0] = ImagePyramidBuf;

    for (i=1; i<s->NumPyramidLevels; ++i) {
      ImagePyramid[i] = ImagePyramid[i - 1] + h * w;
      h /= 2, w /= 2;
    }
  }
  GetImagePyramid(h, w, I, al, s->NumPyramidLevels, s->if_filter,
      s->train_image_height, s->train_image_width, ImagePyramidBuf,
      leye, reye, lx, ly, rx, ry);  //获得金字塔分层图像
  ret = findResultShape_1DP(s, ImagePyramid, p->shape_result, buf, buflen);

  //BUFFREE(ImagePyramidBuf, (4 * s->train_image_height * s->train_image_width)/3 );
  //BUFFREE( buf, buflen );
  {
    IPOINT s1, e1;
    s1.x = (int)(lx * s->train_image_width);
    s1.y = (int)(ly * s->train_image_height);
    e1.x = (int)(rx * s->train_image_width);
    e1.y = (int)(ry * s->train_image_height);
    affine_point(s->NbOfPoints, p->shape_result, p->shape_result,
        leye, reye, s1, e1);
  }
#endif

#else
  memcpy(shape_result, StartShape, sizeof(float) * 2 * s->NbOfPoints);
#endif

  BUFUSEEND();
  return ret;
}

#endif // _ASMFIT_INL_
