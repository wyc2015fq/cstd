# 插入DLL和挂接API——Windows核心编程学习手札之二十二 - 专注于数据挖掘算法研究和应用 - CSDN博客





2009年09月11日 16:15:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2950








# 插入DLL和挂接API

## ——Windows核心编程学习手札之二十二

如下情况，可能要打破进程的界限，访问另一个进程的地址空间：

1）为另一个进程创建的窗口建立子类时；

2）需要调试帮助时，如需要确定另一个进程正在使用那个DLL；

3）需要挂接其他进程时；

基于之上情况，下面的方法可将DLL插入到另一个进程的地址空间中，一旦DLL进入另一个进程的地址空间，就可以对另一个进程为所欲为。

### 使用注册表来插入DLL

整个系统的配置都是在注册表中维护的，可调整其设置改变系统的行为特性，下面的关键字：

HKEY_LOCAL_MACHINE/Software/Microsoft/WindowsNT/

CurrentVersion/Windows/AppInit_DLLS

AppInit_DLLS关键字包含一个DLL文件或一组DLL文件名（用空格或逗号隔开，避免使用含空格的文件名），列出第一个DLL文件名可包含其路径，但包含路径的其他DLL将被忽略，因此最好将DLL放入Windows的系统目录中，这样不需要设定路径，如设置该关键字值为C:/MyLib.dll。当重启计算机及Windows进行初始化时，系统将保存这个关键字的值。然后当User32.dll库被映射到进程中时，将接收到一个DLL_PROCESS_ATTACH通知，这个通知被处理时，User32.dll便检索保存这个关键字的值，并且为字符串中指定的每个DLL调用LoadLibrary函数。当每个库被加载时，便调用和该库相关的DllMain函数，其fdwReason的值是DLL_PROCESS_ATTACH，如此，每个库就能够对自己进行初始化。该方法简单，但不足有：

1）系统是在初始化时读取该关键字的值，因此修改该值需重新启动计算机；

2）插入的DLL是映射到使用User32.dll进程中，所有基于GUI的应用程序都使用User32.dll，不过多数基于GUI的应用程序不使用插入的DLL，而且如将DLL插入编译器或链接程序，这种方法将不起作用；

3）插入的DLL被映射到每个基于GUI应用程序中，DLL映射的进程太多，“容器”进程崩溃的可能性越大；

4）插入的DLL被映射到每个基于GUI应用程序中，应仅必要时保持DLL的插入状态。

### 使用Windows挂钩来插入DLL

进程A（类似Microsoft Spy++的一个实用程序）安装了一个挂钩WN_GETMESSAGE，以便查看系统中的各个窗口处理的消息，挂钩是通过调用SetWindowsHookEx函数来安装的：

HHOOK hHook=SetWindowsHookEx(WH_GETMESSAGE,GetMsgProc,hinstDll,0);

参数WH_GETMESSAGE用于指明要安装的挂钩类型，参数GetMsgProc是窗口准备处理一个消息时系统调用的函数的地址（在进程地址空间内）；第三个参数hinstDll是包含GetMsgProc函数的DLL。在Windows中，DLL的hinstDll的值用于标识DLL被映射到的进程的地址空间中的虚拟内存地址，最后一个参数0表示要挂接的线程，也可以传递系统中另一个线程的ID，传递0告诉系统想要挂接系统中的所有GUI线程。安装挂钩后的情况：

1）进程B中的一个线程准备将一条消息发送到一个窗口；

2）系统查看该线程上是否已安装了WH_GETMESSAGE挂钩；

3）系统查看包含GetMsgProc函数的DLL是否包含被映射到进程B的地址空间中；

4）如果该DLL尚未被映射，系统将强制该DLL映射到进程B的地址空间，并且将进程B中的DLL映像的自动跟踪计数递增1；

5）当DLL的hinstDll用于进程B时，系统查看该函数，并且检查该DLL的hinstDll是否与其在进程A时所处的位置相同；如果两个hinstDll在系统位置上，那GetMsgProc函数的内存地址在两个进程的地址空间中的位置也是相同的，这种情况下，系统只要调用进程A的地址空间中的GetMsgProc函数即可；如位置不同，则系统要确定进程B的地址空间中GetMsgProc函数的虚拟内存地址，用下面公式确定：

GetMsgProc B=hinstDll B+(GetMsgProc A-hinstDll A)；

将GetMsgProc A的地址减去hinstDll A的地址，得到GetMsgProc函数的地址位移（以字节为计量单位），将这个位移与hinstDll B的地址相加，得到GetMsgProc函数在用于进程B的地址空间中该DLL的映像时其位置；

