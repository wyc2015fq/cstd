// ****************************************************************************************************
// 文件: alpha.cpp
// 注释:
// A*寻路算法模块, 判断两点之间是否通行, 并得到路径和各节点的权值
// 扩展不保存路径提速, 不使用Dijkstra, 参见http://www.2cto.com/kf/201104/87378.html
// ****************************************************************************************************
//#include <stdlib.h>
#include <malloc.h>
#define __VENUS_DLL_INC_ // DLL内部编译
#include "api.h"
//#include "debug.h"
using namespace Valiant::Gaming;
using namespace Valiant;
// 创建一个场景, 并初始化
PMAZE fnAlphaInit(long dwSizeX, long dwSizeY, long dwMaxCost, UINT dwFlags)
{
  UINT dwSize;
  PMAZE p;
  long dwCount = dwSizeX * dwSizeY;
  dwSize = sizeof(MAZE) - sizeof(UINT);
  dwSize += dwCount * sizeof(KNOT);
  dwSize += dwCount * sizeof(SPOT);
  dwSize += dwCount * sizeof(MARK);
  dwSize += dwCount;
  p = (PMAZE)malloc(dwSize);
  if (p == NULL) {
    return NULL;
  }
  V32Clear(p, 0, dwSize); // 清零全部数据
  if (dwMaxCost <= 0) {
    dwMaxCost = dwCount * CDC_PROP_NEAR; // 自动相邻格子逐个走遍
  }
  p->Flags = dwFlags;
  p->SizeY = dwSizeY;
  p->SizeX = dwSizeX;
  p->Range = dwMaxCost; // 最大消耗(距离值)
  p->Count = dwCount; // 节点数量
  p->Value = 0; // 无解
  p->Knots = (PKNOT)&p->Matrix[p->Count];
  p->Spots = (PSPOT)&p->Knots[p->Count];
  p->Marks = (PMARK)&p->Spots[p->Count];
  p->Scene = NULL; // 无外部数据
  return p;
}
// 读写场景数据, 坐标全负读取, 一个负值设定外部指针, 界内坐标设定
int fnAlphaEdit(PMAZE pMaze, long dwPosX, long dwPosY, void* lpValue)
{
  PBYTE p;
  if (pMaze == NULL) {
    return 0;
  }
  if (dwPosX < 0 && dwPosY < 0) {
    p = pMaze->Scene;
    if (p == NULL) {
      p = pMaze->Matrix; // 没有外部数据
    }
    *(PBYTE)lpValue = ArrayItem2(p, pMaze->SizeX, dwPosX, dwPosY);
    return 2;
  }
  if (dwPosX < 0 || dwPosY < 0) {
    pMaze->Scene = (PBYTE)lpValue; // 设置外部数据指针
    return 3;
  }
  // 设置字节数值
  if (lpValue == NULL || dwPosX >= pMaze->SizeX || dwPosY >= pMaze->SizeY) {
    //V32Copy(pMaze->Matrix, lpValue, pMaze->Count);
    return -1;
  }
  p = pMaze->Scene;
  if (p == NULL) {
    p = pMaze->Matrix;
  }
  ArrayItem2(p, pMaze->SizeX, dwPosX, dwPosY) = *(PBYTE)lpValue; // 写入
  return 1;
}
inline long AlphaCalc(PSPOT pSpot1, PSPOT pSpot2)
{
  // 直接使用坐标差和代替三角函数开方
  if (pSpot1->X > pSpot2->X) {
    if (pSpot1->Y > pSpot2->Y) {
      return (pSpot1->X - pSpot2->X) + (pSpot1->Y - pSpot2->Y);
    }
    else {
      return (pSpot1->X - pSpot2->X) + (pSpot2->Y - pSpot1->Y);
    }
  }
  else {
    if (pSpot1->Y > pSpot2->Y) {
      return (pSpot2->X - pSpot1->X) + (pSpot1->Y - pSpot2->Y);
    }
    else {
      return (pSpot2->X - pSpot1->X) + (pSpot2->Y - pSpot1->Y);
    }
  }
  //return dX + dY;
}
inline void AlphaNode(PMAZE pMaze, long dwIndex, long dwPrevId, long dwCost, long dwLast, long dwPosX, long dwPosY)
{
  // 开始和循环各调用一次，dwIndex在调用之后自加，初始为0
  if (dwIndex >= pMaze->Count) {
    return;
  }
  //pMaze->Knots[dwIndex].Spot = *pSpot; // 是否会创建临时结构体??
  pMaze->Knots[dwIndex].Spot.X = dwPosX;
  pMaze->Knots[dwIndex].Spot.Y = dwPosY;
  pMaze->Knots[dwIndex].Prev = dwPrevId;
  if (dwPrevId != -1) {
    pMaze->Knots[dwIndex].Step = pMaze->Knots[dwPrevId].Step + 1;
  }
  else {
    pMaze->Knots[dwIndex].Step = 1;
  }
  pMaze->Knots[dwIndex].Cost = dwCost;
  pMaze->Knots[dwIndex].Last = dwLast * 10; // 每次dwLen重新计算得到格子数
  pMaze->Knots[dwIndex].Flags = SNF_PROP_READY;
  return;
}
//inline BYTE fnAlphaUnit(PMAZE pMaze, PSPOT ps)
//{
// BYTE *pUnit = pMaze->Scene;
// if(pUnit == NULL) pUnit = pMaze->Matrix;
// return pUnit[ps->Y * pMaze->SizeX + ps->X];
//}
//inline PMARK fnAlphaStep(PMAZE pMaze, PSPOT ps)
//{
// return &pMaze->Marks[ps->Y * pMaze->SizeX + ps->X];
//}
// 寻路指定场景
int fnAlphaStar(PMAZE pMaze, PSPOT lpStart, PSPOT lpTarget, long* pdwStep)
{
  PMARK pMark;
  long lCurCost, nMaxCost; // 暂存消耗，最大列表
  long i, j; // temp & looping var
  SPOT tps, cps; // test pos, current pos
  long dwValue;
  long dwIndex = 0;
  long dwNode = 0; // 当前Node数
  long dwLoop;
  long dwLen; // 与终点距离
  // check for memory address accessable
  if (pMaze == NULL || lpStart == NULL || lpTarget == NULL) {
    return 0; // 始末坐标无效
  }
  dwLen = AlphaCalc(lpStart, lpTarget); // dwLen = ΔX + ΔY
  if (dwLen == 0) {
    return -1; // 始末坐标相同
  }
  // zero step memory(cell prop list)
  V32Clear32(pMaze->Marks, 0, pMaze->Count * sizeof(MARK));
  pMaze->Value = 0;
  if (pMaze->Scene == NULL) {
    pMaze->Scene = pMaze->Matrix;
  }
  // 添加第一个点
  dwNode = 0;
  AlphaNode(pMaze, dwNode, -1, 0, dwLen, lpStart->X, lpStart->Y);
  dwNode++;
  // enter loop - check around cells
  while (1) {
    nMaxCost = pMaze->Range; // 不可能比这个大
    dwIndex = -1;
    for (dwLoop = 0; dwLoop < dwNode; dwLoop++) {
      if (pMaze->Knots[dwLoop].Flags != SNF_PROP_ERROR) {
        //找未关闭中最小路程和的点
        lCurCost = pMaze->Knots[dwLoop].Cost + pMaze->Knots[dwLoop].Last;
        if (lCurCost < nMaxCost) {
          nMaxCost = lCurCost; // 调整最大距离
          dwIndex = dwLoop; // 保存节点序号
          //break; // 所有节点都要计算
        }
      }
    }
    if (dwIndex == -1) {
      return -2; // there is no path exist!
    }
    cps.X = pMaze->Knots[dwIndex].Spot.X;
    cps.Y = pMaze->Knots[dwIndex].Spot.Y;
    if ((cps.X == lpTarget->X) && (cps.Y == lpTarget->Y)) {
      break; // 当前点已是终点, 跳出while循环
    }
    //sprintf(szText, "select best cell:[%d,%d] for check:", cps.X, cps.Y);
    for (i = -1; i <= 1; i++) {
      for (j = -1; j <= 1; j++) {
        //if(i == 0 && j == 0) continue; // 允许走对角线，只要两个不同时为零(即不是自身)
        if (i == 0 && j == 0) {
          continue;
        }
        if (i != 0 && j != 0) {
          continue;
        } // 禁止走对角线，必有且只有一个为零{[(i & j) == 0]&&[(i | j) != 0]}
        tps.X = cps.X + i;
        tps.Y = cps.Y + j;
        if (tps.X < 0) {
          continue; // 左边越界
        }
        if (tps.X >= pMaze->SizeX) {
          continue; // 右边越界
        }
        if (tps.Y < 0) {
          continue; // 顶边越界
        }
        if (tps.Y >= pMaze->SizeY) {
          continue; // 底边越界
        }
        // 该点坐标在矩阵范围内
        if (ArrayItem2(pMaze->Scene, pMaze->SizeX, tps.X, tps.Y) & (BYTE)pMaze->Flags) {
          continue; // 消除特殊位以后仍然不可通行(独立程序: 仅允许空格或目标)
        }
        pMark = &ArrayItem2(pMaze->Marks, pMaze->SizeX, tps.X, tps.Y);
        switch (pMark->Flags) {
        case SSF_PROP_UNKNOW: // it'v not been check
          dwValue = pMaze->Knots[dwIndex].Cost;
          //if(i * j != 0)
          // dwValue += CDC_PROP_AWAY; // 横纵坐标都是非零, 对角
          //else
          dwValue += CDC_PROP_NEAR; // 任何一个坐标为零, 相邻
          dwLen = AlphaCalc(&tps, lpTarget); //dwLen = ΔX + ΔY
          AlphaNode(pMaze, dwNode, dwIndex, dwValue, dwLen, tps.X, tps.Y);
          pMark->Flags = SSF_PROP_OPENED; // open it
          pMark->Index = dwNode++;
          // sprintf(szText, "add cell:[%d,%d] for check:", tps.X, tps.Y);
          break;
        case SSF_PROP_OPENED: // this cell is valid
          dwValue = pMark->Index;
          dwLen = pMaze->Knots[dwIndex].Cost;
          dwLen += CDC_PROP_NEAR; // 只能相邻
          if (dwLen < pMaze->Knots[dwValue].Cost) {
            pMaze->Knots[dwValue].Cost = dwLen;
            pMaze->Knots[dwValue].Prev = dwIndex;
            pMaze->Knots[dwValue].Step = pMaze->Knots[dwIndex].Step + 1; //change level
          }
          //sprintf(szText, "change pID for cell:[%d,%d] to %d.", tps.X, tps.Y, nID);
          break;
        default:
          //end if(lpOGrid[tps.X][tps.Y].State..
          break;
        }
        //end if((lpCell..
        //end if((tps.X..
        //end if(((i..
      } //next j
    } //next i
    // it will continue if any path and not at target
    pMark = &ArrayItem2(pMaze->Marks, pMaze->SizeX, cps.X, cps.Y);
    pMaze->Knots[dwIndex].Flags = SNF_PROP_ERROR;
    pMark->Flags = SSF_PROP_CLOSED; // close it
    //sprintf(szText, "close cell:[%d,%d] ok.", cps.X, cps.Y);
  }
  // 函数malloc()和realloc()时间消耗大，用空间换取时间
  dwValue = pMaze->Knots[dwIndex].Cost + pMaze->Knots[dwIndex].Last;
  if (pdwStep == NULL) {
    // 不需要结果详情
    return dwValue; // return cost to get here
  }
  //sprintf(szText, "best path found in cost %d.", dwValue);
  pMaze->Value = pMaze->Knots[dwIndex].Step;
  //SafeFree(pMaze->Spots); // release old path
  //pMaze->Spots = (PSPOT)malloc(pMaze->Value * sizeof(SPOT));
  //if(pMaze->Spots == NULL) return -3; // Out of memory
  // ...
  dwLoop = pMaze->Value;
  *pdwStep = dwLoop; // set the steps
  while (dwLoop > 0) { // it can also base on dwIndex
    dwLoop--;
    //pMaze->m_pStarPath[dwLoop].X = pMaze->Knots[dwIndex].Spot.X;
    //pMaze->m_pStarPath[dwLoop].Y = pMaze->Knots[dwIndex].Spot.Y;
    pMaze->Spots[dwLoop] = pMaze->Knots[dwIndex].Spot;
    dwIndex = pMaze->Knots[dwIndex].Prev; // parent id
    //sprintf(szText, "the %d cell:[%d,%d] added to path.", i, lpRoad[i].X, lpRoad[i].Y);
  }
  return dwValue; // return cost to get here
}
int fnAlphaProp(PMAZE pMaze, UINT dwPropId, long dwIndex)
{
  VAR v;
  if (pMaze == NULL) {
    return -1;
  }
  v.pValue = (void*)pMaze;
  v.Value32 += dwPropId;
  if (dwIndex < 0) {
    return (int) * v.pValue31; // 直接定位成员
  }
  v.Value32 = *v.pValue32;
  v.Value32 += dwIndex;
  return (int) * v.pValue31;
}
// 销毁一个场景, 释放资源
int fnAlphaExit(PMAZE pMaze, UINT dwFlags)
{
  if (pMaze) {
    //SafeFree(pMaze->Spots);
    //pMaze->Value = 0;
    //SafeFree(pMaze->Marks);
    //pMaze->Scene = NULL; // 外部数据指针
    //SafeFree(pMaze->Matrix);
    //SafeFree(pMaze->Knots);
    //pMaze->Count = 0;
    free(pMaze);
    //pMaze = NULL;
  }
  return 1;
}

