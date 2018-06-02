/********************************************************************

  iseecnv.c

  ----------------------------------------------------------------
    软件许可证 － GPL
  版权所有 (C) 2003 VCHelp coPathway ISee workgroup.
  ----------------------------------------------------------------
  这一程序是自由软件，你可以遵照自由软件基金会出版的GNU 通用公共许
  可证条款来修改和重新发布这一程序。或者用许可证的第二版，或者（根
  据你的选择）用任何更新的版本。

    发布这一程序的目的是希望它有用，但没有任何担保。甚至没有适合特定
  目地的隐含的担保。更详细的情况请参阅GNU通用公共许可证。

    你应该已经和程序一起收到一份GNU通用公共许可证的副本（本目录
  GPL.txt文件）。如果还没有，写信给：
    The Free Software Foundation, Inc.,  675  Mass Ave,  Cambridge,
    MA02139,  USA
  ----------------------------------------------------------------
  如果你在使用本软件时有什么问题或建议，请用以下地址与我们取得联系：

      http://isee.126.com
      http://cosoft.org.cn/projects/iseeexplorer

  或发信到：

      isee##vip.163.com
  ----------------------------------------------------------------
  本文件用途：  图象读写引擎－图像转换模块实现文件
  本文件编写人：
          YZ        yzfree##sina.com

  本文件版本：  20822
  最后修改于：  2002-8-22

  注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
  地址收集软件。
  ----------------------------------------------------------------
  修正历史：

    2002-8    修正位图转调色板图、及低位图转高位图时的BUG。
    2002-8    将转换引擎并入图象读写引擎，代码全部重写。
    2002-4    修正格式转换函数中颜色分量重叠的BUG，发现者是
          小组成员 tearshark。
    2001-12   小组成员 menghui 发现格式转换函数中缺少对多页
          图象的支持。（此BUG尚未修正）
    2001-7    发布第一个测试版

********************************************************************/


#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "iseemem.h"
#include "iseeirw.h"
#include "iseecnv.h"


/* 标准格式资料结构定义 */
typedef struct _tag_std_fmt {
  int       bits;   /* 每象素的位深度 */
  unsigned long bmask;    /* R、G、B、A 位掩码 */
  unsigned long gmask;
  unsigned long rmask;
  unsigned long amask;
} IRW_STD_FMT, *LPIRW_STD_FMT;


/* 标准格式个数 */
#define IRWSTD_FMT_NUM    7

/* 标准格式数据（注：下面的格式顺序不允许改动！） */
IRW_STD_FMT irw_stdfmt[IRWSTD_FMT_NUM] = {
  { 1, 0, 0, 0, 0 },                /* 1 位调色板图 */
  { 4, 0, 0, 0, 0 },                /* 4 位调色板图 */
  { 8, 0, 0, 0, 0 },                /* 8 位调色板图 */
  { 16, 0x1f, 0x3e0, 0x7c00, 0x0 },       /* 16 位(555)格式位图 */
  { 16, 0x1f, 0x7e0, 0xf800, 0x0 },       /* 16 位(565)格式位图 */
  { 24, 0xff, 0xff00, 0xff0000, 0x0 },      /* 24 位位图 */
  { 32, 0xff, 0xff00, 0xff0000, 0xff000000 },   /* 32 位位图(带Alpha掩码) */
};


/* 标准2色灰度调色板 */
PALITEM irw_std_pal2[2] = {
  {0x0, 0x0, 0x0, 0x0},
  {0xff, 0xff, 0xff, 0x0}
};

/* 标准16色灰度调色板 */
PALITEM irw_std_pal16[16] = {
  {0x0, 0x0, 0x0, 0x0},
  {0x11, 0x11, 0x11, 0x0},
  {0x22, 0x22, 0x22, 0x0},
  {0x33, 0x33, 0x33, 0x0},
  {0x44, 0x44, 0x44, 0x0},
  {0x55, 0x55, 0x55, 0x0},
  {0x66, 0x66, 0x66, 0x0},
  {0x77, 0x77, 0x77, 0x0},
  {0x88, 0x88, 0x88, 0x0},
  {0x99, 0x99, 0x99, 0x0},
  {0xaa, 0xaa, 0xaa, 0x0},
  {0xbb, 0xbb, 0xbb, 0x0},
  {0xcc, 0xcc, 0xcc, 0x0},
  {0xdd, 0xdd, 0xdd, 0x0},
  {0xee, 0xee, 0xee, 0x0},
  {0xff, 0xff, 0xff, 0x0}
};

