#ifndef _FMTGIF_INL_
#define _FMTGIF_INL_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include "_fmtimg.h"
//#include "imcolor.h"
#include "sort.h"

typedef WORD* LPWORD;
typedef DWORD* LPDWORD;

typedef struct gifheader {
  BYTE bySignature[ 3 ];
  BYTE byVersion[ 3 ];
}
GIFHEADER;

typedef struct gifscrdesc {
  WORD wWidth;
  WORD wDepth;
  struct globalflag {
    BYTE PalBits : 3;
    BYTE SortFlag : 1;
    BYTE ColorRes : 3;
    BYTE GlobalPal : 1;
  }
  GlobalFlag;
  BYTE byBackground;
  BYTE byAspect;
}
GIFSCRDESC;

typedef struct gifimage {
  WORD wLeft;
  WORD wTop;
  WORD wWidth;
  WORD wDepth;
  struct localflag {
    BYTE PalBits : 3;
    BYTE Reserved : 2;
    BYTE SortFlag : 1;
    BYTE Interlace : 1;
    BYTE LocalPal : 1;
  }
  LocalFlag;
}
GIFIMAGE;

typedef struct gifcontrol {
  BYTE byBlockSize;
  struct flag {
BYTE Transparency :
    1;
BYTE UserInput :
    1;
BYTE DisposalMethod :
    3;
BYTE Reserved :
    3;
  }
  Flag;
  WORD wDelayTime;
  BYTE byTransparencyIndex;
  BYTE byTerminator;
}
GIFCONTROL;

typedef struct gifplaintext {
  BYTE byBlockSize;
  WORD wTextGridLeft;
  WORD wTextGridTop;
  WORD wTextGridWidth;
  WORD wTextGridDepth;
  BYTE byCharCellWidth;
  BYTE byCharCellDepth;
  BYTE byForeColorIndex;
  BYTE byBackColorIndex;
}
GIFPLAINTEXT;

typedef struct gifapplication {
  BYTE byBlockSize;
  BYTE byIdentifier[ 8 ];
  BYTE byAuthentication[ 3 ];
}
GIFAPPLICATION;

typedef struct gifd_var {
  LPBYTE lpDataBuff;
  LPBYTE lpBgnBuff;
  LPBYTE lpEndBuff;
  DWORD dwDataLen;
  WORD wMemLen;
  WORD wWidth;
  WORD wDepth;
  WORD wLineBytes;
  WORD wBits;
  int bEOF;
  int bInterlace;
}
GIFD_VAR;

typedef GIFD_VAR* LPGIFD_VAR;

typedef struct gifc_var {
  LPBYTE lpDataBuff;
  LPBYTE lpEndBuff;
  DWORD dwTempCode;
  WORD wWidth;
  WORD wDepth;
  WORD wLineBytes;
  WORD wBits;
  WORD wByteCnt;
  WORD wBlockNdx;
  BYTE byLeftBits;
}
GIFC_VAR;
typedef GIFC_VAR* LPGIFC_VAR;

// 宏运算
#define DWORD_WBYTES(x)         ( (((x) + 31UL) >> 5) << 2 )
#define WORD_WBYTES(x)          ( (((x) + 15UL) >> 4) << 1 )
#define BYTE_WBYTES(x)          (  ((x) +  7UL) >> 3       )

//常量
#define MAX_BUFF_SIZE           32768 /* 32K */
#define MAX_HASH_SIZE            5051
#define MAX_TABLE_SIZE           4096 /* 12-bit */
#define MAX_SUBBLOCK_SIZE         255

////////////////////////////////////////////////////////////////////////////////

typedef struct fmtgif {
  int m_bpp;
  int m_offset;
  int m_binary;
  int m_maxval;
  GIFD_VAR GIFDVar; // GIF编码参数
  PaletteEntry m_palette[ 256 ];
}
fmtgif;

int color_clustor(int h, int w, const unsigned char* A, int al, int ai, unsigned char* B, int bl,
    int color_num, unsigned char* colormap, int ci)
{
  int i, j, k, n, m, *iA, *iA2, **iiA, siz;
  typedef int* intptr;
  m = n = 0;
  siz = h * w;
  iA = MALLOC(int, siz);
  iA2 = MALLOC(int, siz);
  iiA = MALLOC(int*, siz);
  memset(colormap, 255, color_num * ci);

  for (i = 0; i < h; ++i) {
    const unsigned char* A1 = A + i * al;
    int* iA1 = iA + i * w;
    int** iiA1 = iiA + i * w;

    for (j = 0; j < w; ++j) {
      const unsigned char* A2 = A1 + j * ai;
      unsigned char a0 = A2[ 0 ], a1 = A2[ 1 ], a2 = A2[ 2 ];
      m = 0;
#define mmm(N) m<<=1; m|=((a0>>(N))&1); m<<=1; m|=((a1>>(N))&1); m<<=1; m|=((a2>>(N))&1);

      mmm(7);
      mmm(6);
      mmm(5);
      mmm(4);
      mmm(3);
      mmm(2);
      mmm(1);
      mmm(0);
#undef mmm

      iA1[ j ] = m;
      iiA1[ j ] = iA1 + j;
    }
  }

#define LESSPTR(a, b)   (*(a) < *(b))
  QSORT(iiA, iiA + siz, LESSPTR, int*);
#undef LESSPTR

  for (j = 0, i = 0; i < siz;) {
    iA2[ j++ ] = *iiA[ i++ ];

    while (i < siz && (*iiA[ i ] == *iiA[ i - 1 ])) {
      ++i;
    }
  }

  color_num = MIN(color_num, j);

  for (i = 0; i < color_num; ++i) {
    unsigned char a0, a1, a2;
    iA2[ i ] = iA2[ i * j / color_num ];
    k = m = iA2[ i ];
    a0 = a1 = a2 = 0;
#define mmm(N) a2|=((m&1)<<(N)); m>>=1; a1|=((m&1)<<(N)); m>>=1; a0|=((m&1)<<(N)); m>>=1;

    mmm(0);
    mmm(1);
    mmm(2);
    mmm(3);
    mmm(4);
    mmm(5);
    mmm(6);
    mmm(7);
#undef mmm

    colormap[ i * 3 + 0 ] = a2;
    colormap[ i * 3 + 1 ] = a1;
    colormap[ i * 3 + 2 ] = a0;
  }

  for (j = 1, i = 0; i < siz; ++i) {
    if (*(iiA[ i ]) > iA2[ j ] && j < (color_num - 1)) {
      ++j;
    }

    *(iiA[ i ]) = ((*(iiA[ i ]) - iA2[ j - 1 ]) < (iA2[ j ] - * (iiA[ i ]))) ? (j - 1) : j;
  }

  for (i = 0; i < h; ++i) {
    unsigned char* B1 = B + i * bl;
    int* iA1 = iA + i * w;

    for (j = 0; j < w; ++j) {
      B1[ j ] = iA1[ j ];
    }
  }

#if 0
  {
    unsigned char* lpDIBBits = MALLOC(BYTE, siz * 3);
    PaletteEntry m_palette[ 256 ];
    int fmtbmp_write(ihand_t m_strm, const img_t * im, const PaletteEntry * palette);

    for (i = 0; i < 256; ++i) {
      m_palette[ i ].b = colormap[ i * 3 + 0 ];
      m_palette[ i ].g = colormap[ i * 3 + 1 ];
      m_palette[ i ].r = colormap[ i * 3 + 2 ];
      m_palette[ i ].a = 0;
    }

    for (i = 0; i < h; ++i) {
      unsigned char* data1 = lpDIBBits + i * w * 3;
      unsigned char* src = B + i * bl;
      FillColorRow8(data1, src, w, m_palette);
    }

    fmtbmp_write("testa.bmp", h, w, lpDIBBits, w * 3, 3);
    fmtbmp_write("testa2.bmp", h, w, A, al, ai);
    SAFEFREE(lpDIBBits);
    //FPRINT2D("a.txt", "%3d, ", h, w, B, bl, 1);
    //FPRINT2D("m.txt", "%3d, ", 256, 3, colormap, 3, 1);
  }
#endif
  SAFEFREE(iA);
  SAFEFREE(iA2);
  SAFEFREE(iiA);
  return TRUE;
}

