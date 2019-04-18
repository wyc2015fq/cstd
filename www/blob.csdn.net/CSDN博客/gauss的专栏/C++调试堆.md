# C++调试堆 - gauss的专栏 - CSDN博客
2013年01月04日 00:23:25[gauss](https://me.csdn.net/mathlmx)阅读数：261
- 内存管理和调试堆 
描述堆函数的“Debug”版本。这些函数解决两个最难处理的内存分配问题：改写已分配缓冲区的结尾和内存泄漏（当不再需要分配后未能释放它们）。 - 调试堆中的块类型 
描述在调试堆中内存块所分配到的五种分配类型。出于泄漏检测和状态报告的目的，以不同方式对这些分配类型进行跟踪和报告。 - 调试堆 
提供有关使用调试堆的信息。信息包括：哪些调用用于“Debug”版本，释放内存块时将发生什么，哪些调试功能必须从代码内部进行访问，更改 **_crtDbgFlag** 位域以创建标志的新状态的步骤，以及一个阐释如何打开自动泄漏检测和如何关闭**_CRT_BLOCK** 类型块的检查的代码示例。- C++ 中的调试堆 
讨论 C++ **new** 和 **delete** 运算符的“Debug”版本和使用 **_CRTDBG_MAP_ALLOC** 的效果。 - 堆状态报告函数 
描述 **_CrtMemState** 结构，可以使用它来捕捉堆状态的摘要快照。本主题还列出一些 CRT 函数，这些函数报告堆的状态和内容并使用这些信息来帮助检测内存泄漏和其他问题。- 跟踪堆分配请求 
包含用于标识出错的特定堆分配调用的方法。 
#### 相关章节
- CRT 调试技术 
链接到用于 C 运行时库的调试技术，包括：使用 CRT 调试库、用于报告的宏、**malloc** 和 **_malloc_dbg** 之间的差异、编写调试挂钩函数以及 CRT 调试堆(参考msnd)。
# 内存管理和调试堆
程序员遇到的两种最常见而又难处理的问题是，改写已分配缓冲区的末尾以及内存泄漏（未能在不再需要某些分配后将其释放）。调试堆提供功能强大的工具来解决这类内存分配问题。
## 堆函数的“Debug”版本
堆函数的“Debug”版本调用“Release”版本中使用的标准版本或基版本。当请求内存块时，调试堆管理器从基堆分配略大于所请求的块的内存块，并返回指向该块中属于您的部分的指针。例如，假定应用程序包含调用：`malloc( 10 )` 。在“Release”版本中，malloc 将调用基堆分配例程以请求分配 10 个字节。但在“Debug”版本中，**malloc** 将调用 _malloc_dbg，该函数接着调用基堆分配例程以请求分配
 10 个字节加上大约 36 个字节的额外内存。调试堆中产生的所有内存块在单个链接列表中连接起来，按照分配时间排序。
调试堆例程分配的附加内存的用途为：存储簿记信息，存储将调试内存块链接在一起的指针，以及形成数据两侧的小缓冲区（用于捕捉已分配区域的改写）。
当前，用于存储调试堆的簿记信息的块头结构在 DBGINT.H 头文件中声明如下：
typedef struct _CrtMemBlockHeader
{
// Pointer to the block allocated just before this one:
   struct _CrtMemBlockHeader *pBlockHeaderNext;
// Pointer to the block allocated just after this one:
   struct _CrtMemBlockHeader *pBlockHeaderPrev;
   char *szFileName;    // File name
   int nLine;           // Line number
   size_t nDataSize;    // Size of user block
   int nBlockUse;       // Type of block
   long lRequest;       // Allocation number
// Buffer just before (lower than) the user's memory:
   unsigned char gap[nNoMansLandSize];
} _CrtMemBlockHeader;
/* In an actual memory block in the debug heap,
 * this structure is followed by:
 *   unsigned char data[nDataSize];
 *   unsigned char anotherGap[nNoMansLandSize];
 */
该块的用户数据区域两侧的 `NoMansLand `缓冲区当前大小为 4 个字节，并用调试堆例程所使用的已知字节值填充，以验证尚未改写用户内存块限制。调试堆还用已知值填充新的内存块。如果选择在堆的链接列表中保留已释放块（如下文所述），则这些已释放块也用已知值填充。当前，所用的实际字节值如下所示：
- NoMansLand (0xFD)(deFencde Data) 
应用程序所用内存两侧的“NoMansLand”缓冲区当前用 0xFD 填充。 - 已释放块 (0xDD)(Dead Data) 
设置 **_CRTDBG_DELAY_FREE_MEM_DF** 标志后，调试堆的链接列表中保留未使用的已释放块当前用 0xDD 填充。- 新对象 (0xCD) (Cleared Data) 
分配新对象时，这些对象用 0xCD 填充。 
# 调试堆中的块类型
调试堆中的每个内存块都分配以五种分配类型之一。出于泄漏检测和状态报告目的对这些类型进行不同地跟踪和报告。可以指定块的类型，方法是使用对其中一个调试堆分配函数（如 _malloc_dbg）的直接调用来分配块。调试堆中的五种内存块类型（在**_CrtMemBlockHeader** 结构的**nBlockUse** 成员中设置）如下所示：
- **_NORMAL_BLOCK**
对 malloc 或 calloc 的调用将创建“普通”块。如果打算只使用“普通”块而不需要“客户端”块，则可能想要定义 _CRTDBG_MAP_ALLOC，它导致所有堆分配调用映射到它们在“Debug”版本中的调试等效项。这将允许将关于每个分配调用的文件名和行号信息存储到对应的块头中。- **_CRT_BLOCK**
由许多运行时库函数内部分配的内存块被标记为 CRT 块，以便可以单独处理这些块。结果，泄漏检测和其他操作不需要受这些块影响。分配永不可以分配、重新分配或释放任何 CRT 类型的块。- **_CLIENT_BLOCK**
出于调试目的，应用程序可以专门跟踪一组给定的分配，方法是使用对调试堆函数的显式调用将它们作为该类型的内存块进行分配。例如，MFC 以“客户端”块类型分配所有的**CObjects** ；其他应用程序则可能在“客户端”块中保留不同的内存对象。还可以指定“客户端”块的子类型以获得更大的跟踪粒度。若要指定“客户端”块子类型，请将该数字向左移 16 位，并将它与**_CLIENT_BLOCK** 进行**OR** 运算。例如： 
```
#define MYSUBTYPE 4
*free*
dbg(pbData, _CLIENT_BLOCK|(MYSUBTYPE<<16));
```
客户端提供的挂钩函数（用于转储在“客户端”块中存储的对象）可以使用 _CrtSetDumpClient 进行安装，然后，每当调试函数转储“客户端”块时均会调用该挂钩函数。同样，对于调试堆中的每个“客户端”块，可以使用 _CrtDoForAllClientObjects 来调用应用程序提供的给定函数。
- **_FREE_BLOCK**
通常，所释放的块将从列表中移除。为了检查并未仍在向已释放的内存写入数据，或为了模拟内存不足情况，可以选择在链接列表上保留已释放块，将其标记为“可用”，并用已知字节值（当前为 0xDD）填充。- **_IGNORE_BLOCK**
有可能在一段时间内关闭调试堆操作。在该时间段内，内存块保留在列表上，但被标记为“忽略”块。 
若要确定给定块的类型和子类型，请使用 _CrtReportBlockType 函数以及 **_BLOCK_TYPE** 和 **_BLOCK_SUBTYPE **宏。宏的定义（在 crtdbg.h 中）如下所示：
#define _BLOCK_TYPE(block)          (block & 0xFFFF)
#define _BLOCK_SUBTYPE(block)       (block >> 16 & 0xFFFF)
# 调试堆
对堆函数（如 **malloc** 、**free** 、**calloc** 、**realloc** 、**new**和**delete** ）的所有调用均解析为这些函数在调试堆中运行的“Debug”版本。当释放内存块时，调试堆自动检查已分配区域两侧的缓冲区的完整性，如果发生改写，将发出错误报告。
**使用调试堆**
- 用 C 运行时库的“Debug”版本链接应用程序的调试版本。 
## 从代码内部访问的调试堆功能
- **_CrtCheckMemory**
许多调试堆功能必须从代码内访问。例如，可以使用对 _CrtCheckMemory 的调用来检查堆在任意点的完整性。该函数检查堆中的每个内存块，验证内存块头信息有效，并确认尚未修改缓冲区。- **_CrtSetDbgFlag**
可以使用内部标志 _crtDbgFlag 来控制调试堆跟踪分配的方式，该标志可使用 _CrtSetDbgFlag 函数进行读取和设置。通过更改该标志，可以指示调试堆在程序退出时检查内存泄漏，并报告检测到的所有泄漏。类似地，可以指定不将已释放的内存块从链接列表移除，以模拟内存不足情况。当检查堆时，将完全检查这些已释放的块，以确保它们未受打扰。
**_crtDbgFlag** 标志包含下列位域： 
|位域|默认值|说明|
|----|----|----|
|**_CRTDBG_ALLOC_MEM_DF**|On|打开调试分配。当该位为 off 时，分配仍链接在一起，但它们的块类型为 **_IGNORE_BLOCK** 。|
|**_CRTDBG_DELAY_FREE_MEM_DF**|Off|防止实际释放内存，与模拟内存不足情况相同。当该位为 on 时，已释放块保留在调试堆的链接列表中，但标记为 **_FREE_BLOCK** ，并用特殊字节值填充。|
|**_CRTDBG_CHECK_ALWAYS_DF**|Off|导致每次分配和释放时均调用 **_CrtCheckMemory** 。这将减慢执行，但可快速捕捉错误。|
|**_CRTDBG_CHECK_CRT_DF**|Off|导致将标记为 **_CRT_BLOCK** 类型的块包括在泄漏检测和状态差异操作中。当该位为 off 时，在这些操作期间将忽略由运行时库内部使用的内存。|
|**_CRTDBG_LEAK_CHECK_DF**|Off|导致在程序退出时通过调用 _CrtDumpMemoryLeaks 来执行泄漏检查。如果应用程序未能释放其所分配的所有内存，将生成错误报告。|
**更改一个或多个 _crtDbgFlag 位域并创建标志的新状态**
- 在 *newFlag* 参数设置为 **_CRTDBG_REPORT_FLAG** 的情况下调用 **_CrtSetDbgFlag** （以获得当前的 **_crtDbgFlag** 状态），并在一个临时变量中存储返回值。 
- 打开任何位，对临时变量与相应位屏蔽（在应用程序代码中由清单常数表示）进行 **OR** 运算（按位 | 符号）。 
- 关闭其他位，对该变量与相应位屏蔽的 **NOT** （按位 ~ 符号）进行 **AND** 运算（按位 & 符号）。
- 在 *newFlag* 参数设置为临时变量中存储的值的情况下调用 **_CrtSetDbgFlag** ，以创建 **_crtDbgFlag** 的新状态。 
例如，下列代码行打开自动泄漏检测，关闭检查 **_CRT_BLOCK** 类型的块：
// Get current flag
int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
// Turn on leak-checking bit
tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
// Turn off CRT block checking bit
tmpFlag &= ~_CRTDBG_CHECK_CRT_DF;
// Set flag to the new value
_CrtSetDbgFlag( tmpFlag );
# C++ 中的调试堆
C 运行时库的“Debug”版本包含 C++ 的 **new **和 **delete** 运算符的“Debug”版本。如果 C++ 代码定义了 _CRTDBG_MAP_ALLOC，则**new** 的所有实例都映射到“Debug”版本，该版本将记录源文件和行号信息。
如果希望使用 **_CLIENT_BLOCK** 分配类型，请不要定义 **_CRTDBG_MAP_ALLOC** 。而必须直接调用**new** 运算符的“Debug”版本，或创建替换调试模式中的**new** 运算符的宏，如下面的示例所示：
/* MyDbgNew.h
 Defines global operator new to allocate from
 client blocks
*/
#ifdef _DEBUG
   #define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
   #define DEBUG_CLIENTBLOCK
#endif // _DEBUG
/* MyApp.cpp
   Compile options needed: /Zi /D_DEBUG /MLd
 *            or use a
 *      Default Workspace for a Console Application to
 *      build a Debug version
*/
#include "crtdbg.h"
#include "mydbgnew.h"
#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif
int main( )   {
   char *p1;
   p1 =  new char[40];
   _CrtMemDumpAllObjectsSince( NULL );
 }
**delete **运算符的“Debug”版本可用于所有块类型，并且编译“Release”版本时程序中不需要任何更改。
# 堆状态报告函数
有几个函数可报告给定时刻调试堆的内容。
## _CrtMemState
若要捕获给定时刻堆状态的摘要快照，请使用 CRTDBG.H 中定义的 **_CrtMemState** 结构：
typedef struct _CrtMemState
{
// Pointer to the most recently allocated block:
   struct _CrtMemBlockHeader * pBlockHeader;
// A counter for each of the 5 types of block:
   size_t lCounts[_MAX_BLOCKS];
// Total bytes allocated in each block type:
   size_t lSizes[_MAX_BLOCKS];
// The most bytes allocated at a time up to now:
   size_t lHighWaterCount;
// The total bytes allocated at present:
   size_t lTotalCount;
} _CrtMemState;
该结构保存指向调试堆的链接列表中的第一个（最近分配的）块的指针。然后，它在两个数组中记录列表中每种类型的内存块（**_NORMAL_BLOCK** 、**_CLIENT_BLOCK** 、**_FREE_BLOCK** 等等）的个数，以及每种类型的块中分配的字节数。最后，它记录到该点为止堆中总共分配的最大字节数以及当前分配的字节数。
## 其他 CRT 报告函数
下列函数报告堆的状态和内容，并使用这些信息帮助检测内存泄漏及其他问题：
|函数|说明|
|----|----|
|_CrtMemCheckpoint|在应用程序提供的 **_CrtMemState** 结构中保存堆的快照。|
|_CrtMemDifference|比较两个内存状态结构，在第三个状态结构中保存二者之间的差异，如果两个状态不同，则返回 **TRUE** 。|
|_CrtMemDumpStatistics|转储给定的 **_CrtMemState** 结构。该结构可能包含给定时刻调试堆状态的快照或两个快照之间的差异。|
|_CrtMemDumpAllObjectsSince|转储自对堆拍了给定快照以来或从执行开始以来所分配的所有对象的信息。如果已经使用 **_CrtSetDumpClient** 安装了挂钩函数，那么，**_CrtMemDumpAllObjectsSince** 每次转储**_CLIENT_BLOCK** 块时，都会调用应用程序所提供的挂钩函数。|
|_CrtDumpMemoryLeaks|确定自程序开始执行以来是否发生过内存泄漏，如果发生过，则转储所有已分配对象。如果已使用 **_CrtSetDumpClient** 安装了挂钩函数，那么，**_CrtDumpMemoryLeaks**每次转储**_CLIENT_BLOCK** 块时，都会调用应用程序所提供的挂钩函数。|
# 跟踪堆分配请求
尽管查明在其中执行断言或报告宏的源文件名和行号对于定位问题原因常常很有用，对于堆分配函数却可能不是这样。虽然可在应用程序的逻辑树中的许多适当点插入宏，但分配经常隐藏在特殊例程中，该例程会在很多不同时刻从很多不同位置进行调用。问题通常并不在于如何确定哪行代码进行了错误分配，而在于如何确定该行代码进行的上千次分配中的哪一次是错误分配以及原因。
## 唯一分配请求编号和 _crtBreakAlloc
标识发生错误的特定堆分配调用的最简单方法是利用与调试堆中的每个块关联的唯一分配请求编号。当其中一个转储函数报告某块的有关信息时，该分配请求编号将括在大括号中（例如“{36}”）。
知道某个错误分配块的分配请求编号后，可以将该编号传递给 _CrtSetBreakAlloc 以创建一个断点。执行将恰在分配该块以前中断，您可以向回追踪以确定哪个例程执行了错误调用。为避免重新编译，可以在调试器中完成同样的操作，方法是将**_crtBreakAlloc** 设置为所感兴趣的分配请求编号。
## 创建分配例程的“Debug”版本
略微复杂的方法是创建您自己的分配例程的“Debug”版本，等同于[堆分配函数](http://blog.csdn.net/ugg/archive/2007/04/26/_core_using_the_debug_version_versus_the_base_version.htm) 的**_dbg** 版本。然后，可以将源文件和行号参数传递给基础堆分配例程，并能立即看到错误分配的出处。
例如，假定您的应用程序包含与下面类似的常用例程：
int addNewRecord(struct RecStruct * prevRecord,
                 int recType, int recAccess)
{
   // ...code omitted through actual allocation... 
   if ((newRec = malloc(recSize)) == NULL)
   // ... rest of routine omitted too ... 
}
在头文件中，可以添加如下代码：
#ifdef _DEBUG
#define  addNewRecord(p, t, a) /
         addNewRecord(p, t, a, __FILE__, __LINE__)
#endif
接下来，可以如下更改记录创建例程中的分配：
int addNewRecord(struct RecStruct *prevRecord,
                  int recType, int recAccess
#ifdef _DEBUG
               , const char *srcFile, int srcLine
#endif
   )
{
   /* ... code omitted through actual allocation ... */
   if ((newRec = _malloc_dbg(recSize, _NORMAL_BLOCK,
         srcFile, scrLine)) == NULL)
   /* ... rest of routine omitted too ... */
}
在其中调用 `addNewRecord` 的源文件名和行号将存储在产生的每个块中（这些块是在调试堆中分配的），并将在检查该块时进行报告。
Trackback: http://tb.blog.csdn.net/TrackBack.aspx?PostId=1585639
