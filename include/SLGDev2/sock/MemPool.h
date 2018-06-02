// MemPool.h: interface for the CMemPool class.
//
#if !defined(AFX_MEMPOOL_H__D1684C85_1FD8_4FF2_8B8E_12A184695C3E__INCLUDED_)
#define AFX_MEMPOOL_H__D1684C85_1FD8_4FF2_8B8E_12A184695C3E__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "common.h"
typedef struct structMEMBLOCK {
  int8*   m_pbBlock;
  int32   m_nSize;
  int32   m_nUsed;
  int32   m_bFirst;
  structMEMBLOCK* m_pNext;
} MEMBLOCK, *PMEMBLOCK;
class CMemPool
{
public:
  static CMemPool& GetInstance();
public:
  virtual ~CMemPool();
public:
  void  CreateMemPool(uint32 nPoolSize, uint32 nUnitSize);
  int8* GetMem(uint32 nSize, BOOL bCreate =  true);
  void  FreeMem(int8* pMem);
  void  FreeAllMem();
private:
  PMEMBLOCK FindSuitbleBlock(uint32 nSize);
  PMEMBLOCK SkipBlock(PMEMBLOCK pCur);
  void  AssignMemToBlock(PMEMBLOCK pFirst, uint32 nChunks, int8* pRawMem);
  int32 CalcueMemBlock(uint32 size);
private:
  CMemPool();
private:
  PMEMBLOCK m_pHead;
  PMEMBLOCK m_pCur;
  PMEMBLOCK m_pLast;
  int8*     m_pbMemPool;
  uint32    m_nUnitsize;
  uint32    m_nPoolSize;
private:
  CriticalSection m_syn;
};
#endif // !defined(AFX_MEMPOOL_H__D1684C85_1FD8_4FF2_8B8E_12A184695C3E__INCLUDED_)

