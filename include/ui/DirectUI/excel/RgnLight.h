#pragma once

//using namespace Loki;
typedef struct RgnLight {
  RECT rc[100];
  int len;
} RgnLight;
typedef RgnLight CRgnLight;
#define RgnLight_GetRectsCount(me) (me->len)
#define RgnLight_Clear(me) (me->len=0)
#define RgnLight_AddTail(me, rcValue)  (me->rc[me->len++] = rcValue)

void RgnLight_FromGdi(RgnLight* me, HRGN hRgn)
{
  if (hRgn)
  {
    // crack it into rectangles.
    BYTE pRgnDataStatic[0x200]; // usually enough
    DWORD dwIndex, dwSize = GetRegionData(hRgn, 0, NULL);
    RGNDATA* pRgnData;
    RECT* pRect;
    _ASSERT(dwSize);

    pRgnData = (RGNDATA*) ((dwSize > sizeof(pRgnDataStatic)) ? malloc(dwSize) : pRgnDataStatic);

    pRgnData->rdh.dwSize = sizeof(pRgnData->rdh);
    pRgnData->rdh.iType = RDH_RECTANGLES;
    GetRegionData(hRgn, dwSize, pRgnData);

    // Because we trust GDI, let's assume that all rects are well-ordered and not overlapping
    pRect = (RECT*) pRgnData->Buffer;
    for (dwIndex = pRgnData->rdh.nCount; dwIndex--; pRect++) {
      RgnLight_AddTail(me, *pRect);
    }

    if (pRgnData != (RGNDATA*) pRgnDataStatic)
      free(pRgnData);
  }
}

HRGN RgnLight_ToGdi(RgnLight* me)
{
  // count how many rects we have
  DWORD dwCount = RgnLight_GetRectsCount(me);

  // assemble a HRGN from its data
  HRGN hRgn;
  RECT* pDst;
  BYTE pRgnDataStatic[0x200]; // usually enough
  DWORD dwSizeNeeded = sizeof(RECT) * dwCount + sizeof(RGNDATAHEADER);
  RGNDATA* pRgnData = (RGNDATA*) ((dwSizeNeeded > sizeof(pRgnDataStatic)) ? malloc(dwSizeNeeded) : pRgnDataStatic);

  ZeroMemory(&pRgnData->rdh, sizeof(RGNDATAHEADER));

  pDst = (RECT*) pRgnData->Buffer;
  memcpy(pDst, me->rc, sizeof(RECT) * dwCount);

  pRgnData->rdh.dwSize = sizeof(pRgnData->rdh);
  pRgnData->rdh.iType = RDH_RECTANGLES;
  pRgnData->rdh.nCount = dwCount;

  hRgn = ExtCreateRegion(NULL, dwSizeNeeded, pRgnData);
  _ASSERT(hRgn);

  if (pRgnData != (RGNDATA*) pRgnDataStatic)
    free(pRgnData);
  return hRgn;
}

bool RgnLight_GetBox(RgnLight* me, RECT* prcValue)
{
  if (me->len)
  {
    int i;
    RECT rcValue = me->rc[0];
    for (i=1; i<me->len; ++i)
    {
      RECT* pRect = me->rc+i;
      if (rcValue.left > pRect->left)
        rcValue.left = pRect->left;
      if (rcValue.right < pRect->right)
        rcValue.right = pRect->right;
      if (rcValue.top > pRect->top)
        rcValue.top = pRect->top;
      if (rcValue.bottom < pRect->bottom)
        rcValue.bottom = pRect->bottom;
    }
    *prcValue = rcValue;
    return true;
  } else {
    ZeroMemory(prcValue, sizeof(RECT));
    return false;
  }
}

void RgnLight_Offset(RgnLight* me, long nX, long nY)
{
  if (nX || nY) {
    int i;
    for (i=0; i<me->len; ++i)
    {
      RECT* pRect = me->rc+i;
      pRect->left += nX;
      pRect->right += nX;
      pRect->top += nY;
      pRect->bottom += nY;
    }
  }
}

// Static function that tests two region for intersection.
static bool RectsIntersect(const RECT rcValue1, const RECT rcValue2)
{
    return 
      ((rcValue1.left < rcValue2.left) ? (rcValue1.right > rcValue2.left) : (rcValue1.left < rcValue2.right)) &&
      ((rcValue1.top < rcValue2.top) ? (rcValue1.bottom > rcValue2.top) : (rcValue1.top < rcValue2.bottom));
}
static RECT iRECT( int l, int t, int r, int b ) {
  RECT rt;
  rt.left = l, rt.top = t, rt.right = r, rt.bottom = b;
  return rt;
}
#define AddHead(l, t, r, b)  RgnLight_AddTail(me, iRECT(l, t, r, b))