/*************************************************************************
* 函数名称:
*   GIF_LZW_WriteCode()
 *
* 参数:
*   ihand_t m_strm   - 要保存的文件
*  WORD wCode    - 要添加的编码
*  LPBYTE lpSubBlock  - 子块
*  LPBYTE lpbyCurrentBits - 当前位数
*  LPGIFC_VAR lpGIFCVar - 指向GIFC_VAR结构的指针
 *
 * 返回值:
 *   无
 *
 * 说明:
 *   该函数用来输出一个编码。
 *
 *************************************************************************/
void GIF_LZW_WriteCode(ihand_t m_strm, WORD wCode, LPBYTE lpSubBlock,
    LPBYTE lpbyCurrentBits, LPGIFC_VAR lpGIFCVar)
{
  // 输出该编码
  lpGIFCVar->dwTempCode |= ((DWORD) wCode << lpGIFCVar->byLeftBits);
  lpGIFCVar->byLeftBits += (*lpbyCurrentBits);

  while (lpGIFCVar->byLeftBits >= 0x08) {
    lpSubBlock[ lpGIFCVar->wBlockNdx++ ] = (BYTE) lpGIFCVar->dwTempCode;

    // 判断是否超出MAX_SUBBLOCK_SIZE
    if (lpGIFCVar->wBlockNdx > MAX_SUBBLOCK_SIZE) {
      // 判断wByteCnt + 256是否超过MAX_BUFF_SIZE
      if ((lpGIFCVar->wByteCnt + 256) >= MAX_BUFF_SIZE) {
        // 输出
        WRITE_NBYTE(lpGIFCVar->lpDataBuff, lpGIFCVar->wByteCnt);
        lpGIFCVar->lpEndBuff = lpGIFCVar->lpDataBuff;
        lpGIFCVar->wByteCnt = 0;
      }

      lpSubBlock[ 0 ] = (BYTE)(lpGIFCVar->wBlockNdx - 1);
      memcpy(lpGIFCVar->lpEndBuff, lpSubBlock, lpGIFCVar->wBlockNdx);
      lpGIFCVar->lpEndBuff += lpGIFCVar->wBlockNdx;
      lpGIFCVar->wByteCnt += lpGIFCVar->wBlockNdx;
      lpGIFCVar->wBlockNdx = 1;
    }

    lpGIFCVar->dwTempCode >>= 8;
    lpGIFCVar->byLeftBits -= 0x08;
  }

  // 返回
  return ;
}

/*************************************************************************
 *
* 函数名称:
*   EncodeGIF_LZW()
 *
* 参数:
*   LPBYTE lpDIBBits  - 指向源DIB图像指针
*   ihand_t m_strm   - 要保存的文件
*   LPGIFC_VAR lpGIFCVar - 指向GIFC_VAR结构的指针
*  WORD wWidthBytes  - 每行图像字节数
*  int bInterlace  - 是否按照交错方式保存
 *
* 返回值:
*   无
 *
* 说明:
*   该函数对指定图像进行GIF_LZW编码。
 *
 *************************************************************************/
