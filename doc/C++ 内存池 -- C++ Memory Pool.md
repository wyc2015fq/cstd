# C++ 内存池 -- C++ Memory Pool

 																				2006年10月08日 16:01:00 					[060](https://me.csdn.net/060) 						阅读数：29952 										

 									

这是我翻译的文章，来自 Code Project， 

原文作者: [**DanDanger2000**](http://www.codeproject.com/script/Articles/list_articles.asp?userid=750146). 

原文链接: <http://www.codeproject.com/cpp/MemoryPool.asp>

**C++ 内存池**

l  [下载示例工程 – 105Kb](http://www.codeproject.com/cpp/MemoryPool/MemoryPool_demo.zip)



l  [下载源代码 – 17.3Kb](http://www.codeproject.com/cpp/MemoryPool/MemoryPool_src.zip)



**![img](https://p-blog.csdn.net/images/p_blog_csdn_net/060/MemoryPool_ScreenShot.png)**



**目录**

l **引言**

l **它怎样工作**

l **示例**

l **使用这些代码**

l **好处**

l **关于代码**

l **ToDo**

l **历史**

 

## **引言**

C/C++的内存分配(通过malloc或new)可能需要花费很多时。

更糟糕的是，随着时间的流逝，内存(memory)将形成碎片，所以一个应用程序的运行会越来越慢当它运行了很长时间和/或执行了很多的内存分配(释放)操作的时候。特别是，你经常申请很小的一块内存，堆(heap)会变成碎片的。

### **解决方案:你自己的内存池**

一个(可能的)解决方法是内存池(Memory Pool)。

在启动的时候，一个”内存池”(Memory Pool)分配一块很大的内存，并将会将这个大块(block)分成较小的块(smaller chunks)。每次你从内存池申请内存空间时，它会从先前已经分配的块(chunks)中得到，而不是从操作系统。最大的优势在于：

l 非常少(几没有) 堆碎片

l 比通常的内存申请/释放(比如通过`malloc`, new等)的方式快

另外，你可以得到以下好处：

l 检查任何一个指针是否在内存池里

l 写一个”堆转储(Heap-Dump)”到你的硬盘(对事后的调试非常有用)

l 某种”内存泄漏检测(memory-leak detection)”：当你没有释放所有以前分配的内存时，内存池(Memory Pool)会抛出一个*断言*(*assertion*).

## **它怎样工作**

让我们看一看内存池(Memory Pool)的UML模式图：



![Memory Pool UML schema](https://p-blog.csdn.net/images/p_blog_csdn_net/060/MemoryPool_UML.png)



*这个模式图只显示了类*`*CMemoryPool*``*的一小部分，参看由*`*Doxygen**生成的文档以得到详细的类描述。*

 



**一个关于内存块(MemoryChunks)的单词**

你应该从模式图中看到，内存池(Memory Pool)管理了一个指向结构体`SMemoryChunk` (`m_ptrFirstChunk`, `m_ptrLastChunk`, and `m_ptrCursorChunk`)的指针。这些块(chunks)建立一个内存块(memory chunks)的链表。各自指向链表中的下一个块(chunk)。当从操作系统分配到一块内存时，它将完全的被`SMemoryChunk`s管理。让我们近一点看看一个块(chunk)。







 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)typedef struct SMemoryChunk
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  TByte *Data ;             // The actual Data
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  std::size_t DataSize ;    // Size of the "Data"-Block
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  std::size_t UsedSize ;    // actual used Size
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  bool IsAllocationChunk ;  // true, when this MemoryChunks
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                            // Points to a "Data"-Block
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                            // which can be deallocated via "free()"
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  SMemoryChunk *Next ;      // Pointer to the Next MemoryChunk
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                            // in the List (may be NULL)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)} SmemoryChunk;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)



每个块(chunk)持有一个指针，指针指向:

l 一小块内存(`Data`)，

l 从块(chunk)开始的可用内存的总大小(`DataSize`)，

l 实际使用的大小(`UsedSize`)，

l 以及一个指向链表中下一个块(chunk)的指针。

**第一步：预申请内存(pre-allocating the memory)**

当你调用`CmemoryPool`的构造函数，内存池(Memory Pool)将从操作系统申请它的第一块(大的)内存块(memory-chunk)





 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)/*Constructor
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)******************/
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CMemoryPool::CMemoryPool(const std::size_t &sInitialMemoryPoolSize,
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)                         const std::size_t &sMemoryChunkSize,
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)                         const std::size_t &sMinimalMemorySizeToAllocate,
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)                         bool bSetMemoryData)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  m_ptrFirstChunk  = NULL ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  m_ptrLastChunk   = NULL ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  m_ptrCursorChunk = NULL ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  m_sTotalMemoryPoolSize = 0 ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  m_sUsedMemoryPoolSize  = 0 ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  m_sFreeMemoryPoolSize  = 0 ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  m_sMemoryChunkSize   = sMemoryChunkSize ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  m_uiMemoryChunkCount = 0 ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  m_uiObjectCount      = 0 ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  m_bSetMemoryData               = bSetMemoryData ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  m_sMinimalMemorySizeToAllocate = sMinimalMemorySizeToAllocate ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  // Allocate the Initial amount of Memory from the Operating-System...
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  AllocateMemory(sInitialMemoryPoolSize) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)



