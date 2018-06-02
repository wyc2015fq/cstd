
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "macro.h"
#include "imglib.h"

/*************************************************************************
 *
 * 函数名称：
 *   HoleNumber()
 *
 * 参数:
 *   CDib* pDib        - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回False。
 *
 * 说明:
 *   该函数将消去图象中面积小于阈值的小区域
 *
 *************************************************************************/
BOOL HoleNumber(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  // 循环变量
  int i, j, s, n;

  // 空穴的数目以及面积阈值
  int nHoleNum, nMinArea;

  int nBlackPix, temp;

  // 正向和反响传播标志
  int nDir1, nDir2;

  // 用来存储的一位数组
  int* pnBinary;

  pnBinary = MALLOC(int, lHeight * lLineBytes);

  // 小区域的阈值面积为20个象素点
  nMinArea = 50;

  // 将图象二值化
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // 白色象素为背景，存成0
      if (*lpSrc > 200) {
        pnBinary[ lLineBytes * j + i ] = 0;
      }
      // 黑象素存成-1
      else {
        pnBinary[ lLineBytes * j + i ] = -1;
      }
    }
  }

  // 空穴数赋初值
  nHoleNum = 1;

  do {
    s = 0;

    // 寻找每个空穴的初始象素值
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 1; i < lWidth - 1; i++) {
        // 找到初始象素
        if (pnBinary[ lLineBytes * j + i ] == -1) {
          s = 1;
          // 将象素值改成当前的空穴数值
          pnBinary[ lLineBytes * j + i ] = nHoleNum;

          // 跳出循环
          j = lHeight;
          i = lLineBytes;
        }
      }
    }

    //没有初始象素，跳出循环
    if (s == 0) {
      break;
    }
    else {
      do {
        // 正向和反响传播系数赋初值0
        nDir1 = 0;
        nDir2 = 0;

        // 正向扫描
        for (j = 1; j < lHeight - 1; j++) {
          for (i = 1; i < lWidth - 1; i++) {
            nBlackPix = pnBinary[ lLineBytes * j + i ];

            // 如果象素已经被扫描，或者是背景色，进行下一个循环
            if (nBlackPix != -1) {
              continue;
            }

            // 如果上侧或者左侧的象素值已经被扫描，且属于当前的空穴，当前的象素值
            // 改成空穴的数值
            nBlackPix = pnBinary[ lLineBytes * (j - 1) + i ];

            if (nBlackPix == nHoleNum) {
              pnBinary[ lLineBytes * j + i ] = nHoleNum;
              nDir1 = 1;

              continue;
            }

            nBlackPix = pnBinary[ lLineBytes * j + i - 1 ];

            if (nBlackPix == nHoleNum) {
              pnBinary[ lLineBytes * j + i ] = nHoleNum;
              nDir1 = 1;
            }

          }
        }

        // 正向象素全部被扫描，跳出循环
        if (nDir1 == 0) {
          break;
        }

        // 反向扫描
        for (j = lHeight - 2; j >= 1; j--) {
          for (i = lWidth - 2; i >= 1; i--) {
            nBlackPix = pnBinary[ lLineBytes * j + i ];

            // 如果象素已经被扫描，或者是背景色，进行下一个循环
            if (nBlackPix != -1) {
              continue;
            }

            // 如果下侧或者右侧的象素值已经被扫描，且属于当前的空穴，当前的象素值
            // 改成空穴的数值
            nBlackPix = pnBinary[ lLineBytes * (j + 1) + i ];

            if (nBlackPix == nHoleNum) {
              pnBinary[ lLineBytes * j + i ] = nHoleNum;
              nDir2 = 1;

              continue;
            }

            nBlackPix = pnBinary[ lLineBytes * j + i + 1 ];

            if (nBlackPix == nHoleNum) {
              pnBinary[ lLineBytes * j + i ] = nHoleNum;
              nDir2 = 1;
            }
          }
        }

        if (nDir2 == 0) {
          break;
        }
      }
      while (1);
    }

    // 空穴数增加
    nHoleNum++;
  }
  while (1);

  nHoleNum --;

  // 寻找面积小于阈值的空穴区域
  for (n = 1; n <= nHoleNum; n++) {
    s = 0;

    for (j = 0; j < lHeight - 1; j++) {
      for (i = 0; i < lWidth - 1; i++) {
        nBlackPix = pnBinary[ lLineBytes * j + i ];

        if (nBlackPix == n) {
          s++;
        }

        // 如果区域面积已经大于阈值，跳出循环
        if (s > nMinArea) {
          break;
        }
      }

      if (s > nMinArea) {
        break;
      }
    }

    // 小于阈值的区域，赋以与背景一样的颜色，进行消去
    if (s <= nMinArea) {
      for (j = 0; j < lHeight - 1; j++) {
        for (i = 0; i < lWidth - 1; i++) {
          nBlackPix = pnBinary[ lLineBytes * j + i + 1 ];

          if (nBlackPix == n) {
            pnBinary[ lLineBytes * j + i + 1 ] = 0;
          }

        }
      }

    }

  }

  // 存储象素值，输出
  for (j = 0; j < lHeight; j++) {
    // 列
    for (i = 0; i < lWidth; i++) {
      // 二值图象
      temp = pnBinary[ j * lLineBytes + i ];

      // 指向位图i行j列象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // 更新源图像
      if (temp != 0) {
        * (lpSrc) = 0;
      }
      else {
        * (lpSrc) = 255;
      }
    }
  }

  SAFEFREE(pnBinary);

  return TRUE;
}

