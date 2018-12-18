
#ifndef _FACERECOG_INL_
#define _FACERECOG_INL_
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
//#include "../cstd.h"
//#include "../codec/Huffman.inl"
//#include "../codec/LZARI.h"
//#include "../codec/LZF.h" UINT
#include "../std/color.h"
#include "../codec/huff.inl"
//#include "SCIO_Export.h"

#include "std/popcount.h"
#include "_faceRecognition.h"

//int vvv;
//int vvv1;

//#define USE_FEATURE_GEN
#ifdef USE_FEATURE_GEN
//#include "feature_gen.h"
//#pragma comment(lib,"feature_gen.lib") // dll
//#pragma comment(lib,"feature_gen_lib.lib") // lib
#endif // USE_FEATURE_GEN

//

#if 0
//int feature_gen_inited = 0;
typedef int ( *fun_feature_gen_type ) ( int ah, int aw, const unsigned char* A, int al, int ai, int type, float* featptr );
typedef float ( *fun_dist_type ) ( int n, const float *a, const float *b );

fun_feature_gen_type fun_feature_gen = NULL;
fun_feature_gen_type fun_feature_gen2 = NULL;
fun_dist_type fun_cos_dist = NULL;

int feature_gen_init() {
  if ( 0 == feature_gen_inited ) {
    LOADDLL_BEGIN( _T("feature_gen.dll") );
    //fun_feature_gen = ( fun_feature_gen_type ) GETFUN( _T("feature_gen") );
    //fun_feature_gen2 = ( fun_feature_gen_type ) GETFUN( _T("feature_gen2") );
    fun_cos_dist = ( fun_dist_type ) GETFUN( "cos_dist" );
    feature_gen_inited = 1;
    LOADDLL_END();
  }
  return 1;
}
#endif

  /* 滤波测试函数（无用） */
int imfilter( int ah, int aw, const unsigned char* A, int al, int ai,
                          unsigned char* B, int bl, int bi, int x, int y ) {
#if 0
  SKIP_FILTEREX( ah, aw, A, al, ai, B, bl, bi, x, y, LBP_FT, unsigned char );
#else

  SKIP_FILTER_RCEX( ah, aw, A, al, ai, B, bl, bi, 9, 9, GAUSS13X13S2_FT, GAUSS13X13S2_FT, unsigned char );
#endif

  return 0;
}

#if 0
static int get_gauss( double* A, int mu, double sigma ) {
  int x, n = 2 * ( mu ) + 1;
  double invsi = -1.0f / ( 2.0f * sigma * sigma );
  *( A + mu ) = 1.f;
  for ( x = 1; x <= mu; ++x ) {
    *( A + mu + x ) = *( A + mu - x ) = ( double ) exp( ( x * x ) * invsi );
  }
  VEC_NOMELIZE( n, A );
  return 0;
}
#endif

//int vvv;
//int vvv1;


#include "pplight_16s.inl"
//#include "pplight_32f.inl"


#include "faceRecog_txt.inl"
#include "faceRecog_feat.inl"

// 函数功能：返回图像A和图像A的左右对称图像的距离
// 参数说明：
// type    - 特征类型(推荐值为： LBP59RECTBIN)
// 函数返回：图像A和图像A的左右对称图像的距离
// 使用说明：如果type非法则给默认值： LBP59RECTBIN
double CPM_SymmetryDist( int h, int w, const unsigned char* A, int al, int type ) {
  unsigned char * A2 = MALLOC( unsigned char, h * w );
  int i, j, len;
  double* featptr, *featptr2, dis = 0.f;
  //type = LBP59RECTBIN;
  if ( type < 0 || type > LBP59RECTBIN ) {
    type = LBP59RECTBIN;
  }
  len = CPM_GetFeature( h, w, A, al, 1, type, NULL );
  featptr = MALLOC( double, len * 2 );
  featptr2 = featptr + len;
  FILL( len * 2, featptr, 0.f );
  for ( i = 0; i < h; ++i ) {
    for ( j = 0; j < w; ++j ) {
      A2[ i * w + j ] = A[ i * al + w - j - 1 ];
    }
  }
  //imwrite("a.bmp", h, w, A2, w, 1);
  CPM_GetFeature( h, w, A, al, 1, type, featptr );
  CPM_GetFeature( h, w, A2, w, 1, type, featptr2 );
#if 0

  dis = CPM_ChiSquare( len, featptr, featptr2 );
#else
  // ABS_DISTOR2(len, featptr, featptr2, dis, double);
  {
    int i = 0;
    for ( i = 0; i < len; ++i ) {
      dis += ( double ) fabs( featptr[ i ] - featptr2[ i ] );
    }
  }
#endif
  FREE( A2 );
  FREE( featptr );
  return dis;
}

