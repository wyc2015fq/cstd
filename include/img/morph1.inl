// ************************************************************************
//  文件名：morph.cpp
//
//  图像形态学变换API函数库：
//
//  ErosionBlock()  - 图像腐蚀
//  DilationBlock() - 图像膨胀
//  OpenBlock()     - 图像开运算
//  Closelock()    - 图像闭运算
//  Thining()  - 图像细化
//
// ************************************************************************

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

/*************************************************************************
* 函数名称：
*   ErosionBlock()
* 参数:
*   LPBYTE lpDIBBits       - 指向源DIB图像指针
*   int lWidth           - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight          - 源图像高度（象素数）
*   int  nMode            - 腐蚀方式：0表示水平方向，1垂直方向，2自定义结构元素。
*   int  structure[3][3]  - 自定义的3×3结构元素。
* 返回值:
*   BOOL                  - 腐蚀成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行腐蚀运算。结构元素为水平方向或垂直方向的三个点，
* 中间点位于原点；或者由用户自己定义3×3的结构元素。
* 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/
BOOL ErosionBlock(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nMode , int structure[ 3 ][ 3 ])
{
  LPBYTE lpSrc;   // 指向源图像的指针
  LPBYTE lpDst;   // 指向缓存图像的指针
  LPBYTE lpNewDIBBits; // 指向缓存DIB图像的指针
  int i, j, n, m;   //循环变量
  BYTE pixel; //像素值

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  if (nMode == 0) {
    //使用水平方向的结构元素进行腐蚀
    for (j = 0; j < lHeight; j++) {
      for (i = 1; i < lWidth - 1; i++) {
        // 由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边
        // 的两列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = lpDIBBits + lWidth * j + i;
        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = lpNewDIBBits + lWidth * j + i;
        //取得当前指针处的像素值，注意要转换为BYTE型
        pixel = (BYTE) * lpSrc;

        //目标图像中含有0和255外的其它灰度值
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //目标图像中的当前点先赋成黑色
        *lpDst = (BYTE) 0;

        //如果源图像中当前点自身或者左右有一个点不是黑色，
        //则将目标图像中的当前点赋成白色
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + n - 1);

          if (pixel == 255) {
            *lpDst = (BYTE) 255;
            break;
          }
        }
      }
    }
  }
  else if (nMode == 1) {
    //使用垂直方向的结构元素进行腐蚀
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // 由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边
        // 的两列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = lpDIBBits + lWidth * j + i;
        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = lpNewDIBBits + lWidth * j + i;
        //取得当前指针处的像素值，注意要转换为BYTE型
        pixel = (BYTE) * lpSrc;

        //目标图像中含有0和255外的其它灰度值
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //目标图像中的当前点先赋成黑色
        *lpDst = (BYTE) 0;

        //如果源图像中当前点自身或者上下有一个点不是黑色，
        //则将目标图像中的当前点赋成白色
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + (n - 1) * lWidth);

          if (pixel == 255) {
            *lpDst = (BYTE) 255;
            break;
          }
        }
      }
    }
  }
  else {
    //使用自定义的结构元素进行腐蚀
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // 由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边
        // 的两列像素和最上边和最下边的两列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = lpDIBBits + lWidth * j + i;
        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = lpNewDIBBits + lWidth * j + i;
        //取得当前指针处的像素值，注意要转换为BYTE型
        pixel = (BYTE) * lpSrc;

        //目标图像中含有0和255外的其它灰度值
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //目标图像中的当前点先赋成黑色
        *lpDst = (BYTE) 0;

        //如果原图像中对应结构元素中为黑色的那些点中有一个不是黑色，
        //则将目标图像中的当前点赋成白色
        //注意在DIB图像中内容是上下倒置的
        for (m = 0; m < 3; m++) {
          for (n = 0; n < 3; n++) {
            if (structure[ m ][ n ] == -1) {
              continue;
            }

            pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));

            if (pixel == 255) {
              *lpDst = (BYTE) 255;
              break;
            }
          }
        }
      }
    }
  }

  // 复制腐蚀后的图像
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits);

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   DilationBlock()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
*   int  nMode  - 膨胀方式：0表示水平方向，1垂直方向，2自定义结构元素。
*  int  structure[3][3] - 自定义的3×3结构元素。
* 返回值:
*   BOOL              - 膨胀成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行膨胀运算。结构元素为水平方向或垂直方向的三个点，
* 中间点位于原点；或者由用户自己定义3×3的结构元素。
* 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/
BOOL DilationBlock(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nMode, int structure[ 3 ][ 3 ])
{
  LPBYTE lpSrc;   // 指向源图像的指针
  LPBYTE lpDst;   // 指向缓存图像的指针
  LPBYTE lpNewDIBBits; // 指向缓存DIB图像的指针
  int i, j, m, n;   // 循环变量
  BYTE pixel; // 像素值

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  if (nMode == 0) {
    //使用水平方向的结构元素进行膨胀
    for (j = 0; j < lHeight; j++) {
      for (i = 1; i < lWidth - 1; i++) {
        // 由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边
        // 的两列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = lpDIBBits + lWidth * j + i;
        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = lpNewDIBBits + lWidth * j + i;
        //取得当前指针处的像素值，注意要转换为BYTE型
        pixel = (BYTE) * lpSrc;

        //目标图像中含有0和255外的其它灰度值
        if (pixel != 255 && pixel != 0) {
          return FALSE;
        }

        //目标图像中的当前点先赋成白色
        *lpDst = (BYTE) 255;

        //源图像中当前点自身或者左右只要有一个点是黑色，
        //则将目标图像中的当前点赋成黑色
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + n - 1);

          if (pixel == 0) {
            *lpDst = (BYTE) 0;
            break;
          }
        }
      }
    }
  }
  else if (nMode == 1) {
    //使用垂直方向的结构元素进行膨胀
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // 由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边
        // 的两列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = lpDIBBits + lWidth * j + i;
        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = lpNewDIBBits + lWidth * j + i;
        //取得当前指针处的像素值，注意要转换为BYTE型
        pixel = (BYTE) * lpSrc;

        //目标图像中含有0和255外的其它灰度值
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //目标图像中的当前点先赋成白色
        *lpDst = (BYTE) 255;

        //源图像中当前点自身或者上下只要有一个点是黑色，
        //则将目标图像中的当前点赋成黑色
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + (n - 1) * lWidth);

          if (pixel == 0) {
            *lpDst = (BYTE) 0;
            break;
          }
        }
      }
    }
  }
  else {
    //使用自定义的结构元素进行膨胀
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // 由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边
        // 的两列像素和最上边和最下边的两列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = lpDIBBits + lWidth * j + i;
        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = lpNewDIBBits + lWidth * j + i;
        //取得当前指针处的像素值，注意要转换为BYTE型
        pixel = (BYTE) * lpSrc;

        //目标图像中含有0和255外的其它灰度值
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //目标图像中的当前点先赋成白色
        *lpDst = (BYTE) 255;

        //原图像中对应结构元素中为黑色的那些点中只要有一个是黑色，
        //则将目标图像中的当前点赋成黑色
        //注意在DIB图像中内容是上下倒置的
        for (m = 0; m < 3; m++) {
          for (n = 0; n < 3; n++) {
            if (structure[ m ][ n ] == -1) {
              continue;
            }

            pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));

            if (pixel == 0) {
              *lpDst = (BYTE) 0;
              break;
            }
          }
        }
      }
    }
  }

  // 复制膨胀后的图像
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits);

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   OpenBlock()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
*   int  nMode  - 开运算方式：0表示水平方向，1垂直方向，2自定义结构元素。
*  int  structure[3][3] - 自定义的3×3结构元素。
* 返回值:
*   BOOL              - 开运算成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行开运算。结构元素为水平方向或垂直方向的三个点，
* 中间点位于原点；或者由用户自己定义3×3的结构元素。
* 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/
BOOL OpenBlock(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nMode, int structure[ 3 ][ 3 ])
{
  LPBYTE lpSrc;   // 指向源图像的指针
  LPBYTE lpDst;   // 指向缓存图像的指针
  LPBYTE lpNewDIBBits; // 指向缓存DIB图像的指针
  int i, j, m, n;   //循环变量
  BYTE pixel; //像素值

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  if (nMode == 0) {
    //使用水平方向的结构元素进行腐蚀
    for (j = 0; j < lHeight; j++) {
      for (i = 1; i < lWidth - 1; i++) {
        // 由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边
        // 的两列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = lpDIBBits + lWidth * j + i;
        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = lpNewDIBBits + lWidth * j + i;
        //取得当前指针处的像素值，注意要转换为BYTE型
        pixel = (BYTE) * lpSrc;

        //目标图像中含有0和255外的其它灰度值
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //目标图像中的当前点先赋成黑色
        *lpDst = (BYTE) 0;

        //如果源图像中当前点自身或者左右有一个点不是黑色，
        //则将目标图像中的当前点赋成白色
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + n - 1);

          if (pixel == 255) {
            *lpDst = (BYTE) 255;
            break;
          }
        }
      }
    }
  }
  else if (nMode == 1) {
    //使用垂直方向的结构元素进行腐蚀
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // 由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边
        // 的两列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = lpDIBBits + lWidth * j + i;
        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = lpNewDIBBits + lWidth * j + i;
        //取得当前指针处的像素值，注意要转换为BYTE型
        pixel = (BYTE) * lpSrc;

        //目标图像中含有0和255外的其它灰度值
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //目标图像中的当前点先赋成黑色
        *lpDst = (BYTE) 0;

        //如果源图像中当前点自身或者上下有一个点不是黑色，
        //则将目标图像中的当前点赋成白色
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + (n - 1) * lWidth);

          if (pixel == 255) {
            *lpDst = (BYTE) 255;
            break;
          }
        }
      }
    }
  }
  else {
    //使用自定义的结构元素进行腐蚀
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // 由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边
        // 的两列像素和最上边和最下边的两列像素
        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = lpDIBBits + lWidth * j + i;
        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = lpNewDIBBits + lWidth * j + i;
        //取得当前指针处的像素值，注意要转换为BYTE型
        pixel = (BYTE) * lpSrc;

        //目标图像中含有0和255外的其它灰度值
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //目标图像中的当前点先赋成黑色
        *lpDst = (BYTE) 0;

        //如果原图像中对应结构元素中为黑色的那些点中有一个不是黑色，
        //则将目标图像中的当前点赋成白色
        //注意在DIB图像中内容是上下倒置的
        for (m = 0; m < 3; m++) {
          for (n = 0; n < 3; n++) {
            if (structure[ m ][ n ] == -1) {
              continue;
            }

            pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));

            if (pixel == 255) {
              *lpDst = (BYTE) 255;
              break;
            }
          }
        }
      }
    }
  }

  // 复制腐蚀后的图像
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  // 重新初始化新分配的内存，设定初始值为255
  //lpDst = lpNewDIBBits;
  //memset(lpDst, (BYTE)255, lWidth* lHeight);

  if (nMode == 0) {
    //使用水平方向的结构元素进行膨胀
    for (j = 0; j < lHeight; j++) {
      for (i = 1; i < lWidth - 1; i++) {
        // 由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边
        // 的两列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = lpDIBBits + lWidth * j + i;
        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = lpNewDIBBits + lWidth * j + i;
        //取得当前指针处的像素值，注意要转换为BYTE型
        pixel = (BYTE) * lpSrc;

        //目标图像中含有0和255外的其它灰度值
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //目标图像中的当前点先赋成白色
        *lpDst = (BYTE) 255;

        //源图像中当前点自身或者左右只要有一个点是黑色，
        //则将目标图像中的当前点赋成黑色
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + n - 1);

          if (pixel == 0) {
            *lpDst = (BYTE) 0;
            break;
          }
        }
      }
    }
  }
  else if (nMode == 1) {
    //使用垂直方向的结构元素进行膨胀
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // 由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边
        // 的两列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = lpDIBBits + lWidth * j + i;
        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = lpNewDIBBits + lWidth * j + i;
        //取得当前指针处的像素值，注意要转换为BYTE型
        pixel = (BYTE) * lpSrc;

        //目标图像中含有0和255外的其它灰度值
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //目标图像中的当前点先赋成白色
        *lpDst = (BYTE) 255;

        //源图像中当前点自身或者上下只要有一个点是黑色，
        //则将目标图像中的当前点赋成黑色
        for (n = 0; n < 3; n++) {
          pixel = *(lpSrc + (n - 1) * lWidth);

          if (pixel == 0) {
            *lpDst = (BYTE) 0;
            break;
          }
        }
      }
    }
  }
  else {
    //使用自定义的结构元素进行膨胀
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth; i++) {
        // 由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边
        // 的两列像素和最上边和最下边的两列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = lpDIBBits + lWidth * j + i;
        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = lpNewDIBBits + lWidth * j + i;
        //取得当前指针处的像素值，注意要转换为BYTE型
        pixel = (BYTE) * lpSrc;

        //目标图像中含有0和255外的其它灰度值
        if (pixel != 255 && *lpSrc != 0) {
          return FALSE;
        }

        //目标图像中的当前点先赋成白色
        *lpDst = (BYTE) 255;

        //原图像中对应结构元素中为黑色的那些点中只要有一个是黑色，
        //则将目标图像中的当前点赋成黑色
        //注意在DIB图像中内容是上下倒置的
        for (m = 0; m < 3; m++) {
          for (n = 0; n < 3; n++) {
            if (structure[ m ][ n ] == -1) {
              continue;
            }

            pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));

            if (pixel == 0) {
              *lpDst = (BYTE) 0;
              break;
            }
          }
        }
      }
    }
  }

  // 复制膨胀后的图像
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits);

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   CloseBlock()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
*   int  nMode  - 闭运算方式：0表示水平方向，1垂直方向，2自定义结构元素。
*  int  structure[3][3] - 自定义的3×3结构元素。
* 返回值:
*   BOOL              - 闭运算成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行开运算。结构元素为水平方向或垂直方向的三个点，
* 中间点位于原点；或者由用户自己定义3×3的结构元素。
* 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/
BOOL CloseBlock(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int nMode, int structure[ 3 ][ 3 ])
{
  if (DilationBlock(lHeight, lWidth, lpDIBBits, lLineBytes, nMode , structure)) {
    if (ErosionBlock(lHeight, lWidth, lpDIBBits, lLineBytes, nMode , structure)) {
      return TRUE;
    }
  }

  return FALSE;
}

/*************************************************************************
* 函数名称：
*   Thining()
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight      - 源图像高度（象素数）
* 返回值:
*   BOOL              - 闭运算成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行细化运算。要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/
BOOL Thining(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  LPBYTE lpSrc;   // 指向源图像的指针
  LPBYTE lpDst;   // 指向缓存图像的指针
  LPBYTE lpNewDIBBits; // 指向缓存DIB图像的指针
  BOOL bModified;   //脏标记
  int i, j, m, n;   //循环变量

  //四个条件
  BOOL bCondition1;
  BOOL bCondition2;
  BOOL bCondition3;
  BOOL bCondition4;

  BYTE nCount; //计数器
  BYTE pixel; //像素值
  BYTE neighbour[ 5 ][ 5 ]; //5×5相邻区域像素值

  // 暂时分配内存，以保存新图像
  lpNewDIBBits = MALLOC(BYTE, lWidth * lHeight);

  if (lpNewDIBBits == NULL) {
    return FALSE;
  }

  // 初始化新分配的内存，设定初始值为255
  lpDst = lpNewDIBBits;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  bModified = TRUE;

  while (bModified) {
    bModified = FALSE;
    // 初始化新分配的内存，设定初始值为255
    lpDst = lpNewDIBBits;
    memset(lpDst, (BYTE) 255, lWidth * lHeight);

    for (j = 2; j < lHeight - 2; j++) {
      for (i = 2; i < lWidth - 2; i++) {
        bCondition1 = FALSE;
        bCondition2 = FALSE;
        bCondition3 = FALSE;
        bCondition4 = FALSE;

        //由于使用5×5的结构元素，为防止越界，所以不处理外围的几行和几列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = lpDIBBits + lWidth * j + i;
        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = lpNewDIBBits + lWidth * j + i;
        //取得当前指针处的像素值，注意要转换为BYTE型
        pixel = (BYTE) * lpSrc;

        //目标图像中含有0和255外的其它灰度值
        if (pixel != 255 && *lpSrc != 0) {
          continue;
        }
        //如果源图像中当前点为白色，则跳过
        else if (pixel == 255) {
          continue;
        }

        //获得当前点相邻的5×5区域内像素值，白色用0代表，黑色用1代表
        for (m = 0; m < 5; m++) {
          for (n = 0; n < 5; n++) {
            neighbour[ m ][ n ] = (255 - (BYTE) * (lpSrc +
                ((4 - m) - 2) * lWidth + n - 2)) / 255;
          }
        }

        //逐个判断条件。
        //判断2<=NZ(P1)<=6
        nCount = neighbour[ 1 ][ 1 ] + neighbour[ 1 ][ 2 ] + neighbour[ 1 ][ 3 ] \
            + neighbour[ 2 ][ 1 ] + neighbour[ 2 ][ 3 ] + \
            + neighbour[ 3 ][ 1 ] + neighbour[ 3 ][ 2 ] + neighbour[ 3 ][ 3 ];

        if (nCount >= 2 && nCount <= 6) {
          bCondition1 = TRUE;
        }

        //判断Z0(P1)=1
        nCount = 0;

        if (neighbour[ 1 ][ 2 ] == 0 && neighbour[ 1 ][ 1 ] == 1) {
          nCount++;
        }

        if (neighbour[ 1 ][ 1 ] == 0 && neighbour[ 2 ][ 1 ] == 1) {
          nCount++;
        }

        if (neighbour[ 2 ][ 1 ] == 0 && neighbour[ 3 ][ 1 ] == 1) {
          nCount++;
        }

        if (neighbour[ 3 ][ 1 ] == 0 && neighbour[ 3 ][ 2 ] == 1) {
          nCount++;
        }

        if (neighbour[ 3 ][ 2 ] == 0 && neighbour[ 3 ][ 3 ] == 1) {
          nCount++;
        }

        if (neighbour[ 3 ][ 3 ] == 0 && neighbour[ 2 ][ 3 ] == 1) {
          nCount++;
        }

        if (neighbour[ 2 ][ 3 ] == 0 && neighbour[ 1 ][ 3 ] == 1) {
          nCount++;
        }

        if (neighbour[ 1 ][ 3 ] == 0 && neighbour[ 1 ][ 2 ] == 1) {
          nCount++;
        }

        if (nCount == 1) {
          bCondition2 = TRUE;
        }

        //判断P2*P4*P8=0 or Z0(p2)!=1
        if (neighbour[ 1 ][ 2 ] * neighbour[ 2 ][ 1 ] * neighbour[ 2 ][ 3 ] == 0) {
          bCondition3 = TRUE;
        }
        else {
          nCount = 0;

          if (neighbour[ 0 ][ 2 ] == 0 && neighbour[ 0 ][ 1 ] == 1) {
            nCount++;
          }

          if (neighbour[ 0 ][ 1 ] == 0 && neighbour[ 1 ][ 1 ] == 1) {
            nCount++;
          }

          if (neighbour[ 1 ][ 1 ] == 0 && neighbour[ 2 ][ 1 ] == 1) {
            nCount++;
          }

          if (neighbour[ 2 ][ 1 ] == 0 && neighbour[ 2 ][ 2 ] == 1) {
            nCount++;
          }

          if (neighbour[ 2 ][ 2 ] == 0 && neighbour[ 2 ][ 3 ] == 1) {
            nCount++;
          }

          if (neighbour[ 2 ][ 3 ] == 0 && neighbour[ 1 ][ 3 ] == 1) {
            nCount++;
          }

          if (neighbour[ 1 ][ 3 ] == 0 && neighbour[ 0 ][ 3 ] == 1) {
            nCount++;
          }

          if (neighbour[ 0 ][ 3 ] == 0 && neighbour[ 0 ][ 2 ] == 1) {
            nCount++;
          }

          if (nCount != 1) {
            bCondition3 = TRUE;
          }
        }

        //判断P2*P4*P6=0 or Z0(p4)!=1
        if (neighbour[ 1 ][ 2 ] * neighbour[ 2 ][ 1 ] * neighbour[ 3 ][ 2 ] == 0) {
          bCondition4 = TRUE;
        }
        else {
          nCount = 0;

          if (neighbour[ 1 ][ 1 ] == 0 && neighbour[ 1 ][ 0 ] == 1) {
            nCount++;
          }

          if (neighbour[ 1 ][ 0 ] == 0 && neighbour[ 2 ][ 0 ] == 1) {
            nCount++;
          }

          if (neighbour[ 2 ][ 0 ] == 0 && neighbour[ 3 ][ 0 ] == 1) {
            nCount++;
          }

          if (neighbour[ 3 ][ 0 ] == 0 && neighbour[ 3 ][ 1 ] == 1) {
            nCount++;
          }

          if (neighbour[ 3 ][ 1 ] == 0 && neighbour[ 3 ][ 2 ] == 1) {
            nCount++;
          }

          if (neighbour[ 3 ][ 2 ] == 0 && neighbour[ 2 ][ 2 ] == 1) {
            nCount++;
          }

          if (neighbour[ 2 ][ 2 ] == 0 && neighbour[ 1 ][ 2 ] == 1) {
            nCount++;
          }

          if (neighbour[ 1 ][ 2 ] == 0 && neighbour[ 1 ][ 1 ] == 1) {
            nCount++;
          }

          if (nCount != 1) {
            bCondition4 = TRUE;
          }
        }

        if (bCondition1 && bCondition2 && bCondition3 && bCondition4) {
          *lpDst = (BYTE) 255;
          bModified = TRUE;
        }
        else {
          *lpDst = (BYTE) 0;
        }
      }
    }

    // 复制腐蚀后的图像
    memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
  }

  // 复制腐蚀后的图像
  memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

  SAFEFREE(lpNewDIBBits);
  return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   ErosionLine()
 *
 * 参数:
 *   LPBYTE lpDIBBits    - 指向源DIB图像指针
 *   int lWidth          - 源图像宽度（象素数，必须是4的倍数）
 *   int lHeight         - 源图像高度（象素数）
 *   int type            - 腐蚀方式，0表示水平方向，1表示垂直方向。
 *   int num             - 结构元素的个数（水平方向和垂直方向）。
 *
 * 返回值:
 *   BOOL                - 腐蚀成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行腐蚀运算。结构元素为水平方向或垂直方向，个数由用户选择
 * 为3，5或7，结构原点位于其中心。
 * 要求目标图像为只有0和255两个灰度值的灰度图像，如果不满足，则强行转换。
 ************************************************************************/
