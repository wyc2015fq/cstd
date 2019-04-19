
#ifndef __TEMPL_HPP__
#define __TEMPL_HPP__

#define AFX_INLINE

template<class TYPE>
AFX_INLINE void ConstructElements(TYPE* pElements, int nCount)
{
	ASSERT(nCount == 0 ||
		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));

	// first do bit-wise zero initialization
	memset((void*)pElements, 0, nCount * sizeof(TYPE));

	// then call the constructor(s)
	for (; nCount--; pElements++)
		::new((void*)pElements) TYPE;
}

template<class TYPE>
AFX_INLINE void DestructElements(TYPE* pElements, int nCount)
{
	ASSERT(nCount == 0 ||
		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));

	// call the destructor(s)
	for (; nCount--; pElements++)
		pElements->~TYPE();
}

template<class TYPE>
AFX_INLINE void CopyElements(TYPE* pDest, const TYPE* pSrc, int nCount)
{
	ASSERT(nCount == 0 ||
		AfxIsValidAddress(pDest, nCount * sizeof(TYPE)));
	ASSERT(nCount == 0 ||
		AfxIsValidAddress(pSrc, nCount * sizeof(TYPE)));

	// default is element-copy using assignment
	while (nCount--)
		*pDest++ = *pSrc++;
}

template<class TYPE>
void SerializeElements(CArchive& ar, TYPE* pElements, int nCount)
{
	ASSERT(nCount == 0 ||
		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));

	// default is bit-wise read/write
	if (ar.IsStoring())
		ar.Write((void*)pElements, nCount * sizeof(TYPE));
	else
		ar.Read((void*)pElements, nCount * sizeof(TYPE));
}

#ifdef _DEBUG
template<class TYPE>
void DumpElements(CDumpContext& dc, const TYPE* pElements, int nCount)
{
	ASSERT(nCount == 0 ||
		AfxIsValidAddress(pElements, nCount * sizeof(TYPE), FALSE));
	&dc; // not used
	pElements;  // not used
	nCount; // not used

	// default does nothing
}
#endif

template<class TYPE, class ARG_TYPE>
BOOL CompareElements(const TYPE* pElement1, const ARG_TYPE* pElement2)
{
	ASSERT(AfxIsValidAddress(pElement1, sizeof(TYPE), FALSE));
	ASSERT(AfxIsValidAddress(pElement2, sizeof(ARG_TYPE), FALSE));

	return *pElement1 == *pElement2;
}

template<class ARG_KEY>
AFX_INLINE UINT HashKey(ARG_KEY key)
{
	// default identity hash - works for most primitive values
	return ((UINT)(void*)(DWORD)key) >> 4;
}

// special versions for CString
#if _MSC_VER >= 1100
template<> void ConstructElements<CString> (CString* pElements, int nCount);
template<> void DestructElements<CString> (CString* pElements, int nCount);
template<> void CopyElements<CString> (CString* pDest, const CString* pSrc, int nCount);
template<> void SerializeElements<CString> (CArchive& ar, CString* pElements, int nCount);
#ifndef OLE2ANSI
template<> UINT HashKey<LPCWSTR> (LPCWSTR key);
#endif
template<> UINT HashKey<LPCSTR> (LPCSTR key);
#else // _MSC_VER >= 1100
void ConstructElements(CString* pElements, int nCount);
void DestructElements(CString* pElements, int nCount);
void CopyElements(CString* pDest, const CString* pSrc, int nCount);
void SerializeElements(CArchive& ar, CString* pElements, int nCount);
#ifndef OLE2ANSI
UINT HashKey(LPCWSTR key);
#endif
UINT HashKey(LPCSTR key);
#endif // _MSC_VER >= 1100

// forward declarations
class COleVariant;
struct tagVARIANT;

