// ****************************************************************************************************
// 文件: box_hash.cpp
// 注释:
// 推箱子属性计算模块, 根据场景计算包括指纹在内的结构体属性
// ****************************************************************************************************
#include <malloc.h> // malloc, realloc, free
//#include <stdio.h> // FILE
#define __VENUS_DLL_INC_ // DLL内部编译
#include "api.h"
#include "debug.h"
using namespace Valiant::Cipher;
// globals
CRC32 g_Hash = {{0}, 0};
UINT fnStageHash(PSPOT pStar, PSPOT pSort, UINT dwCount)
{
  //if(pStar == NULL || pSort == NULL || dwCount == 0)
  //{
  // return 0; // 外部控制指针有效
  //}
  // 排序pStar到pSort(指针相同则直接排序)
  fnStageSort(pStar, NULL, pSort, dwCount);
  if (g_Hash.Seed == 0) {
    // 自动初始化CRC余式表
    InitCyclicTable(g_Hash.Table, CRC_INIT_SEED);
    g_Hash.Seed = CRC_INIT_SEED;
  }
  return CalcCyclicValue(g_Hash.Table, pSort, sizeof(SPOT) * dwCount);
}
int fnStageProp(PBYTE pMatrix, PPROP pProp)
{
  UINT dwLoopX;
  UINT dwLoopY;
  long dwRoles;
  UINT dwNum1; // 箱子数量
  UINT dwNum2; // 目的数量
  UINT dwNum3; // 归位数量
  PSPOT pStars, ps;
  BYTE bValue;
  if (pProp == NULL) {
    fnStageCode(SEC_PARAM_NULL);
    return 0; // 无效参数
  }
  // 初始化没有场景数据
  pProp->Count = 0;
  pProp->Value = 0;
  pProp->Hash = 0;
  pProp->RoleX = 0;
  pProp->RoleY = 0;
  pProp->Spots = NULL;
  if (pMatrix == NULL) {
    return 1; // 没有场景数据, 计算完成(不算错误)
  }
  // 开始分析场景
  dwNum1 = dwNum2 = dwNum3 = dwRoles = 0;
  pStars = NULL;
  for (dwLoopY = 0; dwLoopY < pProp->SizeY; dwLoopY++) {
    // 每一行
    for (dwLoopX = 0; dwLoopX < pProp->SizeX; dwLoopX++) {
      // 每一行
      bValue = pMatrix[dwLoopY * pProp->SizeX + dwLoopX];
      if (bValue & SMT_PORTER) {
        // 角色位置
        if (dwRoles > 0) {
          SafeFree(pStars);
          fnStageCode(SEC_STAGE_ROLE);
          return -1; // 出现多个角色(可能已有箱子坐标)
        }
        pProp->RoleX = dwLoopX;
        pProp->RoleY = dwLoopY;
        dwRoles++;
        if (bValue & SMT_TARGET) {
          dwNum2++; // 角色目的重叠
        }
        continue; // 角色不可能与箱子重叠
      }
      if (bValue & SMT_OBJECT) {
        // 箱子位置
        if (pStars == NULL) {
          // 第一个箱子
          ps = (PSPOT)malloc(sizeof(SPOT) * 2);
          if (ps == NULL) {
            //SafeFree(pStars);
            fnStageCode(SEC_CACHE_NULL);
            return -2; // 内存不足(首次申请, 无需释放)
          }
        }
        else {
          // 新增的箱子
          ps = (PSPOT)realloc(pStars, sizeof(SPOT) * (dwNum1 + 1) * 2);
          if (ps == NULL) {
            SafeFree(pStars);
            fnStageCode(SEC_CACHE_NULL);
            return -3; // 内存不足(可以合并)
          }
        }
        ps[dwNum1].X = dwLoopX;
        ps[dwNum1].Y = dwLoopY;
        pStars = ps; // Update
        // 计数
        dwNum1++; // 增加箱子数量
        if (bValue & SMT_TARGET) {
          dwNum2++; // 增加目标数量
          dwNum3++; // 增加归位数量
          continue; // 归位包括目标, 不再检测单纯目标
        }
      }
      if (bValue & SMT_TARGET) {
        dwNum2++; // 增加目标数量
      }
    }
  }
  if (dwNum1 != dwNum2 || dwNum1 <= 0) {
    SafeFree(pStars);
    fnStageCode(SEC_STAGE_VARY);
    return -4; // 箱子和目标数量不相等
  }
  // 赋值场景数据(角色坐标已经赋值)
  pProp->Count = dwNum1; // dwNum2;
  pProp->Value = dwNum3;
  pProp->Hash = fnStageHash(pStars, &pStars[dwNum1], dwNum1); // Hash of sorted box position
  if ((pProp->Flags & SPF_SPOT_ARRAY) == 0) {
    SafeFree(pStars); // 不保存坐标数据
    return 2;
  }
  pProp->Spots = pStars;
  return 3;
}

