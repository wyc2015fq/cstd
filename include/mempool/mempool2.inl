typedef unsigned char EigthByte;
//内存池的默认大小和分配节点的默认大小
static  const size_t DEFAULTMEMEPOOLSIZE = 1000;
static  const size_t DEFAULTMEMENODESIZE = 100;
//内存初始分配内容   二进制位1111 1111
static  const int NEW_ALLOCATED_MEMORY_CONTENT = 0xFF;
//内存分配节点（双向链表）
typedef struct TMemeNode
{
  //指向前一节点
  TMemeNode *first;
  //指向后一节点
  TMemeNode *next;
  //节点大小
  size_t idataSize;
  //节点是否被使用
  bool isUsed;
  //分配的节点的后一节点
  TMemeNode *pEndNode;
  //记录内存池分配的首地址
  bool isMemeBegin;
  //保存分配的内存地址
  EigthByte *Data;
  //使用者对象的地址
  void **pUser;
} TMemeLinkNode;

//内存池的实现
class  CMemePool
{
public:
  //线程池构造函数
  CMemePool(const size_t &sInitialMemoryPoolSize = DEFAULTMEMEPOOLSIZE,
    const size_t &sMemoryChunkSize = DEFAULTMEMENODESIZE);
  ~CMemePool();
  //分配内存
  void *GetMemeroy(void **p,const size_t &sSize);
  //释放分配内存
  void FreeAllocMemeroy(void *p,const size_t &sSize);
  //释放内存池所有内存
  void FreeAllMemeroy();
  //展示内存池的使用情况
  void ShowTheMemePoolStatue();
  //获取错误信息
  void GetErrorInfo();
private:
  //禁止复制与构造，要传递就用引用吧
  CMemePool(CMemePool *tCMemePool);
  CMemePool& operator =(CMemePool &tCMemePool);
  void AllocPoolMemeroy();
  void CalLinkNodeNum();
  void CalMemeSize();
  void LinkMemeryToNode(EigthByte *PAlloc);
  void UpdateLinkNodeSize(TMemeNode *PNode);
  void CalNeetLinkNumber(const size_t &sSize);
  void* FindMemeNode(const size_t &sSize);
  TMemeNode * SearchAllocNode(void *p);
  void CleanAllMemeDate();
  void CleatAllLinkNode();
  void ResetLinkToMemery();
  //双向链表的头节点
  TMemeLinkNode *m_Head;
  //双向链表的当前节点
  TMemeLinkNode *m_Current;
  //双向链表的最后节点
  TMemeLinkNode *m_LastNode;
  EigthByte *m_PAlloc;
  //保存第一次运行头地址
  bool m_isFirst;
  //内存块分配数目
  size_t m_Number;
  //内存块总的数目
  size_t m_AllNumber;
  //每一个内存块的大小
  size_t m_MemLinkSize;
  //内存池分配的大小
  size_t m_MemePollSize;
  //内存块总分配大小
  size_t m_AllAloctsize;
  //内存池使用的大小
  size_t m_MemePoolUseSize;
  //内存池空闲的大小
  size_t m_MemePoolFreeSize;
  //分配了多少个对象
  size_t m_iUseObject;
  //保存错误信息
  //保存请求分配内存用户信息
  void **m_User;
};

//---------------------------------------------------------------------------
//recalloc分配新内存后，之前指向旧内存的指针就失效了
//需要重新定位，之前分配对象的指向也要重新定位

