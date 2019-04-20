
#include "Archive.hpp"
#include "Plex.hpp"

struct CMapStringToPtr
{
  
  DECLARE_DYNAMIC(CMapStringToPtr);
  
  // Association
  struct CAssoc
  {
    CAssoc* pNext;
    UINT nHashValue;  // needed for efficient iteration
    CString key;
    void* value;
  };
  
  
  CAssoc** m_pHashTable;
  UINT m_nHashTableSize;
  int m_nCount;
  CAssoc* m_pFreeList;
  struct CPlex* m_pBlocks;
  int m_nBlockSize;
  
  
  // local typedefs for CTypedPtrMap struct template
  typedef CString BASE_KEY;
  typedef LPCTSTR BASE_ARG_KEY;
  typedef void* BASE_VALUE;
  typedef void* BASE_ARG_VALUE;
  ////////////////////////////////////////////////////////////////////////////
  _AFXCOLL_INLINE int CMapStringToPtr::GetCount() const
  { return m_nCount; }
  _AFXCOLL_INLINE BOOL CMapStringToPtr::IsEmpty() const
  { return m_nCount == 0; }
  _AFXCOLL_INLINE void CMapStringToPtr::SetAt(LPCTSTR key, void* newValue)
  { (*this)[key] = newValue; }
  _AFXCOLL_INLINE POSITION CMapStringToPtr::GetStartPosition() const
  { return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }
  _AFXCOLL_INLINE UINT CMapStringToPtr::GetHashTableSize() const
  { return m_nHashTableSize; }
  
  CMapStringToPtr::CMapStringToPtr(int nBlockSize = 10)
  {
    ASSERT(nBlockSize > 0);
    
    m_pHashTable = NULL;
    m_nHashTableSize = 17;  // default size
    m_nCount = 0;
    m_pFreeList = NULL;
    m_pBlocks = NULL;
    m_nBlockSize = nBlockSize;
  }
  
  inline UINT CMapStringToPtr::HashKey(LPCTSTR key) const
  {
    UINT nHash = 0;
    while (*key)
      nHash = (nHash<<5) + nHash + *key++;
    return nHash;
  }
  