/* 标准256色灰度调色板 */
PALITEM irw_std_pal256[256] = {
  {0x0, 0x0, 0x0, 0x0}, {0x1, 0x1, 0x1, 0x0}, {0x2, 0x2, 0x2, 0x0},
  {0x3, 0x3, 0x3, 0x0}, {0x4, 0x4, 0x4, 0x0}, {0x5, 0x5, 0x5, 0x0},
  {0x6, 0x6, 0x6, 0x0}, {0x7, 0x7, 0x7, 0x0}, {0x8, 0x8, 0x8, 0x0},
  {0x9, 0x9, 0x9, 0x0}, {0xa, 0xa, 0xa, 0x0}, {0xb, 0xb, 0xb, 0x0},
  {0xc, 0xc, 0xc, 0x0}, {0xd, 0xd, 0xd, 0x0}, {0xe, 0xe, 0xe, 0x0},
  {0xf, 0xf, 0xf, 0x0},     {0x10, 0x10, 0x10, 0x0},  {0x11, 0x11, 0x11, 0x0},
  {0x12, 0x12, 0x12, 0x0},  {0x13, 0x13, 0x13, 0x0},  {0x14, 0x14, 0x14, 0x0},
  {0x15, 0x15, 0x15, 0x0},  {0x16, 0x16, 0x16, 0x0},  {0x17, 0x17, 0x17, 0x0},
  {0x18, 0x18, 0x18, 0x0},  {0x19, 0x19, 0x19, 0x0},  {0x1a, 0x1a, 0x1a, 0x0},
  {0x1b, 0x1b, 0x1b, 0x0},  {0x1c, 0x1c, 0x1c, 0x0},  {0x1d, 0x1d, 0x1d, 0x0},
  {0x1e, 0x1e, 0x1e, 0x0},  {0x1f, 0x1f, 0x1f, 0x0},  {0x20, 0x20, 0x20, 0x0},
  {0x21, 0x21, 0x21, 0x0},  {0x22, 0x22, 0x22, 0x0},  {0x23, 0x23, 0x23, 0x0},
  {0x24, 0x24, 0x24, 0x0},  {0x25, 0x25, 0x25, 0x0},  {0x26, 0x26, 0x26, 0x0},
  {0x27, 0x27, 0x27, 0x0},  {0x28, 0x28, 0x28, 0x0},  {0x29, 0x29, 0x29, 0x0},
  {0x2a, 0x2a, 0x2a, 0x0},  {0x2b, 0x2b, 0x2b, 0x0},  {0x2c, 0x2c, 0x2c, 0x0},
  {0x2d, 0x2d, 0x2d, 0x0},  {0x2e, 0x2e, 0x2e, 0x0},  {0x2f, 0x2f, 0x2f, 0x0},
  {0x30, 0x30, 0x30, 0x0},  {0x31, 0x31, 0x31, 0x0},  {0x32, 0x32, 0x32, 0x0},
  {0x33, 0x33, 0x33, 0x0},  {0x34, 0x34, 0x34, 0x0},  {0x35, 0x35, 0x35, 0x0},
  {0x36, 0x36, 0x36, 0x0},  {0x37, 0x37, 0x37, 0x0},  {0x38, 0x38, 0x38, 0x0},
  {0x39, 0x39, 0x39, 0x0},  {0x3a, 0x3a, 0x3a, 0x0},  {0x3b, 0x3b, 0x3b, 0x0},
  {0x3c, 0x3c, 0x3c, 0x0},  {0x3d, 0x3d, 0x3d, 0x0},  {0x3e, 0x3e, 0x3e, 0x0},
  {0x3f, 0x3f, 0x3f, 0x0},  {0x40, 0x40, 0x40, 0x0},  {0x41, 0x41, 0x41, 0x0},
  {0x42, 0x42, 0x42, 0x0},  {0x43, 0x43, 0x43, 0x0},  {0x44, 0x44, 0x44, 0x0},
  {0x45, 0x45, 0x45, 0x0},  {0x46, 0x46, 0x46, 0x0},  {0x47, 0x47, 0x47, 0x0},
  {0x48, 0x48, 0x48, 0x0},  {0x49, 0x49, 0x49, 0x0},  {0x4a, 0x4a, 0x4a, 0x0},
  {0x4b, 0x4b, 0x4b, 0x0},  {0x4c, 0x4c, 0x4c, 0x0},  {0x4d, 0x4d, 0x4d, 0x0},
  {0x4e, 0x4e, 0x4e, 0x0},  {0x4f, 0x4f, 0x4f, 0x0},  {0x50, 0x50, 0x50, 0x0},
  {0x51, 0x51, 0x51, 0x0},  {0x52, 0x52, 0x52, 0x0},  {0x53, 0x53, 0x53, 0x0},
  {0x54, 0x54, 0x54, 0x0},  {0x55, 0x55, 0x55, 0x0},  {0x56, 0x56, 0x56, 0x0},
  {0x57, 0x57, 0x57, 0x0},  {0x58, 0x58, 0x58, 0x0},  {0x59, 0x59, 0x59, 0x0},
  {0x5a, 0x5a, 0x5a, 0x0},  {0x5b, 0x5b, 0x5b, 0x0},  {0x5c, 0x5c, 0x5c, 0x0},
  {0x5d, 0x5d, 0x5d, 0x0},  {0x5e, 0x5e, 0x5e, 0x0},  {0x5f, 0x5f, 0x5f, 0x0},
  {0x60, 0x60, 0x60, 0x0},  {0x61, 0x61, 0x61, 0x0},  {0x62, 0x62, 0x62, 0x0},
  {0x63, 0x63, 0x63, 0x0},  {0x64, 0x64, 0x64, 0x0},  {0x65, 0x65, 0x65, 0x0},
  {0x66, 0x66, 0x66, 0x0},  {0x67, 0x67, 0x67, 0x0},  {0x68, 0x68, 0x68, 0x0},
  {0x69, 0x69, 0x69, 0x0},  {0x6a, 0x6a, 0x6a, 0x0},  {0x6b, 0x6b, 0x6b, 0x0},
  {0x6c, 0x6c, 0x6c, 0x0},  {0x6d, 0x6d, 0x6d, 0x0},  {0x6e, 0x6e, 0x6e, 0x0},
  {0x6f, 0x6f, 0x6f, 0x0},  {0x70, 0x70, 0x70, 0x0},  {0x71, 0x71, 0x71, 0x0},
  {0x72, 0x72, 0x72, 0x0},  {0x73, 0x73, 0x73, 0x0},  {0x74, 0x74, 0x74, 0x0},
  {0x75, 0x75, 0x75, 0x0},  {0x76, 0x76, 0x76, 0x0},  {0x77, 0x77, 0x77, 0x0},
  {0x78, 0x78, 0x78, 0x0},  {0x79, 0x79, 0x79, 0x0},  {0x7a, 0x7a, 0x7a, 0x0},
  {0x7b, 0x7b, 0x7b, 0x0},  {0x7c, 0x7c, 0x7c, 0x0},  {0x7d, 0x7d, 0x7d, 0x0},
  {0x7e, 0x7e, 0x7e, 0x0},  {0x7f, 0x7f, 0x7f, 0x0},  {0x80, 0x80, 0x80, 0x0},
  {0x81, 0x81, 0x81, 0x0},  {0x82, 0x82, 0x82, 0x0},  {0x83, 0x83, 0x83, 0x0},
  {0x84, 0x84, 0x84, 0x0},  {0x85, 0x85, 0x85, 0x0},  {0x86, 0x86, 0x86, 0x0},
  {0x87, 0x87, 0x87, 0x0},  {0x88, 0x88, 0x88, 0x0},  {0x89, 0x89, 0x89, 0x0},
  {0x8a, 0x8a, 0x8a, 0x0},  {0x8b, 0x8b, 0x8b, 0x0},  {0x8c, 0x8c, 0x8c, 0x0},
  {0x8d, 0x8d, 0x8d, 0x0},  {0x8e, 0x8e, 0x8e, 0x0},  {0x8f, 0x8f, 0x8f, 0x0},
  {0x90, 0x90, 0x90, 0x0},  {0x91, 0x91, 0x91, 0x0},  {0x92, 0x92, 0x92, 0x0},
  {0x93, 0x93, 0x93, 0x0},  {0x94, 0x94, 0x94, 0x0},  {0x95, 0x95, 0x95, 0x0},
  {0x96, 0x96, 0x96, 0x0},  {0x97, 0x97, 0x97, 0x0},  {0x98, 0x98, 0x98, 0x0},
  {0x99, 0x99, 0x99, 0x0},  {0x9a, 0x9a, 0x9a, 0x0},  {0x9b, 0x9b, 0x9b, 0x0},
  {0x9c, 0x9c, 0x9c, 0x0},  {0x9d, 0x9d, 0x9d, 0x0},  {0x9e, 0x9e, 0x9e, 0x0},
  {0x9f, 0x9f, 0x9f, 0x0},  {0xa0, 0xa0, 0xa0, 0x0},  {0xa1, 0xa1, 0xa1, 0x0},
  {0xa2, 0xa2, 0xa2, 0x0},  {0xa3, 0xa3, 0xa3, 0x0},  {0xa4, 0xa4, 0xa4, 0x0},
  {0xa5, 0xa5, 0xa5, 0x0},  {0xa6, 0xa6, 0xa6, 0x0},  {0xa7, 0xa7, 0xa7, 0x0},
  {0xa8, 0xa8, 0xa8, 0x0},  {0xa9, 0xa9, 0xa9, 0x0},  {0xaa, 0xaa, 0xaa, 0x0},
  {0xab, 0xab, 0xab, 0x0},  {0xac, 0xac, 0xac, 0x0},  {0xad, 0xad, 0xad, 0x0},
  {0xae, 0xae, 0xae, 0x0},  {0xaf, 0xaf, 0xaf, 0x0},  {0xb0, 0xb0, 0xb0, 0x0},
  {0xb1, 0xb1, 0xb1, 0x0},  {0xb2, 0xb2, 0xb2, 0x0},  {0xb3, 0xb3, 0xb3, 0x0},
  {0xb4, 0xb4, 0xb4, 0x0},  {0xb5, 0xb5, 0xb5, 0x0},  {0xb6, 0xb6, 0xb6, 0x0},
  {0xb7, 0xb7, 0xb7, 0x0},  {0xb8, 0xb8, 0xb8, 0x0},  {0xb9, 0xb9, 0xb9, 0x0},
  {0xba, 0xba, 0xba, 0x0},  {0xbb, 0xbb, 0xbb, 0x0},  {0xbc, 0xbc, 0xbc, 0x0},
  {0xbd, 0xbd, 0xbd, 0x0},  {0xbe, 0xbe, 0xbe, 0x0},  {0xbf, 0xbf, 0xbf, 0x0},
  {0xc0, 0xc0, 0xc0, 0x0},  {0xc1, 0xc1, 0xc1, 0x0},  {0xc2, 0xc2, 0xc2, 0x0},
  {0xc3, 0xc3, 0xc3, 0x0},  {0xc4, 0xc4, 0xc4, 0x0},  {0xc5, 0xc5, 0xc5, 0x0},
  {0xc6, 0xc6, 0xc6, 0x0},  {0xc7, 0xc7, 0xc7, 0x0},  {0xc8, 0xc8, 0xc8, 0x0},
  {0xc9, 0xc9, 0xc9, 0x0},  {0xca, 0xca, 0xca, 0x0},  {0xcb, 0xcb, 0xcb, 0x0},
  {0xcc, 0xcc, 0xcc, 0x0},  {0xcd, 0xcd, 0xcd, 0x0},  {0xce, 0xce, 0xce, 0x0},
  {0xcf, 0xcf, 0xcf, 0x0},  {0xd0, 0xd0, 0xd0, 0x0},  {0xd1, 0xd1, 0xd1, 0x0},
  {0xd2, 0xd2, 0xd2, 0x0},  {0xd3, 0xd3, 0xd3, 0x0},  {0xd4, 0xd4, 0xd4, 0x0},
  {0xd5, 0xd5, 0xd5, 0x0},  {0xd6, 0xd6, 0xd6, 0x0},  {0xd7, 0xd7, 0xd7, 0x0},
  {0xd8, 0xd8, 0xd8, 0x0},  {0xd9, 0xd9, 0xd9, 0x0},  {0xda, 0xda, 0xda, 0x0},
  {0xdb, 0xdb, 0xdb, 0x0},  {0xdc, 0xdc, 0xdc, 0x0},  {0xdd, 0xdd, 0xdd, 0x0},
  {0xde, 0xde, 0xde, 0x0},  {0xdf, 0xdf, 0xdf, 0x0},  {0xe0, 0xe0, 0xe0, 0x0},
  {0xe1, 0xe1, 0xe1, 0x0},  {0xe2, 0xe2, 0xe2, 0x0},  {0xe3, 0xe3, 0xe3, 0x0},
  {0xe4, 0xe4, 0xe4, 0x0},  {0xe5, 0xe5, 0xe5, 0x0},  {0xe6, 0xe6, 0xe6, 0x0},
  {0xe7, 0xe7, 0xe7, 0x0},  {0xe8, 0xe8, 0xe8, 0x0},  {0xe9, 0xe9, 0xe9, 0x0},
  {0xea, 0xea, 0xea, 0x0},  {0xeb, 0xeb, 0xeb, 0x0},  {0xec, 0xec, 0xec, 0x0},
  {0xed, 0xed, 0xed, 0x0},  {0xee, 0xee, 0xee, 0x0},  {0xef, 0xef, 0xef, 0x0},
  {0xf0, 0xf0, 0xf0, 0x0},  {0xf1, 0xf1, 0xf1, 0x0},  {0xf2, 0xf2, 0xf2, 0x0},
  {0xf3, 0xf3, 0xf3, 0x0},  {0xf4, 0xf4, 0xf4, 0x0},  {0xf5, 0xf5, 0xf5, 0x0},
  {0xf6, 0xf6, 0xf6, 0x0},  {0xf7, 0xf7, 0xf7, 0x0},  {0xf8, 0xf8, 0xf8, 0x0},
  {0xf9, 0xf9, 0xf9, 0x0},  {0xfa, 0xfa, 0xfa, 0x0},  {0xfb, 0xfb, 0xfb, 0x0},
  {0xfc, 0xfc, 0xfc, 0x0},  {0xfd, 0xfd, 0xfd, 0x0},  {0xfe, 0xfe, 0xfe, 0x0},
  {0xff, 0xff, 0xff, 0x0}
};




