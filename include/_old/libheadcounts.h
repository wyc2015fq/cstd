#ifndef _LIBHEADCOUNTS_H_
#define _LIBHEADCOUNTS_H_
#include <stdio.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
  ;
#endif
#define COUNTOF_PI  5
#define MAXOBJ  32
#define MAXPT 1000
#define SERVERPROCID 10
#define SERVERPROCPORT 4444
#if 1
#define _WW 640
#define _HH 480
#else
#define _WW 320
#define _HH 240
#endif
  typedef struct ptid_t {
    POINT pt[MAXOBJ];
    int id[MAXOBJ];
    int ptn;
  } ptid_t;
  typedef struct seid_t {
    POINT s, e;
    POINT pt[MAXPT];
    int ptlen;
    int id;
    int cnted_top;
    int cnted_bot;
  } seid_t;
  typedef struct headcounts_t {
    int h, w;
    int ImerodeThresh; // 腐蚀阈值
    int ImdilateThresh; // 膨胀阈值
    int AreaThresh; // 面积阈值
    int FilterLayerThresh; // 深度滤波
    int FilterAreaThresh; // 面积滤波
    int verson; // 选择进行腐蚀
    int BRsize;
    int RN, RD;
    int idmax;
    int lables;
    int Order_verson;
    // 前景倍数
    int beishu;
    int top2bot;
    int bot2top;
    int topIn;
    int topOut;
    int botIn;
    int botOut;
    int cnt_t1;
    int cnt_t2;
#define MAXSEID 100
    seid_t se[MAXSEID];
    ptid_t pi[COUNTOF_PI];
    unsigned short* depth;
    unsigned char* BI;
    unsigned char* FrontI;
    unsigned char* FAI;
    unsigned short* RDI;
    unsigned short* FRDI;
    unsigned short* g;
    unsigned char* buf;
    int buflen;
    POINT pt1, pt2;
  } headcounts_t;
  typedef unsigned int SOCKET_T;
  // 客户端连接人头检测服务器
  // ipaddrstr [in] 人头检测服务器IP及端口，例如 192.168.1.7:4444
  // 返回套接字
  SOCKET_T ClientProc_beg(const char* ipaddrstr);
  // 关闭接
  // svi [in] 套接字
  int ClientProc_end(SOCKET_T svi);
  // 命令标志
  enum {
    NET_HEADCOUNTS_SETZRRO = 1 << 0, // 将服务器统计结果清0
    NET_HEADCOUNTS_SETPARAM = 1 << 1, // 设置服务器检测参数
    NET_HEADCOUNTS_GETCOLOR = 1 << 2, // 从服务器获取彩色图像 大小为 _HH*_WW*3 字节
    NET_HEADCOUNTS_GETDEPTH = 1 << 3, // 从服务器获取深度图像 大小为 _HH*_WW 字节
  };
  // 发送命令
  // cmd [in] 命令掩码 可以使 NET_HEADCOUNTS_SETZRRO等标志，的组合
  // ph0 [in out] 包含参数，和统计结果
  // BGRImg [out] 指向彩色图像地址
  // DepthImg [out] 指向深度图像地址
  int ClientProc_cmd(SOCKET_T svi, int cmd, headcounts_t* ph0,
      unsigned char* BGRImg, unsigned char* DepthImg);
#ifdef __cplusplus
}
#endif
#endif // _LIBHEADCOUNTS_H_