6）系统将进程B中的DLL映像的自动跟踪计数递增1；

7）系统调用进程B的地址空间中的GetMsgProc函数；

8）当GetMsgProc函数返回时，系统将进程B中的DLL映像的自动跟踪计数递减1；

当系统插入或者映射包含挂钩过滤器函数的DLL时，整个DLL均被映射，而不只是挂钩过滤器函数被映射，这意味着DLL中包含的任何一个函数或所有函数都被映射，可被进程B的环境运行的线程所调用。

若要为进程B中的线程创建的窗口建立子类，首先可以在创建该窗口的挂钩上设置一个WH_GETMESSAGE挂钩，然后在GetMsgProc函数被调用时，调用SetWindowLongPtr函数来建立窗口的子类，子类的过程与GetMsgProc函数在同一DLL文件中。

进程B中不再需要DLL时删除DLL映像，方法是调用：

BOOL UnhookWindowsHookEx(HHOOK hhook);

当洋线程调用UnhookWindowsHookEx函数时，系统将遍历将DLL插入到的各个进程的内部列表，并对DLL的自动跟踪计数进行递减，当递减到0时，DLL就从进程的地址空间中被删除。

### 使用远程线程来插入DLL

Windows的多数函数允许进程只对自己进行操作，可防止一个进程破坏另一个进程的运行，但对调试程序和一些工具而言，则需要操作其他进程。使用远程线程来插入DLL的方法要求目标进程中的线程调用LoadLibrary函数来加载必要的DLL，需要在目标进程中创建一个新线程，Windows提供了这样一个函数：

HANDLE CreateRomoteThread(

HANDLE hProcess,

PSECURITY_ATTRIBUTES psa,

DWORD dwStackSize,

PTHREAD_START_ROUTINE pfnStartAddr,

PVOID pvParam,

DWORD fdwCreate,

PDWORD pdwThreadId);

参数hProcess指明拥有新创建线程的进程，参数pfnStartAddr指明线程函数的内存地址，该内存地址和远程进程是相关的，线程函数代码不能在自己进程的地址空间中。

如执行下面代码：

