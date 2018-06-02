
/***************************************************************
faceRecognition   date: 06/03/2009
------------------------------------------------------------- 
版本: 1.0.0.1
作者：易伟
说明：人脸图像规一化、预处理、特征提取、比对
-------------------------------------------------------------
创合世纪 Copyright (C) 2009 - All Rights Reserved
****************************************************************/
#ifndef _FACERECOGNITION_H_
#define _FACERECOGNITION_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _USE_STDCALL_
#define CHC_STDCALL __stdcall
#else
#define CHC_STDCALL __cdecl
#endif


#ifndef REAL_TYPE
  // 基本类型
  typedef int INT;
  typedef float real;
  typedef unsigned char uchar;
#define USEFLOT
#ifdef USEFLOT
#define REAL_TYPE real
#else
#define REAL_TYPE int
#endif // USEFLOT
#endif

  // point结构
#ifndef _DEFINED_IPOINT_
#define _DEFINED_IPOINT_

  typedef struct tagIPOINT {
    int x;
    int y;
  }
  IPOINT, *PIPOINT;
#endif

#ifndef _DEFINED_FPOINT_
#define _DEFINED_FPOINT_

  typedef struct tagFPOINT {
    float x;
    float y;
  }
  FPOINT, *PFPOINT;
#endif


  /*
   函数功能：人脸规一化
   参数说明：
   [ah, aw, A, al, ai] - 输入图片
   [bh, bw, B, bl, bi] - 输出图片
   leye, reye - 左右眼中心位置
   lx - 左眼x坐标除以bw 一般取 0.25
   ly - 左眼y坐标除以bh 一般取 0.25
   rx - 右眼x坐标除以bw 一般取 0.75
   ry - 右眼y坐标除以bh 一般取 0.25
   函数返回：无意义
   使用说明：
  */
  int CHC_STDCALL CHC_FaceStd(int ah, int aw, const uchar* A, int al, int ai,
                              int bh, int bw, uchar* B, int bl, int bi,
                              IPOINT leye, IPOINT reye,
                              real lx, real ly, real rx, real ry);

  /* 滤波测试函数（无用） */
  int CHC_STDCALL imfilter(int ah, int aw, const uchar* A, int al, int ai,
                           uchar* B, int bl, int bi, int x, int y);

  /*
   函数功能：对输入的灰度图像pp预处理
   参数说明：
   [ah, aw, A, al, ai] - 输入图片（规一化图像）
   [ah, aw, B, bl, bi] - 输出图片
   函数返回：无意义
   使用说明：
  */
  int CHC_STDCALL CHC_PpLight(int h, int w, const uchar* A, int al, int ai, uchar* B, int bl, int bi);

  /*
   函数功能：对输入的灰度图像提取特征
   参数说明：
   输入：
   [ah, aw, A, al, ai] - 一副规一化图像或pp处理后的图像
   type                - 特征类型
   输出：
   featptr             - 输出特征数组，指针由右外部分配，指针为NULL时返回所需real的个数
   返回 - 特征数组大小
   注意：只有PPBOOSTLBPXBIN特征要100×100的图像，其他特征不需要
   函数返回：如果只给图像高和宽，返回特征向量维度
   使用说明：
  */
  int CHC_STDCALL CHC_GetFeature(int ah, int aw, const uchar* A, int al, int ai, int type, real* featptr);

  // 定点版本的特征提取函数
  int CHC_STDCALL lbp_ex_ft_16s(int srch, int srcw, const uchar* src, int srcstep, int cn, short* featptr, int isall, int fillborder);