// 直方图交集距离
double CPM_Histinter( int n, const FEAT_T* a, const FEAT_T* b ) {
  int i;
  FEAT_T out = 0;
  for ( i = 0; i < n; ++i ) {
    out += ( a[ i ] < b[ i ] ? a[ i ] : b[ i ] );
  }
  return out;
}

#define MYIABS(a)  (((a)^((a)>>31))+(int)(((unsigned int)(a))>>31))

int CPM_AbsSub( int n, const FEAT_T* a, const FEAT_T* b ) {
  int j=0;
  int d = 0, d0=0, d1=0, d2=0, d3=0;
  int x0, x1, x2, x3;
  for (; j<n-3; j+=4) {
    x0 = a[j+0] - b[j+0];
    x1 = a[j+1] - b[j+1];
    x2 = a[j+2] - b[j+2];
    x3 = a[j+3] - b[j+3];
    d0 += MYIABS(x0);
    d1 += MYIABS(x1);
    d2 += MYIABS(x2);
    d3 += MYIABS(x3);
  }
  d = d0+d1+d2+d3;
  return d;
}

int CPM_FindMinDis(int featnum, int i, const FEAT_T* featptr, int featstep, int nb, const FEAT_T* feature, int featlen, int* pmaxdist)
{
  int maxdist = 999999;
  int maxdist_id=-1;
  for (; i<featnum; ++i, featptr+=featstep) {
    int j, d=0;
    FEAT_T* a=(FEAT_T*)feature, *b=(FEAT_T*)featptr;
#if 1
    int d0=0, d1=0, d2=0, d3=0;
    int x0, x1, x2, x3;
    j=0;
    for (; j<featlen-3; j+=4) {
      x0 = a[j+0] - b[j+0];
      x1 = a[j+1] - b[j+1];
      x2 = a[j+2] - b[j+2];
      x3 = a[j+3] - b[j+3];
      d0 += MYIABS(x0);
      d1 += MYIABS(x1);
      d2 += MYIABS(x2);
      d3 += MYIABS(x3);
    }
    d = d0+d1+d2+d3;
#endif
    for (; j<featlen; ++j) {
      int x0 = a[j] - b[j];
      d += MYIABS(x0);
    }
    if (d < maxdist) {
      maxdist = d;
      maxdist_id = i;
    }
  }
  if (pmaxdist) {
    *pmaxdist = maxdist;
  }
  return maxdist_id;
}
double CPM_AbsSub_32f( int n, const FEAT_T* a, const FEAT_T* b ) {
  return (double)CPM_AbsSub(n, a, b);
}

/* 卡方距离 */
double CPM_Chisquare( int n, const FEAT_T* A, const FEAT_T* B ) {
  int i = 0;
  double d = 0.f;
  for ( ; i < n; ++i ) {
    FEAT_WORK_T t0 = *( A + i ) + *( B + i );
    if ( t0 ) {
      FEAT_WORK_T _t = *( A + i ) - *( B + i );
      if ( _t ) {
        d += ( _t * _t ) / t0;
      }
    }
    //d += *(A+i  ) < *(B+i  ) ? *(A+i  ) : *(B+i  );
  }
  return d;
}

static int CPM_ChiSquare16s( int n, const short* a, const short* b ) {
  int ret;
#if 0

  CPM_ChiSquare( n, a, b, ret, double );
#else

  int i = 0;
  const short* A = a;
  const short* B = b;
  int d = 0;
#ifdef _OPENMP
  //#pragma omp parallel for num_threads(4) reduction(+:d) schedule(dynamic)
#endif // _OPENMP

  for ( ; i < n; ++i ) {
    int t0 = *( A + i ) + *( B + i );
    if ( t0 ) {
      int _t = *( A + i ) - *( B + i );
      if ( _t ) {
        d += ( _t * _t ) << 10 / t0;
      }
    }
    //d += *(A+i  ) < *(B+i  ) ? *(A+i  ) : *(B+i  );
  }
  ret = d;
#endif

  return ret;
}

typedef double (*distance_fun_t)(int n, const FEAT_T* a, const FEAT_T* b);