/*****************************************
内存池构造函数

*****************************************/
CMemePool::CMemePool(const size_t &sInitialMemoryPoolSize,const size_t &sMemoryChunkSize)
{
  //初始化内存池的大小
  m_MemePollSize =  sInitialMemoryPoolSize;
  //初始化每个内存块的大小
  m_MemLinkSize = sMemoryChunkSize;
  //初始化一些参数
  m_MemePoolFreeSize = 0;
  m_MemePoolUseSize = 0;
  m_Current = NULL;
  m_LastNode = NULL;
  m_PAlloc = NULL;
  m_Number = 0;
  m_AllAloctsize = 0;
  m_AllNumber = 0;
  m_iUseObject = 0;
  m_Head = new TMemeLinkNode;
  m_Head->next = NULL;
  m_Head->first = NULL;
  m_Head->Data = NULL;
  m_isFirst = true;
  //分配线程池函数
  AllocPoolMemeroy();
}
/*****************************************
内存池析构函数

*****************************************/
CMemePool::~CMemePool()
{
  FreeAllMemeroy();
}
/*****************************************
内存池分配内存函数

*****************************************/
void CMemePool::AllocPoolMemeroy()
{
  //计算需要的链表节点数目
  CalLinkNodeNum();
  //计算真正要分配的内存大小
  CalMemeSize();
  m_AllNumber = m_AllNumber + m_Number;
  m_AllAloctsize += m_MemePollSize;
  m_MemePoolFreeSize += m_MemePollSize;
  //追加分配内存，原有内存的内容不会受到影响
  m_PAlloc = (EigthByte *)realloc(m_PAlloc,(m_AllAloctsize)*sizeof(EigthByte));
  //内存分配失败
  if(NULL == m_PAlloc)
  {
#define m_sError printf
    m_sError("Alloc Memeroy Pool Failture");
    return;
  }
  //不是第一次分配内存
  if(false ==  m_isFirst)
  {   //新分配内存后原有指针全失效，需要重定向
    ResetLinkToMemery();
  }
  //分配的内存内容初始化
  // memset(((void *) PAlloc), NEW_ALLOCATED_MEMORY_CONTENT, m_MemePollSize);
  //将分配的线程池内存与链表节点关联
  LinkMemeryToNode(&m_PAlloc[m_AllAloctsize - m_MemePollSize]);
}
/*****************************************
将原内存的指针进行重定向（Alloc后原有内存可能被释放了）

*****************************************/
void CMemePool::ResetLinkToMemery()
{
  TMemeLinkNode *pTemp = m_Head->next;
  int iIndex = 0;
  while(NULL != pTemp)
  {
    //重定向指针链表的指向
    pTemp->Data = &m_PAlloc[iIndex * m_MemLinkSize];
    if(NULL != pTemp->pUser)
    {
      //重定向用户指针的指向
      *pTemp->pUser = pTemp->Data;
    }
    iIndex++;
    pTemp = pTemp->next;
  }
}
/*****************************************
计算需要的内存链表节点数目
*****************************************/
void CMemePool::CalLinkNodeNum()
{
  float fTempValue =   m_MemePollSize /  m_MemLinkSize;
  //向上取整需要的节点数目
  m_Number = ceil(fTempValue);
}
/*****************************************
计算内存池真正分配的内存的大小
*****************************************/
void CMemePool::CalMemeSize()
{
  m_MemePollSize = (size_t)(m_Number * m_MemLinkSize);
}
/*****************************************
将分配的内存和链表节点相关联
*****************************************/
void CMemePool::LinkMemeryToNode(EigthByte *PAlloc)
{
  TMemeLinkNode *PNode;
  //遍历每一个节点分配空间
  for(size_t iIndex = 0;  iIndex < m_Number; iIndex ++)
  {
    PNode = new TMemeLinkNode();
    if(NULL == m_LastNode)
    {
      PNode->next =  m_Head->next;
      m_Head->next = PNode;
      PNode->first = m_Head;
      m_LastNode = PNode;
    }
    else
    {
      PNode->next =  m_LastNode->next;
      m_LastNode->next = PNode;
      PNode->first = m_LastNode;
      m_LastNode = PNode;
    }
    m_LastNode->isUsed = false;
    m_LastNode->idataSize = m_MemePollSize - iIndex * m_MemLinkSize;
    m_LastNode->Data = &PAlloc[iIndex * m_MemLinkSize];
    m_LastNode->isMemeBegin = false;
    m_LastNode->pUser = NULL;
    //记录内存块的首地址，释放时使用
    if(true == m_isFirst && 0 == iIndex)
    {
      m_LastNode->isMemeBegin = true;
      m_isFirst = false;
    }
  }
  UpdateLinkNodeSize(m_LastNode);
}
/*****************************************
更新当前节点的前后大小值
*****************************************/
void CMemePool::UpdateLinkNodeSize(TMemeNode *PNode)
{
  TMemeNode *PTemp;
  PTemp = PNode->next;
  int iDateSize = 0;
  //当前节点的后一个节点没分配，得到它的DataSize值
  if(NULL != PTemp && false == PTemp->isUsed)
  {
    iDateSize = PTemp->idataSize;
  }
  //由最后一个节点在向前遍历，更新所有的节点大小值
  int iIndex = 1;
  while(PNode != m_Head && false == PNode->isUsed)
  {
    PNode->idataSize =  iIndex * m_MemLinkSize + iDateSize;
    iIndex++;
    PNode = PNode->first;
  }
  m_Current = PNode->next;
}
/*****************************************
分配内存空间

*****************************************/
void *CMemePool::GetMemeroy(void **p,const size_t &sSize)
{
  m_MemePoolFreeSize -= sSize;
  m_MemePoolUseSize +=  sSize;
  //保存请求内存分配的用户信息
  m_User = p;
  //增加分配对象数目
  m_iUseObject++;
  //有合适的内存块
  void *pFind = FindMemeNode(sSize);
  if(NULL != pFind)
  {
    return pFind;
  }
  TMemeNode *PTemp;
  PTemp = m_Current;
  m_Current = m_Current->next;
  //遍历内存块找到合适的内存
  while(PTemp != m_Current)
  {
    //走到结尾，从头来
    if(NULL == m_Current)
    {
      m_Current = m_Head->next;
    }
    //跳过已经分配的节点
    if(true == m_Current->isUsed)
    {
      m_Current = m_Current->pEndNode;
      //  m_Current = m_Current->first;
    }
    pFind = FindMemeNode(sSize);
    if(NULL != pFind)
    {
      return pFind;
    }
    if(PTemp == m_Current)
    {
      break;
    }
    m_Current = m_Current->next;
  }
  //在当前的所有节点链表中没有合适的，新分配吧
  m_MemePollSize = sSize;
  AllocPoolMemeroy();
  return FindMemeNode(sSize);
}
/*****************************************
计算所需的内存块数目

*****************************************/
void CMemePool::CalNeetLinkNumber(const size_t &sSize)
{
  float fTempValue =   sSize /  m_MemLinkSize;
  //向上取整需要的节点数目
  m_Number = ceil(fTempValue);
  if(0 == m_Number)
  {
    m_Number = 1;
  }
}
/*****************************************
找到合适的内存分配节点了

*****************************************/
void* CMemePool::FindMemeNode(const size_t &sSize)
{
  if(m_Current->idataSize >= sSize && false == m_Current->isUsed)
  {
    CalNeetLinkNumber(sSize);
    size_t iIndex = 0;
    TMemeLinkNode *pTemp = m_Current;
    while(iIndex < m_Number)
    {
      m_Current->isUsed = true;
      m_Current = m_Current->next;
      iIndex++;
    }
    //保存分配内存的用户信息
    pTemp->pUser = m_User;
    //记录分配了几块内存，避免后续遍历
    pTemp->pEndNode = m_Current->next;
    return pTemp->Data;
  }
  else
  {
    return NULL;
  }
}
/*****************************************
收回分配的内存块

*****************************************/
void CMemePool::FreeAllocMemeroy(void *p,const size_t &sSize)
{
  //减少分配数目
  m_iUseObject--;
  m_MemePoolUseSize -= sSize;
  m_MemePoolFreeSize += sSize;
  TMemeNode *pFind = SearchAllocNode(p);
  if(NULL == pFind)
  {
    m_sError("can not find the alloc point,you may use wrong");
  }
  //恢复内存为初始化
  //memset(((void *) pFind->Data), NEW_ALLOCATED_MEMORY_CONTENT, sSize);
  //计算向后移动多少内存块
  CalNeetLinkNumber(sSize);
  size_t iIndex = 0;
  while(iIndex != m_Number)
  {
    pFind->isUsed = false;
    pFind = pFind->next;
    iIndex++;
  }
  //内存归还后，更新前后节点的大小
  UpdateLinkNodeSize(pFind->first);
}
/*****************************************
查找之前分配内存的节点

*****************************************/
TMemeNode * CMemePool::SearchAllocNode(void *p)
{
  TMemeNode *PTemp = m_Head->next;
  while(NULL != PTemp)
  {
    if(PTemp->Data == (EigthByte *) p)
    {
      //释放内存的用户清空之前保存的用户信息
      PTemp->pUser = NULL;
      return PTemp;
    }
    PTemp = PTemp->next;
  }
  return NULL;
}
/*****************************************
清空线程池

*****************************************/
void CMemePool::FreeAllMemeroy()
{
  //所有内存对象都释放了
  if(0 != m_iUseObject)
  {
    m_sError("warning there is some object not release");
  }
  CleanAllMemeDate();
  CleatAllLinkNode();
}
/*****************************************
清空向系统申请的内存

*****************************************/
void CMemePool::CleanAllMemeDate()
{
  TMemeLinkNode *pTemp = m_Head->next;
  while(NULL != pTemp)
  {
    //内存被连城了一块，从首地址就可以全部删除
    if(pTemp->isMemeBegin&&NULL != pTemp->Data)
    {
      delete []pTemp->Data;
      return;
    }
    pTemp = pTemp->next;
  }
}
/*****************************************
清空双向链表

*****************************************/
void CMemePool::CleatAllLinkNode()
{
  TMemeLinkNode *pTemp = m_Head->next;
  while(NULL != pTemp)
  {
    TMemeLinkNode *qTemp = pTemp;
    pTemp = pTemp->next;
    delete qTemp;
  }
  if(NULL != m_Head)
  {
    delete m_Head;
  }
}
/*****************************************
显示内存池运行状态

*****************************************/
void CMemePool::ShowTheMemePoolStatue()
{
  printf("\n\n\t\t\t内存池使用状况输出\t\t\t\n\n");
  printf("\t\t总内存池大小:%d使用大小:%d空闲大小:%d\n", m_AllAloctsize, m_MemePoolUseSize, m_MemePoolFreeSize);
  TMemeLinkNode *pTemp = m_Head->next;
  int iIndex = 1;
  while(NULL != pTemp)
  {
    printf("\n内存池号:%d\t\t大小：%d\t\t是否被使用:%d\n", iIndex, pTemp->idataSize, pTemp->isUsed);
    iIndex++;
    pTemp = pTemp->next;
  }
}