/* 转换函数原型定义 */
#define DECCNVFUN(name)   name(int dstd, unsigned char *soubits, LPPALITEM soupal, int soupatch, unsigned char *desbits, LPPALITEM despal, int despatch, int linenum, int width)

/* 位深转换函数声明 */
static int DECCNVFUN(_1tox);
static int DECCNVFUN(_4tox);
static int DECCNVFUN(_8tox);

static int _paltox(int, int, unsigned char*, LPPALITEM, int, unsigned char*, LPPALITEM, \
    int, int, int, unsigned long, unsigned long, unsigned long, unsigned long);
static int _rgbtox(int, int, unsigned char*, LPPALITEM, int, unsigned char*, LPPALITEM, \
    int, int, int, unsigned long, unsigned long, unsigned long, unsigned long);

static unsigned long _get_next_pix(unsigned char*, int, int, int);
static int _get_mask_offset(unsigned long mask);



/* *********************************************************************************** */
/* 本模块中各函数的转换原则：

  位图－
    1、转为调色板图（先计算灰度值，然后确定它在调色板中的位置）
    2、同位深度转换（如果掩码相同则直拷，否则按掩码提出分量色后合成）
    3、转换为低位深度位图（保留高位数据，去掉低位数据）
    4、转为高位深度位图（向高位扩充）

  调色板图－
    1、转为低位调色板图（均值－平均抽取调色板项，拷至目标调色板）
    2、同位深度转换（直拷）
    3、转为高位深度调色板图（准直拷－象素及调色板数据均直拷）
    4、转为位图（扩展调色板数据－合成）
*/
/* *********************************************************************************** */



/* 判断指定的位深度及相关掩码是否是ISee标准格式之一 */
int iscnv_is_std(int bit,
    unsigned long rmask,
    unsigned long gmask,
    unsigned long bmask,
    unsigned long amask)
{
  int i;

  for (i = 0; i < IRWSTD_FMT_NUM; i++) {
    if ((irw_stdfmt[i].bits == bit) && (irw_stdfmt[i].rmask == rmask) && \
        (irw_stdfmt[i].gmask == gmask) && (irw_stdfmt[i].bmask == bmask) && \
        (irw_stdfmt[i].amask == amask)) {
      break;
    }
  }

  /* 返回-1，如果指定的图象不是标准格式。否则返回标准格式ID */
  return (i == IRWSTD_FMT_NUM) ? -1 : i;
}


/* 匹配最接近的一个标准格式，并返回它的ID */
int iscnv_match(int bit)
{
  int i, result = -1;

  for (i = 0; i < IRWSTD_FMT_NUM; i++) {
    if (irw_stdfmt[i].bits >= bit) {
      result = i;
      break;
    }
  }

  assert(result != -1);               /* 最大位深不能超过32位 */

  return result;
}


/* 获取指定ID标准格式的掩码数据 */
void iscnv_get_mask(int id,
    unsigned long* rmask,
    unsigned long* gmask,
    unsigned long* bmask,
    unsigned long* amask)
{
  assert(rmask && gmask && bmask && amask);
  assert(id < IRWSTD_FMT_NUM);

  *rmask = irw_stdfmt[id].rmask;
  *gmask = irw_stdfmt[id].gmask;
  *bmask = irw_stdfmt[id].bmask;
  *amask = irw_stdfmt[id].amask;
}


/* 标准格式ID转位深度值 */
int iscnv_idtobit(int id)
{
  assert((id >= 0) && (id < IRWSTD_FMT_NUM));

  return irw_stdfmt[id].bits;
}


/* 获取指定ID的相关格式信息 */
int iscnv_get_id_info(int id,
    unsigned long* bit,
    unsigned long* rmask,
    unsigned long* gmask,
    unsigned long* bmask,
    unsigned long* amask,
    int* pal_size,
    LPPALITEM* pppal)
{
  if ((id < 0) || (id >= IRWSTD_FMT_NUM)) {
    return -1;                    /* 非法的ID值 */
  }

  if (bit) {                    /* 填写位深度值 */
    *bit = irw_stdfmt[id].bits;
  }

  if (rmask) {                    /* 填写掩码数据 */
    *rmask = irw_stdfmt[id].rmask;
  }

  if (gmask) {
    *gmask = irw_stdfmt[id].gmask;
  }

  if (bmask) {
    *bmask = irw_stdfmt[id].bmask;
  }

  if (amask) {
    *amask = irw_stdfmt[id].amask;
  }

  if (pal_size) {                 /* 填写调色板尺寸 */
    switch (id) {
    case  0:
      *pal_size = 2;
      break;

    case  1:
      *pal_size = 16;
      break;

    case  2:
      *pal_size = 256;
      break;

    default:
      *pal_size = 0;
      break;
    }
  }

  if (pppal) {                    /* 填写标准调色板地址 */
    switch (id) {
    case  0:
      *pppal = (LPPALITEM)irw_std_pal2;
      break;

    case  1:
      *pppal = (LPPALITEM)irw_std_pal16;
      break;

    case  2:
      *pppal = (LPPALITEM)irw_std_pal256;
      break;

    default:
      *pppal = (LPPALITEM)0;
      break;
    }
  }

  return 0;                     /* 成功返回 */
}