/*************************************************************************
* 函数名称：
*   Moment()
 *
* 参数:
*   CDib* pDib        - 指向CDib类的指针
 *
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回False。
 *
* 说明:
*   该函数计算图象的力矩。
 *************************************************************************/
int Moment(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  // 图象的矩
  int nImageMoment;

  // 循环变量
  int i, j;

  int ip, jq;

  // 临时变量
  Float temp;

  nImageMoment = 0;

  // 计算一阶矩
  ip = jq = 1;

  // 力矩的计算
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      temp = pow((Float) i, ip) * pow((Float)(j), jq);
      temp = temp * (*lpSrc);

      nImageMoment = nImageMoment + (int) temp;
    }
  }

  return nImageMoment;
}

/*************************************************************************
 *
* 函数名称：
*   BaryCenterMoment()
 *
* 参数:
*   CDib* pDib        - 指向CDib类的指针
 *
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回False。
 *
* 说明:
*   该函数计算图象的重心矩。
 *
 *************************************************************************/

BOOL BaryCenterMoment(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes,
    int* pBarycenterX, int* pBarycenterY, int* pBarycenterMoment)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  // 图象象素值
  int nPixelValue;

  // 图象重心矩
  int nBarycenterMoment;

  // 0次矩m00, x方向的一次矩m01和y方向的一次矩m10
  int m00, m10, m01;

  // 重心x，y坐标值
  int nBarycenterX, nBarycenterY;

  // 循环变量
  int i, j;

  // 临时变量
  Float temp;

  // 赋初值为零
  m00 = 0;
  m01 = 0;
  m10 = 0;
  nBarycenterMoment = 0;

  // 求0次矩m00, x方向的一次矩m01和y方向的一次矩m10
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;
      nPixelValue = *lpSrc;

      m00 = m00 + nPixelValue;

      temp = i * nPixelValue;
      m01 = (int)(m01 + temp);

      temp = j * nPixelValue;
      m10 = (int)(m10 + temp);
    }
  }

  // 重心x，y坐标值
  nBarycenterX = (int)(m01 / m00 + 0.5);
  nBarycenterY = (int)(m10 / m00 + 0.5);

  // 计算重心矩
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;
      nPixelValue = *lpSrc;

      temp = (i - nBarycenterX) * (j - nBarycenterY);

      temp = temp * nPixelValue;

      nBarycenterMoment = nBarycenterMoment + (int) temp;
    }
  }

  *pBarycenterX = nBarycenterX;
  *pBarycenterY = nBarycenterY;
  *pBarycenterMoment = nBarycenterMoment;

  return TRUE;

}

