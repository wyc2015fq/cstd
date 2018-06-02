// ****************************************************************************************************
// 文件: box_file.cpp
// 注释:
// 推箱子文件输入输出函数模块, 以及新建接口
// ****************************************************************************************************
#include <malloc.h> // malloc, realloc, free
#include <stdio.h> // FILE
#define __VENUS_DLL_INC_ // DLL内部编译
#include "api.h"
#include "debug.h"
//using namespace Valiant::Cipher;
using namespace Valiant;
// 打开场景文件(无本地逻辑, 不设置错误代码)
PSTAGE fnStageOpen(void* lpVoid, UINT dwFlags)
{
  PSTAGE pStage;
  PBYTE pData;
  PROP prp;
  int dwRet;
  // 从文件生成(尺寸从文件读取)
  pData = fnStageFile(lpVoid, &prp.Size);
  if (pData == NULL) {
    return NULL; // 文件损坏
  }
  // 计算场景参数
  prp.Flags = SPF_SPOT_ARRAY; // 要求返回箱子坐标
  dwRet = fnStageProp(pData, &prp);
  if (dwRet <= 0) { // <= 1
    SafeFree(pData);
    SafeFree(prp.Spots);
    return NULL; // 数据异常
  }
  // 申请场景内存(新建空白场景清零矩阵)
  pStage = fnStageAlloc(pData, &prp); // 所有成员都被赋值
  SafeFree(pData);
  SafeFree(prp.Spots);
  if (pStage == NULL) {
    return NULL; // 内存不足
  }
  return pStage; // 返回指针
}
// 创建新的场景(无本地逻辑, 不设置错误代码)
PSTAGE fnStageApply(void* lpVoid, PSPOT pSpot, UINT dwFlags)
{
  PSTAGE pStage;
  PBYTE pData;
  PROP prp;
  int dwRet;
  // 从内存创建(尺寸参数有效)
  prp.Flags = 0; //V32Clear32(&prp, 0, sizeof(PROP));
  prp.Size.Value = pSpot->Value;
  pData = (PBYTE)lpVoid;
  if (pData != NULL) {
    prp.Flags = SPF_SPOT_ARRAY; // 要求返回箱子坐标
  }
  // 计算场景参数
  dwRet = fnStageProp(pData, &prp);
  if (dwRet <= 0) {
    //SafeFree(pData);
    SafeFree(prp.Spots);
    return NULL; // 数据异常
  }
  // 申请场景内存(新建空白场景清零矩阵)
  pStage = fnStageAlloc(pData, &prp); // 所有成员都被赋值
  //SafeFree(pData);
  SafeFree(prp.Spots);
  if (pStage == NULL) {
    return NULL; // 内存不足
  }
  return pStage; // 返回指针
}
// 存储场景到文件
int fnStageSave(PSTAGE pStage, void* lpVoid, UINT dwFlags)
{
  UINT dwNum;
  FILE* pfx;
  if (pStage == NULL) {
    fnStageCode(SEC_PARAM_NULL);
    return 0; // 无效参数
  }
  if ((pStage->Flags & SGF_BASIC) == 0) {
    fnStageCode(SEC_STAGE_SAVE);
    return -1; // 新建场景不能保存
  }
  // 保存场景
  pfx = fopen((const char*)lpVoid, "wb");
  if (pfx == NULL) {
    fnStageCode(SEC_FILE_OPEN);
    return -2; // 文件打开失败
  }
  dwNum = SFH_HEADER;
  fwrite(&dwNum, sizeof(UINT), 1, pfx);
  dwNum = 0;
  fwrite(&dwNum, sizeof(UINT), 1, pfx);
  fwrite(&pStage->SizeX, sizeof(UINT), 1, pfx);
  fwrite(&pStage->SizeY, sizeof(UINT), 1, pfx);
  dwNum = pStage->SizeX * pStage->SizeY;
  fwrite(pStage->Matrix, sizeof(BYTE), dwNum, pfx);
  fclose(pfx);
  return 1;
}

