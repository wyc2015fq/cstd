#include "stdafx.h"
#include "../include/RgnLight.h"
#ifdef _USE_LOKI_
#include <SmallObj.h>
struct MySmallObjAllocator : public Loki::SmallObjAllocator
{
    MySmallObjAllocator() 
    : Loki::SmallObjAllocator(DEFAULT_CHUNK_SIZE, MAX_SMALL_OBJECT_SIZE)
    {}
};
#endif
CRgnLight::CRect* CRgnLight::NodeAlloc()
{
#ifdef _USE_LOKI_
	using namespace Loki;
    CRect* p=(CRect*)SingletonHolder<MySmallObjAllocator, CreateStatic, 
        PhoenixSingleton>::Instance().Allocate(sizeof(CRect));
#else
	CRect* p= new CRect;
#endif
	p->m_pNext=NULL;
	return p;
}
void CRgnLight::NodeFree(CRgnLight::CRect* p)
{
#ifdef _USE_LOKI_
	using namespace Loki;
	SingletonHolder<MySmallObjAllocator, CreateStatic, 
		PhoenixSingleton>::Instance().Deallocate(p, sizeof(CRgnLight::CRect));
#else
	delete p;
#endif
}

void CRgnLight::Clear()
{
	while (m_pHead)
	{
		CRect* pNext = m_pHead->m_pNext;
		NodeFree(m_pHead);
		m_pHead = pNext;
	}
}

void CRgnLight::FromGdi(HRGN hRgn)
{
	Clear();
	if (hRgn)
	{
		// crack it into rectangles.
		DWORD dwSize = GetRegionData(hRgn, 0, NULL);
		_ASSERT(dwSize);

		BYTE pRgnDataStatic[0x200]; // usually enough
		RGNDATA* pRgnData = (RGNDATA*) ((dwSize > sizeof(pRgnDataStatic)) ? new BYTE[dwSize] : pRgnDataStatic);

		pRgnData->rdh.dwSize = sizeof(pRgnData->rdh);
		pRgnData->rdh.iType = RDH_RECTANGLES;
		GetRegionData(hRgn, dwSize, pRgnData);

		// Because we trust GDI, let's assume that all rects are well-ordered and not overlapping
		RECT* pRect = (RECT*) pRgnData->Buffer;
		for (DWORD dwIndex = pRgnData->rdh.nCount; dwIndex--; pRect++)
			AddHead(*pRect);

		if (pRgnData != (RGNDATA*) pRgnDataStatic)
			delete[] pRgnData;
	}
}

//void CRgnLight::ToArr(RECT* pRect) const
//{
//	for (CRect* pValue = m_pHead; pValue; pValue = pValue->m_pNext, ++pRect)
//		*pRect = *pValue;
//}

HRGN CRgnLight::ToGdi() const
{
	// count how many rects we have
	DWORD dwCount = GetRectsCoount();

	// assemble a HRGN from its data
	BYTE pRgnDataStatic[0x200]; // usually enough
	DWORD dwSizeNeeded = sizeof(RECT) * dwCount + sizeof(RGNDATAHEADER);
	RGNDATA* pRgnData = (RGNDATA*) ((dwSizeNeeded > sizeof(pRgnDataStatic)) ? new BYTE[dwSizeNeeded] : pRgnDataStatic);

	ZeroMemory(&pRgnData->rdh, sizeof(RGNDATAHEADER));

	RECT* pDst = (RECT*) pRgnData->Buffer;
	for (CRect* pRect = m_pHead; pRect; pRect = pRect->m_pNext)
		*pDst++ = *pRect;

	pRgnData->rdh.dwSize = sizeof(pRgnData->rdh);
	pRgnData->rdh.iType = RDH_RECTANGLES;
	pRgnData->rdh.nCount = dwCount;

	HRGN hRgn = ExtCreateRegion(NULL, dwSizeNeeded, pRgnData);
	_ASSERT(hRgn);

	if (pRgnData != (RGNDATA*) pRgnDataStatic)
		delete pRgnData;
	return hRgn;
}