void EncodeGIF_LZW(LPBYTE lpDIBBits, ihand_t m_strm,
    LPGIFC_VAR lpGIFCVar, int wWidthBytes, int bInterlace)
{
  LPWORD lpwTableNdx; // 指向字串表指针

  // 用于字串表搜索的索引
  LPWORD lpwPrefix;
  LPBYTE lpbySuffix;

  LPBYTE lpImage; // 指向当前编码像素的指针
  DWORD dwDataNdx; // 计算当前数据图像的偏移量
  WORD wLZW_CLEAR; // LZW_CLEAR
  WORD wLZW_EOI; // LZW_EOI
  BYTE byLZW_MinCodeLen; // LZW_MinCodeLen

  // 字串表索引
  WORD wPreTableNdx;
  WORD wNowTableNdx;
  WORD wTopTableNdx;

  // 哈希表索引
  WORD wHashNdx;
  WORD wHashGap;
  WORD wPrefix;
  WORD wShiftBits;

  // 当前图像的行数
  WORD wRowNum;
  WORD wWidthCnt;

  // 循环变量
  WORD wi;
  WORD wj;

  // 交错方式存储时每次增加的行数
  WORD wIncTable[ 5 ] = { 8, 8, 4, 2, 0 };

  // 交错方式存储时起始行数
  WORD wBgnTable[ 5 ] = { 0, 4, 2, 1, 0 };

  int bStart;
  BYTE bySuffix;
  BYTE bySubBlock[ 256 ];
  BYTE byCurrentBits;
  BYTE byMask;
  BYTE byChar;
  BYTE byPass;

  // 临时字节变量
  BYTE byTemp;

  // 给字串表分配内存
  lpwTableNdx = MALLOC(WORD, MAX_HASH_SIZE);
  lpwPrefix = MALLOC(WORD, MAX_HASH_SIZE);
  lpbySuffix = MALLOC(BYTE, MAX_HASH_SIZE);

  // 计算LZW_MinCodeLen
  byLZW_MinCodeLen = (BYTE)((lpGIFCVar->wBits > 1) ? lpGIFCVar->wBits : 0x02);

  // 写GIF LZW最小代码大小
  WRITE_NBYTE((LPBYTE) & byLZW_MinCodeLen, 1);

  wRowNum = 0;
  bStart = TRUE;
  byPass = 0x00;

  // 计算LZW_CLEAR
  wLZW_CLEAR = 1 << byLZW_MinCodeLen;

  // 计算LZW_EOI
  wLZW_EOI = wLZW_CLEAR + 1;

  // 初始化字串表
  byCurrentBits = byLZW_MinCodeLen + (BYTE) 0x01;
  wNowTableNdx = wLZW_CLEAR + 2;
  wTopTableNdx = 1 << byCurrentBits;

  for (wi = 0; wi < MAX_HASH_SIZE; wi++) {
    // 初始化为0xFFFF
    *(lpwTableNdx + wi) = 0xFFFF;
  }

  // 输出LZW_CLEAR
  GIF_LZW_WriteCode(m_strm, wLZW_CLEAR, (LPBYTE) bySubBlock,
      &byCurrentBits, lpGIFCVar);

  // 逐行编码
  for (wi = 0; wi < lpGIFCVar->wDepth; wi++) {
    // 计算当前偏移量
    dwDataNdx = (DWORD)(wRowNum) * (DWORD) wWidthBytes;

    // 指向当前行图像的指针
    lpImage = (LPBYTE)(((LPBYTE) lpDIBBits) + dwDataNdx);

    wWidthCnt = 0;
    wShiftBits = 8 - lpGIFCVar->wBits;
    byMask = (BYTE)((lpGIFCVar->wBits == 1) ? 0x80 : 0xF0);

    if (bStart) {
      // 判断是否是256色位图（一个像素一字节）
      if (lpGIFCVar->wBits == 8) {
        // 256色，直接赋值即可
        byTemp = *lpImage++;
      }
      else {
        // 非256色，需要移位获取像素值
        wShiftBits = 8 - lpGIFCVar->wBits;
        byMask = (BYTE)((lpGIFCVar->wBits == 1) ? 0x80 : 0xF0);
        byTemp = (BYTE)((*lpImage & byMask) >> wShiftBits);
        byMask >>= lpGIFCVar->wBits;
        wShiftBits -= lpGIFCVar->wBits;
      }

      wPrefix = (WORD) byTemp;
      bStart = FALSE;
      wWidthCnt ++;
    }

    // 每行编码
    while (wWidthCnt < lpGIFCVar->wWidth) {
      // 判断是否是256色位图（一个像素一字节）
      if (lpGIFCVar->wBits == 8) {
        // 256色，直接赋值即可
        byTemp = *lpImage++;
      }
      else {
        // 非256色，需要移位获取像素值
        byChar = *lpImage;
        byTemp = (BYTE)((byChar & byMask) >> wShiftBits);

        if (wShiftBits) {
          byMask >>= lpGIFCVar->wBits;
          wShiftBits -= lpGIFCVar->wBits;
        }
        else {
          wShiftBits = 8 - lpGIFCVar->wBits;
          byMask = (BYTE)((lpGIFCVar->wBits == 1) ? 0x80 : 0xF0);
          lpImage ++;
        }
      }

      bySuffix = byTemp;
      wWidthCnt ++;

      // 查找当前字符串是否存在于字串表中
      wHashNdx = wPrefix ^ ((WORD) bySuffix << 4);
      wHashGap = (wHashNdx ? (MAX_HASH_SIZE - wHashNdx) : 1);

      // 判断当前字符串是否在字串表中
      while (TRUE) {
        // 当前字符串不在字串表中
        if (*(lpwTableNdx + wHashNdx) == 0xFFFF) {
          // 新字符串，退出循环
          break;
        }

        // 判断是否找到该字符串
        if ((*(lpwPrefix + wHashNdx) == wPrefix) &&
            (*(lpbySuffix + wHashNdx) == bySuffix)) {
          // 找到，退出循环
          break;
        }

        // 第二哈希表
        if (wHashNdx < wHashGap) {
          wHashNdx += MAX_HASH_SIZE;
        }

        wHashNdx -= wHashGap;
      }

      // 判断是否是新字符串
      if (*(lpwTableNdx + wHashNdx) != 0xFFFF) {
        // 不是新字符串
        wPrefix = *(lpwTableNdx + wHashNdx);
      }
      else {
        // 新字符串

        // 输出该编码
        GIF_LZW_WriteCode(m_strm, wPrefix, (LPBYTE) bySubBlock,
            &byCurrentBits, lpGIFCVar);

        // 将该新字符串添加到字串表中
        wPreTableNdx = wNowTableNdx;

        // 判断是否达到最大字串表大小
        if (wNowTableNdx < MAX_TABLE_SIZE) {
          *(lpwTableNdx + wHashNdx) = wNowTableNdx++;
          *(lpwPrefix + wHashNdx) = wPrefix;
          *(lpbySuffix + wHashNdx) = bySuffix;
        }

        if (wPreTableNdx == wTopTableNdx) {
          if (byCurrentBits < 12) {
            byCurrentBits ++;
            wTopTableNdx <<= 1;
          }
          else {
            // 字串表到达最大长度

            // 输出LZW_CLEAR
            GIF_LZW_WriteCode(m_strm, wLZW_CLEAR, (LPBYTE) bySubBlock,
                &byCurrentBits, lpGIFCVar);

            // 重新初始化字串表
            byCurrentBits = byLZW_MinCodeLen + (BYTE) 0x01;
            wLZW_CLEAR = 1 << byLZW_MinCodeLen;
            wLZW_EOI = wLZW_CLEAR + 1;
            wNowTableNdx = wLZW_CLEAR + 2;
            wTopTableNdx = 1 << byCurrentBits;

            for (wj = 0; wj < MAX_HASH_SIZE; wj++) {
              // 初始化为0xFFFF
              *(lpwTableNdx + wj) = 0xFFFF;
            }
          }
        }

        wPrefix = (WORD) bySuffix;
      }
    }

    // 判断是否是交错方式
    if (bInterlace) {
      // 交错方式，计算下一行位置
      wRowNum += wIncTable[ byPass ];

      if (wRowNum >= lpGIFCVar->wDepth) {
        byPass ++;
        wRowNum = wBgnTable[ byPass ];
      }
    }
    else {
      // 非交错方式，直接将行数加一即可
      wRowNum ++;
    }
  }

  // 输出当前编码
  GIF_LZW_WriteCode(m_strm, wPrefix, (LPBYTE) bySubBlock,
      &byCurrentBits, lpGIFCVar);

  // 输出LZW_EOI
  GIF_LZW_WriteCode(m_strm, wLZW_EOI, (LPBYTE) bySubBlock,
      &byCurrentBits, lpGIFCVar);

  if (lpGIFCVar->byLeftBits) {
    // 加入该字符
    bySubBlock[ lpGIFCVar->wBlockNdx++ ] = (BYTE) lpGIFCVar->dwTempCode;

    // 判断是否超出MAX_SUBBLOCK_SIZE
    if (lpGIFCVar->wBlockNdx > MAX_SUBBLOCK_SIZE) {
      // 判断wByteCnt + 256是否超过MAX_BUFF_SIZE
      if ((lpGIFCVar->wByteCnt + 256) >= MAX_BUFF_SIZE) {
        // 输出
        WRITE_NBYTE(lpGIFCVar->lpDataBuff,
            lpGIFCVar->wByteCnt);
        lpGIFCVar->lpEndBuff = lpGIFCVar->lpDataBuff;
        lpGIFCVar->wByteCnt = 0;
      }

      bySubBlock[ 0 ] = (BYTE)(lpGIFCVar->wBlockNdx - 1);
      memcpy(lpGIFCVar->lpEndBuff, (LPBYTE) bySubBlock, lpGIFCVar->wBlockNdx);
      lpGIFCVar->lpEndBuff += lpGIFCVar->wBlockNdx;
      lpGIFCVar->wByteCnt += lpGIFCVar->wBlockNdx;
      lpGIFCVar->wBlockNdx = 1;
    }

    lpGIFCVar->dwTempCode = 0UL;
    lpGIFCVar->byLeftBits = 0x00;
  }

  if (lpGIFCVar->wBlockNdx > 1) {
    // 判断wByteCnt + 256是否超过MAX_BUFF_SIZE
    if ((lpGIFCVar->wByteCnt + 256) >= MAX_BUFF_SIZE) {
      // 输出
      WRITE_NBYTE(lpGIFCVar->lpDataBuff,
          lpGIFCVar->wByteCnt);
      lpGIFCVar->lpEndBuff = lpGIFCVar->lpDataBuff;
      lpGIFCVar->wByteCnt = 0;
    }

    bySubBlock[ 0 ] = (BYTE)(lpGIFCVar->wBlockNdx - 1);
    memcpy(lpGIFCVar->lpEndBuff, (LPBYTE) bySubBlock, lpGIFCVar->wBlockNdx);
    lpGIFCVar->lpEndBuff += lpGIFCVar->wBlockNdx;
    lpGIFCVar->wByteCnt += lpGIFCVar->wBlockNdx;
    lpGIFCVar->wBlockNdx = 1;
  }

  // 释放内存
  SAFEFREE(lpwTableNdx);
  SAFEFREE(lpwPrefix);
  SAFEFREE(lpbySuffix);

  // 退出
  return ;
}

