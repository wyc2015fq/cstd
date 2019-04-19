# WinCE6.0的内存地址重建 - xqhrs232的专栏 - CSDN博客
2014年12月02日 14:40:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：682
原文地址::[http://blog.sina.com.cn/s/blog_537bca2a0100cn4m.html](http://blog.sina.com.cn/s/blog_537bca2a0100cn4m.html)
相关文章
1、[CE6 驱动: 你不得不知道的事情](http://blog.csdn.net/armeasy/article/details/4965710)----[http://blog.csdn.net/armeasy/article/details/4965710](http://blog.csdn.net/armeasy/article/details/4965710)
2、[探索Windows CE 6驱动程序新特性](http://bbs.21ic.com/icview-139132-1-1.html) ----[http://bbs.21ic.com/icview-139132-1-1.html](http://bbs.21ic.com/icview-139132-1-1.html)
3、WINCE6.0学习笔记(序) 
 ----[http://hgh123.blog.163.com/blog/static/598042212008628381314/](http://hgh123.blog.163.com/blog/static/598042212008628381314/)
4、CeOpenCallerBuffer----[http://msdn.microsoft.com/zh-cn/library/bb202723.aspx](http://msdn.microsoft.com/zh-cn/library/bb202723.aspx)
本文描述CE6和之前的版本，在内存访问和传递的差异，阐述CE6与之前版本的不一致之处。BSP和驱动开发人员需要了解OS之间的变化。
首先给出client到server的指针传递的定义：
1、指针参数Pointer parameter：把指针作为函数参数传递
2、内嵌指针Embedded pointer：把指针存储在buffer中传递
3、访问权限检查Access Checking：检查调用进程有无访问buffer的权限
4、安全复制Secure-copy：备份buffer，防止调用者异步修改buffer的内容
5、同步访问Synchronous：当函数调用时, 在被调用线程中访问内存
**指针参数**：是把指针作为参数传递。例如：
ReadFile (hFile, pBuffer, dwBufferSize, ...);
pBuffer是函数ReadFile的参数，而且它是个指针。
**内嵌指针**：把指针被包含在一个指针参数里，传递给API。或者嵌套在其他内嵌指针中。例如
struct MyStruct  {
  BYTE *pEmbedded;
  DWORD dwSize;
};
DeviceIoControl (hFile, pMyStruct, sizeof(MyStruct), ...);
参数pMyStruct是一个指针参数，而pEmbedded包含在MyStruct结构体内，那么pEmbedded就作为一个内嵌指针传递给DeviceIoControl。
指针也可以通过其他方式传递，如存：存放在一片共享内存中；或者使用SetEventData函数，通过事件响应来获取。
**访问权限检查**：是检验函数的调用者，是否有足够权限去访问一块函数间传递的buffer。（访问权限检查不仅仅局限在内存，在此只定义权限检查为内存的权限检查）。驱动有许多权限，能够访问许多系统的数据，而应用程序就没有这些权限。访问权限检查目的，就是阻止恶意的程序通过驱动来达到某些不良动作。如果恶意程序能通过驱动读写系统内存，那么恶意程序就能访问到原来不能访问的数据。适当的访问权限检查，可以有效保护系统内存。
CE5中：
1、驱动使用**MapCallerPtr**()去检查指针参数和内嵌指针的权限。CE5内核检查指针参数是多余的，因为内核无法得知传递buffer的大小。所以内核只能检查buffer的1个byte。
2、访问权限检查可以通过调用进程的trust level，来开放或关闭权限。
CE6中：
1、  函数调用方式有所改变，参数已经可以包括指针参数的大小。所以内核现在能对指针参数，提供一个全面的访问权限检查。（后面会详细描述这些CE6的细节）
2、  驱动只需要检查内嵌指针，检查函数改为新的函数：**CeOpenCallerBuffer**().。此函数同时支持内存地址重建（marshalling）
3、  访问权限检查可以通过调用进程是内核态还是用户态，来开放或关闭权限。（将来可能修改为，通过权限等级来开关权限）
**同步访问**：当函数被调用时，在调用线程中完成访问内存的动作。
**异步访问**：如果一个驱动有线程，当函数调用完毕返回后，访问其他进程的内存，那就是“异步访问”。但需要注意的是，如果驱动有一个线程是在函数调用期间，访问其他进程的内存，在完成返回前，那也是“异步访问”。
指针映射和内存地址重建，是为了驱动能够访问调用者的buffer，而做的指针准备工作。驱动能在应用程序调用它们时，在其他进程空间中被调用运行。因为每个进程的虚拟地址空间，默认是与其他进程互斥、有冲突的。所以驱动必须在访问其他进程的空间前，做好准备工作。
CE5中，所有的进程共享一段公共的地址空间。为了能访问指针所指向的内存，驱动需要映射指针到自己所在的进程空间里。映射是对指针指向值的简单转换，只是让指针在公共地址空间中，指向其他进程slot。下面图片，为device.exe访问调用它的应用进程地址空间。
![Windows <wbr>CE的内存地址重建](http://s8.sinaimg.cn/bmiddle/537bca2ah708b160647f7)
CE6中，每个进程都有自己唯一的地址空间，内存地址重建不能是简单的转换了。每块内存必须从一个进程复制到另一个进程中。或者在驱动进程中申请一个新的虚拟地址，并且映射为调用者正在使用的、原来的那块物理内存。另外，在驱动进程申请的资源，在驱动工作完毕后，必须释放掉。下图为，内核和udevice创建的内存地址重建的示意图。
![Windows <wbr>CE的内存地址重建](http://s8.sinaimg.cn/bmiddle/537bca2ah656ed62af327)![Windows <wbr>CE的内存地址重建](http://s16.sinaimg.cn/bmiddle/537bca2ah656ed88c8a7f)
CE6的内存地址重建有许多方法的，包括buffer是**in-only, in/out** 或 **out-only。基于这些设置，**内存地址重建**能确保适合时候的做读取的动作。这也能作为**访问权限检查的功能，例如：用户态的应用程序不能传递共享的堆地址（对于应用是只读的），作为in/out 或out-only的参数。
为了解释驱动为何一定需要重建内存地址，分别从同步和异步访问的角度说明。
首先是同步：
1、  内核自动映射或者重组指针参数
2、  驱动必须注意内嵌指针。CE5中，驱动使用**MapCallerPtr**()来做检查，而CE6中改为**CeOpenCallerBuffer**()和**CeFreeCallerBuffer**()。
无论是**MapCallerPtr还是CeOpenCallerBuffer函数，在使用buffer前都是一个很好的检查手段。**
**异步访问比较复杂：**
**CE5中，线程要访问调用线程的内存时，有些必须做些额外的工作。每个进程slot会防止其他进程访问它们，而每个线程有自己的权限去访问不同的进程。当调用线程切入到驱动后，带入了它自己本身的访问权限，是属于它所在的进程slot。因此访问调用线程内存的权限，在整个调用过程中都是有效的（这个比较特别）。而不同线程要访问其他进程slot，就必须先获得权限。**
**CE6和CE5类似，都需要做额外工作，但是做这些工作的原因就不一样了，也不容易解释。内核态和用户态的内存地址重建是不一样的，指针参数和内嵌参数也是不一样的。要保证驱动代码在所有模式下正常工作，就需要异步访问前，在其他线程中做好准备工作。**
对于异步访问，指针变量和内嵌指针处理的方式是一样的。如果我们带入一个buffer是已经为同步访问映射或者重组过的，那么驱动访问这个buffer需要的工作是：
1、  CE5：驱动需要在异步线程中，调用**SetProcPermissions**()函数。以便在不同进程中访问同一个buffer。
2、  CE6：驱动需要调用**CeAllocAsynchronousBuffer**()，来申请一个可异步访问的buffer，给同步访问。这个动作必须在传递buffer给异步线程前调用。当线程结束对buffer的访问，调用**CeFreeAsynchronousBuffer**()来释放刚才申请的资源。
不幸的是，不是所有异步情况可以支持用户态驱动，用户态驱动不能异步回写指针参数。而内核态驱动、只读指针（不会回写到caller的）和内嵌指针就可以。简单来说，用户态驱动不支持异步访问。如果都按这个规则办，以后出现问题会少点。假如驱动一定需要异步访问caller buffer的话，在CE6中最好放在内核态中运行。（或者重新修改协议的框架，让caller内存的访问从不异步。又或者通知caller知道数据已经准备好，在驱动中通过回调方式获取数据）
驱动产品化的其他细节：
安全的从其他进程接受内存数据，安全复制和异常处理是必要的。
许多开发人员不太了解的一个安全风险问题：caller有一个buffer，而这个buffer被传递给驱动。当驱动还在使用此buffer时，caller另一个线程可能有会同时使用这块buffer。恶意程序可以控制内嵌指针来获取内存的访问权限，或导致内存溢出，或者导致内存越界和异常。为防止这些情况，驱动必须把caller的buffer做备份，称为安全复制（secure copy），防止caller异步的修改。
第一个例子：caller传递内嵌指针给驱动，可以通过安全复制来防止攻击。驱动使用MapCallerPtr (CE5) 或CeOpenCallerBuffer (CE6)来检查指针的权限，同时映射和重组内存地址。如果驱动继续保存指针到caller buffer，caller会稍后的把指针指向其他内存，那么驱动可能会访问到错误的地址。驱动必须备份从caller传递下来的指针，防止异步的修改。同样，驱动必须备份从caller传递下来的buffer大小的内容。
因此，当需要调用MapCallerPtr 或CeOpenCallerBuffer。复制内嵌指针到本地空间，是一个明智的手段。不要保存映射、重组的指针到caller的buffer。不要未映射、重组前，使用caller的指针。谨慎处理buffer的大小长度，那么caller就不能通过指针越界访问了。
第二个安全复制例子是：文件名字。CreateFile函数会带入能有效访问的文件名。假如CreateFile先读取文件名、权限检查，通过检查后才使用这个文件名打开文件。CreateFile先传入了一个检查后的文件名，而异步修改为一个不能访问的文件名，那么会有一段时间内caller让CreateFile进入打开非法文件。这可能只有很小的机会存在问题，但是黑客程序会利用这个缺点直到成功。这会影响整个系统的安全性。对这类攻击的保护方法是，在打开文件前，CreateFile一定要对文件名在caller不能访问的地方做备份。（CE6已经对CreateFile的文件名做了安全备份，这只是举例说明一下）
任何数据得到确认后，都需要做备份，防止在数据确认后有异步修改。安全备份可以简单复制buffer或指针到有效的栈，或者从一个临时的堆中申请地址空间。函数CeOpenCallerBuffer有一个强行备份参数，可以通过此参数做安全备份。可以选择使用CeAllocDuplicateBuffer函数（这基于堆的分配，必要时候使用memcpy作为复制函数）。不管是否使用安全备份，你都需要保护从caller传递进来的数据。
驱动使用异常处理，来作为简单的数据安全备份。重点注意的是： caller可能会传递一个指向无效的内存地址。因为应用程序可以传递一个指针指向一个没有分配的用户态地址，或者异步的释放内存。所以驱动必须经常使用try/except的语句来解决异常。还有，函数返回前，确认调用的申请内存被释放，离开所有的临界区。
进程间传递数据是比较复杂的，但是在驱动中，有些简单的规则可以依循：
|**情景**|**CE6驱动必须的动作**||
|----|----|----|
|**同步使用参数**|如果需要安全备份，自己备份或者使用CeAllocDuplicateBuffer||
|**异步使用参数**|如果需要安全备份，自己备份或者使用 CeAllocDuplicateBuffer / CeFreeDuplicateBuffer.否则使用CeAllocAsynchronousBuffer / CeFreeAsynchronousBuffer.||
|**同步使用内嵌指针**|必须使用CeOpenCallerBuffer / CeCloseCallerBuffer来地址重建和安全备份||
|| | |
|| | |
|**异步使用内嵌指针**|使用 CeOpenCallerBuffer 然后使用CeAllocAsynchronousBuffer.  必须在CeCloseCallerBuffer前调用 CeFreeAsynchronousBuffer。||
**记住，经常使用try/except。**
CE6有些类可以帮助简化以上的操作，代码在public\common\oak\inc\marshal.hpp中。
- **MarshalledBuffer_t**: 封装了CeOpenCallerBuffer、CeAllocAsynchronousBuffer和它们的释放函数。在所有内嵌指针中使用这个类。
- **DuplicatedBuffer_t**: 封装了 CeAllocDuplicateBuffer 和释放函数. 需要安全备份时指针参数可以使用这个类。
- **AsynchronousBuffer_t**: 封装了CeAllocAsynchronousBuffer和释放函数。需要异步访问指针参数可以使用这个类
C++的版本为：
|**情景**|**CE6驱动必须的动作**|
|----|----|
|**同步使用参数**|如果需要安全备份，使用DuplicatedBuffer_t|
|**异步使用参数**|如果需要安全备份，使用DuplicatedBuffer_t否则使用 AsynchronousBuffer_t|
|**内嵌指针**|使用 MarshalledBuffer_t|
**[http://blogs.msdn.com/ce_base/archive/2006/11/09/Memory-marshalling-in-Windows-CE.aspx](http://blogs.msdn.com/ce_base/archive/2006/11/09/Memory-marshalling-in-Windows-CE.aspx)**
