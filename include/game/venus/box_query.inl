// ****************************************************************************************************
// 文件: box_query.cpp
// 注释:
// 推箱子场景查询函数模块, 查询场景有关属性及成员
// ****************************************************************************************************
#include <malloc.h> // malloc, realloc
#include <stdio.h> // FILE
#define __VENUS_DLL_INC_ // DLL内部编译
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
// 根据名称获取偏移量(仅限32位成员)
long fnStageIndex(void* lpVoid)
{
  return 0;
}
int fnStageEdit(PSTAGE pStage, void* lpVoid, long dwPosX, long dwPosY)
{
  // 编辑地图单元或者全部数据
  if (pStage == NULL) {
    fnStageCode(SEC_PARAM_NULL);
    return 0;
  }
  if (lpVoid == NULL) {
    return pStage->Matrix[dwPosY * pStage->SizeX + dwPosX]; // 读取单元尺寸
  }
  // 设置矩阵内容(自动转为新建场景)
  if (dwPosX < 0 || dwPosX >= pStage->SizeX) {
    fnStageCode(SEC_PARAM_MANY);
    return -1;
  }
  if (dwPosY < 0 || dwPosY >= pStage->SizeY) {
    fnStageCode(SEC_PARAM_MANY);
    return -2;
  }
  pStage->Matrix[dwPosY * pStage->SizeX + dwPosX] = *(BYTE*)lpVoid;
  pStage->Flags = SGF_PRIME;
  return 1; // 单个
}
int fnStageQuery(PSTAGE pStage, void* lpVoid, long dwPropId, UINT dwFlags)
{
  PROP prp;
  union {
    PMOVE pMove;
    PSPOT pPos;
    BYTE* pNum;
  };
  union {
    UINT dwLoop;
    int dwRet;
  };
  if (pStage == NULL) {
    fnStageCode(SEC_PARAM_NULL);
    return 0;
  }
  // 读取当前移动路径(或设置自定义寻路算法)
  switch (dwPropId) {
  case SPF_SAFE_CHECK: // 检测指针
    try {
      pMove = &pStage->Moves[pStage->Count - 1];
    }
    catch (...) { // EXCEPTION_EXECUTE_HANDLER
      return -1; // 指针无效, 忽略异常
    }
    return 1;
  case SPF_UNIT_VALUE: // 计算指定位置箱子索引
    pPos = (PSPOT)lpVoid;
    dwLoop = pStage->Count;
    while (dwRet >= 0) {
      if (pStage->Stars[dwLoop].Value == pPos->Value) {
        break;
      }
      dwRet--;
    }
    return dwRet;
  case SPF_LAST_ERROR: // 最后的错误码
    dwLoop = fnStageCode(SEC_ERROR_DUMP);
    *(UINT*)lpVoid = dwLoop;
    return 2;
  case SPF_PREV_STAGE: // 返回上一个场景(撤销, 删除本场景及以后的场景)
    if (pStage->Prev == NULL) {
      return 0;
    }
    *(PSTAGE*)lpVoid = pStage->Prev;
    fnStageErase(pStage, 0);
    return 1;
  case SPF_NEXT_STAGE: // 申请下一个场景(移动)
    if (pStage->Next) {
      fnStageErase(pStage->Next, 0); // 已有, 先释放
    }
    pStage->Next = (PSTAGE)malloc(pStage->Volume);
    if (pStage->Next == NULL) {
      return 0;
    }
    V32Copy(pStage->Next, pStage, pStage->Volume);
    pStage->Next->Prev = pStage;
    pStage->Next->Next = NULL; // ...
    *(PSTAGE*)lpVoid = pStage->Next;
    return 1;
  case SPF_HEAP_STAGE: // 压缩场景, 当一个场景超过4K, 删除坐标后面的数据, realloc
    return 0;
  case SPF_CALC_STAGE: // 计算场景参数(新建场景获取玩家位置, 箱子数量等, 不保存坐标数据)
    if (pStage->Flags & SGF_BASIC) {
      return -100; // 场景基础数据已经计算
    }
    //V32Clear32(&prp, 0, sizeof(prp));
    prp.Flags = 0; // 无需返回箱子坐标
    prp.SizeX = pStage->SizeX;
    prp.SizeY = pStage->SizeY;
    dwRet = fnStageProp(pStage->Matrix, &prp);
    if (dwRet <= 0) {
      return dwRet; // 场景无效
    }
    pStage->Count = prp.Count;
    pStage->Value = prp.Value;
    pStage->Hash = prp.Hash;
    pStage->Position.Value = prp.Role.Value;
    pStage->Flags |= SGF_BASIC; // 基础场景
    return 1;
  case SPF_PUSH_STAGE: // 移动场景
    return fnStageProbe(pStage, *(UINT*)lpVoid);
  case SPF_PATH_STAGE: // 自动寻路
    return fnStageStars(pStage, (PSPOT)lpVoid);
  default: // 直接定位成员
    if (lpVoid == NULL) {
      return -2;
    }
    pNum = (BYTE*)pStage;
    pNum += dwPropId;
    //V32Copy(lpVoid, pNum, sizeof(UINT)); // 逐个UINT复制
    *(PUINT)lpVoid = *pNum;
    break;
  }
  return sizeof(STAGE);
}