/*************************************************************************
 *
* 函数名称：
*   StreetDis()
 *
* 参数:
*   CDib* pDib        - 指向CDib类的指针
 *
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回False。
 *
* 说明:
*   该函数利用棋盘距离对图象进行变换。
 *
 *************************************************************************/
BOOL StreetDis(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  // 循环和临时变量
  int i, j;
  int temp, s;

  // 存储象素值的数组
  int* pnBinary, *pnStore;

  int nImageValue;

  pnBinary = MALLOC(int, lHeight * lLineBytes);
  pnStore = MALLOC(int, lHeight * lLineBytes);

  // 将图象二值化
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // 白色象素为背景，存成0
      if (*lpSrc > 200) {
        pnBinary[ lLineBytes * j + i ] = 0;
        pnStore [ lLineBytes * j + i ] = 0;
      }
      // 黑象素存成1
      else {
        pnBinary[ lLineBytes * j + i ] = 1;
        pnStore [ lLineBytes * j + i ] = 1;
      }
    }
  }

  s = 1;

  while (s == 1) {
    s = 0;

    // 进行距离的累加
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 1; i < lWidth - 1; i++) {
        nImageValue = pnBinary[ lLineBytes * j + i ];

        // 如果是背景，进行下一个循环
        if (nImageValue == 0) {
          continue;
        }

        // 如果当前象素值和四周的值一样，象素值增加一
        if (nImageValue == pnBinary[ lLineBytes * (j - 1) + i ] && nImageValue == pnBinary[ lLineBytes * (j + 1) + i ])
          if (nImageValue == pnBinary[ lLineBytes * j + i - 1 ] && nImageValue == pnBinary[ lLineBytes * j + i + 1 ]) {
            pnStore[ lLineBytes * j + i ] ++;
            s = 1;
          }
      }
    }

    // 在进行下一轮循环前将当前的结果储存
    for (j = 0; j < lHeight; j++)
      for (i = 1; i < lWidth; i++) {
        pnBinary[ lLineBytes * j + i ] = pnStore[ lLineBytes * j + i ];
      }
  }

  // 存储象素值，输出
  for (j = 0; j < lHeight; j++) {
    // 列
    for (i = 0; i < lWidth; i++) {
      // 骨架的象素值
      temp = pnStore[ j * lLineBytes + i ];

      // 指向位图i行j列象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // 更新源图像
      * (lpSrc) = (BYTE)((25 - temp) * 10 + 5);
    }
  }

  SAFEFREE(pnStore);
  SAFEFREE(pnBinary);

  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   FreameWork()
 *
* 参数:
*   CDib* pDib        - 指向CDib类的指针
 *
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回False。
 *
* 说明:
*   该函数利用棋盘距离，生成图象的骨架。
 *
 *************************************************************************/