/* 获取标准灰度调色板地址 */
LPPALITEM iscnv_get_pal(int bit)
{
  switch (bit) {
  case  1:
    return irw_std_pal2;
    break;

  case  4:
    return irw_std_pal16;
    break;

  case  8:
    return irw_std_pal256;
    break;

  default:
    return 0;
  }
}


/* 转换模块接口 */
int iscnv_cnvfun(int sbit,                /* 源图象位深度 */
    int dstd,                /* 目标格式标准ID号 */
    unsigned char* soubits,        /* 源图象象素数据地址 */
    LPPALITEM soupal,            /* 源图象调色板地址 */
    int soupatch,              /* 源图象扫描行尺寸 */
    unsigned char* desbits,        /* 目标图象象素数据地址 */
    LPPALITEM despal,            /* 目标图象调色板地址 */
    int despatch,              /* 目标图象扫描行尺寸 */
    int linenum,               /* 要转换的图象行数 */
    int width,               /* 一行图象的象素宽度 */
    unsigned long bmask,         /* 源图象掩码数据 */
    unsigned long gmask,
    unsigned long rmask,
    unsigned long amask)
{
  int result;

  assert((dstd >= 0) && (dstd < IRWSTD_FMT_NUM));

  if (sbit > 8) {                 /* 位图转换 */
    result = _rgbtox(sbit, dstd, soubits, soupal, soupatch, desbits, despal, \
        despatch, linenum, width, bmask, gmask, rmask, amask);
  }
  else {                      /* 调色板图转换 */
    switch (sbit) {
    case  1:
      result = _1tox(dstd, soubits, soupal, soupatch, desbits, despal, \
          despatch, linenum, width);
      break;

    case  4:
      result = _4tox(dstd, soubits, soupal, soupatch, desbits, despal, \
          despatch, linenum, width);
      break;

    case  8:
      result = _8tox(dstd, soubits, soupal, soupatch, desbits, despal, \
          despatch, linenum, width);
      break;

    default:
      result = _paltox(sbit, dstd, soubits, soupal, soupatch, desbits, despal, \
          despatch, linenum, width, bmask, gmask, rmask, amask);
      break;
    }
  }

  return result;
}


/* 1位调色板图转换函数 */
static int DECCNVFUN(_1tox)
{
  int       i, j, k;
  unsigned long lz, lo;
  unsigned short  sz, so;
  unsigned char c;

  switch (dstd) {
  case  0:  /* 1位调色板图 */
    while (linenum--) {
      /* 同位调色板图直拷 */
      memcpy((void*)desbits, (const void*)soubits, (size_t)((width + 7) / 8));

      /* 定位到下一行 */
      desbits += despatch;
      soubits += soupatch;
    }

    break;

  case  1:  /* 4位调色板图 */
    while (linenum--) {
      i = 0;
      j = -1;
      k = 0;

      while (i < width) {           /* 转换一行的象素数据 */
        if (j == -1) {
          c = soubits[i / 8];       /* 提取8个象素（共一字节）*/
          j = 7;
          k = 0;
        }

        if (k == 0) {             /* 高4位 */
          desbits[i / 2] = ((c >> j) & 0x1) << 4;
          k = 1;
        }
        else {                /* 低4位 */
          desbits[i / 2] |= ((c >> j) & 0x1);
          k = 0;
        }

        j--;
        i++;                  /* 象素位置加一 */
      }

      desbits += despatch;            /* 开始下一行 */
      soubits += soupatch;
    }

    break;

  case  2:  /* 8位调色板图 */
    while (linenum--) {
      i = 0;
      j = -1;

      while (i < width) {           /* 转换一行的象素数据 */
        if (j == -1) {
          c = soubits[i / 8];       /* 提取8个象素（共一字节）*/
          j = 7;
        }

        desbits[i++] = ((c >> j--) & 0x1);
      }

      desbits += despatch;            /* 开始下一行 */
      soubits += soupatch;
    }

    break;

  case  3:  /* 16位(555)位图 */
    /* 合成两项调色板数据，以加快操作速度 */
    sz = ((((unsigned short)soupal[0].blue) >> 3) & 0x1f) | \
        ((((unsigned short)soupal[0].green) << 2) & 0x3e0) | \
        ((((unsigned short)soupal[0].red) << 7) & 0x7c00);

    so = ((((unsigned short)soupal[1].blue) >> 3) & 0x1f) | \
        ((((unsigned short)soupal[1].green) << 2) & 0x3e0) | \
        ((((unsigned short)soupal[1].red) << 7) & 0x7c00);

    while (linenum--) {
      i = 0;
      j = -1;

      while (i < width) {           /* 转换一行的象素数据 */
        if (j == -1) {
          c = soubits[i / 8];       /* 提取8个象素（共一字节）*/
          j = 7;
        }

        ((unsigned short*)desbits)[i++] = ((c >> j--) & 0x1) ? so : sz;
      }

      desbits += despatch;            /* 开始下一行 */
      soubits += soupatch;
    }

    break;

  case  4:  /* 16位(565)位图 */
    sz = ((((unsigned short)soupal[0].blue) >> 3) & 0x1f) | \
        ((((unsigned short)soupal[0].green) << 3) & 0x7e0) | \
        ((((unsigned short)soupal[0].red) << 8) & 0xf800);

    so = ((((unsigned short)soupal[1].blue) >> 3) & 0x1f) | \
        ((((unsigned short)soupal[1].green) << 3) & 0x7e0) | \
        ((((unsigned short)soupal[1].red) << 8) & 0xf800);

    while (linenum--) {
      i = 0;
      j = -1;

      while (i < width) {           /* 转换一行的象素数据 */
        if (j == -1) {
          c = soubits[i / 8];       /* 提取8个象素（共一字节）*/
          j = 7;
        }

        ((unsigned short*)desbits)[i++] = ((c >> j--) & 0x1) ? so : sz;
      }

      desbits += despatch;            /* 开始下一行 */
      soubits += soupatch;
    }

    break;

  case  5:  /* 24位(888)位图 */
    while (linenum--) {
      i = 0;
      j = -1;

      while (i < width) {           /* 转换一行的象素数据 */
        if (j == -1) {
          c = soubits[i / 8];       /* 提取8个象素（共一字节）*/
          j = 7;
        }

        k = (int)(unsigned int)((c >> j) & 0x1); /* 提取索引值 */

        if ((i + 1) == width) {
          desbits[i * 3]   = soupal[k].blue;
          desbits[i * 3 + 1] = soupal[k].green;
          desbits[i * 3 + 2] = soupal[k].red;
        }
        else {
          *((unsigned long*)(((unsigned char*)desbits) + i * 3)) = \
              (((unsigned long*)soupal)[k]) & 0xffffff;
        }

        i++;
        j--;
      }

      desbits += despatch;            /* 开始下一行 */
      soubits += soupatch;
    }

    break;

  case  6:  /* 32位(888)位图 */
    lz = (((unsigned long*)soupal)[0]) & 0xffffff;
    lo = (((unsigned long*)soupal)[1]) & 0xffffff;

    while (linenum--) {
      i = 0;
      j = -1;

      while (i < width) {           /* 转换一行的象素数据 */
        if (j == -1) {
          c = soubits[i / 8];       /* 提取8个象素（共一字节）*/
          j = 7;
        }

        ((unsigned long*)desbits)[i++] = ((c >> j--) & 0x1) ? lo : lz;
      }

      desbits += despatch;            /* 开始下一行 */
      soubits += soupatch;
    }

    break;

  default:  /* 非法格式 */
    assert(0);
    return -1;
    break;
  }

  return 0;
}