/*************************************************************************
 * 函数名称:
 *   fmtgif_write()
 *
 * 参数:
 *   LPBYTE lpDIB     - 指向DIB对象的指针
 *   ihand_t m_strm       - 要保存的文件
 *   int bInterlace  - 是否按照交错方式保存
 *   LPBYTE lpDIBBits - 指向象素的指针
 *   int wHeight      - 高度
 *   int wWidth       - 宽度
 *   int wWidthBytes  - 每行的字节数
 *
 * 返回值:
 *   int               - 成功返回TRUE，否则返回False。
 *
 * 说明:
 *   该函数将指定的图象（< 256色）保存为GIF文件。
 *
 *************************************************************************/
int fmtgif_write(ihand_t m_strm, const img_t* im, const PaletteEntry* palette)
{
  // 循环变量
  int i, j;
  int wHeight = im->h, wWidth = im->w, wWidthBytes = im->s, channels = im->c;
  const unsigned char* data = im->tt.data;

  int isBinary = FALSE;
  int result = FALSE;
  int bInterlace = 0;

  // 指向DIB象素指针
  LPBYTE lpDIBBits = (LPBYTE) data;
  LPBYTE lpSrcBuff;
  GIFHEADER GIFH;   // GIF文件头
  GIFSCRDESC GIFS; // GIF逻辑屏幕描述块
  GIFIMAGE GIFI; // GIF图像描述块
  GIFC_VAR GIFCVar; // GIF编码参数
  WORD wColors; // 颜色数目
  BYTE byGIF_Pal[ 768 ]; // 调色板
  BYTE byChar; // 字节变量
  int bWinStyleDIB = channels > 1; // 表明是否是Win3.0 DIB的标记

  // 给GIFCVar结构赋值
  GIFCVar.wWidth = wWidth;
  GIFCVar.wDepth = wHeight;
  GIFCVar.wBits = 8;
  GIFCVar.wLineBytes = (WORD) BYTE_WBYTES((DWORD) GIFCVar.wWidth *
      (DWORD) GIFCVar.wBits);

  // 计算颜色数目
  wColors = 1 << GIFCVar.wBits;

  // 给调色板赋值
  if (bWinStyleDIB) {
    lpDIBBits = MALLOC(BYTE, wHeight * wWidth);
    color_clustor(wHeight, wWidth, data, wWidthBytes, channels, lpDIBBits, wWidth, wColors, byGIF_Pal, 3);
    wWidthBytes = wWidth;
  }
  else {
    j = 0;

    for (i = 0; i < wColors; ++i, j += 3) {
      byGIF_Pal[ j + 0 ] = i; // 读取蓝色分量
      byGIF_Pal[ j + 1 ] = i; // 读取绿色分量
      byGIF_Pal[ j + 2 ] = i; // 读取红色分量
    }
  }

  // 开始写GIF文件
  GIFH.bySignature[ 0 ] = 'G';
  GIFH.bySignature[ 1 ] = 'I';
  GIFH.bySignature[ 2 ] = 'F';
  GIFH.byVersion[ 0 ] = '8';
  GIFH.byVersion[ 1 ] = '9';
  GIFH.byVersion[ 2 ] = 'a';
  WRITE_NBYTE((LPBYTE) & GIFH, 6);

  // 写GIF逻辑屏幕描述块
  GIFS.wWidth = GIFCVar.wWidth;
  GIFS.wDepth = GIFCVar.wDepth;
  GIFS.GlobalFlag.PalBits = (BYTE)(GIFCVar.wBits - 1);
  GIFS.GlobalFlag.SortFlag = 0x00;
  GIFS.GlobalFlag.ColorRes = (BYTE)(GIFCVar.wBits - 1);
  GIFS.GlobalFlag.GlobalPal = 0x01;
  GIFS.byBackground = 0x00;
  GIFS.byAspect = 0x00;
  WRITE_NBYTE((LPBYTE) & GIFS, 7);

  // 写GIF全局调色板
  WRITE_NBYTE((LPBYTE) byGIF_Pal, (wColors * 3));

  // 写GIF图像描述间隔符
  byChar = 0x2C;
  WRITE_NBYTE((LPBYTE) & byChar, 1);

  // 写GIF图像描述块
  GIFI.wLeft = 0;
  GIFI.wTop = 0;
  GIFI.wWidth = GIFCVar.wWidth;
  GIFI.wDepth = GIFCVar.wDepth;
  GIFI.LocalFlag.PalBits = 0x00;
  GIFI.LocalFlag.Reserved = 0x00;
  GIFI.LocalFlag.SortFlag = 0x00;
  GIFI.LocalFlag.Interlace = (BYTE)(bInterlace ? 0x01 : 0x00);
  GIFI.LocalFlag.LocalPal = 0x00;
  WRITE_NBYTE((LPBYTE) & GIFI, 9);

  // 写GIF图像压缩数据
  lpSrcBuff = MALLOC(BYTE, MAX_BUFF_SIZE);
  GIFCVar.lpDataBuff = lpSrcBuff;
  GIFCVar.lpEndBuff = GIFCVar.lpDataBuff;
  GIFCVar.dwTempCode = 0UL;
  GIFCVar.wByteCnt = 0;
  GIFCVar.wBlockNdx = 1;
  GIFCVar.byLeftBits = 0x00;

  // 进行GIF_LZW编码
  EncodeGIF_LZW(lpDIBBits, m_strm, &GIFCVar, wWidthBytes, bInterlace);

  // 判断是否编码成功
  if (GIFCVar.wByteCnt) {
    // 写入文件
    WRITE_NBYTE(GIFCVar.lpDataBuff, GIFCVar.wByteCnt);
  }

  // 释放内存
  SAFEFREE(lpSrcBuff);

  // 写GIF Block Terminator
  byChar = 0x00;
  WRITE_NBYTE((LPBYTE) & byChar, 1);

  // 写GIF文件结尾块
  byChar = 0x3B;
  WRITE_NBYTE((LPBYTE) & byChar, 1);

  if (data != lpDIBBits) {
    SAFEFREE(lpDIBBits);
  }

  // 返回
  return TRUE;
}