BOOL FreameWork(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  // 循环和临时变量
  int i, j;
  int temp, s;

  // 存储象素值的数组
  int* pnBinary, *pnStore;

  int nImageValue;

  pnBinary = MALLOC(int, lHeight * lLineBytes);
  pnStore = MALLOC(int, lHeight * lLineBytes);

  // 将图象二值化
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // 白色象素为背景，存成0
      if (*lpSrc > 200) {
        pnBinary[ lLineBytes * j + i ] = 0;
        pnStore [ lLineBytes * j + i ] = 0;
      }
      // 黑象素存成1
      else {
        pnBinary[ lLineBytes * j + i ] = 1;
        pnStore [ lLineBytes * j + i ] = 1;
      }
    }
  }

  s = 1;

  while (s == 1) {
    s = 0;

    // 进行距离的累加
    for (j = 1; j < lHeight - 1; j++) {
      for (i = 1; i < lWidth - 1; i++) {
        nImageValue = pnBinary[ lLineBytes * j + i ];

        // 如果是背景，进行下一个循环
        if (nImageValue == 0) {
          continue;
        }

        // 如果当前象素值和四周的值一样，象素值增加一
        if (nImageValue == pnBinary[ lLineBytes * (j - 1) + i ] && nImageValue == pnBinary[ lLineBytes * (j + 1) + i ])
          if (nImageValue == pnBinary[ lLineBytes * j + i - 1 ] && nImageValue == pnBinary[ lLineBytes * j + i + 1 ]) {
            pnStore[ lLineBytes * j + i ] ++;
            s = 1;
          }
      }
    }

    // 在进行下一轮循环前将当前的结果储存
    for (j = 0; j < lHeight; j++)
      for (i = 1; i < lWidth; i++) {
        pnBinary[ lLineBytes * j + i ] = pnStore[ lLineBytes * j + i ];
      }
  }

  for (j = 0; j < lHeight; j++)
    for (i = 0; i < lWidth; i++) {
      pnStore[ lLineBytes * j + i ] = 0;
    }

  // 如果当前的象素值比周围的都要高，做为骨架
  for (j = 1; j < lHeight - 1; j++)
    for (i = 1; i < lWidth - 1; i++) {
      nImageValue = pnBinary[ lLineBytes * j + i ] + 1;

      // 在四连通域进行比较
      if (nImageValue != pnBinary[ lLineBytes * (j - 1) + i ] && nImageValue != pnBinary[ lLineBytes * (j + 1) + i ])
        if (nImageValue != pnBinary[ lLineBytes * j + i - 1 ] && nImageValue != pnBinary[ lLineBytes * j + i + 1 ]) {
          pnStore[ lLineBytes * j + i ] = pnBinary[ lLineBytes * j + i ];
        }
    }


  // 存储象素值，输出
  for (j = 0; j < lHeight; j++) {
    // 列
    for (i = 0; i < lWidth; i++) {
      // 骨架的象素值
      temp = pnStore[ j * lLineBytes + i ];

      // 指向位图i行j列象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // 更新源图像，并将象素值进行变换，以便输出
      if (temp != 0) {
        * (lpSrc) = temp;
      }
      else {
        * (lpSrc) = 255;
      }
    }
  }

  SAFEFREE(pnStore);
  SAFEFREE(pnBinary);

  return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   ChessBoarddisrestore()
 *
 * 参数:
 *   CDib* pDib        - 指向CDib类的指针
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回False。
 *
 * 说明:
 *   该函数利用骨架对原图象进行恢复。
 *
 *************************************************************************/
