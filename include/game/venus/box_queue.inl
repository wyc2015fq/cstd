// ****************************************************************************************************
// 文件: box_queue.cpp
// 注释:
// 自定义队列模块, 用队列结合循环代替递归, 队列法则是: 先进先出, 保证更低步数局面先被分析
// ****************************************************************************************************
#include <malloc.h> // malloc, realloc
#define __VENUS_DLL_INC_ // DLL内部编译
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
PQUEUE fnQueueInit(PSTAGE pStage, PSTACK pStack, PSHOT pShot, UINT dwSize, UINT dwCount, UINT dwFlags)
{
  PQUEUE pQueue;
  union {
    UINT dwVolume;
    int dwRet;
  };
  if (dwSize == 0) {
    dwSize = pStage->Volume; // 场景完整
  }
  dwVolume = dwSize * dwCount; // 队列个数
  dwVolume += sizeof(QUEUE) - sizeof(UINT); // 队列首部
  pQueue = (PQUEUE)malloc(dwVolume);
  if (pQueue == NULL) {
    fnStageCode(SEC_CACHE_NULL);
    SafeFree(pStack->Stars);
    return NULL; // 内存不足
  }
  V32Copy(pQueue->Dummy, pStage, pStage->Volume);
  pStage = (PSTAGE)pQueue->Dummy; // 更新指针
  pStage->Volume = dwSize; // 更新场景尺寸
  pStage->Flags |= SGF_EXTRA; // 数据完整
  pStage->Flags |= SGF_ARRAY; // 队列元素(标识不可释放)
  if (dwFlags & SMF_FLAG_TEST) {
    pStage->Flags |= SGF_DEBUG;
  }
  else {
    pStage->Flags &= ~SGF_DEBUG;
  }
  pStage->Prev = NULL; // 清理队列(如果是游戏中, 可能有链表)
  pStage->Next = NULL;
  pStage->Host = NULL;
  fnStagePtr(pStage); // 修正内部指针
  if (pStack->Stars != NULL) {
    // 复制箱子坐标(包括排序的)
    V32Copy(pStage->Stars, pStack->Stars, sizeof(SPOT) * pStage->Count * 2);
  }
  SafeFree(pStack->Stars);
  // 初始化堆栈和快照(无效局面)
  pStack->Count = pStage->SizeX * pStage->SizeY;
  pStack->Stars = (PSPOT)malloc(sizeof(SPOT) * pStack->Count);
  if (pStack->Stars == NULL) {
    SafeFree(pQueue);
    fnStageCode(SEC_CACHE_NULL);
    return NULL; // 内存不足
  }
  pShot->Count = 0;
  pShot->Value = 0;
  if (dwFlags & SMF_FLAG_HASH) {
    pShot->Size = sizeof(SNAP) - sizeof(SPOT); // 使用哈希, 没有坐标数据
    pStage->Flags |= SGF_CRC32;
  }
  else {
    pShot->Size = sizeof(SNAP) + sizeof(SPOT) * (pStage->Count - 1); // 绝对比较
    pStage->Flags &= ~SGF_CRC32;
  }
  pShot->Snaps = NULL;
  // 初始化队列
  pQueue->Volume = dwVolume; // 队列尺寸
  pQueue->Size = dwSize; // 元素尺寸
  pQueue->Count = dwCount; // 元素上限
  pQueue->Value = 1;
  pQueue->Used = 1;
  pQueue->Active = pStage;
  pQueue->Backup = pStage;
  pQueue->Stages = NULL;
  pQueue->Shots = pShot; // 无效局面
  pQueue->Stacks = pStack; // 扫描堆栈
  dwRet = fnStageScan(pQueue, pStage); // 扫描第一个场景
  return pQueue;
}
// 检查一个元素是否在缓冲区范围内
int fnQueueRange(PQUEUE pQueue, PSTAGE pStage)
{
  union {
    PSTAGE pStage1;
    BYTE* pDummy1;
    UINT dwNum1;
  };
  union {
    PSTAGE pStage2;
    BYTE* pDummy2;
    UINT dwNum2;
  };
  union {
    PSTAGE pStage3;
    BYTE* pDummy3;
    UINT dwNum3;
  };
  pStage1 = pStage;
  pDummy2 = pQueue->Dummy;
  pDummy3 = pDummy2 + pQueue->Size * pQueue->Count;
  if (dwNum1 < dwNum2 || dwNum1 >= dwNum3) {
    return 0; // 必须缓存区内元素指针
  }
  pStage2 = pQueue->Active;
  while (pStage2) {
    if (pStage2 == pStage) {
      return 1;
    }
    pStage2 = pStage2->Next;
  }
  return -1;
}
// 弹出一个元素
PSTAGE fnQueuePop(PQUEUE pQueue)
{
  PSTAGE pStage = pQueue->Active;
  if (pStage) {
    // 第一个元素有效, 指向下一个
    pQueue->Active = pStage->Next;
  }
  return pStage;
}
// 追加一个元素
int fnQueuePush(PQUEUE pQueue, PSTAGE pStage)
{
  pStage->Prev = pQueue->Backup;
  pStage->Next = NULL;
  if (pQueue->Backup) {
    pQueue->Backup->Next = pStage;
  }
  pQueue->Backup = pStage; // 加到末尾
  return 1;
}
// 申请一个元素
PSTAGE fnQueueApply(PQUEUE pQueue)
{
  union {
    PSTAGE pStage;
    BYTE* pDummy;
  };
  pStage = pQueue->Stages;
  if (pStage) {
#ifdef _DEBUG
    fnPrint("回收场景=0x%08X.\r\n", pStage);
#endif
    // 存在过期场景, 使用过期场景
    pQueue->Stages = pStage->Next; // 下一个前移
    if (pStage->Next) {
      pStage->Next->Prev = NULL;
    }
  }
  else {
    // 申请一个元素
    if ((pQueue->Value + 1) >= pQueue->Count) {
      return NULL; // 堆栈耗尽
    }
    pDummy = pQueue->Dummy;
    pDummy += pQueue->Size * pQueue->Value; // 每个元素必须相同尺寸
    pQueue->Value++; // 增加一个
  }
  pQueue->Used++; // 已用个数增加一个
  return pStage;
}
// 删除一个元素, 返回链表下个元素
PSTAGE fnQueueRemove(PQUEUE pQueue, PSTAGE pStage)
{
  union {
    PSTAGE pPrev;
    PSTAGE pStages;
  };
  PSTAGE pNext = pStage->Next;
  //if(!fnQueueRange(pQueue, pStage))
  //{
  // return NULL;
  //}
  // 连接首尾
  pPrev = pStage->Prev;
  if (pPrev) {
    pPrev->Next = pNext;
  }
  if (pNext) {
    pNext->Prev = pPrev;
  }
  // 检查指针
  if (pStage == pQueue->Active) {
    pQueue->Active = pNext;
  }
  if (pStage == pQueue->Backup) {
    pQueue->Backup = pPrev;
  }
  // 加到过期
  pStages = pQueue->Stages;
  pStage->Prev = NULL;
  pStage->Next = pStages;
  //pStage->Host = NULL;
  if (pStages) {
    pStages->Prev = pStage;
  }
  pQueue->Stages = pStage;
  pQueue->Used--; // 已用个数减少一个
  return pNext;
}

