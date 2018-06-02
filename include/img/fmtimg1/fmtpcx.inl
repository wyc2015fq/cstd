
#ifndef SUPPORT_PCX
#define SUPPORT_PCX

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include "_fmtimg.h"

// P?M filter factory

typedef struct fmtpcx {
  int m_bpp;
  int m_offset;
  int m_binary;
  int m_maxval;
  PaletteEntry m_palette[ 256 ];
}
fmtpcx;

int fmtpcx_checksign(ihand_t m_strm)
{
  return 0;
}

int fmtpcx_free(fmtpcx* reader)
{
  return 0;
}

int fmtpcx_read_header(ihand_t m_strm, img_t* im, fmtpcx* fmt)
{
  int result = FALSE;
  int m_sign_len = 0;
  const char* m_signature = "";
  const char* m_description = _T("Portable image format (*.pcx)");
  fmt->m_offset = -1;
  fmt->m_bpp = 0;
  fmt->m_binary = fmt->m_maxval = 0;

  if (!m_strm) {
    return FALSE;
  }

  if (!result) {
    fmt->m_offset = -1;
    im->w = im->h = -1;
  }

  return result;
}

int fmtpcx_read_data(ihand_t m_strm, img_t* im, fmtpcx* fmt)
{
  return 0;
}

/*************************************************************************
* 函数名称:
*     PCXCoding()
* 参数:
*   uchar* lpDIBBits  - 指向DIB对象像素的指针
*   int lWidth   - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight  - 源图像高度（象素数）
*   ihand_t file         - 要保存的文件
* 返回值:
*   int               - 成功返回TRUE，否则返回False。
* 说明:
*   该函数将指定的256色DIB对象保存为256色PCX文件。
 *************************************************************************/
