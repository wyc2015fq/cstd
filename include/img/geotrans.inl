
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

//角度到弧度转化的宏
#define RADIAN(angle) ((angle)*PI/180.0)

//////////////////////////////////////////////////////////////////////
// 图像几何变换函数
//////////////////////////////////////////////////////////////////////

/*************************************************************************
* 函数名称：
*   Translation()
* 参数:
*   LPBYTE lpDIBBits   - 指向源DIB图像指针
*   int  lWidth      - 源图像宽度（象素数）
*   int  lHeight     - 源图像高度（象素数）
*   int  lLineBytes  - 图像每行的字节数
*   int  lXOffset    - X轴平移量（象素数）
*   int  lYOffset    - Y轴平移量（象素数）
* 返回值:
*   BOOL               - 平移成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数用来水平移动DIB图像。函数不会改变图像的大小，移出的部分图像
* 将截去，空白部分用白色填充。
 ************************************************************************/
BOOL Translation(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int lXOffset, int lYOffset)
{
  LPBYTE lpSrc;     // 指向源图像的指针
  LPBYTE lpDst;     // 指向要复制区域的指针
  LPBYTE lpNewDIBBits;   // 指向复制图像的指针
  int i, j;        // 象素在新DIB中的坐标
  int i0, j0;      // 象素在源DIB中的坐标

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lLineBytes * lHeight);

  for (i = 0; i < lHeight; i++) {   // 每行
    for (j = 0; j < lWidth; j++) {   // 每列
      // 指向新DIB第i行，第j个象素的指针
      // 注意由于DIB中图像第一行其实保存在最后一行的位置，因此lpDst
      // 值不是lpNewDIBBits + lLineBytes* i + j，而是
      // lpNewDIBBits + lLineBytes* (lHeight - 1 - i) + j
      lpDst = lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      // 计算该象素在源DIB中的坐标
      i0 = i - lXOffset;
      j0 = j - lYOffset;

      // 判断是否在源图范围内
      if ((j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight)) {
        // 指向源DIB第i0行，第j0个象素的指针
        // 同样要注意DIB上下倒置的问题
        lpSrc = lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;
        *lpDst = *lpSrc;     // 复制象素
      }
      else {
        // 对于源图中没有的象素，直接赋值为255
        * ((LPBYTE) lpDst) = 255;
      }
    }
  }

  // 复制平移后的图像
  memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

  SAFEFREE(lpNewDIBBits);
  return TRUE;
}

/*************************************************************************
* 函数名称：Translation(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int lXOffset,
                            int lYOffset, int lLineBytes, int lDstLineBytes)
* 函数参数:
*   LPBYTE lpDIBBits，指向源DIB起始像素的指针
*      int lWidth，DIB图象的宽度
*      int lHeight，DIB图象的高度
*      int lXOffset, X方向偏移量
*      int lYOffset, Y方向偏移量
*      int lLineBytes，DIB图象的行字节数，为4的倍数
*      int lDstLineBytes，临时DIB图象的行字节数，为4的倍数
* 函数类型:BOOL
* 函数功能:该函数用来平移DIB图象
 ************************************************************************/
BOOL Translation1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int lXOffset, int lYOffset, int lDstLineBytes)
{
  int i;                 //行循环变量
  int j;                 //列循环变量
  LPBYTE lpSrcDIBBits;    //指向源像素的指针
  LPBYTE lpDstDIBBits;    //指向临时图象对应像素的指针
  LPBYTE lpDstStartBits;    //指向临时图象对应像素的指针

  lpDstStartBits = MALLOC(BYTE, lWidth * lDstLineBytes);   // 分配临时内存

  if (lpDstStartBits == NULL) { // 判断是否内存分配
    return FALSE;  // 分配内存失败
  }

  for (i = 0; i < lHeight; i++) {   // 行
    for (j = 0; j < lWidth; j++) {   // 列
      lpDstDIBBits = (LPBYTE) lpDstStartBits + lLineBytes * (lHeight - 1 - i)
          + j; // 指向新DIB第i行，第j个像素的指针

      if ((j - lYOffset >= 0) && (j - lYOffset < lWidth) &&       // 像素在源DIB中的坐标j-lXOffset
          (i - lXOffset >= 0) && (i - lXOffset < lHeight)) {    // 判断是否在源图范围内
        lpSrcDIBBits = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 -
            (i - lXOffset)) + (j - lYOffset);      // 指向源DIB第i0行，第j0个像素的指针
        *lpDstDIBBits = *lpSrcDIBBits; // 复制像素
      }
      else {
        * ((LPBYTE) lpDstDIBBits) = 255;     // 源图中没有的像素，赋为255
      }
    }
  }

  memcpy(lpDIBBits, lpDstStartBits, lLineBytes * lHeight);   // 复制图象

  // 释放内存
  SAFEFREE(lpDstStartBits);
  return TRUE;
}