/* 4位调色板图转换函数 */
static int DECCNVFUN(_4tox)
{
  int       i, j, k;
  unsigned long lz, lo;
  unsigned char c;

  switch (dstd) {
  case  0:  /* 1位调色板图 */
    while (linenum--) {
      i = 0;
      j = -1;
      k = -1;

      while (i < width) {           /* 转换一行的象素数据 */
        if (k == -1) {
          c = soubits[i / 2];       /* 提取2个象素（共一字节）*/
          k = 0;
        }

        if (j == -1) {
          j = 7;
          desbits[i / 8] = 0;
        }

        if (k == 0) {             /* 高4位 */
          desbits[i / 8] |= (((c >> 4) & 0xf) / 8) << j;
          k = 1;
        }
        else {                /* 低4位 */
          desbits[i / 8] |= ((c & 0xf) / 8) << j; /* 除8的含义：0～7变0，8～15变1 */
          k = -1;
        }

        j--;
        i++;                  /* 象素位置加一 */
      }

      desbits += despatch;            /* 开始下一行 */
      soubits += soupatch;
    }

    break;

  case  1:  /* 4位调色板图 */
    while (linenum--) {
      /* 同位调色板图直拷 */
      memcpy((void*)desbits, (const void*)soubits, (size_t)((width + 1) / 2));

      /* 定位到下一行 */
      desbits += despatch;
      soubits += soupatch;
    }

    break;

  case  2:  /* 8位调色板图 */
    while (linenum--) {
      i = 0;
      k = -1;

      while (i < width) {           /* 转换一行的象素数据 */
        if (k == -1) {
          c = soubits[i / 2];       /* 提取2个象素（共一字节）*/
          k = 0;
        }

        if (k == 0) {             /* 高4位 */
          desbits[i++] = ((c >> 4) & 0xf);
          k = 1;
        }
        else {                /* 低4位 */
          desbits[i++] = (c & 0xf);
          k = -1;
        }
      }

      desbits += despatch;            /* 开始下一行 */
      soubits += soupatch;
    }

    break;

  case  3:  /* 16位(555)位图 */
    while (linenum--) {
      i = 0;
      k = -1;

      while (i < width) {           /* 转换一行的象素数据 */
        if (k == -1) {
          c = soubits[i / 2];       /* 提取2个象素（共一字节）*/
          k = 0;
        }

        if (k == 0) {
          j = (int)(unsigned int)((c >> 4) & 0xf);
          k = 1;
        }
        else {
          j = (int)(unsigned int)(c & 0xf);
          k = -1;
        }

        ((unsigned short*)desbits)[i++] = \
            ((((unsigned short)soupal[j].blue) >> 3) & 0x1f) | \
            ((((unsigned short)soupal[j].green) << 2) & 0x3e0) | \
            ((((unsigned short)soupal[j].red) << 7) & 0x7c00);
      }

      desbits += despatch;            /* 开始下一行 */
      soubits += soupatch;
    }

    break;

  case  4:  /* 16位(565)位图 */
    while (linenum--) {
      i = 0;
      k = -1;

      while (i < width) {           /* 转换一行的象素数据 */
        if (k == -1) {
          c = soubits[i / 2];       /* 提取2个象素（共一字节）*/
          k = 0;
        }

        if (k == 0) {
          j = (int)(unsigned int)((c >> 4) & 0xf);
          k = 1;
        }
        else {
          j = (int)(unsigned int)(c & 0xf);
          k = -1;
        }

        ((unsigned short*)desbits)[i++] = \
            ((((unsigned short)soupal[j].blue) >> 3) & 0x1f) | \
            ((((unsigned short)soupal[j].green) << 3) & 0x7e0) | \
            ((((unsigned short)soupal[j].red) << 8) & 0xf800);
      }

      desbits += despatch;            /* 开始下一行 */
      soubits += soupatch;
    }

    break;

  case  5:  /* 24位位图 */
    while (linenum--) {
      i = 0;
      k = -1;

      while (i < width) {           /* 转换一行的象素数据 */
        if (k == -1) {
          c = soubits[i / 2];       /* 提取2个象素（共一字节）*/
          k = 0;
        }

        if (k == 0) {
          j = (int)(unsigned int)((c >> 4) & 0xf);
          k = 1;
        }
        else {
          j = (int)(unsigned int)(c & 0xf);
          k = -1;
        }

        if ((i + 1) == width) {       /* 最后一个象素 */
          desbits[i * 3]   = soupal[j].blue;
          desbits[i * 3 + 1] = soupal[j].green;
          desbits[i * 3 + 2] = soupal[j].red;
        }
        else {
          *((unsigned long*)(((unsigned char*)desbits) + i * 3)) = \
              (((unsigned long*)soupal)[j]) & 0xffffff;
        }

        i++;
      }

      desbits += despatch;            /* 开始下一行 */
      soubits += soupatch;
    }

    break;

  case  6:  /* 32位位图 */
    lz = (((unsigned long*)soupal)[0]) & 0xffffff;
    lo = (((unsigned long*)soupal)[1]) & 0xffffff;

    while (linenum--) {
      i = 0;
      k = -1;

      while (i < width) {           /* 转换一行的象素数据 */
        if (k == -1) {
          c = soubits[i / 2];       /* 提取2个象素（共一字节）*/
          k = 0;
        }

        if (k == 0) {
          j = (int)(unsigned int)((c >> 4) & 0xf);
          k = 1;
        }
        else {
          j = (int)(unsigned int)(c & 0xf);
          k = -1;
        }

        ((unsigned long*)desbits)[i++] = ((unsigned long*)soupal)[j] & 0xffffff;
      }

      desbits += despatch;            /* 开始下一行 */
      soubits += soupatch;
    }

    break;

  default:  /* 非法格式 */
    assert(0);
    return -1;
    break;
  }

  return 0;
}