int test_mempool()
{
  CMemePool *g_ptrMemPool = new CMemePool();
  char *ptrCharArray = (char *) g_ptrMemPool->GetMemeroy((void**)&ptrCharArray,700);
  g_ptrMemPool->ShowTheMemePoolStatue();
  char * ptrCharArrayB = (char *) g_ptrMemPool->GetMemeroy((void**)&ptrCharArrayB,80);
  g_ptrMemPool->ShowTheMemePoolStatue();
  char * ptrCharArrayC = (char *) g_ptrMemPool->GetMemeroy((void**)&ptrCharArrayC,400);
  g_ptrMemPool->ShowTheMemePoolStatue();
  g_ptrMemPool->FreeAllocMemeroy(ptrCharArray, 700);
  g_ptrMemPool->ShowTheMemePoolStatue();
  g_ptrMemPool->FreeAllocMemeroy(ptrCharArrayC, 400);
  g_ptrMemPool->FreeAllocMemeroy(ptrCharArrayB, 80);
  ptrCharArray = (char *) g_ptrMemPool->GetMemeroy((void**)&ptrCharArray,1300);
  g_ptrMemPool->ShowTheMemePoolStatue();
  for(int i=0; i< 1200; i++)
  {
    ptrCharArray[i] = 'a';
  }
  g_ptrMemPool->ShowTheMemePoolStatue();
  char c = ptrCharArray[800];
  g_ptrMemPool->FreeAllocMemeroy(ptrCharArray, 1300);
  g_ptrMemPool->ShowTheMemePoolStatue();
  delete g_ptrMemPool;
  printf("MemoryPool Program finished...\n");
  //system("PAUSE");
  return 0;
}