BOOL ErosionLine(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int type, int num)
{
  // 指向源图像的指针
  LPBYTE lpSrc;
  // 指向要复制区域的指针
  LPBYTE lpDst;
  // 指向复制图像的指针
  LPBYTE lpImage;
  // 循环变量
  int i, j, k;
  //像素值
  BYTE pixeltemp;
  // 暂时分配内存，以保存新图像
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // 判断是否内存分配失败
  if (lpImage == NULL) {
    // 分配内存失败
    return FALSE;
  }

  lpDst = (LPBYTE) lpImage;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  //如果图像不是二值图像，转化为二值图像
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

      //取得当前指针处的像素值，注意要转换为BYTE型
      pixeltemp = (BYTE) * lpSrc;

      //目标图像中灰度值大于120的设置为255，否则设置为0
      if (pixeltemp >= 120) {
        *lpSrc = (BYTE) 255;
      }
      else {
        *lpSrc = (BYTE) 0;
      }
    }
  }


  if (type == 0) {
    //使用水平方向的结构元素进行腐蚀
    for (j = 0; j < lHeight; j++) {
      for (i = (num - 1) / 2; i < lWidth - (num - 1) / 2; i++) {
        //由于使用1×num的结构元素，为防止越界，所以不处理最左边和最右边的(num-1)/2列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = (LPBYTE) lpImage + lWidth * j + i;

        //取得当前指针处的像素值，注意要转换为BYTE型
        pixeltemp = (BYTE) * lpSrc;

        //目标图像中的当前点先赋成黑色
        *lpDst = (BYTE) 0;

        //如果源图像中当前点自身或者左右有(num-1)/2个点不是黑色，
        //则将目标图像中的当前点赋成白色
        for (k = 0; k < num; k++) {
          //读取像素灰度值
          pixeltemp = *(lpSrc + k - (num - 1) / 2);

          //发现一个白点
          if (pixeltemp == 255) {
            //该点设置为白点
            *lpDst = (BYTE) 255;
            break;
          }
        }

      }
    }

  }
  else if (type == 1) {
    //使用垂直方向的结构元素进行腐蚀
    for (j = (num - 1) / 2; j < lHeight - (num - 1) / 2; j++) {
      for (i = 0; i < lWidth; i++) {
        //由于使用1×num的结构元素，为防止越界，所以不处理最上边和最下边的(num-1)/2列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = (LPBYTE) lpImage + lWidth * j + i;

        //取得当前指针处的像素值，注意要转换为BYTE型
        pixeltemp = (BYTE) * lpSrc;

        //目标图像中的当前点先赋成黑色
        *lpDst = (BYTE) 0;

        //如果源图像中当前点自身或者上下有(num-1)/2个点不是黑色，，
        //则将目标图像中的当前点赋成白色
        for (k = 0; k < num; k++) {
          //读取像素灰度值
          pixeltemp = *(lpSrc + (k - (num - 1) / 2) * lWidth);

          //发现一个白点
          if (pixeltemp == 255) {
            //该点设置为白点
            *lpDst = (BYTE) 255;
            break;
          }
        }

      }
    }

  }

  // 复制腐蚀后的图像
  memcpy(lpDIBBits, lpImage, lWidth * lHeight);

  // 释放内存
  SAFEFREE(lpImage);

  // 返回
  return TRUE;
}



