#ifndef _SOCK_HASH_TABLE_H
#define _SOCK_HASH_TABLE_H
#ifdef WIN32
#define HASH_SIZE 1024
#else
#define HASH_SIZE 1
#endif
struct SOCK_GetIndex
{
    static int GetIndex(int key)
    {
        return key % HASH_SIZE;
    }
};
template <typename KEY, typename Element, typename SOCK_GetIndex, int size>
class SOCKHashTable
{
    class CElement
    {
    public:
        CElement(const KEY& key, const Element& element) :
            m_Key(key), m_Element(element), m_pHashPrev(NULL), m_pHashNext(NULL), m_pListPrev(NULL), m_pListNext(NULL) {}
        ~CElement() {}
    public:
        KEY   m_Key;
        Element m_Element;
        CElement* m_pHashPrev;
        CElement* m_pHashNext;
        CElement* m_pListPrev;
        CElement* m_pListNext;
    };
public:
    SOCKHashTable();
    ~SOCKHashTable()
    {
        Clear();
    }
public:
    BOOL InsertElement(const KEY& key, const Element& element);
    void ReleaseElement(const KEY& key);
    Element& GetElement(const KEY& key);
    BOOL HasElement(const KEY& key);
    BOOL GotoElement(const KEY& key);
    void GotoFirstElement() ;
    void GotoLastElement() ;
    void GotoNextElement() ;
    Element& GetCurElement() ;
    KEY& GetCurElementKey() ;
    BOOL HasCurElement() ;
    //  void ReleaseCurElement();
    void Clear();
private:
    CElement* m_pElement[size];
    CElement* m_pListHead;
    CElement* m_pListTail;
    CElement* m_pCurElement;
    CriticalSection m_syn;
};
template <typename KEY, typename Element, typename SOCK_GetIndex, int size>
inline SOCKHashTable<KEY, Element, SOCK_GetIndex, size>::SOCKHashTable()
{
    int i = 0;

    for (; i < size; ++i)
    {
        m_pElement[i] = NULL;
    }

    m_pListHead = NULL;
    m_pListTail = NULL;
    m_pCurElement = NULL;
}
template <typename KEY, typename Element, typename SOCK_GetIndex, int size>
inline BOOL SOCKHashTable<KEY, Element, SOCK_GetIndex, size>::InsertElement(const KEY& key, const Element& element)
{
    int index = SOCK_GetIndex::GetIndex(key);

    if (index < 0)
    {
        return false;
    }

    AoutLock lock(&m_syn);
    CElement* tmp = m_pElement[index];
    BOOL found = false;
    CElement* e = tmp;

    while (!found && NULL != e)
    {
        if (e->m_Key == key)
        {
            found = true;
            break;
        }

        e = e->m_pHashNext;
    }

    if (!found)
    {
        CElement* pNew = new CElement(key, element);

        if (NULL != tmp)
        {
            tmp->m_pHashPrev = pNew;
            pNew->m_pHashNext = tmp;
        }

        m_pElement[index] = pNew;

        if (NULL == m_pListHead)
        {
            m_pListHead = pNew;
            m_pListTail = pNew;
        }

        else
        {
            CElement* tail = m_pListTail;
            m_pListTail->m_pListNext = pNew;
            pNew->m_pListPrev = m_pListTail;
            m_pListTail = pNew;
        }

        //    m_pCurElement = pNew;
    }

    return true;
}
template <typename KEY, typename Element, typename SOCK_GetIndex, int size>
inline void SOCKHashTable<KEY, Element, SOCK_GetIndex, size>::ReleaseElement(const KEY& key)
{
    int index = SOCK_GetIndex::GetIndex(key);

    if (index < 0)
    {
        return;
    }

    AoutLock lock(&m_syn);
    CElement* tmp = m_pElement[index];
    BOOL found = false;
    CElement* e = tmp;

    while (!found && NULL != e)
    {
        if (e->m_Key == key)
        {
            found = true;
            break;
        }

        e = e->m_pHashNext;
    }

    if (found)
    {
        if (NULL == e->m_pHashPrev)
        {
            m_pElement[index] = e->m_pHashNext;

            if (m_pElement[index])
            {
                m_pElement[index]->m_pHashPrev = NULL;
            }
        }

        else
        {
            CElement* prev = e->m_pHashPrev;
            prev->m_pHashNext = e->m_pHashNext;

            if (e->m_pHashNext)
            {
                e->m_pHashNext->m_pHashPrev = prev;
            }
        }

        if (e == m_pCurElement)
        {
            m_pCurElement = e->m_pListNext;
        }

        if (NULL == e->m_pListPrev)
        {
            m_pListHead = e->m_pListNext;

            if (m_pListHead)
            {
                m_pListHead->m_pListPrev = NULL;
            }
        }

        else
        {
            CElement* prev = e->m_pListPrev;
            prev->m_pListNext = e->m_pListNext;

            if (e->m_pListNext)
            {
                e->m_pListNext->m_pHashPrev = prev;
            }

            else
            {
                m_pListTail = prev;
            }
        }

        delete e;
    }
}
template <typename KEY, typename Element, typename SOCK_GetIndex, int size>
inline Element& SOCKHashTable<KEY, Element, SOCK_GetIndex, size>::GetElement(const KEY& key)
{
    AoutLock lock(&m_syn);
    int index = SOCK_GetIndex::GetIndex(key);
    CElement* tmp = m_pElement[index];
    BOOL found = false;
    CElement* e = tmp;

    while (!found && NULL != e)
    {
        if (e->m_Key == key)
        {
            found = true;
            break;
        }

        e = e->m_pHashNext;
    }

    return e->m_Element;
}
template <typename KEY, typename Element, typename SOCK_GetIndex, int size>
inline BOOL SOCKHashTable<KEY, Element, SOCK_GetIndex, size>::HasElement(const KEY& key)
{
    AoutLock lock(&m_syn);
    int index = SOCK_GetIndex::GetIndex(key);

    if (index < 0)
    {
        return false;
    }

    CElement* tmp = m_pElement[index];
    BOOL found = false;
    CElement* e = tmp;

    while (!found && NULL != e)
    {
        if (e->m_Key == key)
        {
            found = true;
            break;
        }

        e = e->m_pHashNext;
    }

    return found;
}
template <typename KEY, typename Element, typename SOCK_GetIndex, int size>
inline void SOCKHashTable<KEY, Element, SOCK_GetIndex, size>::Clear()
{
    CElement* e = NULL;

    while (NULL != m_pListHead)
    {
        e = m_pListHead;
        m_pListHead = m_pListHead->m_pListNext;
        delete e;
    }

    m_pListHead = NULL;
    m_pListTail = NULL;
    int i = 0;

    for (; i < size ; ++i)
    {
        m_pElement[i] = NULL;
    }
}
template <typename KEY, typename Element, typename SOCK_GetIndex, int size>
inline void SOCKHashTable<KEY, Element, SOCK_GetIndex, size>::GotoFirstElement()
{
    AoutLock lock(&m_syn);
    m_pCurElement = m_pListHead;
}
template <typename KEY, typename Element, typename SOCK_GetIndex, int size>
inline void SOCKHashTable<KEY, Element, SOCK_GetIndex, size>::GotoLastElement()
{
    AoutLock lock(&m_syn);
    m_pCurElement = m_pListTail;
}
template <typename KEY, typename Element, typename SOCK_GetIndex, int size>
inline void SOCKHashTable<KEY, Element, SOCK_GetIndex, size>::GotoNextElement()
{
    AoutLock lock(&m_syn);

    if (m_pCurElement)
    {
        m_pCurElement = m_pCurElement->m_pListNext;
    }
}
template <typename KEY, typename Element, typename SOCK_GetIndex, int size>
inline Element& SOCKHashTable<KEY, Element, SOCK_GetIndex, size>::GetCurElement()
{
    AoutLock lock(&m_syn);
    return m_pCurElement->m_Element;
}
template <typename KEY, typename Element, typename SOCK_GetIndex, int size>
inline KEY& SOCKHashTable<KEY, Element, SOCK_GetIndex, size>::GetCurElementKey()
{
    AoutLock lock(&m_syn);
    return m_pCurElement->m_Key;
}
template <typename KEY, typename Element, typename SOCK_GetIndex, int size>
inline BOOL SOCKHashTable<KEY, Element, SOCK_GetIndex, size>::HasCurElement()
{
    AoutLock lock(&m_syn);
    return m_pCurElement ? true : false ;
}
//
// template <typename KEY, typename Element, typename SOCK_GetIndex, int size>
// inline void SOCKHashTable<KEY,Element,SOCK_GetIndex,size>::ReleaseCurElement()
// {
//  AoutLock lock( &m_syn );
//  if ( m_pCurElement )
//  {
//
//    m_pCurElement = m_pCurElement->m_pListNext;
//    ReleaseElement( tmp->m_Key );
//  }
// }
#endif

