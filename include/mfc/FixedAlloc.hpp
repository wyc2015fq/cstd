// fixalloc.cpp - implementation of fixed block allocator

#ifndef __CFIXEDALLOC_H__
#define __CFIXEDALLOC_H__

#include "Plex.hpp"

/////////////////////////////////////////////////////////////////////////////
// CFixedAlloc

struct CFixedAlloc
{
	struct CNode {
		CNode* pNext;   // only valid when in free list
	};
	UINT m_nAllocSize;  // size of each block from Alloc
	UINT m_nBlockSize;  // number of blocks to get at a time
	CPlex* m_pBlocks;   // linked list of blocks (is nBlocks*nAllocSize)
	CNode* m_pNodeFree; // first free node (NULL if no free nodes)
	CRITICAL_SECTION m_protect;
	
	CFixedAlloc::CFixedAlloc(UINT nAllocSize, UINT nBlockSize)
	{
		ASSERT(nAllocSize >= sizeof(CNode));
		ASSERT(nBlockSize > 1);
		
		m_nAllocSize = nAllocSize;
		m_nBlockSize = nBlockSize;
		m_pNodeFree = NULL;
		m_pBlocks = NULL;
		InitializeCriticalSection(&m_protect);
	}
	
	CFixedAlloc::~CFixedAlloc()
	{
		FreeAll();
		DeleteCriticalSection(&m_protect);
	}
	
	void CFixedAlloc::FreeAll()
	{
		EnterCriticalSection(&m_protect);
		m_pBlocks->FreeDataChain();
		m_pBlocks = NULL;
		m_pNodeFree = NULL;
		LeaveCriticalSection(&m_protect);
	}
	
	void* CFixedAlloc::Alloc()
	{
		EnterCriticalSection(&m_protect);
		if (m_pNodeFree == NULL)
		{
			CPlex* pNewBlock = NULL;
			// add another block
			pNewBlock = CPlex::Create(m_pBlocks, m_nBlockSize, m_nAllocSize);
			
			if (pNewBlock) {
				// chain them into free list
				CNode* pNode = (CNode*)pNewBlock->data();
				// free in reverse order to make it easier to debug
				(BYTE*&)pNode += (m_nAllocSize * m_nBlockSize) - m_nAllocSize;
				for (int i = m_nBlockSize-1; i >= 0; i--, (BYTE*&)pNode -= m_nAllocSize)
				{
					pNode->pNext = m_pNodeFree;
					m_pNodeFree = pNode;
				}
			}
		}
		ASSERT(m_pNodeFree != NULL);  // we must have something
		
		// remove the first available node from the free list
		void* pNode = m_pNodeFree;
		m_pNodeFree = m_pNodeFree->pNext;
		
		LeaveCriticalSection(&m_protect);
		return pNode;
	}
	
	void CFixedAlloc::Free(void* p)
	{
		if (p != NULL)
		{
			EnterCriticalSection(&m_protect);
			
			// simply return the node to the free list
			CNode* pNode = (CNode*)p;
			pNode->pNext = m_pNodeFree;
			m_pNodeFree = pNode;
			LeaveCriticalSection(&m_protect);
		}
	}
};

#if 0
#ifndef USE_CFIXEDALLOC

#pragma warning(disable: 4074)
#pragma init_seg(compiler)

#define ROUND(x,y) (((x)+(y-1))&~(y-1))
#define ROUND4(x) ROUND(x, 4)
AFX_STATIC CFixedAlloc _afxAlloc64(ROUND4(65*sizeof(TCHAR)+sizeof(CStringData)));
AFX_STATIC CFixedAlloc _afxAlloc128(ROUND4(129*sizeof(TCHAR)+sizeof(CStringData)));
AFX_STATIC CFixedAlloc _afxAlloc256(ROUND4(257*sizeof(TCHAR)+sizeof(CStringData)));
AFX_STATIC CFixedAlloc _afxAlloc512(ROUND4(513*sizeof(TCHAR)+sizeof(CStringData)));

#endif //!_DEBUG

void* fix_realloc(int nLen, int* pAllocLength) {
  void* pData;
  if (nLen <= 64)
  {
    pData = _afxAlloc64.Alloc();
    *pAllocLength = 64;
  }
  else if (nLen <= 128)
  {
    pData = _afxAlloc128.Alloc();
    *pAllocLength = 128;
  }
  else if (nLen <= 256)
  {
    pData = _afxAlloc256.Alloc();
    *pAllocLength = 256;
  }
  else if (nLen <= 512)
  {
    pData = _afxAlloc512.Alloc();
    *pAllocLength = 512;
  } else {

  }
  return pData;
}

void freeaaa() {
  int nLen = pData->nAllocLength;
  if (nLen == 64)
    _afxAlloc64.Free(pData);
  else if (nLen == 128)
    _afxAlloc128.Free(pData);
  else if (nLen == 256)
    _afxAlloc256.Free(pData);
  else  if (nLen == 512)
    _afxAlloc512.Free(pData);
  else
  {
    ASSERT(nLen > 512);
    delete[] (BYTE*)pData;
    }
}
#endif

#endif // __CFIXEDALLOC_H__