/* 8位调色板图转换函数 */
static int DECCNVFUN(_8tox)
{
  int       i, j, k;
  unsigned short  sz;
  unsigned char c;

  switch (dstd) {
  case  0:  /* 1位调色板图 */
    while (linenum--) {
      i = 0;
      j = -1;

      while (i < width) {
        c = soubits[i];             /* 提出一个象素 */

        if (j == -1) {
          desbits[i / 8] = 0;       /* 初始化目标字节位0 */
          j = 7;
        }

        desbits[i / 8] |= (c / 128) << j; /* 0～127项变0项，128～255项变1项 */

        i++;
        j--;
      }

      soubits += soupatch;            /* 转入下一行处理 */
      desbits += despatch;
    }

    break;

  case  1:  /* 4位调色板图 */
    while (linenum--) {
      i = 0;
      k = -1;

      while (i < width) {
        c = soubits[i];             /* 提出一个象素 */

        if (k == -1) {
          desbits[i / 2] = 0;       /* 初始化目标象素 */
          k = 0;
        }

        if (k == 0) {             /* 高4位 */
          desbits[i / 2] |= ((c / 16) & 0xf) << 4; /* 转为16色调色板 */
          k = 1;
        }
        else {                /* 低4位 */
          desbits[i / 2] |= ((c / 16) & 0xf);
          k = -1;
        }

        i++;
      }

      soubits += soupatch;
      desbits += despatch;
    }

    break;

  case  2:  /* 8位调色板图 */
    while (linenum--) {
      /* 同位调色板图直拷 */
      memcpy((void*)desbits, (const void*)soubits, (size_t)width);

      /* 定位到下一行 */
      desbits += despatch;
      soubits += soupatch;
    }

    break;

  case  3:  /* 16位(555)位图 */
    while (linenum--) {
      i = 0;

      while (i < width) {
        c = soubits[i];             /* 提取一个象素 */

        sz = ((((unsigned short)soupal[(unsigned int)c].blue) >> 3) & 0x1f) | \
            ((((unsigned short)soupal[(unsigned int)c].green) << 2) & 0x3e0) | \
            ((((unsigned short)soupal[(unsigned int)c].red) << 7) & 0x7c00);

        ((unsigned short*)desbits)[i++] = sz;
      }

      soubits += soupatch;
      desbits += despatch;
    }

    break;

  case  4:  /* 16位(565)位图 */
    while (linenum--) {
      i = 0;

      while (i < width) {
        c = soubits[i];             /* 提取一个象素 */

        sz = ((((unsigned short)soupal[(unsigned int)c].blue) >> 3) & 0x1f) | \
            ((((unsigned short)soupal[(unsigned int)c].green) << 3) & 0x7e0) | \
            ((((unsigned short)soupal[(unsigned int)c].red) << 8) & 0xf800);

        ((unsigned short*)desbits)[i++] = sz;
      }

      soubits += soupatch;
      desbits += despatch;
    }

    break;

  case  5:  /* 24位位图 */
    while (linenum--) {
      i = 0;

      while (i < width) {
        c = soubits[i];

        if ((i + 1) == width) {       /* 最后一个象素 */
          desbits[i * 3 + 0] = soupal[(unsigned int)c].blue;
          desbits[i * 3 + 1] = soupal[(unsigned int)c].green;
          desbits[i * 3 + 2] = soupal[(unsigned int)c].red;
        }
        else {                /* 非最后一个象素，则用快速算法 */
          *(unsigned long*)(desbits + i * 3) = ((unsigned long*)soupal)[(unsigned int)c] & 0xffffff;
        }

        i++;
      }

      soubits += soupatch;            /* 转入下一行 */
      desbits += despatch;
    }

    break;

  case  6:  /* 32位位图 */
    while (linenum--) {
      i = 0;

      while (i < width) {
        c = soubits[i];

        ((unsigned long*)desbits)[i++] = (((unsigned long*)soupal)[(unsigned int)c]) & 0xffffff;
      }

      soubits += soupatch;
      desbits += despatch;
    }

    break;

  default:  /* 非法格式 */
    assert(0);
    return -1;
    break;
  }

  return 0;
}


/* 2、3、5、6、7位调色板图转换函数 */
static int _paltox(
    int sbit,
    int dstd,
    unsigned char* soubits,
    LPPALITEM soupal,
    int soupatch,
    unsigned char* desbits,
    LPPALITEM despal,
    int despatch,
    int linenum,
    int width,
    unsigned long bmask,
    unsigned long gmask,
    unsigned long rmask,
    unsigned long amask)
{
  unsigned long code, descode;
  int       i, j, k;
  int       bits;               /* 一行中象素的位数累计 */
  unsigned long sr, sb, sg, sa;           /* 源象素分量 */
  unsigned long dmr, dmg, dmb, dma;         /* 目标图象掩码 */
  int       dmor, dmog, dmob, dmoa;       /* 目标图象掩码位移 */


  switch (dstd) {
  case  0:  /* 1位调色板图 */
    while (linenum--) {
      j = -1;
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        if (j == -1) {
          desbits[i / 8] = 0;       /* 初始化目标字节 */
          j = 7;
        }

        /* 转为1位调色板索引 */
        descode = (code / (((unsigned long)pow((int)2, (int)sbit)) / 2)) & 0x1;

        desbits[i / 8] |= ((unsigned char)descode) << j;

        j--;
      }

      soubits += soupatch;            /* 开始下一行 */
      desbits += despatch;
    }

    break;

  case  1:  /* 4位调色板图 */
    while (linenum--) {
      k = -1;
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        if (k == -1) {
          desbits[i / 2] = 0;       /* 初始化目标字节 */
          k = 0;
        }

        if (sbit == 2) {            /* 2位 */
          if (k == 0) {           /* 高4位 */
            desbits[i / 2] |= ((unsigned char)code & 0x3) << 4;
            k = 1;
          }
          else {              /* 低4位 */
            desbits[i / 2] |= (unsigned char)code & 0x3;
            k = -1;
          }
        }
        else if (sbit == 3) {
          if (k == 0) {           /* 高4位 */
            desbits[i / 2] |= ((unsigned char)code & 0x7) << 4;
            k = 1;
          }
          else {              /* 低4位 */
            desbits[i / 2] |= (unsigned char)code & 0x7;
            k = -1;
          }
        }
        else {                /* 5、6、7位 */
          descode = (code / (((unsigned long)pow((int)2, (int)sbit)) / 16)) & 0xf;

          if (k == 0) {           /* 高4位 */
            desbits[i / 2] |= ((unsigned char)descode & 0xf) << 4;
            k = 1;
          }
          else {              /* 低4位 */
            desbits[i / 2] |= (unsigned char)descode & 0xf;
            k = -1;
          }
        }
      }

      soubits += soupatch;            /* 开始下一行 */
      desbits += despatch;
    }

    break;

  case  2:  /* 8位调色板图 */
    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        desbits[i] = (unsigned char)code & 0xff;
      }

      soubits += soupatch;            /* 开始下一行 */
      desbits += despatch;
    }

    break;

  case  3:  /* 16位(555)位图 */
    dmr = 0x7c00;                 /* 设为标准16位555格式掩码 */
    dmg = 0x3e0;
    dmb = 0x1f;
    dma = 0;

    dmor = _get_mask_offset(dmr);         /* 计算掩码距0位的偏移量 */
    dmog = _get_mask_offset(dmg);
    dmob = _get_mask_offset(dmb);
    dmoa = _get_mask_offset(dma);

    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* 从调色板项中分离出颜色分量，并缩小至555分量 */
        sr = ((unsigned long)((unsigned char)(soupal[(int)code].red))) >> 3;
        sg = ((unsigned long)((unsigned char)(soupal[(int)code].green))) >> 3;
        sb = ((unsigned long)((unsigned char)(soupal[(int)code].blue))) >> 3;
        sa = 0;

        sr <<= dmor;
        sg <<= dmog;
        sb <<= dmob;
        sa <<= dmoa;

        /* 合成象素 */
        descode = sr | sg | sb | sa;

        ((unsigned short*)desbits)[i] = (unsigned short)descode;
      }

      soubits += soupatch;            /* 开始下一行 */
      desbits += despatch;
    }

    break;

  case  4:  /* 16位(565)位图 */
    dmr = 0xf800;                 /* 设为标准16位565格式掩码 */
    dmg = 0x7e0;
    dmb = 0x1f;
    dma = 0;

    dmor = _get_mask_offset(dmr);         /* 计算掩码距0位的偏移量 */
    dmog = _get_mask_offset(dmg);
    dmob = _get_mask_offset(dmb);
    dmoa = _get_mask_offset(dma);

    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* 从调色板项中分离出颜色分量，并缩小至565分量 */
        sr = ((unsigned long)((unsigned char)(soupal[(int)code].red))) >> 3;
        sg = ((unsigned long)((unsigned char)(soupal[(int)code].green))) >> 2;
        sb = ((unsigned long)((unsigned char)(soupal[(int)code].blue))) >> 3;
        sa = 0;

        sr <<= dmor;
        sg <<= dmog;
        sb <<= dmob;
        sa <<= dmoa;

        /* 合成象素 */
        descode = sr | sg | sb | sa;

        ((unsigned short*)desbits)[i] = (unsigned short)descode;
      }

      soubits += soupatch;            /* 开始下一行 */
      desbits += despatch;
    }

    break;

  case  5:  /* 24位位图 */
    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* 从调色板项中分离出颜色分量 */
        sr = (unsigned long)((unsigned char)(soupal[(int)code].red));
        sg = (unsigned long)((unsigned char)(soupal[(int)code].green));
        sb = (unsigned long)((unsigned char)(soupal[(int)code].blue));
        sa = 0;

        /* 合成象素 */
        descode = (sa << 24) | (sr << 16) | (sg << 8) | sb;

        if ((i + 1) == width) {
          desbits[i * 3 + 0] = (unsigned char)sb;
          desbits[i * 3 + 1] = (unsigned char)sg;
          desbits[i * 3 + 2] = (unsigned char)sr;
        }
        else {
          *(unsigned long*)((unsigned char*)desbits + i * 3) = descode;
        }
      }

      soubits += soupatch;            /* 开始下一行 */
      desbits += despatch;
    }

    break;

  case  6:  /* 32位位图 */
    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        ((unsigned long*)desbits)[i] = ((unsigned long*)soupal)[(int)code] & 0xffffff;
      }

      soubits += soupatch;            /* 开始下一行 */
      desbits += despatch;
    }

    break;

  default:  /* 非法格式 */
    assert(0);
    return -1;
    break;
  }

  return 0;
}