/*************************************************************************
* 函数名称:
*   ReadSrcData()
 *
* 参数:
*   ihand_t m_strm   - 源GIF文件
*   LPWORD lpwMemLen  - 缓冲区长度（指针）
*   LPDWORD lpdwDataLen - 剩余数据长度（指针）
*   LPBYTE lpSrcBuff  - 缓冲区指针
*  LPBOOL lpbEOF   - 结束标志
 *
* 返回值:
*   无
 *
* 说明:
*   该函数用来读取指定GIF文件中的图像编码，每次最多读取MAX_BUFF_SIZE
* 字节，是否读完由标志lpbEOF指定。
 *************************************************************************/
void ReadSrcData(ihand_t m_strm, LPWORD lpwMemLen, LPDWORD lpdwDataLen,
    LPBYTE lpSrcBuff, int* lpbEOF)
{
  // 判断数据长度是否仍然大于内存大小
  if ((*lpdwDataLen) > (DWORD)(*lpwMemLen)) {
    // 数据长度大于内存大小，表示没有解码完
    // 数据长度减内存大小
    (*lpdwDataLen) -= (DWORD)(*lpwMemLen);
  }
  else {
    // 数据长度不大于内存大小，表示解码将要完成内存大小就是剩余数据长度
    (*lpwMemLen) = (WORD)(*lpdwDataLen);
    // EOF标志设置为TRUE
    (*lpbEOF) = TRUE;
  }

  // 读取编码数据
  READ_NBYTE(lpSrcBuff, (*lpwMemLen));
  // 返回
  return ;
}

/*************************************************************************
* 函数名称:
*   DecodeGIF_LZW()
 *
* 参数:
*   ihand_t m_strm           - 源GIF文件
*   LPBYTE lpDIBBits     - 指向要保存的DIB图像指针
*   LPGIFD_VAR lpGIFDVar - 指向GIFC_VAR结构的指针
*   WORD wWidthBytes     - 每行图像字节数
 *
* 返回值:
*   无
 *
* 说明:
*   该函数对指定GIF_LZW编码数据进行解码。
 *************************************************************************/
