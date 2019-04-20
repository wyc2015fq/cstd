
#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

#include "templ.hpp"

template <class TYPE, class ARG_TYPE>
class CArray {
public:
  TYPE* m_pData;   // the actual array of data
  int m_nSize;     // # of elements (upperBound - 1)
  int m_nMaxSize;  // max allocated
  int m_nGrowBy;   // grow amount
// CArray<TYPE, ARG_TYPE> inline functions
    AFX_INLINE int GetSize() const
  { return m_nSize; }
  
    AFX_INLINE int GetUpperBound() const
  { return m_nSize-1; }
  
    AFX_INLINE void RemoveAll()
  { SetSize(0, -1); }
  
    AFX_INLINE TYPE GetAt(int nIndex) const
  { ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
  
    AFX_INLINE void SetAt(int nIndex, ARG_TYPE newElement)
  { ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
  
    AFX_INLINE TYPE& ElementAt(int nIndex)
  { ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
  
    AFX_INLINE const TYPE* GetData() const
  { return (const TYPE*)m_pData; }
  
    AFX_INLINE TYPE* GetData()
  { return (TYPE*)m_pData; }
  
    AFX_INLINE int Add(ARG_TYPE newElement)
  { int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
    return nIndex; }
  
    AFX_INLINE TYPE operator[](int nIndex) const
  { return GetAt(nIndex); }
  
    AFX_INLINE TYPE& operator[](int nIndex)
  { return ElementAt(nIndex); }


    
/////////////////////////////////////////////////////////////////////////////
// CArray<TYPE, ARG_TYPE> out-of-line functions


CArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}


~CArray()
{
	ASSERT_VALID(this);

	if (m_pData != NULL)
	{
		DestructElements<TYPE>(m_pData, m_nSize);
		delete[] (BYTE*)m_pData;
	}
}


void SetSize(int nNewSize, int nGrowBy = -1)
{
	ASSERT_VALID(this);
	ASSERT(nNewSize >= 0);

	if (nGrowBy != -1)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing
		if (m_pData != NULL)
		{
			DestructElements<TYPE>(m_pData, m_nSize);
			delete[] (BYTE*)m_pData;
			m_pData = NULL;
		}
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		ASSERT(nNewSize <= SIZE_T_MAX/sizeof(TYPE));    // no overflow
#endif
		m_pData = (TYPE*) new BYTE[nNewSize * sizeof(TYPE)];
		ConstructElements<TYPE>(m_pData, nNewSize);
		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements
			ConstructElements<TYPE>(&m_pData[m_nSize], nNewSize-m_nSize);
		}
		else if (m_nSize > nNewSize)
		{
			// destroy the old elements
			DestructElements<TYPE>(&m_pData[nNewSize], m_nSize-nNewSize);
		}
		m_nSize = nNewSize;
	}
	else
	{
		// otherwise, grow array
		int nGrowBy = m_nGrowBy;
		if (nGrowBy == 0)
		{
			// heuristically determine growth when nGrowBy == 0
			//  (this avoids heap fragmentation in many situations)
			nGrowBy = m_nSize / 8;
			nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);
		}
		int nNewMax;
		if (nNewSize < m_nMaxSize + nGrowBy)
			nNewMax = m_nMaxSize + nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

		ASSERT(nNewMax >= m_nMaxSize);  // no wrap around
#ifdef SIZE_T_MAX
		ASSERT(nNewMax <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		TYPE* pNewData = (TYPE*) new BYTE[nNewMax * sizeof(TYPE)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));

		// construct remaining elements
		ASSERT(nNewSize > m_nSize);
		ConstructElements<TYPE>(&pNewData[m_nSize], nNewSize-m_nSize);

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}


int Append(const CArray& src)
{
	ASSERT_VALID(this);
	ASSERT(this != &src);   // cannot append to itself

	int nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize);
	CopyElements<TYPE>(m_pData + nOldSize, src.m_pData, src.m_nSize);
	return nOldSize;
}


void Copy(const CArray& src)
{
	ASSERT_VALID(this);
	ASSERT(this != &src);   // cannot append to itself

	SetSize(src.m_nSize);
	CopyElements<TYPE>(m_pData, src.m_pData, src.m_nSize);
}


void FreeExtra()
{
	ASSERT_VALID(this);

	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		ASSERT(m_nSize <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		TYPE* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (TYPE*) new BYTE[m_nSize * sizeof(TYPE)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));
		}

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}


void SetAtGrow(int nIndex, ARG_TYPE newElement)
{
	ASSERT_VALID(this);
	ASSERT(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1, -1);
	m_pData[nIndex] = newElement;
}


void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1)
{
	ASSERT_VALID(this);
	ASSERT(nIndex >= 0);    // will expand to meet need
	ASSERT(nCount > 0);     // zero or negative size not allowed

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount, -1);   // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		SetSize(m_nSize + nCount, -1);  // grow it to new size
		// destroy intial data before copying over it
		DestructElements<TYPE>(&m_pData[nOldSize], nCount);
		// shift old data up to fill gap
		memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
			(nOldSize-nIndex) * sizeof(TYPE));

		// re-init slots we copied from
		ConstructElements<TYPE>(&m_pData[nIndex], nCount);
	}

	// insert new value in the gap
	ASSERT(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}


void RemoveAt(int nIndex, int nCount = 1)
{
	ASSERT_VALID(this);
	ASSERT(nIndex >= 0);
	ASSERT(nCount >= 0);
	ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);
	DestructElements<TYPE>(&m_pData[nIndex], nCount);
	if (nMoveCount)
		memmove(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(TYPE));
	m_nSize -= nCount;
}


void InsertAt(int nStartIndex, CArray* pNewArray)
{
	ASSERT_VALID(this);
	ASSERT(pNewArray != NULL);
	ASSERT_VALID(pNewArray);
	ASSERT(nStartIndex >= 0);

	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
		for (int i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i));
	}
}


void Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar.WriteCount(m_nSize);
	}
	else
	{
		DWORD nOldSize = ar.ReadCount();
		SetSize(nOldSize, -1);
	}
	SerializeElements<TYPE>(ar, m_pData, m_nSize);
}

void Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << "with " << m_nSize << " elements";
	if (dc.GetDepth() > 0)
	{
		dc << "\n";
		DumpElements<TYPE>(dc, m_pData, m_nSize);
	}

	dc << "\n";
}


void AssertValid() const
{
	CObject::AssertValid();

	if (m_pData == NULL)
	{
		ASSERT(m_nSize == 0);
		ASSERT(m_nMaxSize == 0);
	}
	else
	{
		ASSERT(m_nSize >= 0);
		ASSERT(m_nMaxSize >= 0);
		ASSERT(m_nSize <= m_nMaxSize);
		ASSERT(AfxIsValidAddress(m_pData, m_nMaxSize * sizeof(TYPE)));
	}
}

};


#endif // __ARRAY_HPP__