BOOL ChessBoarddisreStore(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{
  // 指向源图像的指针
  LPBYTE lpSrc;

  // 循环变量
  int i, j, n, m;

  // 临时变量
  int temp, s;

  // 用来存放象素值的数组
  int* pnBinary, *pnStore;

  int nImageValue;

  pnBinary = MALLOC(int, lHeight * lLineBytes);
  pnStore = MALLOC(int, lHeight * lLineBytes);

  // 数组赋值
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      pnBinary[ lLineBytes * j + i ] = 0;

      if ((*lpSrc) != 255) {
        pnStore [ lLineBytes * j + i ] = (*lpSrc);
      }
      else {
        pnStore [ lLineBytes * j + i ] = 0;
      }
    }
  }


  // 进行图象的恢复
  for (j = 1; j < lHeight - 1; j++) {
    for (i = 1; i < lWidth - 1; i++) {
      nImageValue = pnStore[ lLineBytes * j + i ];

      if (nImageValue == 0) {
        continue;
      }

      if (nImageValue == 1) {
        pnBinary[ lLineBytes * j + i ] = 1;
        continue;
      }

      // 如果象素值大于等于2，将以（2×nImageValue）＋1的菱形范围的象素值赋1
      s = nImageValue;

      // 菱形的主轴
      for (m = -s; m <= s; m++) {
        pnBinary[ lLineBytes * j + i + m ] = 1;
      }

      // 菱形的上半部分
      for (n = -s; n < 0; n++)
        for (m = -s - n; m <= s + n; m++) {
          pnBinary[ lLineBytes * (j + n) + i + m ] = 1;
        }

      // 菱形的下半部分
      for (n = 1; n <= s; n++)
        for (m = -s + n; m <= s - n; m++) {
          pnBinary[ lLineBytes * (j + n) + i + m ] = 1;
        }

    }
  }

  // 存储象素值，输出
  for (j = 0; j < lHeight; j++) {
    // 列
    for (i = 0; i < lWidth; i++) {
      // 骨架的象素值
      temp = pnBinary[ j * lLineBytes + i ];

      // 指向位图i行j列象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // 更新源图像
      if (temp != 0)
        // 黑色象素输出
      {
        * (lpSrc) = 0;
      }
      else
        // 白色象素输出
      {
        * (lpSrc) = 255;
      }
    }
  }

  SAFEFREE(pnStore);
  SAFEFREE(pnBinary);

  return TRUE;
}

/*************************************************************************
 *
* 函数名称：
*   OutLine()
 *
* 参数:
*   CDib* pDib        - 指向CDib类的指针
 *
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回False。
 *
* 说明:
*   该函数对二值图象进行轮廓检出。
 *
 **************************************************************************/
BOOL OutLine(int lHeight, int lWidth, LPBYTE lpDIBBits, int lLineBytes)
{

  // 指向源图像的指针
  LPBYTE lpSrc;

  // 循环变量
  int i, j;

  int nPixelValue;

  int n1, n2, n3, n4, n5, n6, n7, n8;

  // 用来存放象素值的数组
  int* pnBinary;

  pnBinary = MALLOC(int, lHeight * lLineBytes);

  // 将图象二值化
  for (j = 0; j < lHeight; j++) {
    for (i = 0; i < lWidth; i++) {
      // 指向源图像倒数第j行，第i个象素的指针
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // 白色象素为背景，存成0
      if (*lpSrc > 200) {
        pnBinary[ lLineBytes * j + i ] = 0;
      }
      // 黑象素存成1
      else {
        pnBinary[ lLineBytes * j + i ] = 1;
      }
    }
  }

  for (j = 1; j < lHeight - 1; j++) {
    for (i = 1; i < lWidth - 1; i++) {
      nPixelValue = pnBinary[ lLineBytes * j + i ];
      lpSrc = (LPBYTE) lpDIBBits + lLineBytes * j + i;

      // 如果当前象素是白色，进入下一个循环
      if (nPixelValue == 0) {
        // 将相应的象素值改成零

        *lpSrc = 255;
        continue;
      }

      // 如果当前象素是黑色
      else {
        // 检查周边的8连通域
        n1 = pnBinary[ lLineBytes * (j + 1) + i + 1 ];
        n2 = pnBinary[ lLineBytes * (j + 1) + i - 1 ];
        n3 = pnBinary[ lLineBytes * (j + 1) + i ];
        n4 = pnBinary[ lLineBytes * (j - 1) + i + 1 ];
        n5 = pnBinary[ lLineBytes * (j - 1) + i - 1 ];
        n6 = pnBinary[ lLineBytes * (j - 1) + i ];
        n7 = pnBinary[ lLineBytes * j + i + 1 ];
        n8 = pnBinary[ lLineBytes * j + i - 1 ];

        //如果相邻的八个点都是黑点
        if (n1 && n2 && n3 && n4 && n5 && n6 && n7 && n8 == 1) {
          *lpSrc = (BYTE) 255;
        }
      }
    }
  }

  SAFEFREE(pnBinary);

  // 返回
  return TRUE;
}