// special versions for COleVariant
#if _MSC_VER >= 1100
template<> void ConstructElements<COleVariant> (COleVariant* pElements, int nCount);
template<> void DestructElements<COleVariant> (COleVariant* pElements, int nCount);
template<> void CopyElements<COleVariant> (COleVariant* pDest, const COleVariant* pSrc, int nCount);
template<> void SerializeElements<COleVariant> (CArchive& ar, COleVariant* pElements, int nCount);
#ifdef _DEBUG
template<> void DumpElements<COleVariant> (CDumpContext& dc, const COleVariant* pElements, int nCount);
#endif
template<> UINT HashKey<const struct tagVARIANT&> (const struct tagVARIANT& var);
#else // _MSC_VER >= 1100
void ConstructElements(COleVariant* pElements, int nCount);
void DestructElements(COleVariant* pElements, int nCount);
void CopyElements(COleVariant* pDest, const COleVariant* pSrc, int nCount);
void SerializeElements(CArchive& ar, COleVariant* pElements, int nCount);
#ifdef _DEBUG
void DumpElements(CDumpContext& dc, const COleVariant* pElements, int nCount);
#endif
UINT HashKey(const struct tagVARIANT& var);
#endif // _MSC_VER >= 1100

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CTypedPtrArray<BASE_CLASS, TYPE>

template<class BASE_CLASS, class TYPE>
class CTypedPtrArray : public BASE_CLASS
{
public:
	// Accessing elements
	TYPE GetAt(int nIndex) const
		{ return (TYPE)BASE_CLASS::GetAt(nIndex); }
	TYPE& ElementAt(int nIndex)
		{ return (TYPE&)BASE_CLASS::ElementAt(nIndex); }
	void SetAt(int nIndex, TYPE ptr)
		{ BASE_CLASS::SetAt(nIndex, ptr); }

	// Potentially growing the array
	void SetAtGrow(int nIndex, TYPE newElement)
	   { BASE_CLASS::SetAtGrow(nIndex, newElement); }
	int Add(TYPE newElement)
	   { return BASE_CLASS::Add(newElement); }
	int Append(const CTypedPtrArray<BASE_CLASS, TYPE>& src)
	   { return BASE_CLASS::Append(src); }
	void Copy(const CTypedPtrArray<BASE_CLASS, TYPE>& src)
		{ BASE_CLASS::Copy(src); }

	// Operations that move elements around
	void InsertAt(int nIndex, TYPE newElement, int nCount = 1)
		{ BASE_CLASS::InsertAt(nIndex, newElement, nCount); }
	void InsertAt(int nStartIndex, CTypedPtrArray<BASE_CLASS, TYPE>* pNewArray)
	   { BASE_CLASS::InsertAt(nStartIndex, pNewArray); }

	// overloaded operator helpers
	TYPE operator[](int nIndex) const
		{ return (TYPE)BASE_CLASS::operator[](nIndex); }
	TYPE& operator[](int nIndex)
		{ return (TYPE&)BASE_CLASS::operator[](nIndex); }
};

/////////////////////////////////////////////////////////////////////////////
// CTypedPtrList<BASE_CLASS, TYPE>

template<class BASE_CLASS, class TYPE>
class _CTypedPtrList : public BASE_CLASS
{
public:
// Construction
	_CTypedPtrList(int nBlockSize = 10)
		: BASE_CLASS(nBlockSize) { }

	// peek at head or tail
	TYPE& GetHead()
		{ return (TYPE&)BASE_CLASS::GetHead(); }
	TYPE GetHead() const
		{ return (TYPE)BASE_CLASS::GetHead(); }
	TYPE& GetTail()
		{ return (TYPE&)BASE_CLASS::GetTail(); }
	TYPE GetTail() const
		{ return (TYPE)BASE_CLASS::GetTail(); }

	// get head or tail (and remove it) - don't call on empty list!
	TYPE RemoveHead()
		{ return (TYPE)BASE_CLASS::RemoveHead(); }
	TYPE RemoveTail()
		{ return (TYPE)BASE_CLASS::RemoveTail(); }

	// iteration
	TYPE& GetNext(POSITION& rPosition)
		{ return (TYPE&)BASE_CLASS::GetNext(rPosition); }
	TYPE GetNext(POSITION& rPosition) const
		{ return (TYPE)BASE_CLASS::GetNext(rPosition); }
	TYPE& GetPrev(POSITION& rPosition)
		{ return (TYPE&)BASE_CLASS::GetPrev(rPosition); }
	TYPE GetPrev(POSITION& rPosition) const
		{ return (TYPE)BASE_CLASS::GetPrev(rPosition); }