/*************************************************************************
* 函数名称：
*   Mirror()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数）
*   int lHeight      - 源图像高度（象素数）
*   BOOL  bDirection   - 镜像的方向，TRUE表示水平镜像，FALSE表示垂直镜像
* 返回值:
*   BOOL               - 镜像成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数用来镜像DIB图像。可以指定镜像的方式是水平还是垂直。
 ************************************************************************/
BOOL Mirror(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    BOOL bDirection)
{
  LPBYTE lpSrc;      // 指向源图像的指针
  LPBYTE lpDst;      // 指向要复制区域的指针
  LPBYTE lpBits;      // 指向复制图像的指针
  int i;       // 循环变量
  int j;

  lpBits = MALLOC(BYTE, lLineBytes);   // 暂时分配内存，以保存一行图像

  if (lpBits == NULL) {
    return FALSE;
  }

  // 判断镜像方式
  if (bDirection) {   // 水平镜像
    // 针对图像每行进行操作
    for (i = 0; i < lHeight; i++) {
      // 针对每行图像左半部分进行操作
      for (j = 0; j < lWidth / 2; j++) {
        // 指向倒数第i行，第j个象素的指针
        lpSrc = lpDIBBits + lLineBytes * i + j;
        // 指向倒数第i行，倒数第j个象素的指针
        lpDst = lpDIBBits + lLineBytes * (i + 1) - j;

        // 备份一个象素
        *lpBits = *lpDst;

        // 将倒数第i行，第j个象素复制到倒数第i行，倒数第j个象素
        *lpDst = *lpSrc;
        // 将倒数第i行，倒数第j个象素复制到倒数第i行，第j个象素
        *lpSrc = *lpBits;
      }
    }
  }
  else {   // 垂直镜像
    // 针对上半图像进行操作
    for (i = 0; i < lHeight / 2; i++) {
      // 指向倒数第i行象素起点的指针
      lpSrc = lpDIBBits + lLineBytes * i;
      // 指向第i行象素起点的指针
      lpDst = lpDIBBits + lLineBytes * (lHeight - i - 1);

      // 备份一行，宽度为lWidth
      memcpy(lpBits, lpDst, lLineBytes);

      // 将倒数第i行象素复制到第i行
      memcpy(lpDst, lpSrc, lLineBytes);
      // 将第i行象素复制到倒数第i行
      memcpy(lpSrc, lpBits, lLineBytes);
    }
  }

  SAFEFREE(lpBits);
  return TRUE;
}

/*************************************************************************
* 函数名称：
*   Transpose()
* 参数:
*   LPBYTE lpDIB  - 指向源DIB的指针
* 返回值:
*   BOOL               - 转置成功返回TRUE，否则返回FALSE。
* 说明:
*   该函数用来转置DIB图像，即图像x、y坐标互换。函数将不会改变图像的大小，
* 但是图像的高宽将互换。
 ************************************************************************/
BOOL Transpose(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int lNewLineBytes)
{
  LPBYTE lpSrc;    // 指向源象素的指针
  LPBYTE lpDst;    // 指向转置图像对应象素的指针
  LPBYTE lpNewDIBBits;  // 指向转置图像的指针
  int i;
  int j;

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lWidth * lNewLineBytes);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  for (i = 0; i < lHeight; i++) {  // 针对图像每行进行操作
    for (j = 0; j < lWidth; j++) {  // 针对每行图像每列进行操作
      // 指向源DIB第i行，第j个象素的指针
      lpSrc = lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

      // 指向转置DIB第j行，第i个象素的指针
      // 注意此处lWidth和lHeight是源DIB的宽度和高度，应该互换
      lpDst = lpNewDIBBits + lNewLineBytes * (lWidth - 1 - j) + i;

      // 复制象素
      *lpDst = *lpSrc;
    }
  }

  // 复制转置后的图像
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lNewLineBytes);

  SAFEFREE(lpNewDIBBits);
  return TRUE;
}

/*************************************************************************
* 函数名称：
*   Zoom()
* 参数:
*   LPBYTE lpDIB  - 指向源DIB的指针
*   Float fXZoomRatio - X轴方向缩放比率
*   Float fYZoomRatio - Y轴方向缩放比率
* 返回值:
*   HGLOBAL            - 缩放成功返回新DIB句柄，否则返回NULL。
* 说明:
*   该函数用来缩放DIB图像，返回新生成DIB的句柄。
 ************************************************************************/