/*************************************************************************
 *
* 函数名称：
*   DilationLine()
 *
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth   - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight  - 源图像高度（象素数）
*   int  type            - 膨胀方式，0表示水平方向，1表示垂直方向。
*     int  num            - 结构元素的个数（水平方向和垂直方向）。
 *
* 返回值:
*   BOOL              - 膨胀成功返回TRUE，否则返回FALSE。
 *
* 说明:
* 该函数用于对图像进行膨胀运算。结构元素为水平方向或垂直方向，个数由用户选择
* 为3，5或7，结构原点位于其中心。
* 要求目标图像为只有0和255两个灰度值的灰度图像，如果不满足，则强行转换。
 ************************************************************************/
BOOL DilationLine(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes, int type , int num)
{
  LPBYTE lpSrc;  // 指向源图像的指针
  LPBYTE lpDst;  // 指向要复制区域的指针
  LPBYTE lpImage; // 指向复制图像的指针
  // 循环变量
  int i, j, k;
  //像素值
  BYTE pixeltemp;
  // 暂时分配内存，以保存新图像
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // 判断是否内存分配失败
  if (lpImage == NULL) {
    // 分配内存失败
    return FALSE;
  }

  lpDst = (LPBYTE) lpImage;
  memset(lpDst, (BYTE) 255, lWidth * lHeight);

  //如果图像不是二值图像，转化为二值图像
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {

      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

      //取得当前指针处的像素值，注意要转换为BYTE型
      pixeltemp = (BYTE) * lpSrc;

      //目标图像中灰度值大于120的设置为255，否则设置为0
      if (pixeltemp >= 120) {
        *lpSrc = (BYTE) 255;
      }
      else {
        *lpSrc = (BYTE) 0;
      }
    }
  }


  if (type == 0) {
    //使用水平方向的结构元素进行膨胀
    for (j = 0; j < lHeight; j++) {
      for (i = (num - 1) / 2; i < lWidth - (num - 1) / 2; i++) {
        //由于使用1×num的结构元素，为防止越界，所以不处理最左边和最右边的(num-1)/2列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = (LPBYTE) lpImage + lWidth * j + i;

        //取得当前指针处的像素值，注意要转换为BYTE型
        pixeltemp = (BYTE) * lpSrc;

        //目标图像中的当前点先赋成白色
        *lpDst = (BYTE) 255;

        //如果源图像中当前点自身或者左右各(num-1)/2只要有个点是黑色，
        //则将目标图像中的当前点赋成黑色
        for (k = 0; k < num; k++) {
          //读取像素灰度值
          pixeltemp = *(lpSrc + k - (num - 1) / 2);

          //发现一个黑点
          if (pixeltemp == 0) {
            //该点设置为黑点
            *lpDst = (BYTE) 0;
            break;
          }
        }

      }
    }

  }
  else if (type == 1) {
    //使用垂直方向的结构元素进行膨胀
    for (j = (num - 1) / 2; j < lHeight - (num - 1) / 2; j++) {
      for (i = 0; i < lWidth; i++) {
        //由于使用1×num的结构元素，为防止越界，所以不处理最上边和最下边的(num-1)/2列像素

        // 指向源图像倒数第j行，第i个象素的指针
        lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

        // 指向目标图像倒数第j行，第i个象素的指针
        lpDst = (LPBYTE) lpImage + lWidth * j + i;

        //取得当前指针处的像素值，注意要转换为BYTE型
        pixeltemp = (BYTE) * lpSrc;

        //目标图像中的当前点先赋成白色
        *lpDst = (BYTE) 255;

        //如果源图像中当前点自身或者上下各(num-1)/2只要有个点是黑色，
        //则将目标图像中的当前点赋成白色
        for (k = 0; k < num; k++) {
          //读取像素灰度值
          pixeltemp = *(lpSrc + (k - (num - 1) / 2) * lWidth);

          //发现一个白点
          if (pixeltemp == 0) {
            //该点设置为白点
            *lpDst = (BYTE) 0;
            break;
          }
        }

      }
    }

  }

  // 复制膨胀后的图像
  memcpy(lpDIBBits, lpImage, lWidth * lHeight);

  // 释放内存
  SAFEFREE(lpImage);

  // 返回
  return TRUE;
}