bool CRgnLight::GetBox(RECT& rcValue) const
{
	if (m_pHead)
	{
		rcValue = *m_pHead;
		for (CRect* pRect = m_pHead->m_pNext; pRect; pRect = pRect->m_pNext)
		{
			if (rcValue.left > pRect->left)
				rcValue.left = pRect->left;
			if (rcValue.right < pRect->right)
				rcValue.right = pRect->right;
			if (rcValue.top > pRect->top)
				rcValue.top = pRect->top;
			if (rcValue.bottom < pRect->bottom)
				rcValue.bottom = pRect->bottom;
		}
		return true;

	} else
	{
		//ZeroObject(rcValue);
		ZeroMemory(&rcValue,sizeof(RECT));
		return false;
	}
}

//ULONG CRgnLight::GetRectsCoount() const
//{
//	ULONG nCount = 0;
//	for (CRect* pRect = m_pHead; pRect; pRect = pRect->m_pNext)
//		nCount++;
//	return nCount;
//}

void CRgnLight::Offset(long nX, long nY)
{
	if (nX || nY)
		for (CRect* pRect = m_pHead; pRect; pRect = pRect->m_pNext)
		{
			pRect->left += nX;
			pRect->right += nX;
			pRect->top += nY;
			pRect->bottom += nY;
		}
}

//void CRgnLight::AddHead(const RECT& rcValue)
//{
//	// the rcValue MUST be well-ordered and MUST NOT intersect with any of current rects.
//	CRect* pRect = NodeAlloc();
//	CopyMemory(pRect, &rcValue, sizeof(RECT));
//	AddHead(pRect);
//}

//void CRgnLight::PerfSubstract(const CRgnLight& rgnValue, CRgnLight* prgnErased)
//{
//	for (CRect* pRect = rgnValue.m_pHead; pRect; pRect = pRect->m_pNext)
//		PerfSubstract(*pRect, prgnErased);
//}

//void CRgnLight::PerfAppend(const CRgnLight& rgnValue)
//{
//	for (CRect* pRect = rgnValue.m_pHead; pRect; pRect = pRect->m_pNext)
//		AddHead(*pRect);
//}

void CRgnLight::PerfSubstract(const RECT& rcValue, CRgnLight* prgnErased)
{
	// the rcValue MUST be well-ordered
	CRect* pPrev = NULL;
	for (CRect* pNode = m_pHead; pNode; )
	{
		CRect* pRect = pNode;
		pNode = pNode->m_pNext;

		if (RectsIntersect(rcValue, *pRect))
		{
			// intersect. This rect should be erased (and added to the rgnErased)
			// But before this - we must 'cut off' all its extra-parts
			if (pRect->top < rcValue.top)
			{
				AddHead(pRect->left, pRect->top, pRect->right, rcValue.top);
				pRect->top = rcValue.top;
			}
			if (pRect->bottom > rcValue.bottom)
			{
				AddHead(pRect->left, rcValue.bottom, pRect->right, pRect->bottom);
				pRect->bottom = rcValue.bottom;
			}
			if (pRect->left < rcValue.left)
			{
				AddHead(pRect->left, pRect->top, rcValue.left, pRect->bottom);
				pRect->left = rcValue.left;
			}
			if (pRect->right > rcValue.right)
			{
				AddHead(rcValue.right, pRect->top, pRect->right, pRect->bottom);
				pRect->right = rcValue.right;
			}

			if (!pPrev)
				// it may exist (although our variable points to NULL), because
				// we could just've added some rects. Locate it.
				for (pPrev = m_pHead; pPrev; pPrev = pPrev->m_pNext)
					if (pPrev->m_pNext == pRect)
						break;

			if (pPrev)
				pPrev->m_pNext = pNode;
			else
				m_pHead = pNode;

			if (prgnErased)
				// because all our rgns share the same allocator object we can
				// just put this node to the rgnErased object
				prgnErased->AddHead(pRect);
			else
				NodeFree(pRect);

		} else
			pPrev = pRect;
	}
}

//void CRgnLight::Intersect(const CRgnLight& rgnOther)
//{
//	CRgnLight rgnIntersect;
//	PerfSubstract(rgnOther, &rgnIntersect);
//	Swap(rgnIntersect);
//}