HANDLE hThread=CreateRemoteThread(hProcessRemote,NULL,0,

LoadLibraryA,”C://MyLib.dll”,0,NULL);

或选用Unicode：

HANDLE hThread=CreateRemoteThread(hProcessRemote,NULL,0,

LoadLibraryW,L”C://MyLib.dll”,0,NULL);

当在远程进程中创建新线程时，该线程立即调用LoadLibrary函数，并将DLL的路径名的地址传递给它。直接将LoadLibrary作为线程执行函数（所传递的参数就是远程线程的起始地址），会出先问题：

1）当编译或链接一个程序时，产生的二进制代码包含一个输入节，这一节是有一系列输入函数的形式替换程序（thunk）组成。当代码调用一个如LoadLibrary函数时，链接程序将生成一个模块输入节中的形实替换程序并调用，然后，该形实替换程序便转移到实际的函数。在CreateRemoteThread的调用中使用一个对LoadLibrary的直接调用，则将在模块的输入节中转换成LoadLibrary的形实替换程序的地址，将形实替换程序的地址作为远程线程的起始地址来传递，会导致线程开始执行莫名其妙的代码。

解决方法是：若要强制直接调用LoadLibrary函数，避开形实替换程序，必须调用GetProcAddress函数，获取LoadLibrary的准确内存位置。对CreateRemoteThread调用的前提是，Kernel32.dll已经被同时映射到本地和远程进程的地址空间中，每个应用程序都需要Kernel32.dll，将Kernel32.dll映射到每个进程的同一个地址，如调用如下函数：

PTHREAD_START_ROUTINE pfnThreadRtn=(PTHREAD_START_ROUTINE)

GetProcAddress(GetModuleHandle(TEXT(“Kernel32”)),”LoadLibraryA”);

HANDLE hThread=CreateRemoteThread(hProcessRemote,NULL,0,

pfnThreadRtn,” C://MyLib.dll”,0,NULL);

或选用Unicode：

PTHREAD_START_ROUTINE pfnThreadRtn=(PTHREAD_START_ROUTINE)

GetProcAddress(GetModuleHandle(TEXT(“Kernel32”)),”LoadLibraryW”);

HANDLE hThread=CreateRemoteThread(hProcessRemote,NULL,0,

pfnThreadRtn,L” C://MyLib.dll”,0,NULL);

2）字符串” C://MyLib.dll”是在调用进程的地址空间中，该字符串的地址已经被赋予新创建的远程线程，该线程将其传递给LoadLibrary，但是，当LoadLibrary取消对内存地址的引用时，DLL路径名字符串将不再存在，远程进程的线程就可能引发访问违规，向用户显示一个未处理的异常条件消息框，并终止远程进程。

解决方法是：将DLL的路径名字符串放入远程进程的地址空间中，然后当CreateRemoteThread函数被调用时，必须将放置该字符串的地址（相对于远程进程的地址）传递给它，Windows提供了一个函数使得一个进程能够分配另一个进程的地址空间中的内存：

PVOID VirtualAllcoEx(

HANDLE hProcess,

PVOID pvAddress,

SIZE_T dwSize,

DWORD flAllocationType,

DWORD flProtect);

另一个函数则能够释放该内存：

BOOL VirtualFreeEx(

HANDLE hProcess,

PVOID pvAddress,

SIZE_T dwSize,

DWORD dwFreeType);

一旦为该字符串分配内存，还需要将该字符串从进程的地址空间拷贝到远程进程的地址空间中，Windows提供了一些函数，使得一个进程能够从另一个进程的地址空间中读取数据，并将数据写入另一个进程的地址空间。

BOOL ReadProcessMemory(

HANDLE hProcess,

PVOID pvAddressRemote,

PVOID pvBufferlocal,

DWORD dwSize,

PDWORD pdwNumbytesRead);

BOOL WriteProcessMemory(

HANDLE hProcess,

PVOID pvAddressRemote,

PVOID pvBufferlocal,

DWORD dwSize,

PDWORD pdwNumbytesWritten);

远程进程由hProcess参数来标识，参数pvAddressRemote用于指明远程进程的地址，参数pvBufferlocal是本地进程中的内存地址，参数dwSize需要传送的字节数，pdwNumbytesWritten和pdwNumbytesRead用于指明实际传送的字节数，当函数返回时，可查看这两个参数的值。

对此，执行步骤归纳如下：

1）使用VirtualAllocEx函数，分配远程进程的地址空间中的内存；

2）使用WriteProcessMemory函数，将DLL路径名拷贝到第一个步骤中已经分配的内存中；

3）使用GetProcAddress函数，获取LoadLibrary函数的实地址（在kernel32.dll中）；

4）使用CreateRemoteThread函数，在远程进程中创建一个线程，调用正确的LoadLibrary函数，为其传递第一个步骤中分配的内存地址；此时，DLL已经被插入到远程进程的地址空间中，同时DLL的DllMain函数接收到一个DLL_PROCESS_ATTACH通知，并且能够执行需要的代码，当DllMain函数返回时，远程线程从它对LoadLibrary的调用返回到BaseThreadStart函数，然后BaseThreadStart调用ExitThread，使远程线程终止运行；现在远程进程拥有第一个步骤中分配的内存块，而DLL仍保留在它的地址空间中，若要将它删除，需要在远程线程退出后执行下面步骤：

5）使用VirtualFreeEx函数，释放第一个步骤中分配的内存；

6）使用GetProcAddress函数，获得FreeLibrary函数的实地址（在Kernel32.dll中）；

7）使用CreateRemotThread函数，在远程进程中创建一个线程，调用FreeLibrary函数，传递远程DLL的HINSTANCE；

使用远程线程调用DLL只适用Windows2000，Windows1998不支持。

### 使用特洛伊DLL来插入DLL

该方法是取代所知道进程将要加载的DLL，如当你知道一个进程将要加载Xyz.dll，则创建自己的DLL并赋予相同的文件名，而把原来的Xyz.dll改成别的名字。在自己定义的Xyz.dll中，输出的全部符号必须与原始的Xyz.dll输出的符号相同，使用函数转发器可挂接某些函数。如果只想在单个应用程序中使用这个方法，那可以为自己的DLL给个独一的名字，并改变应用程序的.exe模块的输入节，并且只包含模块需要的DLL的名字。可搜索文件中的这个输入节，并且将它改变，使加载程序加载自己的DLL，需要数字.exe和DLL文件的格式。

### 将DLL作为调试程序来插入

调试程序能够对调试的进程执行特殊的操作，当被调试进程加载时，在被调试进程的地址空间作好准备，但是被调试进程的主线程尚未执行任何代码之前，系统将自动将这个情况统治调试程序，这时，调试程序可以强制将某些代码插入被调试进程的地址空间中（比如使用WriteProcessMemory函数来插入），然后使被调试进程的主线程执行该代码。这种方法要求对被调试线程的CONTEXT结构进行操作，意味着要编写特定CPU的代码，因此需要修改源代码，使之能够在不同的CPU平台上正确地运行，且对被调试进程执行的机器语言指令进行硬编码。调试程序和被调试程序之间必须存在固定关系。如果调试程序终止运行，Windows将自动撤消被调试进程。

### 用Windows98上的内存映射文件插入代码

在Windows98上运行的所有32位Windows应用程序均共享同样的最上面的2GB地址空间，如果分配这里面的某些存储器，那该存储器在每个进程的地址空间中均可以使用。若要分配2GB以上的存储器，则可使用内存映射文件。可以创建一个内存映射文件，然后调用函数MapViewOfFile显示，然后将数据写入地址空间区域（所有进程地址空间中的相同区域），必须使用硬编码的机器语言来进行这项操作，其结果是这种解决方案很难移植到别的CPU平台。

### 用CreateProcess插入代码

如果是父子进程关系，父进程可以得到子进程的主线程的句柄，使用该句柄，可以修改线程执行的代码。控制子进程的主线程执行：

1）使进程生成暂停运行的子进程；

2）从.exe模块的头文件中检索主线程的起始内存地址；

3）将机器指令保存在该内存地址中；

4）将某些硬编码的机器指令强制放入该地址中，这些指令应该调用LoadLibrary函数来加载DLL；

5）继续运行子进程的主线程，使该代码得以执行；

6）将原始指令重新放入起始地址；

7）让进程继续从起始地址开始执行，如同没有任何事一样。

### 挂接API

将DLL插入进程的地址空间是确定进程运行状况的方法，但仅仅插入DLL无法提供足够信息，有时需要知道进程中的线程是如何调用各个函数的，也可能需要修改Windows函数的功能。

例子：某DLL是由一个数据库产品加载的，该DLL作用是增强和扩展数据库产品的功能。当数据库产品终止运行时，该DLL就会收到DLL_PROCESS_DETACH通知，并且只有在这时才执行它的所有清除代码，清除工作包括调用其他DLL中的函数，以关闭套接字连接、文件和其他资源，但是在该DLL收到DLL_PROCESS_DETACH通知时，进程地址空间中的其他DLL已经收到它们的DLL_PROCESS_DETACH通知。因此，当该DLL视图清除时，其所调用其他DLL中的函数都失败，因为其他DLL已经撤消了初始化信息。

解决建议是挂接函数ExitProcess，调用ExitProcess将导致系统向该DLL发送DLL_PROCESS_DETACH通知，这个通知将在任何DLL得到DLL_PROCESS_DETACH通知之前进来，因此进程中的所有DLL仍然处于初始状态，并且能够正常运行，此时，该DLL知道进程将要终止，从而能够成功执行它的全部清除操作。然后，操作系统的ExitProcess函数被调用，使所有DLL收到它们的DLL_PROCESS_DETACH通知并进行清除操作，当该公司的DLL收到这个通知时，它将不执行专门清除操作，因为之前已经执行了。

插入DLL是随意进行，当该DLL被加载时，必须扫描所有已经加载的可执行模块和DLL模块，以便找出对ExitProcess的调用。该DLL必须修改调用ExitProcess的模块，该模块能调用该DLL中的函数，而不是调用操作系统的ExitProcess函数，一旦该DLL中的ExitProcess替换函数（挂钩函数）执行它的清除代码，操作系统的ExitProcess函数（在Kernel32.dll文件中）就被调用。下面是通过改写代码挂接API的具体操作方法：

1）找到要挂接的函数在内存中的地址（如Kernel32.dll中的ExitProcess）；

2）将该函数的头几个字节保存在你自己的内存中；

3）用一个JUMP CPU指令改写该函数的头几个字节，该指令会转移到替换函数的内存地址。当然，替换函数的标记必须与挂接的函数标记完全相同，即所有参数必须一样，返回值一样，调用规则一样；

4）当一个线程调用已经挂接的函数时，JUMP指令实际上将转移到替换函数，这样就能够执行任何代码；

5）取消函数的挂接状态，取出第二步保存的字节，将它们放会挂接函数的开头；

6）调用挂接函数（已不再挂接），该函数将执行其通常的处理操作；

7）但原始函数返回时，再次执行第二步和第三步，如此，替换函数就可以被调用；

该方法对CPU依赖性很大，需使用手工编码的机器指令才能使这个方法生效，且在抢占式多线程环境中根本不起作用。

另一种挂接方法是通过操作模块的输入节来实现。模块的输入节包含一组该模块运行时需要的DLL，另外，还包含该模块从每个DLL输入的符号的列表，当模块调用一个输入函数时，线程实际上要从模块的输入节中捕获需要的输入函数的地址，然后转移到该地址。要挂接一个特定函数，只要改变模块输入节中的地址。