/*************************************************************************
 *
* 函数名称：
*   ThiningLine()
 *
* 参数:
*   LPBYTE lpDIBBits    - 指向源DIB图像指针
*   int lWidth   - 源图像宽度（象素数，必须是4的倍数）
*   int lHeight  - 源图像高度（象素数）
 *
* 返回值:
*   BOOL              - 膨胀成功返回TRUE，否则返回FALSE。
 *
* 说明:
* 采用8临域细化的方法，分别进行水平方向和垂直方向的细化
 ************************************************************************/

BOOL ThiningLine(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  //创建八临域销点表
  static int erasetable[ 256 ] = {
    0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0
  };

  // 指向源图像的指针
  LPBYTE lpSrc;
  // 指向要复制区域的指针
  LPBYTE lpDst;
  // 指向复制图像的指针
  LPBYTE lpImage;
  // 循环变量
  int i, j, num;
  //像素值
  BYTE pixeltemp;

  //临时变量
  int nw, n, ne, w, e, sw, s, se;
  // 暂时分配内存，以保存新图像
  lpImage = (LPBYTE) MALLOC(BYTE, lWidth * lHeight);

  // 判断是否内存分配失败
  if (lpImage == NULL) {
    // 分配内存失败
    return FALSE;
  }

  lpDst = (LPBYTE) lpImage;
  //将目标图像赋值（全白）
  //memset(lpDst, (BYTE)255, lWidth* lHeight);

  //如果图像不是二值图像，转化为二值图像
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {

      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

      //取得当前指针处的像素值，注意要转换为BYTE型
      pixeltemp = (BYTE) * lpSrc;

      //目标图像中灰度值大于120的设置为255，否则设置为0
      if (pixeltemp >= 120) {
        *lpSrc = (BYTE) 255;
      }
      else {
        *lpSrc = (BYTE) 0;
      }
    }
  }

  // 初始化图像为原始图像
  memcpy(lpImage, lpDIBBits, lWidth * lHeight);

  //先进行水平方向的细化
  for (j = 1; j < lHeight - 1; j++) {
    for (i = 1; i < lWidth - 1; i++) {
      //为防止越界，宽度，高度上下左右各留出一行

      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;

      // 指向目标图像倒数第j行，第i个象素的指针
      lpDst = (LPBYTE) lpImage + lWidth * j + i;

      //是黑点才做处理
      if (*(lpSrc) == 0) {
        w = (BYTE) * (lpSrc - 1);      //左邻点
        e = (BYTE) * (lpSrc + 1);      //右邻点

        //如果左右两个邻居中至少有一个是白点才处理
        if ((w == 255) || (e == 255)) {
          nw = (BYTE) * (lpSrc + lWidth - 1);     //左上邻点
          n = (BYTE) * (lpSrc + lWidth);     //上邻点
          ne = (BYTE) * (lpSrc + lWidth + 1);     //右上邻点
          sw = (BYTE) * (lpSrc - lWidth - 1);     //左下邻点
          s = (BYTE) * (lpSrc - lWidth);     //下邻点
          se = (BYTE) * (lpSrc - lWidth + 1);     //右下邻点
          //计算索引
          num = nw / 255 + n / 255 * 2 + ne / 255 * 4 + w / 255 * 8 + e / 255 * 16 + sw / 255 * 32 + s / 255 * 64 + se / 255 * 128;

          if (erasetable[ num ] == 1) {   //经查表，可以删除
            //在原图缓冲区中将该黑点删除
            *(lpSrc) = (BYTE) 255;
            //结果图中该黑点也删除
            *(lpDst) = (BYTE) 255;
          }
        }
      }
    }
  }

  //再进行垂直方向的细化

  for (i = 1; i < lWidth - 1; i++) {
    for (j = 1; j < lHeight - 1; j++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lWidth * j + i;
      // 指向目标图像倒数第j行，第i个象素的指针
      lpDst = (LPBYTE) lpImage + lWidth * j + i;

      //是黑点才做处理
      if (*(lpSrc) == 0) {
        n = (BYTE) * (lpSrc + lWidth);
        s = (BYTE) * (lpSrc - lWidth);

        //如果上下两个邻居中至少有一个是白点才处理
        if ((n == 255) || (s == 255)) {
          nw = (BYTE) * (lpSrc + lWidth - 1);
          ne = (BYTE) * (lpSrc + lWidth + 1);
          w = (BYTE) * (lpSrc - 1);
          e = (BYTE) * (lpSrc + 1);
          sw = (BYTE) * (lpSrc - lWidth - 1);
          se = (BYTE) * (lpSrc - lWidth + 1);
          //计算索引
          num = nw / 255 + n / 255 * 2 + ne / 255 * 4 + w / 255 * 8 + e / 255 * 16 + sw / 255 * 32 + s / 255 * 64 + se / 255 * 128;

          if (erasetable[ num ] == 1) {
            //经查表，可以删除
            //在原图缓冲区中将该黑点删除
            *(lpSrc) = (BYTE) 255;
            //结果图中该黑点也删除
            *(lpDst) = (BYTE) 255;
          }
        }
      }
    }
  }

  // 复制细化后的图像
  memcpy(lpDIBBits, lpImage, lWidth * lHeight);
  // 释放内存
  SAFEFREE(lpImage);
  // 返回
  return TRUE;
}

