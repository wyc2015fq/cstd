# WINCE下内核模式和用户模式的区别 - xqhrs232的专栏 - CSDN博客
2013年10月23日 17:17:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：723
原文地址::[http://www.cnblogs.com/sankye/articles/1590065.html](http://www.cnblogs.com/sankye/articles/1590065.html)
相关文章
1、WINCE下内核模式和用户模式的区别 ----[http://motion.chinaitlab.com/WINCE/878224.html](http://motion.chinaitlab.com/WINCE/878224.html)
为了使读者能够详细了解WINCE的地址映射原理还有两种模式，在这里我分几个部分说明： 
1、 WINCE内核nk.exe的任务是管理操作系统核心功能。按照OEMAddressTable的映射要求，所有物理地址都映射到0x80000000以上，所以对于内核程序nk.exe和内核模式下的线程来说，只要访问0x80000000以上的有效虚拟地址经MMU就能够访问物理地址，无需再映射是内核模式的一个特点。内核模式的第二个特点是没有地址访问限制，内核模式线程可以访问任何有效虚拟地址，所谓有效虚拟地址是指有实际事物对应。
2、用户模式线程只能访问0x80000000以下的虚拟地址空间，WINCE6.0之前版本的内核为每个进程划分32MB的地址空间，在不调用特殊函数的情况下不能相互访问，这样的设计使得WINCE系统更安全、更稳定，限制访问地址是用户模式的第一个特点。第二个特点就是需要多一层映射，如果线程要访问物理内存的话需要先映射到0x80000000以上，再经MMU访问物理内存地址。 
WINCE的线程具有转移性（参考 API GetCallerProcess的说明，有一个很好的例子），当应用程序的线程调用API或者调用驱动程序接口函数时，该线程会转移到 gwes.exe、device.exe、filesys.exe等进程中执行，转移是由WINCE内核操作的，它会修改线程的上下文，记录线程的当前进程、调用者进程、拥有者进程三个值。 
3、如果在定制内核的时候选择了“Full Kernel Mode”，那么在这个内核上运行的所有线程都处于内核模式，即使调用SetKMode(FALSE)后线程仍然具有内核模式的特点，能够访问任何有效的虚拟地址。假设现有一个64MB RAM的 WINCE产品，RAM映射从0x80000000到0x84000000，如果线程处于内核模式，它就直接可以访问这个范围的虚拟地址： 
在OnButton1()中编写 
DWORD oldMode = SetKMode(FALSE); 
volatile int *piTemp = (volatile int*)(0x20000000+0x84000000-0x00019000); ///或者(0x84000000-0x00019000) 
*piTemp = 12345; 
在OnButton2()中编写 
DWORD oldMode = SetKMode(FALSE); 
volatile int *piTemp = (volatile int*)(0x20000000+0x84000000-0x00019000); ///或者(0x84000000-0x00019000) 
int iTemp = *piTemp; 
先只执行OnButton1()然后关闭程序，再重启程序然后执行OnButton2()，iTemp仍然等于12345。结果说明了两点：内核模式线程可以直接访问0x80000000以上的有效虚拟地址；我们写到RAM中的数据没有丢失，说明虚拟地址有效。 
如果在定制内核的时候没有选择“Full Kernel Mode”，那么在这个内核上运行的所有线程都处于用户模式。可以调用SetKMode(TRUE)使调用线程暂时处于内核模式，还是原来的假设环境，我再举个例子： 
在OnButton1()中编写 
DWORD oldMode = SetKMode(TRUE); 
volatile int *piTemp = (volatile int*)(0x20000000+0x84000000-0x00019000); ///或者(0x84000000-0x00019000) 
*piTemp = 12345; 
在用户模式下，如果不调用SetKMode(TRUE)，那么执行*piTemp = 12345一定会弹出对话框，提示地址访问非法，如果调用SetKMode(TRUE)就不会提示地址访问非法，而且在OnButton2()中仍然能得到12345这个值。 
通过这两个例子我相信读者能够完全了解两种模式的区别了。
4、 WINCE提供了两个函数SetKMode和SetProcPermissions，其中SetKMode能够把调用线程切换到内核模式，还可以切换回用户模式。SetProcPermissions + GetCurrentPermissions添加当前进程访问权限给调用线程， SetProcPermissions (0xFFFFFFFF)能让调用线程访问所有进程空间，但是调用线程仍然处于用户模式。SetKMode和 SetProcPermissions函数使得用户模式的特点不那么明晰。 
如上所说一个应用程序的线程可能转移到其它两个进程地址空间中读写数据，而每一个线程在被创建的时候只有访问创建它的进程地址空间的权限，所以驱动程序开发者必须在驱动程序读写数据前调用SetKMode或者 SetProcPermissions增加调用此函数的线程访问其它进程空间的权限。如果一个应用程序的线程只转移到一个进程地址空间，一般为设备管理器进程device.exe，这种情况下不必增加线程访问其它进程空间的权限，但如果驱动程序本身创建了一个线程，那还是要调用SetKMode或者
 SetProcPermissions增加新的线程访问其它进程的权限的，因为驱动程序创建线程时，当前进程为设备管理器，所以新线程只具有访问设备管理器进程空间的权限，而不具备访问应用程序进程空间的权限。 
5、可能一个编写过简单的流驱动的初学者会很疑惑，因为开发一个简单的流驱动程序根本不需要调用这些函数，也没有调用过MapPtrToProcess，那是因为如果标准流驱动接口函数的参数为指针（ReadFile、 WriteFile、DeviceIoControl参数都有指针），WINCE内核会自动映射指针包含的地址，但仅此而已，其余任何情况都要求开发者自行处理，比如流接口函数的参数是一个指向结构体的指针PA，而结构体中包括指针PB，PB指针就必须在流接口函数中映射，映射后才能访问，否则就会造成地址访问非法。所以结构体中每个指针都要映射。 
为了让读者能了解其中的原因，我举个例子： 
假设设备管理器被加载到Slot4，应用程序A被加载到Slot 8，A只有一个主线程T，T开始执行，按照WINCE的规定，正获得CPU的进程必须映射到Slot0，那么在执行代码的时候 A的所有虚拟地址都被减去一个偏移值，也就是8×0x02000000，A调用DeviceIoControl，传递一个指向一个结构体的指针B，而这个结构体中包含一个指针C，指针C包含的地址假设为0x00030000，当执行DeviceIoControl时WINCE把设备管理器的进程地址空间映射到Slot0，因为放在注册表[HKLM\Drivers\BuiltIn]下的驱动程序是由设备管理器加载的，自然驱动程序的代码段被加载到设备管理器进程空间，但是线程仍然是T，此时T的当前所在进程为设备管理器（CurrentProcess），A变成了T的调用者进程（CallerProcess），T自动具有了访问调用者进程空间的权限。这时访问Slot0中的虚拟地址其实质就是访问设备管理器的进程地址空间，要把地址加上一个偏移值，也就是4×0x02000000，所以DeviceIoControl访问指针C包含的地址时本应该加上8×0x02000000，却加上4×0x02000000，结果地址并不是设备管理器的合法区域，系统就会提示地址访问非法。而如果做了一个映射，指针C包含的地址就会被加一个正确的偏移值，使地址处于A的地址空间Slot
 8中，T此时具有访问A进程空间的权限，访问到正确的虚拟地址当然会得到正确的数据了。 
//=============================================
备注：：
1》WINCE6.0已经不支SetKMode/SetProcPermissions/GetCurrentPermissions这样的模式切换函数了！！！