void DecodeGIF_LZW(ihand_t m_strm, LPBYTE lpDIBBits, LPGIFD_VAR lpGIFDVar, int wWidthBytes)
{
  LPBYTE lpDst; // 指向编码后图像数据的指针
  // 用于字串表搜索的索引
  LPWORD lpwPrefix;
  LPBYTE lpbySuffix;
  LPBYTE lpbyStack;
  LPBYTE lpbyStackBgn;
  LPBYTE lpImageBgn;      // 指向图像当前行解码结果的指针
  LPBYTE lpImage;         // 指向当前编码像素的指针
  DWORD dwDataNdx;       // 计算当前数据图像的偏移量
  WORD wLZW_CLEAR;      // LZW_CLEAR
  WORD wLZW_EOI;        // LZW_EOI
  BYTE byLZW_MinCodeLen; // LZW_MinCodeLen

  // 字串表索引
  WORD wNowTableNdx;
  WORD wTopTableNdx;

  WORD wRowNum; // 当前图像的行数

  // 计数
  WORD wWidthCnt;
  WORD wBitCnt;
  WORD wRowCnt;
  WORD wi; // 循环变量
  // 交错方式存储时每次增加的行数
  WORD wIncTable[ 5 ] = { 8, 8, 4, 2, 0 };
  // 交错方式存储时起始行数
  WORD wBgnTable[ 5 ] = { 0, 4, 2, 1, 0 };
  BYTE byBlockSize; // 块大小
  BYTE byBlockNdx; // 块索引
  DWORD dwData;
  WORD wCode;   // 当前编码
  WORD wOldCode; // 上一个编码
  WORD wTempNdx; // 临时索引

  WORD wCodeMask[ 13 ] = {   //
    0x0000,
    0x0001, 0x0003, 0x0007, 0x000F,
    0x001F, 0x003F, 0x007F, 0x00FF,
    0x01FF, 0x03FF, 0x07FF, 0x0FFF
  };

  BYTE byLeftBits;
  BYTE byFirstChar;
  BYTE byCode;
  BYTE byCurrentBits;
  BYTE byPass;
  BYTE byTempChar; // 临时字节变量

  // 给字串表分配内存
  lpwPrefix = MALLOC(WORD, MAX_TABLE_SIZE);
  lpbySuffix = MALLOC(BYTE, MAX_TABLE_SIZE);
  lpbyStack = MALLOC(BYTE, MAX_TABLE_SIZE);
  lpImage = MALLOC(BYTE, wWidthBytes);

  lpbyStackBgn = lpbyStack;
  lpImageBgn = lpImage;
  byLZW_MinCodeLen = *lpGIFDVar->lpBgnBuff++; // 读取GIF LZW最小编码大小
  byCurrentBits = byLZW_MinCodeLen + (BYTE) 0x01;
  wLZW_CLEAR = 1 << byLZW_MinCodeLen; // 计算LZW_CLEAR
  wLZW_EOI = wLZW_CLEAR + 1; // 计算LZW_EOI

  // 计算字串表索引
  wNowTableNdx = wLZW_CLEAR + 2;
  wTopTableNdx = 1 << byCurrentBits;

  // 赋初值
  dwData = 0UL;
  wBitCnt = lpGIFDVar->wBits;
  wRowNum = 0;
  wRowCnt = 1;
  wWidthCnt = 0;
  wCode = 0;
  wOldCode = 0xFFFF;
  byBlockSize = 0x01;
  byBlockNdx = 0x00;
  byLeftBits = 0x00;
  byTempChar = 0x00;
  byPass = 0x00;

  // 读取下一个编码
  while (byLeftBits < byCurrentBits) {
    // 读取下一个字符
    // 判断是否读完一个数据块
    if (++byBlockNdx == byBlockSize) {
      // 读取下一个数据块
      byBlockSize = *lpGIFDVar->lpBgnBuff++;
      byBlockNdx = 0x00;

      // 判断是否读完
      if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
          !lpGIFDVar->bEOF) {
        // 读取下一个数据块
        ReadSrcData(m_strm, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen,
            lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);

        // 指针重新赋值
        lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
        lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
      }
    }

    // 下一个字符
    byCode = *lpGIFDVar->lpBgnBuff++;

    // 移位
    dwData |= ((DWORD) byCode << byLeftBits);
    byLeftBits += 0x08;

    // 判断是否读完
    if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
        !lpGIFDVar->bEOF) {
      // 读取下一个数据块
      ReadSrcData(m_strm, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen,
          lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);

      // 指针重新赋值
      lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
      lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
    }
  }

  wCode = (WORD) dwData & wCodeMask[ byCurrentBits ];
  dwData >>= byCurrentBits;
  byLeftBits -= byCurrentBits;

  // 解码
  while (wCode != wLZW_EOI) {
    // 当前编码不是LZW_EOI码
    // 判断是否是LZW_CLEAR码
    if (wCode == wLZW_CLEAR) {
      // 是LZW_CLEAR，清除字串表
      // 重新初始化字串表
      for (wi = 0; wi < wLZW_CLEAR; wi++) {
        *(lpwPrefix + wi) = 0xFFFF;
        *(lpbySuffix + wi) = (BYTE) wi;
      }

      for (wi = wNowTableNdx; wi < MAX_TABLE_SIZE; wi++) {
        *(lpwPrefix + wi) = 0xFFFF;
        *(lpbySuffix + wi) = 0x00;
      }

      byCurrentBits = byLZW_MinCodeLen + (BYTE) 0x01;
      wNowTableNdx = wLZW_CLEAR + 2;
      wTopTableNdx = 1 << byCurrentBits;
      wOldCode = 0xFFFF;

      // 获取下一个编码
      while (byLeftBits < byCurrentBits) {
        // 读取下一个字符
        // 判断是否读完一个数据块
        if (++byBlockNdx == byBlockSize) {
          // 读取下一个数据块
          byBlockSize = *lpGIFDVar->lpBgnBuff++;
          byBlockNdx = 0x00;

          // 判断是否读完
          if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) && !lpGIFDVar->bEOF) {
            // 读取下一个数据块
            ReadSrcData(m_strm, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen,
                lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);

            // 指针重新赋值
            lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
            lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
          }
        }

        byCode = *lpGIFDVar->lpBgnBuff++;
        dwData |= ((DWORD) byCode << byLeftBits);
        byLeftBits += 0x08;

        // 判断是否读完
        if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
            !lpGIFDVar->bEOF) {
          // 读取下一个数据块
          ReadSrcData(m_strm, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen,
              lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);

          // 指针重新赋值
          lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
          lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
        }
      }

      wCode = (WORD) dwData & wCodeMask[ byCurrentBits ];
      dwData >>= byCurrentBits;
      byLeftBits -= byCurrentBits;

      // 判断编码是否为LZW_EOI
      if (wCode != wLZW_EOI) {
        lpbyStack++; // 这里没有用到lpbyStack[0]

        // 将数据压入堆栈
        while ((*(lpwPrefix + wCode)) != 0xFFFF) {
          *lpbyStack++ = *(lpbySuffix + wCode);
          wCode = *(lpwPrefix + wCode);
        }

        *lpbyStack = *(lpbySuffix + wCode);
        byFirstChar = *lpbyStack;

        // 输出数据
        while (lpbyStack > lpbyStackBgn) {
          byTempChar |= (*lpbyStack-- << (8 - wBitCnt));

          if (wBitCnt == 8) {
            *lpImage++ = byTempChar;
            byTempChar = 0x00;
            wBitCnt = lpGIFDVar->wBits;
          }
          else {
            wBitCnt += lpGIFDVar->wBits;
          }

          wWidthCnt++;

          if (wWidthCnt == lpGIFDVar->wWidth) {
            if (wBitCnt != lpGIFDVar->wBits) {
              *lpImage = byTempChar;
              byTempChar = 0x00;
              wBitCnt = lpGIFDVar->wBits;
            }


            dwDataNdx = (wRowNum) * wWidthBytes;   // 图像当前行偏移量


            lpDst = (LPBYTE) lpDIBBits + dwDataNdx;   // 图像当前行起始位置

            // 赋值
            memcpy(lpDst, lpImageBgn, wWidthBytes);

            lpImage = lpImageBgn;

            // 判断是否按照交错方式保存
            if (lpGIFDVar->bInterlace) {
              // 交错方式
              // 计算下一行的行号
              wRowNum += wIncTable[ byPass ];

              if (wRowNum >= lpGIFDVar->wDepth) {
                byPass ++;
                wRowNum = wBgnTable[ byPass ];
              }
            }
            else {
              // 非交错方式，行号直接加1
              wRowNum ++;
            }

            wWidthCnt = 0;
          }
        }
      }
    }
    else {
      // 这里没有用到lpbyStack[0]
      lpbyStack++;

      // 判断字符串是否在字串表中
      if (wCode < wNowTableNdx) {
        // 不在字串表中
        wTempNdx = wCode;
      }
      else {
        // 在字串表中
        wTempNdx = wOldCode;
        *lpbyStack++ = byFirstChar;
      }

      // 将数据压入堆栈
      while ((*(lpwPrefix + wTempNdx)) != 0xFFFF) {
        *lpbyStack++ = *(lpbySuffix + wTempNdx);
        wTempNdx = *(lpwPrefix + wTempNdx);
      }

      *lpbyStack = *(lpbySuffix + wTempNdx);
      byFirstChar = *lpbyStack;

      // 将字符串添加到字串表中
      *(lpwPrefix + wNowTableNdx) = wOldCode;
      *(lpbySuffix + wNowTableNdx) = byFirstChar;

      if (++wNowTableNdx == wTopTableNdx && byCurrentBits < 12) {
        byCurrentBits ++;
        wTopTableNdx = 1 << byCurrentBits;
      }

      // 输出数据
      while (lpbyStack > lpbyStackBgn) {
        byTempChar |= (*lpbyStack-- << (8 - wBitCnt));

        if (wBitCnt == 8) {
          *lpImage++ = byTempChar;
          byTempChar = 0x00;
          wBitCnt = lpGIFDVar->wBits;
        }
        else {
          wBitCnt += lpGIFDVar->wBits;
        }

        wWidthCnt++;

        if (wWidthCnt == lpGIFDVar->wWidth) {
          if (wBitCnt != lpGIFDVar->wBits) {
            *lpImage = byTempChar;
            byTempChar = 0x00;
            wBitCnt = lpGIFDVar->wBits;
          }


          dwDataNdx = (wRowNum) * wWidthBytes;   // 图像当前行偏移量


          lpDst = (LPBYTE) lpDIBBits + dwDataNdx;   // 图像当前行起始位置


          memcpy(lpDst, lpImageBgn, wWidthBytes);   // 赋值

          lpImage = lpImageBgn;

          // 判断是否按照交错方式保存
          if (lpGIFDVar->bInterlace) {
            // 交错方式

            // 计算下一行的行号
            wRowNum += wIncTable[ byPass ];

            if (wRowNum >= lpGIFDVar->wDepth) {
              byPass ++;
              wRowNum = wBgnTable[ byPass ];
            }
          }
          else {
            // 非交错方式，行号直接加1
            wRowNum ++;
          }

          wWidthCnt = 0;
        }
      }
    }

    wOldCode = wCode;

    // 读取下一个编码
    while (byLeftBits < byCurrentBits) {
      // 读取下一个字符
      // 判断是否读完一个数据块
      if (++byBlockNdx == byBlockSize) {
        // 读取下一个数据块
        byBlockSize = *lpGIFDVar->lpBgnBuff++;
        byBlockNdx = 0x00;

        // 判断是否读完
        if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
            !lpGIFDVar->bEOF) {
          // 读取下一个数据块
          ReadSrcData(m_strm, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen,
              lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);

          // 指针重新赋值
          lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
          lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
        }
      }

      byCode = *lpGIFDVar->lpBgnBuff++;
      dwData |= ((DWORD) byCode << byLeftBits);
      byLeftBits += 0x08;

      // 判断是否读完
      if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
          !lpGIFDVar->bEOF) {
        // 读取下一个数据块
        ReadSrcData(m_strm, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen,
            lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);

        // 指针重新赋值
        lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
        lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
      }
    }

    wCode = (WORD) dwData & wCodeMask[ byCurrentBits ];
    dwData >>= byCurrentBits;
    byLeftBits -= byCurrentBits;
  }

  // 释放内存
  SAFEFREE(lpwPrefix);
  SAFEFREE(lpbySuffix);
  SAFEFREE(lpbyStack);

  // 返回
  return ;
}

