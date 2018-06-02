// ****************************************************************************************************
// 文件: box_loop.cpp
// 注释:
// 局面重复检测算法模块, 在队列和失败走法中寻找相同局面, 检测角色是否通行, 可通行则局面重复
// ****************************************************************************************************
#define __VENUS_DLL_INC_ // DLL内部编译
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
// globals
PMAZE g_pScene = NULL;
void fnFreeScene()
{
  if (g_pScene) {
    fnAlphaExit(g_pScene, 0);
    g_pScene = NULL;
  }
}
inline void fnStageScene(PSTAGE pStage)
{
  if (g_pScene == NULL) {
_labCreate:
    g_pScene = fnAlphaInit(pStage->SizeX, pStage->SizeY, 0, SMT_MASKED);
    goto _labCheck;
  }
  if (g_pScene->SizeY != pStage->SizeY || g_pScene->SizeX != pStage->SizeX) {
    fnAlphaExit(g_pScene, 0);
    goto _labCreate;
  }
_labCheck:
  // 外部数据指针(X和Y一个为零)
  fnAlphaEdit(g_pScene, -1, 0, pStage->Matrix);
}
int fnStageStars(PSTAGE pStage, PSPOT pStars)
{
  int dwRet;
  long dwStep;
  if ((pStage->Flags & SGF_BASIC) == 0) {
    return 0; // 新建场景当作无法到达
  }
  fnStageScene(pStage); // 设定外部矩阵
  dwRet = fnAlphaStar(g_pScene, &pStage->Position, pStars, &dwStep); // 直接寻路(lpVoid要求足够的坐标数组)
  if (dwRet <= 0 || dwStep <= 0) {
    return dwRet; // 无法到达
  }
  V32Copy(pStars, g_pScene->Spots, sizeof(SPOT) * dwStep);
  return dwStep;
}
int fnStageLoopHash(PQUEUE pQueue, PSTAGE pStage)
{
  union {
    PSTAGE pNode; // 要比较的场景指针
    PSNAP pSnap; // 要比较的快照指针
    BYTE* pNum; // 用于递进快照指针
  };
  PSHOT pShot;
  UINT dwSize; // 单位快照内存大小
  UINT dwLoop; // 循环变量
  //int dwRet; // 返回值
  //if((pStage->Flags & SGF_READY) != SGF_READY) return -1; // 外部检测
  pNode = pQueue->Active;
  //if(pStage->Flags & SGF_CRC32) // ******** 使用哈希比较 ********
  // 1. 检测现有队列场景, 如果队列中已有则无须追加
  while (pNode) {
    //if(pNode != pStage) // 与自己不比较(场景尚未加入队列)
    if (pNode->Hash == pStage->Hash) {
      // 箱子位置一致, 检测角色通行
      //dwRet = AlphaStar(g_pScene, &pStage->Position, &pNode->Position, NULL);
      //if(dwRet > 0)
      if (GetMatrix(pStage, pNode->PosX, pNode->PosY) & SMT_OPENED) {
        return 1; // 此种走法重复, 应该终止
      }
    }
    pNode = pNode->Next; // 继续下个场景
  }
  // 2. 检测失败快照, 如果已经失败过就不要重蹈覆辙
  pShot = pQueue->Shots;
  dwSize = pShot->Size;
  pSnap = pShot->Snaps;
  dwLoop = pShot->Value;
  while (dwLoop--) {
    if (pSnap->Hash == pStage->Hash) {
      // 箱子位置一致, 检测角色通行
      //dwRet = AlphaStar(g_pScene, &pStage->Position, &pNode->Position, NULL);
      //if(dwRet > 0)
      if (GetMatrix(pStage, pSnap->PosX, pSnap->PosY) & SMT_OPENED) {
        return 2; // 失败走法重现, 应该终止
      }
    }
    pNum += dwSize; // pSnap++;
  }
  return 0;
}
int fnStageLoopSpot(PQUEUE pQueue, PSTAGE pStage)
{
  union {
    PSTAGE pNode; // 要比较的场景指针
    PSNAP pSnap; // 要比较的快照指针
    BYTE* pNum; // 用于递进快照指针
  };
  PSHOT pShot;
  UINT dwSize; // 单位快照内存大小
  UINT dwLoop; // 循环变量
  int dwRet; // 返回值
  //if((pStage->Flags & SGF_READY) != SGF_READY) return -1; // 外部检测
  pNode = pQueue->Active;
  //if((pStage->Flags & SGF_CRC32) == 0) // ******** 完整坐标比较 ********
  // 1. 检测现有队列场景, 此时pStage->Series必须完成排序
  while (pNode) {
    //if(pNode != pStage) // 与自己不比较(场景尚未加入队列)
    // 比较pNode坐标列表与当前排序坐标列表
    dwRet = fnStageSort(pNode->Series, pStage->Series, NULL, pStage->Count);
    if (dwRet == 2) {
      // 箱子位置一致, 检测角色通行
      //dwRet = AlphaStar(g_pScene, &pStage->Position, &pNode->Position, NULL);
      //if(dwRet > 0)
      if (GetMatrix(pStage, pNode->PosX, pNode->PosY) & SMT_OPENED) {
        return 1; // 此种走法重复, 应该终止
      }
    }
#ifdef _DEBUG
    else if (pNode->Hash == pStage->Hash) {
      fnPrint("[警告]布局不同, 但哈希值相同!"); // 调试断点
    }
#endif
    pNode = pNode->Next; // 继续下个场景
  }
  // 2. 检测失败快照, 如果已经失败过就不要重蹈覆辙
  pShot = pQueue->Shots;
  dwSize = pShot->Size;
  pSnap = pShot->Snaps;
  dwLoop = pShot->Value;
  while (dwLoop--) {
    // 比较失败局面的坐标(已排序存储)与当前排序坐标
    dwRet = fnStageSort(pSnap->Stars, pStage->Series, NULL, pStage->Count);
    if (dwRet == 2) {
      // 箱子位置一致, 检测角色通行
      //dwRet = AlphaStar(g_pScene, &pStage->Position, &pNode->Position, NULL);
      //if(dwRet > 0)
      if (GetMatrix(pStage, pSnap->PosX, pSnap->PosY) & SMT_OPENED) {
        return 2; // 失败走法重现, 应该终止
      }
    }
#ifdef _DEBUG
    else if (pSnap->Hash == pStage->Hash) {
      fnPrint("[警告]布局不同, 但哈希值相同!"); // 调试断点
    }
#endif
    pNum += dwSize; // pSnap++;
  }
  return 0;
}
// 判断局面重复(此时局面必须完成扫描, 且已就绪)
int fnStageLoop(PQUEUE pQueue, PSTAGE pStage)
{
  if (pStage->Flags & SGF_CRC32) {
    // ******** 使用哈希比较 ********
    return fnStageLoopHash(pQueue, pStage);
  }
  // ******** 完整坐标比较 ********
  return fnStageLoopSpot(pQueue, pStage);
}

