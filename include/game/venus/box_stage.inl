// ****************************************************************************************************
// 文件: box_stage.cpp
// 注释:
// 推箱子基础函数模块, 包括尺寸计算, 指针修正, 坐标排序, 文件加载及内存申请释放
// ****************************************************************************************************
#include <malloc.h> // malloc, realloc
#include <stdio.h> // FILE
#define __VENUS_DLL_INC_ // DLL内部编译
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
UINT fnStageSize(UINT dwCount, UINT dwSizeX, UINT dwSizeY)
{
  UINT dwSize;
  dwSize = sizeof(STAGE) - sizeof(UINT); // 结构尺寸
  dwSize += dwSizeX * dwSizeY; // 单元矩阵
  dwSize += sizeof(SPOT) * dwCount * 2; // 箱子坐标
  dwSize += sizeof(MOVE) * dwCount * 4; // 可行走法
  return dwSize;
}
void fnStagePtr(PSTAGE pStage)
{
  UINT dwNum = pStage->SizeX * pStage->SizeY;
  // 修正内部指针
  pStage->Stars = (PSPOT)&pStage->Matrix[dwNum]; // 箱子坐标, 指向内部
  dwNum = pStage->Count;
  pStage->Series = &pStage->Stars[dwNum]; // 箱子排序, 指向内部
  pStage->Moves = (PMOVE)&pStage->Series[dwNum]; // 可行走法, 指向内部
  return;
}
int fnStageSort(PSPOT pStar1, PSPOT pStar2, PSPOT pCache, UINT dwCount)
{
  __int64 nValue;
  union {
    UINT dwLoop1;
    int dwRet;
  };
  UINT dwLoop2;
  //if(dwCount == 0) return 0;
  // 排序检测两列坐标是否相同(或排序存储pStar1到pCache)
  if (pStar2 == NULL) {
    //if(pCache == NULL) return -1;
    if ((UINT)pStar1 != (UINT)pCache) {
      // 指针不同, 先复制数据
      V32Copy(pCache, pStar1, dwCount * sizeof(SPOT));
    }
    // 排序pStar1并存储
    for (dwLoop1 = 0; dwLoop1 < dwCount - 1; dwLoop1++) {
      for (dwLoop2 = dwLoop1 + 1; dwLoop2 < dwCount; dwLoop2++) {
        // 从小到大排序
        nValue = pCache[dwLoop2].Value;
        if (nValue < pCache[dwLoop1].Value) {
          pCache[dwLoop2].Value = pCache[dwLoop1].Value;
          pCache[dwLoop1].Value = nValue;
        }
      }
    }
    return 1;
  }
  // 比较pStar1到pStar2
  if (pCache == NULL) {
    // 直接比较
    dwLoop1 = 0;
    while (dwLoop1 < dwCount) {
      if (pStar1->Value != pStar2->Value) {
        return -2; // 彼此差异
      }
      dwLoop1++;
      pStar1++; // 使用指针注意递进
      pStar2++;
    }
    return 2; // 完全一致
  }
  // 排序pStar1到pCache并与pStar2比较
  dwRet = fnStageSort(pStar1, NULL, pCache, dwCount);
  if (dwRet <= 0) {
    return -3;
  }
  return fnStageSort(pCache, pStar2, NULL, dwCount);
}
PBYTE fnStageFile(void* lpVoid, PSPOT pSpot)
{
  BYTE* pData;
  FILE* pfx;
  UINT dwNum;
  int dwRet;
  pfx = fopen((const char*)lpVoid, "rb");
  if (pfx == NULL) {
    fnStageCode(SEC_FILE_OPEN);
    return NULL; // 文件打开失败
  }
  fread(&dwNum, sizeof(UINT), 1, pfx);
  if (dwNum != SFH_HEADER) {
    fclose(pfx);
    fnStageCode(SEC_FILE_SIGN);
    return NULL; // .box文件头部
  }
  dwRet = fread(&dwNum, sizeof(UINT), 1, pfx); // 版本(预留)
  dwRet = fread(&pSpot->X, sizeof(long), 1, pfx);
  dwRet = fread(&pSpot->Y, sizeof(long), 1, pfx);
  dwNum = pSpot->X * pSpot->Y;
  if (dwNum <= 16 || pSpot->X < 0) {
    fclose(pfx);
    fnStageCode(SEC_FILE_SIZE);
    return NULL; // 四行四列及以内的地图无意义
  }
  pData = (PBYTE)malloc(dwNum);
  if (pData == NULL) {
    fclose(pfx);
    fnStageCode(SEC_CACHE_NULL);
    return NULL; // 内存不足
  }
  dwRet = fread(pData, sizeof(BYTE), dwNum, pfx);
  fclose(pfx); // 关闭文件
  if ((UINT)dwRet != dwNum) {
    SafeFree(pData);
    fnStageCode(SEC_FILE_SIZE);
    return NULL; // 数据损坏
  }
  return pData; // pSpot保持场景尺寸
}
// 要求提供有效匹配的场景数据
PSTAGE fnStageAlloc(PBYTE pData, PPROP pProp)
{
  PSTAGE pStage;
  UINT dwCount;
  UINT dwSize;
  UINT dwFlags = SGF_PRIME; // 初始场景
  if (pProp == NULL) {
    fnStageCode(SEC_PARAM_NULL);
    return NULL; // 无效参数
  }
  dwCount = 0; // 没有实际坐标当作没有箱子
  if (pProp->Spots != NULL) {
    dwCount = pProp->Count;
    dwFlags |= SGF_READY; // 场景已就绪: SGF_BASIC | SGF_EXTRA
  }
  dwSize = fnStageSize(dwCount, pProp->SizeX, pProp->SizeY); // 内存大小
  pStage = (PSTAGE)malloc(dwSize);
  if (pStage == NULL) {
    fnStageCode(SEC_CACHE_NULL);
    return NULL; // 内存不足
  }
  //V32Clear(pStage, 0, sizeof(STAGE)); // memset, 自动计算DWORD数量(只清零结构体)
  pStage->Volume = dwSize;
  pStage->Flags = dwFlags;
  pStage->Stars = NULL; // 等候修正
  pStage->Series = NULL; // 等候修正
  pStage->Count = dwCount; // 箱子数量
  pStage->Value = pProp->Value;
  pStage->Hash = pProp->Hash;
  pStage->Prev = NULL;
  pStage->Next = NULL;
  pStage->Host = NULL;
  pStage->SizeX = pProp->SizeX;
  pStage->SizeY = pProp->SizeY;
  pStage->PosX = pProp->RoleX;
  pStage->PosY = pProp->RoleY;
  pStage->AutoX = 0;
  pStage->AutoY = 0;
  pStage->Moves = NULL; // 等候修正
  pStage->Range = 0; // 暂无可行走法
  pStage->Index = 0;
  pStage->Slaves = 0; // 无子场景
  pStage->Layer = 0; // 初始一步
  //pStage->Data = 0; // 清零数据
  if (pData != NULL) {
    V32Copy(pStage->Matrix, pData, pProp->SizeX * pProp->SizeY); // 复制矩阵数据
  }
  else {
    V32Clear(pStage->Matrix, 0, pProp->SizeX * pProp->SizeY); // 清零矩阵数据
  }
  if (dwCount != 0) {
    fnStagePtr(pStage); // 修正内部指针
    V32Copy(pStage->Stars, pProp->Spots, sizeof(SPOT) * dwCount * 2); // 复制箱子坐标(包括排序的)
  }
  return pStage; // 返回指针
}
int fnStageErase(PSTAGE pStage, UINT dwFlags)
{
  if (pStage == NULL) {
    fnStageCode(SEC_PARAM_NULL);
    return 0;
  }
  // 检测场景
  if (pStage->Flags & SGF_ARRAY) {
    fnStageCode(SEC_STAGE_FLAG);
    return -1; // 子集场景不能释放
  }
  // 递归释放子场景
  while (pStage->Next) {
    fnStageErase(pStage->Next, 0);
    //pStage->Next = NULL;
  }
  // 断开链表
  if (pStage->Prev) {
    pStage->Prev->Next = NULL;
  }
  // 释放场景
  SafeFree(pStage);
  return 1;
}