/*************************************************************************
* 函数名称:
*   GIFRead()
 *
* 参数:
*   ihand_t m_strm        - 要读取的文件
 *
* 返回值:
*   HDIB               - 成功返回DIB的句柄，否则返回NULL。
 *
* 说明:
*   该函数将读取指定的GIF文件。将读取的结果保存在一个未压缩
* 编码的DIB对象中。
 *
 *************************************************************************/
int fmtgif_read_data(ihand_t m_strm, img_t* im, fmtgif* fmt)
{
  unsigned char* data = im->tt.data;
  int step = im->s, nch = im->c;
  int color = nch > 1;
  GIFD_VAR GIFDVar = fmt->GIFDVar;
  LPBYTE lpDIBBits = (LPBYTE) data;
  int wWidthBytes = step;
  PaletteEntry* m_palette = fmt->m_palette;
  unsigned char gray_palette[ 256 ];
  int i, m_bpp = fmt->m_bpp;
  int m_width = im->w;
  int m_height = im->h;

  // 解码
  // 获取编码数据长度
  GIFDVar.dwDataLen = (DWORD)(xfsize(m_strm) - GetPos());

  // 计算内存大小（最大不超过MAX_BUFF_SIZE）
  GIFDVar.wMemLen = ((GIFDVar.dwDataLen > (DWORD) MAX_BUFF_SIZE) ?
      (WORD) MAX_BUFF_SIZE : (WORD) GIFDVar.dwDataLen);

  // 分配内存
  GIFDVar.lpDataBuff = MALLOC(BYTE, GIFDVar.wMemLen);

  // 读取编码数据
  ReadSrcData(m_strm, &GIFDVar.wMemLen, &GIFDVar.dwDataLen,
      GIFDVar.lpDataBuff, &GIFDVar.bEOF);

  // 缓冲区起始位置
  GIFDVar.lpBgnBuff = GIFDVar.lpDataBuff;

  // 缓冲区中止位置
  GIFDVar.lpEndBuff = GIFDVar.lpBgnBuff + GIFDVar.wMemLen;

  if (color) {
    lpDIBBits = MALLOC(BYTE, m_width * m_height);
    wWidthBytes = m_width;
  }

  // 解码
  DecodeGIF_LZW(m_strm, lpDIBBits, &GIFDVar, wWidthBytes);

  if (!color) {
    if (m_bpp <= 8) {
      IMTRANS1D((1 << m_bpp), (unsigned char*) m_palette, 4, gray_palette, 1, BGRA2GRAY);
    }
  }

  for (i = 0; i < m_height; ++i) {
    unsigned char* data1 = data + i * step;
    unsigned char* src = lpDIBBits + i * wWidthBytes;

    if (color) {
      FillColorRow8(data1, src, m_width, m_palette);
    }
    else {
      FillGrayRow8(data1, src, m_width, gray_palette);
    }
  }

  // 释放内存
  SAFEFREE(GIFDVar.lpDataBuff);

  if (lpDIBBits != data) {
    SAFEFREE(lpDIBBits);
  }

  // 返回DIB句柄
  return TRUE;
}

int fmtgif_checksign(ihand_t m_strm)
{
  int re = 0;
  int m_sign_len = 6;
  char psign[10];

  if (psign) {
    re = getsign(m_strm, psign, m_sign_len);

    if (re) {
      // 判断是否是GIF文件
      if (memicmp(psign, "GIF", 3) != 0) {
        // 非GIF文件，返回NULL
        re = FALSE;
      }

      // 判断版本号是否正确
      if ((memicmp(psign + 3, "87a", 3) != 0) &&
          (memicmp(psign + 3, "89a", 3) != 0)) {
        // 不支持该版本，返回NULL
        re = FALSE;
      }
    }
  }

  return re;
}

// PNG Filter Factory
void fmtgif_init(fmtgif* fmt)
{
  //reader->m_sign_len = SIGN_LEN;
  //reader->m_signature = "";
  //reader->m_description = _T( "Portable image format (*.gif)" );
  fmt->m_offset = -1;
  fmt->m_bpp = 0;
  fmt->m_binary = fmt->m_maxval = 0;
}