/* 9~32位位图转换函数 */
static int _rgbtox(
    int sbit,
    int dstd,
    unsigned char* soubits,
    LPPALITEM soupal,
    int soupatch,
    unsigned char* desbits,
    LPPALITEM despal,
    int despatch,
    int linenum,
    int width,
    unsigned long bmask,
    unsigned long gmask,
    unsigned long rmask,
    unsigned long amask)
{
  unsigned long code, descode;
  int       i, j, k;
  int       bits;               /* 一行中象素的位数累计 */
  unsigned long rcou, offr, offg, offb, offa;
  unsigned long sr, sb, sg, sa;           /* 源象素分量 */
  unsigned long smr, smg, smb, sma;         /* 源图象掩码 */
  unsigned long dmr, dmg, dmb, dma;         /* 目标图象掩码 */
  int       smor, smog, smob, smoa;       /* 源图象掩码位移 */
  int       dmor, dmog, dmob, dmoa;       /* 目标图象掩码位移 */


  smr = rmask;                    /* 复制源掩码 */
  smg = gmask;
  smb = bmask;
  sma = amask;

  smor = _get_mask_offset(smr);           /* 计算掩码距0位的偏移量 */
  smog = _get_mask_offset(smg);
  smob = _get_mask_offset(smb);
  smoa = _get_mask_offset(sma);

  smr >>= smor;                   /* 将掩码对齐到0位 */
  smg >>= smog;
  smb >>= smob;
  sma >>= smoa;

  switch (dstd) {
  case  0:  /* 1位调色板图 */
    while (linenum--) {
      j = -1;
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        if (j == -1) {
          desbits[i / 8] = 0;       /* 初始化目标字节 */
          j = 7;
        }

        /* 分离出颜色分量 */
        sr = (code >> smor)&smr;        /* R */
        sg = (code >> smog)&smg;        /* G */
        sb = (code >> smob)&smb;        /* B */

        descode = (sr * 3 + sg * 6 + sb) / 10; /* 转换为灰度数据 */
        descode = (descode / 128) & 0x1;  /* 转为2色调色板项索引 */

        desbits[i / 8] |= ((unsigned char)descode) << j;

        j--;
      }

      soubits += soupatch;            /* 开始下一行 */
      desbits += despatch;
    }

    break;

  case  1:  /* 4位调色板图 */
    while (linenum--) {
      k = -1;
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        if (k == -1) {
          desbits[i / 2] = 0;       /* 初始化目标字节 */
          k = 0;
        }

        /* 分离出颜色分量 */
        sr = (code >> smor)&smr;        /* R */
        sg = (code >> smog)&smg;        /* G */
        sb = (code >> smob)&smb;        /* B */

        descode = (sr * 3 + sg * 6 + sb) / 10; /* 转换为灰度数据 */
        descode = (descode / 16) & 0xf;   /* 转为16色调色板项索引 */

        if (k == 0) {             /* 高4位 */
          desbits[i / 2] |= ((unsigned char)descode) << 4;
          k = 1;
        }
        else {                /* 低4位 */
          desbits[i / 2] |= ((unsigned char)descode) & 0xf;
          k = -1;
        }
      }

      soubits += soupatch;            /* 开始下一行 */
      desbits += despatch;
    }

    break;

  case  2:  /* 8位调色板图 */
    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* 分离出颜色分量 */
        sr = (code >> smor)&smr;        /* R */
        sg = (code >> smog)&smg;        /* G */
        sb = (code >> smob)&smb;        /* B */

        descode = ((sr * 3 + sg * 6 + sb) / 10) & 0xff; /* 转换为灰度数据 */

        desbits[i] = (unsigned char)descode;
      }

      soubits += soupatch;            /* 开始下一行 */
      desbits += despatch;
    }

    break;

  case  3:  /* 16位(555)位图 */
    dmr = 0x7c00;                 /* 设为标准16位555格式掩码 */
    dmg = 0x3e0;
    dmb = 0x1f;
    dma = 0;

    dmor = _get_mask_offset(dmr);         /* 计算掩码距0位的偏移量 */
    dmog = _get_mask_offset(dmg);
    dmob = _get_mask_offset(dmb);
    dmoa = _get_mask_offset(dma);

    dmr >>= dmor;                 /* 将掩码对齐到0位 */
    dmg >>= dmog;
    dmb >>= dmob;
    dma >>= dmoa;

    offr = offg = offb = offa = 0;

    if (smr > dmr) {
      rcou = smr;

      while (rcou > dmr) {
        offr++;
        rcou >>= 1;
      }
    }
    else if (smr < dmr) {
      rcou = smr;

      while (rcou < dmr) {
        offr++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smg > dmg) {
      rcou = smg;

      while (rcou > dmg) {
        offg++;
        rcou >>= 1;
      }
    }
    else if (smg < dmg) {
      rcou = smg;

      while (rcou < dmg) {
        offg++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smb > dmb) {
      rcou = smb;

      while (rcou > dmb) {
        offb++;
        rcou >>= 1;
      }
    }
    else if (smb < dmb) {
      rcou = smb;

      while (rcou < dmb) {
        offb++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (sma > dma) {
      rcou = sma;

      while (rcou > dma) {
        offa++;
        rcou >>= 1;
      }
    }
    else if (sma < dma) {
      rcou = sma;

      while (rcou < dma) {
        offa++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* 分离出颜色分量 */
        sr = (code >> smor)&smr;        /* R */
        sg = (code >> smog)&smg;        /* G */
        sb = (code >> smob)&smb;        /* B */
        sa = (code >> smoa)&sma;        /* A */

        /* 如果源分量范围大于目标分量范围，则保留分量高位，移出低位 */
        if (smr > dmr) {
          sr >>= offr;
        }
        else if (smr < dmr) {
          sr <<= offr;
        }

        if (smg > dmg) {
          sg >>= offg;
        }
        else if (smg < dmg) {
          sg <<= offg;
        }

        if (smb > dmb) {
          sb >>= offb;
        }
        else if (smb < dmb) {
          sb <<= offb;
        }

        if (sma > dma) {
          sa >>= offa;
        }
        else if (sma < dma) {
          sa <<= offa;
        }

        sr <<= dmor;
        sg <<= dmog;
        sb <<= dmob;
        sa <<= dmoa;

        /* 合成象素 */
        descode = sr | sg | sb | sa;

        ((unsigned short*)desbits)[i] = (unsigned short)descode;
      }

      soubits += soupatch;            /* 开始下一行 */
      desbits += despatch;
    }

    break;

  case  4:  /* 16位(565)位图 */
    dmr = 0xf800;                 /* 设为标准16位565格式掩码 */
    dmg = 0x7e0;
    dmb = 0x1f;
    dma = 0;

    dmor = _get_mask_offset(dmr);         /* 计算掩码距0位的偏移量 */
    dmog = _get_mask_offset(dmg);
    dmob = _get_mask_offset(dmb);
    dmoa = _get_mask_offset(dma);

    dmr >>= dmor;                 /* 将掩码对齐到0位 */
    dmg >>= dmog;
    dmb >>= dmob;
    dma >>= dmoa;

    offr = offg = offb = offa = 0;

    if (smr > dmr) {                /* 计算象素是需要扩展还是压缩 */
      rcou = smr;

      while (rcou > dmr) {
        offr++;
        rcou >>= 1;
      }
    }
    else if (smr < dmr) {
      rcou = smr;

      while (rcou < dmr) {
        offr++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smg > dmg) {
      rcou = smg;

      while (rcou > dmg) {
        offg++;
        rcou >>= 1;
      }
    }
    else if (smg < dmg) {
      rcou = smg;

      while (rcou < dmg) {
        offg++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smb > dmb) {
      rcou = smb;

      while (rcou > dmb) {
        offb++;
        rcou >>= 1;
      }
    }
    else if (smb < dmb) {
      rcou = smb;

      while (rcou < dmb) {
        offb++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (sma > dma) {
      rcou = sma;

      while (rcou > dma) {
        offa++;
        rcou >>= 1;
      }
    }
    else if (sma < dma) {
      rcou = sma;

      while (rcou < dma) {
        offa++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* 分离出颜色分量 */
        sr = (code >> smor)&smr;        /* R */
        sg = (code >> smog)&smg;        /* G */
        sb = (code >> smob)&smb;        /* B */
        sa = (code >> smoa)&sma;        /* A */

        /* 如果源分量范围大于目标分量范围，则保留分量高位，移出低位 */
        /* 否则则扩展象素以增加亮度 */
        if (smr > dmr) {
          sr >>= offr;
        }
        else if (smr < dmr) {
          sr <<= offr;
        }

        if (smg > dmg) {
          sg >>= offg;
        }
        else if (smg < dmg) {
          sg <<= offg;
        }

        if (smb > dmb) {
          sb >>= offb;
        }
        else if (smb < dmb) {
          sb <<= offb;
        }

        if (sma > dma) {
          sa >>= offa;
        }
        else if (sma < dma) {
          sa <<= offa;
        }

        sr <<= dmor;
        sg <<= dmog;
        sb <<= dmob;
        sa <<= dmoa;

        /* 合成象素 */
        descode = sr | sg | sb | sa;

        ((unsigned short*)desbits)[i] = (unsigned short)descode;
      }

      soubits += soupatch;            /* 开始下一行 */
      desbits += despatch;
    }

    break;

  case  5:  /* 24位位图 */
    dmr = 0xff0000;                 /* 设为标准24位格式掩码 */
    dmg = 0xff00;
    dmb = 0xff;
    dma = 0;

    dmor = _get_mask_offset(dmr);         /* 计算掩码距0位的偏移量 */
    dmog = _get_mask_offset(dmg);
    dmob = _get_mask_offset(dmb);
    dmoa = _get_mask_offset(dma);

    dmr >>= dmor;                 /* 将掩码对齐到0位 */
    dmg >>= dmog;
    dmb >>= dmob;
    dma >>= dmoa;

    offr = offg = offb = offa = 0;

    if (smr < dmr) {
      rcou = smr;

      while (rcou < dmr) {
        offr++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smg < dmg) {
      rcou = smg;

      while (rcou < dmg) {
        offg++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smb < dmb) {
      rcou = smb;

      while (rcou < dmb) {
        offb++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (sma < dma) {
      rcou = sma;

      while (rcou < dma) {
        offa++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* 分离出颜色分量 */
        sr = (code >> smor) & 0xff & smr; /* R */
        sg = (code >> smog) & 0xff & smg; /* G */
        sb = (code >> smob) & 0xff & smb; /* B */
        sa = (code >> smoa) & 0xff & sma; /* A */

        /* 如果源分量范围小于目标分量范围，则进行扩展，以提高图象亮度 */
        /* 因24位图象颜色分量是8位（最大分量位深度），所以不需再进行  */
        /* 缩小判断 */
        if (smr < dmr) {
          sr <<= offr;
        }

        if (smg < dmg) {
          sg <<= offg;
        }

        if (smb < dmb) {
          sb <<= offb;
        }

        if (sma < dma) {
          sa <<= offa;
        }

        /* 合成象素 */
        descode = (sa << 24) | (sr << 16) | (sg << 8) | sb;

        if ((i + 1) == width) {       /* 填入象素（快速算法）*/
          desbits[i * 3 + 0] = (unsigned char)sb;
          desbits[i * 3 + 1] = (unsigned char)sg;
          desbits[i * 3 + 2] = (unsigned char)sr;
        }
        else {
          *(unsigned long*)((unsigned char*)desbits + i * 3) = descode;
        }
      }

      soubits += soupatch;            /* 开始下一行 */
      desbits += despatch;
    }

    break;

  case  6:  /* 32位位图 */
    dmr = 0xff0000;                 /* 设为标准24位格式掩码 */
    dmg = 0xff00;
    dmb = 0xff;
    dma = 0xff000000;

    dmor = _get_mask_offset(dmr);         /* 计算掩码距0位的偏移量 */
    dmog = _get_mask_offset(dmg);
    dmob = _get_mask_offset(dmb);
    dmoa = _get_mask_offset(dma);

    dmr >>= dmor;                 /* 将掩码对齐到0位 */
    dmg >>= dmog;
    dmb >>= dmob;
    dma >>= dmoa;

    offr = offg = offb = offa = 0;

    if (smr < dmr) {
      rcou = smr;

      while (rcou < dmr) {
        offr++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smg < dmg) {
      rcou = smg;

      while (rcou < dmg) {
        offg++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (smb < dmb) {
      rcou = smb;

      while (rcou < dmb) {
        offb++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    if (sma < dma) {
      rcou = sma;

      while (rcou < dma) {
        offa++;
        rcou <<= 1;
        rcou |= 0x1;
      }
    }

    while (linenum--) {
      bits = 0;

      for (i = 0; i < width; i++) {
        code = _get_next_pix(soubits, bits / 8, bits % 8, sbit);
        bits += sbit;

        /* 分离出颜色分量 */
        sr = (code >> smor) & 0xff & smr; /* R */
        sg = (code >> smog) & 0xff & smg; /* G */
        sb = (code >> smob) & 0xff & smb; /* B */
        sa = (code >> smoa) & 0xff & sma; /* A */

        if (smr < dmr) {
          sr <<= offr;
        }

        if (smg < dmg) {
          sg <<= offg;
        }

        if (smb < dmb) {
          sb <<= offb;
        }

        if (sma < dma) {
          sa <<= offa;
        }

        /* 合成象素 */
        descode = (sa << 24) | (sr << 16) | (sg << 8) | sb;

        ((unsigned long*)desbits)[i] = descode;
      }

      soubits += soupatch;            /* 开始下一行 */
      desbits += despatch;
    }

    break;

  default:  /* 非法格式 */
    assert(0);
    return -1;
    break;
  }

  return 0;
}


/* 读取下一个象素数据 */
static unsigned long _get_next_pix(
    unsigned char* p, /* 象素扫描行的行首地址 */
    int offset_byte,  /* 偏移字节 */
    int offset_bit,   /* 偏移位数 */
    int bit_len)    /* 象素所占位数 */
{
  union {
    unsigned char b[8];
    unsigned long ul;
    __int64     up;
  } code;

  code.ul   = *(unsigned long*)(unsigned char*)(p + offset_byte);
  code.b[4] = *(unsigned char*)(p + offset_byte + 4);

  code.up >>= offset_bit;
  code.ul  &= (unsigned long)(((__int64)1 << bit_len) - 1);

  return code.ul;
}


/* 获取掩码位置（即掩码右移到0位时所需的移动次数） */
/* 注：这里的掩码必须是连位的，不能有跳接现象，否则此函数无意义！ */
static int _get_mask_offset(unsigned long mask)
{
  register result = 0;

  __asm {
    mov eax, mask;
    get_mask_offset_loop:
    shr eax, 1;
    jc  get_mask_offset_endloop;
    inc result;
    cmp result, 32;
    je  get_mask_offset_enderr;
    jmp get_mask_offset_loop;
    get_mask_offset_enderr:
    mov result, 0;
    get_mask_offset_endloop:
  }

  return (int)result;
}