// 函数功能：向量距离
// 参数说明：
// n    - 特征数组大小
// a, b - 指向特征
// type - 距离类型
// 函数返回：
// 返回浮点距离值(值越小表示a和b越相似)
// 距离值域:D=[0,+inf]
// 映射到[0,1]的公式: S=1/(1+D)
// 使用说明：
double CPM_Distance( int n, const void* a, const void* b, int type ) {
  static distance_fun_t distance_table[4] = {0};
  static int distance_table_inited=0;
  double ret = -1;
  distance_fun_t fun;
  if (0==distance_table_inited) {
    distance_table_inited = 1;
    distance_table[CPM_CHISQUARE] = CPM_Chisquare;
    distance_table[CPM_HISTINTER] =  CPM_Histinter;
    distance_table[CPM_ABSSUB] =  CPM_AbsSub_32f;
  }
  fun = type<(int)(sizeof(distance_table)/sizeof(distance_table[0])) ? distance_table[type] : 0;
  if (fun) {
    ret = fun(n/sizeof(FEAT_T), (const FEAT_T *)a, (const FEAT_T *)b);
  }
  return ret;
}

#define dist_fun(_n, _a, _b)  CPM_AbsSub(_n, _a, _b)
//#define dist_fun(_n, _a, _b)  (dist_type)CPM_AbsSub(_n, _a, _b)

//#include "Transpose.inl"

int oddfix(int h, int w, unsigned char* A, int al) {
  int i;
  for (i=0; i<h; i+=2, A+=al, A+=al) {
    memcpy(A+al, A, w);
  }
  return 0;
}

int get_lbpex_feature(unsigned char* stdface, void* feature, unsigned char* buf, int buflen) {
  lbpex_param_t lp={0};
  lp.height = STDIMGH, lp.width = STDIMGW, lp.src = stdface, lp.srcstep = STDIMGW, lp.srccn = 1;
  lp.feature = (FEAT_T*)feature;//, lp.featurelen = p->featurelen;
  lp.select_type = 2, lp.fillborder = 1, lp.do_pp = 1;
  lp.buf = buf, lp.buflen = buflen;
  lbpex_process(&lp);
  return 0;
}

int get_chi_dist(const void* feature, const void* featptr, int n) {
  int j, d=0;
  FEAT_T* a=(FEAT_T*)feature, *b=(FEAT_T*)featptr;
  for (j=0; j<n; ++j) {
    d += a[j]>b[j]?a[j]-b[j]:b[j]-a[j];
  }
  return d;
}
int getsmsdist(int h, int w, const unsigned char* A, int al) {
  int i, j, s=0;
  for (i=0; i<h; ++i, A+=al) {
    for (j=0; j<w/2; ++j) {
      s += A[j]>A[w-j-1]?A[j]-A[w-j-1]:A[w-j-1]-A[j];
    }
  }
  return s;
}

#include "lbf_face_alignment.inl"

#if 0
int facerecog_process1( void* p0 );

int facerecog_process( void* p0 ) {
  int ret;
  facerecog_param_t* p = (facerecog_param_t*)p0;
  int i, j, isc[] = {10, 12, 18, 20};
  int mode[] = {FR_FACE_DETECT, FR_FACE_DETECT|FR_EYES_DETECT};
  const char* s_mode[] = {"FR_FACE_DETECT", "FR_FACE_DETECT|FR_EYES_DETECT"};
  int mode1 = p->mode&FR_TRANSPOSE;
  for (j=0; j<countof(mode); ++j) {
    for (i=0; i<countof(isc); ++i) {
      double t;
      utime_start(_start_time);
      p->isc = isc[i];
      p->mode = mode[j]|mode1;
      ret = facerecog_process1( p0 );
      t = utime_elapsed(_start_time);
      printf("time=%5.1lfms %d*%d sc=%3.1f ss=%3.1f eyew=%d mode=%s\n", t*1000., p->height, p->width, p->isc/100., p->iss/100., p->reye.w, s_mode[j]);
    }
  }
  printf("\n");
  return ret;
}
#endif

//#include "dnn/ncnn/mtcnn.inl"

#define FACE_RECOG_FALG_DEFDEF(DEF)  \
  DEF(NULL)  \
  DEF(OK)  \
  DEF(ARGS_ERR)  \
  DEF(CAN_NOT_DETECT_FACE)  \
  DEF(CAN_NOT_DETECT_EYES)  \
  DEF(FACE_IS_NOT_FRONT)

enum FACE_RECOG_FALG {
#define FACE_RECOG_FALG_DEF(x)  FACE_RECOG_FALG_ ## x,
FACE_RECOG_FALG_DEFDEF(FACE_RECOG_FALG_DEF)
#undef FACE_RECOG_FALG_DEF
};

