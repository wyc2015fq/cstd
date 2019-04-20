
#include "Archive.hpp"
#include "Plex.hpp"

struct CMapPtrToWord
{
  
  DECLARE_DYNAMIC(CMapPtrToWord);
  
  // Association
  struct CAssoc
  {
    CAssoc* pNext;
    
    void* key;
    WORD value;
  };
  
  
  CAssoc** m_pHashTable;
  UINT m_nHashTableSize;
  int m_nCount;
  CAssoc* m_pFreeList;
  struct CPlex* m_pBlocks;
  int m_nBlockSize;
  
  
  // local typedefs for CTypedPtrMap struct template
  typedef void* BASE_KEY;
  typedef void* BASE_ARG_KEY;
  typedef WORD BASE_VALUE;
  typedef WORD BASE_ARG_VALUE;
  
  
  _AFXCOLL_INLINE int CMapPtrToWord::GetCount() const
  { return m_nCount; }
  _AFXCOLL_INLINE BOOL CMapPtrToWord::IsEmpty() const
  { return m_nCount == 0; }
  _AFXCOLL_INLINE void CMapPtrToWord::SetAt(void* key, WORD newValue)
  { (*this)[key] = newValue; }
  _AFXCOLL_INLINE POSITION CMapPtrToWord::GetStartPosition() const
  { return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }
  _AFXCOLL_INLINE UINT CMapPtrToWord::GetHashTableSize() const
  { return m_nHashTableSize; }
  
  CMapPtrToWord::CMapPtrToWord(int nBlockSize = 10)
  {
    ASSERT(nBlockSize > 0);
    
    m_pHashTable = NULL;
    m_nHashTableSize = 17;  // default size
    m_nCount = 0;
    m_pFreeList = NULL;
    m_pBlocks = NULL;
    m_nBlockSize = nBlockSize;
  }
  
  inline UINT CMapPtrToWord::HashKey(void* key) const
  {
    // default identity hash - works for most primitive values
    return ((UINT)(void*)(DWORD)key) >> 4;
  }
  
  void CMapPtrToWord::InitHashTable(UINT nHashSize, BOOL bAllocNow = TRUE)
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
  
  void CMapPtrToWord::RemoveAll()
  {
    ASSERT_VALID(this);
    
    
    
    if (m_pHashTable != NULL)
    {
      // free hash table
      delete[] m_pHashTable;
      m_pHashTable = NULL;
    }
    
    m_nCount = 0;
    m_pFreeList = NULL;
    m_pBlocks->FreeDataChain();
    m_pBlocks = NULL;
  }
  
  CMapPtrToWord::~CMapPtrToWord()
  {
    RemoveAll();
    ASSERT(m_nCount == 0);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Assoc helpers
  // same as CList implementation except we store CAssoc's not CNode's
  //    and CAssoc's are singly linked all the time
  
  CMapPtrToWord::CAssoc*
    CMapPtrToWord::NewAssoc()
  {
    if (m_pFreeList == NULL)
    {
      // add another block
      CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CMapPtrToWord::CAssoc));
      // chain them into free list
      CMapPtrToWord::CAssoc* pAssoc = (CMapPtrToWord::CAssoc*) newBlock->data();
      // free in reverse order to make it easier to debug
      pAssoc += m_nBlockSize - 1;
      for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)
      {
        pAssoc->pNext = m_pFreeList;
        m_pFreeList = pAssoc;
      }
    }
    ASSERT(m_pFreeList != NULL);  // we must have something
    
    CMapPtrToWord::CAssoc* pAssoc = m_pFreeList;
    m_pFreeList = m_pFreeList->pNext;
    m_nCount++;
    ASSERT(m_nCount > 0);  // make sure we don't overflow
    
    
    pAssoc->key = 0;
    
    
    
    
    pAssoc->value = 0;
    
    return pAssoc;
  }
  
  void CMapPtrToWord::FreeAssoc(CMapPtrToWord::CAssoc* pAssoc)
  {
    
    pAssoc->pNext = m_pFreeList;
    m_pFreeList = pAssoc;
    m_nCount--;
    ASSERT(m_nCount >= 0);  // make sure we don't underflow
    
    // if no more elements, cleanup completely
    if (m_nCount == 0)
      RemoveAll();
  }
  
  CMapPtrToWord::CAssoc*
    CMapPtrToWord::GetAssocAt(void* key, UINT& nHash) const
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
  
  BOOL CMapPtrToWord::Lookup(void* key, WORD& rValue) const
  {
    ASSERT_VALID(this);
    
    UINT nHash;
    CAssoc* pAssoc = GetAssocAt(key, nHash);
    if (pAssoc == NULL)
      return FALSE;  // not in map
    
    rValue = pAssoc->value;
    return TRUE;
  }
  
  WORD& CMapPtrToWord::operator[](void* key)
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
      
      pAssoc->key = key;
      // 'pAssoc->value' is a constructed object, nothing more
      
      // put into hash table
      pAssoc->pNext = m_pHashTable[nHash];
      m_pHashTable[nHash] = pAssoc;
    }
    return pAssoc->value;  // return new reference
  }
  
  
  BOOL CMapPtrToWord::RemoveKey(void* key)
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
  
  void CMapPtrToWord::GetNextAssoc(POSITION& rNextPosition,
    void*& rKey, WORD& rValue) const
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
      
      for (UINT nBucket = (HashKey(pAssocRet->key) % m_nHashTableSize) + 1;
      
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
  
  void CMapPtrToWord::Dump(CDumpContext& dc) const
  {
    
    
    dc << "with " << m_nCount << " elements";
    if (dc.GetDepth() > 0)
    {
      // Dump in format "[key] -> value"
      void* key;
      WORD val;
      
      POSITION pos = GetStartPosition();
      while (pos != NULL)
      {
        GetNextAssoc(pos, key, val);
        dc << "\n\t[" << key << "] = " << val;
      }
    }
    
    dc << "\n";
  }
  
  void CMapPtrToWord::AssertValid() const
  {
    ASSERT(m_nHashTableSize > 0);
    ASSERT(m_nCount == 0 || m_pHashTable != NULL);
    // non-empty map should have hash table
  }
  
  
};