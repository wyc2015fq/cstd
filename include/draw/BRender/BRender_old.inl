
//基于 z-buffer 和 color-buffer 的 Bresenham 直线生成算法
//绘制一条直线,直线的宽度为1,直线的颜色由直线两个端点的颜色共同决定
//注意,浮点颜色的有效值为[0,1]区间内的浮点小数
//与Windows相对应,该算法也不包含最后一个点(x2,y2)
//z1,z2为当前点的深度值
//hDC,pZB分别为颜色缓冲和深度缓冲
//注意,从(x1,y1)--(x2,y2)与(x2,y2)--(x1,y1)略有些许不同.
//评价:该函数绘制的直线(像素位置)与Windows的LineTo()函数绘制的结果是完全一致的
void CBLine_Draw(HDDC hDC, ZBuffer* pZB, Vertex3d* v1, Vertex3d* v2)
{
  POINT pt1 = v1->PtOnScrn, pt2 = v2->PtOnScrn;
  float z1 = v1->vInView.z, z2 = v2->vInView.z;
  FRGBA clr1 = v1->clr, clr2 = v2->clr;
  int x1 = pt1.x, y1 = pt1.y, x2 = pt2.x, y2 = pt2.y;
  //两端点间的水平偏移量和垂直偏移量
  int nDx = x2 - x1;
  int nDy = y2 - y1;
  //两端点间的水平距离和垂直距离
  int nIx = ABS(nDx);
  int nIy = ABS(nDy);
  //描点步数(增量总值)
  int nInc = MAX(nIx, nIy);
  //直线当前点的颜色
  float fPlotR = clr1.r, fPlotG = clr1.g, fPlotB = clr1.b, fPlotA = clr1.a;
  //主要考察区,开始
  //缓冲区中的深度
  float zInBuffer = GetDepth(pZB, x1, y1);
  //当前深度
  float z = z1;
  int i, nTwoIx, nTwoIy, x, y, nJudgeX, nJudgeY;
  float fIncR, fIncG, fIncB, fIncA, fIncZ;

  //如果当前点的z坐标小于深度缓冲区中的相同位置z的坐标
  if (z < zInBuffer) {
    //在颜色缓冲区中绘制第一个点
    CBSetPixel(hDC, x1, y1, fPlotR, fPlotG, fPlotB, fPlotA);
    //重置深度缓冲区的值
    SetDepth(pZB, x1, y1, z);
  }

  if (nInc < 2) {
    return;
  }

  //沿直线的深度增量
  fIncZ = (z2 - z1) / nInc;
  //结束
  //用于判断是否在nJudgeX,nJudgeY方向上向前进
  nJudgeX = -nIy, nJudgeY = -nIx;
  //通过增量计算得到的当前点
  x = x1, y = y1;
  //开始进行增量计算
  //采用Bresenham算法.在这里,我们将算法进行改进,
  //使其对于任意斜率的直线都能绘制,而且绘制方向始终为从起点绘制到终点
  //与Windows相对应,该算法也不包含最后一个点(x2,y2)
  nInc--;
  //注意,整数乘2在计算机中采用移位实现
  //Bresenham算法
  nTwoIx = 2 * nIx, nTwoIy = 2 * nIy;
  //颜色增量
  fIncR = (clr2.r - fPlotR) / nInc;
  fIncG = (clr2.g - fPlotG) / nInc;
  fIncB = (clr2.b - fPlotB) / nInc;
  fIncA = (clr2.a - fPlotA) / nInc;

  //开始增量计算
  for (i = 0; i < nInc; i++) {
    //通过增量法计算的当前点是否属于直线上的点
    BOOL bPlot = FALSE;
    nJudgeX += nTwoIx;
    nJudgeY += nTwoIy;

    //检测 nJudgeX 方向
    if (nJudgeX >= 0) {
      bPlot = TRUE;
      nJudgeX -= nTwoIy;

      //将任意走向的直线统一起来
      if (nDx > 0) {
        x++;
      }
      else if (nDx < 0) {
        x--;
      }
    }

    //检测 nJudgeY 方向
    if (nJudgeY >= 0) {
      bPlot = TRUE;
      nJudgeY -= nTwoIx;

      //将任意走向的直线统一起来
      if (nDy > 0) {
        y++;
      }
      else if (nDy < 0) {
        y--;
      }
    }

    //如果当前点在直线上
    if (bPlot) {
      fPlotR += fIncR;
      fPlotG += fIncG;
      fPlotB += fIncB;
      fPlotA += fIncA;
      //主要考察区,开始
      z += fIncZ;
      zInBuffer = GetDepth(pZB, x, y);

      //如果当前点的z坐标小于深度缓冲区中的相同位置z的坐标
      if (z < zInBuffer) {
        //在颜色缓冲区中绘制第一个点
        CBSetPixel(hDC, x, y, fPlotR, fPlotG, fPlotB, fPlotA);
        //重置深度缓冲区的值
        SetDepth(pZB, x, y, z);
      }

      //结束
    }//end if
  }//end for
}
//基于Z缓冲区的三角形扫描转换，绘制并获取边信息
//获取直线边界上的点的x坐标,颜色和深度
//x1,y1,x2,y2    直线两端点坐标
//z1,z2          端点深度值
//clr1,clr2      直线起点和终点颜色
//pnCoord        记录扫描线与边的交点的x坐标，
//pnCoord        pnCoord数组的长度应该是y坐标差的绝对值加1
//pColor         记录各交点的颜色。
//pfDepth        记录各交点的深度值
//hDC,pZB        分别为颜色缓冲和深度缓冲
//dwFeature      是一个边界记录特征量。当其为TRIANGLE_AREA_ON_LINE_RIGHT时，表示区域在直线右侧；当其为TRIANGLE_AREA_ON_LINE_LEFT时,表示区域在其左侧。
//程序缺省设置为区域在边的右侧。
void CBTriangle_DealEdge(int x1, int y1, int x2, int y2, float z1, float z2,
    FRGBA clr1, FRGBA clr2, HDDC hDC, ZBuffer* pZB,
    int* pnCoord, FRGBA* pColor, float* pfDepth, DWORD dwFeature/*=G3D_TRIANGLE_ON_LINE_RIGHT*/
                        )
{
  //各数组的长度应该是y坐标差的绝对值加1,
  ASSERT(pnCoord);
  ASSERT(pColor);
  ASSERT(pfDepth);

  //对于一条水平直线，则简单地返回第一个点的坐标和颜色
  //对于一条水平直线，我们不区分区域相对它的左右关系
  if (y1 == y2) {
    pnCoord[0] = x1;
    pColor[0] = clr1;
    pfDepth[0] = z1;
    return;
  }

  //下面考查直线斜率不为0的情况:
  {
    int i;
    //两端点间的水平偏移量和垂直偏移量
    int nDx = x2 - x1;
    int nDy = y2 - y1;
    //两端点间的水平距离和垂直距离
    int nIx = ABS(nDx);
    int nIy = ABS(nDy);
    //描点步数(增量总值)
    int nInc = MAX(nIx, nIy);
    //用于判断是否在nJudgeX,nJudgeY方向上向前进
    int nJudgeX = -nIy, nJudgeY = -nIx;
    //通过增量计算得到的当前点
    int x = x1, y = y1;
    //Bresenham算法
    int nTwoIx = 2 * nIx, nTwoIy = 2 * nIy;
    //直线当前点的颜色
    float fPlotR = clr1.r, fPlotG = clr1.g, fPlotB = clr1.b;
    float fPlotA = clr1.a;
    //颜色增量
    float fIncR = (clr2.r - fPlotR) / nInc;
    float fIncG = (clr2.g - fPlotG) / nInc;
    float fIncB = (clr2.b - fPlotB) / nInc;
    float fIncA = (clr2.a - fPlotA) / nInc;
    //水平边界段起点和终点
    FRGBA clrStart = {fPlotR, fPlotG, fPlotB, fPlotA};
    FRGBA clrEnd = clrStart;
    //数组下标，水平边界段起点和终点的 x 坐标
    int nIndex = 0, nStartX = x1, nEndX = x1;
    //水平边界段起点和终点的 z 坐标,当前点的 z 坐标
    //以及 z 坐标增量
    float fStartZ = z1, fEndZ = z1, z = z1, fIncZ = (z2 - z1) / nInc;
    //根据深度值绘制第一个点
    //缓冲区中的深度
    float zInBuffer = GetDepth(pZB, x1, y1);

    //如果当前点的z坐标小于深度缓冲区中的相同位置z的坐标
    if (z < zInBuffer) {
      //在颜色缓冲区中绘制第一个点
      CBSetPixel(hDC, x1, y1, fPlotR, fPlotG, fPlotB, fPlotA);
      //重置深度缓冲区的值
      SetDepth(pZB, x1, y1, z);
    }

    //开始增量计算
    for (i = 0; i <= nInc; i++) {
      //通过增量法计算的当前点是否属于直线上的点
      BOOL bRecord = FALSE;
      //当前点是否为直线上的点
      BOOL bPlot = FALSE;
      nJudgeX += nTwoIx;
      nJudgeY += nTwoIy;

      //先检测 y 方向
      if (nJudgeY >= 0) {
        bPlot = TRUE;
        bRecord = TRUE;
        nJudgeY -= nTwoIx;

        //将任意走向的直线统一起来
        if (nDy > 0) {
          nEndX = x;
          y++;
          //水平边界段终点的颜色
          clrEnd.r = fPlotR;
          clrEnd.g = fPlotG;
          clrEnd.b = fPlotB;
          clrEnd.a = fPlotA;
          //水平边界段终点的深度(沿边进行插值计算而获得)
          fEndZ = z;
        }
        else if (nDy < 0) {
          nEndX = x;
          y--;
          //水平边界段终点的颜色
          clrEnd.r = fPlotR;
          clrEnd.g = fPlotG;
          clrEnd.b = fPlotB;
          clrEnd.a = fPlotA;
          //水平边界段终点的深度(沿边进行插值计算而获得)
          fEndZ = z;
        }
      }//end if

      //后检测 x 方向
      if (nJudgeX >= 0) {
        bPlot = TRUE;
        nJudgeX -= nTwoIy;

        //将任意走向的直线统一起来
        if (nDx > 0) {
          x++;
        }
        else if (nDx < 0) {
          x--;
        }
      }

      if (bPlot) {
        fPlotR += fIncR;
        fPlotG += fIncG;
        fPlotB += fIncB;
        fPlotA += fIncA;
        z += fIncZ;

        if (i < (nInc - 1)) {
          //根据深度值绘制第一个点
          zInBuffer = GetDepth(pZB, x, y);

          //如果当前点的z坐标小于深度缓冲区中的相同位置z的坐标
          if (z < zInBuffer) {
            //在颜色缓冲区中绘制第一个点
            CBSetPixel(hDC, x, y, fPlotR, fPlotG, fPlotB, fPlotA);
            //重置深度缓冲区的值
            SetDepth(pZB, x, y, z);
          }
        }//end if(i < (nInc - 1))
      }//end if(bPlot)

      //如果当前点在直线上，则记录当前点
      if (bRecord) {
        //取最左边点,区域在左侧
        if (dwFeature == G3D_TRIANGLE_ON_LINE_LEFT) {
          //取x坐标最小者
          if (nStartX < nEndX) {
            pnCoord[nIndex] = nStartX;
            pColor[nIndex] = clrStart;
            pfDepth[nIndex] = fStartZ;
          }
          else {
            pnCoord[nIndex] = nEndX;
            pColor[nIndex] = clrEnd;
            pfDepth[nIndex] = fEndZ;
          }
        }//end if
        //取最右边点，区域在右侧
        else {
          //取x坐标最大者
          if (nStartX < nEndX) {
            pnCoord[nIndex] = nEndX;
            pColor[nIndex] = clrEnd;
            pfDepth[nIndex] = fEndZ;
          }
          else {
            pnCoord[nIndex] = nStartX;
            pColor[nIndex] = clrStart;
            pfDepth[nIndex] = fStartZ;
          }
        }//end else

        nIndex++;
        //下一个起点
        nStartX = x;
        //下一个起点的 z 坐标
        fStartZ = z;
        //下一个起点的颜色
        clrStart.r = fPlotR;
        clrStart.g = fPlotG;
        clrStart.b = fPlotB;
        clrStart.a = fPlotA;
      }//end if(bRecord)
    }//end for

    //当斜率的很小时,y坐标增长缓慢,这时需要特别处理
    if (nIndex < (ABS(y2 - y1) + 1)) {
      //取最左边点,区域在左侧
      if (dwFeature == G3D_TRIANGLE_ON_LINE_LEFT) {
        //取x坐标最小者
        if (nStartX < x2) {
          pnCoord[nIndex] = nStartX;
          pColor[nIndex] = clrStart;
          pfDepth[nIndex] = fStartZ;
        }
        else {
          pnCoord[nIndex] = x2;
          pColor[nIndex] = clr2;
          pfDepth[nIndex] = z2;
        }
      }//end if
      //取最右边点,区域在右侧
      else {
        //取x坐标最大者
        if (nStartX < x2) {
          pnCoord[nIndex] = x2;
          pColor[nIndex] = clr2;
          pfDepth[nIndex] = z2;
        }
        else {
          pnCoord[nIndex] = nStartX;
          pColor[nIndex] = clrStart;
          pfDepth[nIndex] = fStartZ;
        }
      }//end else
    }//end if(nIndex < (ABS(y2 - y1) + 1))
  }
}
//着色一条扫描线,扫描坐标的x坐标为x1和x2, y坐标为nScanLine,
//z1,z2         -端点深度值
//clr1和clr2分别为起点和终点的颜色.
//hDC,pZB       分别为颜色缓冲和深度缓冲
void CBTriangle_Scan(int x1, int x2, int nScanLine, float z1, float z2, FRGBA clr1, FRGBA clr2, HDDC hDC, ZBuffer* pZB)
{
  //填充像素个数
  int nFillPixels = ABS(x2 - x1);

  //如果为同一个点或距离相差为0,则返回;
  if (nFillPixels < 2) {
    return;
  }

  {
    int i;
    //颜色增量(颜色差值 / 份数)
    float fIncR = (clr2.r - clr1.r) / nFillPixels;
    float fIncG = (clr2.g - clr1.g) / nFillPixels;
    float fIncB = (clr2.b - clr1.b) / nFillPixels;
    float fIncA = (clr2.a - clr1.a) / nFillPixels;
    //当前颜色
    float fRed = clr1.r + fIncR;
    float fGreen = clr1.g + fIncG;
    float fBlue = clr1.b + fIncB;
    float fAlpha = clr1.a + fIncA;
    //填充方向
    int nDirection = (x1 < x2) ? 1 : -1;
    //当前x坐标
    int x = x1 + nDirection;
    //当前 z 坐标, z坐标增量,
    //注意 我们并没有采用平面系数法计算深度
    float z = z1, fIncZ = (z2 - z1) / nFillPixels;
    z += fIncZ;
    nFillPixels--;

    //开始填充
    for (i = 0; i < nFillPixels ; i++) {
      float zInBuffer = GetDepth(pZB, x, nScanLine);

      //如果当前点的z坐标小于深度缓冲区中的相同位置z的坐标
      if (z < zInBuffer) {
        //在颜色缓冲区中绘制第一个点
        CBSetPixel(hDC, x, nScanLine, fRed, fGreen, fBlue, fAlpha);
        //重置深度缓冲区的值
        SetDepth(pZB, x, nScanLine, z);
      }

      //增量计算
      x += nDirection;
      z += fIncZ;
      fRed += fIncR;
      fGreen += fIncG;
      fBlue += fIncB;
      fAlpha += fIncA;
    }
  }
}

