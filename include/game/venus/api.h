#ifndef __VENUS_H_
#define __VENUS_H_

//#include "v32_api.h"
#include "vrc32_api.h"    // 指纹库

#ifdef __VENUS_DLL_INC_
#define __V32_DLL_API_      // 内部编译标识
#endif
#include "v32_prefix.h"   // 预编译宏, 切换内外部编译

// auto include: box.h require alpha.h
#include "alpha.h"
#include "box.h"

// array item address
#define ArrayItem2(a, w, x, y)    a[y * w + x]

namespace Valiant
{
namespace Gaming
{
class VFC CMaze  //public tagMaze,
{
public:
  CMaze();
  virtual ~CMaze();

  int Create(long dwSizeX, long dwSizeY, long dwMaxCost, UINT dwFlags);
  int Edit(long dwPosX, long dwPosY, void* lpValue);
  int Parse(PSPOT lpStart, PSPOT lpTarget, long* pdwStep);
  int Query(void* lpVoid, UINT dwPropId, long dwIndex);
protected:
  PMAZE m_pMaze;
};

class VFC CPorter  //public tagStage,
{
public:
  CPorter();
  CPorter(PSTAGE pStage);
  CPorter(CPorter* pPorter);
  virtual ~CPorter();

  int Open(void* lpVoid, UINT dwFlags); // 打开文件
  int Apply(void* lpVoid, long dwSizeX, long dwSizeY);  // 创建新建
  int Query(void* lpVoid, long dwPropId, UINT dwFlags); // 查询属性
  int Edit(void* lpVoid, long dwPosX, long dwPosY); // 编辑数据
  int Solve(void* lpVoid, UINT dwCount, UINT dwFlags);  // 求解场景
  int Probe(void* lpVoid, UINT dwFlags);  // 检测移动
  int Save(void* lpVoid, UINT dwFlags); // 写入文件
protected:
  PSTAGE m_pStage;
};
}
}

// C++ not support currently
#ifdef __cplusplus
#endif

#include "v32_subfix.h"   // 预编译宏, 切换内外部编译

#endif