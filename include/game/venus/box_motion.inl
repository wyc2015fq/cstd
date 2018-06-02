// ****************************************************************************************************
// 文件: box_motion.cpp
// 注释:
// 推箱子移动算法模块, 判断或执行某个箱子的移动, 快速撤销, 快速重做函数代码实现
// 寻路另一种更优方法是, 申请缓存时开关每个单元, 寻路只要判断该单元是否打开即可
// ****************************************************************************************************
//#include <malloc.h>
//#include <stdio.h>
#define __VENUS_DLL_INC_ // DLL内部编译
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
int fnStagePush(PSTAGE pStage, PMOVE pMove, UINT dwFlags)
{
  SQUARE squ;
  PSPOT ps;
  //if((pStage->Flags & SGF_READY) != SGF_READY)
  //{
  // return 0; // 场景未就绪当作无效方向(外部控制)
  //}
  ps = &pStage->Stars[pMove->Index]; // 不要直接操作这个指针
  if (dwFlags == SMF_MOVE_NONE) { // 应用动作
    pStage->Position = pMove->Porter;
    pStage->Automation = pMove->Movement;
    *ps = pMove->Object;
    // 应用动作后会扫描, 自动清零非类型位(注意次序!!)
#ifdef _DEBUG
    //if(pMove->PortX == 3 && pMove->PortY == 5)
    //{
    // squ.Value = 0; // 调试断点
    //}
#endif
    //pStage->Matrix[pMove->RoleId] = pMove->Role; // 角色先离开(否则目标可能正好是角色位置)
    //pStage->Matrix[pMove->CubeId] = pMove->Cube; // 箱子被推走(角色进入)
    //pStage->Matrix[pMove->DestId] = pMove->Dest; // 箱子进目标
    pStage->Matrix[pMove->RoleId] ^= SMT_PORTER;
    pStage->Matrix[pMove->CubeId] |= SMT_PORTER;
    pStage->Matrix[pMove->CubeId] ^= SMT_OBJECT;
    pStage->Matrix[pMove->DestId] |= SMT_OBJECT;
    pStage->Value = pMove->Value;
    return 2; //break;
  }
  pMove->CubeId = ps->Y * pStage->SizeX + ps->X; // 箱子的单元索引
#ifdef _DEBUG
  //if((pStage->Matrix[pMove->CubeId] & SMT_OBJECT) == 0)
  //{
  // fnPrint("[警告]目标箱子不存在!\r\n");
  // fnPrintStage(pStage, 1);
  // fnOutput();
  //}
#endif
  pMove->Porter = *ps; // 移动后角色的坐标
  squ.Value = 0;
  switch (dwFlags) {
  case SMF_MOVE_RISE: // 上
    pMove->MoveX = ps->X;
    pMove->MoveY = ps->Y + 1;
    if (pStage->Position.Value == pMove->Movement.Value) {
      pMove->Movement.Value = 0; // 角色正好在推动位置上
    }
    pMove->Object.X = ps->X; // 移动后箱子的坐标
    pMove->Object.Y = ps->Y - 1;
    pMove->DestId = pMove->CubeId - pStage->SizeX;
    pMove->Move = pStage->Matrix[pMove->CubeId + pStage->SizeX]; // 推动位置
    squ.Left = pStage->Matrix[pMove->DestId - 1];
    squ.Right = pStage->Matrix[pMove->DestId + 1];
    squ.DownHill = pStage->Matrix[pMove->DestId - pStage->SizeX - 1];
    squ.Front = pStage->Matrix[pMove->DestId - pStage->SizeX];
    squ.UpHill = pStage->Matrix[pMove->DestId - pStage->SizeX + 1];
    break;
  case SMF_MOVE_DROP: // 下
    pMove->MoveX = ps->X;
    pMove->MoveY = ps->Y - 1;
    if (pStage->Position.Value == pMove->Movement.Value) {
      pMove->Movement.Value = 0; // 角色正好在推动位置上
    }
    pMove->Object.X = ps->X; // 移动后箱子的坐标
    pMove->Object.Y = ps->Y + 1;
    pMove->DestId = pMove->CubeId + pStage->SizeX;
    pMove->Move = pStage->Matrix[pMove->CubeId - pStage->SizeX]; // 推动位置
    squ.Left = pStage->Matrix[pMove->DestId + 1];
    squ.Right = pStage->Matrix[pMove->DestId - 1];
    squ.DownHill = pStage->Matrix[pMove->DestId + pStage->SizeX + 1];
    squ.Front = pStage->Matrix[pMove->DestId + pStage->SizeX];
    squ.UpHill = pStage->Matrix[pMove->DestId + pStage->SizeX - 1];
    break;
  case SMF_MOVE_BACK: // 左
    pMove->MoveX = ps->X + 1;
    pMove->MoveY = ps->Y;
    if (pStage->Position.Value == pMove->Movement.Value) {
      pMove->Movement.Value = 0; // 角色正好在推动位置上
    }
    pMove->Object.X = ps->X - 1; // 移动后箱子的坐标
    pMove->Object.Y = ps->Y;
    pMove->DestId = pMove->CubeId - 1;
    pMove->Move = pStage->Matrix[pMove->CubeId + 1]; // 推动位置
    squ.Left = pStage->Matrix[pMove->DestId + pStage->SizeX];
    squ.Right = pStage->Matrix[pMove->DestId - pStage->SizeX];
    squ.DownHill = pStage->Matrix[pMove->DestId + pStage->SizeX - 1];
    squ.Front = pStage->Matrix[pMove->DestId - 1];
    squ.UpHill = pStage->Matrix[pMove->DestId - pStage->SizeX - 1];
    break;
  case SMF_MOVE_FORE: // 右
    pMove->MoveX = ps->X - 1;
    pMove->MoveY = ps->Y;
    if (pStage->Position.Value == pMove->Movement.Value) {
      pMove->Movement.Value = 0; // 角色正好在推动位置上
    }
    pMove->Object.X = ps->X + 1; // 移动后箱子的坐标
    pMove->Object.Y = ps->Y;
    pMove->DestId = pMove->CubeId + 1;
    pMove->Move = pStage->Matrix[pMove->CubeId - 1]; // 推动位置
    squ.Left = pStage->Matrix[pMove->DestId - pStage->SizeX];
    squ.Right = pStage->Matrix[pMove->DestId + pStage->SizeX];
    squ.DownHill = pStage->Matrix[pMove->DestId - pStage->SizeX + 1];
    squ.Front = pStage->Matrix[pMove->DestId + 1];
    squ.UpHill = pStage->Matrix[pMove->DestId + pStage->SizeX + 1];
    break;
  default:
    return 0; //break;
  }
  if (pMove->Move & SMT_MASKED) {
    return -1; // 推动位置是箱子或边界
  }
  if (pStage->Matrix[pMove->DestId] & SMT_MASKED) {
    return -2; // 目标位置是箱子或边界
  }
  if ((pMove->Move & SMT_OPENED) == 0) {
    return -3; // 推动位置不可寻路到达
  }
  squ.Core = pStage->Matrix[pMove->DestId]; // 推动后箱子位置
  if (fnStageTrapped(&squ) != 0) {
    return -4; // 这种走法将会出现僵局(不改变squ数据)
  }
  pMove->RoleId = pStage->PosY * pStage->SizeX + pStage->PosX; // 移动前角色的单元索引
  // 设定结果数据
  pMove->Dest = pStage->Matrix[pMove->DestId]; // squ.Core
  pMove->Dest |= SMT_OBJECT; // 箱子进入
  pMove->Cube = pStage->Matrix[pMove->CubeId];
  pMove->Cube ^= SMT_OBJECT; // 箱子离开
  pMove->Cube |= SMT_PORTER; // 角色进入
  //pMove->Role = pStage->Matrix[pMove->RoleId];
  //pMove->Role ^= SMT_PORTER; // 角色离开
  pMove->Value = pStage->Value; // 移动前的归位数量
  if (pMove->Cube & SMT_TARGET) {
    pMove->Value--;
  }
  if (pMove->Dest & SMT_TARGET) {
    pMove->Value++;
  }
  return 1;
}
// ********** 以下函数游戏专用 ********
// 游戏移动
int fnStageProbe(PSTAGE pStage, UINT dwFlags)
{
  //int dwRet;
  BYTE* pNum;
  if ((pStage->Flags & SGF_BASIC) == 0) {
    fnStageCode(SEC_STAGE_GAME);
    return -1; // 新建场景当作无效方向
  }
  pNum = &GetMatrix(pStage, pStage->PosX, pStage->PosY);
  if (dwFlags & SMF_MOVE_TEST) {
    // 测试移动
    switch (dwFlags & SMF_MOVE_MASK) {
    case SMF_MOVE_RISE: // 上
      pNum -= pStage->SizeX;
      if (*pNum & SMT_BORDER) {
        return 0; // 顶住边界
      }
      if (*pNum & SMT_OBJECT) {
        pNum -= pStage->SizeX;
        if (*pNum & SMT_MASKED) {
          return -1; // 阻塞
        }
        return 2; // 移动了箱子
      }
      break;
    case SMF_MOVE_DROP: // 下
      pNum += pStage->SizeX;
      if (*pNum & SMT_BORDER) {
        return 0; // 顶住边界
      }
      if (*pNum & SMT_OBJECT) {
        pNum += pStage->SizeX;
        if (*pNum & SMT_MASKED) {
          return -1; // 阻塞
        }
        return 2; // 移动了箱子
      }
      break;
    case SMF_MOVE_BACK: // 左
      pNum--;
      if (*pNum & SMT_BORDER) {
        return 0; // 顶住边界
      }
      if (*pNum & SMT_OBJECT) {
        pNum--;
        if (*pNum & SMT_MASKED) {
          return -1; // 阻塞
        }
        return 2; // 移动了箱子
      }
      break;
    case SMF_MOVE_FORE: // 右
      pNum++;
      if (*pNum & SMT_BORDER) {
        return 0; // 顶住边界
      }
      if (*pNum & SMT_OBJECT) {
        pNum++;
        if (*pNum & SMT_MASKED) {
          return -1; // 阻塞
        }
        return 2; // 移动了箱子
      }
      break;
    default:
      return -1;
    }
    return 1; // 空走
  }
  // 正式移动(可以再检测一次, 或者直接递归)
  *pNum ^= SMT_PORTER; // 角色离开
  switch (dwFlags & SMF_MOVE_MASK) {
  case SMF_MOVE_RISE: // 上
    pStage->PosY--;
    pNum -= pStage->SizeX;
    *pNum |= SMT_PORTER; // 角色进入
    if (*pNum & SMT_OBJECT) {
      *pNum ^= SMT_OBJECT; // 箱子离开
      if (*pNum & SMT_TARGET) {
        pStage->Value--;
      }
      pNum -= pStage->SizeX;
      *pNum |= SMT_OBJECT; // 箱子进入
      if (*pNum & SMT_TARGET) {
        pStage->Value++;
      }
      return 2; // 移动了箱子
    }
    break;
  case SMF_MOVE_DROP: // 下
    pStage->PosY++;
    pNum += pStage->SizeX;
    *pNum |= SMT_PORTER; // 角色进入
    if (*pNum & SMT_OBJECT) {
      *pNum ^= SMT_OBJECT; // 箱子离开
      if (*pNum & SMT_TARGET) {
        pStage->Value--;
      }
      pNum += pStage->SizeX;
      *pNum |= SMT_OBJECT; // 箱子进入
      if (*pNum & SMT_TARGET) {
        pStage->Value++;
      }
      return 2; // 移动了箱子
    }
    break;
  case SMF_MOVE_BACK: // 左
    pStage->PosX--;
    pNum--;
    *pNum |= SMT_PORTER; // 角色进入
    if (*pNum & SMT_OBJECT) {
      *pNum ^= SMT_OBJECT; // 箱子离开
      if (*pNum & SMT_TARGET) {
        pStage->Value--;
      }
      pNum--;
      *pNum |= SMT_OBJECT; // 箱子进入
      if (*pNum & SMT_TARGET) {
        pStage->Value++;
      }
      return 2; // 移动了箱子
    }
    break;
  case SMF_MOVE_FORE: // 右
    pStage->PosX++;
    pNum++;
    *pNum |= SMT_PORTER; // 角色进入
    if (*pNum & SMT_OBJECT) {
      *pNum ^= SMT_OBJECT; // 箱子离开
      if (*pNum & SMT_TARGET) {
        pStage->Value--;
      }
      pNum++;
      *pNum |= SMT_OBJECT; // 箱子进入
      if (*pNum & SMT_TARGET) {
        pStage->Value++;
      }
      return 2; // 移动了箱子
    }
    break;
  default:
    return -1;
  }
  return 1; // 空走
}

