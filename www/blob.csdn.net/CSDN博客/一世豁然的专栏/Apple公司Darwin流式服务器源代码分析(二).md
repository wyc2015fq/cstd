# Apple公司Darwin流式服务器源代码分析(二) - 一世豁然的专栏 - CSDN博客





2016年10月25日 19:30:11[一世豁然](https://me.csdn.net/Explorer_day)阅读数：689








本博客转载于：[http://www.cublog.cn/u/11445/showart.php?id=225276](http://www.cublog.cn/u/11445/showart.php?id=225276)







3     基础功能类库（Common Utilities）

3.1   OS类
Darwin Streaming Server支持包括Windows，Linux以及Solaris在内的多种操作系统平台。我们知道，Windows和Unix（或Unix-like）操作系统之间无论从内核还是编程接口上都有着本质的区别，即使是Linux和Solaris，在编程接口上也大为不同。为此，DSS开发了多个用于处理时间、临界区、信号量、事件、互斥量和线程等操作系统相关的类，这些类为上层提供了统一的使用接口，但在内部却需要针对不同的操作系统采用不同的方法实现。表2罗列出了DSS中的主要OS类和数据结构。
表2  DSS中的主要OS类和数据结构
类（数据结构）名主要功能
OS   平台相关的功能类，如内存分配、时间等
OSCond  状态变量的基本功能和操作
OSMutex 互斥量的基本功能和操作
OSThread       线程类
OSFileSource  简单文件类
OSQueue 队列类
OSHashTable  哈希表类
OSHeap   堆类
OSRef     参考引用类
3.1.1       OSMutex/OSCond Class
在有多个线程并发运行的环境中，能同步不同线程的活动是很重要的，DSS开发了OSMutex和OSCond两个类用以封装不同操作系统对线程同步支持的差异。
我们首先分析OSMutex类，这个类定义了广义互斥量的基本操作，类定义如下：
class OSMutex

{

1     public:

2            OSMutex()；         //构造函数
3            ~OSMutex()；              //析构函数

4            inline void Lock()；              //加锁
5            inline void Unlock()；    //解锁
6            inline Bool16 TryLock()；     //异步锁，无论是否成功立即返回

7     private:

8     #ifdef __Win32__

9            CRITICAL_SECTION fMutex；  //临界区
10           DWORD        fHolder；                     //拥有临界区的线程id

11           UInt32           fHolderCount；             //进入临界区线程数
              //其他略…

}
在Windows平台上，OSMutex类是通过临界区（CRITICAL_SECTION）来实现的，第10行定义了临界区变量fMutex。类实例化时构造函数调用InitializeCriticalSection(&fMutex)初始化临界区变量，对应的在析构函数中调用DeleteCriticalSection(&fMutex)清除。
Lock()函数用于对互斥量加锁，它调用私有方法RecursiveLock实现：
void        OSMutex::RecursiveLock()

{

       // 当前线程已经拥有互斥量，只需增加引用计数
1     if (OSThread::GetCurrentThreadID() == fHolder)

2     {

3            fHolderCount++；  //增加引用计数
4            return；
5     }

6     #ifdef __Win32__

7            ::EnterCriticalSection(&fMutex)； //申请进入临界区
8     #else

9            (void)pthread_mutex_lock(&fMutex)；
10    #endif

11    Assert(fHolder == 0)；
12    fHolder = OSThread::GetCurrentThreadID()；    //更新临界区拥有者标志
13    fHolderCount++；

14    Assert(fHolderCount == 1)；
}
第1行检测如果当前线程已经拥有互斥量，就只需将内部计数fHolderCount加1，以便纪录正在使用互斥量的方法数。如果当前线程还没有得到互斥量，第7行调用EnterCriticalSection()函数申请进入临界区；如果当前已经有其他线程进入临界区，该函数就会阻塞，使得当前线程进入睡眠状态，直到占用临界区的线程调用LeaveCriticalSection(&fMutex)离开临界区后才可能被唤醒。一旦线程进入临界区后，它将首先更新临界区持有者标志（第12行），同时将临界区引用计数加1。
注意到另外一个函数TryLock()，该函数也是用于为互斥量加锁，但与Lock()不同的是，TryLock()函数为用户提供了异步调用互斥量的功能，这是因为它调用::TryEnterCriticalSection(&fMutex)函数申请进入缓冲区：如果临界区没有被任何线程拥有，该函数将临界区的访问区给予调用的线程，并返回TRUE，否则它将立刻返回FALSE。TryEnterCriticalSection()和EnterCriticalSection()函数的本质区别在于前者从不挂起线程。
接着分析OSCond类，该类定义了状态变量（Condition Variable）的基本操作，类定义如下：
class OSCond 

{

1     public:

2            OSCond()；   //构造函数
3            ~OSCond()； //析构函数


4            inline void      Signal()；       //传信函数
5            inline void      Wait(OSMutex* inMutex, SInt32 inTimeoutInMilSecs = 0)；

//等待传信函数
6            inline void       Broadcast()； //广播传信函数

7     private:

8     #ifdef __Win32__

9            HANDLE                     fCondition；   //事件句柄
10           UInt32                         fWaitCount；  //等待传信用户数
//其他略…

       }
虽然同是用于线程同步，但OSCond类与OSMutex大不相同，后者用来控制对关键数据的访问，而前者则通过发信号表示某一操作已经完成。在Windows平台中，OSCond是通过事件（event）来实现的；构造函数调用CreateEvent()函数初始化事件句柄fCondition，而析构函数则调用CloseHandle()关闭句柄。
       OSCond的使用流程是这样的：线程调用Wait(OSMutex* inMutex, SInt32 inTimeoutInMilSecs = 0)函数等待某个事件的发生，其中inTimeoutInMilSecs是最长等待时间，0代表无限长。Wait()函数内部调用了WaitForSingleObject
 (fCondition, theTimeout)函数，该函数告诉系统线程在等待由事件句柄fCondition标识的内核对象变为有信号，参数theTimeout告诉系统线程最长愿意等待多少毫秒。如果指定的内核对象在规定时间内没有变为有信号，系统就会唤醒该线程，让它继续执行。而函数Signal()正是用来使事件句柄fCondition有信号的。Signal()函数内部实现很简单，只是简单调用SetEvent函数将事件句柄设置为有信号状态。
使用OSCond的过程中存在一种需求，就是希望通知所有正在等待的用户事件已经完成，而Signal()函数每次只能通知一个用户，因此又开发了另外一个广播传信函数如下：
inline void OSCond::Broadcast()

{     //提示：本函数相当循环调用Signal()函数
1     #ifdef __Win32__

2     UInt32 waitCount = fWaitCount；       //等待传信的用户数
3     for (UInt32 x = 0； x < waitCount； x++)        //循环为每个用户传信
4     {

5            BOOL theErr = ::SetEvent(fCondition)；             //设置事件句柄为有信号状态
6            Assert(theErr == TRUE)；
7     }

//此处略…

       }

       Broadcast首先统计所有等待传信的用户数（第2行），然后用一个循环为每个用户传信（第3~7）行。这种编程方法虽然不是很优雅（elegant），但是由于Windows平台上不支持广播传信功能（Linux和Solaris均支持），也只好如此。
3.1.2       OSThread Class

OSThread是DSS中最重要的类之一，它封装并且定义了使用线程的方式，因此需要重点讨论。OSThread类的定义如下：
class OSThread

{

1     public:

       // 必须在使用其他OSThread函数前调用该初始化函数
2     static void              Initialize()；


3     OSThread()； //构造函数
4     virtual                                 ~OSThread()；      //析构函数


       //子类继承该纯虚函数完成自己的工作
5     virtual     void                     Entry() = 0；


6     void                     Start()； //启动线程
7     void                     Join()；  //等待线程运行完成后删除
8     void                     Detach()；     //使线程处于fDetached状态
9     static void              ThreadYield()；     //Windows平台不用
10    static void              Sleep(UInt32 inMsec)； //让线程睡眠
       …

11    private:

       //标识线程的状态
12    Bool16 fStopRequested:1；
13    Bool16 fRunning:1；
14    Bool16 fCancelThrown:1；
15    Bool16 fDetached:1；
16    Bool16 fJoined:1；
       …

17    static void              CallEntry(OSThread* thread)；//调用子类重载的虚函数
18    #ifdef __Win32__

//使用_beginghreadex创建线程时的标准入口函数
19    static unsigned int WINAPI _Entry(LPVOID inThread)；

20    #else

21    static void*    _Entry(void* inThread)； //unix下的入口函数
22    #endif

}

OSThread封装了线程的基本功能，一个OSThread的实例代表一个线程。用户通过继承OSThread，并且重载其中的纯虚函数Entry（第5行），从而将自己的任务交给该线程运行。OSThread内部运行机制比较复杂，为此我们用图3所示的流程来描述其运行过程。
另外，OSThread对于线程的状态定义了一套完整的控制方法。当用户调用start()函数后，按照上图，最终将调用CallEntry()函数，而该函数在调用Entry()之前将线程设定为运行状态（thread->fRunning
 = true），当Entry()函数运行完后再设为非运行状态；在运行过程中，用户可以通过StopAndWaitForThread()、join()、Detach()以及ThrowStopRequest()等函数改变线程其他状态变量。
3.1.3       OSHashTable/OSQueue/OSHeap/OSRef Class

DSS定义了几个通用的较为复杂的数据结构，它们都以类的方式封装。这些数据结构不但贯穿于DSS的所有源代码，而且由于其封装的十分好，读者可以在看懂源代码的基础上很容易的将它们从DSS的工程中抽取出来，构建自己的基础类库，为将来的开发工作打下良好的基础。另外，对这些基础数据结构源代码的研究将提高我们对于面向对象技术的掌握和领会。
最主要的数据结构有四种：哈希表（OSHashTable）、队列（OSQueue）、堆（OSHeap）和对象引用表（OSRef）。前三种是我们在编程中大量使用的数据结构，而对象引用表则是类似于COM/DCOM组件编程中IUNKOWN接口功能的数据结构，它首先为每个对象建立了一个字符串形式的ID，以便于通过这个ID找到对象（类似于QueryInterface）；另外OSRef类还为每个对象实例建立了引用计数，只有一个对象不再被任何人引用，才可能被释放（类似于AddRef和Release）。
鉴于这几个类在结构上有相似之处，下面我们将分析OSHashTable的源代码，以便能够帮助读者更好的理解其他几个类。OSHashTable的代码如下：
       template

       class OSHashTable {

       /*提示：OSHashTable被设计成为一个类模版，两个输入参数分别为：class T：实际的对象类；class K：用于为class
 T计算哈希表键值的功能类。*/

1     public:

2     OSHashTable( UInt32 size )   //构造函数，入参是哈希表中对象的最大个数
3     {

4            fHashTable = new ( T*[size] )；   //申请分配size个哈希对象class T的空间
5            Assert( fHashTable )；
6            memset( fHashTable, 0, sizeof(T*) * size )；      //初始化
7            fSize = size；
/*下面的代码决定用哪种方式为哈希表的键值计算索引；
如果哈希表的大小不是2的幂，只好采用对fSize求余的方法；
否则可以直接用掩码的方式，这种方式相对速度更快*/

8            fMask = fSize - 1；
9            if ((fMask & fSize) != 0)             //fSize不是2的幂
10                  fMask = 0；
11           fNumEntries = 0； //当前对象数
12    }

13    ~OSHashTable()     //析构函数
14    {

15           delete [] fHashTable；   //释放空间
16    }

//下面介绍向哈希表中添加一个class T对象的源代码
17    void Add( T* entry ) {

18           Assert( entry->fNextHashEntry == NULL )；
                     /*利用功能类class K，计算class T对象的哈希键值，其计算方法由用户在class
 K中定义*/

       19           K key( entry )；

20           UInt32 theIndex = ComputeIndex( key.GetHashKey() )；//利用键值计算索引
21           entry->fNextHashEntry = fHashTable[ theIndex ]； //在新加对象中存储索引值
22           fHashTable[ theIndex ] = entry； //将该对象插入到索引指定的位置
23           fNumEntries++；   /

24    }

//下面介绍从哈希表中删除一个class T对象的源代码
25    void Remove( T* entry )

26    {

//首先从哈希表中找到待删除的对象
//1、计算哈希键值和其对应的对象索引
27           key( entry )；

28           UInt32 theIndex = ComputeIndex( key.GetHashKey() )；

29           T* elem = fHashTable[ theIndex ]；
30           T* last = NULL；
/*2、通过对象索引查找对象，如果不是要找的对象，接着找下一个，直到找到为止。这是因为，存放的时候就是按照这种模式计算索引的。*/

31           while (elem && elem != entry) { 

32                  last = elem；
33                  elem = elem->fNextHashEntry；
34           }

              //找到该对象，将其删除
35           if ( elem )      


36           {

37                  Assert(elem)；
38                  if (last)   


39                         last->fNextHashEntry = elem->fNextHashEntry；
40                  else //elem在头部
41                         fHashTable[ theIndex ] = elem->fNextHashEntry；
42                  elem->fNextHashEntry = NULL；
43                  fNumEntries--；
44           }

45    }

//下面介绍从哈希表中查找一个class T对象的方法
46    T* Map( K* key )  //入参为哈希键值
47    {

48                  UInt32 theIndex = ComputeIndex( key->GetHashKey() )； //计算索引
49                  T* elem = fHashTable[ theIndex ]；     //找到索引对应的对象
50                  while (elem) {

51                         K elemKey( elem )；
52                         if (elemKey =*key) //检查是否找对
53                                break；
54                         elem = elem->fNextHashEntry；   //如果不是，继续找下一个
55                  }

56                  return elem；
57           }

//以下略…

}
以上介绍了哈希表的构造以及三种基本操作：添加、删除和查询。另外，DSS还定义了OSHashTableIter类用于枚举OSHashTable中的class
 T对象；其中主要的操作有First和Next等，限于篇幅，此处就不再详述。