类的所有成员通用的初始化在此完成，`AllocateMemory`最终完成了从操作系统申请内存。





 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)/******************
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)AllocateMemory
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)******************/
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)bool CMemoryPool::AllocateMemory(const std::size_t &sMemorySize)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  std::size_t sBestMemBlockSize = CalculateBestMemoryBlockSize(sMemorySize) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  // allocate from Operating System
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  TByte *ptrNewMemBlock = (TByte *) malloc (sBestMemBlockSize) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  ...
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)



那么，是如何管理数据的呢？

**第二步：已分配内存的分割(segmentation of allocated memory)**

正如前面提到的，内存池(Memory Pool)使用`SMemoryChunk`s管理所有数据。从OS申请完内存之后，我们的块(chunks)和实际的内存块(block)之间就不存在联系：



![Memory Pool after initial allocation](https://p-blog.csdn.net/images/p_blog_csdn_net/060/MemoryPool_Step1.png)

*Memory Pool after initial allocation*





我们需要分配一个结构体`SmemoryChunk`的数组来管理内存块：







 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  // (AllocateMemory()continued) : 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  ...
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  unsigned int uiNeededChunks = CalculateNeededChunks(sMemorySize) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  // allocate Chunk-Array to Manage the Memory
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  SMemoryChunk *ptrNewChunks = 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)    (SMemoryChunk *) malloc ((uiNeededChunks * sizeof(SMemoryChunk))) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  assert(((ptrNewMemBlock) && (ptrNewChunks)) 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)                           && "Error : System ran out of Memory") ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  ...
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)



`CalculateNeededChunks()`负责计算为管理已经得到的内存需要的块(chunks)的数量。分配完块(chunks)之后(通过`malloc`)，`ptrNewChunks`将指向一个`SmemoryChunk`s的数组。注意，数组里的块(chunks)现在持有的是垃圾数据，因为我们还没有给chunk-members赋有用的数据。内存池的堆(Memory Pool-"Heap"):



![after SMemoryChunk allocation](https://p-blog.csdn.net/images/p_blog_csdn_net/060/MemoryPool_Step2.png)



*Memory Pool after* `*SMemoryChunk*` *allocation*





还是那句话，数据块(data block)和chunks之间没有联系。但是，`AllocateMemory()`会照顾它。`LinkChunksToData()`最后将把数据块(data block)和chunks联系起来，并将为每个chunk-member赋一个可用的值。





 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// (AllocateMemory()continued) : 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  ...
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  // Associate the allocated Memory-Block with the Linked-List of MemoryChunks
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  return LinkChunksToData(ptrNewChunks, uiNeededChunks, ptrNewMemBlock) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)



