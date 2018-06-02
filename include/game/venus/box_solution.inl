// ****************************************************************************************************
// 文件: box_solution.cpp
// 注释:
// 推箱子关卡假发算法模块, 自动求解推箱子游戏关卡主逻辑函数代码实现
// ****************************************************************************************************
#include <malloc.h> // malloc, realloc
//#include <stdio.h>
#define __VENUS_DLL_INC_ // DLL内部编译
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
// 生成可行走法列表
UINT fnStageMoves(PSTAGE pStage)
{
  UINT dwFlags;
  UINT dwMoves;
  UINT dwLoop;
  PMOVE pMove = pStage->Moves;
  dwMoves = 0; // 清空走法
  dwLoop = 0;
  while (dwLoop < pStage->Count) {
    dwFlags = SMF_MOVE_RISE; // 初始方向
    pMove->Index = dwLoop;
    while (dwFlags <= SMF_MOVE_FORE) {
      if (fnStagePush(pStage, pMove, dwFlags) > 0) { // 尝试移动
        // 可以移动, 递增有效解法数量, 指向下个走法
        pMove->Flags = SMF_MOVE_NONE; // 尚未使用
        pMove++;
        dwMoves++;
        pMove->Index = dwLoop;
      }
      dwFlags++; // 下一个方向
    }
    dwLoop++; // 下一个箱子
  }
  return dwMoves;
}
// 从队列中申请一个场景, 并以当前场景填充, 扫描后检测重复, 有效则追加到队列
PSTAGE fnStageNext(PQUEUE pQueue, PSTAGE pStage, int* pdwCode)
{
  PSTAGE pNext; // 生成下一步场景
  PMOVE pMove;
  int dwRet;
  pNext = fnQueueApply(pQueue);
  if (pNext == NULL) {
    if (pdwCode) {
      *pdwCode = 0; // 队列耗尽
    }
    fnStageCode(SEC_CACHE_NULL);
    return NULL;
  }
  // 复制上级数据, 修正指针
  V32Copy(pNext, pStage, pStage->Volume);
  pNext->Host = pStage; // .Prev和.Next在丢弃前或加入队列时赋值
  fnStagePtr(pNext); // 修正内部指针
  // 根据当前动作, 推动场景
  pMove = &pStage->Moves[pStage->Index];
#ifdef _DEBUG
  //fnPrint("当前场景=0x%08X, 父级场景=0x%08X, 玩家=(%d, %d), 箱子:\r\n", pStage, pStage->Host, pStage->PosX, pStage->PosY);
  //fnPrintBox(pStage);
  //fnPrint("当前动作: 箱子%d移至(%d, %d), 玩家移至(%d, %d), 寻路坐标为(%d, %d).\r\n\r\n",
  // pMove->Index, pMove->ObjX, pMove->ObjY, pMove->PortX, pMove->PortY, pMove->MoveX, pMove->MoveY);
#endif
  fnStagePush(pNext, pMove, SMF_MOVE_NONE); // 应用走法
  pNext->Range = 0; // 没有走法
  pNext->Index = 0; // ...
  pNext->Layer++; // 步数
  // 扫描线填充可通行单元
  dwRet = fnStageScan(pQueue, pNext);
  // 检验局面重复
  pNext->Hash = fnStageHash(pNext->Stars, pNext->Series, pNext->Count); // 排序计算哈希
  dwRet = fnStageLoop(pQueue, pNext);
  if (dwRet != 0) {
#ifdef _DEBUG
    fnPrint("丢弃重复场景=0x%08X.\r\n", pNext);
#endif
    pNext->Prev = NULL; // 孤立, 防止队列删除(场景尚未加入队列, 只追加到回收链表)
    pNext->Next = NULL;
    fnQueueRemove(pQueue, pNext); // 移除场景
    if (pdwCode) {
      *pdwCode = -1; // 重复局面
    }
    fnStageCode(SEC_ERROR_NONE); // 清零错误
    return NULL;
  }
  // 函数返回
  if (pdwCode) {
    *pdwCode = 1;
  }
  return pNext;
}
// 记录失败场景快照
int fnStageSnap(PQUEUE pQueue, PSTAGE pStage)
{
  union {
    PSNAP pSnap; // 快照指针
    BYTE* pDummy;
  };
  PSHOT pShot;
  UINT dwCount;
  int dwRet;
  //#if 1
  // return 1;
  //#endif
  pShot = pQueue->Shots;
  dwCount = pShot->Count;
  if (pShot->Value >= dwCount) {
    if (dwCount) {
      dwCount *= 2; // 缓存用完, 翻倍重分配缓冲区
      pSnap = (PSNAP)realloc(pShot->Snaps, pShot->Size * dwCount);
      if (pSnap == NULL) {
        goto _labCacheNull;
      }
    }
    else {
      dwCount = pQueue->Count; // 分配队列元素上限个数
      pSnap = (PSNAP)malloc(pShot->Size * dwCount);
      if (pSnap == NULL) {
        goto _labCacheNull;
      }
      //pShot->Value = 0; // 使用第一个(队列初始化时赋值)
    }
    pShot->Count = dwCount; // 更新计数
    pShot->Snaps = pSnap; // 更新指针
  }
  else {
    pSnap = pShot->Snaps;
  }
  //pSnap = &pSnap[pShot->Value]; // 元素不定大小
  pDummy += pShot->Size * pShot->Value;
  pShot->Value++; // 递增失败局面数量
  pSnap->Position = pStage->Position;
  pSnap->Hash = pStage->Hash; // 布局指纹
  if ((pStage->Flags & SGF_CRC32) == 0) {
    // 排序存储坐标详情(只要不归位, 都会撤销回到最初, 不必排序)
    //fnStageSort(pNext->Stars, NULL, pSnap->Stars, pNext->Count);
    V32Copy(pSnap->Stars, pStage->Series, sizeof(SPOT) * pStage->Count);
  }
  // 向上递归父级场景, 所有待分析子场景数量为零的都将记录到失败列表并弹出队列
  pStage = pStage->Host;
  if (pStage != NULL) {
#ifdef _DEBUG
    fnPrint("父级场景=0x%08X, 剩余子级场景数量=%d.\r\n", pStage, pStage->Slaves);
    dwRet = fnQueueRange(pQueue, pStage);
    if (dwRet <= 0) {
      fnPrint("[警告]场景0x%08X不在有效队列中!\r\n", pStage);
      return 1;
    }
    if (pStage->Slaves == 0) {
      fnPrint("[警告]场景0x%08X已经没有子场景!\r\n", pStage);
      return 1;
    }
#endif
    pStage->Slaves--; // 调试完成可以确保计数为正
    if (pStage->Slaves == 0) {
#ifdef _DEBUG
      fnPrint("记录父级场景=0x%08X到失败集合.\r\n", pStage);
#endif
      dwRet = fnStageSnap(pQueue, pStage);
      if (dwRet <= 0) {
        return dwRet - 1;
      }
#ifdef _DEBUG
      fnPrint("移除父级场景=0x%08X.\r\n", pStage);
#endif
      fnQueueRemove(pQueue, pStage);
      return dwRet + 1; // 返回级数
    }
  }
  return 1;
_labCacheNull:
  fnStageCode(SEC_CACHE_NULL);
  return 0; // 内存不足, 数据不变
}
// 选定最大归位数的走法(与从少到多步数逻辑部分重复, 废弃此函数)
void fnStageBest(PSTAGE pStage, UINT dwMoves)
{
  UINT dwValue; // 最大归位
  PMOVE pMove = pStage->Moves; // 首个走法
  // 找出第一个有效走法
  while (dwMoves) {
    if (pMove->Flags == SMF_MOVE_NONE) {
      dwValue = pMove->Value;
      pStage->Index = ((UINT)pMove - (UINT)pStage->Moves) / sizeof(MOVE); // 保存索引
      dwMoves--; // 待检查数量递减
      break;
    }
    pMove++;
  }
  // 找出更高归位的走法
  while (dwMoves) {
    if (pMove->Flags == SMF_MOVE_NONE) {
      // 这个走法还没处理
      if (pMove->Value > dwValue) {
        dwValue = pMove->Value;
        pStage->Index = ((UINT)pMove - (UINT)pStage->Moves) / sizeof(MOVE); // 保存索引
      }
      dwMoves--; // 待检查数量递减
    }
    pMove++;
  }
}
void fnStagePath(PQUEUE pQueue, PSTAGE pStage, PSPOT pPath)
{
  UINT dwStep = pStage->Layer * 2;
  // 存储走法坐标(缓冲区第一个坐标.X存储其尺寸)
  if (dwStep > (UINT)pPath->X) {
    return; // 解法步数超出缓冲区
  }
  pPath = &pPath[dwStep - 1]; // 不包括初始局面
  pQueue->Step = pStage->Layer;
  // 遍历位置(不包括初始场景)
  while (pStage->Host) {
#ifdef _DEBUG
    //fnPrint("\r\n当前场景: 玩家=(%d, %d), 箱子:", pStage->PosX, pStage->PosY);
    //fnPrintBox(pStage);
#endif
    //if(pStage->Automation.Value != 0)
    *pPath-- = pStage->Position; // 移动位置
    *pPath-- = pStage->Automation; // 自动位置
    pStage = pStage->Host; // 上一级场景
  }
  return;
}
// 生成子场景列表并加入队列
UINT fnStageSlaves(PQUEUE pQueue, PSTAGE pStage, void* lpVoid, int* pdwCode)
{
  PSTAGE pNext;
  UINT dwStage;
  int dwRet;
  UINT dwMoves = pStage->Range;
  dwStage = 0;
  while (dwMoves) {
#if 0 // *** 最大归位走法仅小概率提升一点速度, 内存开支更大 ***
    fnStageBest(pStage, dwMoves); // 选择最大归位数走法
#else
    pStage->Index = pStage->Range - dwMoves;
#endif
    pNext = fnStageNext(pQueue, pStage, &dwRet); // 尝试生成: .Index已设置
    if (dwRet == 0) {
      if (pdwCode) {
        *pdwCode = -1;
      }
      return 0; // 队列耗尽
    }
    pStage->Moves[pStage->Index].Flags |= SMF_MOVE_USED; // 标识已经处理
    if (pNext != NULL) {
      // 下一步场景无重复, 追加到队列
      dwRet = fnQueuePush(pQueue, pNext);
      // 如果已经完全复位, 则返回(此时pNext有链表, 容易计算解法)
      if (pNext->Value == pNext->Count) {
        // ******** 场景归位(即为最优走法) ********
        fnStagePath(pQueue, pNext, (PSPOT)lpVoid); // 生成解法坐标
        if (pdwCode) {
          *pdwCode = 1;
        }
        return 0;
      }
      dwStage++; // 增加了一个子场景
    }
    dwMoves--; // 走法数量递减
  }
  if (pdwCode) {
    *pdwCode = 0;
  }
  return dwStage;
}
// *** 此函数调用前应保证队列场景是就绪场景 ***
// 位置在(3,1), 移动至(4,3), 直接向上即可, 因遵循序号和方向顺序, id=0僵局重复, 故推id=1向左
// 解决: 追加启发代码, 即优先选择归位数量最多的箱子和方向(迫不得已再选择归位数量更少的走法)
// 先尝试和快速撤销得到可行走法及其归位数量列表, 依次按归位数量从大到小快速重做后递归
// 为解决更多归位阻塞未归位箱子问题, 得到解法后, 以此作为步数限制, 反向求所有更优解法
// 解决: 从当前场景生成有效走法数量的场景, 复位则返回, 否则继续生成, 一步步向后分析
// 得到第一个解法就是最优解法, 100步每步16种走法就有16^100个场景, 内存开销巨大
// 队列代替递归, 后进后出保证更低步数最先被处理, 失效和重复场景丢弃, 大幅降低内存消耗
// 解决: 生成的子级场景, 多数是无效的, 无效连乘导致大幅增加计算时间, 改为最高步数递进的逻辑
// 到达后反向再尝试其他方向, 所有方向试完递增最高步数, 从而减少重复检测循环次数
int fnStageCalc(PQUEUE pQueue, void* lpVoid)
{
  PSTAGE pStage;
  int dwRet;
  // 循环代替递归(在本函数外初始化队列)
  pStage = pQueue->Active; // fnQueuePop()会移除元素
  // 第一个场景已经扫描
  while (pStage != NULL) {
    pStage->Range = fnStageMoves(pStage); // 生成可行走法列表
    if (pStage->Range == 0) {
      // 没有可行走法
#ifdef _DEBUG
      fnPrint("没有可行走法, 记录场景=0x%08X到失败集合, 下个场景=0x%08X.\r\n\r\n", pStage, pStage->Next);
#endif
      dwRet = fnStageSnap(pQueue, pStage); // 记录到失败队列
      if (dwRet <= 0) {
        return -1;
      }
      pStage = fnQueueRemove(pQueue, pStage); // 移除场景, 重新加到未用, 指向链表下一个场景
      continue;
    }
    // 从更大归位的走法开始, 依次分析
    pStage->Slaves = fnStageSlaves(pQueue, pStage, lpVoid, &dwRet);
    if (dwRet > 0) {
      fnStageCode(SEC_ERROR_NONE); // 清零错误代码
      return 1; // 得到最优解法
    }
    else if (dwRet < 0) {
      return -2;
    }
    if (pStage->Slaves == 0) {
#ifdef _DEBUG
      fnPrint("没有子级场景, 记录场景=0x%08X到失败集合, 下个场景=0x%08X.\r\n\r\n", pStage, pStage->Next);
#endif
      dwRet = fnStageSnap(pQueue, pStage); // 记录到失败队列
      if (dwRet <= 0) {
        return -3;
      }
      pStage = fnQueueRemove(pQueue, pStage); // 移除场景, 重新加到未用, 指向链表下一个场景
      continue;
    }
    pStage = pStage->Next; // 当前有子场景, 保留元素, 继续分析队列
  }
  return 0; // 队列无解
}
int fnStageSolve(PSTAGE pStage, void* lpVoid, UINT dwCount, UINT dwFlags)
{
  PROP prp; // 属性, 用于计算数据不完整的场景
  PQUEUE pQueue; // 队列指针
  STACK stk; // 扫描堆栈
  SHOT sht; // 失败快照
  union {
    UINT dwSize; // 场景内存大小, 存储结果步数
    int dwStep;
  };
  int dwRet; // 返回值
  if (pStage == NULL) {
    fnStageCode(SEC_PARAM_NULL);
    return 0; // 无效指针
  }
  if (dwCount <= 1) {
    fnStageCode(SEC_PARAM_MANY);
    return -1; // 缓冲区过小
  }
  dwSize = 0; // 自动尺寸
  V32Clear32(&prp, 0, sizeof(prp)); // 清零属性
  if ((pStage->Flags & SGF_READY) != SGF_READY) {
    // 新建场景, 计算场景参数(只有基础数据的场景也要计算坐标)
    prp.SizeX = pStage->SizeX;
    prp.SizeY = pStage->SizeY;
    prp.Flags = SPF_SPOT_ARRAY; // 要求返回箱子坐标
    dwRet = fnStageProp(pStage->Matrix, &prp);
    pStage->Count = prp.Count;
    pStage->Position.Value = prp.Role.Value;
    pStage->Flags |= SGF_BASIC; // 场景参数完成计算, 但具体坐标没有保存
    if (dwRet <= 0) {
      fnStageCode(SEC_STAGE_NULL);
      return -2; // 场景无效
    }
    dwSize = fnStageSize(pStage->Count, pStage->SizeX, pStage->SizeY);
  }
  if (pStage->Value == pStage->Count) {
    fnStageCode(SEC_SOLVE_DONE);
    return -3; // 已经归位
  }
  // 初始化场景队列
  stk.Stars = prp.Spots;
  pQueue = fnQueueInit(pStage, &stk, &sht, dwSize, dwCount, dwFlags);
  if (pQueue == NULL) {
    return -4; // 内存不足(错误代码由子函数赋值)
  }
#ifdef _DEBUG
  dwRet = fnDebugInit();
  prp.Flags = ((PSPOT)lpVoid)->X;
  fnPrint("堆栈大小=%d, 队列尺寸=%d, 解法尺寸=%d, 开始求解...\r\n", stk.Count, dwCount, prp.Flags);
#endif
  // 求解输出(如果成功, 解法坐标保存到lpVoid, 步数保存到pQueue->Step)
  dwRet = fnStageCalc(pQueue, lpVoid);
  dwStep = pQueue->Step;
#ifdef _DEBUG
  prp.Value = pQueue->Value;
  prp.Hash = pQueue->Used;
#endif
  SafeFree(sht.Snaps); // 释放无效局面
  SafeFree(stk.Stars); // 释放扫描堆栈
  SafeFree(pQueue); // 释放队列
  if (dwRet <= 0) {
#ifdef _DEBUG
    fnPrint("求解失败, 返回值=%d, 最后错误代码=%d!\r\n", dwRet, fnStageCode(SEC_ERROR_DUMP));
    fnDebugExit();
#endif
    return -6; // 场景无解(不覆盖错误代码)
  }
#ifdef _DEBUG
  fnPrint("关卡求解成功, 队列使用峰值=%d, 队列剩余个数=%d, 快照峰值=%d, 快照数量=%d!\r\n", prp.Value, prp.Hash, sht.Count, sht.Value);
  stk.Count = dwSize; // 步数即是推动, union
  if (stk.Count > 0) {
    stk.Stars = (PSPOT)lpVoid; // 用作结果指针
    prp.Count = 0; // 用作寻路数量
    while (stk.Count > 0) {
      stk.Count--;
      if (stk.Stars->Value != 0) {
        fnPrint("(%d, %d)\r\n", stk.Stars->X, stk.Stars->Y);
        prp.Count++;
      }
      stk.Stars++;
      fnPrint("<%d, %d>\r\n", stk.Stars->X, stk.Stars->Y);
      stk.Stars++;
    }
    fnPrint("最优解法推动 %d 次, 寻路 %d 次, 合计坐标 %d 个!\r\n", dwSize, prp.Count, dwSize + prp.Count);
  }
  fnDebugExit();
#endif
  return dwStep;
}