int Zoom(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int lNewLineBytes,
    Float fXZoomRatio, Float fYZoomRatio)
{
  int lNewWidth;   // 缩放后图像的宽度
  int lNewHeight;   // 缩放后图像的高度
  LPBYTE lpSrc;    // 指向源象素的指针
  LPBYTE lpDst;    // 指向缩放图像对应象素的指针
  LPBYTE lpNewDIBBits;

  int i;     // 循环变量（象素在新DIB中的坐标）
  int j;
  int i0;     // 象素在源DIB中的坐标
  int j0;

  // 计算缩放后的图像实际宽度
  // 此处直接加0.5是由于强制类型转换时不四舍五入，而是直接截去小数部分
  lNewWidth = (int)(lWidth * fXZoomRatio + 0.5);

  // 计算缩放后的图像高度
  lNewHeight = (int)(lHeight * fYZoomRatio + 0.5);

  // 分配内存，以保存新DIB
  lpNewDIBBits = MALLOC(BYTE, lNewLineBytes * lNewHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  for (i = 0; i < lNewHeight; i++) {   // 针对图像每行进行操作
    for (j = 0; j < lNewWidth; j++) {   // 针对图像每列进行操作
      // 指向新DIB第i行，第j个象素的指针
      // 注意此处宽度和高度是新DIB的宽度和高度
      lpDst = lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;

      // 计算该象素在源DIB中的坐标
      i0 = (int)(i / fYZoomRatio + 0.5);
      j0 = (int)(j / fXZoomRatio + 0.5);

      // 判断是否在源图范围内
      if ((j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight)) {
        // 指向源DIB第i0行，第j0个象素的指针
        lpSrc = lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;
        // 复制象素
        *lpDst = *lpSrc;
      }
      else {
        // 对于源图中没有的象素，直接赋值为255
        * ((LPBYTE) lpDst) = 255;
      }
    }
  }

  SAFEFREE(lpNewDIBBits);
  return 0;
}

/*************************************************************************
* 函数名称：
*   Rotate()
* 参数:
*   LPBYTE lpDIB  - 指向源DIB的指针
*   int iRotateAngle - 旋转的角度（0-360度）
* 返回值:
*   HGLOBAL            - 旋转成功返回新DIB句柄，否则返回NULL。
* 说明:
*   该函数用来以图像中心为中心旋转DIB图像，返回新生成DIB的句柄。
* 调用该函数会自动扩大图像以显示所有的象素。函数中采用最邻近插
* 值算法进行插值。
 ************************************************************************/
int Rotate(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int lNewLineBytes,
    int iRotateAngle)
{
  int lNewWidth;    // 旋转后图像的宽度
  int lNewHeight;   // 旋转后图像的高度
  LPBYTE lpSrc;    // 指向源象素的指针
  LPBYTE lpDst;    // 指向旋转图像对应象素的指针
  LPBYTE lpNewDIBBits; // 指向旋转图像的指针

  int i;     // 循环变量（象素在新DIB中的坐标）
  int j;
  int i0;     // 象素在源DIB中的坐标
  int j0;

  Float fRotateAngle;  // 旋转角度（弧度）
  Float fSina, fCosa;  // 旋转角度的正弦和余弦
  // 源图四个角的坐标（以图像中心为坐标系原点）
  Float fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;
  // 旋转后四个角的坐标（以图像中心为坐标系原点）
  Float fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;
  Float f1, f2;

  // 将旋转角度从度转换到弧度
  fRotateAngle = (Float) RADIAN(iRotateAngle);
  fSina = (Float) sin((Float) fRotateAngle);       // 计算旋转角度的正弦
  fCosa = (Float) cos((Float) fRotateAngle);       // 计算旋转角度的余弦

  // 计算原图的四个角的坐标（以图像中心为坐标系原点）
  fSrcX1 = (Float)(- (lWidth - 1) / 2);
  fSrcY1 = (Float)((lHeight - 1) / 2);
  fSrcX2 = (Float)((lWidth - 1) / 2);
  fSrcY2 = (Float)((lHeight - 1) / 2);
  fSrcX3 = (Float)(- (lWidth - 1) / 2);
  fSrcY3 = (Float)(- (lHeight - 1) / 2);
  fSrcX4 = (Float)((lWidth - 1) / 2);
  fSrcY4 = (Float)(- (lHeight - 1) / 2);

  // 计算新图四个角的坐标（以图像中心为坐标系原点）
  fDstX1 = fCosa * fSrcX1 + fSina * fSrcY1;
  fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
  fDstX2 = fCosa * fSrcX2 + fSina * fSrcY2;
  fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
  fDstX3 = fCosa * fSrcX3 + fSina * fSrcY3;
  fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
  fDstX4 = fCosa * fSrcX4 + fSina * fSrcY4;
  fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

  // 计算旋转后的图像实际宽度
  lNewWidth = (int)(MAX(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);

  // 计算旋转后的图像高度
  lNewHeight = (int)(MAX(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

  // 两个常数，这样不用以后每次都计算了
  f1 = (Float)(-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina
      + 0.5 * (lWidth - 1));
  f2 = (Float)(0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa
      + 0.5 * (lHeight - 1));

  // 分配内存，以保存新DIB
  lpNewDIBBits = (LPBYTE) MALLOC(BYTE, lNewLineBytes * lNewHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  for (i = 0; i < lNewHeight; i++) {   // 针对图像每行进行操作
    for (j = 0; j < lNewWidth; j++) {   // 针对图像每列进行操作
      // 指向新DIB第i行，第j个象素的指针
      // 注意此处宽度和高度是新DIB的宽度和高度
      lpDst = lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;

      // 计算该象素在源DIB中的坐标
      i0 = (int)(-((Float) j) * fSina + ((Float) i) * fCosa + f2 + 0.5);
      j0 = (int)(((Float) j) * fCosa + ((Float) i) * fSina + f1 + 0.5);

      // 判断是否在源图范围内
      if ((j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight)) {
        // 指向源DIB第i0行，第j0个象素的指针
        lpSrc = lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;

        // 复制象素
        *lpDst = *lpSrc;
      }
      else {
        // 对于源图中没有的象素，直接赋值为255
        * ((LPBYTE) lpDst) = 255;
      }
    }
  }

  return 0;
}

/*************************************************************************
* 函数名称：
*   Interpolation()
* 参数:
*   LPBYTE lpDIBBits   - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数）
*   int lHeight      - 源图像高度（象素数）
*   Float x            - 插值元素的x坐标
*   Float y            - 插值元素的y坐标
* 返回值:
*   BYTE      - 返回插值计算结果。
* 说明:
*   该函数利用双线性插值算法来估算象素值。对于超出图像范围的象素，直接返回255。
 ************************************************************************/
BYTE Interpolation(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    Float x, Float y)
{
  // 四个最临近象素的坐标(i1, j1), (i2, j1), (i1, j2), (i2, j2)
  int i1, i2;
  int j1, j2;

  BYTE f1, f2, f3, f4; // 四个最临近象素值
  BYTE f12, f34;  // 二个插值中间值

  // 定义一个值，当象素坐标相差小于改值时认为坐标相同
  Float EXP;

  EXP = (Float) 0.0001;

  // 计算四个最临近象素的坐标
  i1 = (int) x;
  i2 = i1 + 1;
  j1 = (int) y;
  j2 = j1 + 1;

  // 根据不同情况分别处理
  if ((x < 0) || (x > lWidth - 1) || (y < 0) || (y > lHeight - 1)) {
    return 255;  // 要计算的点不在源图范围内，直接返回255。
  }
  else {
    if (fabs(x - lWidth + 1) <= EXP) {
      // 要计算的点在图像右边缘上
      if (fabs(y - lHeight + 1) <= EXP) {
        // 要计算的点正好是图像最右下角那一个象素，直接返回该点象素值
        f1 = *((LPBYTE) lpDIBBits + lLineBytes *
            (lHeight - 1 - j1) + i1);
        return f1;
      }
      else {
        // 在图像右边缘上且不是最后一点，直接一次插值即可
        f1 = *((LPBYTE) lpDIBBits + lLineBytes *
            (lHeight - 1 - j1) + i1);
        f3 = *((LPBYTE) lpDIBBits + lLineBytes *
            (lHeight - 1 - j1) + i2);

        // 返回插值结果
        return ((BYTE)(f1 + (y - j1) * (f3 - f1)));
      }
    }
    else if (fabs(y - lHeight + 1) <= EXP) {
      // 要计算的点在图像下边缘上且不是最后一点，直接一次插值即可
      f1 = *((LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j1) + i1);
      f2 = *((LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j2) + i1);

      // 返回插值结果
      return ((BYTE)(f1 + (x - i1) * (f2 - f1)));
    }
    else {
      // 计算四个最临近象素值
      f1 = *((LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j1) + i1);
      f2 = *((LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j2) + i1);
      f3 = *((LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j1) + i2);
      f4 = *((LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - j2) + i2);

      // 插值1
      f12 = (BYTE)(f1 + (x - i1) * (f2 - f1));
      // 插值2
      f34 = (BYTE)(f3 + (x - i1) * (f4 - f3));
      // 插值3
      return ((BYTE)(f12 + (y - j1) * (f34 - f12)));
    }
  }
}

/*************************************************************************
* 函数名称：
*   Rotate2()
* 参数:
*   LPBYTE lpDIB  - 指向源DIB的指针
*   int iRotateAngle - 旋转的角度（0-360度）
* 返回值:
*   HGLOBAL            - 旋转成功返回新DIB句柄，否则返回NULL。
* 说明:
*   该函数用来以图像中心为中心旋转DIB图像，返回新生成DIB的句柄。
* 调用该函数会自动扩大图像以显示所有的象素。函数中采用双线性插
* 值算法进行插值。
 ************************************************************************/
int Rotate2(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int lNewLineBytes,
    int iRotateAngle)
{
  int lNewWidth;   // 旋转后图像的宽度
  int lNewHeight;   // 旋转后图像的高度
  LPBYTE lpDst;    // 指向旋转图像对应象素的指针
  LPBYTE lpNewDIBBits; // 指向旋转图像的指针

  int i;     // 循环变量（象素在新DIB中的坐标）
  int j;
  Float i0;     // 象素在源DIB中的坐标
  Float j0;

  Float fRotateAngle;  // 旋转角度（弧度）
  Float fSina, fCosa;  // 旋转角度的正弦和余弦
  // 源图四个角的坐标（以图像中心为坐标系原点）
  Float fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;
  // 旋转后四个角的坐标（以图像中心为坐标系原点）
  Float fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;
  Float f1, f2;

  // 将旋转角度从度转换到弧度
  fRotateAngle = (Float) RADIAN(iRotateAngle);

  fSina = (Float) sin((Float) fRotateAngle);       // 计算旋转角度的正弦
  fCosa = (Float) cos((Float) fRotateAngle);       // 计算旋转角度的余弦

  // 计算原图的四个角的坐标（以图像中心为坐标系原点）
  fSrcX1 = (Float)(- (lWidth - 1) / 2);
  fSrcY1 = (Float)((lHeight - 1) / 2);
  fSrcX2 = (Float)((lWidth - 1) / 2);
  fSrcY2 = (Float)((lHeight - 1) / 2);
  fSrcX3 = (Float)(- (lWidth - 1) / 2);
  fSrcY3 = (Float)(- (lHeight - 1) / 2);
  fSrcX4 = (Float)((lWidth - 1) / 2);
  fSrcY4 = (Float)(- (lHeight - 1) / 2);

  // 计算新图四个角的坐标（以图像中心为坐标系原点）
  fDstX1 = fCosa * fSrcX1 + fSina * fSrcY1;
  fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
  fDstX2 = fCosa * fSrcX2 + fSina * fSrcY2;
  fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
  fDstX3 = fCosa * fSrcX3 + fSina * fSrcY3;
  fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
  fDstX4 = fCosa * fSrcX4 + fSina * fSrcY4;
  fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

  // 计算旋转后的图像实际宽度
  lNewWidth = (int)(MAX(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);

  // 计算旋转后的图像高度
  lNewHeight = (int)(MAX(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

  f1 = (Float)(-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina
      + 0.5 * (lWidth - 1));
  f2 = (Float)(0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa
      + 0.5 * (lHeight - 1));

  // 分配内存，以保存新DIB
  lpNewDIBBits = (LPBYTE) MALLOC(BYTE, lNewLineBytes * lNewHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  for (i = 0; i < lNewHeight; i++) {   // 针对图像每行进行操作
    for (j = 0; j < lNewWidth; j++) {   // 针对图像每列进行操作
      // 指向新DIB第i行，第j个象素的指针
      // 注意此处宽度和高度是新DIB的宽度和高度
      lpDst = lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;

      // 计算该象素在源DIB中的坐标
      i0 = -((Float) j) * fSina + ((Float) i) * fCosa + f2;
      j0 = ((Float) j) * fCosa + ((Float) i) * fSina + f1;

      // 利用双线性插值算法来估算象素值
      *lpDst = Interpolation(lHeight, lWidth, lpDIBBits, lNewLineBytes, j0, i0);
    }
  }

  return 0;
}

/*************************************************************************
* 函数名称：Rotate(LPBYTE lpSrcDib, int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
                   int lLineBytes, WORD palSize, int lDstWidth,
                   int lDstHeight, int lDstLineBytes, Float    fSina, Float fCosa)
* 函数参数:
*   LPBYTE lpDIBBits   -指向源DIB的起始像素的指针
*   int lWidth         -源DIB图象宽度
*   int lHeight        -源DIB图象高度
*   int lLineBytes     -源DIB图象字节宽度（4的倍数）
*   int lDstWidth      -目标图象宽度
*   int lDstHeight     -目标DIB图象高度
*   int lDstLineBytes  -目标DIB图象行字节数（4的倍数）
*   Float fSina        -旋转角的余弦，说明：为了避免两次求取正余弦，这里作为两个函数参数来用
*   Float fCosa        -旋转角的正弦
* 函数类型: LPBYTE
* 函数功能: 用来旋转DIB图象
 ************************************************************************/
BOOL Rotate1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int lDstHeight, int lDstWidth, LPBYTE lpDstDib, int lDstLineBytes,
    Float fSina, Float fCosa)
{
  Float varFloat1;       //浮点参数变量1
  Float varFloat2;       //浮点参数变量2
  int i, i1;             //行循环变量
  int j, j1;             //列循环变量
  LPBYTE lpSrcDIBBits;   //指向源像素的指针
  LPBYTE lpDstDIBBits;   //指向临时图象对应像素的指针
  LPBYTE lpDstStartBits; //指向临时图象对应像素的指针

  varFloat1 = (Float)(-0.5 * (lDstWidth - 1) * fCosa - 0.5 * (lDstHeight - 1) * fSina         // 将经常用到的两个常数事先求出，以便作为常数使用
      + 0.5 * (lDstWidth - 1));
  varFloat2 = (Float)(0.5 * (lDstWidth - 1) * fSina - 0.5 * (lDstHeight - 1) * fCosa
      + 0.5 * (lDstHeight - 1));
  // 求像素起始位置, 作用如同FindDIBBits(gCo.lpSrcDib)，这里尝试使用了这种方法，以避免对全局函数的调用
  lpDstStartBits = lpDstDib;

  for (i = 0; i < lDstHeight; i++) {   // 行操作
    for (j = 0; j < lDstWidth; j++) {   // 列操作
      lpDstDIBBits = (LPBYTE) lpDstStartBits + lDstLineBytes * (lDstHeight - 1 - i) + j;     // 指向新DIB第i行，第j个像素的指针
      i1 = (int)(-((Float) j) * fSina + ((Float) i) * fCosa + varFloat2 + 0.5);              // 计算该像素在源DIB中的坐标
      j1 = (int)(((Float) j) * fCosa + ((Float) i) * fSina + varFloat1 + 0.5);

      if ((j1 >= 0) && (j1 < lWidth) && (i1 >= 0) && (i1 < lHeight)) {           // 判断是否在源图内
        lpSrcDIBBits = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i1) + j1;     // 指向源DIB第i0行，第j0个像素的指针
        *lpDstDIBBits = *lpSrcDIBBits; // 复制像素
      }
      else {
        * ((LPBYTE) lpDstDIBBits) = 255;     // 源图中不存在的像素，赋为255
      }
    }
  }

  return TRUE;
}

/*************************************************************************
 *
* 函数名称：EigenvalueVector(int rank, Float* pMainCross, Float* pHypoCross,
                  Float* pMatrix, Float Precision, int MaxT)
* 函数参数:
*   int    rank ，矩阵A的阶数
*   Float     *pMainCross  ，对称三角阵中的主对角元素的指针，返回时存放A的特征值
*   Float  *pHypoCross  ，对称三角阵中的次对角元素的指针
*     Float  *pMatrix  ，对称矩阵A的特征向量的指针
*   Float Precision ，控制精度
*   int MaxT  ， 最大迭代次数
* 函数类型:BOOL
* 函数功能:用计算实对称三角矩阵的全部特征值以及相应的特征向量
 ************************************************************************/
BOOL EigenvalueVector(int rank, Float* pMainCross, Float* pHypoCross,
    Float* pMatrix, Float Precision, int MaxT)
{
  int i, j, k, m, it, u, v; // 变量声明
  Float d, f, h, g, p, r, e, s;

  pHypoCross[ rank - 1 ] = 0.0; // 初始化
  d = 0.0;
  f = 0.0;

  for (j = 0; j <= rank - 1; j++) {                              //  迭代精度的控制
    it = 0;
    h = Precision * (fabs(pMainCross[ j ]) + fabs(pHypoCross[ j ]));

    if (h > d) {
      d = h;
    }

    m = j;

    while ((m <= rank - 1) && (fabs(pHypoCross[ m ]) > d)) {
      m = m + 1;
    }

    if (m != j) {
      // 迭代求矩阵A的特征值和特征向量
      do {
        // 超过迭代次数，迭代失败
        if (it == MaxT) {
          return (FALSE);
        }

        it = it + 1;
        g = pMainCross[ j ];
        p = (pMainCross[ j + 1 ] - g) / (2.0 * pHypoCross[ j ]);
        r = sqrt(p * p + 1.0);

        // 如果p大于0
        if (p >= 0.0) {
          pMainCross[ j ] = pHypoCross[ j ] / (p + r);
        }
        else {
          pMainCross[ j ] = pHypoCross[ j ] / (p - r);
        }

        h = g - pMainCross[ j ];

        //  计算主对角线的元素
        for (i = j + 1; i <= rank - 1; i++) {
          pMainCross[ i ] = pMainCross[ i ] - h;
        }

        // 赋值
        f = f + h;
        p = pMainCross[ m ];
        e = 1.0;
        s = 0.0;

        for (i = m - 1; i >= j; i--) {
          g = e * pHypoCross[ i ];
          h = e * p;

          //  主对角线元素的绝对值是否大于次对角线元素的
          if (fabs(p) >= fabs(pHypoCross[ i ])) {
            e = pHypoCross[ i ] / p;
            r = sqrt(e * e + 1.0);
            pHypoCross[ i + 1 ] = s * p * r;
            s = e / r;
            e = 1.0 / r;
          }
          else {
            e = p / pHypoCross[ i ];
            r = sqrt(e * e + 1.0);
            pHypoCross[ i + 1 ] = s * pHypoCross[ i ] * r;
            s = 1.0 / r;
            e = e / r;
          }

          p = e * pMainCross[ i ] - s * g;
          pMainCross[ i + 1 ] = h + s * (e * g + s * pMainCross[ i ]);

          // 重新存储特征向量
          for (k = 0; k <= rank - 1; k++) {
            u = k * rank + i + 1;
            v = u - 1;
            h = pMatrix[ u ];
            pMatrix[ u ] = s * pMatrix[ v ] + e * h;
            pMatrix[ v ] = e * pMatrix[ v ] - s * h;
          }

        }

        // 将主对角线和次对角线元素重新赋值
        pHypoCross[ j ] = s * p;
        pMainCross[ j ] = e * p;

      }
      while (fabs(pHypoCross[ j ]) > d);
    }

    pMainCross[ j ] = pMainCross[ j ] + f;
  }

  for (i = 0; i <= rank - 1; i++) {   // 返回A的特征值
    k = i;
    p = pMainCross[ i ];

    if (i + 1 <= rank - 1) {   // 将A特征值赋给p
      j = i + 1;

      while ((j <= rank - 1) && (pMainCross[ j ] <= p)) {
        k = j;
        p = pMainCross[ j ];
        j = j + 1;
      }
    }

    if (k != i) {   // 存储A的特征值和特征向量
      pMainCross[ k ] = pMainCross[ i ];
      pMainCross[ i ] = p;

      for (j = 0; j <= rank - 1; j++) {
        u = j * rank + i;
        v = j * rank + k;
        p = pMatrix[ u ];
        pMatrix[ u ] = pMatrix[ v ];
        pMatrix[ v ] = p;
      }
    }
  }

  return (TRUE);   // 返回
}

/*************************************************************************
* 函数名称：Mirror(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
* 函数参数:
*   LPBYTE lpDIBBits, 指向DIB起始像素的指针
*   int   lWidth，DIB图象的宽度
*   int   lHeight，DIB图象的高度
*   int   lLineBytes，DIB图象的行字节数，为4的倍数
* 函数类型:BOOL
* 函数功能:该函数用来镜像DIB图象，本程序只实现了水平镜像，垂直镜像的原理书中也谈到。 很容易实现
 ************************************************************************/
BOOL Mirror1(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  int i;               // 行循环变量
  int j;               // 列循环变量
  LPBYTE lpSrcDIBBits; // 指向源像素的指针
  LPBYTE lpDstDIBBits; // 指向临时图象对应像素的指针
  LPBYTE lpBits;    // 指向中间像素的指针，当复制图象时，提供临时的像素内存空间
  lpDstDIBBits = MALLOC(BYTE, lLineBytes);   // 分配临时内存保存行图象

  if (lpDstDIBBits == NULL) {
    return FALSE;    // 分配内存失败
  }

  for (i = 0; i < lHeight; i++) {   // 水平镜像，针对图象每行进行操作
    for (j = 0; j < lWidth / 2; j++) {   // 针对每行图象左半部分进行操作
      lpSrcDIBBits = (LPBYTE) lpDIBBits + lLineBytes * i + j;   // 指向倒数第i行，第j个像素的指针
      lpBits = (LPBYTE) lpDIBBits + lLineBytes * (i + 1) - j;     // 指向倒数第i+1行，倒数第j个像素的指针
      *lpDstDIBBits = *lpBits; //保存中间像素
      *lpBits = *lpSrcDIBBits; // 将倒数第i行，第j个像素复制到倒数第i行，倒数第j个像素
      *lpSrcDIBBits = *lpDstDIBBits; // 将倒数第i行，倒数第j个像素复制到倒数第i行，第j个像素
    }
  }

  // 释放内存
  SAFEFREE(lpDstDIBBits);
  return TRUE;
}
/*************************************************************************
* 函数名称：
*   DisK_L(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
* 函数参数:
*   LPBYTE lpDIBBits, 指向源DIB图象指针
*   int lWidth, 源DIB图象宽度
*   int lHeight, 源DIB图象高度
*   int lLineBytes, 源DIB图象的行字节数，为4的倍数
* 函数类型:BOOL
* 函数功能:用来对图象进行旋转
 ************************************************************************/
BOOL DisK_L(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  Float Precision = 0.000001; // 规定迭代的计算精度
  LPBYTE lpSrcUnChr;    //指向像素的指针
  int i, j,  // 循环变量
      lMaxRange,  // 经过变换后图象最大可能范围
      AverEx, AverEy,  //  目标坐标均值
      ToaCount; //  目标总的像素数
  Float Matr4C[ 2 ][ 2 ],  // 坐标值的协方差矩阵
        QMatrix[ 2 ][ 2 ],  // 存放协方差矩阵的特征向量
        MainCross[ 2 ], HypoCross[ 2 ],  //  三对角阵的主对角和次对角线元素
        dTemp; // 临时变量
  int lTempI, lTempJ;
  Float* F;

  if (lWidth > lHeight) {   // 估计图象经过旋转后可能最大的宽度和高度
    lMaxRange = lWidth;
  }
  else {
    lMaxRange = lHeight;
  }

  AverEx = 0; // 初始化
  AverEy = 0;
  ToaCount = 0;
  Matr4C[ 0 ][ 0 ] = Matr4C[ 0 ][ 1 ] = Matr4C[ 1 ][ 0 ] = Matr4C[ 1 ][ 1 ] = 0.0;

  F = MALLOC(Float, lWidth * lHeight);   // 分配内存

  for (i = 0; i < lHeight; i++) {   // 行
    for (j = 0; j < lWidth; j++) {   // 列
      F[ i * lWidth + j ] = 255; // 给旋转后坐标轴的每个点赋零值
      // 指向位图i行j列像素的指针
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * i + j;

      // 值小于255（非背景色白色）的像素认为目标的一部分
      // 并将其坐标值x和y看作二维随机矢量
      if ((*lpSrcUnChr) < 255) {

        AverEx = AverEx + i; // 属于目标像素的Y坐标和X坐标累计值
        AverEy = AverEy + j;
        ToaCount++; // 目标总的像素数加一

        // 随机矢量协方差矩阵的累计值
        Matr4C[ 0 ][ 0 ] = Matr4C[ 0 ][ 0 ] + i * i;
        Matr4C[ 0 ][ 1 ] = Matr4C[ 0 ][ 1 ] + i * j;
        Matr4C[ 1 ][ 0 ] = Matr4C[ 1 ][ 0 ] + j * i;
        Matr4C[ 1 ][ 1 ] = Matr4C[ 1 ][ 1 ] + j * j;
      }
    }
  }

  AverEx = AverEx / ToaCount; // 计算随机矢量的均值
  AverEy = AverEy / ToaCount;

  Matr4C[ 0 ][ 0 ] = Matr4C[ 0 ][ 0 ] / ToaCount - AverEx * AverEx; //  计算随机矢量的协方差矩阵
  Matr4C[ 0 ][ 1 ] = Matr4C[ 0 ][ 1 ] / ToaCount - AverEx * AverEy;
  Matr4C[ 1 ][ 0 ] = Matr4C[ 1 ][ 0 ] / ToaCount - AverEx * AverEy;
  Matr4C[ 1 ][ 1 ] = Matr4C[ 1 ][ 1 ] / ToaCount - AverEy * AverEy;

  ThreeCross(*Matr4C, 2, *QMatrix, MainCross, HypoCross);   // 将协方差矩阵化作三对角对称阵
  EigenvalueVector(2, MainCross, HypoCross, *Matr4C, Precision, 50);   // 求协方差矩阵的特征值和特征矢向量

  dTemp = Matr4C[ 0 ][ 1 ]; // 将特征列向量转化称特征列向量
  Matr4C[ 0 ][ 1 ] = Matr4C[ 1 ][ 0 ];
  Matr4C[ 1 ][ 0 ] = dTemp;

  for (i = 0; i <= 1; i++) { // 对特征列向量进行归一化
    dTemp = pow(Matr4C[ i ][ 0 ], 2) + pow(Matr4C[ i ][ 1 ], 2);
    dTemp = sqrt(dTemp);
    Matr4C[ i ][ 0 ] = Matr4C[ i ][ 0 ] / dTemp;
    Matr4C[ i ][ 1 ] = Matr4C[ i ][ 1 ] / dTemp;
  }

  // 查找经离散K-L变换后的坐标点在原坐标系中的坐标
  for (i = -lMaxRange + 1; i < lMaxRange; i++) {
    for (j = -lMaxRange + 1; j < lMaxRange; j++) {
      //  将新坐标值映射到旧的坐标系
      int Cx = (int)(i * Matr4C[ 0 ][ 0 ] - j * Matr4C[ 0 ][ 1 ]) + AverEx;
      int Cy = (int)(-i * Matr4C[ 1 ][ 0 ] + j * Matr4C[ 1 ][ 1 ]) + AverEy;

      //  映射值是否属于源图象
      if (Cx >= 0 && Cx < lHeight && Cy >= 0 && Cy < lWidth) {
        lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * Cx + Cy;

        // 映射值是否属于原来的目标
        if (*(lpSrcUnChr) < 255) {
          //  将新坐标系原点平移到中心，以便显示
          lTempI = (int)(lHeight / 2) + j;
          lTempJ = (int)(lWidth / 2) + i;

          // 看如果能够进行显示，赋值给数组，进行存储
          if (lTempI >= 0 && lTempI < lHeight && lTempJ >= 0 && lTempJ < lWidth) {
            F[ lTempJ + (lTempI) * lWidth ] = *(lpSrcUnChr);
          }
        }
      }
    }
  }

  for (i = 0; i < lMaxRange; i++) {   // 行
    for (j = 0; j < lMaxRange; j++) {   // 列
      dTemp = F[ i * lMaxRange + j ]; // 离散K-L变换后的像素值
      // 指向位图i行j列像素的指针
      lpSrcUnChr = (LPBYTE) lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
      * (lpSrcUnChr) = (BYTE)(dTemp);        // 更新源图象
    }
  }

  SAFEFREE(F);   // 释放内存
  return TRUE; // 返回
}
