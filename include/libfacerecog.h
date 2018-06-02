#ifndef _LIBFACERECOG_H_
#define _LIBFACERECOG_H_
#include <stdio.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" //
{
#endif
#ifndef COLOR_TYPE_DEF_DEF
#define COLOR_TYPE_DEF_DEF(COLOR_TYPE_DEF) \
  COLOR_TYPE_DEF(1, GRAY) \
  COLOR_TYPE_DEF(3, BGR) \
  COLOR_TYPE_DEF(3, RGB) \
  COLOR_TYPE_DEF(4, BGRA) \
  COLOR_TYPE_DEF(2, BGR555) \
  COLOR_TYPE_DEF(2, BGR565) \
  COLOR_TYPE_DEF(2, YUYV) \
  COLOR_TYPE_DEF(2, UYVY) \
  COLOR_TYPE_DEF(2, YUV420P) \
  COLOR_TYPE_DEF(2, YUV420) \
  COLOR_TYPE_DEF(2, YUV411P) \
  COLOR_TYPE_DEF(1, BayerGR) \
  COLOR_TYPE_DEF(1, BayerGB) \
  COLOR_TYPE_DEF(1, BayerBG) \
  COLOR_TYPE_DEF(3, HSV) \
  COLOR_TYPE_DEF(3, XYZ) \
  COLOR_TYPE_DEF(3, HLS) \
  COLOR_TYPE_DEF(3, Lab) \
  COLOR_TYPE_DEF(3, Luv) \
  COLOR_TYPE_DEF(3, YCrCb) \
  COLOR_TYPE_DEF(2, YCrYCb) \
  /* COLOR_TYPE_DEF(2, SBGGR8) */ \
  COLOR_TYPE_DEF(2, SN9C10X) \
  COLOR_TYPE_DEF(2, SGBRG) \
  COLOR_TYPE_DEF(12, HSV32F)
enum //
{
#define COLOR_TYPE_DEF(a, b)  T_##b,
COLOR_TYPE_DEF_DEF(COLOR_TYPE_DEF)
#undef COLOR_TYPE_DEF
};
#endif // COLOR_TYPE_DEF_DEF
#if 0
#define FEAT_T unsigned char
#define FEAT_MAX 255
#define FEAT_WORK_T int
#else
#if 1
#define FEAT_T unsigned char
#define FEAT_WORK_T unsigned int
#define FEAT_MAX 255
#else
#define FEAT_T float
#define FEAT_WORK_T float
#define FEAT_MAX 65532.f
#endif
#endif
#ifndef XRECT_DEFINED
#define XRECT_DEFINED
  // 矩形框
  typedef struct XRECT {
    int x, y, w, h;      /* 一个目标的所有框的位置平均值 */
    int score;           /* 一个目标的所有框的得分平均值 */
    int count;             /* 一个目标的所有框的个数       */
    //INT id;
  }
  XRECT;
#endif // XRECT_DEFINED
  /*
  建议参数：
  p.cas[1] = pNIRDoubleEYE;
  p.stepxy = 1, p.mincnt = 1, p.fastmode = 0, p.maxoutlen = 50, p.casnum = 1, p.cas[ 0 ] = pvis_nesting_face0701;
  p.isc = FR_F2I( 0.25f ), p.iss = FR_F2I( 1.1f ), p.ithd = FR_F2I( 0.8f ), p.issmin = FR_F2I( 1.f ), p.issmax = FR_F2I( 100.f );
  p.leyecasnum = 3, p.leyecas[ 0 ] = pNIREYEWithoutGlass, p.leyecas[ 1 ] = pNIREYELeftWithGlass , p.leyecas[ 2 ] = pNIRAllEYE;
  p.reyecasnum = 3, p.reyecas[ 0 ] = pNIREYEWithoutGlass, p.reyecas[ 1 ] = pNIREYERightWithGlass, p.reyecas[ 2 ] = pNIRAllEYE;
  */
#define STDIMGH   (100)
#define STDIMGW   (100)
#define FEAT_SIZE1 (2443)
#define FEAT_SIZE2 (2165)
  //#define FEAT_SIZE3 (1332) // 阈值=6372 最大=16645
  //#define FEAT_SIZE3 (1670)
  //#define FEAT_SIZE3 (2093)
#define FEAT_SIZE3 (2566) // 阈值=11047 最大=27113
#define FR_FACE_DETECT   (1<<0)
#define FR_EYES_DETECT   (1<<1)
#define FR_FACE_STD      (1<<2)
#define FR_FACE_FEATURE  (1<<3)
#define FR_FACE_RECOG    (1<<4)
#define FR_CVTCOLOR      (1<<5)
#define FR_IMRESIZE      (1<<6) // 
#define FR_ODDFIX        (1<<8) // 奇偶场
#define FR_ENCODE        (1<<9) // 编码 输入[featlib, 大小featstep] 输出[feature, featurelen] 输出大小必须>2*featstep
#define FR_DECODE        (1<<10) // 编码
#define FR_RECOG_TEST    (1<<11) // 人脸识别对比测试 用来去除不对称的误检 maxdist 保存距离值 如果 maxdist>25000*0.59 则不进行下面的步骤，并把检测框清0
#define FR_FEAT_23       (1<<12) // 使用2165+1332 的复合特征 p.leye.count ==id, p.leye.score==得分
#define FR_FACE_ALIGNMENT   (1<<13) // 使用2165+1332 的复合特征 p.leye.count ==id, p.leye.score==得分
#define FR_HASH_FEATURE  (1<<14)  // 哈希特征
#define FACERECOG_MODE_SORT_DIST   (1<<3)
#ifndef MAX_CAS_NUM
#define MAX_CAS_NUM (4)
#endif
  typedef struct facerecog_param_t {
    //输入
    //检测参数
    int height, width, /*图像的高宽*/
        outheight, outwidth, /*输出图像的高宽*/
        datastep, maskstep, outimgstep, /*图像的行宽*/
        datacn, maskcn, outimgcn, /*图像的通道*/
        datacode, outimgcode, /* 图像的编码类型 譬如：T_YUYV */
        stepxy, mincnt, maxoutlen, casnum, fastmode, /*人脸检测参数*/
        code, transopt;
    /*stepxy 步进数单位像素 mincnt 合并框数小于此处当成误检 casnum 分类器个数*/
    /* ssmin 最小检测框倍数,ssmax最大检测框倍数,ss 检测框缩放倍数 */
    /*cs 缩放倍数, thd 覆盖率阈值 */
#if 0
    real sc, ssmin, ssmax, ss, thd;
#else
    // 定点转浮点与浮点转定点的宏
#define FR_I2F(_X)  (double)((_X)/100.f)
#define FR_F2I(_X)  (int)((_X)*100)
    // 相应的浮点数乘以100 （比如原sc=0.12 ics=0.12*100）
    int isc, issmin, issmax, iss, ithd;
#endif
    unsigned char* data, *mask;
    unsigned char* outimg; // 编码转换后的图像
    void const* cas[MAX_CAS_NUM]; // 人脸分类器
    int deyecasnum, leyecasnum, reyecasnum;
    void const* deyecas[MAX_CAS_NUM];// 双眼分类器组
    void const* leyecas[MAX_CAS_NUM];// 左眼分类器组
    void const* reyecas[MAX_CAS_NUM];// 右眼分类器组
    XRECT roi;
    int featstart, featnum, featstep; // 从第几个特征开始，特征个数，特征步进
    const FEAT_T* featlib; // 人脸库
    int mode; // 模式选择
    //int* index;
    //输出
    XRECT face, deye, leye, reye; // [人脸框, 双眼框, 左眼框, 右眼框]
    unsigned char* stdface; // 100×100的归一化人脸
    int featurelen; // 特征大小
    FEAT_T* feature; // 人脸特征
    int distlen; // ==featnum
    int* dist; // 距离列表
    int maxdist;
    int maxdist_id;
    //int id[10];
    //int ds[10];
    int buflen;
    unsigned char* buf; // 临时空间大小
  } facerecog_param_t;
  
  int facerecog_memsize(void* p0);
  int facerecog_process(void* p0);
  void UYVY_to_YUY2(int n, unsigned char* in_Y, unsigned char* out_Y);
  void YUY2_to_UYVY(int n, unsigned char* in_Y, unsigned char* out_Y);
  void face_recog_open();
  void face_recog_close();
  int face_recog_process(void* p0);
  void* dsp_malloc(size_t n);
  void dsp_free(void* buf);
  void* dspmalloc(size_t n);
  void dspfree(void* p, size_t n);
  // maxFrameRate 码率越小压缩率越高，默认 25000
  int venc1_create(int height, int width, int maxFrameRate, int maxBitRate);
  int venc1_process(const unsigned char* src, int slen, unsigned char* dst, int dlen, int type);
  int venc1_delete();
  int vdec2_create(int height, int width);
  int vdec2_process(const unsigned char* src, int slen, unsigned char* dst, int dlen, int type);
  int vdec2_delete();
  int Vdec2a_create(int h, int w);
  int Vdec2a_delete();
  int Vdec2a_process(const char* hInBuf, int hInBufSz, char* hDstBuf, int hDstBufSz);
  typedef int (*decode_callback_fun_t)(unsigned char* data, int w, int h);
  int decode264(const char* filename, decode_callback_fun_t fun);
  // G711语音编码算法
  int Senc1_create();
  int Senc1_delete();
  // 压缩后大小=Senc1_process(原始数据指针, 原始数据大小, 目标空间指针, 目标空间大小, 0);
  int Senc1_process(const unsigned char* src, int slen, unsigned char* dst, int dlen, int type);
  // G711语音解码算法
  int Sdec1_create1();
  int Sdec1_delete1();
  // 解码后大小=Sdec1_process(原始数据指针, 原始数据大小, 目标空间指针, 目标空间大小, 0);
  int Sdec1_process1(const unsigned char* src, int slen, unsigned char* dst, int dlen, int type);

#ifdef __cplusplus
}
#endif
#endif // _LIBFACERECOG_H_