//采用Gouraud明暗着色方法填充三角形
//扫描转换三角形
//pt1,pt2,pt3      三角形顶点坐标
//z1, z2,z3      --三角形顶点深度
//clr1,clr2,clr3   三角形顶点颜色
//hDC,pZB         -颜色缓冲器和深度缓冲器
void CBTriangle_Draw(HDDC hDC, ZBuffer* pZB, Vertex3d* v1, Vertex3d* v2, Vertex3d* v3)
{
  POINT pt1 = v1->PtOnScrn, pt2 = v2->PtOnScrn, pt3 = v3->PtOnScrn;
  float z1 = v1->vInView.z, z2 = v2->vInView.z, z3 = v3->vInView.z;
  FRGBA clr1 = v1->clr, clr2 = v2->clr, clr3 = v3->clr;
  float zInBuffer;
  POINT point1, point2, point3;
  FRGBA color1, color2, color3;
  float fz1, fz2, fz3;
  int nDy12, nDy13, nDy32, *pnEdge12, *pnEdge13, *pnEdge32, nDelta;
  FRGBA* pClrEdge12, *pClrEdge13, *pClrEdge32;
  float* pfzEdge12, *pfzEdge13, *pfzEdge32;
  int y, nPlumbScan;
  int nPlumbScan32;
  ASSERT(hDC);
  ASSERT(pZB);

  //第一部分代码:处理退化情况中的一种情况
  //处理退化情况(三点在一条水平直线上)
  //尽管 z-buffer算法与绘制顺序一般情况下是无关的,
  //但是,当三点在一条扫描线上时,仍然有必要这样处理
  if ((pt1.y == pt2.y) && (pt2.y == pt3.y)) {
    //pt1位于中间(采用矢量点积进行判断)
    if ((pt2.x - pt1.x) * (pt3.x - pt1.x) <= 0) {
      CBTriangle_Scan(pt1.x, pt2.x, pt1.y, z1, z2, clr1, clr2, hDC, pZB);
      CBTriangle_Scan(pt1.x, pt3.x, pt1.y, z1, z3, clr1, clr3, hDC, pZB);
    }
    //pt2位于中间
    else if ((pt1.x - pt2.x) * (pt3.x - pt2.x) <= 0) {
      CBTriangle_Scan(pt2.x, pt1.x, pt1.y, z2, z1, clr2, clr1, hDC, pZB);
      CBTriangle_Scan(pt2.x, pt3.x, pt1.y, z2, z3, clr2, clr3, hDC, pZB);
    }
    //pt3位于中间
    else {
      CBTriangle_Scan(pt3.x, pt1.x, pt1.y, z3, z1, clr3, clr1, hDC, pZB);
      CBTriangle_Scan(pt3.x, pt2.x, pt1.y, z3, z2, clr3, clr2, hDC, pZB);
    }

    //增加三个端点
    zInBuffer = GetDepth(pZB, pt1.x, pt1.y);

    //如果当前点的z坐标小于深度缓冲区中的相同位置z的坐标
    if (z1 < zInBuffer) {
      //在颜色缓冲区中绘制第一个点
      CBSetPixel(hDC, pt1.x, pt1.y, clr1.r, clr1.g, clr1.b, clr1.a);
      //重置深度缓冲区的值
      SetDepth(pZB, pt1.x, pt1.y, z1);
    }

    //
    zInBuffer = GetDepth(pZB, pt2.x, pt2.y);

    //如果当前点的z坐标小于深度缓冲区中的相同位置z的坐标
    if (z2 < zInBuffer) {
      //在颜色缓冲区中绘制第一个点
      CBSetPixel(hDC, pt2.x, pt2.y, clr2.r, clr2.g, clr2.b, clr2.a);
      //重置深度缓冲区的值
      SetDepth(pZB, pt2.x, pt2.y, z2);
    }

    zInBuffer = GetDepth(pZB, pt3.x, pt3.y);

    //如果当前点的z坐标小于深度缓冲区中的相同位置z的坐标
    if (z3 < zInBuffer) {
      //在颜色缓冲区中绘制第一个点
      CBSetPixel(hDC, pt3.x, pt3.y, clr3.r, clr3.g, clr3.b, clr3.a);
      //重置深度缓冲区的值
      SetDepth(pZB, pt3.x, pt3.y, z3);
    }

    return;
  }

  //第二部分代码:处理一般情况
  //第一步,排序:
  //排序规则:第一个点的y坐标最小,第二个点的y坐标最大,第三个点则位于它们的中间
  //顶点和颜色备份
  point1 = pt1, point2 = pt2, point3 = pt3;
  color1 = clr1, color2 = clr2, color3 = clr3;
  fz1 = z1, fz2 = z2, fz3 = z3;

  //point1记录y坐标最小者
  if (point1.y > point2.y) {
    point1 = pt2;
    point2 = pt1;
    color1 = clr2;
    color2 = clr1;
    fz1 = z2;
    fz2 = z1;
  }

  if (point1.y > point3.y) {
    POINT ptTemp = point1;
    FRGBA clrTemp = color1;
    float zTemp = fz1;
    point1 = point3;
    color1 = color3;
    fz1 = fz3;
    point3 = ptTemp;
    color3 = clrTemp;
    fz3 = zTemp;
  }

  //point2记录y坐标最大者
  if (point2.y < point3.y) {
    POINT ptTemp = point2;
    FRGBA clrTemp = color2;
    float zTemp = fz2;
    point2 = point3;
    color2 = color3;
    fz2 = fz3;
    point3 = ptTemp;
    color3 = clrTemp;
    fz3 = zTemp;
  }

  //第二步,获取边界点的 x 坐标,计算边界颜色
  //12     1 to 2, 13    - 1 to 3, 32    - 3 to 2
  //y坐标之间的距离
  nDy12 = (point2.y - point1.y) + 1;
  nDy13 = (point3.y - point1.y) + 1;
  nDy32 = (point2.y - point3.y) + 1;
  //获取扫描线的 x 坐标
  pnEdge12 = MALLOC(int, nDy12);
  pnEdge13 = MALLOC(int, nDy13);
  pnEdge32 = MALLOC(int, nDy32);
  //获取边界颜色
  //注意,这里支持透明计算,因而颜色用三个分量
  pClrEdge12 = MALLOC(FRGBA, nDy12);
  pClrEdge13 = MALLOC(FRGBA, nDy13);
  pClrEdge32 = MALLOC(FRGBA, nDy32);
  //记录边界深度
  pfzEdge12 = MALLOC(float, nDy12);
  pfzEdge13 = MALLOC(float, nDy13);
  pfzEdge32 = MALLOC(float, nDy32);
  //获取边界及其颜色
  //计算矢量叉积(只需要计算Z分量),以判断三角形的位置关系
  nDelta = ((point3.x - point1.x) * (point2.y - point3.y) - (point2.x - point3.x) * (point3.y - point1.y));

  if (nDelta > 0) {
    CBTriangle_DealEdge(point1.x, point1.y, point2.x, point2.y, fz1, fz2, color1, color2, hDC, pZB, pnEdge12, pClrEdge12, pfzEdge12, G3D_TRIANGLE_ON_LINE_RIGHT);
    CBTriangle_DealEdge(point1.x, point1.y, point3.x, point3.y, fz1, fz3, color1, color3, hDC, pZB, pnEdge13, pClrEdge13, pfzEdge13, G3D_TRIANGLE_ON_LINE_LEFT);
    CBTriangle_DealEdge(point3.x, point3.y, point2.x, point2.y, fz3, fz2, color3, color2, hDC, pZB, pnEdge32, pClrEdge32, pfzEdge32, G3D_TRIANGLE_ON_LINE_LEFT);
  }
  else {
    CBTriangle_DealEdge(point1.x, point1.y, point2.x, point2.y, fz1, fz2, color1, color2, hDC, pZB, pnEdge12, pClrEdge12, pfzEdge12, G3D_TRIANGLE_ON_LINE_LEFT);
    CBTriangle_DealEdge(point1.x, point1.y, point3.x, point3.y, fz1, fz3, color1, color3, hDC, pZB, pnEdge13, pClrEdge13, pfzEdge13, G3D_TRIANGLE_ON_LINE_RIGHT);
    CBTriangle_DealEdge(point3.x, point3.y, point2.x, point2.y, fz3, fz2, color3, color2, hDC, pZB, pnEdge32, pClrEdge32, pfzEdge32, G3D_TRIANGLE_ON_LINE_RIGHT);
  }

  //保证最后一个点也能绘制
  //这是因为,CPLine基元与Windows绘制直线的方式是一致的,即不绘制最后一个点
  zInBuffer = GetDepth(pZB, point2.x, point2.y);

  //如果当前点的z坐标小于深度缓冲区中的相同位置z的坐标
  if (fz2 < zInBuffer) {
    //在颜色缓冲区中绘制第一个点
    CBSetPixel(hDC, point2.x, point2.y, color2.r, color2.g, color2.b, color2.a);
    //重置深度缓冲区的值
    SetDepth(pZB, point2.x, point2.y, fz2);
  }

  //第三个顶点
  if (point3.y == point2.y) {
    zInBuffer = GetDepth(pZB, point3.x, point3.y);

    //如果当前点的z坐标小于深度缓冲区中的相同位置z的坐标
    if (fz3 < zInBuffer) {
      //在颜色缓冲区中绘制第一个点
      CBSetPixel(hDC, point3.x, point3.y, color3.r, color3.g, color3.b, color3.a);
      //重置深度缓冲区的值
      SetDepth(pZB, point3.x, point3.y, fz3);
    }
  }

  //第三步,进行扫描填充
  //扫描分为两个阶段:
  //第一,扫描第一点至第三点之间的区域
  //第二,扫描第三点至第二点之间的区域
  //当前扫描线的y坐标及扫描计数器
  nPlumbScan = 0;

  if (point1.y < point3.y) {
    for (y = point1.y; y < point3.y; y++) {
      //一条扫描线的x坐标
      int x12 = pnEdge12[nPlumbScan];
      int x13 = pnEdge13[nPlumbScan];
      //一条扫描线的深度变化
      float z12 = pfzEdge12[nPlumbScan];
      float z13 = pfzEdge13[nPlumbScan];
      //颜色
      FRGBA clrEdge12 = pClrEdge12[nPlumbScan];
      FRGBA clrEdge13 = pClrEdge13[nPlumbScan];
      //填充
      CBTriangle_Scan(x12, x13, y, z12, z13, clrEdge12, clrEdge13, hDC, pZB);
      nPlumbScan++;
    }//end for y
  }//end (point1.y < point3.y)

  //第二阶段扫描
  //准备数据
  //沿边3-2的计数器
  nPlumbScan32 = 0;

  for (y = point3.y; y <= point2.y; y++) {
    //一条扫描线的x坐标
    int x12 = pnEdge12[nPlumbScan];
    int x32 = pnEdge32[nPlumbScan32];
    //一条扫描线的深度变化
    float z12 = pfzEdge12[nPlumbScan];
    float z32 = pfzEdge32[nPlumbScan32];
    //颜色
    FRGBA clrEdge12 = pClrEdge12[nPlumbScan];
    FRGBA clrEdge32 = pClrEdge32[nPlumbScan32];
    //填充
    CBTriangle_Scan(x12, x32, y, z12, z32, clrEdge12, clrEdge32, hDC, pZB);
    nPlumbScan++;
    nPlumbScan32++;
  }//end for y

  free(pnEdge12);
  free(pnEdge13);
  free(pnEdge32);
  free(pClrEdge12);
  free(pClrEdge13);
  free(pClrEdge32);
  free(pfzEdge12);
  free(pfzEdge13);
  free(pfzEdge32);
}
//绘制线框模型
//pO  三维物体对象
//hDC   颜色缓冲
//pZB   深度缓冲
void FlatWirize(HDDC hDC, ZBuffer* pZB, Object3d* pO)
{
  int i, j;

  //绘制工具:直线基元
  //绘制单色模型
  for (i = 0; i < pO->sListLen; i++) {
    //获取边的顶点索引
    for (j = 0; j < 3; ++j) {
      int j1 = (j == 2) ? 0 : (j + 1);
      int n1 = pO->sList[i].vIndex[j];
      int n2 = pO->sList[i].vIndex[j1];
      Vertex3d v1 = pO->vList[n1];
      Vertex3d v2 = pO->vList[n2];

      if (n1 > n2) {
        continue;
      }

      if (pO->bAutoColor) {
        //获取第一个顶点的颜色代替整条边的颜色
        v1.clr = v2.clr = v1.clr;
      }
      else {
        v1.clr = v2.clr = pO->sList[i].clr;
      }

      //绘制
      CBLine_Draw(hDC, pZB, &v1, &v2);
    }
  }//end for
}
//绘制实体模型, Flat模型
//pO          三维物体对象
//hDC         颜色缓冲
//pZB         深度缓冲
void Flat(HDDC hDC, ZBuffer* pZB, Object3d* pO)
{
  int i;

  //面绘制工具: 三角形基元
  //由顶点决定小面的颜色, 还是由外部决定小面的颜色
  //绘制, 面循环
  for (i = 0; i < pO->sListLen; i++) {
    //获取顶点索引号
    int n1 = pO->sList[i].vIndex[0];
    int n2 = pO->sList[i].vIndex[1];
    int n3 = pO->sList[i].vIndex[2];
    Vertex3d v1 = pO->vList[n1];
    Vertex3d v2 = pO->vList[n2];
    Vertex3d v3 = pO->vList[n3];

    //小面颜色
    FRGBA rgba;

    //获取顶点颜色
    if (pO->bAutoColor) {
      //一般地, 采用第一个顶点的颜色来表示小面的颜色
      //这对模型的顶点安排要求比较高
      //因而, 我们采用三个顶点颜色的平均值来表示小面的颜色
      //在光照条件下, 这样处理也是可以接受的
      //顶点颜色归一化应该在外部进行
      rgba.r = (v1.clr.r + v2.clr.r + v3.clr.r) / 3.0f;
      rgba.g = (v1.clr.g + v2.clr.g + v3.clr.g) / 3.0f;
      rgba.b = (v1.clr.b + v2.clr.b + v3.clr.b) / 3.0f;
      //透明度, 这里也简单地处理成同一值
      rgba.a = (v1.clr.a + v2.clr.a + v3.clr.a) / 3.0f;
    }
    //获取小面颜色
    else {
      rgba = pO->sList[i].clr;
    }

    v1.clr = v2.clr = v3.clr = rgba;
    //绘制
    CBTriangle_Draw(hDC, pZB, &v1, &v2, &v3);

  }//end for
}
//绘制实体模型, Gouraud模型
//pO          三维物体对象
//hDC         颜色缓冲
//pZB         深度缓冲
int Gouraud1(HDDC hDC, ZBuffer* pZB, Object3d* pO)
{
  int i;

  //面绘制工具: 三角形基元
  //绘制，面循环
  for (i = 0; i < pO->sListLen; i++) {
    //获取顶点索引号
    int n1 = pO->sList[i].vIndex[0];
    int n2 = pO->sList[i].vIndex[1];
    int n3 = pO->sList[i].vIndex[2];
    //绘制
    CBTriangle_Draw(hDC, pZB, pO->vList + n1, pO->vList + n2, pO->vList + n3);
  }//end for

  return 0;
}