void CRgnLight::Optimize()
{
	if (!m_pHead)
		return; // nothing to optimize

//#ifdef _DEBUG
//	AssertValid();
//	CRgnLight rgnDup;
//	rgnDup.Copy(*this);
//#endif

	// After several operations on our object can become too complicated.
	// In other words - it can contain different rectangles, whereas there is no actual need
	// in it, some of them could be merged.

	// Beside of this, because of drawing algorythms, we always prefer our rectangles to be
	// wide, not high.

	// 1st step. Sort our rectangles by their 'top' member
	bool bAltered;
	do
	{
		bAltered = false;
		CRect* pPrev1 = m_pHead;
		CRect* pPrev2 = NULL;
		for (CRect* pRect = m_pHead->m_pNext; pRect; )
		{
			bool bSwap = (pRect->top < pPrev1->top);
			AdvanceIteration(pPrev2, pPrev1, pRect, bSwap);
			if (bSwap)
				bAltered = true;
		}
	} while (bAltered);

//#ifdef _DEBUG
//	// verification
//	for (CRect* pDbgRect = m_pHead; pDbgRect->m_pNext; pDbgRect = pDbgRect->m_pNext)
//		_ASSERT(pDbgRect->top <= pDbgRect->m_pNext->top);
//
//	AssertEqualNotOptimized(rgnDup);
//#endif

	// 2nd step. Divide rectangles vertically until there is no overlapping on Y axis.
	long nClosest;
	for (CRect* pRect = m_pHead; pRect; pRect = pRect->m_pNext)
	{
		CRect* pNewPos = NULL;
		if ((m_pHead == pRect) || (nClosest <= pRect->top) || (nClosest >= pRect->bottom))
		{
			// find the closest horizontal line that cuts us.
			nClosest = pRect->bottom;

			for (CRect* pClosest = pRect->m_pNext; pClosest; pClosest = pClosest->m_pNext)
				if (pClosest->top == pRect->top)
					if (nClosest > pClosest->bottom)
						nClosest = pClosest->bottom;
					else;
				else
				{
					if (nClosest > pClosest->top)
					{
						nClosest = pClosest->top;
						pNewPos = pClosest;
					}
					break;
				}
		}

		if (nClosest < pRect->bottom)
		{
			// divide
			CRect* pDiv = NodeAlloc();
			pDiv->left = pRect->left;
			pDiv->right = pRect->right;
			pDiv->top = nClosest;
			pDiv->bottom = pRect->bottom;
			pRect->bottom = nClosest;

			// Decide where to insert this new rect.
			// If it has been cut off by someone's 'top' - insert it right after cutter.
			// Otherwise we'll have to search for the right place.
			if (!pNewPos)
				for (pNewPos = pRect; pNewPos->m_pNext; pNewPos = pNewPos->m_pNext)
					if (pNewPos->m_pNext->top >= nClosest)
						break;

			pDiv->m_pNext = pNewPos->m_pNext;
			pNewPos->m_pNext = pDiv;
		}
	}

//#ifdef _DEBUG
//
//	// verification
//	int nDbgT, nDbgB;
//	for (pDbgRect = m_pHead; pDbgRect; pDbgRect = pDbgRect->m_pNext)
//	{
//		if (m_pHead != pDbgRect)
//			if (pDbgRect->top == nDbgT)
//				_ASSERT(pDbgRect->bottom == nDbgB);
//			else
//				_ASSERT(pDbgRect->top >= nDbgT);
//
//		nDbgT = pDbgRect->top;
//		nDbgB = pDbgRect->bottom;
//	}
//
//	AssertEqualNotOptimized(rgnDup);
//#endif

	// 3rd step. For all rects that are equal vertically - sort them horizontally, and unite
	// if possible
	CRect* pLinePrev = NULL;
	CRect* pLine = m_pHead;
	for (; pLine; )
	{
		CRect* pNextPrev = SeekNextLine(pLine);
		CRect* pNext = pNextPrev->m_pNext;

		bool bAltered;
		do
		{
			bAltered = false;
			CRect* pPrev1 = pLine;
			CRect* pPrev2 = pLinePrev;
			for (CRect* pRect = pLine->m_pNext; pRect != pNext; )
				if ((pRect->right == pPrev1->left) || (pRect->left == pPrev1->right))
				{
					// unite them (enlarge pPrev1, and purge pRect)
					if (pPrev1->left > pRect->left)
						pPrev1->left = pRect->left;
					else
						pPrev1->right = pRect->right;

					if (pNextPrev == pRect)
						pNextPrev = pPrev1;

					pPrev1->m_pNext = pRect->m_pNext;

					NodeFree(pRect);
					pRect = pPrev1->m_pNext;

				} else
				{
					bool bSwap = (pRect->left < pPrev1->left);
					if (bSwap)
					{
						if (pNextPrev == pRect)
							pNextPrev = pPrev1;
						if (pLine == pPrev1)
							pLine = pRect;
						bAltered = true;
					}
					AdvanceIteration(pPrev2, pPrev1, pRect, bSwap);
				}

		} while (bAltered);

		pLinePrev = pNextPrev;
		pLine = pNext;
	}

//#ifdef _DEBUG
//	AssertEqualNotOptimized(rgnDup);
//#endif

	// 4th step. Finally unite whole lines that are equal horizontally and adjucent vertically.
	CRect* pNext = SeekNextLine(m_pHead)->m_pNext;
	for (pLine = m_pHead; pLine; )
	{
		if (!pNext)
			break;
		CRect* pNextNext = SeekNextLine(pNext)->m_pNext;

		bool bMerge = false;
		if (pNext->top == pLine->bottom)
		{
			// check if those lines are totally identical horizontally
			CRect* pPos2 = pNext;
			CRect* pPos1 = pLine;
			for (; (pPos1 != pNext) && (pPos2 != pNextNext); pPos1 = pPos1->m_pNext, pPos2 = pPos2->m_pNext)
				if ((pPos1->left != pPos2->left) || (pPos1->right != pPos2->right))
					break;

			bMerge = (pPos1 == pNext) && (pPos2 == pNextNext);
		}
		if (bMerge)
		{
			// merge those lines
			long nBottom = pNext->bottom;
			CRect* pNextPrev;
			CRect* pPos = pLine;
			for (; pPos != pNext; pPos = pPos->m_pNext)
			{
				pPos->bottom = nBottom;
				pNextPrev = pPos;
			}

			for (pPos = pNext; pPos != pNextNext; )
			{
				CRect* pPos2 = pPos->m_pNext;
				NodeFree(pPos);
				pPos = pPos2;
			}

			pNextPrev->m_pNext = pNextNext;

		} else
			// advance
			pLine = pNext;

		pNext = pNextNext;
	}

//#ifdef _DEBUG
//	AssertEqualNotOptimized(rgnDup);
//#endif
}

