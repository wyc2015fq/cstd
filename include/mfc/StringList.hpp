
#include "Archive.hpp"
#include "Plex.hpp"

struct CStringList
{
  
  DECLARE_SERIAL(CStringList);
  
  
  struct CNode
  {
    CNode* pNext;
    CNode* pPrev;
    CString data;
  };
  // Implementation
  
  CNode* m_pNodeHead;
  CNode* m_pNodeTail;
  int m_nCount;
  CNode* m_pNodeFree;
  struct CPlex* m_pBlocks;
  int m_nBlockSize;
  
  // local typedefs for struct templates
  typedef CString BASE_TYPE;
  typedef LPCTSTR BASE_ARG_TYPE;
  
  
  
  _AFXCOLL_INLINE int CStringList::GetCount() const
  { return m_nCount; }
  _AFXCOLL_INLINE BOOL CStringList::IsEmpty() const
  { return m_nCount == 0; }
  _AFXCOLL_INLINE CString& CStringList::GetHead()
  { ASSERT(m_pNodeHead != NULL);
  return m_pNodeHead->data; }
  _AFXCOLL_INLINE CString CStringList::GetHead() const
  { ASSERT(m_pNodeHead != NULL);
  return m_pNodeHead->data; }
  _AFXCOLL_INLINE CString& CStringList::GetTail()
  { ASSERT(m_pNodeTail != NULL);
  return m_pNodeTail->data; }
  _AFXCOLL_INLINE CString CStringList::GetTail() const
  { ASSERT(m_pNodeTail != NULL);
  return m_pNodeTail->data; }
  _AFXCOLL_INLINE POSITION CStringList::GetHeadPosition() const
  { return (POSITION) m_pNodeHead; }
  _AFXCOLL_INLINE POSITION CStringList::GetTailPosition() const
  { return (POSITION) m_pNodeTail; }
  _AFXCOLL_INLINE CString& CStringList::GetNext(POSITION& rPosition) // return *Position++
  { CNode* pNode = (CNode*) rPosition;
  ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
  rPosition = (POSITION) pNode->pNext;
  return pNode->data; }
  _AFXCOLL_INLINE CString CStringList::GetNext(POSITION& rPosition) const // return *Position++
  { CNode* pNode = (CNode*) rPosition;
  ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
  rPosition = (POSITION) pNode->pNext;
  return pNode->data; }
  _AFXCOLL_INLINE CString& CStringList::GetPrev(POSITION& rPosition) // return *Position--
  { CNode* pNode = (CNode*) rPosition;
  ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
  rPosition = (POSITION) pNode->pPrev;
  return pNode->data; }
  _AFXCOLL_INLINE CString CStringList::GetPrev(POSITION& rPosition) const // return *Position--
  { CNode* pNode = (CNode*) rPosition;
  ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
  rPosition = (POSITION) pNode->pPrev;
  return pNode->data; }
  _AFXCOLL_INLINE CString& CStringList::GetAt(POSITION position)
  { CNode* pNode = (CNode*) position;
  ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
  return pNode->data; }
  _AFXCOLL_INLINE CString CStringList::GetAt(POSITION position) const
  { CNode* pNode = (CNode*) position;
  ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
  return pNode->data; }
  _AFXCOLL_INLINE void CStringList::SetAt(POSITION pos, LPCTSTR newElement)
  { CNode* pNode = (CNode*) pos;
  ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
  pNode->data = newElement; }
  
  _AFXCOLL_INLINE void CStringList::SetAt(POSITION pos, const CString& newElement)
  { CNode* pNode = (CNode*) pos;
  ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
  pNode->data = newElement; }
  
  
  CStringList::CStringList(int nBlockSize = 10)
  {
    ASSERT(nBlockSize > 0);
    
    m_nCount = 0;
    m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
    m_pBlocks = NULL;
    m_nBlockSize = nBlockSize;
  }
  
  void CStringList::RemoveAll()
  {
    ASSERT_VALID(this);
    
    // destroy elements
    
    CNode* pNode;
    for (pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)
      DestructElement(&pNode->data);
    
    
    m_nCount = 0;
    m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
    m_pBlocks->FreeDataChain();
    m_pBlocks = NULL;
  }
  
