# Windows CE 6.x驱动程序的新特性 - xqhrs232的专栏 - CSDN博客
2013年11月08日 17:12:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：686
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://space.itpub.net/16910633/viewspace-504206](http://space.itpub.net/16910633/viewspace-504206)
### 好久又没有更新博客了。这次发表一篇古文，当然不是古典文言文，只是已经在我的硬盘上放了很长时间的文章，介绍Windows CE 6的驱动程序跟以前相比的新特性。
### 1            访问检查
访问检查是用来检查函数的调用者是否有足够的权限去访问传递给这个函数的内存。访问检查是很必要的，它可以防止恶意的应用程序利用驱动程序去完成需要特权才能访问的资源。设备驱动程序由于在Windows Embedded CE 6.0中处于内核空间所以是一种特权程序，可以访问很多系统的资源。而工作在用户态的应用程序却不是。如果一个应用程序利用设备驱动程序去读写系统的内存，那么设备驱动程序实际上就相当于是授予了这个应用程序高的访问权限。所以在设备驱动程序中进行访问检查可以保护操作系统的内存不受恶意应用程序的破坏。
在Windows CE 5.0中，设备驱动程序是通过MapCallerPtr API来对指针参数和嵌套的指针参数进行访问检查的。
struct MyStruct { UCHAR *pEmbedded; DWORD dwSize; };
// Windows CE 5.0 and prior versions
// In XXX_IOControl...
MyStuct *p = ( MyStruct*) pInput;
g_pMappedEmbedded = MapCallerPtr(p->pEmbedded);
// Fail if g_pMappedEmbedded == NULL ...
在Windows Embedded CE 6.0中，设备驱动程序只需要检查嵌套的指针参数是否有效就可以了。所不同的是在Windows Embedded CE 6.0中需要使用CeOpenCallerBuffer这个API来检查是否对于某一段内存，调用进程是否有访问权限。
// Now in the New CE Version
// In XXX_IOControl...
hr = CeOpenCallerBuffer((PVOID*)&g_pMappedEmbedded, pInput->pEmbedded, pInput->dwSize, ARG_I_PTR, FALSE);
// Fail if FAILED(hr) == true
// When done with pointer...
hr = CeCloseCallerBuffer((PVOID)g_pMappedEmbedded, pInput->pEmbedded, pInput->dwSize, ARG_I_PTR );
### 2            Marshalling
处于内核态的线程对位于用户空间的内存的访问可以分为两种方式。同步访问（Synchronous Access）和异步访问（Asynchronous Access）。同步访问是指，当访问这块内存区域时，处于内核态的线程是在调用者的上下文环境内。而异步访问正好相反，设备驱动程序所拥有的一个线程要访问的一块内存区域，而该区域正位于其他进程的地址空间中，那么这就是异步访问。
举个异步访问的例子，比如应用程序通过WriteFile API来向设备SD卡中写入一段数据，最后写的请求会通过操作系统内核发送到SD卡的设备驱动中，由设备驱动程序来写入数据。但是I/O操作是一个很慢的操作，为了使CPU达到高的利用率，内核往往会在这时重新调度一个线程开始执行，这样用户空间就可能发生了一次切换。这里说可能，是因为如果被调度的线程和当前的应用程序（也就是当前进程）处于同一进程空间的话，那么就不发生进程的切换，并且要写入的这段数据由于还是位于当前进程空间中，所以就是对于这块数据的访问就是同步的。反之，如果被调度的线程不处于当前进程的空间，那么就会发生一次进程的切换，新的进程会替换老的进程，这时要写入的这块数据区域对于设备驱动程序中的线程来说就是无效的，这也称为异步访问。
对于异步访问必须采用一种叫Marshalling的技术来处理被访问的数据区域。
在Windows CE 5.0中，所有的用户态进程共享底部的虚拟地址。所以当Slot 0中被替换为不同进程时，由于所有的进程的虚拟地址空间对于设备驱动程序来说都可见，只需要对指向数据块首地址的指针做一次偏移，就可以得到这块数据。
在Windows Embedded CE 6.0中，每个用户态的进程都有自己独有的虚拟地址空间，每个进程的虚拟地址空间都是受到保护的。所以对一块内存做Marshall不再是简单的将指针进行偏移。这时可以采用两种方法来对一块内存做Marshall，一种方法是将这块内存拷贝一份这样就可以安全的访问，这种方法称为复制。另一种方法是用一个新的虚拟地址去引用所对应的物理，这样一来这块数据就分别被两个指针分别引用，这种方法称为别名。
Marshall一块内存区域需要区分同步访问和异步访问。在Windows CE 5.0中，对于同步访问来说，不需要做多余的工作，只需要调用MapCallerPtr API来Marshall嵌套指针所指的地址即可。对于异步访问，线程对于每个Slot都有一定的访问权限，所以需要通过调用SetProcPermissions来先获得调用进程Slot的访问权限，然后调用MapCallerPtr来Marshall所要异步访问的内存。
// Windows CE 5.0 and prior versions
// In XXX_IOControl...
 SetProcPermissions(-1);