让我们看看`LinkChunksToData()`：







 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)/******************
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)LinkChunksToData
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)******************/
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)bool CMemoryPool::LinkChunksToData(SMemoryChunk *ptrNewChunks, 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)     unsigned int uiChunkCount, TByte *ptrNewMemBlock)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  SMemoryChunk *ptrNewChunk = NULL ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  unsigned int uiMemOffSet = 0 ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  bool bAllocationChunkAssigned = false ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  for(unsigned int i = 0; i < uiChunkCount; i++)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)  {
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    if(!m_ptrFirstChunk)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)    {
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      m_ptrFirstChunk = SetChunkDefaults(&(ptrNewChunks[0])) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      m_ptrLastChunk = m_ptrFirstChunk ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      m_ptrCursorChunk = m_ptrFirstChunk ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    else
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)    {
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      ptrNewChunk = SetChunkDefaults(&(ptrNewChunks[i])) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      m_ptrLastChunk->Next = ptrNewChunk ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      m_ptrLastChunk = ptrNewChunk ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    uiMemOffSet = (i * ((unsigned int) m_sMemoryChunkSize)) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    m_ptrLastChunk->Data = &(ptrNewMemBlock[uiMemOffSet]) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    // The first Chunk assigned to the new Memory-Block will be 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    // a "AllocationChunk". This means, this Chunks stores the
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    // "original" Pointer to the MemBlock and is responsible for
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    // "free()"ing the Memory later....
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    if(!bAllocationChunkAssigned)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)    {
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      m_ptrLastChunk->IsAllocationChunk = true ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      bAllocationChunkAssigned = true ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)  }
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  return RecalcChunkMemorySize(m_ptrFirstChunk, m_uiMemoryChunkCount) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)



让我们一步步地仔细看看这个重要的函数：第一行检查链表里是否已经有可用的块(chunks):





 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  ...
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  if(!m_ptrFirstChunk)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  ...
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)



我们第一次给类的成员赋值：





 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  ...
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  m_ptrFirstChunk = SetChunkDefaults(&(ptrNewChunks[0])) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  m_ptrLastChunk = m_ptrFirstChunk ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  m_ptrCursorChunk = m_ptrFirstChunk ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  ...
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)



`m_ptrFirstChunk`现在指向块数组(chunks-array)的**第一个**块，每一个块严格的管理来自内存(memory block)的`m_sMemoryChunkSize`个字节。一个”偏移量”(offset)——这个值是可以计算的所以每个(chunk)能够指向内存块(memory block)的特定部分。





  

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  uiMemOffSet = (i * ((unsigned int) m_sMemoryChunkSize)) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  m_ptrLastChunk->Data = &(ptrNewMemBlock[uiMemOffSet]) ; 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)



另外，每个新的来自数组的`SmemoryChunk`将被追加到链表的最后**一个**元素(并且它自己将成为最后一个元素):



 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  ...
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  m_ptrLastChunk->Next = ptrNewChunk ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  m_ptrLastChunk = ptrNewChunk ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  ...
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)





在接下来的"*for loop" 中，内存池(memory pool)将连续的给数组中的所有块(chunks)赋一个可用的数据。*



*![Memory and chunks linked together](https://p-blog.csdn.net/images/p_blog_csdn_net/060/MemoryPool_Step3.png)*

*Memory and chunks linked together, pointing to valid data*



最后，我们必须重新计算每个块(chunk)能够管理的总的内存大小。这是一个费时的，但是在新的内存追加到内存池时必须做的一件事。这个总的大小将被赋值给chunk的`DataSize `成员。



 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)/******************
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)RecalcChunkMemorySize
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)******************/
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)bool CMemoryPool::RecalcChunkMemorySize(SMemoryChunk *ptrChunk, 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)                  unsigned int uiChunkCount)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  unsigned int uiMemOffSet = 0 ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  for(unsigned int i = 0; i < uiChunkCount; i++)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)  {
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    if(ptrChunk)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)    {
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      uiMemOffSet = (i * ((unsigned int) m_sMemoryChunkSize)) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      ptrChunk->DataSize = 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        (((unsigned int) m_sTotalMemoryPoolSize) - uiMemOffSet) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      ptrChunk = ptrChunk->Next ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    else
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)    {
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     assert(false && "Error : ptrChunk == NULL") ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     return false ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)  }
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  return true ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)



`RecalcChunkMemorySize`之后，每个chunk都知道它指向的空闲内存的大小。所以，将很容易确定一个chunk是否能够持有一块特定大小的内存：当`DataSize`成员大于(或等于)已经申请的内存大小以及`DataSize`成员是0，于是chunk有能力持有一块内存。最后，内存分割完成了。为了不让事情太抽象，我们假定内存池(memory pool )包含600字节，每个chunk持有100字节。