#define CHI_DIST(a, b, out)  { out = (a) + (b); (out) = (out) ? (((a)-(b))*((a)-(b))/(out)) : 0; }
#define ABS_DIST(a, b, out)  { (out) = ((a)>(b)) ? ((a)-(b)) : ((b)-(a)); }

  /*
   函数功能：向量距离
   参数说明：
   n    - 特征数组大小
   a, b - 指向特征
   type - 距离类型
   函数返回：
   返回浮点距离值(值越小表示a和b越相似)
   距离值域:D=[0,+inf]
   映射到[0,1]的公式: S=1/(1+D)
   使用说明：
  */
  real CHC_STDCALL CHC_Distance(int n, const real* a, const real* b, int type);

  int CHC_STDCALL CHC_ChiSquare16s(int n, const short* a, const short* b);

  /*
   函数功能：返回图像A和图像A的左右对称图像的距离
   参数说明：
   type    - 特征类型(推荐值为： LBP59RECTBIN)
   函数返回：图像A和图像A的左右对称图像的距离
   使用说明：如果type非法则给默认值： LBP59RECTBIN
  */
  real CHC_STDCALL CHC_SymmetryDist(int h, int w, const uchar* A, int al, int type);

#define RECOGNITION_0 (0)  /* 适用场合为可见光的计算机管理，采用特征类型11，距离度量类型0，要求输入图片为80*80，特征长度为64000维，适用场合为可见光的                                    计算机管理，注册人数1-10个人的系统，归一化的最小值为0，最大值为50000，阈值为0.52，大于0.52则为拒绝通过，小于0.52则为通过*/
#define RECOGNITION_1 (1)  /* 适用场合为智能视频监控系统的可见光黑名单比对系统可见光，采用特征类型11，距离度量方式2，要求输入图片为80*80，特征                                               长度为64000维，，归一化的最小值为0，最大值为288550，阈值为0.52，小于0.52则为拒绝通过，大于0.52则为通过*/
#define RECOGNITION_2 (2)  /* 适用场合为近红外的计算机管理系统，注册人数为1-10个人。采用特征类型11，距离度量方式2，要求输入图片为80*80，特征长度为64000维，                                                     比对系统，归一化的最小值为0，最大值为169000 ，阈值为0.52，小于0.52则为拒绝通过，大于0.52则为通过*/
#define RECOGNITION_3 (3)  /* 适用场合为PC机版,注册人数为（1-600个人）的近红外的计算机管理，考勤，门禁系统。采用的特征类型为6，距离度量方式为0。要求输                                    入的图片为100*100，特征长度为2243维，归一化的最小值为0，最大值为8000，阈值为0.51，识别得分大于0.51则拒绝通过，小于0.51则                                    为通过。*/
#define RECOGNITION_4 (4)  /* 适用场合为人脸图像质量的判断，判断图像是否为正面合格人脸。采用特征类型9，要求输入图片为100*100，得分大于100000则人脸图                                     像质量不合格 */
#define RECOGNITION_5 (5)  /* 适用场合为PC机版,注册人数为（1-600个人）的近红外的计算机管理，考勤，门禁系统。采用的特征类型为12，距离度量方式为0。要求                                   入的图片为100*100，特征长度为2243维，归一化的最小值为0，最大值为12154，阈值为0.52，识别得分大于0.52则拒绝通过，小于0.52                                       为通过。针对尉秦出现问题对算法的一个调整  */

  /* 识别算法 */
  typedef struct RECOALGO {
    int feat_type;    /* 特征类型 */
    int feat_dims;    /* 特征维度 */
    int std_height;   /* 规一化图片高，0表示任意值 */
    int std_width;    /* 规一化图片高，0表示任意值 */
    int std_cn;       /* 规一化图片通道数 */
    int is_dist;      /* 1表示是距离，0表示相似度 */
    real dist_min;    /* 距离最小值，用于把距离规一化到0~1之间 */
    real dist_max;    /* 距离最大值，公式：(距离值-dist_min)/(dist_max-dist_min) */
    real dist_th;     /* 建议阈值 */
    int dist_type;    /* 距离类型 */
  }
  RECOALGO;

  /*
   函数功能：取输入识别算法类型ID的特征信息
   参数说明：
   输入：
   type                - 特征类型
   输出：识别算法结构的指针，NULL表示没有这个特征的信息
  */
  const struct RECOALGO* CHC_STDCALL CHC_GetRecoAlgoInfo(int type);

#ifdef __cplusplus
}
#endif

#endif // _FACERECOGNITION_H_
