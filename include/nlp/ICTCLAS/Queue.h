
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEUE_H__382C7319_66D8_4041_ABA2_EE25B9D15D9C__INCLUDED_)
#define AFX_QUEUE_H__382C7319_66D8_4041_ABA2_EE25B9D15D9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DynamicArray.h"

struct tagQueueElem {
  unsigned int nParent;//the weight
  unsigned int nIndex;//number of index in the parent node
  ELEMENT_TYPE eWeight;//the weight of last path
  struct tagQueueElem* next;
};
typedef struct tagQueueElem QUEUE_ELEMENT, *PQUEUE_ELEMENT;

class CQueue
{
public:
  bool IsSingle();
  bool IsEmpty(bool bBrowsed = false);
  int Push(unsigned int nValue = 0, //The value for parent node
      unsigned int nIndex = 0, //number of index in the parent node
      ELEMENT_TYPE eWeight = 0 //the weight of last path
          );
  int Pop(unsigned int* npValue, //The value for node
      unsigned int* npIndex,//number of index in the parent node
      ELEMENT_TYPE* epWeight = 0, //the weight of last path
      bool  bModify = true, //false: not modify the data
      bool bFirstGet = true //first get data,just for browse
         );

  CQueue();
  virtual ~CQueue();
private:
  PQUEUE_ELEMENT m_pHead;//The chain sort according the weight of shortest path
  PQUEUE_ELEMENT m_pLastAccess;//The node last accessed
};

#include "malloc.h"
#include "stdlib.h"
#include "memory.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQueue::CQueue()
{
  m_pHead = NULL;
  m_pLastAccess = NULL;
}

CQueue::~CQueue()
{
  PQUEUE_ELEMENT pCur = m_pHead, pTemp; //The pointer of queue chain

  while (pCur != NULL) {
    pTemp = pCur->next;
    delete pCur;
    pCur = pTemp;
  }
}

int CQueue::Push(unsigned int nValue,//The value for parent node
    unsigned int nIndex,//number of index in the parent node
    ELEMENT_TYPE eWeight//the weight of last path
                )
{
  //Sort it
  PQUEUE_ELEMENT pAdd, pCur = m_pHead, pPre = 0; //The pointer of queue chain

  while (pCur != NULL && pCur->eWeight < eWeight) {
    //Get the proper posption,sort according the weight
    pPre = pCur;
    pCur = pCur->next;
  }

  pAdd = (PQUEUE_ELEMENT)new QUEUE_ELEMENT;
  pAdd->nParent = nValue;
  pAdd->nIndex = nIndex;
  pAdd->eWeight = eWeight;
  pAdd->next = pCur;

  if (pPre == 0) {
    m_pHead = pAdd;
  }
  else {
    pPre->next = pAdd;
  }

  return 1;
}
int CQueue::Pop(unsigned int* npValue,//The value for parent node
    unsigned int* npIndex,//number of index in the parent node
    ELEMENT_TYPE* epWeight,//the weight of last path
    bool  bModify,//not modify the data
    bool  bFirstGet//first get data,just for browse
               )
{
  PQUEUE_ELEMENT pTemp;

  if (bModify) {
    pTemp = m_pHead;  //The temp buffer
  }
  else {
    if (bFirstGet) { //First get the data
      m_pLastAccess = m_pHead;  //The temp buffer
    }

    pTemp = m_pLastAccess;
  }

  if (pTemp == NULL) {
    return -1;  //fail get the value
  }

  if (npValue != 0) {
    *npValue = pTemp->nParent;
  }

  if (npIndex != 0) {
    *npIndex = pTemp->nIndex;
  }

  if (epWeight != 0) {
    *epWeight = pTemp->eWeight;
  }

  if (bModify) { //modify and get rid of the node
    m_pHead = pTemp->next;
    delete pTemp;//free the buffer
  }
  else {
    m_pLastAccess = pTemp->next;
  }

  return 1;
}

bool CQueue::IsEmpty(bool bBrowsed)//bBrowsed=true: judge whether the browse pointer got end.
{
  if (bBrowsed == true) {
    return (m_pLastAccess == NULL);
  }

  return (m_pHead == NULL);
}

bool CQueue::IsSingle()
{
  return (m_pHead != NULL && m_pHead->next == NULL);
}

#endif // !defined(AFX_QUEUE_H__382C7319_66D8_4041_ABA2_EE25B9D15D9C__INCLUDED_)