  CStringList::~CStringList()
  {
    RemoveAll();
    ASSERT(m_nCount == 0);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Node helpers
  /*
  * Implementation note: CNode's are stored in CPlex blocks and
  *  chained together. Free blocks are maintained in a singly linked list
  *  using the 'pNext' member of CNode with 'm_pNodeFree' as the head.
  *  Used blocks are maintained in a doubly linked list using both 'pNext'
  *  and 'pPrev' as links and 'm_pNodeHead' and 'm_pNodeTail'
  *   as the head/tail.
  *
  * We never free a CPlex block unless the List is destroyed or RemoveAll()
  *  is used - so the total number of CPlex blocks may grow large depending
  *  on the maximum past size of the list.
  */
  
  CStringList::CNode*
    CStringList::NewNode(CStringList::CNode* pPrev, CStringList::CNode* pNext)
  {
    if (m_pNodeFree == NULL)
    {
      // add another block
      CPlex* pNewBlock = CPlex::Create(m_pBlocks, m_nBlockSize,
        sizeof(CNode));
      
      // chain them into free list
      CNode* pNode = (CNode*) pNewBlock->data();
      // free in reverse order to make it easier to debug
      pNode += m_nBlockSize - 1;
      for (int i = m_nBlockSize-1; i >= 0; i--, pNode--)
      {
        pNode->pNext = m_pNodeFree;
        m_pNodeFree = pNode;
      }
    }
    ASSERT(m_pNodeFree != NULL);  // we must have something
    
    CStringList::CNode* pNode = m_pNodeFree;
    m_pNodeFree = m_pNodeFree->pNext;
    pNode->pPrev = pPrev;
    pNode->pNext = pNext;
    m_nCount++;
    ASSERT(m_nCount > 0);  // make sure we don't overflow
    
    
    ConstructElement(&pNode->data);
    
    
    
    return pNode;
  }
  
  void CStringList::FreeNode(CStringList::CNode* pNode)
  {
    
    DestructElement(&pNode->data);
    
    pNode->pNext = m_pNodeFree;
    m_pNodeFree = pNode;
    m_nCount--;
    ASSERT(m_nCount >= 0);  // make sure we don't underflow
    
    // if no more elements, cleanup completely
    if (m_nCount == 0)
      RemoveAll();
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  POSITION CStringList::AddHead(LPCTSTR newElement)
  {
    
    return AddHead(CString(newElement));
    
  }
  
  
  POSITION CStringList::AddHead(const CString& newElement)
  {
    ASSERT_VALID(this);
    
    CNode* pNewNode = NewNode(NULL, m_pNodeHead);
    pNewNode->data = newElement;
    if (m_pNodeHead != NULL)
      m_pNodeHead->pPrev = pNewNode;
    else
      m_pNodeTail = pNewNode;
    m_pNodeHead = pNewNode;
    return (POSITION) pNewNode;
  }
  
  
  POSITION CStringList::AddTail(LPCTSTR newElement)
  {
    
    return AddTail(CString(newElement));
    
  }
  
  
  POSITION CStringList::AddTail(const CString& newElement)
  {
    ASSERT_VALID(this);
    
    CNode* pNewNode = NewNode(m_pNodeTail, NULL);
    pNewNode->data = newElement;
    if (m_pNodeTail != NULL)
      m_pNodeTail->pNext = pNewNode;
    else
      m_pNodeHead = pNewNode;
    m_pNodeTail = pNewNode;
    return (POSITION) pNewNode;
  }
  
  
  void CStringList::AddHead(CStringList* pNewList)
  {
    ASSERT_VALID(this);
    
    ASSERT(pNewList != NULL);
    ASSERT_KINDOF(CStringList, pNewList);
    ASSERT_VALID(pNewList);
    
    // add a list of same elements to head (maintain order)
    POSITION pos = pNewList->GetTailPosition();
    while (pos != NULL)
      AddHead(pNewList->GetPrev(pos));
  }
  
  void CStringList::AddTail(CStringList* pNewList)
  {
    ASSERT_VALID(this);
    ASSERT(pNewList != NULL);
    ASSERT_KINDOF(CStringList, pNewList);
    ASSERT_VALID(pNewList);
    
    // add a list of same elements
    POSITION pos = pNewList->GetHeadPosition();
    while (pos != NULL)
      AddTail(pNewList->GetNext(pos));
  }
  
  CString CStringList::RemoveHead()
  {
    ASSERT_VALID(this);
    ASSERT(m_pNodeHead != NULL);  // don't call on empty list !!!
    ASSERT(AfxIsValidAddress(m_pNodeHead, sizeof(CNode)));
    
    CNode* pOldNode = m_pNodeHead;
    CString returnValue = pOldNode->data;
    
    m_pNodeHead = pOldNode->pNext;
    if (m_pNodeHead != NULL)
      m_pNodeHead->pPrev = NULL;
    else
      m_pNodeTail = NULL;
    FreeNode(pOldNode);
    return returnValue;
  }
  
  CString CStringList::RemoveTail()
  {
    ASSERT_VALID(this);
    ASSERT(m_pNodeTail != NULL);  // don't call on empty list !!!
    ASSERT(AfxIsValidAddress(m_pNodeTail, sizeof(CNode)));
    
    CNode* pOldNode = m_pNodeTail;
    CString returnValue = pOldNode->data;
    
    m_pNodeTail = pOldNode->pPrev;
    if (m_pNodeTail != NULL)
      m_pNodeTail->pNext = NULL;
    else
      m_pNodeHead = NULL;
    FreeNode(pOldNode);
    return returnValue;
  }
  
  POSITION CStringList::InsertBefore(POSITION position, LPCTSTR newElement)
  {
    
    return InsertBefore(position, CString(newElement));
    
  }
  
  
  POSITION CStringList::InsertBefore(POSITION position, const CString& newElement)
  {
    ASSERT_VALID(this);
    
    if (position == NULL)
      return AddHead(newElement); // insert before nothing -> head of the list
    
    // Insert it before position
    CNode* pOldNode = (CNode*) position;
    CNode* pNewNode = NewNode(pOldNode->pPrev, pOldNode);
    pNewNode->data = newElement;
    
    if (pOldNode->pPrev != NULL)
    {
      ASSERT(AfxIsValidAddress(pOldNode->pPrev, sizeof(CNode)));
      pOldNode->pPrev->pNext = pNewNode;
    }
    else
    {
      ASSERT(pOldNode == m_pNodeHead);
      m_pNodeHead = pNewNode;
    }
    pOldNode->pPrev = pNewNode;
    return (POSITION) pNewNode;
  }
  
  
  POSITION CStringList::InsertAfter(POSITION position, LPCTSTR newElement)
  {
    
    return InsertAfter(position, CString(newElement));
    
  }
  
  
  POSITION CStringList::InsertAfter(POSITION position, const CString& newElement)
  {
    ASSERT_VALID(this);
    
    if (position == NULL)
      return AddTail(newElement); // insert after nothing -> tail of the list
    
    // Insert it before position
    CNode* pOldNode = (CNode*) position;
    ASSERT(AfxIsValidAddress(pOldNode, sizeof(CNode)));
    CNode* pNewNode = NewNode(pOldNode, pOldNode->pNext);
    pNewNode->data = newElement;
    
    if (pOldNode->pNext != NULL)
    {
      ASSERT(AfxIsValidAddress(pOldNode->pNext, sizeof(CNode)));
      pOldNode->pNext->pPrev = pNewNode;
    }
    else
    {
      ASSERT(pOldNode == m_pNodeTail);
      m_pNodeTail = pNewNode;
    }
    pOldNode->pNext = pNewNode;
    return (POSITION) pNewNode;
  }
  
  
  void CStringList::RemoveAt(POSITION position)
  {
    ASSERT_VALID(this);
    
    CNode* pOldNode = (CNode*) position;
    ASSERT(AfxIsValidAddress(pOldNode, sizeof(CNode)));
    
    // remove pOldNode from list
    if (pOldNode == m_pNodeHead)
    {
      m_pNodeHead = pOldNode->pNext;
    }
    else
    {
      ASSERT(AfxIsValidAddress(pOldNode->pPrev, sizeof(CNode)));
      pOldNode->pPrev->pNext = pOldNode->pNext;
    }
    if (pOldNode == m_pNodeTail)
    {
      m_pNodeTail = pOldNode->pPrev;
    }
    else
    {
      ASSERT(AfxIsValidAddress(pOldNode->pNext, sizeof(CNode)));
      pOldNode->pNext->pPrev = pOldNode->pPrev;
    }
    FreeNode(pOldNode);
  }
  
  
  /////////////////////////////////////////////////////////////////////////////
  // slow operations
  
  POSITION CStringList::FindIndex(int nIndex) const
  {
    ASSERT_VALID(this);
    
    if (nIndex >= m_nCount || nIndex < 0)
      return NULL;  // went too far
    
    CNode* pNode = m_pNodeHead;
    while (nIndex--)
    {
      ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
      pNode = pNode->pNext;
    }
    return (POSITION) pNode;
  }
  
  POSITION CStringList::Find(LPCTSTR searchValue, POSITION startAfter = NULL) const
  {
    ASSERT_VALID(this);
    
    CNode* pNode = (CNode*) startAfter;
    if (pNode == NULL)
    {
      pNode = m_pNodeHead;  // start at head
    }
    else
    {
      ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
      pNode = pNode->pNext;  // start after the one specified
    }
    
    for (; pNode != NULL; pNode = pNode->pNext)
      if (pNode->data == searchValue)
        return (POSITION) pNode;
      return NULL;
  }
  
  
  /////////////////////////////////////////////////////////////////////////////
  // Serialization
  
  void CStringList::Serialize(CArchive& ar)
  {
    ASSERT_VALID(this);
    
    
    
    if (ar.IsStoring())
    {
      ar.WriteCount(m_nCount);
      for (CNode* pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)
      {
        ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
        ar << pNode->data;
      }
    }
    else
    {
      DWORD nNewCount = ar.ReadCount();
      CString newData;
      while (nNewCount--)
      {
        ar >> newData;
        AddTail(newData);
      }
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Diagnostics
  
#ifdef _DEBUG
  void CStringList::Dump(CDumpContext& dc) const
  {
    
    
    dc << "with " << m_nCount << " elements";
    if (dc.GetDepth() > 0)
    {
      POSITION pos = GetHeadPosition();
      while (pos != NULL)
        dc << "\n\t" << GetNext(pos);
    }
    
    dc << "\n";
  }
  
  void CStringList::AssertValid() const
  {
    
    
    if (m_nCount == 0)
    {
      // empty list
      ASSERT(m_pNodeHead == NULL);
      ASSERT(m_pNodeTail == NULL);
    }
    else
    {
      // non-empty list
      ASSERT(AfxIsValidAddress(m_pNodeHead, sizeof(CNode)));
      ASSERT(AfxIsValidAddress(m_pNodeTail, sizeof(CNode)));
    }
  }
#endif //_DEBUG
  
  
  
};