
#ifndef _DIBAPI_INL_
#define _DIBAPI_INL_

//  DIB(Independent Bitmap)API函数库：用来存放有关DIB的操作函数
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "cfile.h"

#include "img/bitmap.inl"
// 用来角度到弧度的转换
//#define AngleToRadian(angle)  ((angle)*PI/180.0)
#undef  FWRITE
#undef  FREAD
#define FWRITE(file, _PTR, _N)     fwrite( _PTR, _N, 1, file)
#define FWRITEHUGE(file, _PTR, _N) fwrite( _PTR, _N, 1, file)
#define FREAD(file, _PTR, _N)      fread( _PTR, _N, 1, file)
#define FREADHUGE(file, _PTR, _N)  fread( _PTR, _N, 1, file)
// GetLength(file
//DWORD GetLength(FILE* file);
//#define GetPosition(file)  ftell(file)
//DECLARE_HANDLE(HDIB);
#define PALVERSION   0x300
// 判断是否是Win 3.0的DIB
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))

// 在计算图像大小时，采用公式：biSizeImage = biWidth' × biHeight。
// 是biWidth'，而不是biWidth，这里的biWidth'必须是4的整倍数，表示
// 大于或等于biWidth的，离4最近的整倍数。WIDTHBYTES就是用来计算
// biWidth'
#define WIDTHBYTES(bits)    (((bits) + 31) / 32* 4)
// Dib文件头标志（字符串"BM"，写DIB时用到该常数）
#define DIB_HEADER_MARKER   ((WORD)('M' << 8) | 'B')
// PCX文件头结构
typedef struct PCXHEADER {
  BYTE bManufacturer;
  BYTE bVersion;
  BYTE bEncoding;
  BYTE bBpp;
  WORD wLeft;
  WORD wTop;
  WORD wRight;
  WORD wBottom;
  WORD wXResolution;
  WORD wYResolution;
  BYTE bPalette[ 48 ];
  BYTE bReserved;
  BYTE bPlanes;
  WORD wLineBytes;
  WORD wPaletteType;
  WORD wSrcWidth;
  WORD wSrcDepth;
  BYTE bFiller[ 54 ];
}
PCXHEADER;
// 用指定的伪彩色编码表来替换指定的DIB图像的颜色表
// 参数:
//   LPRGBQUAD pDibQuad     - 指向DIB的颜色表
//   LPBYTE pColorsTable    - 伪彩色编码表
//   int nColorEntries      - DIB中颜色表中的表项数
// 返回值:
//   BOOL            - 成功返回TRUE，否则返回FALSE
static BOOL ReplaceDIBColorTable(LPRGBQUAD pDibQuad, int nColorEntries, LPBYTE pColorsTable)
{
  int i; // 循环变量

  // 读取伪彩色编码，更新DIB颜色表
  for (i = 0; i < nColorEntries; i++) {
    // 更新DIB调色板红色分量
    pDibQuad->rgbRed = pColorsTable[ i * 4 + 0 ];
    // 更新DIB调色板绿色分量
    pDibQuad->rgbGreen = pColorsTable[ i * 4 + 1 ];
    // 更新DIB调色板蓝色分量
    pDibQuad->rgbBlue = pColorsTable[ i * 4 + 2 ];
    // 更新DIB调色板保留位
    pDibQuad->rgbReserved = 0;
    pDibQuad++;
  }

  // 返回
  return TRUE;
}
// 返回DIB中调色板的颜色的种数
//   对于单色位图，返回2，
//   对于16色位图，返回16，
//   对于256色位图，返回256；
//   对于真彩色位图（24位），没有调色板，返回0。
// 参数:
//   LPBYTE lpbi         - 指向DIB对象的指针
// 返回值:
//   WORD               - 返回调色板中颜色的种数
static WORD DIBNumColors(LPBYTE lpbi)
{
  WORD wBitCount;

  // 对于Windows的DIB, 实际颜色的数目可以比象素的位数要少。
  // 对于这种情况，则返回一个近似的数值。
  // 判断是否是WIN3.0 DIB
  if (IS_WIN30_DIB(lpbi)) {
    DWORD dwClrUsed;
    dwClrUsed = ((LPBITMAPINFOHEADER) lpbi) ->biClrUsed;     // 读取dwClrUsed值

    if (dwClrUsed != 0) {
      // 如果dwClrUsed（实际用到的颜色数）不为0，直接返回该值
      return (WORD) dwClrUsed;
    }
  }

  // 读取象素的位数
  if (IS_WIN30_DIB(lpbi)) {
    wBitCount = ((LPBITMAPINFOHEADER) lpbi) ->biBitCount;     // 读取biBitCount值
  }
  else {
    wBitCount = ((LPBITMAPCOREHEADER) lpbi) ->bcBitCount;     // 读取biBitCount值
  }

  // 按照象素的位数计算颜色数目
  switch (wBitCount) {
  case 1:
    return 2;
    break;

  case 4:
    return 16;
    break;

  case 8:
    return 256;
    break;

  default:
    return 0;
    break;
  }
}
// 返回DIB调色板的大小
//   对于Windows 3.0 DIB，返回颜色数目×RGBQUAD的大小；
//   对于其它返回颜色数目×RGBTRIPLE的大小。
// 函数名称：
//   PaletteSize()
// 参数:
//   LPBYTE lpbi         - 指向DIB对象的指针
// 返回值:
//   WORD               - DIB中调色板的大小
static WORD PaletteSize(LPBYTE lpbi)
{
  // 计算DIB中调色板的大小
  if (IS_WIN30_DIB(lpbi)) {
    //返回颜色数目×RGBQUAD的大小
    return (WORD)(DIBNumColors(lpbi) * sizeof(RGBQUAD));
  }
  else {
    //返回颜色数目×RGBTRIPLE的大小
    return (WORD)(DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
  }
}
// 返回DIB图像象素起始位置
//   计算DIB中图像象素的起始位置，并返回指向它的指针。
//   LPBYTE lpbi  - 指向DIB对象的指针
// 返回值:
//   LPBYTE - 指向DIB图像象素起始位置
static LPBYTE DIBBits(LPBYTE lpbi)
{
  return (lpbi + * (LPDWORD) lpbi + PaletteSize(lpbi));
}
// 返回DIB图像宽度 对于Windows 3.0 DIB，返回BITMAPINFOHEADER中的biWidth值；对于其它返回BITMAPCOREHEADER中的bcWidth值。
// 参数:
//   LPBYTE lpbi         - 指向DIB对象的指针
// 返回值:
//   DWORD              - DIB中图像的宽度
#define DIBWidth(lpDIB)   (IS_WIN30_DIB(lpDIB) ? ((LPBITMAPINFOHEADER) lpDIB)->biWidth : ((LPBITMAPCOREHEADER) lpDIB)->bcWidth)

// 返回DIB图像高度 对于Windows 3.0 DIB，返回BITMAPINFOHEADER中的biHeight值； 对于其它返回BITMAPCOREHEADER中的bcHeight值。
// 参数:
//   LPBYTE lpDIB        - 指向DIB对象的指针
// 返回值:
//   DWORD              - DIB中图像的高度
#define DIBHeight(lpDIB)   (IS_WIN30_DIB(lpDIB) ? ((LPBITMAPINFOHEADER) lpDIB)->biHeight : ((LPBITMAPCOREHEADER) lpDIB)->bcHeight)

// 创建DIB对象调色板
// 按照DIB创建一个逻辑调色板，从DIB中读取颜色表并存到调色板中，
// 最后按照该逻辑调色板创建一个新的调色板，并返回该调色板的句柄。这样
// 可以用最好的颜色来显示DIB图像。
//   LPBYTE lpbi          - 指向DIB的指针
//   HPALETTE pPal     - 指向DIB对象调色板的指针
// 返回值:
//   BOOL               - 创建成功返回TRUE，否则返回FALSE。
static BOOL DIBCreatePalette(LPBYTE lpbi, HPALETTE pPal)
{
  LPLOGPALETTE lpPal;  // 指向逻辑调色板的指针
  HANDLE hLogPal;   // 逻辑调色板的句柄
  HPALETTE hPal = NULL; // 调色板的句柄
  int i;     // 循环变量
  WORD wNumColors;  // 颜色表中的颜色数目
  LPBITMAPINFO lpbmi;  // 指向BITMAPINFO结构的指针（Win3.0）
  LPBITMAPCOREINFO lpbmc; // 指向BITMAPCOREINFO结构的指针
  BOOL bWinStyleDIB;  // 表明是否是Win3.0 DIB的标记
  BOOL bResult = FALSE; // 创建结果

  if (lpbi == NULL) {
    return FALSE;
  }

  lpbmi = (LPBITMAPINFO) lpbi;    // 获取指向BITMAPINFO结构的指针（Win3.0）
  lpbmc = (LPBITMAPCOREINFO) lpbi;   // 获取指向BITMAPCOREINFO结构的指针
  wNumColors = DIBNumColors(lpbi);   // 获取DIB中颜色表中的颜色数目

  if (wNumColors != 0) {
    // 分配为逻辑调色板内存
    hLogPal = GMALLOC(BYTE, sizeof(LOGPALETTE)
        + sizeof(PALETTEENTRY)
        * wNumColors);

    // 如果内存不足，退出
    if (hLogPal == 0) {
      return FALSE;
    }

    lpPal = (LPLOGPALETTE)(LPBYTE) GlobalLock((HGLOBAL) hLogPal);
    lpPal->palVersion = PALVERSION;   // 设置版本号
    lpPal->palNumEntries = (WORD) wNumColors;   // 设置颜色数目
    bWinStyleDIB = IS_WIN30_DIB(lpbi);    // 判断是否是WIN3.0的DIB

    // 读取调色板
    for (i = 0; i < (int) wNumColors; i++) {
      if (bWinStyleDIB) {
        // 读取红色绿色蓝色分量
        lpPal->palPalEntry[ i ].peRed = lpbmi->bmiColors[ i ].rgbRed;
        lpPal->palPalEntry[ i ].peGreen = lpbmi->bmiColors[ i ].rgbGreen;
        lpPal->palPalEntry[ i ].peBlue = lpbmi->bmiColors[ i ].rgbBlue;
        // 保留位
        lpPal->palPalEntry[ i ].peFlags = 0;
      }
      else {
        // 读取红色绿色蓝色分量
        lpPal->palPalEntry[ i ].peRed = lpbmc->bmciColors[ i ].rgbtRed;
        lpPal->palPalEntry[ i ].peGreen = lpbmc->bmciColors[ i ].rgbtGreen;
        lpPal->palPalEntry[ i ].peBlue = lpbmc->bmciColors[ i ].rgbtBlue;
        // 保留位
        lpPal->palPalEntry[ i ].peFlags = 0;
      }
    }

    pPal = CreatePalette(lpPal);   // 按照逻辑调色板创建调色板，并返回指针
    bResult = pPal != 0;
    GlobalUnlock((HGLOBAL) hLogPal);     // 解除锁定
    GlobalFree((HGLOBAL) hLogPal);     // 释放逻辑调色板
  }

  return bResult;
}
//   用指定的伪彩色编码表来替换图像的调试板，参数bpColorsTable
// 指向要替换的伪彩色编码表。
// 参数:
//   LPBYTE lpDIB   - 指向源DIB图像指针
//   LPBYTE bpColorsTable - 伪彩色编码表
// 返回值:
//   BOOL     - 成功返回TRUE，否则返回FALSE。
static BOOL ReplaceColorPal(LPBYTE lpDIB, LPBYTE bpColorsTable)
{
  int i;     // 循环变量
  WORD wNumColors;  // 颜色表中的颜色数目
  LPBITMAPINFO lpbmi;  // 指向BITMAPINFO结构的指针（Win3.0）
  LPBITMAPCOREINFO lpbmc; // 指向BITMAPCOREINFO结构的指针
  BOOL bWinStyleDIB;  // 表明是否是Win3.0 DIB的标记
  BOOL bResult = FALSE; // 创建结果
  lpbmi = (LPBITMAPINFO) lpDIB;    // 获取指向BITMAPINFO结构的指针（Win3.0）
  lpbmc = (LPBITMAPCOREINFO) lpDIB;   // 获取指向BITMAPCOREINFO结构的指针
  wNumColors = DIBNumColors(lpDIB);   // 获取DIB中颜色表中的颜色数目

  if (wNumColors == 256) { // 判断颜色数目是否是256色
    bWinStyleDIB = IS_WIN30_DIB(lpDIB);   // 判断是否是WIN3.0的DIB

    // 读取伪彩色编码，更新DIB调色板
    for (i = 0; i < (int) wNumColors; i++) {
      if (bWinStyleDIB) {
        // 更新DIB调色板红色绿色蓝色分量
        lpbmi->bmiColors[ i ].rgbRed = bpColorsTable[ i * 4 ];
        lpbmi->bmiColors[ i ].rgbGreen = bpColorsTable[ i * 4 + 1 ];
        lpbmi->bmiColors[ i ].rgbBlue = bpColorsTable[ i * 4 + 2 ];
        // 更新DIB调色板保留位
        lpbmi->bmiColors[ i ].rgbReserved = 0;
      }
      else {
        // 更新DIB调色板红色绿色蓝色分量
        lpbmc->bmciColors[ i ].rgbtRed = bpColorsTable[ i * 4 ];
        lpbmc->bmciColors[ i ].rgbtGreen = bpColorsTable[ i * 4 + 1 ];
        lpbmc->bmciColors[ i ].rgbtBlue = bpColorsTable[ i * 4 + 2 ];
      }
    }
  }

  return bResult;
}
// 拷贝内存块
//   复制指定的内存区域。返回复制后的新内存区域，出错时返回0。
// 参数:
//   HGLOBAL h          - 要复制的内存区域
// 返回值:
//   HGLOBAL            - 复制后的新内存区域
static HGLOBAL CopyHandle(HGLOBAL h)
{
  if (h != NULL) {
    DWORD dwLen = GlobalSize((HGLOBAL) h);     // 获取指定内存区域大小
    HGLOBAL hCopy = MALLOC(BYTE, dwLen);   // 分配新内存空间

    if (hCopy != NULL) {   // 判断分配是否成功
      void* lpCopy = (LPBYTE) GlobalLock((HGLOBAL) hCopy);
      void* lp = (LPBYTE) GlobalLock((HGLOBAL) h);
      memcpy(lpCopy, lp, dwLen);
      GlobalUnlock(hCopy);
      GlobalUnlock(h);
    }

    return hCopy;
  }

  return NULL;
}
// 将DIB保存到指定文件中
// 将指定的DIB对象保存到指定的CFile中。该CFile由调用程序打开和关闭。
//   SaveDIB()
// 参数:
//   LPBYTE pDib          - 要保存的DIB
//   FILE* file        - 保存文件CFile
// 返回值:
//   BOOL               - 成功返回TRUE，否则返回FALSE或者CFileException
static BOOL SaveDIB(LPBYTE pDib, FILE* file)
{
  BITMAPFILEHEADER bmfHdr; // Bitmap文件头
  LPBITMAPINFOHEADER lpBI; // 指向BITMAPINFOHEADER的指针
  DWORD dwDIBSize;   // DIB大小

  if (pDib == NULL) {
    return FALSE;
  }

  // 读取BITMAPINFO结构，并锁定
  lpBI = (LPBITMAPINFOHEADER)pDib;

  if (lpBI == NULL) {
    return FALSE;
  }

  // 判断是否是WIN3.0 DIB
  if (!IS_WIN30_DIB(lpBI)) {
    // 不支持其它类型的DIB保存
    return FALSE;
  }

  // 填充文件头
  bmfHdr.bfType = DIB_HEADER_MARKER;  // 文件类型"BM"
  // 计算DIB大小时，最简单的方法是调用GlobalSize()函数。但是全局内存大小并
  // 不是DIB真正的大小，它总是多几个字节。这样就需要计算一下DIB的真实大小。
  // 文件头大小＋颜色表大小
  // （BITMAPINFOHEADER和BITMAPCOREHEADER结构的第一个DWORD都是该结构的大小）
  dwDIBSize = *(LPDWORD) lpBI + PaletteSize((LPBYTE) lpBI);

  // 计算图像大小
  if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4)) {
    // 对于RLE位图，没法计算大小，只能信任biSizeImage内的值
    dwDIBSize += lpBI->biSizeImage;
  }
  else {
    DWORD dwBmBitsSize;   // 象素的大小
    dwBmBitsSize = WIDTHBYTES((lpBI->biWidth) * ((DWORD) lpBI->biBitCount))
        * lpBI->biHeight;  // 大小为Width* Height
    dwDIBSize += dwBmBitsSize; // 计算出DIB真正的大小
    // 更新biSizeImage（很多BMP文件头中biSizeImage的值是错误的）
    lpBI->biSizeImage = dwBmBitsSize;
  }

  // 计算文件大小：DIB大小＋BITMAPFILEHEADER结构大小
  bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
  // 两个保留字
  bmfHdr.bfReserved1 = 0;
  bmfHdr.bfReserved2 = 0;
  // 计算偏移量bfOffBits，它的大小为Bitmap文件头大小＋DIB头大小＋颜色表大小
  bmfHdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + lpBI->biSize
      + PaletteSize((LPBYTE) lpBI);
  // 尝试写文件
  FWRITE(file, (LPBYTE) & bmfHdr, sizeof(BITMAPFILEHEADER));       // 写文件头
  FWRITEHUGE(file, lpBI, dwDIBSize);       // 写DIB头和象素
  return TRUE;
}
// 重指定文件中读取DIB对象
//   将指定的文件中的DIB对象读到指定的内存区域中。除BITMAPFILEHEADER
// 外的内容都将被读入内存。
// 参数:
//   FILE* file        - 要读取得文件文件CFile
// 返回值:
//   LPBYTE               - 成功返回DIB，否则返回NULL。
static LPBYTE ReadDIBFile(FILE* file)
{
  BITMAPFILEHEADER bmfHeader;
  LPBYTE pDIB;
  DWORD dwBitsSize;
  dwBitsSize = (int)fsize(file);    // 获取DIB（文件）长度（字节）

  // 尝试读取DIB文件头
  if (FREAD(file, (LPBYTE) & bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader)) {
    return NULL;
  }

  // 判断是否是DIB对象，检查头两个字节是否是"BM"
  if (bmfHeader.bfType != DIB_HEADER_MARKER) {
    return NULL;
  }

  // 为DIB分配内存
  pDIB = MALLOC(BYTE, dwBitsSize);

  if (pDIB == 0) {
    return NULL;
  }

  if (FREADHUGE(file, pDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) !=
      dwBitsSize - sizeof(BITMAPFILEHEADER)) {  // 读象素
    free(pDIB);
    return NULL;
  }

  return pDIB;
}
// 返回DIBBitCount。
// 参数:
//   LPBYTE lpbi         - 指向DIB对象的指针
// 返回值:
//   WORD               - 返回调色板中颜色的种数
static WORD DIBBitCount(LPBYTE lpbi)
{
  WORD wBitCount;

  // 读取象素的位数
  if (IS_WIN30_DIB(lpbi)) {
    // 读取biBitCount值
    wBitCount = ((LPBITMAPINFOHEADER) lpbi) ->biBitCount;
  }
  else {
    // 读取biBitCount值
    wBitCount = ((LPBITMAPCOREHEADER) lpbi) ->bcBitCount;
  }

  // 返回wBitCount
  return wBitCount;
}
// 将指定的256色DIB对象保存为256色PCX文件
// 参数:
//   LPBYTE lpDIB        - 指向DIB对象的指针
//   FILE* file        - 要保存的文件
// 返回值:
//   BOOL               - 成功返回TRUE，否则返回False。
static BOOL DIBToPCX256(LPBYTE lpDIB, FILE* file)
{
  // 循环变量
  int i;
  int j;
  // DIB高度
  WORD wHeight;
  // DIB宽度
  WORD wWidth;
  // 中间变量
  BYTE bChar1;
  BYTE bChar2;
  // 指向源图像象素的指针
  LPBYTE lpSrc;
  // 指向编码后图像数据的指针
  LPBYTE lpDst;
  // 图像每行的字节数
  int lLineBytes;
  // 重复像素计数
  int iCount;
  // 缓冲区已使用的字节数
  DWORD dwBuffUsed;
  // 指向DIB象素指针
  LPBYTE lpDIBBits;
  // PCX文件头
  PCXHEADER pcxHdr;
  // 指向BITMAPINFO结构的指针（Win3.0）
  LPBITMAPINFO lpbmi;
  // 指向BITMAPCOREINFO结构的指针
  LPBITMAPCOREINFO lpbmc;
  // 表明是否是Win3.0 DIB的标记
  BOOL bWinStyleDIB;
  // 获取DIB高度
  wHeight = (WORD) DIBHeight(lpDIB);
  // 获取DIB宽度
  wWidth = (WORD) DIBWidth(lpDIB);
  // 找到DIB图像象素起始位置
  lpDIBBits = DIBBits(lpDIB);
  // 计算图像每行的字节数
  lLineBytes = WIDTHBYTES(wWidth * 8);
  // 给文件头赋值
  // PCX标识码
  pcxHdr.bManufacturer = 0x0A;
  // PCX版本号
  pcxHdr.bVersion = 5;
  // PCX编码方式（1表示RLE编码）
  pcxHdr.bEncoding = 1;
  // 像素位数（256色为8位）
  pcxHdr.bBpp = 8;
  // 图像相对于屏幕的左上角X坐标（以像素为单位）
  pcxHdr.wLeft = 0;
  // 图像相对于屏幕的左上角Y坐标（以像素为单位）
  pcxHdr.wTop = 0;
  // 图像相对于屏幕的右下角X坐标（以像素为单位）
  pcxHdr.wRight = wWidth - 1;
  // 图像相对于屏幕的右下角Y坐标（以像素为单位）
  pcxHdr.wBottom = wHeight - 1;
  // 图像的水平分辨率
  pcxHdr.wXResolution = wWidth;
  // 图像的垂直分辨率
  pcxHdr.wYResolution = wHeight;

  // 调色板数据（对于256色PCX无意义，直接赋值为0）
  for (i = 0; i < 48; i ++) {
    pcxHdr.bPalette[ i ] = 0;
  }

  // 保留域，设定为0。
  pcxHdr.bReserved = 0;
  // 图像色彩平面数目（对于256色PCX设定为1）。
  pcxHdr.bPlanes = 1;
  // 图像的宽度（字节为单位），必须为偶数。
  // if ((wWidth & 1)== 0)
  // {
  pcxHdr.wLineBytes = wWidth;
  // }
  // else
  // {
  //  pcxHdr.wLineBytes = wWidth + 1;
  // }
  // 图像调色板的类型，1表示彩色或者单色图像，2表示图像是灰度图。
  pcxHdr.wPaletteType = 1;
  // 制作该图像的屏幕宽度（像素为单位）
  pcxHdr.wSrcWidth = 0;
  // 制作该图像的屏幕高度（像素为单位）
  pcxHdr.wSrcDepth = 0;

  // 保留域，取值设定为0。
  for (i = 0; i < 54; i ++) {
    pcxHdr.bFiller[ i ] = 0;
  }

  // 写入文件头
  FWRITE(file, (LPBYTE) & pcxHdr, sizeof(PCXHEADER));
  // 开始编码
  // 开辟一片缓冲区(2被原始图像大小)以保存编码结果
  lpDst = MALLOC(BYTE, wHeight * wWidth * 2);
  // 指明当前已经用了多少缓冲区（字节数）
  dwBuffUsed = 0;

  // 每行
  for (i = 0; i < wHeight; i++) {
    // 指向DIB第i行，第0个象素的指针
    lpSrc = (LPBYTE) lpDIBBits + lLineBytes * (wHeight - 1 - i);
    // 给bChar1赋值
    bChar1 = *lpSrc;
    // 设置iCount为1
    iCount = 1;

    // 剩余列
    for (j = 1; j < wWidth; j ++) {
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
          lpDst[ dwBuffUsed ] = iCount | 0xC0;
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
      lpDst[ dwBuffUsed ] = iCount | 0xC0;
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
  FWRITEHUGE(file, (LPBYTE) lpDst, dwBuffUsed);
  // 释放内存
  FREE(lpDst);
  // 写入调色板信息
  // 开辟一片缓冲区以保存调色板
  lpDst = MALLOC(BYTE, 769);
  // 调色板起始字节
  * lpDst = 0x0C;
  // 获取指向BITMAPINFO结构的指针（Win3.0）
  lpbmi = (LPBITMAPINFO) lpDIB;
  // 获取指向BITMAPCOREINFO结构的指针
  lpbmc = (LPBITMAPCOREINFO) lpDIB;
  // 判断是否是WIN3.0的DIB
  bWinStyleDIB = IS_WIN30_DIB(lpDIB);

  // 读取当前DIB调色板
  for (i = 0; i < 256; i ++) {
    if (bWinStyleDIB) {
      // 读取DIB调色板红色分量
      lpDst[ i * 3 + 1 ] = lpbmi->bmiColors[ i ].rgbRed;
      // 读取DIB调色板绿色分量
      lpDst[ i * 3 + 2 ] = lpbmi->bmiColors[ i ].rgbGreen;
      // 读取DIB调色板蓝色分量
      lpDst[ i * 3 + 3 ] = lpbmi->bmiColors[ i ].rgbBlue;
    }
    else {
      // 读取DIB调色板红色分量
      lpDst[ i * 3 + 1 ] = lpbmc->bmciColors[ i ].rgbtRed;
      // 读取DIB调色板绿色分量
      lpDst[ i * 3 + 2 ] = lpbmc->bmciColors[ i ].rgbtGreen;
      // 读取DIB调色板蓝色分量
      lpDst[ i * 3 + 3 ] = lpbmc->bmciColors[ i ].rgbtBlue;
    }
  }

  // 写入调色板信息
  FWRITE(file, (LPBYTE) lpDst, 769);
  // 返回
  return TRUE;
}
// 读取256色PCX文件
//   将读取指定的256色PCX文件。将读取的结果保存在一个未压缩
// 编码的DIB对象中。
// 参数:
//   FILE* file        - 要读取的文件
// 返回值:
//   LPBYTE               - 成功返回DIB的，否则返回NULL。
static LPBYTE ReadPCX256(FILE* file)
{
  // PCX文件头
  PCXHEADER pcxHdr;
  // DIB大小（字节数）
  DWORD dwDIBSize;
  // DIB指针
  LPBYTE pDIB;
  // 循环变量
  int i;
  int j;
  // 重复像素计数
  int iCount;
  // DIB高度
  WORD wHeight;
  // DIB宽度
  WORD wWidth;
  // 图像每行的字节数
  int lLineBytes;
  // 中间变量
  BYTE bChar;
  // 指向源图像象素的指针
  LPBYTE lpSrc;
  // 指向编码后图像数据的指针
  LPBYTE lpDst;
  // 临时指针
  LPBYTE lpTemp;
  // 指向BITMAPINFOHEADER的指针
  LPBITMAPINFOHEADER lpBI;

  // 尝试读取PCX文件头
  if (FREAD(file, (LPBYTE) & pcxHdr, sizeof(PCXHEADER)) != sizeof(PCXHEADER)) {
    // 大小不对，返回NULL。
    return NULL;
  }

  // 判断是否是256色PCX文件，检查第一个字节是否是0x0A，
  if ((pcxHdr.bManufacturer != 0x0A) || (pcxHdr.bBpp != 8) || (pcxHdr.bPlanes != 1)) {
    // 非256色PCX文件，返回NULL。
    return NULL;
  }

  // 获取图像高度
  wHeight = pcxHdr.wBottom - pcxHdr.wTop + 1;
  // 获取图像宽度
  wWidth = pcxHdr.wRight - pcxHdr.wLeft + 1;
  // 计算图像每行的字节数
  lLineBytes = WIDTHBYTES(wWidth * 8);
  // 计算DIB长度（字节）
  dwDIBSize = sizeof(BITMAPINFOHEADER) + 1024 + wHeight * lLineBytes;
  // 为DIB分配内存
  pDIB = MALLOC(BYTE, dwDIBSize);

  if (pDIB == 0) {
    // 内存分配失败，返回NULL。
    return NULL;
  }

  // 赋值
  lpBI = (LPBITMAPINFOHEADER) pDIB;
  // 给lpBI成员赋值
  lpBI->biSize = 40;
  lpBI->biWidth = wWidth;
  lpBI->biHeight = wHeight;
  lpBI->biPlanes = 1;
  lpBI->biBitCount = 8;
  lpBI->biCompression = BI_RGB;
  lpBI->biSizeImage = wHeight * lLineBytes;
  lpBI->biXPelsPerMeter = pcxHdr.wXResolution;
  lpBI->biYPelsPerMeter = pcxHdr.wYResolution;
  lpBI->biClrUsed = 0;
  lpBI->biClrImportant = 0;
  // 分配内存以读取编码后的象素
  lpSrc = MALLOC(BYTE, GetLength(file) - sizeof(PCXHEADER) - 769);
  lpTemp = lpSrc;

  // 读取编码后的象素
  if (FREADHUGE(file, lpSrc, GetLength(file) - sizeof(PCXHEADER) - 769) !=
      GetLength(file) - sizeof(PCXHEADER) - 769) {
    // 大小不对
    // 释放内存
    free(lpSrc);
    free(pDIB);
    // 返回NULL。
    return NULL;
  }

  // 计算DIB中像素位置
  lpDst = (LPBYTE) DIBBits(pDIB);

  // 一行一行解码
  for (j = 0; j < wHeight; j++) {
    i = 0;

    while (i < wWidth) {
      // 读取一个字节
      bChar = *lpTemp;
      lpTemp++;

      if ((bChar & 0xC0) == 0xC0) {
        // 行程
        iCount = bChar & 0x3F;
        // 读取下一个字节
        bChar = *lpTemp;
        lpTemp++;
        // bChar重复iCount次保存
        memset(&lpDst[(wHeight - j - 1) * lLineBytes + i ], bChar, iCount);
        // 已经读取像素的个数加iCount
        i += iCount;
      }
      else {
        // 保存当前字节
        lpDst[(wHeight - j - 1) * lLineBytes + i ] = bChar;
        // 已经读取像素的个数加1
        i += 1;
      }
    }
  }

  // 释放内存
  FREE(lpSrc);
  // 调色板
  // 读调色板标志位
  FREAD(file, &bChar, 1);

  if (bChar != 0x0C) {
    // 出错
    // 释放内存
    free(pDIB);
    // 返回NULL。
    return NULL;
  }

  // 分配内存以读取编码后的象素
  lpSrc = MALLOC(BYTE, 768);
  // 计算DIB中调色板的位置
  lpDst = (LPBYTE) pDIB + sizeof(BITMAPINFOHEADER);

  // 读取调色板
  if (FREAD(file, lpSrc, 768) != 768) {
    // 大小不对。
    // 释放内存
    free(pDIB);
    // 返回NULL。
    return NULL;
  }

  // 给调色板赋值
  for (i = 0; i < 256; i++) {
    lpDst[ i * 4 ] = lpSrc[ i * 3 + 2 ];
    lpDst[ i * 4 + 1 ] = lpSrc[ i * 3 + 1 ];
    lpDst[ i * 4 + 2 ] = lpSrc[ i * 3 ];
    lpDst[ i * 4 + 3 ] = 0;
  }

  // 释放内存
  FREE(lpSrc);
  // 返回DIB句柄
  return pDIB;
}
// 获得当前正在使用的系统调色板的句柄
// 返回值:
//   HPALETTE    - 系统调色板句柄
static HPALETTE GetSystemPalette()
{
  // 设备上下文
  HDC hDC;
  // 声明调色板句柄、指针等临时变量
  static HPALETTE hPal = NULL;
  HANDLE hLogPal;
  LPLOGPALETTE lpLogPal;
  // 当前系统调色板的颜色数
  int nColors;
  // 获得当前系统设备上下文
  hDC = GetDC(NULL);

  if (!hDC) {
    return NULL;
  }

  // 获得当前系统调色板的颜色数目
  nColors = (1 << (GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES)));
  // 给调色板分配内存
  hLogPal = MALLOC(BYTE, sizeof(LOGPALETTE) + nColors *
      sizeof(PALETTEENTRY));

  if (!hLogPal) {
    return NULL;
  }

  // 得到调色板内存指针
  lpLogPal = (LPLOGPALETTE) GlobalLock(hLogPal);
  // 设置调色板信息
  lpLogPal->palVersion = 0x300;
  lpLogPal->palNumEntries = nColors;
  // 将系统的调色板拷贝到当前的逻辑调色板中
  GetSystemPaletteEntries(hDC, 0, nColors,
      (LPPALETTEENTRY)(lpLogPal->palPalEntry));
  // 创建Windows调色板
  hPal = CreatePalette(lpLogPal);
  // 释放已分配内存并删除临时对象
  GlobalUnlock(hLogPal);
  GlobalFree(hLogPal);
  ReleaseDC(NULL, hDC);
  // 返回
  return hPal;
}
// 将创建一个新的调色板，并从指定的调色板拷贝调色板内容
// 输入参数:
//   HPALETTE hPalSrc   - 需要拷贝的源调色板句柄
//
// 返回值:
//   HPALETTE    - 如果操作成功，则返回拷贝的调色板句柄
static HPALETTE CopyPalette(HPALETTE hPalSrc)
{
  // 调色板指针，临时变量
  PLOGPALETTE plogPal;
  // 声明一个调色板句柄和一个临时句柄
  HPALETTE hPalette;
  HANDLE hTemp;
  // 调色板表项数
  int iNumEntries = 0;
  // 获取调色板中的表项数
  iNumEntries = GetPaletteEntries(hPalSrc, 0, iNumEntries, NULL);

  if (iNumEntries == 0) {
    return (HPALETTE) NULL;
  }

  // 分配调色板内存，得到句柄
  hTemp = MALLOC(BYTE, sizeof(DWORD) + sizeof(PALETTEENTRY) * iNumEntries);

  if (! hTemp) {
    return (HPALETTE) NULL;
  }

  // 得到调色板的指针
  plogPal = (PLOGPALETTE) GlobalLock(hTemp);

  if (! plogPal) {
    return (HPALETTE) NULL;
  }

  // 设置调色板的信息
  plogPal->palVersion = 0x300;
  plogPal->palNumEntries = (WORD) iNumEntries;
  // 获取逻辑调色板中指定范围的调色板表项
  GetPaletteEntries(hPalSrc, 0, iNumEntries, plogPal->palPalEntry);
  // 创建一个Windows调色板
  hPalette = CreatePalette(plogPal);
  // 释放以分配的内存
  GlobalUnlock(hTemp);
  GlobalFree(hTemp);
  return hPalette;
}
// 将读取颜色表，并创建一个Windows调色板，并返回此调色板的句柄
// 输入参数:
//   LPVOID  lpvColorTable - 颜色表指针
//   LPBITMAPINFOHEADER lpBMIH  - DIB信息头指针
// 返回值:
//   HPALETTE    - 如果成功，则调色板句柄
static HPALETTE MakeDIBPalette(LPVOID lpvColorTable, LPBITMAPINFOHEADER lpBMIH)
{
  // 调色板句柄
  HPALETTE hPalette = NULL;
  LPLOGPALETTE pLogPal;
  LPRGBQUAD pDibQuad;
  // 颜色表颜色数
  int i, nColorTableEntries;

  // 设置DIB中的调色板指针
  // lpvColorTable = (LPBYTE )lpBMIH + sizeof(BITMAPINFOHEADER);
  // 计算调色板的表项数
  if (lpBMIH->biClrUsed == 0) {
    switch (lpBMIH->biBitCount) {
    case 1:
      nColorTableEntries = 2;
      break;

    case 4:
      nColorTableEntries = 16;
      break;

    case 8:
      nColorTableEntries = 256;
      break;

    case 16:
    case 24:
    case 32:
      nColorTableEntries = 0;
      break;

    default:
      break;
    }
  }
  // 否则调色板的表项数就是用到的颜色数目
  else {
    nColorTableEntries = lpBMIH->biClrUsed;
  }

  assert((nColorTableEntries >= 0) && (nColorTableEntries <= 256));

  // 如果不存在调色板，则返回FALSE
  if (nColorTableEntries == 0) {
    return 0;
  }

  // 给逻辑调色板分配内存
  pLogPal = (LPLOGPALETTE) MALLOC(BYTE, 2 * sizeof(WORD) +
      nColorTableEntries * sizeof(PALETTEENTRY));
  // 设置逻辑调色板的信息
  pLogPal->palVersion = 0x300;
  pLogPal->palNumEntries = nColorTableEntries;
  // 拷贝DIB中的颜色表到逻辑调色板
  pDibQuad = (LPRGBQUAD) lpvColorTable;

  for (i = 0; i < nColorTableEntries; i++) {
    pLogPal->palPalEntry[ i ].peRed = pDibQuad->rgbRed;
    pLogPal->palPalEntry[ i ].peGreen = pDibQuad->rgbGreen;
    pLogPal->palPalEntry[ i ].peBlue = pDibQuad->rgbBlue;
    pLogPal->palPalEntry[ i ].peFlags = 0;
    pDibQuad++;
  }

  // 创建逻辑调色板
  hPalette = CreatePalette(pLogPal);
  // 删除临时变量
  FREE(pLogPal);
  // 返回调色板句柄
  return hPalette;
}
// 绘制DIB对象
// 其中调用了StretchDIBits()或者SetDIBitsToDevice()来绘制DIB对象。
// 输出的设备由由参数hDC指定；绘制的矩形区域由参数lpDRECT指定；
// 输出DIB的区域由参数lpDIBRect指定。
//   HDC hDC            - 输出设备DC
//   const IRECT* lpDRECT     - 绘制矩形区域
//   const BITMAPINFO* lpDIBHdr    - BITMAPINFOHEADER指针
//   LPBYTE lpBits;     - DIB象素指针
//   const IRECT* lpDIBRect   - 要输出的DIB区域
//   HPALETTE pPal      - 指向DIB对象调色板的指针
// 返回值:
//   BOOL               - 绘制成功返回TRUE，否则返回FALSE。
static BOOL DrawDIB(HDC hDC, const IRECT* dRc, const BITMAPINFO* lpDIBHdr, const BYTE* lpBits, const IRECT* sRc, HPALETTE pPal)
{
  BOOL bSuccess = FALSE;   // 成功标志
  HPALETTE hPal = NULL;    // DIB调色板
  HPALETTE hOldPal = NULL; // 以前的调色板

  if (lpDIBHdr == NULL) {
    return FALSE;
  }

  //lpBits = DIBBits(lpDIBHdr);   // 找到DIB图像象素起始位置

  if (pPal != NULL) {  // 获取DIB调色板，并选中它
    hPal = (HPALETTE) pPal;
    hOldPal = SelectPalette(hDC, hPal, TRUE);   // 选中调色板
  }

  SetStretchBltMode(hDC, COLORONCOLOR);     // 设置显示模式

  // 判断是调用StretchDIBits()还是SetDIBitsToDevice()来绘制DIB对象
  if ((RCW(dRc) == RCW(sRc)) && (RCH(dRc) == RCH(sRc))) {
    // 原始大小，不用拉伸。
    bSuccess = SetDIBitsToDevice(hDC,
        dRc->l, dRc->t, RCW(dRc), RCH(dRc),
        sRc->l,  // SrcX
        DIBHeight(lpDIBHdr) - sRc->t - RCH(sRc),    // SrcY
        0,  // nStartScan
        DIBHeight(lpDIBHdr),      // nNumScans
        lpBits,  // lpBits
        lpDIBHdr,    // lpBitsInfo
        DIB_RGB_COLORS);             // wUsage
  }
  else {
    // 非原始大小，拉伸。
    bSuccess = StretchDIBits(hDC,   // hDC
        dRc->l, dRc->t, RCW(dRc), RCH(dRc),
        sRc->l, sRc->t, RCW(sRc), RCH(sRc),
        lpBits,  // lpBits
        lpDIBHdr,    // lpBitsInfo
        DIB_RGB_COLORS, SRCCOPY); // dwROP
  }

  if (hOldPal != NULL) {
    SelectPalette(hDC, hOldPal, TRUE);   // 恢复以前的调色板
  }

  return bSuccess;
}
//   将源BITMAP类pDibSrc中的数据拷贝到pDibDst中，并对相应的数据成员赋值
// 输入参数:
//   HBITMAP hBitmap   - 指向源数据的BITMAP句柄
//   CDib* pDibDst   - 指向转换目标的CDib对象指针
// 返回值:
//   LPBYTE    - 如果操作成功，则返回pDib
static LPBYTE DDBToDIB(HBITMAP hBitmap, HPALETTE hPal)
{
  // 声明一个BITMAP结构
  LPBYTE pDib;
  BITMAP bm;
  // 设备上下文
  HDC hDC;
  // 象素位数
  WORD biBitCount;
  // 调色板表项数
  int nColorTableEntries, lLineBytes;
  LPBITMAPINFOHEADER lpBMIH;
  LPBYTE lpImage;

  // 如果hBitmap句柄无效，则返回
  if (!hBitmap) {
    return 0;
  }

  // 填充图象数据到bm中，其中最后一个参数表示接收这个指定的对象的指针
  if (!GetObject(hBitmap, sizeof(BITMAP), (LPBYTE) & bm)) {
    return 0;
  }

  // 计算象素位数
  biBitCount = bm.bmPlanes * bm.bmBitsPixel;

  if (biBitCount <= 1) {
    biBitCount = 1;
  }
  else if (biBitCount <= 4) {
    biBitCount = 4;
  }
  else if (biBitCount <= 8) {
    biBitCount = 8;
  }
  else {
    biBitCount = 24;
  }

  // 计算调色板的尺寸
  // 如果biClrUsed为零，则用到的颜色数为2的biBitCount次方
  switch (biBitCount) {
  case 1:
    nColorTableEntries = 2;
    break;

  case 4:
    nColorTableEntries = 16;
    break;

  case 8:
    nColorTableEntries = 256;
    break;

  case 16:
  case 24:
  case 32:
    nColorTableEntries = 0;
    break;

  default:
    ASSERT(FALSE);
  }

  lLineBytes = WIDTHBYTES(bm.bmWidth * biBitCount);
  ASSERT((nColorTableEntries >= 0) && (nColorTableEntries <= 256));
  // 分配DIB信息头和调色板的内存
  pDib = MALLOC(BYTE, sizeof(BITMAPINFOHEADER) + 1024 + bm.bmHeight * lLineBytes);
  lpBMIH = (LPBITMAPINFOHEADER)pDib;
  lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
  lpBMIH->biWidth = bm.bmWidth;
  lpBMIH->biHeight = bm.bmHeight;
  lpBMIH->biPlanes = 1;
  lpBMIH->biBitCount = biBitCount;
  lpBMIH->biCompression = BI_RGB;
  lpBMIH->biSizeImage = 0;
  lpBMIH->biXPelsPerMeter = 0;
  lpBMIH->biYPelsPerMeter = 0;
  lpBMIH->biClrUsed = nColorTableEntries;
  lpBMIH->biClrImportant = nColorTableEntries;
  // 获得设备上下文句柄
  hDC = GetDC(NULL);

  // select and realize our palette
  // 如果没有指定调色板，则从系统中获得当前的系统调色板
  if (hPal == NULL) {
    hPal = GetSystemPalette();
  }

  hPal = SelectPalette(hDC, hPal, FALSE);
  RealizePalette(hDC);
  // 调用GetDIBits填充信息头，并获得图象数据的尺寸。注意这里图象数据指针为NULL
  GetDIBits(hDC, hBitmap, 0, (UINT) lpBMIH->biHeight, NULL, (LPBITMAPINFO) lpBMIH, DIB_RGB_COLORS);

  // 如果没有正确的获得图象数据尺寸，则重新计算
  if (lpBMIH->biSizeImage == 0) {
    lpBMIH->biSizeImage = (((bm.bmWidth * biBitCount) + 31) / 32 * 4) * bm.bmHeight;
  }

  // 分配存放图象数据的内存
  lpImage = DIBBits(pDib);

  // 调用GetDIBits加载图象数据，注意这里给出了图象数据指针
  // 如果加载图象数据不成功，则释放已经分配的内存，并返回FALSE
  if (GetDIBits(hDC, hBitmap, 0, (UINT) lpBMIH->biHeight, (LPBYTE) lpImage,
      (LPBITMAPINFO) lpBMIH, DIB_RGB_COLORS) == 0) {
    //clean up and return NULL
    FREE(pDib);
    SelectPalette(hDC, hPal, TRUE);
    RealizePalette(hDC);
    ReleaseDC(NULL, hDC);
    return 0;
  }

  // 删除临时变量
  SelectPalette(hDC, hPal, TRUE);
  RealizePalette(hDC);
  ReleaseDC(NULL, hDC);
  return pDib;
}

// 将指定矩形位置内的屏幕内容拷贝到DIB中源CDib类pDibSrc中的数据拷贝到pDibDst中
// 输入参数:
//   const IRECT* lpRect   - 需要拷贝的屏幕区域
// 返回值:
//   BOOL    - 如果操作成功，则返回pDib对象
static LPBYTE ScreenToDIB(const IRECT* lpRect)
{
  LPBYTE pDib = 0;
  // 屏幕设备上下文和内存设备上下文句柄
  HDC hScrDC, hMemDC;
  // 声明BITMAP临时句柄和以前的BITMAP句柄
  HBITMAP hBitmap, hOldBitmap;
  // 调色板句柄
  HPALETTE hPalette;
  // 获取矩形区域的坐标
  int nX, nY, nX2, nY2;
  // DIB图象的高度和宽度
  int lWidth, lHeight;
  // 屏幕分辨率
  int xScrn, yScrn;

  // 如果给定的矩形区域为空，则不进行进一步的处理
  if (0==RCH(lpRect) || 0==RCW(lpRect)) {
    return pDib;
  }

  // 得到一个屏幕设备上下文
  hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
  // 创建与屏幕设备兼容的内存设备上下文
  hMemDC = CreateCompatibleDC(hScrDC);
  // 得到矩形的区域坐标
  nX = lpRect->l;
  nY = lpRect->t;
  nX2 = lpRect->r;
  nY2 = lpRect->b;
  // 得到屏幕的分辨率，以便后面的判断处理
  xScrn = GetDeviceCaps(hScrDC, HORZRES);
  yScrn = GetDeviceCaps(hScrDC, VERTRES);

  // 判断矩形区域是否超出屏幕
  nX = MAX(nX, 0);
  nY = MAX(nY, 0);
  nX2 = MIN(nX2, xScrn);
  nY2 = MIN(nY2, xScrn);

  // 计算DIB图象的高度和宽度
  lWidth = nX2 - nX;
  lHeight = nY2 - nY;
  // 创建一个与屏幕设备上下文兼容的DDB位图
  hBitmap = CreateCompatibleBitmap(hScrDC, lWidth, lHeight);
  // 将DDB位图选入内存设备上下文
  hOldBitmap = (HBITMAP) SelectObject(hMemDC, hBitmap);
  // 将屏幕中指定区域的图象传输到内存设备上下文中
  BitBlt(hMemDC, 0, 0, lWidth, lHeight, hScrDC, nX, nY, SRCCOPY);
  // 然后将以前的图象选入，并得到屏幕区域的DDB图象句柄
  hBitmap = (HBITMAP) SelectObject(hMemDC, hOldBitmap);
  // 将临时的设备上下文删除
  DeleteDC(hScrDC);
  DeleteDC(hMemDC);
  // 得到当前系统调色板
  hPalette = GetSystemPalette();
  // 将DDB图象转换为DIB图象
  pDib = DDBToDIB(hBitmap, hPalette);

  // 删除临时对象
  DeleteObject(hPalette);
  DeleteObject(hBitmap);
  return pDib;
}

#undef FWRITE
#undef FREAD

#endif // _DIBAPI_INL_