g_pMappedEmbedded = MapCallerPtr( p->pEmbedded );
// Fail if g_pMappedEmbedded == NULL ...
在Windows Embedded CE 6.0中，对于同步访问，通过设备驱动程序通过调用CeOpenCallerBuffer来Marshall嵌套指针所指的数据块的首地址，当这块数据使用完以后，设备驱动程序调用CeFreeCallerBuffer来释放Marshall所得资源。对于异步访问，假设设备驱动程序可以同步的访问一块内存，那么这时就可以通过调用CeAllocAsynchronousBuffer来Marshall这块内存用来做异步访问之用。在使用完之后，可以通过调用[CeFreeAsynchronousBuffer]()来释放Marshall所得资源。
// Now in the New OS Version
// In XXX_IOControl after CeOpenCallerBuffer generates
// g_pMappedEmbedded...
hr = CeAllocAsynchronousBuffer((PVOID*)&g_pMarshalled, g_pMappedEmbedded, pInput->dwSize, ARG_I_PTR);
// Fail if FAILED(hr) == true
// When done with pointer...
hr = CeFreeAsynchronousBuffer((PVOID)g_pMarshalled, g_pMappedEmbedded, pInput->dwSize, ARG_I_PTR);
// Now call CeCloseCallerBuffer as usual...
### 3            用户模式下的驱动程序
在Windows Embedded CE 6.0之前，设备驱动程序是加载在device.exe进程之中的，而device.exe与普通应用程序一样也是用户态的进程。所以每次应用程序希望能够与外设进行交互时，都需要通过操作系统内核转发请求到相应的驱动程序。这样一个请求就可以需要反复的进出内核多次，还需要在不同的进程间进行切换。这样做的好处和缺点同样的明显，优点是操作系统的稳定性得到了提高，不会因为某个设备驱动中的缺陷而使整个操作系统崩溃。但缺点是完成请求的效率太低。
在Windows Embedded CE 6.0中，设备驱动程序能够工作在用户模式或是内核模式两种不同的模式。由于在Windows Embedded CE 6.0新的体系结构中将操作系统关键的部件如文件系统Filesystem.exe、设备驱动程序管理Device.exe等都移进了操作系统内核之中，驱动程序完成一个请求不再需要在不同的进程下进行切换，也不需要反复的进出内核，所以内核模式下的驱动程序完成请求的效率将会大大的提高。但是这样的效率提高也是要有代价的。内核模式下的驱动程序需要有很高的稳定性，任何一个错误都可能引起整个操作系统的崩溃。为了解决这个问题，Windows
 Embedded CE 6.0中还设计了另一类称为用户模式的设备驱动程序。
Windows Embedded CE 6.0中设计了一个用户进程udevice.exe用来加载设备驱动程序。因为是被用户态的进程所加载，所以这类驱动程序就工作在用户空间，这类驱动程序就成为用户模式下的设备驱动程序。用户模式的驱动程序无疑将增加操作系统的稳定性，并且由于这类驱动程序工作在用户空间，所以能力有限，不能使用诸如VirtualCopy这样的特权API，也不能对系统中的硬件资源有任意的访问权限。
用户模式的设备驱动程序还有一个特点就是它们与内核模式的设备驱动程序具有高度的兼容性。一个好的用户模式的设备驱动程序的源代码不需要做任何的改动就可以做为内核模式的设备驱动程序被加载进内核空间。区分用户模式的设备驱动程序和内核模式的设备驱动程序标志就是设备驱动程序在系统注册表中的Flag值。当Flag具有DEVFLAGS_LOAD_AS_USERPROC（0x10）时，系统会将设备驱动程序加载成用户模式，如果没有该标志就加载成内核模式。
总的来说，用户模式下的设备驱动程序和内核模式下的设备驱动程序是很相似的。在有了用户模式设备驱动程序和内核模式设备驱动程序后，在OEM开发BSP的过程中，如果采用了某些第三方的，未经充分测试的驱动程序后。可以先将这些驱动程序做为用户模式下的设备驱动程序加载到用户空间，等到整个系统经过测试可以长时间稳定运行后，再将其转变成内核模式下的驱动程序加载到内核空间以提高整个系统的效率。