![Memory segmentation finished.](https://p-blog.csdn.net/images/p_blog_csdn_net/060/MemoryPool_Step4.png)

  

*Memory segmentation finished. Each chunk manages exactly 100 bytes*

**第三步：从内存池申请内存(requesting memory from the memory pool)**

那么，如果用户从内存池申请内存会发生什么？最初，内存池里的所有数据是空闲的可用的：

 



![All memory blocks are available](https://p-blog.csdn.net/images/p_blog_csdn_net/060/MemoryPool_Step5.png)

*All memory blocks are available*

我们看看`GetMemory`:





 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)/******************
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)GetMemory
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)******************/
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)void *CMemoryPool::GetMemory(const std::size_t &sMemorySize)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  std::size_t sBestMemBlockSize = CalculateBestMemoryBlockSize(sMemorySize) ;  
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  SMemoryChunk *ptrChunk = NULL ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  while(!ptrChunk)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)  {
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    // Is a Chunks available to hold the requested amount of Memory ?
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    ptrChunk = FindChunkSuitableToHoldMemory(sBestMemBlockSize) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    if (!ptrChunk)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)    {
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      // No chunk can be found
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      // => Memory-Pool is to small. We have to request 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      //    more Memory from the Operating-System....
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      sBestMemBlockSize = MaxValue(sBestMemBlockSize, 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        CalculateBestMemoryBlockSize(m_sMinimalMemorySizeToAllocate)) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      AllocateMemory(sBestMemBlockSize) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)  }
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  // Finally, a suitable Chunk was found.
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  // Adjust the Values of the internal "TotalSize"/"UsedSize" Members and 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  // the Values of the MemoryChunk itself.
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  m_sUsedMemoryPoolSize += sBestMemBlockSize ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  m_sFreeMemoryPoolSize -= sBestMemBlockSize ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  m_uiObjectCount++ ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  SetMemoryChunkValues(ptrChunk, sBestMemBlockSize) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  // eventually, return the Pointer to the User
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  return ((void *) ptrChunk->Data) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)



当用户从内存池中申请内存是，它将从链表搜索一个能够持有被申请大小的chunk。那意味着：

l 那个chunk的`DataSize`必须大于或等于被申请的内存的大小； 

l 那个chunk的`UsedSize `必须是0。

 

这由 `FindChunkSuitableToHoldMemory`  方法完成。如果它返回`NULL`，那么在内存池中没有可用的内存。这将导致`AllocateMemory `的调用(上面讨论过)，它将从OS申请更多的内存。如果返回值不是`NULL``，`一个可用的chunk被发现。`SetMemoryChunkValues`会调整chunk成员的值，并且最后`Data`指针被返回给用户...





 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)/******************
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    SetMemoryChunkValues
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)    ******************/
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)void CMemoryPool::SetMemoryChunkValues(SMemoryChunk *ptrChunk, 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)     const std::size_t &sMemBlockSize)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  if(ptrChunk) 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)  {
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    ptrChunk->UsedSize = sMemBlockSize ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)  }
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)  ...
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)    } 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)



**示例**

假设，用户从内存池申请250字节：

 

![Memory in use](https://p-blog.csdn.net/images/p_blog_csdn_net/060/MemoryPool_Step6.png)

 

*Memory in use*

如我们所见，每个内存块(chunk)管理100字节，所以在这里250字节不是很合适。发生了什么事？Well，`GetMemory` 从第一个chunk返回 `Data`指针并把它的`UsedSize`设为300字节，因为300字节是能够被管理的内存的最小值并大于等于250。那些剩下的**(300 - 250 = 50)**字节被称为内存池的"*memory overhead*"。这没有看起来的那么坏，因为这些内存还可以使用(它仍然在内存池里)。

当`FindChunkSuitableToHoldMemory`搜索可用chunk时，它仅仅从一个空的chunk跳到另一个空的chunk。那意味着，如果某个人申请另一块内存(memory-chunk)，第四块(持有300字节的那个)会成为下一个可用的("valid") chunk。

 

![Jump to next valid chunk](https://p-blog.csdn.net/images/p_blog_csdn_net/060/MemoryPool_Step7.png)

*Jump to next valid chunk*

**使用代码**

使用这些代码是简单的、直截了当的：只需要在你的应用里包含"*CMemoryPool.h*"，并添加几个相关的文件到你的IDE/Makefile:

- *CMemoryPool.h* 
- *CMemoryPool.cpp* 
- *IMemoryBlock.h* 
- *SMemoryChunk.h* 

你只要创建一个`CmemoryPool`类的实例，你就可以从它里面申请内存。所有的内存池的配置在`CmemoryPool`类的构造函数(使用可选的参数)里完成。看一看头文件("*CMemoryPool.h*")或Doxygen-doku。所有的文件都有详细的(Doxygen-)文档。

**应用举例**





 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)MemPool::CMemoryPool *g_ptrMemPool = new MemPool::CMemoryPool() ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)char *ptrCharArray = (char *) g_ptrMemPool->GetMemory(100) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)...
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)g_ptrMemPool->FreeMemory(ptrCharArray, 100) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)delete g_ptrMemPool ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)