int fmtgif_read_header(ihand_t m_strm, img_t* im, fmtgif* fmt)
{
  int result = FALSE;
  GIFHEADER GIFH; // GIF文件头
  GIFSCRDESC GIFS; // GIF逻辑屏幕描述块
  GIFIMAGE GIFI; // GIF图像描述块
  GIFCONTROL GIFC; // GIF图像控制扩充块
  GIFPLAINTEXT GIFP; // GIF图像说明扩充块
  GIFAPPLICATION GIFA; // GIF应用程序扩充块
  GIFD_VAR GIFDVar; // GIF编码参数
  WORD wColors; // 颜色数目
  BYTE byGIF_Pal[ 768 ]; // 调色板

  // 16色系统调色板
  BYTE bySysPal16[ 48 ] = {   //
    0, 0, 0, 0, 0, 128,
    0, 128, 0, 0, 128, 128,
    128, 0, 0, 128, 0, 128,
    128, 128, 0, 128, 128, 128,
    192, 192, 192, 0, 0, 255,
    0, 255, 0, 0, 255, 255,
    255, 0, 0, 255, 0, 255,
    255, 255, 0, 255, 255, 255
  };

  WORD wPalSize; // 调色板大小(字节数)
  BYTE byTemp; // 字节变量
  WORD wTemp; // 字变量
  WORD wi; // 循环变量
  BYTE byLabel; // 标签
  BYTE byBlockSize; // 块大小
  LPBYTE lpTemp; // 内存指针

  if (!m_strm) {
    return FALSE;
  }

  if (!fmtgif_checksign(m_strm)) {
    return 0;
  }

  fmtgif_init(fmt);

  //if( setjmp( m_strm.JmpBuf()) == 0 )

  SetPos(0);
  // 读取GIF文件头
  READ_NBYTE((LPBYTE) & GIFH, sizeof(GIFH));

  // 读取GIF逻辑屏幕描述块
  READ_NBYTE((LPBYTE) & GIFS, 7);

  // 获取调色板的位数
  GIFDVar.wBits = (WORD) GIFS.GlobalFlag.PalBits + 1;

  // 判断是否有全局调色板
  if (GIFS.GlobalFlag.GlobalPal) {
    // 赋初值
    memset((LPBYTE) byGIF_Pal, 0, 768);
    // 全局调色板大小
    wPalSize = 3 * (1 << GIFDVar.wBits);
    // 读取全局调色板
    READ_NBYTE((LPBYTE) byGIF_Pal, wPalSize);
  }

  // 读取下一个字节
  READ_NBYTE((LPBYTE) & byTemp, 1);

  // 对每一个描述块循环
  while (TRUE) {
    // 判断是否是图像描述块
    if (byTemp == 0x2C) {
      // 是图像描述块，退出循环
      break;
    }

    // 判断是否是GIF扩展块
    if (byTemp == 0x21) {
      // 是GIF扩展块
      // 分配内存
      lpTemp = MALLOC(BYTE, (DWORD) MAX_BUFF_SIZE);
      // 读取下一个字节
      READ_NBYTE((LPBYTE) & byLabel, 1);

      // 针对各种扩充块，进行分别处理
      switch (byLabel) {
      case 0xF9: {
        // 图像控制扩充块
        READ_NBYTE((LPBYTE) & GIFC, 6);
        // 跳出
        break;
      }

      case 0x01: {
        // 图像说明扩充块
        READ_NBYTE((LPBYTE) & GIFP, sizeof(GIFP));
        // 读取扩充块大小
        READ_NBYTE((LPBYTE) & byBlockSize, 1);

        // 当byBlockSize > 0时循环读取
        while (byBlockSize) {
          // 读取图像说明扩充块（这里没有进行任何处理）
          READ_NBYTE(lpTemp, byBlockSize);
          // 读取扩充块大小
          READ_NBYTE((LPBYTE) & byBlockSize, 1);
        }

        // 跳出
        break;
      }

      case 0xFE: {
        // 注释说明扩充块
        // 读取扩充块大小
        READ_NBYTE((LPBYTE) & byBlockSize, 1);

        // 当byBlockSize > 0时循环读取
        while (byBlockSize) {
          // 读取注释说明扩充块（这里没有进行任何处理）
          READ_NBYTE(lpTemp, byBlockSize);
          // 读取扩充块大小
          READ_NBYTE((LPBYTE) & byBlockSize, 1);
        }

        // 跳出
        break;
      }

      case 0xFF: {
        // 应用程序扩充块
        READ_NBYTE((LPBYTE) & GIFA, sizeof(GIFA));
        // 读取扩充块大小
        READ_NBYTE((LPBYTE) & byBlockSize, 1);

        // 当byBlockSize > 0时循环读取
        while (byBlockSize) {
          // 读取应用程序扩充块（这里没有进行任何处理）
          READ_NBYTE(lpTemp, byBlockSize);

          // 读取扩充块大小
          READ_NBYTE((LPBYTE) & byBlockSize, 1);
        }

        // 跳出
        break;
      }

      default: {
        // 忽略未知的扩充块

        // 读取扩充块大小
        READ_NBYTE((LPBYTE) & byBlockSize, 1);

        // 当byBlockSize > 0时循环读取
        while (byBlockSize) {
          // 读取未知的扩充块（这里没有进行任何处理）
          READ_NBYTE(lpTemp, byBlockSize);

          // 读取扩充块大小
          READ_NBYTE((LPBYTE) & byBlockSize, 1);
        }

        // 跳出
        break;
      }

      // 释放内存
      SAFEFREE(lpTemp);
      }
    }

    // 读取下一个字节
    READ_NBYTE((LPBYTE) & byTemp, 1);
  }

  // 读取GIF图像描述块
  READ_NBYTE((LPBYTE) & GIFI, 9);
  GIFDVar.wWidth = GIFI.wWidth; // 获取图像宽度
  GIFDVar.wDepth = GIFI.wDepth; // 获取图像高度

  // 判断是否有区域调色板
  if (GIFI.LocalFlag.LocalPal) {
    // 赋初值
    memset((LPBYTE) byGIF_Pal, 0, 768);

    // 读取区域调色板位数
    GIFDVar.wBits = (WORD) GIFI.LocalFlag.PalBits + 1;

    // 区域调色板大小
    wPalSize = 3 * (1 << GIFDVar.wBits);

    // 读取区域调色板
    READ_NBYTE((LPBYTE) byGIF_Pal, wPalSize);
  }

  // 给GIFDVar成员赋值
  GIFDVar.wBits = ((GIFDVar.wBits == 1) ? 1 : (GIFDVar.wBits <= 4) ? 4 : 8);

  GIFDVar.wLineBytes = (WORD) BYTE_WBYTES((DWORD) GIFDVar.wWidth *
      (DWORD) GIFDVar.wBits);
  GIFDVar.bEOF = FALSE;

  // 交错方式
  GIFDVar.bInterlace = (GIFI.LocalFlag.Interlace ? TRUE : FALSE);

  // 颜色数目
  wColors = 1 << GIFDVar.wBits;

  // 调色板大小
  wPalSize = wColors * sizeof(PaletteEntry);

  ////////////////////////////////////////////////////////////////////////////////
  // 设置BITMAPINFOHEADER

#if 0
  // 给lpBIH成员赋值
  lpBIH->biSize = (DWORD) sizeof(BITMAPINFOHEADER);
  lpBIH->biWidth = (DWORD) GIFDVar.wWidth;
  lpBIH->biHeight = (DWORD) GIFDVar.wDepth;
  lpBIH->biPlanes = 1;
  lpBIH->biBitCount = GIFDVar.wBits;
  lpBIH->biCompression = BI_RGB;
  lpBIH->biSizeImage = (DWORD) wWidthBytes * GIFDVar.wDepth;
  lpBIH->biXPelsPerMeter = 0;
  lpBIH->biYPelsPerMeter = 0;
  lpBIH->biClrUsed = wColors;
  lpBIH->biClrImportant = 0;
#endif

  im->w = GIFDVar.wWidth;
  im->h = GIFDVar.wDepth;
  im->c = 3;
  fmt->m_bpp = (wColors >> 8) * 8;

  ////////////////////////////////////////////////////////////////////////////////
  // 设置调色板

  // 判断是否指定全局或区域调色板
  if (GIFS.GlobalFlag.GlobalPal || GIFI.LocalFlag.LocalPal) {
    wTemp = 0;

    for (wi = 0; wi < wColors; wi++) {
      fmt->m_palette[ wi ].r = byGIF_Pal[ wTemp++ ];
      fmt->m_palette[ wi ].g = byGIF_Pal[ wTemp++ ];
      fmt->m_palette[ wi ].b = byGIF_Pal[ wTemp++ ];
      fmt->m_palette[ wi ].a = 0x00;
    }
  }
  else {
    // 没有指定全局和区域调色板，采用系统调色板

    // 按照颜色数目来分别给调色板赋值
    switch (wColors) {
    case 2: {
      // 单色位图
      fmt->m_palette[ 0 ].r = 0x00;
      fmt->m_palette[ 0 ].g = 0x00;
      fmt->m_palette[ 0 ].b = 0x00;
      fmt->m_palette[ 0 ].a = 0x00;
      fmt->m_palette[ 1 ].r = 0xFF;
      fmt->m_palette[ 1 ].g = 0xFF;
      fmt->m_palette[ 1 ].b = 0xFF;
      fmt->m_palette[ 1 ].a = 0x00;

      // 跳出
      break;
    }

    case 16: {
      // 16色位图

      wTemp = 0;

      for (wi = 0; wi < wColors; wi++) {
        fmt->m_palette[ wi ].r = bySysPal16[ wTemp++ ];
        fmt->m_palette[ wi ].g = bySysPal16[ wTemp++ ];
        fmt->m_palette[ wi ].b = bySysPal16[ wTemp++ ];
        fmt->m_palette[ wi ].a = 0x00;
      }

      // 跳出
      break;
    }

    case 256: {
      // 256色位图
      for (wi = 0; wi < wColors; wi++) {
        fmt->m_palette[ wi ].r = (BYTE) wi;
        fmt->m_palette[ wi ].g = (BYTE) wi;
        fmt->m_palette[ wi ].b = (BYTE) wi;
        fmt->m_palette[ wi ].a = 0x00;
      }

      // 跳出
      break;
    }
    }
  }

  result = TRUE;
  fmt->GIFDVar = GIFDVar;


  if (!result) {
    fmt->m_offset = -1;
    im->w = im->h = -1;
  }

  return result;
}

#define fmtgif_free(x)  (0)

#endif // _FMTGIF_INL_
