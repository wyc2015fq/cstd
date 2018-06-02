// ****************************************************************************************************
// 文件: box_trapped.cpp
// 注释:
// 推箱子僵局算法模块, 生成僵局九宫格级掩码, 僵局检测函数实现
// 图示:
// ○=检测 ■=边界 □=箱子 ◇=任意 ◆=箱子或边界
// ****************************************************************************************************
//#include <malloc.h>
//#include <stdio.h>
#define __VENUS_DLL_INC_ // DLL内部编译
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
int fnStageTrapped(PSQUARE ps)
{
  if (ps->Core & SMT_TARGET) {
    // *** 目标已归位 ***
    if (ps->Front & SMT_OBJECT) {
      // 正前有个箱子
      if ((ps->Front & SMT_TARGET) == 0) {
        // 箱子未归位
        if (ps->Left & SMT_MASKED) {
          if (ps->DownHill & SMT_MASKED) {
            return 1;
          }
        }
        if (ps->Right & SMT_MASKED) {
          if (ps->UpHill & SMT_MASKED) {
            return 2;
          }
        }
      }
    }
    if (ps->Left & SMT_OBJECT) {
      // 左边有个箱子
      if ((ps->Left & SMT_TARGET) == 0) {
        // 箱子未归位
        if (ps->Front & SMT_MASKED) {
          if (ps->DownHill & SMT_MASKED) {
            return 3;
          }
        }
      }
    }
    if (ps->Right & SMT_OBJECT) {
      // 右边有个箱子
      if ((ps->Right & SMT_TARGET) == 0) {
        // 箱子未归位
        if (ps->Front & SMT_MASKED) {
          if (ps->UpHill & SMT_MASKED) {
            return 4;
          }
        }
      }
    }
    if (ps->DownHill & SMT_OBJECT) {
      // 左前有个箱子
      if ((ps->DownHill & SMT_TARGET) == 0) {
        // 箱子未归位
        if (ps->Front & SMT_MASKED) {
          if (ps->Left & SMT_MASKED) {
            return 5;
          }
        }
      }
    }
    if (ps->UpHill & SMT_OBJECT) {
      // 右前有个箱子
      if ((ps->UpHill & SMT_TARGET) == 0) {
        // 箱子未归位
        if (ps->Front & SMT_MASKED) {
          if (ps->Right & SMT_MASKED) {
            return 6;
          }
        }
      }
    }
    // 没有未归位且卡住的箱子
  }
  else {
    // *** 目标未归位 ***
    if (ps->Front & SMT_MASKED) {
      // 正前边界或箱子
      if (ps->Front & SMT_BORDER) {
        // 正前边界
        if (ps->Left & SMT_BORDER) {
          return 10; // 左前夹住
        }
        if (ps->Left & SMT_OBJECT) {
          if (ps->DownHill & SMT_MASKED) {
            return 11;
          }
        }
        if (ps->Right & SMT_BORDER) {
          return 20; // 右前夹住
        }
        if (ps->Right & SMT_OBJECT) {
          if (ps->UpHill & SMT_MASKED) {
            return 21;
          }
        }
      }
      else {
        // 正前箱子
        if (ps->Left & SMT_MASKED) {
          if (ps->DownHill & SMT_MASKED) {
            return 31;
          }
        }
        if (ps->Right & SMT_MASKED) {
          if (ps->UpHill & SMT_MASKED) {
            return 32;
          }
        }
      }
    }
    // 可以继续前推
  }
  return 0;
}