	// getting/modifying an element at a given position
	TYPE& GetAt(POSITION position)
		{ return (TYPE&)BASE_CLASS::GetAt(position); }
	TYPE GetAt(POSITION position) const
		{ return (TYPE)BASE_CLASS::GetAt(position); }
	void SetAt(POSITION pos, TYPE newElement)
		{ BASE_CLASS::SetAt(pos, newElement); }
};

template<class BASE_CLASS, class TYPE>
class CTypedPtrList : public _CTypedPtrList<BASE_CLASS, TYPE>
{
public:
// Construction
	CTypedPtrList(int nBlockSize = 10)
		: _CTypedPtrList<BASE_CLASS, TYPE>(nBlockSize) { }

	// add before head or after tail
	POSITION AddHead(TYPE newElement)
		{ return BASE_CLASS::AddHead(newElement); }
	POSITION AddTail(TYPE newElement)
		{ return BASE_CLASS::AddTail(newElement); }

	// add another list of elements before head or after tail
	void AddHead(CTypedPtrList<BASE_CLASS, TYPE>* pNewList)
		{ BASE_CLASS::AddHead(pNewList); }
	void AddTail(CTypedPtrList<BASE_CLASS, TYPE>* pNewList)
		{ BASE_CLASS::AddTail(pNewList); }
};

// need specialized version for CPtrList because of AddHead/Tail ambiguity
template<> class CTypedPtrList<CPtrList, CPtrList*>
	: public _CTypedPtrList<CPtrList, CPtrList*>
{
public:
// Construction
	CTypedPtrList(int nBlockSize = 10)
		: _CTypedPtrList<CPtrList, CPtrList*>(nBlockSize) { }

	// add before head or after tail
	template<typename TYPE>
	POSITION AddHead(TYPE newElement)
		{ return _CTypedPtrList<CPtrList, CPtrList*>::AddHead((void*)newElement); }
	template<typename TYPE>
	POSITION AddTail(TYPE newElement)
		{ return _CTypedPtrList<CPtrList, CPtrList*>::AddTail((void*)newElement); }

	// add another list of elements before head or after tail
	template<typename BASE_CLASS, typename TYPE>
	void AddHead(CTypedPtrList<BASE_CLASS, TYPE>* pNewList)
		{ _CTypedPtrList<CPtrList, CPtrList*>::AddHead(pNewList); }
	template<typename BASE_CLASS, typename TYPE>
	void AddTail(CTypedPtrList<BASE_CLASS, TYPE>* pNewList)
		{ _CTypedPtrList<CPtrList, CPtrList*>::AddTail(pNewList); }
};

/////////////////////////////////////////////////////////////////////////////
// CTypedPtrMap<BASE_CLASS, KEY, VALUE>
#if 0
template<class BASE_CLASS, class KEY, class VALUE>
class CTypedPtrMap : public BASE_CLASS
{
public:

// Construction
	CTypedPtrMap(int nBlockSize = 10)
		: BASE_CLASS(nBlockSize) {}

	// Lookup
	BOOL Lookup(BASE_CLASS::BASE_ARG_KEY key, VALUE& rValue) const
		{ return BASE_CLASS::Lookup(key, (BASE_CLASS::BASE_VALUE&)rValue); }

	// Lookup and add if not there
	VALUE& operator[](BASE_CLASS::BASE_ARG_KEY key)
		{ return (VALUE&)BASE_CLASS::operator[](key); }

	// add a new key (key, value) pair
	void SetAt(KEY key, VALUE newValue)
		{ BASE_CLASS::SetAt(key, newValue); }

	// removing existing (key, ?) pair
	BOOL RemoveKey(KEY key)
		{ return BASE_CLASS::RemoveKey(key); }

	// iteration
	void GetNextAssoc(POSITION& rPosition, KEY& rKey, VALUE& rValue) const
		{ BASE_CLASS::GetNextAssoc(rPosition, (BASE_CLASS::BASE_KEY&)rKey,
			(BASE_CLASS::BASE_VALUE&)rValue); }
};
#endif

/////////////////////////////////////////////////////////////////////////////

#endif // __TEMPL_HPP__
