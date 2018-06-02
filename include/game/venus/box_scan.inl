// ****************************************************************************************************
// 文件: box_scan.cpp
// 注释:
// 扫描线填充算法模块, 用循环代替递归, 从角色位置开始填充场景可通行单元
// ****************************************************************************************************
#define __VENUS_DLL_INC_ // DLL内部编译
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
// 扫描线填充(用循环代替递归, 玩家必须在边界封闭的曲线内)
int fnStageScan(PQUEUE pQueue, PSTAGE pStage)
{
  UINT x0, xl, xr, y0, xid;
  UINT flag, c;
  PSTACK s;
  PSPOT p;
  union {
    UINT* pData;
    BYTE* pNum;
  };
  UINT X, Y;
  int i;
  //if((pStage->Flags & SGF_BASIC) == 0)
  //{
  // return 0; // 新建场景当作无法填充(外部控制)
  //}
  // 首先清零非类型位
  Y = pStage->SizeX * pStage->SizeY;
  X = Y % 4;
  pNum = pStage->Matrix;
  while (X--) {
    *pNum++ &= SMT_FILTER; // 清零非类型信息
  }
  Y /= 4;
  while (Y--) {
    *pData++ &= SMT_MATRIX; // 清零非类型信息
  }
  // 清空堆栈, 种子入栈
  s = pQueue->Stacks;
  p = s->Stars;
  p->X = pStage->PosX;
  p->Y = pStage->PosY;
  c = 1; // 堆栈使用一个
  while (c) {
    X = p->X;
    Y = p->Y;
    p--;
    c--;
    pNum = &pStage->Matrix[Y * pStage->SizeX + X];
    *pNum |= SMT_OPENED; // Me.PSet (x0, Y), newvalue
    x0 = X + 1;
    pNum++;
    // 填充右边不是箱子也不是边界的单元
    while ((*pNum & SMT_MASKED) == 0) { // Me.Point(x0, Y) <> boundaryvalue
      //if(x0 >= pStage->SizeX) break; // 到最右边(地图控制)
      *pNum |= SMT_OPENED;
      pNum++;
      x0++;
    }
    xr = x0 - 1; // 最右坐标
    x0 = X - 1;
    pNum = &pStage->Matrix[Y * pStage->SizeX + x0];
    // 填充左边不是箱子也不是边界的单元
    while ((*pNum & SMT_MASKED) == 0) { // Me.Point(x0, Y) <> boundaryvalue
      //if(x0 < 0) break; // 到最左边(地图控制)
      *pNum |= SMT_OPENED;
      pNum--;
      x0--;
    }
    xl = x0 + 1; // 最左象素
    // 检查上一条扫描线和下一条扫描线，若存在非边界且未填充的象素，则选取代表各连续区间的种子象素入栈。
    y0 = Y;
    for (i = 1; i >= -1; i -= 2) {
      x0 = xr;
      Y = y0 + i;
      while (x0 >= xl) {
        flag = 0; // 向左传递未填充的点直到边界, 记录最后一个点的X坐标
        pNum = &pStage->Matrix[Y * pStage->SizeX + x0]; // c = Me.Point(x0, Y)
        //while(((*pNum & SMT_MASKED) == 0) && ((*pNum & SMT_OPENED) == 0) && (x0 >= xl))
        while (((*pNum & SMT_OPNMSK) == 0) && (x0 >= xl)) {
          // (c <> boundaryvalue) And (c <> newvalue) And (x0 >= xl)
          if (flag == 0) {
            flag = 1;
            xid = x0;
          }
          pNum--; // c = Me.Point(x0, Y)
          x0--;
        }
        // 将最右侧可填充象素压入栈中, if(c >= s.Count)
        if (flag == 1) {
          p++;
          p->X = xid;
          p->Y = Y;
          c++; // s.push(Point(xid,y));
          flag = 0;
        }
        // 检查当前填充行是否被中断，若被中断，寻找左方第一个可填充象素
        pNum = &pStage->Matrix[Y * pStage->SizeX + x0]; // c = Me.Point(x0, Y)
        while (*pNum & SMT_OPNMSK) {
          // (c = boundaryvalue) Or (c = newvalue) '判断当前点是否为边界或箱子 或 判断当前点是否为已填充点
          if (x0 == 0) {
            break; // 到最左边(...)
          }
          pNum--;
          x0--; // 若当前点为边界点或已填充点，根据前面的判断，当前点必然未超出左边界，则当前点向左移动
        }
      } // loop while(x0 >= xl)
    } // next for(i = 1; i >= -1; i -= 2)
  } // loop while(!s.isempty())
  return 1;
}