  void CMapStringToPtr::InitHashTable(
    UINT nHashSize, BOOL bAllocNow = TRUE)
    //
    // Used to force allocation of a hash table or to override the default
    //   hash table size of (which is fairly small)
  {
    ASSERT_VALID(this);
    ASSERT(m_nCount == 0);
    ASSERT(nHashSize > 0);
    
    if (m_pHashTable != NULL)
    {
      // free hash table
      delete[] m_pHashTable;
      m_pHashTable = NULL;
    }
    
    if (bAllocNow)
    {
      m_pHashTable = new CAssoc* [nHashSize];
      memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);
    }
    m_nHashTableSize = nHashSize;
  }
  
  void CMapStringToPtr::RemoveAll()
  {
    ASSERT_VALID(this);
    
    if (m_pHashTable != NULL)
    {
      // destroy elements
      for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
      {
        CAssoc* pAssoc;
        for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
        pAssoc = pAssoc->pNext)
        {
          DestructElement(&pAssoc->key);  // free up string data
          
        }
      }
      
      // free hash table
      delete [] m_pHashTable;
      m_pHashTable = NULL;
    }
    
    m_nCount = 0;
    m_pFreeList = NULL;
    m_pBlocks->FreeDataChain();
    m_pBlocks = NULL;
  }
  
  CMapStringToPtr::~CMapStringToPtr()
  {
    RemoveAll();
    ASSERT(m_nCount == 0);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Assoc helpers
  // same as CList implementation except we store CAssoc's not CNode's
  //    and CAssoc's are singly linked all the time
  
  CMapStringToPtr::CAssoc*
    CMapStringToPtr::NewAssoc()
  {
    if (m_pFreeList == NULL)
    {
      // add another block
      CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize,
        sizeof(CMapStringToPtr::CAssoc));
      // chain them into free list
      CMapStringToPtr::CAssoc* pAssoc =
        (CMapStringToPtr::CAssoc*) newBlock->data();
      // free in reverse order to make it easier to debug
      pAssoc += m_nBlockSize - 1;
      for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)
      {
        pAssoc->pNext = m_pFreeList;
        m_pFreeList = pAssoc;
      }
    }
    ASSERT(m_pFreeList != NULL);  // we must have something
    
    CMapStringToPtr::CAssoc* pAssoc = m_pFreeList;
    m_pFreeList = m_pFreeList->pNext;
    m_nCount++;
    ASSERT(m_nCount > 0);  // make sure we don't overflow
    memcpy(&pAssoc->key, &afxEmptyString, sizeof(CString));
    
    
    
    pAssoc->value = 0;
    
    return pAssoc;
  }
  
  void CMapStringToPtr::FreeAssoc(CMapStringToPtr::CAssoc* pAssoc)
  {
    DestructElement(&pAssoc->key);  // free up string data
    
    pAssoc->pNext = m_pFreeList;
    m_pFreeList = pAssoc;
    m_nCount--;
    ASSERT(m_nCount >= 0);  // make sure we don't underflow
    
    // if no more elements, cleanup completely
    if (m_nCount == 0)
      RemoveAll();
  }
  
  CMapStringToPtr::CAssoc*
    CMapStringToPtr::GetAssocAt(LPCTSTR key, UINT& nHash) const
    // find association (or return NULL)
  {
    nHash = HashKey(key) % m_nHashTableSize;
    
    if (m_pHashTable == NULL)
      return NULL;
    
    // see if it exists
    CAssoc* pAssoc;
    for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
    {
      if (pAssoc->key == key)
        return pAssoc;
    }
    return NULL;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  BOOL CMapStringToPtr::Lookup(LPCTSTR key, void*& rValue) const
  {
    ASSERT_VALID(this);
    
    UINT nHash;
    CAssoc* pAssoc = GetAssocAt(key, nHash);
    if (pAssoc == NULL)
      return FALSE;  // not in map
    
    rValue = pAssoc->value;
    return TRUE;
  }
  
  BOOL CMapStringToPtr::LookupKey(LPCTSTR key, LPCTSTR& rKey) const
  {
    ASSERT_VALID(this);
    
    UINT nHash;
    CAssoc* pAssoc = GetAssocAt(key, nHash);
    if (pAssoc == NULL)
      return FALSE;  // not in map
    
    rKey = pAssoc->key.c_str();
    return TRUE;
  }
  
  void*& CMapStringToPtr::operator[](LPCTSTR key)
  {
    ASSERT_VALID(this);
    
    UINT nHash;
    CAssoc* pAssoc;
    if ((pAssoc = GetAssocAt(key, nHash)) == NULL)
    {
      if (m_pHashTable == NULL)
        InitHashTable(m_nHashTableSize);
      
      // it doesn't exist, add a new Association
      pAssoc = NewAssoc();
      pAssoc->nHashValue = nHash;
      pAssoc->key = key;
      // 'pAssoc->value' is a constructed object, nothing more
      
      // put into hash table
      pAssoc->pNext = m_pHashTable[nHash];
      m_pHashTable[nHash] = pAssoc;
    }
    return pAssoc->value;  // return new reference
  }
  
  
  BOOL CMapStringToPtr::RemoveKey(LPCTSTR key)
    // remove key - return TRUE if removed
  {
    ASSERT_VALID(this);
    
    if (m_pHashTable == NULL)
      return FALSE;  // nothing in the table
    
    CAssoc** ppAssocPrev;
    ppAssocPrev = &m_pHashTable[HashKey(key) % m_nHashTableSize];
    
    CAssoc* pAssoc;
    for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
    {
      if (pAssoc->key == key)
      {
        // remove it
        *ppAssocPrev = pAssoc->pNext;  // remove from list
        FreeAssoc(pAssoc);
        return TRUE;
      }
      ppAssocPrev = &pAssoc->pNext;
    }
    return FALSE;  // not found
  }
  
  
  /////////////////////////////////////////////////////////////////////////////
  // Iterating
  
  void CMapStringToPtr::GetNextAssoc(POSITION& rNextPosition,
    CString& rKey, void*& rValue) const
  {
    ASSERT_VALID(this);
    ASSERT(m_pHashTable != NULL);  // never call on empty map
    
    CAssoc* pAssocRet = (CAssoc*)rNextPosition;
    ASSERT(pAssocRet != NULL);
    
    if (pAssocRet == (CAssoc*) BEFORE_START_POSITION)
    {
      // find the first association
      for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
        if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
          break;
        ASSERT(pAssocRet != NULL);  // must find something
    }
    
    // find next association
    ASSERT(AfxIsValidAddress(pAssocRet, sizeof(CAssoc)));
    CAssoc* pAssocNext;
    if ((pAssocNext = pAssocRet->pNext) == NULL)
    {
      // go to next bucket
      for (UINT nBucket = pAssocRet->nHashValue + 1;
      nBucket < m_nHashTableSize; nBucket++)
        if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
          break;
    }
    
    rNextPosition = (POSITION) pAssocNext;
    
    // fill in return data
    rKey = pAssocRet->key;
    rValue = pAssocRet->value;
  }
  
  
  /////////////////////////////////////////////////////////////////////////////
  // Diagnostics
  
  void CMapStringToPtr::Dump(CDumpContext& dc) const
  {
    
    
    dc << "with " << m_nCount << " elements";
    if (dc.GetDepth() > 0)
    {
      // Dump in format "[key] -> value"
      CString key;
      void* val;
      
      POSITION pos = GetStartPosition();
      while (pos != NULL)
      {
        GetNextAssoc(pos, key, val);
        dc << "\n\t[" << key << "] = " << val;
      }
    }
    
    dc << "\n";
  }
  
  void CMapStringToPtr::AssertValid() const
  {
    ASSERT(m_nHashTableSize > 0);
    ASSERT(m_nCount == 0 || m_pHashTable != NULL);
    // non-empty map should have hash table
  }
  
};