**好处**

**内存转储(Memory dump)**

你可以在任何时候通过`WriteMemoryDumpToFile(*strFileName*)`写一个"memory dump"到你的HDD。看看一个简单的测试类的构造函数(使用内存池重载了new和delete运算符)：





![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)/******************
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)Constructor
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)******************/
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)MyTestClass::MyTestClass()
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   m_cMyArray[0] = 'H' ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   m_cMyArray[1] = 'e' ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   m_cMyArray[2] = 'l' ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   m_cMyArray[3] = 'l' ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   m_cMyArray[4] = 'o' ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   m_cMyArray[5] = NULL ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   m_strMyString = "This is a small Test-String" ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   m_iMyInt = 12345 ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   m_fFloatValue = 23456.7890f ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   m_fDoubleValue = 6789.012345 ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   Next = this ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)







 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)MyTestClass *ptrTestClass = new MyTestClass ; 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)g_ptrMemPool->WriteMemoryDumpToFile("MemoryDump.bin") ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)



看一看内存转储文件("*MemoryDump.bin*"):

![img](https://p-blog.csdn.net/images/p_blog_csdn_net/060/1.jpg)

如你所见，在内存转储里有`MyTestClass`类的所有成员的值。明显的，"Hello"字符串(`m_cMyArray`)在那里，以及整型数`m_iMyInt` (3930 0000 = 0x3039 = 12345 decimal)等等。这对调式很有用。

**速度测试**

我在Windows平台上做了几个非常简单的测试(通过`timeGetTime()`)，但是结果说明内存池大大提高了应用程序的速度。所有的测试在*Microsoft Visual Studio .NET 2003*的**debug**模式下(测试计算机: *Intel Pentium IV Processor (32 bit), 1GB RAM, MS Windows XP Professional*).





 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//Array-test (Memory Pool): 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)for(unsigned int j = 0; j < TestCount; j++)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        // ArraySize = 1000
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    char *ptrArray = (char *) g_ptrMemPool->GetMemory(ArraySize)  ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    g_ptrMemPool->FreeMemory(ptrArray, ArraySize) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)  
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)    //Array-test (Heap):
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)for(unsigned int j = 0; j < TestCount; j++)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        // ArraySize = 1000
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    char *ptrArray = (char *) malloc(ArraySize)  ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    free(ptrArray) ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)   }
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)







![img](https://p-blog.csdn.net/images/p_blog_csdn_net/060/MemoryPool_SpeedTestArray.png)

*Results for the "array-test*

 

​    //Class-Test for MemoryPool and Heap (overloaded new/delete) 

 

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif) //Class-Test for MemoryPool and Heap (overloaded new/delete) 
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)for(unsigned int j = 0; j < TestCount; j++)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    MyTestClass *ptrTestClass = new MyTestClass ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    delete ptrTestClass ;
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)







 

![img](https://p-blog.csdn.net/images/p_blog_csdn_net/060/MemoryPool_SpeedTestClasses.png)

*Results for the "classes-test" (overloaded* *new/delete operators)*

**关于代码**

这些代码在Windows和Linux平台的下列编译器测试通过：

- Microsoft Visual C++ 6.0 
- Microsoft Visual C++ .NET 2003 
- MinGW (GCC) 3.4.4 (Windows) 
- GCC 4.0.X (Debian GNU Linux) 

*Microsoft Visual C++ 6.0*(**.dsw*, **.dsp*)和*Microsoft Visual C++ .NET 2003* (**.sln*, **.vcproj*)的工程文件已经包含在下载中。内存池仅用于ANSI/ISO C++,所以它应当在任何OS上的标准的C++编译器编译。在64位处理器上应当没有问题。

**注意**：内存池不是线程安全的。

**ToDo**

这个内存池还有许多改进的地方;-) ToDo列表包括：

l 对于大量的内存，memory-"overhead"能够足够大。

l 某些`CalculateNeededChunks`调用能够通过从新设计某些方法而去掉

l 更多的稳定性测试(特别是对于那些长期运行的应用程序)

l 做到线程安全。

 

**历史**

l 05.09.2006: Initial release

 

*EoF*

DanDanger2000

 

 

 