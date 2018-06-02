
#ifndef SUPPORT_IMA
#define SUPPORT_IMA


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include "_fmtimg.h"

//typedef unsigned char BYTE;
//typedef unsigned short WORD;

typedef struct {
  ihand_t m_strm;
  BYTE bBpp;
  WORD wLeft;
  WORD wTop;
  WORD wRight;
  WORD wBottom;
  WORD wXResolution;
  WORD wYResolution;
}
IMGHEADER;

typedef struct fmtima {
  PaletteEntry m_palette[ 256 ];
}
fmtima;

int fmtima_free(fmtima* reader)
{
  return 0;
}

#define fmtima_checkext(x)  0
int fmtima_checksign(const TCHAR* picname)
{
  return fmtima_checkext(picname);
}

// PNG Filter Factory
int fmtima_read_header(ihand_t m_strm, img_t* im, fmtbmp* fmt)
{
  int result = FALSE;
  IMGHEADER Header4IMG;  // IMG头文件
  im->c = 1;

  // 读出IMG文件头信息
  // 尝试读取文件头
  if (READ_NBYTE((LPBYTE) &Header4IMG, sizeof(IMGHEADER)) != sizeof(IMGHEADER)) {
    // 大小不对，返回NULL。
    return FALSE;
  }

  // 获取图像高度
  im->h = Header4IMG.wBottom - Header4IMG.wTop + 1;

  // 获取图像宽度
  im->w = Header4IMG.wRight - Header4IMG.wLeft + 1;

  im->f = 1;
  // 返回值
  return TRUE;
}

int fmtima_read(ihand_t m_strm, img_t* im, fmtbmp* fmt)
{
  unsigned char* data = im->tt.data;
  int step = im->s, nch = im->c;
  int result = FALSE;
  int color = nch > 1;
  int lWidth = im->w;
  int lHeight = im->h;
  int i, j, nTemp;
  PaletteEntry* lpbmc = fmt->m_palette;
  BYTE bChar; // 中间变量
  int* lpSrc; // 指向源图像象素的指针
  LPBYTE lpDst; // 指向编码后图像数据的指针
  int* lpTemp; // 临时指针
  // 用来参与预测的三个象素和当前编码象素
  BYTE bCharA, bCharB, bCharC;
  int lLineBytes = step; // 图像每行的字节数

  lpSrc = GMALLOC(int, lWidth * lHeight);
  lpTemp = lpSrc;

  // 读取编码后的象素
  if ((int) READ_NBYTE(lpSrc, (lWidth * lHeight * sizeof(int)))
      != (int)(lWidth * lHeight * sizeof(int))) {
    return FALSE;
  }

  // 像素位置
  lpDst = data;

  // 解码第0行
  i = 0;

  for (j = 0; j < lWidth; j++) {
    if (j == 0) {
      // 如果是0行0列，编码值就是真实值
      lpDst[ j + lLineBytes * (lHeight - 1 - i) ] = (BYTE)(*lpTemp);
      lpTemp ++;
    }
    else {
      // 利用 D＝A＋残差 得到原来的象素
      lpDst[ j + lLineBytes * (lHeight - 1 - i) ]
        = (BYTE)(*lpTemp) + lpDst[ j + lLineBytes * (lHeight - 1 - i) - 1 ];
      lpTemp++;
    }
  }

  // 解码第1行到第lHeight－1行
  for (i = 1; i < lHeight; i++) {
    for (j = 0; j < lWidth; j++) {
      // 得到象素B的值
      bCharB = lpDst[ j + lLineBytes * (lHeight - i) ];

      // 解码第一列
      if (j == 0) {
        // 利用 D＝B＋残差 得到原来的象素值
        lpDst[ j + lLineBytes * (lHeight - 1 - i) ] = (BYTE)((*lpTemp) + bCharB);
        lpTemp++;
      }

      // 解码剩下的列
      else {
        // 利用 D=(B-C)/2 + A + 残差 得到原来的象素值
        bCharA = lpDst[ j - 1 + lLineBytes * (lHeight - 1 - i) ];
        bCharC = lpDst[ j - 1 + lLineBytes * (lHeight - i) ];

        // 解码时的预测
        nTemp = (int)((bCharB - bCharC) / 2 + bCharA);

        // 预测值小于0，直接赋0
        if (nTemp < 0) {
          nTemp = 0;
        }
        // 预测值大于255，直接赋值255
        else if (nTemp > 255) {
          nTemp = 255;
        }
        else {
          nTemp = nTemp;
        }

        // 预测值＋残差
        lpDst[ j + lLineBytes * (lHeight - 1 - i) ]
          = (BYTE)(*lpTemp + (BYTE) nTemp);
        lpTemp++;
      }
    }
  }

  // 释放内存
  GFREE(lpSrc);
  lpDst = NULL;

  // 读调色板标志位
  READ_NBYTE(&bChar, 1);

  if (bChar != 0x0C) {
    // 返回NULL。
    return FALSE;
  }

  // 分配内存以读取编码后的象素
  lpDst = GMALLOC(BYTE, 768);

  // 读取调色板
  if (READ_NBYTE(lpDst, 768) != 768) {
    return FALSE;
  }

  // 给调色板赋值
  for (i = 0; i < 256; i++) {
    lpbmc[ i ].r = lpDst[ i * 3 + 2 ];
    lpbmc[ i ].g = lpDst[ i * 3 + 1 ];
    lpbmc[ i ].b = lpDst[ i * 3 ];
    lpbmc[ i ].a = 0;
  }

  GFREE(lpSrc);

  return result;
}