void CRgnLight::AdvanceIteration(CRect*& pPrev2, CRect*& pPrev1, CRect*& pCurrent, bool bSwap)
{
	if (bSwap)
	{
		if (pPrev2)
			pPrev2->m_pNext = pCurrent;
		else
			m_pHead = pCurrent;

		pPrev1->m_pNext = pCurrent->m_pNext;
		pCurrent->m_pNext = pPrev1;
		pPrev2 = pCurrent;
		pCurrent = pPrev1->m_pNext;
	} else
	{
		pPrev2 = pPrev1;
		pPrev1 = pCurrent;
		pCurrent = pCurrent->m_pNext;
	}
}

CRgnLight::CRect* CRgnLight::SeekNextLine(CRect* pLine)
{
	CRect* pNextPrev = pLine;
	for (; pNextPrev->m_pNext; pNextPrev = pNextPrev->m_pNext)
		if (pNextPrev->m_pNext->top != pLine->top)
			break;
		return pNextPrev;
}


bool CRgnLight::IsEqual(const CRgnLight& rgnOther) const
{
	// Important: Ensure that both regions are optimized, otherwise this function
	// is obsolete.
	CRect* pPos1 = m_pHead;
	for (CRect* pPos2 = rgnOther.m_pHead; pPos2; pPos2 = pPos2->m_pNext)
	{
		if (!pPos1 ||
			(pPos1->left != pPos2->left) ||
			(pPos1->top != pPos2->top) ||
			(pPos1->right != pPos2->right) ||
			(pPos1->bottom != pPos2->bottom))
			return false;
		pPos1 = pPos1->m_pNext;
	}
	return !pPos1;
}

//bool CRgnLight::RectInRgn(const RECT& rcValue) const
//{
//	for (CRect* pRect = m_pHead; pRect; pRect = pRect->m_pNext)
//		if (RectsIntersect(rcValue, *pRect))
//			return true;
//	return false;
//}

//void CRgnLight::AddRect(const RECT& rcValue)
//{
//	if ((rcValue.right > rcValue.left) && (rcValue.bottom > rcValue.top))
//	{
//		PerfSubstract(rcValue, NULL);
//		AddHead(rcValue);
//	}
//}