void RgnLight_PerfSubstract(RgnLight* me, RECT rcValue, RgnLight* prgnErased)
{
  // the rcValue MUST be well-ordered
  int nPrev = -1;
  int i;
  for (i=0; i<me->len; ++i) {
    RECT* pRect = me->rc+i;

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

      if (prgnErased) {
        // because all our rgns share the same allocator object we can
        // just put this node to the rgnErased object
        RgnLight_AddTail(prgnErased, *pRect);
      }
    } else {
      nPrev = i;
    }
  }
}

// Test if the region intersects with a givern rectangle.
bool RectInRgn(RgnLight* me, RECT rcValue)
{
  if(rcValue.right>rcValue.left && rcValue.bottom>rcValue.top) {
    int i;
    for (i=0; i<me->len; ++i) {
      RECT* pRect = me->rc+i;
      if (RectsIntersect(rcValue, *pRect))
        return true;
    }
  }
  return false;
}

#if 0
void RgnLight_AdvanceIteration(RgnLight* me, RECT*& pPrev2, RECT*& pPrev1, RECT*& pCurrent, bool bSwap)
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

void RgnLight_Optimize(RgnLight* me)
{
  bool bAltered;
  if (me->len<=0)
    return; // nothing to optimize

  // 1st step. Sort our rectangles by their 'top' member
  do
  {
    RECT* pPrev1 = m_pHead;
    RECT* pPrev2 = NULL;
    bAltered = false;
    for (RECT* pRect = m_pHead->m_pNext; pRect; )
    {
      bool bSwap = (pRect->top < pPrev1->top);
      AdvanceIteration(pPrev2, pPrev1, pRect, bSwap);
      if (bSwap)
        bAltered = true;
    }
  } while (bAltered);

  // 2nd step. Divide rectangles vertically until there is no overlapping on Y axis.
  long nClosest;
  for (RECT* pRect = m_pHead; pRect; pRect = pRect->m_pNext)
  {
    RECT* pNewPos = NULL;
    if ((m_pHead == pRect) || (nClosest <= pRect->top) || (nClosest >= pRect->bottom))
    {
      // find the closest horizontal line that cuts us.
      nClosest = pRect->bottom;

      for (RECT* pClosest = pRect->m_pNext; pClosest; pClosest = pClosest->m_pNext)
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
      RECT* pDiv = NodeAlloc();
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


  // 3rd step. For all rects that are equal vertically - sort them horizontally, and unite
  // if possible
  RECT* pLinePrev = NULL;
  RECT* pLine = m_pHead;
  for (; pLine; )
  {
    RECT* pNextPrev = SeekNextLine(pLine);
    RECT* pNext = pNextPrev->m_pNext;

    bool bAltered;
    do
    {
      bAltered = false;
      RECT* pPrev1 = pLine;
      RECT* pPrev2 = pLinePrev;
      for (RECT* pRect = pLine->m_pNext; pRect != pNext; )
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
//  AssertEqualNotOptimized(rgnDup);
//#endif

  // 4th step. Finally unite whole lines that are equal horizontally and adjucent vertically.
  RECT* pNext = SeekNextLine(m_pHead)->m_pNext;
  for (pLine = m_pHead; pLine; )
  {
    if (!pNext)
      break;
    RECT* pNextNext = SeekNextLine(pNext)->m_pNext;

    bool bMerge = false;
    if (pNext->top == pLine->bottom)
    {
      // check if those lines are totally identical horizontally
      RECT* pPos2 = pNext;
      RECT* pPos1 = pLine;
      for (; (pPos1 != pNext) && (pPos2 != pNextNext); pPos1 = pPos1->m_pNext, pPos2 = pPos2->m_pNext)
        if ((pPos1->left != pPos2->left) || (pPos1->right != pPos2->right))
          break;

      bMerge = (pPos1 == pNext) && (pPos2 == pNextNext);
    }
    if (bMerge)
    {
      // merge those lines
      long nBottom = pNext->bottom;
      RECT* pNextPrev;
      RECT* pPos = pLine;
      for (; pPos != pNext; pPos = pPos->m_pNext)
      {
        pPos->bottom = nBottom;
        pNextPrev = pPos;
      }

      for (pPos = pNext; pPos != pNextNext; )
      {
        RECT* pPos2 = pPos->m_pNext;
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
//  AssertEqualNotOptimized(rgnDup);
//#endif
}

RgnLight_RECT* RgnLight_SeekNextLine(RgnLight* me, RECT* pLine)
{
  RECT* pNextPrev = pLine;
  for (; pNextPrev->m_pNext; pNextPrev = pNextPrev->m_pNext)
    if (pNextPrev->m_pNext->top != pLine->top)
      break;
    return pNextPrev;
}


bool RgnLight_IsEqual(RgnLight* me, const RgnLight& rgnOther)
{
  // Important: Ensure that both regions are optimized, otherwise this function
  // is obsolete.
  RECT* pPos1 = m_pHead;
  for (RECT* pPos2 = rgnOther.m_pHead; pPos2; pPos2 = pPos2->m_pNext)
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

  // Exports the region into an array of rectangles. Prior to calling this function
  // you must query the count of rectanges by RgnLight_GetRectsCount(me)
  void ToArr(RgnLight* me, RECT* pRect)
  {
    for (RECT* pValue = m_pHead; pValue; pValue = pValue->m_pNext, ++pRect)
      *pRect = *pValue;
  }

  // Subtracts a rectangle from the region.
  void Substract(RgnLight* me, const RECT& rcValue)
  {
    if ((rcValue.left < rcValue.right) && (rcValue.top < rcValue.bottom))
      PerfSubstract(rcValue, NULL);
  }

  // RGN_DIFF
  void Substract(RgnLight* me, const RgnLight& rgnOther) { PerfSubstract(rgnOther, NULL); }

  // RGN_COPY
  void Copy(const RgnLight& rgnOther) { Clear(); PerfAppend(rgnOther); }

  // RGN_AND
  void Intersect(RgnLight* me, const RgnLight& rgnOther)
  {
    RgnLight rgnIntersect;
    PerfSubstract(rgnOther, &rgnIntersect);
    Swap(rgnIntersect);
  }

  // RGN_OR
  void Combine(const RgnLight& rgnOther) { Substract(rgnOther); PerfAppend(rgnOther); }

  // The following function subtracts one region from another,
  // and the intersection (that has been cut out) is appended to rgnIntersect parameter.
  // Pay attention: the rgnIntersect is NOT cleared, so that you must be sure
  // that it doesn't intersect with what it will be appended with.
  void SubstractEx(const RgnLight& rgnValue, RgnLight& rgnIntersect)
  {
    PerfSubstract(rgnValue, &rgnIntersect);
#ifdef _DEBUG
    rgnIntersect.AssertValid();
#endif
  }

  // Unite the region with a given rectangle.
  void AddRect(const RECT& rcValue)
  {
    if ((rcValue.right > rcValue.left) && (rcValue.bottom > rcValue.top))
    {
      PerfSubstract(rcValue, NULL);
      AddHead(rcValue);
    }
  }
  void AddRect(int nL, int nT, int nR, int nB)
  {
    RECT rcValue = { nL, nT, nR, nB };
    AddRect(rcValue);
  }

  // Iteration
  const RECT* GetFirst() { return m_pHead; }
  static const RECT* GetNext(const RECT* pRect) { return ((RECT*) pRect)->m_pNext; }

  // Debug
#ifdef _DEBUG
  void AssertValid()
  {
    for (RECT* pRect = m_pHead; pRect; pRect = pRect->m_pNext)
      for (RECT* pNext = pRect->m_pNext; pNext; pNext = pNext->m_pNext)
        _ASSERT(!RectsIntersect(*pRect, *pNext));
  }
  void AssertEqualNotOptimized(const RgnLight& rgnOther)
  {
    RgnLight rgnDup;
    rgnDup.Copy(rgnOther);
    rgnDup.PerfSubstract(*this, NULL);
    _ASSERT(rgnDup.IsEmpty());

    rgnDup.Copy(*this);
    rgnDup.Substract(rgnOther);
    _ASSERT(rgnDup.IsEmpty());
  }
#endif

  void PerfSubstract(const RgnLight& rgnValue, RgnLight* prgnErased)
  {
    for (RECT* pRect = rgnValue.m_pHead; pRect; pRect = pRect->m_pNext)
      PerfSubstract(*pRect, prgnErased);
  }
  void PerfAppend(const RgnLight& rgnValue)
  {
    for (RECT* pRect = rgnValue.m_pHead; pRect; pRect = pRect->m_pNext)
      AddHead(*pRect);
  }


  
#endif
