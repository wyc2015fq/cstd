
#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "templ.hpp"

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class CMap
{
public:
  // Association
  struct CAssoc
  {
    CAssoc* pNext;
    UINT nHashValue;  // needed for efficient iteration
    KEY key;
    VALUE value;
  };
  CAssoc** m_pHashTable;
  UINT m_nHashTableSize;
  int m_nCount;
  CAssoc* m_pFreeList;
  struct CPlex* m_pBlocks;
  int m_nBlockSize;
  
  
  /////////////////////////////////////////////////////////////////////////////
  // CMap<KEY, ARG_KEY, VALUE, ARG_VALUE> inline functions
  
  
  AFX_INLINE int GetCount() const
  { return m_nCount; }
  
  AFX_INLINE BOOL IsEmpty() const
  { return m_nCount == 0; }
  
  AFX_INLINE void SetAt(ARG_KEY key, ARG_VALUE newValue)
  { (*this)[key] = newValue; }
  
  AFX_INLINE POSITION GetStartPosition() const
  { return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }
  
  AFX_INLINE UINT GetHashTableSize() const
  { return m_nHashTableSize; }
  
  /////////////////////////////////////////////////////////////////////////////
  // CMap<KEY, ARG_KEY, VALUE, ARG_VALUE> out-of-line functions
  
  
  CMap(int nBlockSize = 10)
  {
    ASSERT(nBlockSize > 0);
    
    m_pHashTable = NULL;
    m_nHashTableSize = 17;  // default size
    m_nCount = 0;
    m_pFreeList = NULL;
    m_pBlocks = NULL;
    m_nBlockSize = nBlockSize;
  }
  
  
  void InitHashTable(UINT nHashSize, BOOL bAllocNow = TRUE)
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
  
  
  void RemoveAll()
  {
    ASSERT_VALID(this);
    
    if (m_pHashTable != NULL)
    {
      // destroy elements (values and keys)
      for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
      {
        CAssoc* pAssoc;
        for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
        pAssoc = pAssoc->pNext)
        {
          DestructElements<VALUE>(&pAssoc->value, 1);
          DestructElements<KEY>(&pAssoc->key, 1);
        }
      }
    }
    
    // free hash table
    delete[] m_pHashTable;
    m_pHashTable = NULL;
    
    m_nCount = 0;
    m_pFreeList = NULL;
    m_pBlocks->FreeDataChain();
    m_pBlocks = NULL;
  }
  
  
  ~CMap()
  {
    RemoveAll();
    ASSERT(m_nCount == 0);
  }
  
  
  CAssoc*
    NewAssoc()
  {
    if (m_pFreeList == NULL)
    {
      // add another block
      CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CMap::CAssoc));
      // chain them into free list
      CMap::CAssoc* pAssoc = (CMap::CAssoc*) newBlock->data();
      // free in reverse order to make it easier to debug
      pAssoc += m_nBlockSize - 1;
      for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)
      {
        pAssoc->pNext = m_pFreeList;
        m_pFreeList = pAssoc;
      }
    }
    ASSERT(m_pFreeList != NULL);  // we must have something
    
    CMap::CAssoc* pAssoc = m_pFreeList;
    m_pFreeList = m_pFreeList->pNext;
    m_nCount++;
    ASSERT(m_nCount > 0);  // make sure we don't overflow
    ConstructElements<KEY>(&pAssoc->key, 1);
    ConstructElements<VALUE>(&pAssoc->value, 1);   // special construct values
    return pAssoc;
  }
  
  
  void FreeAssoc(CMap::CAssoc* pAssoc)
  {
    DestructElements<VALUE>(&pAssoc->value, 1);
    DestructElements<KEY>(&pAssoc->key, 1);
    pAssoc->pNext = m_pFreeList;
    m_pFreeList = pAssoc;
    m_nCount--;
    ASSERT(m_nCount >= 0);  // make sure we don't underflow
    
    // if no more elements, cleanup completely
    if (m_nCount == 0)
      RemoveAll();
  }
  
  
  CAssoc*
    GetAssocAt(ARG_KEY key, UINT& nHash) const
    // find association (or return NULL)
  {
    nHash = HashKey<ARG_KEY>(key) % m_nHashTableSize;
    
    if (m_pHashTable == NULL)
      return NULL;
    
    // see if it exists
    CAssoc* pAssoc;
    for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
    {
      if (CompareElements(&pAssoc->key, &key))
        return pAssoc;
    }
    return NULL;
  }
  
  
  BOOL Lookup(ARG_KEY key, VALUE& rValue) const
  {
    ASSERT_VALID(this);
    
    UINT nHash;
    CAssoc* pAssoc = GetAssocAt(key, nHash);
    if (pAssoc == NULL)
      return FALSE;  // not in map
    
    rValue = pAssoc->value;
    return TRUE;
  }
  
  
  VALUE& operator[](ARG_KEY key)
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
  
  
  BOOL RemoveKey(ARG_KEY key)
    // remove key - return TRUE if removed
  {
    ASSERT_VALID(this);
    
    if (m_pHashTable == NULL)
      return FALSE;  // nothing in the table
    
    CAssoc** ppAssocPrev;
    ppAssocPrev = &m_pHashTable[HashKey<ARG_KEY>(key) % m_nHashTableSize];
    
    CAssoc* pAssoc;
    for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
    {
      if (CompareElements(&pAssoc->key, &key))
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
  
  
  void GetNextAssoc(POSITION& rNextPosition,
    KEY& rKey, VALUE& rValue) const
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
  
  
  void Serialize(CArchive& ar)
  {
    ASSERT_VALID(this);
    
    CObject::Serialize(ar);
    
    if (ar.IsStoring())
    {
      ar.WriteCount(m_nCount);
      if (m_nCount == 0)
        return;  // nothing more to do
      
      ASSERT(m_pHashTable != NULL);
      for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
      {
        CAssoc* pAssoc;
        for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
        pAssoc = pAssoc->pNext)
        {
          SerializeElements<KEY>(ar, &pAssoc->key, 1);
          SerializeElements<VALUE>(ar, &pAssoc->value, 1);
        }
      }
    }
    else
    {
      DWORD nNewCount = ar.ReadCount();
      while (nNewCount--)
      {
        KEY newKey;
        VALUE newValue;
        SerializeElements<KEY>(ar, &newKey, 1);
        SerializeElements<VALUE>(ar, &newValue, 1);
        SetAt(newKey, newValue);
      }
    }
  }
  
#ifdef _DEBUG
  
  void Dump(CDumpContext& dc) const
  {
    CObject::Dump(dc);
    
    dc << "with " << m_nCount << " elements";
    if (dc.GetDepth() > 0)
    {
      // Dump in format "[key] -> value"
      KEY key;
      VALUE val;
      
      POSITION pos = GetStartPosition();
      while (pos != NULL)
      {
        GetNextAssoc(pos, key, val);
        dc << "\n\t[";
        DumpElements<KEY>(dc, &key, 1);
        dc << "] = ";
        DumpElements<VALUE>(dc, &val, 1);
      }
    }
    
    dc << "\n";
  }
  
  
  void AssertValid() const
  {
    CObject::AssertValid();
    
    ASSERT(m_nHashTableSize > 0);
    ASSERT(m_nCount == 0 || m_pHashTable != NULL);
    // non-empty map should have hash table
  }
#endif //_DEBUG
  
};

#endif // __MAP_HPP__