int fmtima_write(ihand_t m_strm, const img_t* im, const PaletteEntry* palette)
{
  int height = im->h, width = im->w, step = im->s, channels = im->c;
  const unsigned char* data = im->tt.data;
  int isBinary = FALSE;
  int result = FALSE;

  int channels0 = channels > 1 ? 3 : 1;
  int fileStep = width * channels0;

  assert(data && width > 0 && height > 0 && step >= fileStep);

  if (m_strm) {    // 指向源图像的指针
    const unsigned char* lpSrc;
    PaletteEntry lpbmc[ 256 ] = {0};
    int i, j; // 循环变量
    // 参与预测的象素和当前编码的象素
    BYTE bCharA, bCharB, bCharC, bCharD;
    int nTemp; // 预测值
    int nDpcm; // 预测后的残差
    BYTE lpDst[769]; // 指向编码后图像数据的指针

    //写入IMG文件头信息
    IMGHEADER Header4IMG;
    // 给文件头赋值
    Header4IMG.bBpp = 8;                // 像素位数（256色为8位）
    Header4IMG.wLeft = 0;               // 图像相对于屏幕的左上角X坐标（以像素为单位）
    Header4IMG.wTop = 0;                // 图像相对于屏幕的左上角Y坐标（以像素为单位）
    Header4IMG.wRight = width - 1;     // 图像相对于屏幕的右下角X坐标（以像素为单位）
    Header4IMG.wBottom = height - 1;   // 图像相对于屏幕的右下角Y坐标（以像素为单位）
    Header4IMG.wXResolution = (unsigned short) width;    // 图像的水平分辨率
    Header4IMG.wYResolution = (unsigned short) height;   // 图像的垂直分辨率

    // 写入文件头
    WRITE_NBYTE((LPBYTE) & Header4IMG, sizeof(IMGHEADER));

    // 编码第0行
    i = 0;

    for (j = 0; j < width; j++) {
      // 指向图象0行j列象素的指针
      lpSrc = data + step * (height - 1 - i) + j;

      // 给bCharD赋值
      bCharD = *lpSrc;

      // 如果是第0行0列，直接将象素值写入
      if (j == 0) {
        nDpcm = (int) bCharD;
      }
      // 利用 Dpcm ＝D － A 计算残差
      else {
        bCharA = *(lpSrc - 1);
        nDpcm = (int) bCharD - (int) bCharA;
      }

      // 将残差写入文件
      WRITE_NBYTE(&nDpcm , sizeof(int));
    }

    // 编码第1行到lHeight-1行
    for (i = 1; i < height; i++) {
      for (j = 0; j < width; j++) {
        // 指向当前编码元素的指针
        lpSrc = (LPBYTE) data + j + step * (height - 1 - i);

        // 赋值
        bCharD = *lpSrc;
        bCharB = *(lpSrc + step);

        // 如果是第一列，利用 残差＝D －B 进行预测
        if (j == 0) {
          nDpcm = (int) bCharD - (int) bCharB;
        }
        else {
          // 利用(B-C)/2+A计算预测值
          bCharA = *(lpSrc - 1);
          bCharC = *(lpSrc + step - 1);
          nTemp = (int)((bCharB - bCharC) / 2 + bCharA);

          // 如果预测值小于0，直接赋零
          if (nTemp < 0) {
            nTemp = 0;
          }
          // 如果预测值大于255，直接赋值255
          else if (nTemp > 255) {
            nTemp = 255;
          }
          else {
            nTemp = nTemp;
          }

          // 得到残差
          nDpcm = (int) bCharD - nTemp;
        }

        // 将残差写入文件
        WRITE_NBYTE(&nDpcm , sizeof(int));
      }
    }

    // 开辟一片缓冲区以保存调色板

    // 调色板起始字节
    * lpDst = 0x0C;

    // 读取当前图象的调色板
    for (i = 0; i < 256; i ++) {
      lpDst[ i * 3 + 1 ] = lpbmc[ i ].r; // 读取DIB调色板红色分量
      lpDst[ i * 3 + 2 ] = lpbmc[ i ].g; // 读取DIB调色板绿色分量
      lpDst[ i * 3 + 3 ] = lpbmc[ i ].b; // 读取DIB调色板蓝色分量
    }

    // 写入调色板信息
    WRITE_NBYTE((LPBYTE) lpDst, 769);

    // 返回值
    result = TRUE;
  }

  return result;
}


#endif // SUPPORT_IMA