int PCXCoding(int lHeight, int lWidth, uchar* lpDIBBits, int lLineBytes, ihand_t m_strm)
{
  typedef struct {
    char manufacturer;
    char version;
    char encoding;
    char bits_per_pixel;
    WORD xmin, ymin;
    WORD xmax, ymax;
    WORD hres;
    WORD vres;
    char palette[ 48 ];
    char reserved;
    char colour_planes;
    WORD bytes_per_line;
    WORD palette_type;
    char filler[ 58 ];
  }
  PCXHEAD;

  // 循环变量
  int i, j;
  // 中间变量
  uint8 bChar1, bChar2;

  // 指向源图像象素的指针
  uchar* lpSrc;
  // 指向编码后图像数据的指针
  uchar* lpDst;
  // 重复像素计数
  int iCount;
  // 缓冲区已使用的字节数
  uint32 dwBuffUsed;
  // PCX文件头
  PCXHEAD pcxHeadr;
  // PCX标识码
  pcxHeadr.manufacturer = 0x0A;
  // PCX版本号
  pcxHeadr.version = 5;
  // PCX编码方式（1表示RLE编码）
  pcxHeadr.encoding = 1;
  // 像素位数（256色为8位）
  pcxHeadr.bits_per_pixel = 8;
  // 图像相对于屏幕的左上角X坐标（以像素为单位）
  pcxHeadr.xmin = 0;
  // 图像相对于屏幕的左上角Y坐标（以像素为单位）
  pcxHeadr.ymin = 0;
  // 图像相对于屏幕的右下角X坐标（以像素为单位）
  pcxHeadr.xmax = (unsigned short)(lWidth - 1);
  // 图像相对于屏幕的右下角Y坐标（以像素为单位）
  pcxHeadr.ymax = (unsigned short)(lHeight - 1);
  // 图像的水平分辨率
  pcxHeadr.hres = (unsigned short) lWidth;
  // 图像的垂直分辨率
  pcxHeadr.vres = (unsigned short) lHeight;

  // 调色板数据（对于256色PCX无意义，直接赋值为0）
  for (i = 0; i < 48; i ++) {
    pcxHeadr.palette[ i ] = 0;
  }

  // 保留域，设定为0。
  pcxHeadr.reserved = 0;
  // 图像色彩平面数目（对于256色PCX设定为1）。
  pcxHeadr.colour_planes = 1;
  // 图像的宽度（字节为单位），必须为偶数。
  pcxHeadr.bytes_per_line = (unsigned short) lWidth;
  // 图像调色板的类型，1表示彩色或者单色图像，2表示图像是灰度图。
  pcxHeadr.palette_type = 1;

  // 制作该图像的屏幕宽度（像素为单位）
  //    pcxHeadr.wSrcWidth = 0;
  // 制作该图像的屏幕高度（像素为单位）
  //    pcxHeadr.wSrcDepth = 0;
  // 保留域，取值设定为0。
  for (i = 0; i < 54; i ++) {
    pcxHeadr.filler[ i ] = 0;
  }

  // 写入文件头
  WRITE_NBYTE((uchar*) & pcxHeadr, sizeof(PCXHEAD));
  // 开始编码

  // 开辟一片缓冲区(2被原始图像大小)以保存编码结果
  lpDst = GMALLOC(uint8, lHeight * lWidth * 2);
  // 指明当前已经用了多少缓冲区（字节数）
  dwBuffUsed = 0;

  // 每行
  for (i = 0; i < lHeight; i++) {
    // 指向DIB第i行，第0个象素的指针
    lpSrc = (uchar*) lpDIBBits + lLineBytes * (i);
    // 给bChar1赋值
    bChar1 = *lpSrc;
    // 设置iCount为1
    iCount = 1;

    // 剩余列
    for (j = 1; j < lWidth; j ++) {
      // 指向DIB第i行，第j个象素的指针
      lpSrc++;
      // 读取下一个像素
      bChar2 = *lpSrc;

      // 判断是否和bChar1相同并且iCount < 63
      if ((bChar1 == bChar2) && (iCount < 63)) {
        // 相同，计数加1
        iCount ++;
        // 继续读下一个
      }
      else {
        // 不同，或者iCount = 63
        // 写入缓冲区
        if ((iCount > 1) || (bChar1 >= 0xC0)) {
          // 保存码长信息
          lpDst[ dwBuffUsed ] = (uint8)(iCount | 0xC0);
          // 保存bChar1
          lpDst[ dwBuffUsed + 1 ] = bChar1;
          // 更新dwBuffUsed
          dwBuffUsed += 2;
        }
        else {
          // 直接保存该值
          lpDst[ dwBuffUsed ] = bChar1;
          // 更新dwBuffUsed
          dwBuffUsed ++;
        }

        // 重新给bChar1赋值
        bChar1 = bChar2;
        // 设置iCount为1
        iCount = 1;
      }
    }

    // 保存每行最后一部分编码
    if ((iCount > 1) || (bChar1 >= 0xC0)) {
      // 保存码长信息
      lpDst[ dwBuffUsed ] = (uint8)(iCount | 0xC0);
      // 保存bChar1
      lpDst[ dwBuffUsed + 1 ] = bChar1;
      // 更新dwBuffUsed
      dwBuffUsed += 2;
    }
    else {
      // 直接保存该值
      lpDst[ dwBuffUsed ] = bChar1;
      // 更新dwBuffUsed
      dwBuffUsed ++;
    }
  }

  // 写入编码结果
  WRITE_NBYTE((uchar*) lpDst, dwBuffUsed);
  // 释放内存
  GFREE(lpDst) ;
  // 开辟一片缓冲区以保存调色板
  lpDst = GMALLOC(uint8, 769);
  // 调色板起始字节
  * lpDst = 0x0C;

  for (i = 0; i < 256; i ++) {
    lpDst[ i * 3 + 1 ] = (uint8)(i);      // 读取DIB调色板红色分量
    lpDst[ i * 3 + 2 ] = (uint8)(i);      // 读取DIB调色板绿色分量
    lpDst[ i * 3 + 3 ] = (uint8)(i);      // 读取DIB调色板蓝色分量
  }

  // 写入调色板信息
  WRITE_NBYTE((uchar*) lpDst, 769);
  // 返回
  GFREE(lpDst);
  return TRUE;
}

int fmtpcx_write(ihand_t m_strm, const img_t* im, const PaletteEntry* palette)
{
  int height = im->h, width = im->w, step = im->s, channels = im->c;
  const unsigned char* data = im->tt.data;
  int isBinary = FALSE;
  int result = FALSE;

  int channels0 = channels > 1 ? 3 : 1;
  int fileStep = width * channels0;

  assert(data && width > 0 && height > 0 && step >= fileStep);

  if (m_strm) {}

  return result;
}


#endif // SUPPORT_PCX