const char* face_recog_falg_to_str(int flag) {
  static char* face_recog_falg_str[32] = {0};
#define FACE_RECOG_FALG_DEF(x)  face_recog_falg_str[FACE_RECOG_FALG_ ## x] = #x ;
FACE_RECOG_FALG_DEFDEF(FACE_RECOG_FALG_DEF)
#undef FACE_RECOG_FALG_DEF
  return face_recog_falg_str[flag&(countof(face_recog_falg_str)-1)];
}

int facerecog_process( void* p0 ) {
  facerecog_param_t* p = (facerecog_param_t*)p0;
  lut_detect_param_t pp={0};
  int mode = p->mode;
  int do_cvtcolor = mode & FR_CVTCOLOR;
  int select_type=(mode & FR_FEAT_23) ? 23 : 2;
  int ret = FACE_RECOG_FALG_OK;
  COPY_MEMBER9(p, &pp, height, width, datastep, maskstep, stepxy, mincnt, fastmode, maxoutlen, casnum);
  COPY_MEMBER7(p, &pp, data, mask, roi, cas[0], cas[1], cas[2], cas[3]);
  pp.sc = FR_I2F(p->isc), pp.ss = FR_I2F(p->iss), pp.thd = FR_I2F(p->ithd);
  pp.ssmin = FR_I2F(p->issmin), pp.ssmax = FR_I2F(p->issmax);
  pp.buf = p->buf, pp.buflen = p->buflen;
  pp.is_trans = 0;
  pp.is_flip = 0;
  pp.is_flop = 0;
  pp.buflen -= pp.maxoutlen * sizeof(XRECT);
  pp.out = (XRECT*)(pp.buf);
  pp.buf += pp.maxoutlen * sizeof(XRECT);
  pp.data = p->data;
  pp.fastmode=0;
  memset(&(p->face), 0, sizeof(XRECT));
  memset(&(p->deye), 0, sizeof(XRECT));
  memset(&(p->reye), 0, sizeof(XRECT));
  memset(&(p->leye), 0, sizeof(XRECT));
  //p->face.h=p->face.w=100;
  //return 0;
  if (p->data && ((mode & FR_FACE_DETECT) || (mode & FR_EYES_DETECT) || (mode & FR_FACE_STD))) {
    if (mode & FR_ODDFIX) {
      oddfix(p->height, p->datastep, (unsigned char*)p->data, p->datastep);
      //cvShowImage("asdf", p->height, p->width, p->data, p->datastep, 1); cvWaitKey(-1);
    }
    if (T_GRAY!=p->datacode) {
      int ret;
      unsigned char* gray;
      pp.buflen -= p->height* p->width;
      pp.data = gray = pp.buf + pp.buflen;
      pp.datastep = p->width;
      ret = colorcvt(p->height, p->width, p->data, p->datastep, 0, gray, p->width, 1, (ColorSpace)p->datacode, T_GRAY);
      //imwrite("./graya.bmp", p->height, p->width, gray, p->width, 1);
      if (!ret) {
        mode = 0; // 失败
      }
    }
#if 0
    if (mode & FR_TRANSPOSE && !is_trans) {
      int t;
      unsigned char* gray;
      pp.buflen -= pp.height* pp.width;
      gray = pp.buf + pp.buflen;
      //IMRESIZE(pp.height, pp.width, pp.data, pp.datastep, 1, pp.height/2, pp.width/2, gray, 1, pp.height/2, INTER_BILINEARITY);
      //cvShowImage("asdf", pp.width/2, pp.height/2, gray, pp.height/2, 1); cvWaitKey(-1);
      //TRANSPOSE(pp.height, pp.width, pp.data, pp.datastep, gray, pp.height);
      Transpose_8uC12(pp.height, pp.width, pp.data, pp.datastep, gray, pp.height);
      //Transpose_8u1C(pp.height, pp.width, pp.data, pp.datastep, gray, pp.height);
      CC_SWAP(pp.width, pp.height, t);
      pp.datastep = pp.width;
      pp.data = gray;
      //cvShowImage("asdf", pp.height, pp.width, pp.data, pp.datastep, 1); cvWaitKey(-1);
    }
#endif
  }
  if (p->fastmode) {
    int t;
    unsigned char* gray = (unsigned char*)pp.data;
    if (p->fastmode&1) {
      FLOP2D(pp.height, pp.width, gray, pp.datastep, 1, t);
    }
    if (p->fastmode&2) {
      FLIP2D(pp.height, pp.width, gray, pp.datastep, 1, t);
    }
  }
#if 0
  printf("width=%d height=%d step=%d\n", p->width, p->height, p->datastep);
  printf("p->isc=%d\n", p->isc);
  printf("pp.is_trans=%d\n", pp.is_trans);
  printf("p->buflen=%d\n", p->buflen);
  printf("p->datacode=%d\n", p->datacode);
  printf("p->mode=%d\n", mode);
  if (0==mode) {
    mode = FR_FACE_DETECT|FR_EYES_DETECT;
  }
#endif
  //printf("aaa.bmp\n");
    //IMWRITE("aaa.bmp", pp.height, pp.width, pp.data, pp.width, 1);
#if 0
  if (mode & FR_FACE_DETECT) {
    //_chdir("C:/");
    Bbox out[10];
    img_t im[1] = {0};
    if(p->datacn!=3) {
      mode=0;
      return 0;
    }
    buf_t bf[1] = {0};
    bfinit(bf, p->buf, p->buflen);
    IMINIT(im, p->height, p->width, p->data, p->datastep, 3, 1);
    int nface = mtcnn_findFace(bf, im, out, countof(out), 160);
    //imshow_(im);WaitKey(-1);
    int i, iface = -1;
    if (nface>0) {
      iface = 0;
      for (i=0; i<nface; ++i) {
        if (out[i].area>out[iface].area) {
          iface = i;
        }
      }
      float3 eav={0};
      EstimateHeadPose(out[iface].ppoint, eav);
      float etPitch=10;
      float etYaw=7;
      float etRoll=10;
      //printf("Pitch=%f, Yaw=%f, Roll=%f\n", eav[0], eav[1], eav[2]);
      if (eav[0]>12 || eav[0]<-4 || eav[1]>etYaw || eav[1]<-etYaw || eav[2]>etRoll || eav[2]<-etRoll) {
        mode=0;
        ret = FACE_RECOG_FALG_FACE_IS_NOT_FRONT;
      } else {
        p->face.y = out[iface].x1;
        p->face.x = out[iface].y1;
        p->face.h = out[iface].x2-out[iface].x1;
        p->face.w = out[iface].y2-out[iface].y1;
        p->leye.x = out[iface].ppoint[0];
        p->leye.y = out[iface].ppoint[5];
        p->leye.w = 1;
        p->leye.h = 1;
        p->reye.x = out[iface].ppoint[1];
        p->reye.y = out[iface].ppoint[6];
        p->reye.w = 1;
        p->reye.h = 1;
      }
    } else {
      mode=0;
      ret = FACE_RECOG_FALG_CAN_NOT_DETECT_FACE;
    }
  }
#else
  //cvShowImage("pp.data", pp.height, pp.width, pp.data, pp.datastep, 1); //cvWaitKey(-1);
  if (mode & FR_FACE_DETECT) {
    memset(&(p->face), 0, sizeof(XRECT));
    memset(&(p->deye), 0, sizeof(XRECT));
    //COPY_MEMBER5(p, &pp, sc, ssmin, ssmax, ss, thd);
    detect_process(&pp);
    if (pp.outlen>0) {
#if 0
      int i=0;
      for (i=0; i<pp.outlen; ++i) {
        DrawRectangle2(pp.width, pp.height, pp.data, pp.datastep, 1, pp.out[i], CC_RGB(255, 0, 0), 2);
      }
      cvShowImage("asdf", pp.width, pp.height, pp.data, pp.datastep, 1);
      cvWaitKey(-1);
#endif
      p->face = pp.out[0];
    } else {
      mode=0;
    }
    p->face.count=pp.outlen;
  }
#if 0
  {// 人脸角度检测 失败
#define SSH 10
#define SSW 10
    unsigned char stdface[SSH*SSW];
    int k=10, m=3;
    int d = p->face.w/4, a = p->face.w/(2*k), b, bb, dis, mindis=1000000;
    IPOINT leye, reye;
    int i;
    for (i=k-m; i<=k+m; ++i) {
      b = i*a-k*a;
      leye = iPOINT(p->face.x+d, p->face.y+d-b), reye = iPOINT(p->face.x+3*d, p->face.y+d);
      CPM_FaceStd( p->height, p->width, pp.data, pp.datastep, 1, SSH, SSW, stdface, SSW, 1,
      leye, reye, 0.25, 0.25, 0.75, 0.25 );
      dis = getsmsdist(SSH, SSW, stdface, SSW);
      if (dis<mindis) {
        bb = b, mindis = dis;
      }
    }
    printf("%d\n", bb);
    leye = iPOINT(p->face.x+d, p->face.y+d-bb), reye = iPOINT(p->face.x+3*d, p->face.y+d+bb);
    CPM_FaceStd( p->height, p->width, pp.data, pp.datastep, 1, SSH, SSW, stdface, SSW, 1,
      leye, reye, 0.25, 0.25, 0.75, 0.25 );
    DrawLine( SSH, SSW, stdface, SSW, 1, iPOINT(25,25), iPOINT(75,25), CC_RGB( 0, 0, 0 ), 3, 0, 0 );
    cvShowImage("stdface", SSH, SSW, stdface, SSW, 1);
  }
#endif
  if (mode & FR_EYES_DETECT) {
    if (mode & FR_FACE_ALIGNMENT) {
    int detect_eyes1(const void* model, int h, int w, const unsigned char* data, int datastep, const XRECT* face, XRECT* deye, XRECT* leye, XRECT* reye, int is_trans);
    detect_eyes1(p->leyecas[ 0 ], pp.height, pp.width, pp.data, pp.datastep, &(p->face), &(p->deye), &(p->leye), &(p->reye), 0);
    } else {
      int ret = 0;
      memset(&(p->leye), 0, sizeof(XRECT));
      memset(&(p->reye), 0, sizeof(XRECT));
      if (p->data && p->face.w && p->face.h) {
        int max_width = pp.width;
        if (0) {
          int mm=MIN(p->height, p->width);
          int d=((mm-p->face.w*2)*5)/p->face.w, d2=d*2;
          if (d>0) {
            p->face.x+=d;
            p->face.y+=d;
            p->face.w-=d2;
            p->face.h-=d2;
          }
      }
      ret = detect_eyes(pp.height, pp.width, pp.data, pp.datastep, p->deyecas, p->leyecas, p->reyecas,
        &(p->face), &(p->deye), &(p->leye), &(p->reye), pp.buf, pp.buflen, pp.out, pp.maxoutlen, max_width, 0, 0, 0);
      }
      if (!ret) {
        mode=0;
      } else if (0) {
        if (0) {
          int t1=p->leye.y-p->face.y;
          int t2=p->face.y+p->face.h-p->reye.y-p->reye.h;
          p->face.y+=(t1-t2)/2;
          p->deye.y+=(t1-t2)/2;
        } else {
          int t1=p->leye.x-p->face.x;
          int t2=p->face.x+p->face.w-p->reye.x-p->reye.w;
          p->face.x+=(t1-t2)/2;
          p->deye.x+=(t1-t2)/2;
        }
      }
    }
  }
#endif
  if ((mode & FR_FACE_STD)) {
    if (pp.data && p->stdface) {
      IPOINT leye, reye;
      int i, s=0;
      memset(p->stdface, 0, STDIMGH*STDIMGW);
      leye.x = p->leye.x + p->leye.w / 2, leye.y = p->leye.y + p->leye.h / 2;
      reye.x = p->reye.x + p->reye.w / 2, reye.y = p->reye.y + p->reye.h / 2;
      CPM_FaceStd( p->height, p->width, pp.data, pp.datastep, 1, STDIMGH, STDIMGW, p->stdface, STDIMGW, 1,
        leye, reye, 0.25, 0.25, 0.75, 0.25 );
      for (i=0; i<STDIMGH*STDIMGW; ++i) {
        s += p->stdface[i];
      }
#if 0
      if (s<(40*STDIMGH*STDIMGW) || s>(200*STDIMGH*STDIMGW)) {
        p->face.w=p->face.h=0, p->deye.w=p->deye.h=0;
        p->leye.w=p->leye.h=0, p->reye.w=p->reye.h=0;
        mode=0;
      }
#endif
    } else {
      mode=0;
    }
  }

  if ((mode & FR_FACE_FEATURE)) {
    //cvShowImage("4", 100, 100, p->stdface, 100, 1);
    if (p->feature && p->stdface) {
      lbpex_param_t lp={0};
      lp.height = STDIMGH, lp.width = STDIMGW, lp.src = p->stdface, lp.srcstep = STDIMGW, lp.srccn = 1;
      lp.feature = (FEAT_T*)p->feature;//, lp.featurelen = p->featurelen;
      lp.select_type = select_type, lp.fillborder = 1, lp.do_pp = 1;
      lp.buf = p->buf, lp.buflen = p->buflen;
#if 0
      {
        FILE* pf = fopen("./std_1_0.txt", "rb");
        fread(lp.src, 10000, 1, pf);
        imwrite("./std_1_0.bmp", 100, 100, lp.src, 100, 1);
        fclose(pf);
      }
#endif
      lbpex_process23(&lp);
      mode |= FR_HASH_FEATURE;
#if 0
      {
        int i;
        FEAT_T fff[2500];
        FILE* pf = fopen("./feat_1_0.txt", "rb");
        fread(fff, 2500, 1, pf);
        for (i=0; i<2000; ++i) {
          if (fff[i]-lp.feature[i]) {
            int asdf=0;
          }
        }
        fclose(pf);
      }
#endif
    } else {
      mode=0;
    }
  }
#if 1
  //cvShowImage("0", 100, 100, p->stdface, 100, 1);
  if ((mode & FR_RECOG_TEST) && (mode & FR_FACE_FEATURE)) {
    if (p->stdface) {
      unsigned char* stdface2=p->stdface;
      FEAT_T *feat1, *feat2;
      unsigned char* buf=0, cht;
      int buflen=0, dist=0;
      feat1 = (FEAT_T*)(p->feature);
      feat2 = (FEAT_T*)(p->buf);
      buf=(unsigned char*)(p->buf+2200*sizeof(FEAT_T));
      buflen=p->buflen-(2200*sizeof(FEAT_T));
      //get_lbpex_feature(stdface2, feat1, buf, buflen);
      //memcpy(feat1, p->feature, p->featurelen);
      //cvShowImage("1", 100, 100, stdface2, 100, 1);
      FLOP2D(100, 100, stdface2, 100, 1, cht);
      //cvShowImage("2", 100, 100, stdface2, 100, 1);cvWaitKey(-1);
      get_lbpex_feature(stdface2, feat2, buf, buflen);
      FLOP2D(100, 100, stdface2, 100, 1, cht);
      //cvShowImage("3", 100, 100, stdface2, 100, 1);//  cvWaitKey(0);
      dist = get_chi_dist(feat1, feat2, FEAT_SIZE2);
      //p->maxdist = dist;
      //p->maxdist_id = -1;
      printf("dist=%d\n", dist);
      if (dist>12050) {
        // 25000*0.59=14750
        //p->face.w=p->face.h=0, p->deye.w=p->deye.h=0;
        p->leye.w=p->leye.h=0, p->reye.w=p->reye.h=0;
        mode=0;
      }
    } else {
      mode=0;
    }
  }
  if ((mode & FR_HASH_FEATURE)) {
      //int featlen = p->featurelen/sizeof(FEAT_T);
      uint h = *(uint*)(p->feature) = hashfeat2165(p->feature);
  }
  {
    //int d;
    //FEAT_T feature1[2555];
    //memset(feature1, 255, sizeof(feature1));
    //d = dist_fun( p->featurelen/sizeof(FEAT_T), (void*)feature1, (void*)p->feature );
    //d = d;
  }
  //p->maxdist_id = -1;
  //p->maxdist=mode;
  p->maxdist = MAX_INT, p->maxdist_id = -1;
  if ((mode & FR_FACE_RECOG))
  {
    //int kk=10;    for (; kk--;)
    if (p->feature && p->featlib && p->featnum>0)
    {
      int i;
      char *feature, *featptr;
      int featlen = p->featurelen/sizeof(FEAT_T);
      uint ha = *(uint*)(p->feature);
      feature = (char*)p->feature;
      featptr = (char*)p->featlib + p->featstart * p->featstep;
      p->maxdist_id = -2;
      p->maxdist=1000000000;
      
      if (2==select_type) {
        if (mode & FR_HASH_FEATURE) {
          for (i=p->featstart; i<p->featnum; ++i, featptr+=p->featstep) {
            int j, d=0;
            FEAT_T* a=(FEAT_T*)feature, *b=(FEAT_T*)featptr;
            uint hb = *(uint*)(b);
            uint disham = popcnt_lookup2(ha^hb);
            if (disham<10) {
#if 1
              int d0=0, d1=0, d2=0, d3=0;
              int x0, x1, x2, x3;
              j=0;
              for (; j<featlen-3; j+=4) {
                x0 = a[j+0] - b[j+0];
                x1 = a[j+1] - b[j+1];
                x2 = a[j+2] - b[j+2];
                x3 = a[j+3] - b[j+3];
                d0 += MYIABS(x0);
                d1 += MYIABS(x1);
                d2 += MYIABS(x2);
                d3 += MYIABS(x3);
              }
              d = d0+d1+d2+d3;
#endif
              for (; j<featlen; ++j) {
                int x0 = a[j] - b[j];
                d += MYIABS(x0);
              }
              if (d < p->maxdist) {
                p->maxdist = d;
                p->maxdist_id = i;
              }
            }
          }
        } else {
          for (i=p->featstart; i<p->featnum; ++i, featptr+=p->featstep) {
            int j, d=0;
            FEAT_T* a=(FEAT_T*)feature, *b=(FEAT_T*)featptr;
#if 1
            int d0=0, d1=0, d2=0, d3=0;
            int x0, x1, x2, x3;
            j=0;
            for (; j<featlen-3; j+=4) {
              x0 = a[j+0] - b[j+0];
              x1 = a[j+1] - b[j+1];
              x2 = a[j+2] - b[j+2];
              x3 = a[j+3] - b[j+3];
              d0 += MYIABS(x0);
              d1 += MYIABS(x1);
              d2 += MYIABS(x2);
              d3 += MYIABS(x3);
            }
            d = d0+d1+d2+d3;
#endif
            for (; j<featlen; ++j) {
              int x0 = a[j] - b[j];
              d += MYIABS(x0);
            }
            if (d < p->maxdist) {
              p->maxdist = d;
              p->maxdist_id = i;
            }
          }
        }
      } else if (23==select_type) {
        p->leye.score = 1000000000;
        p->leye.count=-2;
        for (i=p->featstart; i<p->featnum; ++i, featptr+=p->featstep) {
          int j, d0=0, d1=0;
          FEAT_T* a=(FEAT_T*)feature, *b=(FEAT_T*)featptr;
          for (j=0; j<FEAT_SIZE2; ++j) {
            int x0 = a[j] - b[j];
            d0 += MYIABS(x0);
          }
          for (j=FEAT_SIZE2; j<FEAT_SIZE2+FEAT_SIZE3; ++j) {
            int x0 = a[j] - b[j];
            d1 += MYIABS(x0);
          }
          if (d0 < p->maxdist) {
            p->maxdist = d0;
            p->maxdist_id = i;
          }
          if (d1 < p->leye.score) {
            p->leye.score = d1;
            p->leye.count = i;
          }
        }

      }
    }
  }
#if 0
  if (do_transpose && !is_trans) {
    int t;
    CC_SWAP(p->face.x, p->face.y, t), CC_SWAP(p->face.w, p->face.h, t);
    CC_SWAP(p->deye.x, p->deye.y, t), CC_SWAP(p->deye.w, p->deye.h, t);
    CC_SWAP(p->leye.x, p->leye.y, t), CC_SWAP(p->leye.w, p->leye.h, t);
    CC_SWAP(p->reye.x, p->reye.y, t), CC_SWAP(p->reye.w, p->reye.h, t);
  }
#endif
  if (do_cvtcolor) {
    p->outimgcn = colorcn((ColorSpace)p->outimgcode);
    if ((mode & FR_IMRESIZE) &&
      (T_BGR == p->datacode || T_RGB == p->datacode || T_GRAY == p->datacode )
      ) {
      IMRESIZE(p->height, p->width, p->data, p->datastep, p->datacn, p->outheight, p->outwidth, p->buf, p->outimgstep, p->outimgcn, INTER_BILINEARITY);
      colorcvt(p->outheight, p->outwidth, p->buf, p->outimgstep, 0, p->outimg, p->outimgstep, 0, (ColorSpace)p->datacode, (ColorSpace)p->outimgcode);
    } else {
      colorcvt(p->height, p->width, p->data, p->datastep, 0, p->outimg, p->outimgstep, 0, (ColorSpace)p->datacode, (ColorSpace)p->outimgcode);
    }
    p->outimgstep = p->outimgcn*p->outwidth;
  }
#define COMPALG HUFF
#define COMPALG_Encode0 HUFF_Encode0
#define COMPALG_Decode0 HUFF_Decode0
  if (((mode & FR_ENCODE)||(mode & FR_DECODE))&&p->feature&&p->featlib) {
    int newfeaturelen=0;
    int datalen=p->featstep;
    COMPALG* L = (COMPALG*)(p->buf);
    ASSERT(p->buflen>sizeof(COMPALG));
    if (mode & FR_ENCODE) {
      //abab2aabb(p->feature, p->featurelen, buf1);
      newfeaturelen = COMPALG_Encode0(L, p->featlib, datalen, p->feature, p->featurelen);
    } else if (mode & FR_DECODE) {
      newfeaturelen = COMPALG_Decode0(L, p->featlib, datalen, p->feature, p->featurelen);
    }
    p->featurelen = newfeaturelen;
  }
#endif
  if (p->maxdist>100000) {
    //p->maxdist = ((unsigned short*)(p->featlib))[2000];
  }
  //p->maxdist = p->featstart;
  return ret;
}


#endif // _FACERECOG_INL_
