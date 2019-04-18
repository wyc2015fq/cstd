# WINDOWS核心编程笔记(22-27) - 深之JohnChen的专栏 - CSDN博客

2005年12月10日 12:57:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：5027


第22章插入DLL和挂接API

在MicrosoftWindows中，每个进程都有它自己的私有地址空间。当使用指针来引用内存时，指针的值将引用你自己进程的地址空间中的一个内存地址。你的进程不能创建一个其引用属于另一个进程的内存指针。因此，如果你的进程存在一个错误，改写了一个随机地址上的内存，那么这个错误不会影响另一个进程使用的内存。
在Windows98下运行的各个进程共享2GB的地址空间，该地址空间从0x80000000至0xFFFFFFFF。只有内存映像文件和系统组件才能映射到这个区域。
独立的地址空间对于编程人员和用户来说都是非常有利的。然而有些情况下，必须打破进程的界限，访问另一个进程的地址空间，这些情况包括：
•当你想要为另一个进程创建的窗口建立子类时。
•当你需要调试帮助时（例如，当你需要确定另一个进程正在使用哪个DLL时）。
•当你想要挂接其他进程时。
本章将介绍若干种方法，可以用来将DLL插入到另一个进程的地址空间中。一旦你的DLL进入另一个进程的地址空间，就可以对另一个进程为所欲为。
22.1插入DLL 
假设你想为由另一个进程创建的窗口建立一个子类。你可能记得，建立子类就能够改变窗口的行为特性。若要建立子类，只需要调用SetWindowLongPtr函数，改变窗口的内存块中的窗口过程地址，指向一个新的（你自己的）WndProc。PlatformSDK文档说，应用程序不能为另一个进程创建的窗口建立子类。这并不完全正确。为另一个进程的窗口建立子类的关键问题与进程地址空间的边界有关。
当调用下面所示的SetWindowsLongPtr函数，建立一个窗口的子类时，你告诉系统，发送到或者显示在hwnd设定的窗口中的所有消息都应该送往MySubclassProc，而不是送往窗口的正常窗口过程：SetWindowLongPtr(hwnd,GWLP_WNDPROC,MySubclassProc);
换句话说，当系统需要将消息发送到指定窗口的WndProc时，要查看它的地址，然后直接调用WndProc。在本例中，系统发现MySubclassProc函数的地址与窗口相关联，因此就直接调用MySubclassProc函数。
为另一个进程创建的窗口建立子类时遇到的问题是，建立子类的过程位于另一个地址空间中。解决办法是：只需要用DLL“插入”进程的地址空间的方法来进行这项操作。有若干种方法可以用来进行这项操作。下面将逐个介绍它们。
22.2使用注册表来插入DLL
Windows98Windows98将忽略注册表的这个关键字。在Windows98下，无法使用该方法插入DLL。
启动注册表编辑器，进入：HKEY_LOCAL_MACHINE/Software/Microsoft/Windows NT/CurrentVersion/Windows/AppInit_Dlls.该关键字的值包含一个DLL文件名或者一组DLL文件名（用空格或逗号隔开）。由于空格用来将文件名隔开，因此必须避免使用包含空格的文件名。列出的第一个DLL文件名可以包含一个路径，但是包含路径的其他DLL均被忽略。由于这个原因，最好将你的DLL放入Windows的系统目录中，这样就不必设定路径。在窗口中，我将该值设置为单个DLL路径名C:/MyLib.dll。
当重新启动计算机及Windows进行初始化时，系统将保存这个关键字的值。然后，当User32.dll库被映射到进程中时，它将接收到一个DLL_PROCESS_ATTACH通知。当这个通知被处理时，User32.dll便检索保存的这个关键字中的值，并且为字符串中指定的每个DLL调用LoadLibrary函数。当每个库被加载时，便调用与该库相关的DllMain函数，其fdwReason的值是DLL_PROCESS_ATTACH，这样，每个库就能够对自己进行初始化。由于插入的DLL在进程的寿命期中早早地就进行了加载，因此在调用函数时应该格外小心。调用kernel32.dll中的函数时应该不会出现什么问题，不过调用其他DLL中的函数时就可能产生一些问题。User32.dll并不检查每个库是否已经加载成功，或者初始化是否取得成功。
在插入DLL时所用的所有方法中，这是最容易的一种方法。要做的工作只是将一个值添加到一个已经存在的注册表关键字中。不过这种方法也有它的某些不足：
•由于系统在初始化时要读取这个关键字的值，因此在修改这个值后必须重新启动你的计算机—即使退出后再登录，也不行。当然，如果从这个关键字的值中删除DLL，那么在计算机重新启动之前，系统不会停止对库的映射操作。
•你的DLL只会映射到使用User32.dll的进程中。所有基于GUI的应用程序均使用User32.dll，不过大多数基于CUI的应用程序并不使用它。因此，如果需要将DLL插入编译器或链接程序，这种方法将不起作用。
•你的DLL将被映射到每个基于GUI的应用程序中，但是必须将你的库插入一个或几个进程中。你的DLL映射到的进程越多，“容器”进程崩溃的可能性就越大。毕竟在这些进程中运行的线程是在执行你的代码。如果你的代码进入一个无限循环，或者访问的内存不正确，就会影响代码运行时所在进程的行为特性和健壮性。因此，最好将你的库插入尽可能少的进程中。
•你的DLL将被映射到每个基于GUI的应用程序中。这与上面的问题相类似。在理想的情况下，你的DLL只应该映射到需要的进程中，同时，它应该以尽可能少的时间映射到这些进程中。假设在用户调用你的应用程序时你想要建立WordPad的主窗口的子类。在用户调用你的应用程序之前，你的DLL不必映射到WordPad的地址空间中。如果用户后来决定终止你的应用程序的运行，那么你必须撤消WordPad的主窗口。在这种情况下，你的DLL将不再需要被插入WordPad的地址空间。最好是仅在必要时保持DLL的插入状态。
22.3使用Windows挂钩来插入DLL
可以使用挂钩将DLL插入进程的地址空间。为了使挂钩能够像它们在16位Windows中那样工作，Microsoft不得不设计了一种方法，使得DLL能够插入另一个进程的地址空间中。下面让我们来看一个例子。进程A（类似MicrosoftSpy++的一个实用程序）安装了一个挂钩WN_GETMESSAGE，以便查看系统中的各个窗口处理的消息。该挂钩是通过调用下面的SetWindowsHookEx函数来安装的：
HHOOK hHook=SetWindowsHookEx(WH_GETMESSAGE,GetMsgProc,hinstdll,0);
第一个参数WH_GETMESSAGE用于指明要安装的挂钩的类型。第二个参数GetMsgProc用于指明窗口准备处理一个消息时系统应该调用的函数的地址（在你的地址空间中）。第三个参数hinstDll用于指明包含GetMsgProc函数的DLL。在Windows中，DLL的hinstDll的值用于标识DLL被映射到的进程的地址空间中的虚拟内存地址。最后一个参数0用于指明要挂接的线程。
对于一个线程来说，它可以调用SetWindowsHookEx函数，传递系统中的另一个线程的ID。通过为这个参数传递0，就告诉系统说，我们想要挂接系统中的所有GUI线程。
现在让我们来看一看将会发生什么情况：
1)进程B中的一个线程准备将一条消息发送到一个窗口。
2)系统查看该线程上是否已经安装了WH_GETMESSAGE挂钩。
3)系统查看包含GetMsgProc函数的DLL是否被映射到进程B的地址空间中。
4)如果该DLL尚未被映射，系统将强制该DLL映射到进程B的地址空间，并且将进程B中的DLL映像的自动跟踪计数递增1。
5)当DLL的hinstDll用于进程B时，系统查看该函数，并检查该DLL的hinstDll是否与它用于进程A时所处的位置相同。
如果两个hinstDll是在相同的位置上，那么GetMsgProc函数的内存地址在两个进程的地址空间中的位置也是相同的。在这种情况下，系统只需要调用进程A的地址空间中的GetMsgProc函数即可。
如果hinstDll的位置不同，那么系统必须确定进程B的地址空间中GetMsgProc函数的虚拟内存地址。这个地址可以使用下面的公式来确定：
GetMsgProc B=hinstDll B+(GetMsgProc A – hinstDll A)
将GetMsgProcA的地址减去hinstDllA的地址，就可以得到GetMsgProc函数的地址位移（以字节为计量单位）。将这个位移与hinstDllB的地址相加，就得出GetMsgProc函数在用于进程B的地址空间中该DLL的映像时它的位置。
6)系统将进程B中的DLL映像的自动跟踪计数递增1。
7)系统调用进程B的地址空间中的GetMsgProc函数。
8)当GetMsgProc函数返回时，系统将进程B中的DLL映像的自动跟踪计数递减1。
注意，当系统插入或者映射包含挂钩过滤器函数的DLL时，整个DLL均被映射，而不只是挂钩过滤器函数被映射。这意味着DLL中包含的任何一个函数或所有函数现在都存在，并且可以从进程B的环境下运行的线程中调用。
若要为另一个进程中的线程创建的窗口建立子类，首先可以在创建该窗口的挂钩上设置一个WH_GETMESSAGE挂钩，然后，当GetMsgProc函数被调用时，调用SetWindowLongPtr函数来建立窗口的子类。当然，子类的过程必须与GetMsgProc函数位于同一个DLL中。
与插入DLL的注册表方法不同，这个方法允许你在另一个进程的地址空间中不再需要DLL时删除该DLL的映像，方法是调用下面的函数：BOOL UnhookWindowsHookEx(HHOOK hhook);
当一个线程调用UnhookWindowsHookEx函数时，系统将遍历它必须将DLL插入到的各个进程的内部列表，并且对DLL的自动跟踪计数进行递减。当自动跟踪计数递减为0时，DLL就自动从进程的地址空间中被删除。应该记得，就在系统调用GetMsgProc函数之前，它对DLL的自动跟踪计数进行了递增（见上面的第6个步骤）。这可以防止产生内存访问违规。如果该自动跟踪计数没有递增，那么当进程B的线程试图执行GetMsgProc函数中的代码时，系统中运行的另一个线程就可以调用UnlookWindowsHookEx函数。
这一切意味着不能撤消该窗口的子类并且立即撤消该挂钩。该挂钩必须在该子类的寿命期内保持有效状态。

22.4使用远程线程来插入DLL
插入DLL的第三种方法是使用远程线程。这种方法具有更大的灵活性。它要求你懂得若干个Windows特性、如进程、线程、线程同步、虚拟内存管理、DLL和Unicode等（如果对这些特性不清楚，请参阅本书中的有关章节）。Windows的大多数函数允许进程只对自己进行操作。这是很好的一个特性，因为它能够防止一个进程破坏另一个进程的运行。但是，有些函数却允许一个进程对另一个进程进行操作。这些函数大部分最初是为调试程序和其他工具设计的。不过任何函数都可以调用这些函数。
这个DLL插入方法基本上要求目标进程中的线程调用LoadLibrary函数来加载必要的DLL。由于除了自己进程中的线程外，我们无法方便地控制其他进程中的线程，因此这种解决方案要求我们在目标进程中创建一个新线程。由于是自己创建这个线程，因此我们能够控制它执行什么代码。幸好，Windows提供了一个称为CreateRemoteThread的函数，使我们能够非常容易地在另一个进程中创建线程：
HANDLE CreateRemoteThread(
HANDLE hProcess,
PSECURITY_ATTRIBUTES psa,
DWORD dwStackSize,
PTHREAD_START_ROUTINE pfnStartAddr,
PVOID pvParam,
DWORD fdwCreate,
PDWORD pdwThreadId);
CreateRemoteThread与CreateThread很相似，差别在于它增加了一个参数hProcess。该参数指明拥有新创建线程的进程。参数pfnStartAddr指明线程函数的内存地址。当然，该内存地址与远程进程是相关的。线程函数的代码不能位于你自己进程的地址空间中。
注意在Windows2000中，更常用的函数CreateThread是在内部以下面的形式来实现的：
HANDLE CreateThread( PSECURITY_ATTRIBUTES psa, DWORD dwStackSize, PTHREAD_START_ROUTINE pfnStartAddr, PVOID pvParam, DWORD fdwCreate, PDWORD pdwThreadId){
return(CreateRemoteThread(GetCurrentProcess(),psa, dwStackSize, pfnStartAddr, pvParam, fdwCreate, pdwThreadId));
}
Windows98在Windows98中，CreateRemoteThread函数不存在有用的实现代码，它只是返回NULL。调用GetLastError函数将返回ERROR_CALL_NOT_IMPLEMENTED（CreateThread函数包含用于在调用进程中创建线程的完整的实现代码）。由于CreateRemoteThread没有实现，因此，在Windows98下，不能使用本方法来插入DLL。
好了，现在你已经知道如何在另一个进程中创建线程了，但是，如何才能让该线程加载我们的DLL呢？答案很简单，那就是需要该线程调用LoadLibrary函数：
HINSTANCE LoadLibrary(PCTSTR pszLibFile);
实际上有两个LoadLibrary函数，即LoadLibraryA和LoadLibraryW。这两个函数之间存在的唯一差别是，传递给函数的参数类型不同。如果将库的文件名作为ANSI字符串来存储，那么必须调用LoadLibraryA（A是指ANSI）。如果将文件名作为Unicode字符串来存储，那么必须调用LoadLibraryW（W是指通配符）。
下面让我们将必须执行的操作步骤做一个归纳：
1)使用VirtualAllocEx函数，分配远程进程的地址空间中的内存。
2)使用WriteProcessMemory函数，将DLL的路径名拷贝到第一个步骤中已经分配的内存中。
3)使用GetProcAddress函数，获取LoadLibraryA或LoadLibratyW函数的实地址（在Kernel32.dll中）。
4)使用CreateRemoteThread函数，在远程进程中创建一个线程，它调用正确的LoadLibrary函数，为它传递第一个步骤中分配的内存的地址。
这时，DLL已经被插入远程进程的地址空间中，同时DLL的DllMain函数接收到一个DLL_PROCESS_ATTACH通知，并且能够执行需要的代码。当DllMain函数返回时，远程线程从它对LoadLibrary的调用返回到BaseThreadStart函数（第6章中已经介绍）。然后BaseThreadStart调用ExitThread，使远程线程终止运行。
现在远程进程拥有第一个步骤中分配的内存块，而DLL则仍然保留在它的地址空间中。若要将它删除，需要在远程线程退出后执行下面的步骤：
5)使用VirtualFreeEx函数，释放第一个步骤中分配的内存。
6)使用GetProcAddress函数，获得FreeLibrary函数的实地址（在Kernel32.dll中）。
7)使用CreateRemoteThread函数，在远程进程中创建一个线程，它调用FreeLibrary函数，传递远程DLL的HINSTANCE。
这就是它的基本操作步骤。这种插入DLL的方法存在的唯一一个不足是,Windows98并不支持这样的函数。只能在Windows2000上使用这种方法。

22.5使用特洛伊DLL来插入DLL
插入DLL的另一种方法是取代你知道进程将要加载的DLL。例如，如果你知道一个进程将要加载Xyz.dll，就可以创建你自己的DLL，为它赋予相同的文件名。当然，你必须将原来的Xyz.dll改为别的什么名字。
在你的Xyz.dll中，输出的全部符号必须与原始的Xyz.dll输出的符号相同。使用函数转发器（第20章做了介绍），很容易做到这一点。虽然函数转发器使你能够非常容易地挂接某些函数，你应该避免使用这种方法，因为它不具备版本升级能力。例如，如果你取代了一个系统DLL，而Microsoft在将来增加了一些新函数，那么你的DLL将不具备它们的函数转发器。引用这些新函数的应用程序将无法加载和执行。
如果你只想在单个应用程序中使用这种方法，那么可以为你的DLL赋予一个独一无二的名字，并改变应用程序的.exe模块的输入节。更为重要的是，输入节只包含模块需要的DLL的名字。你可以仔细搜索文件中的这个输入节，并且将它改变，使加载程序加载你自己的DLL。这种方法相当不错，但是必须要非常熟悉.exe和DLL文件的格式。
22.6将DLL作为调试程序来插入
调试程序能够对被调试的进程执行特殊的操作。当被调试进程加载时，在被调试进程的地址空间已经作好准备，但是被调试进程的主线程尚未执行任何代码之前，系统将自动将这个情况通知调试程序。这时，调试程序可以强制将某些代码插入被调试进程的地址空间中（比如使用WriteProcessMemory函数来插入），然后使被调试进程的主线程执行该代码。这种方法要求你对被调试线程的CONTEXT结构进行操作，意味着必须编写特定CPU的代码。必须修改你的源代码，使之能够在不同的CPU平台上正确地运行。另外，必须对你想让被调试进程执行的机器语言指令进行硬编码。而且调试程序与它的被调试程序之间必须存在固定的关系。如果调试程序终止运行，Windows将自动撤消被调试进程。而你则无法阻止它。
22.7用Windows98上的内存映射文件插入代码
在Windows98上插入你自己的代码是非常简单的。在Windows98上运行的所有32位Windows应用程序均共享同样的最上面的2GB地址空间。如果你分配这里面的某些存储器，那么该存储器在每个进程的地址空间中均可使用。若要分配2GB以上的存储器，只要使用内存映射文件（第17章已经介绍）。可以创建一个内存映射文件，然后调用MapViewOfFile函数，使它显示出来。然后将数据填入你的地址空间区域（这是所有进程地址空间中的相同区域）。
必须使用硬编码的机器语言来进行这项操作，其结果是这种解决方案很难移植到别的CPU平台。不过，如果进行这项操作，那么不必考虑不同的CPU平台，因为Windows98只能在x86CPU上运行。
这种方法的困难之处在于仍然必须让其他进程中的线程来执行内存映射文件中的代码。要做到这一点，需要某种方法来控制远程进程中的线程。CreateRemoteThread函数能够很好地执行这个任务，可惜Windows98不支持该函数的运行，而我也无法提供相应的解决方案。
22.8用CreateProcess插入代码
如果你的进程生成了你想插入代码的新进程，那么事情就会变得稍稍容易一些。原因之一是，你的进程（父进程）能够创建暂停运行的新进程。这就使你能够改变子进程的状态，而不影响它的运行，因为它尚未开始运行。但是父进程也能得到子进程的主线程的句柄。使用该句柄，可以修改线程执行的代码。你可以解决上一节提到的问题，因为可以设置线程的指令指针，以便执行内存映射文件中的代码。
下面介绍一种方法，它使你的进程能够控制子进程的主线程执行什么代码：
1)使你的进程生成暂停运行的子进程。
2)从.exe模块的头文件中检索主线程的起始内存地址。
3)将机器指令保存在该内存地址中。
4)将某些硬编码的机器指令强制放入该地址中。这些指令应该调用LoadLibrary函数来加载DLL。
5)继续运行子进程的主线程，使该代码得以执行。
6)将原始指令重新放入起始地址。
7)让进程继续从起始地址开始执行，就像没有发生任何事情一样。
上面的步骤6和7要正确运行是很困难的，因为你必须修改当前正在执行的代码。不过这是可能的。
这种方法具有许多优点。首先，它在应用程序执行之前就能得到地址空间。第二，它既能在Windows98上使用，也能在Windows2000上使用。第三，由于你不是调试者，因此能够很容易使用插入的DLL来调试应用程序。最后，这种方法可以同时用于控制台和GUI应用程序。当然，这种方法也有某些不足。只有当你的代码是父进程时，才能插入DLL。另外，这种方法当然不能跨越不同的CPU来运行，必须对不同的CPU平台进行相应的修改。
22.9挂接API的一个示例
将DLL插入进程的地址空间是确定进程运行状况的一种很好的方法。但是，仅仅插入DLL无法提供足够的信息，人们常常需要知道某个进程中的线程究竟是如何调用各个函数的，也可能需要修改Windows函数的功能。
例如，我知道一家公司生产的DLL是由一个数据库产品来加载的。该DLL的作用是增强和扩展数据库产品的功能。当数据库产品终止运行时，该DLL就会收到一个DLL_PROCESS_DETACH通知，并且只有在这时，它才执行它的所有清除代码。该DLL将调用其他DLL中的函数，以便关闭套接字连接、文件和其他资源，但是当它收到DLL_PROCESS_DETACH通知时，进程的地址空间中的其他DLL已经收到它们的DLL_PROCESS_DETACH通知。因此，当该公司的DLL试图清除时，它调用的许多函数的运行将会失败，因为其他DLL已经撤消了初始化信息。
一个好的办法就是利用挂接函数ExitProcess。如你所知，调用ExitProcess将导致系统向该DLL发送DLL_PROCESS_DETACH通知。通过挂接ExitPrecess函数，我们就能确保当ExitProcess函数被调用时，该公司的DLL能够得到通知。这个通知将在任何DLL得到DLL_PROCESS_DETACH通知之前进来，因此进程中的所有DLL仍然处于初始化状态中，并且能够正常运行。此时，该公司的DLL知道进程将要终止运行，并且能够成功地执行它的全部清除操作。然后，操作系统的ExitProcess函数被调用，使所有DLL收到它们的DLL_PROCESS_DETACH通知并进行清除操作。当该公司的DLL收到这个通知时，它将不执行专门的清除操作，因为它已经做了它必须做的事情。
在这个例子中，插入DLL是可以随意进行的，因为数据库应用程序的设计已经允许进行这样的插入，并且它加载了公司的DLL。当该公司的DLL被加载时，它必须扫描所有已经加载的可执行模块和DLL模块，以便找出对ExitProcess的调用。当它发现对ExitProcess的调用后，DLL必须修改这些模块，这样，这些模块就能调用公司的DLL中的函数，而不是调用操作系统的ExitProcess函数（这个过程比想象的情况要简单的多）。一旦公司的ExitProcess替换函数（即通常所说的挂钩函数）执行它的清除代码，操作系统的ExitProcess函数（在Kernel32.dll文件中）就被调用。
这个例子显示了挂接API的一种典型用法。它用很少的代码解决了一个非常实际的问题。
22.9.1通过改写代码来挂接API
API挂接并不是一个新技术，多年来编程人员一直在使用API挂接方法。如果要解决上面所说的问题，那么人们首先看到的“解决方案”是通过改写代码来进行挂接。下面是具体的操作方法：
1)找到你想挂接的函数在内存中的地址（比如说Kernel32.dll中的ExitProcess）。
2)将该函数的头几个字节保存在你自己的内存中。
3)用一个JUMPCPU指令改写该函数的头几个字节，该指令会转移到你的替换函数的内存地址。当然，你的替换函数的标记必须与你挂接的函数的标记完全相同，即所有的参数必须一样，返回值必须一样，调用规则必须一样。
4)现在，当一个线程调用已经挂接的函数时，JUMP指令实际上将转移到你的替换函数。这时，你就能够执行任何代码。
5)取消函数的挂接状态，方法是取出（第二步）保存的字节，将它们放回挂接函数的开头。
6)调用挂接的函数（它已不再被挂接），该函数将执行其通常的处理操作。
7)当原始函数返回时，再次执行第二和第三步，这样你的替换函数就可以被调用。
这种方法在16位Windows编程员中使用得非常普遍，并且用得很好。今天，这种方法存在着若干非常严重的不足，因此建议尽量避免使用它。首先，它对CPU的依赖性很大，在x86、Alpha和其他的CPU上的JUMP指令是不同的，必须使用手工编码的机器指令才能使这种方法生效。第二，这种方法在抢占式多线程环境中根本不起作用。线程需要占用一定的时间来改写函数开头的代码。当代码被改写时，另一个线程可能试图调用该同一个函数。结果将是灾难性的。
因此，只有当你知道在规定的时间只有一个线程试图调用某个函数时，才能使用这种方法。Windows98在Windows98上，主要的WindowsDLL（Kernel32、AdvAPI32、User32和GDI32）是这样受到保护的，即应用程序不能改写它们的代码页面。通过编写虚拟设备驱动程序（VxD）才能够获得这种保护。
22.9.2通过操作模块的输入节来挂接API
另一种API挂接方法能够解决我前面讲到的两个问题。这种方法实现起来很容易，并且相当健壮。但是，要理解这种方法，必须懂得动态连接是如何工作的。尤其必须懂得模块的输入节中保护的是什么信息。第19章已经用了较多的篇幅介绍了输入节是如何生成的以及它包含的内容。当阅读下面的内容时，可以回头参考第19章的有关说明。
如你所知，模块的输入节包含一组该模块运行时需要的DLL。另外，它还包含该模块从每个DLL输入的符号的列表。当模块调用一个输入函数时，线程实际上要从模块的输入节中捕获需要的输入函数的地址，然后转移到该地址。
要挂接一个特定的函数，只需要改变模块的输入节中的地址，就这么简单。它不存在依赖CPU的问题。同时，由于修改了函数的代码，因此不需要担心线程的同步问题。

第23章结束处理程序
本章讨论结构化异常处理（SEH）。使用SEH的好处是当你编写程序时，只需要关注程序要完成的任务。如果在运行时发生什么错误，系统会发现并将发生的问题通知你。利用SEH，你可以完全不用考虑代码里是不是有错误，这样就把主要的工作同错误处理分离开来。这样的分离，可以使你集中精力处理眼前的工作，而将可能发生的错误放在后面处理。
微软在Windows中引入SEH的主要动机是为了便于操作系统本身的开发。操作系统的开发人员使用SEH，使得系统更加强壮。我们也可以使用SEH，使我们的自己的程序更加强壮。使用SEH所造成的负担主要由编译程序来承担，而不是由操作系统承担。
由于各编译程序的实现上存在着差别，这样以特定的方式用特定的代码例子讨论SEH的优点就很困难。但大多数编译程序厂商都采用微软建议的文法。本书中的例子使用的文法和关键字可能与其他一些公司编译程序所使用的不同，但主要的SEH概念是一样的。本章使用MicrosoftVisualC++编译程序的文法。
注意不要将结构化异常处理同C++的异常处理相混淆。C++异常处理是一种不同形式的异常处理，其形式是使用C++关键字catch和throw。微软的VisualC++也支持C++的异常处理，并且在内部实现上利用了已经引入到编译程序和Windows操作系统的结构化异常处理的功能。
SEH实际包含两个主要功能：结束处理（terminationhandling）和异常处理（exceptionhandling）。本章讨论结束处理，下一章讨论异常处理。
一个结束处理程序能够确保去调用和执行一个代码块（结束处理程序，terminationhandler），而不管另外一段代码（保护体，guardedbody）是如何退出的。结束处理程序的文法结构（使用微软的VisualC++编译程序）如下：
__try{
//Guarded body
…
}
__finally{
//Termination handler
…
}
--try和--finally关键字用来标出结束处理程序两段代码的轮廓。在上面的代码段中，操作系统和编译程序共同来确保结束处理程序中的--finally代码块能够被执行，不管保护体（try块）是如何退出的。不论你在保护体中使用return，还是goto，或者是longjump，结束处理程序（finally块）都将被调用。下面将通过几个例子来说明这一点。
23.1通过例子理解结束处理程序
由于在使用SEH时，编译程序和操作系统直接参与了程序代码的执行，为了解释SEH如何工作，最好的办法就是考察源代码例子，讨论例子中语句执行的次序。
因此，在下面几节给出不同的源代码片段，对每一个片段解释编译程序和操作系统如何改变代码的执行次序。
23.2Funcenstein1
为了甄别使用结束处理程序的各种情况，我们来考察更具体的代码例子。
DWORD Funcenstein1(){
DWORD dwTemp;
//1.Do any processing here.
…
__try {
//2.Request permission to access protected data,and then use it.
WaitForSingleObject(g_hsem,INFINITE);
G_dwprotectedData=5;
Dwatemp=g_dwprotectedData;
}
__finally{
//3.Allow others to use protected data.
ReleaseSemaphore(g_hsem,1,NULL);
}
//4.continue processing.
Return(dwTemp);
}
上面程序中加了标号的注释指出了代码执行的次序。在Funcenstein1中，使用try-finally块并没有带来很多好处。代码要等待信标（semaphore），改变保护数据的内容，保存局部变量dwTemp的新值，释放信标，将新值返回给调用程序。
23.3Funcenstein2
现在我们把这个程序稍微改动一下，看会发生什么事情。
DWORD Funcenstein2(){
DWORD dwTemp;
//1.Do any processing here.
…
__try {
//2.Request permission to access protected data,and then use it.
WaitForSingleObject(g_hsem,INFINITE);
G_dwprotectedData=5;
dwtemp=g_dwprotectedData;
//return the new value.
Return(dwTemp);
}
__finally{
//3.Allow others to use protected data.
ReleaseSemaphore(g_hsem,1,NULL);
}
//4.continue processing.—this code will never execute in this version.
dwTemp=9;
Return(dwTemp);
}
在Funcenstein2中，try块的末尾增加了一个return语句。这个return语句告诉编译程序在这里要退出这个函数并返回dwTemp变量的内容，现在这个变量的值是5。但是，如果这个return语句被执行，该线程将不会释放信标，其他线程也就不能再获得对信标的控制。可以想象，这样的执行次序会产生很大的问题，那些等待信标的线程可能永远不会恢复执行。
通过使用结束处理程序，可以避免return语句的过早执行。当return语句试图退出try块时，编译程序要确保finally块中的代码首先被执行。要保证finally块中的代码在try块中的return语句退出之前执行。Funcenstein2中，将对ReleaseSemaphore的调用放在结束处理程序块中，保证信标总会被释放。这样就不会造成一个线程一直占有信标，否则将意味着所有其他等待信标的线程永远不会被分配CPU时间。
在finally块中的代码执行之后，函数实际上就返回。任何出现在finally块之下的代码将不再执行，因为函数已在try块中返回。所以这个函数的返回值是5，而不是9。
读者可能要问编译程序是如何保证在try块可以退出之前执行finally块的。当编译程序检查源代码时，它看到在try块中有return语句。这样，编译程序就生成代码将返回值（本例中是5）保存在一个编译程序建立的临时变量中。编译程序然后再生成代码来执行finally块中包含的指令，这称为局部展开。更特殊的情况是，由于try块中存在过早退出的代码，从而产生局部展开，导致系统执行finally块中的内容。在finally块中的指令执行之后，编译程序临时变量的值被取出并从函数中返回。
可以看到，要完成这些事情，编译程序必须生成附加的代码，系统要执行额外的工作。在不同的CPU上，结束处理所需要的步骤也不同。例如，在Alpha处理器上，必须执行几百个甚至几千个CPU指令来捕捉try块中的过早返回并调用finally块。在编写代码时，就应该避免引起结束处理程序的try块中的过早退出，因为程序的性能会受到影响。本章后面，将讨论__leave关键字，它有助于避免编写引起局部展开的代码。
设计异常处理的目的是用来捕捉异常的—不常发生的语法规则的异常情况（在我们的例子中，就是过早返回）。如果情况是正常的，明确地检查这些情况，比起依赖操作系统和编译程序的SEH功能来捕捉常见的事情要更有效。
注意当控制流自然地离开try块并进入finally块（就像在Funcenstein1中）时，进入finally块的系统开销是最小的。在x86CPU上使用微软的编译程序，当执行离开try块进入finally块时，只有一个机器指令被执行，读者可以在自己的程序中注意到这种系统开销。当编译程序要生成额外的代码，系统要执行额外的工作时（如同在Funcenstein2中），系统开销就很值得注意了。
23.4Funcenstein3
现在我们对函数再做修改，看会出现什么情况：
DWORD Funcenstein3(){
DWORD dwTemp;
//1.Do any processing here.
…
__try {
//2.Request permission to access protected data,and then use it.
WaitForSingleObject(g_hsem,INFINITE);
G_dwprotectedData=5;
dwtemp=g_dwprotectedData;
//try to jump over the finally block.
Goto Returnvalue;
}
__finally{
//3.Allow others to use protected data.
ReleaseSemaphore(g_hsem,1,NULL);
}
//4.continue processing.—this code will never execute in this version.
returnValue:
Return(dwTemp);
}
在Funcenstein3中，当编译程序看到try块中的goto语句，它首先生成一个局部展开来执行finally块中的内容。这一次，在finally块中的代码执行之后，在ReturnValue标号之后的代码将执行，因为在try块和finally块中都没有返回发生。这里的代码使函数返回5。而且，由于中断了从try块到finally块的自然流程，可能要蒙受很大的性能损失（取决于运行程序的CPU）。
23.5Funcfurter1
现在我们来考察另外的情况，这里可以真正显示结束处理的价值。看下面的函数：
DWORD Funcfurter1(){
DWORD dwTemp;
//1.Do any processing here.
…
__try {
//2.Request permission to access protected data,and then use it.
WaitForSingleObject(g_hsem,INFINITE);
dwtemp=funcinator(g_dwprotectedData);
}
__finally{
//3.Allow others to use protected data.
ReleaseSemaphore(g_hsem,1,NULL);
}
//4.continue processing.
Return(dwTemp);
}
现在假想一下，try块中的Funcinator函数调用包含一个错误，会引起一个无效内存访问。如果没有SEH，在这种情况下，将会给用户显示一个很常见的ApplicationError对话框。当用户忽略这个错误对话框，该进程就结束了。当这个进程结束（由于一个无效内存访问），信标仍将被占用并且永远不会被释放，这时候，任何等待信标的其他进程中的线程将不会被分配CPU时间。但若将对ReleaseSemaphore的调用放在finally块中，就可以保证信标获得释放，即使某些其他函数会引起内存访问错误。
如果结束处理程序足够强，能够捕捉由于无效内存访问而结束的进程，我们就可以相信它也能够捕捉setjump和longjump的结合，还有那些简单语句如break和continue。
23.6Funcenstein4
我们再看一种异常处理的情况。
DWORD Funcenstein4(){
DWORD dwTemp;
//1.Do any processing here.
…
__try {
//2.Request permission to access protected data,and then use it.
WaitForSingleObject(g_hsem,INFINITE);
G_dwprotectedData=5;
dwtemp=g_dwprotectedData;
//return the new value.
Return(dwTemp);
}
__finally{
//3.Allow others to use protected data.
ReleaseSemaphore(g_hsem,1,NULL);
Return(103);
}
//4.continue processing.—this code will never execute.
dwTemp=9;
Return(dwTemp);
}
在Funcenstein4中，try块将要执行，并试图将dwTemp的值（5）返回给Funcenstein4的调用者。如同对Funcenstein2的讨论中提到的，试图从try块中过早的返回将导致产生代码，把返回值置于由编译程序建立的临时变量中。然后，finally块中的代码被执行。在这里，与Funcenstein2不同的是在finally块中增加了一个return语句。Funcenstein4会向调用程序返回5还是103？这里的答案是103，因finally块中的return语句引起值103存储在值5所存储的临时变量中，覆盖了值5。当finally块完成执行，现在临时变量中的值（103）从Funcenstein4返回给调用程序。
我们已经看到结束处理程序在补救try块中的过早退出的执行方面很有效，但也看到结束处理程序由于要阻止try块的过早退出而产生了我们不希望有的结果。更好的办法是在结束处理程序的try块中避免任何会引起过早退出的语句。实际上，最好将return、continue、break、goto等语句从结束处理程序的try块和finally块中移出去，放在结束处理程序的外面。这样做会使编译程序产生较小的代码，因为不需要再捕捉try块中的过早退出，也使编译程序产生更快的代码（因为执行局部展开的指令也少）。另外，代码也更容易阅读和维护。
23.8Funcarama1
我们已经谈过了结束处理程序的基本语法和语意。现在看一看如何用结束处理程序来简化一个更复杂的编程问题。先看一个完全没有利用结束处理程序的函数：
BOOL Funcarama1(){
HANDLE hFIle=INVALID_HANDLE_VALUE;
PVOID pvBuf=NULL;
DWORD dwNumBytesRead;
BOOL fok;
hFile = CreateFile(“SOMEDATA.DAT”,GENERIC_READ,FILE_SHARE_READ,
NULL,OPEN_EXISTING,0,NULL);
If (hFile=INVALID_HANDLE_VALUE){
  Return(FALSE);
}
pvBuf=VirtualAlloc(NULL,1024,MEM_COMMIT,PAGE_READWRITE);
if (pvBuf=NULL){
CloseHandle(hFile);
Return(FALSE);
}
Fok=ReadFile(hFile,pvBuf,1024,&dwNumBytesRead,NULL);
If(!fok||(dwNumBytesRead==0)){
VirtualFree(pvBuf,MEM_RELEASE|MEM_DECOMMIT);
CloseHandle(hFile);
Return(FALSE);
}
//Do some calculation on the data.
…
//clean up all the resources.
VirtualFree(pvBuf,MEM_RELEASE|MEM_DECOMMIT);
closeHandle(hFile);
return(TRUE);
}
Funcarama1中的各种错误检查使这个函数非常难以阅读，也使这个函数难以理解、维护和修改。
23.9Funcarama2
当然，可以重新编写Funcarama1，使它更清晰一些，也更容易理解。
BOOL Funcarama2(){
HANDLE hFIle=INVALID_HANDLE_VALUE;
PVOID pvBuf=NULL;
DWORD dwNumBytesRead;
BOOL fok,fSuccess=FALSE;
hFile = CreateFile(“SOMEDATA.DAT”,GENERIC_READ,FILE_SHARE_READ,
NULL,OPEN_EXISTING,0,NULL);
If (hFile!=INVALID_HANDLE_VALUE){
  pvBuf=VirtualAlloc(NULL,1024,MEM_COMMIT,PAGE_READWRITE);
if (pvBuf!=NULL){
Fok=ReadFile(hFile,pvBuf,1024,&dwNumBytesRead,NULL);
If(fok||(dwNumBytesRead!=0)){
//Do some calculation on the data.
…
fSuccess=TRUE;
}
}
VirtualFree(pvBuf,MEM_RELEASE|MEM_DECOMMIT);
}
CloseHandle(hFile);
Return(fSuccess);
}
Funcarama2尽管比Funcarama1容易理解一些，但还是不好修改和维护。而且，当增加更多的条件语句时，这里的缩排格式就会走向极端，很快就到屏幕的最右边。
23.10Funcarama3
我们使用一个SEH结束处理程序来重新编写Funcarama1。
BOOL Funcarama3(){
//IMPORTANT:initialize all variables to assume failure.
HANDLE hFIle=INVALID_HANDLE_VALUE;
PVOID pvBuf=NULL;
__try{
DWORD dwNumBytesRead;
BOOL fok;
hFile = CreateFile(“SOMEDATA.DAT”,GENERIC_READ,FILE_SHARE_READ,
NULL,OPEN_EXISTING,0,NULL);
If (hFile=INVALID_HANDLE_VALUE){
  Return(FALSE);
}
pvBuf=VirtualAlloc(NULL,1024,MEM_COMMIT,PAGE_READWRITE);
if (pvBuf=NULL){
Return(FALSE);
}
Fok=ReadFile(hFile,pvBuf,1024,&dwNumBytesRead,NULL);
If(!fok||(dwNumBytesRead==!1024)){
Return(FALSE);
}
//Do some calculation on the data.
…
}
__finally{
//clean up all the resources.
If (pvBuf!=NULL)
VirtualFree(pvBuf,MEM_RELEASE|MEM_DECOMMIT);
If (hFile!=INVALID_HANDLE_VALUE)
closeHandle(hFile);
}
//Continue processing.
return(TRUE);
}
Funcarama3版的真正好处是函数的所有清理（cleanup）代码都局部化在一个地方且只在一个地方：finally块。如果需要在这个函数中再增加条件代码，只需在finally块中简单地增加一个清理行，不需要回到每个可能失败的地方添加清理代码。
23.11Funcarama4：最终的边界
Funcarama3版本的问题是系统开销。就像在Funcenstein4中讨论的，应该尽可能避免在try块中使用return语句。
为了帮助避免在try块中使用return语句，微软在其C/C++编译程序中增加了另一个关键字--leave。这里是Funcarma4版，它使用了--leave关键字：
BOOL Funcarama1(){
//IMPORTANT:initialize all variables to assume failure.
HANDLE hFIle=INVALID_HANDLE_VALUE;
PVOID pvBuf=NULL;
//assume that the function will not execute successfully.
BOOL fFunctionOk=FALSE;
__try{
DWORD dwNumBytesRead;
BOOL fok;
hFile = CreateFile(“SOMEDATA.DAT”,GENERIC_READ,FILE_SHARE_READ,
NULL,OPEN_EXISTING,0,NULL);
If (hFile=INVALID_HANDLE_VALUE){
  __leave;
}
pvBuf=VirtualAlloc(NULL,1024,MEM_COMMIT,PAGE_READWRITE);
if (pvBuf=NULL){
__leave;}
Fok=ReadFile(hFile,pvBuf,1024,&dwNumBytesRead,NULL);
If(!fok||(dwNumBytesRead==!1024)){
__leave;}
//Do some calculation on the data.
…
//indicate that the entire function executed successfully.
fFunctionOk=TRUE;
}
__finally{
//clean up all the resources.
If (pvBuf!=NULL)
VirtualFree(pvBuf,MEM_RELEASE|MEM_DECOMMIT);
If (hFile!=INVALID_HANDLE_VALUE)
closeHandle(hFile);
}
//Continue processing.
return(fFunctionOk);
}
在try块中使用--leave关键字会引起跳转到try块的结尾。可以认为是跳转到try块的右大括号。由于控制流自然地从try块中退出并进入finally块，所以不产生系统开销。当然，需要引入一个新的Boolean型变量fFunctionOk，用来指示函数是成功或失败。这是比较小的代价。
当按照这种方式利用结束处理程序来设计函数时，要记住在进入try块之前，要将所有资源句柄初始化为无效的值。然后，在finally块中，查看哪些资源被成功的分配，就可以知道哪些要释放。另外一种确定需要释放资源的办法是对成功分配的资源设置一个标志。然后，finally块中的代码可以检查标志的状态，来确定资源是否要释放。
23.12关于finally块的说明
我们已经明确区分了强制执行finally块的两种情况：
•从try块进入finally块的正常控制流。
•局部展开：从try块的过早退出（goto、longjump、continue、break、return等）强制控制转移到finally块。
第三种情况，全局展开（globalunwind），在发生的时候没有明显的标识，我们在本章前面Funcfurter1函数中已经见到。在Funcfurter1的try块中，有一个对Funcinator函数的调用。如果Funcinator函数引起一个内存访问违规（memoryaccessviolation），一个全局展开会使Funcfurter1的finally块执行。下一章将详细讨论全局展开。
由于以上三种情况中某一种的结果而导致finally块中的代码开始执行。为了确定是哪一种情况引起finally块执行，可以调用内部函数（或内蕴函数，intrinsicfunction）AbnormalTermination：BOOL AbnormalTermination();
这个内部函数只在finally块中调用，返回一个Boolean值。指出与finally块相结合的try块是否过早退出。换句话说，如果控制流离开try块并自然进入finally块，AbnormalTermination将返回FALSE。如果控制流非正常退出try块—通常由于goto、return、break或continue语句引起的局部展开，或由于内存访问违规或其他异常引起的全局展开—对AbnormalTermination的调用将返回TRUE。没有办法区别finally块的执行是由于全局展开还是由于局部展开。但这通常不会成为问题，因为可以避免编写执行局部展开的代码。
注意内部函数是编译程序识别的一种特殊函数。编译程序为内部函数产生内联（inline）代码而不是生成调用函数的代码。例如，memcpy是一个内部函数（如果指定/Oi编译程序开关）。当编译程序看到一个对memcpy的调用，它直接将memcpy的代码插入调用memcpy的函数中，而不是生成一个对memcpy函数的调用。其作用是代码的长度增加了，但执行速度加快了。
23.13Funcfurter2
我们已经知道如何编写结束处理程序了，下一章读者会看到异常过滤程序和异常处理程序更有用，更重要。在继续之前，回顾一下使用结束处理程序的理由：
•简化错误处理，因所有的清理工作都在一个位置并且保证被执行。
•提高程序的可读性。
•使代码更容易维护。
•如果使用得当，具有最小的系统开销。

第24章 异常处理程序和软件异常
异常有两种，CPU引发的异常，就是所谓的硬件异常（hardware exception）。我们还会看到操作系统和应用程序也可以引发相应的异常，称为软件异常（software exception）。
当出现一个硬件或软件异常时，操作系统向应用程序提供机会来考察是什么类型的异常被引发，并能够让应用程序自己来处理异常。下面就是异常处理程序的文法：
__try{ … }
__except(exception filter){
…}
注意--except关键字。每当你建立一个try块，它必须跟随一个finally块或一个except块。一个try块之后不能既有finally块又有except块。但可以在try-except块中嵌套try-finally块，反过来也可以。
24.1通过例子理解异常过滤器和异常处理程序
与结束处理程序（前一章讨论过）不同，异常过滤器（exception filter）和异常处理程序是通过操作系统直接执行的，编译程序在计算异常过滤器表达式和执行异常处理程序方面不做什么事。下面几节的内容举例说明try-except块的正常执行，解释操作系统如何以及为什么计算异常过滤器，并给出操作系统执行异常处理程序中代码的环境。
24.1.1Funcmeister1
这里是一个try-exception块的更具体的例子。
DWORD Funcmeister1(){
DWORD dwTemp;
//1.Do any processing here.
…
__try{
//2.perform some operation.
dwTemp=0;
}
__except(EXCEPTION_EXECUTE_HANDLER){
//Handle an exception;this never executes.
…;
}
//3.continue processing.
Return(dwTemp);
}
在Funcmeister1的try块中，只是把一个0赋给dwTemp变量。这个操作决不会造成异常的引发，所以except块中的代码永远不会执行。注意这与try-finally行为的不同。在dwTemp被设置成0之后，下一个要执行的指令是return语句。
尽管在结束处理程序的try块中使用return、goto、continue和break语句遭到强烈地反对，但在异常处理程序的try块中使用这些语句不会产生速度和代码规模方面的不良影响。这样的语句出现在与except块相结合的try块中不会引起局部展开的系统开销。
24.1.2Funcmeister2
让我们修改这个函数，看会发生什么事情：
DWORD Funcmeister2(){
DWORD dwTemp=0;
//1.Do any processing here.
…
__try{
//2.perform some operation.
dwTemp=5/dwTemp;   //Generates an exception
dwTemp+=10;          //Never executes
}
__except(EXCEPTION_EXECUTE_HANDLER){
//3.Handle an exception.
MessageBeep(0);
…
}
//4.continue processing.
Return(dwTemp);
}
Funcmeister2中，try块中有一个指令试图以0来除5。CPU将捕捉这个事件，并引发一个硬件异常。当引发了这个异常时，系统将定位到except块的开头，并计算异常过滤器表达式的值，过滤器表达式的结果值只能是下面三个标识符之一，这些标识符定义在Windows的Excpt.h文件中（见表24-1）。
表24-1 标识符及其定义
标识符                                    定义为
EXCEPTION_EXECUTE_HANDLER                   1
EXCEPTION_CONTINUE_SEARCH                   0
EXCEPTION_CONTINUE_EXECUTION               -1
下面几节将讨论这些标识符如何改变线程的执行。在阅读这些内容时可参阅图24-1，该图概括了系统如何处理一个异常的情况。
![1 1.JPG](http://copathway.com/ASP/ibr_upload/1263.jpg)
图24-1系统如何处理一个异常
24.2EXCEPTION_EXECUTE_HANDLER
在Funcmeister2中，异常过滤器表达式的值是EXCEPTION_EXECUTE_HANDLER。在这个时候，系统执行一个全局展开（本章后面将讨论），然后执行向except块中代码（异常处理程序代码）的跳转。在except块中代码执行完之后，系统考虑这个要被处理的异常并允许应用程序继续执行。这种机制使Windows应用程序可以抓住错误并处理错误，再使程序继续运行，不需要用户知道错误的发生。但是，当except块执行后，代码将从何处恢复执行？稍加思索，我们就可以想到几种可能性。
第一种可能性是从产生异常的CPU指令之后恢复执行。在Funcmeister2中执行将从对dwTemp加10的指令开始恢复。这看起来像是合理的做法，但实际上，很多程序的编写方式使得当前面的指令出错时，后续的指令不能够继续成功地执行。
第二种可能性是从产生异常的指令恢复执行。
第三种可能性是从except块之后的第一条指令开始恢复执行。这实际是当异常过滤器表达式的值为EXCEPTION_EXECUTE_HANDLER时所发生的事。在except块中的代码结束执行后，控制从except块之后的第一条指令恢复。
24.2.1全局展开
当一个异常过滤器的值为EXCEPTION_EXECUTE_HANDLER时，系统必须执行一个全局展开（globalunwind）。这个全局展开使所有那些在处理异常的try_except块之后开始执行但未完成的try-finally块恢复执行。图24-2是描述系统如何执行全局展开的流程图，在解释后面的例子时，请参阅这个图。
void Func0stimpy1(){
//1.Do any processing here.
…
__try{
//2.call another function.
Func0Ren1()
//code here never executes.
}
__except(/*6.Evaluate filter.*/EXCEPTION_EXECUTE_HANDLER){
//8.after the unwind .the exception handler executes.
Messagebox(…);
}
//9.Exception handled--continue execution.
…
}
Void Func0Ren1(){
DWORD dwTemp=0;
//3.do any processing here.
…
__try{
//4.request permission to access protected data.
Waitforsingleobject(g_hsem,INFINITE);
//5.Modify the data.
//an exception is generated here.
G_dwProtectedData=5/dwTemp;
}
__finally{
//7.global unwind occurs because filter evaluated to EXCEPTION_EXECUTE_HANDLER.
//allow others to use protected data.
releaseSemaphore(g_hSem,1,NULL);
}
//continue processing –never executes.
…
}
函数FuncOStimpy1和FuncOren1结合起来可以解释SEH最令人疑惑的方面。程序中注释的标号给出了执行的次序，我们现在开始做一些分析。
FuncOStimpy1开始执行，进入它的try块并调用FuncORen1。FuncORen1开始执行，进入它的try块并等待获得信标。当它得到信标，FuncORen1试图改变全局数据变量g_dwProtectedData。但由于除以0而产生一个异常。系统因此取得控制，开始搜索一个与except块相配的try块。因为FuncORen1中的try与同一个finally块相配，所以系统再上溯寻找另外的try块。这里，系统在FuncOStimpy1中找到一个try块，并且发现这个try块与一个except块相配。
系统现在计算与FuncOStimpy1中except块相联的异常过滤器的值，并等待返回值。当系统看到返回值是EXCEPTION_EXECUTE_HANDLER的，系统就在FuncORen1的finally块中开始一个全局展开。注意这个展开是在系统执行FuncOStimpy1的except块中的代码之前发生的。对于一个全局展开，系统回到所有未完成的try块的结尾，查找与finally块相配的try块。在这里，系统发现的finally块是FuncORen1中所包含的finally块。
当系统执行FuncORen1的finally块中的代码时，就可以清楚地看到SEH的作用了。FuncORen1释放信标，使另一个线程恢复执行。如果这个finally块中不包含ReleaseSemaphore的调用，则信标不会被释放。
在finally块中包含的代码执行完之后，系统继续上溯，查找需要执行的未完成finally块。在这个例子中已经没有这样的finally块了。系统到达要处理异常的try-except块就停止上溯。这时，全局展开结束，系统可以执行except块中所包含的代码。
结构化异常处理就是这样工作的。SEH比较难于理解，是因为在代码的执行当中与系统牵扯太多。程序代码不再是从头到尾执行，系统使代码段按照它的规定次序执行。这种执行次序虽然复杂，但可以预料。为了更好地理解这个执行次序，我们再从不同的角度来看发生的事情。当一个过滤器返回EXCEPTION_EXECUTE_HANDLER时，过滤器是在告诉系统，线程的指令指针应该指向except块中的代码。但这个指令指针在FuncORen1的try块里。回忆一下第23章，每当一个线程要从一个try-finally块离开时，必须保证执行finally块中的代码。在发生异常时，全局展开就是保证这条规则的机制。
24.2.3暂停全局展开
通过在finally块里放入一个return语句，可以阻止系统去完成一个全局展开。请看下面的代码：
void FuncMonkey(){
__try{
FuncFish()
}
__except(EXCEPTION_EXECUTE_HANDLER){
Messagebeep(0);
}
MessageBox(…);
}
Void FuncFish(){
FuncPheasant();
MessageBox(…);
}
Void FuncPheasant(){
__try{
Strcpy(NULL,NULL);
}
__finally{
Return;
}
}
在FuncPheasant的try块中，当调用strcpy函数时，会引发一个内存存取异常。当异常发生时，系统开始查看是否有一个过滤器可以处理这个异常。系统会发现在FuncMonkey中的异常过滤器是处理这个异常的，并且系统开始一个全局展开。
全局展开启动，先执行FuncPheasant的finally块中的代码。这个代码块包含一个return语句。这个return语句使系统停止做展开，FuncPheasant将实际返回到FuncFish。然后FuncFish又返回到函数FuncMonkey。FuncMonkey中的代码继续执行，调用MessageBox。
注意FuncMonkey的异常块中的代码从不会执行对MessageBeep的调用。FuncPheasant的finally块中的return语句使系统完全停止了展开，继续执行，就像什么也没有发生。
微软专门设计SEH按这种方式工作。程序员有可能希望使展开停止，让代码继续执行下去。这种方法为程序员提供了一种手段。原则上，应该小心避免在finally块中安排return语句。
24.3EXCEPTION_CONTINUE_EXECUTION
我们再仔细考察一下异常过滤器，看它是如何计算出定义在Excpt.h中的三个异常标识符之一的。在“Funcmeister2”一节中，为简单起见，在过滤器里直接硬编码了标识符EXCEPTION_EXECUTE_HANDLER，但实际上可以让过滤器调用一个函数确定应该返回哪一个标识符。这里是另外一个例子。
Char g_szBuffer[100];
Void FunclinRooseveltl(){
Int x=0;
Char *pchBuffer=NULL;
__try{
*pchBuffer=’J’;
X=5/x;
}
__except(oilFilter1(&pchBuffer)){
MessageBox(NULL,”an exception occurred”,NULL,MB_OK);
}
MessageBox(NULL,”Function completed”,NULL,MB_OK);
}
LONG OilFilter1(char **ppchBuffer){
If (*ppchBuffer==NULL){
*ppchBuffer=g_szBuffer;
Return(EXCEPTION_CONTINUE_EXECUTION);
}
Return(EXCEPTION_EXECUTE_HANDLER);
}
这里，首先遇到的问题是在我们试图向pchBuffer所指向的缓冲区中放入一个字母‘J’时发生的。因为这里没有初始化pchBuffer，使它指向全局缓冲区g_szBuffur。pchBuffer实际指向NULL。CPU将产生一个异常，并计算与异常发生的try块相关联的except块的异常过滤器。在except块中，对OilFilter函数传递了pchBuffer变量的地址。
当OilFilter获得控制时，它要查看*ppchBuffer是不是NULL，如果是，把它设置成指向全局缓冲区g_szBuffer。然后这个过滤器返回EXCEPTION_CONTINUE_EXECUTION。当系统看到过滤器的值是EXCEPTION_CONTINUE_EXECUTION时，系统跳回到产生异常的指令，试图再执行一次。这一次，指令将执行成功，字母‘J’将放在g_szBuffer的第一个字节。
随着代码继续执行，我们又在try块中碰到除以0的问题。系统又要计算过滤器的值。这一次，OilFilter看到*ppchBuffer不是NULL，就返回EXCEPTION_EXECUTE_HANDLER，这是告诉系统去执行except块中的代码。这会显示一个消息框，用文本串报告发生了异常。
如你所见，在异常过滤器中可以做很多的事情。当然过滤器必须返回三个异常标识符之一，但可以执行任何其他你想执行的任务。
24.4EXCEPTION_CONTINUE_SEARCH
迄今为止我们看到的例子都很平常。通过增加一个函数调用，让我们来看看其他方面的问题：
Char g_szBuffer[100];
Void FunclinRoosevelt2(){
Char *pchBuffer=NULL;
__try{
FuncAtude2(pchBuffer);
}
__except(oilFilter2(&pchBuffer)){
MessageBox(…);
}
}
Void FuncAtude2(char *sz){
*sz=0;
}
LONG OilFilter2(char **ppchBuffer){
If (*ppchBuffer==NULL){
*ppchBuffer=g_szBuffer;
Return(EXCEPTION_CONTINUE_EXECUTION);
}
Return(EXCEPTION_EXECUTE_HANDLER);
}
当FunclinRoosevelt2执行时，它调用FuncAtude2并传递参数NULL。当FuncAtude2执行时，引发了一个异常。同前面的例子一样，系统计算与最近执行的try块相关联的异常过滤器的值。在这个例子中，FunclinRoosevelt2中的try块是最近执行的try块，所以系统调用OilFilter2函数来求异常过滤器的值——尽管这个异常是在FunclinAtude2函数中产生的。
现在我们让问题变得更复杂一点，在程序中再增加一个try_except块。
Char g_szBuffer[100];
Void FunclinRoosevelt3(){
Char *pchBuffer=NULL;
__try{
FuncAtude3(pchBuffer);
}
__except(oilFilter3(&pchBuffer)){
MessageBox(…);
}
}
Void FuncAtude3(char *sz){
__try{
*sz=0;
}
__except(EXCEPTION_CONTINUE_SEARCH){
//This never executes.
…
}
}
LONG OilFilter3(char **ppchBuffer){
If (*ppchBuffer==NULL){
*ppchBuffer=g_szBuffer;
Return(EXCEPTION_CONTINUE_EXECUTION);
}
Return(EXCEPTION_EXECUTE_HANDLER);
}
现在，当FuncAtude3试图向地址NULL里存放0时，会引发一个异常。但这时将执行FuncAtude3的异常过滤器。FuncAtude3的异常过滤器很简单，只是取值EXCEPTION_CONTINUE_SEARCH。这个标识符是告诉系统去查找前面与一个except块相匹配的try块，并调用这个try块的异常处理器。
因为FuncAtude3的过滤器的值为EXCEPTION_CONTINUE_SEARCH，系统将查找前面的try块（在FunclinRoosevelt3里），并计算其异常过滤器的值，这里异常过滤器是OilFilter3。OilFilter3看到pchBuffer是NULL，将pchBuffer设定为指向全局缓冲区，然后告诉系统恢复执行产生异常的指令。这将使FuncAtude3的try块中的代码执行，但不幸的是，FuncAtude3的局部变量sz没有变化，恢复执行失败的指令只是产生另一个异常。这样，又造成死循环。
前面说过，系统要查找最近执行的与except块相匹配的try块，并计算它的过滤器值。这就是说，系统在查找过程当中，将略过那些与finally块相匹配而不是与except块相匹配的try块。
这样做的理由很明显：finally块没有异常过滤器，系统没有什么要计算的。如果前面例子中FuncAtude3包含一个finally块而不是except块，系统将在一开始就通过FunclinRoosevelt3的OilFilter3计算异常过滤器的值。
第25章提供有关EXCEPTION_CONTINUE_SEARCH的更多信息。
24.5GetExceptionCode
一个异常过滤器在确定要返回什么值之前，必须分析具体情况。例如，异常处理程序可能知道发生了除以0引起的异常时该怎么做，但是不知道该如何处理一个内存存取异常。异常过滤器负责检查实际情况并返回适当的值。
可用内部函数GetExceptionCode，他返回一个值，该值指出所发生异常的种类：
DWORD GetExceptionCode();
下面列出所有预定义的异常和相应的含意，这些内容取自PlatformSDK文档。这些异常标识符可以在WinBase.h文件中找到。我们对这些异常做了分类。
1.与内存有关的异常
•EXCEPTION_ACCESS_VIOLATION。线程试图对一个虚地址进行读或写，但没有做适当的存取。这是最常见的异常。
•EXCEPTION_DATATYPE_MISALIGNMENT。线程试图读或写不支持对齐（alignment）的硬件上的未对齐的数据。例如，16位数值必须对齐在2字节边界上，32位数值要对齐在4字节边界上。
•EXCEPTION_ARRAY_BOUNDS_EXCEEDED。线程试图存取一个越界的数组元素，相应的硬件支持边界检查。
•EXCEPTION_IN_PAGE_ERROR。由于文件系统或一个设备启动程序返回一个读错误，造成不能满足要求的页故障。
•EXCEPTION_GUARD_PAGE。一个线程试图存取一个带有PAGE_GUARD保护属性的内存页。该页是可存取的，并引起一个EXCEPTION_GUARD_PAGE异常。
•EXCEPTION_STACK_OVERFLOW。线程用完了分配给它的所有栈空间。
•EXCEPTION_ILLEGAL_INSTRUCTION。线程执行了一个无效的指令。这个异常由特定的CPU结构来定义；在不同的CPU上，执行一个无效指令可引起一个陷井错误。
•EXCEPTION_PRIV_INSTRUCTION。线程执行一个指令，其操作在当前机器模式中不允许。
2.与异常相关的异常
•EXCEPTION_INVALID_DISPOSITION。一个异常过滤器返回一值，这个值不是EXCEPTION_EXECUTE_HANDLER、EXCEPTION_CONTINUE_SEARCH、EXCEPTION_CONTINUE_EXECUTION三者之一。
•EXCEPTION_NONCONTINUABLE_EXCEPTION。一个异常过滤器对一个不能继续的异常返回EXCEPTION_CONTINUE_EXECUTION。
3.与调试有关的异常
•EXCEPTION_BREAKPOINT。遇到一个断点。
•EXCEPTION_SINGLE_STEP。一个跟踪陷井或其他单步指令机制告知一个指令已执行完毕。
•EXCEPTION_INVALID_HANDLE。向一个函数传递了一个无效句柄。
4.与整数有关的异常
•EXCEPTION_INT_DIVIDE_BY_ZERO。线程试图用整数0来除一个整数
•EXCEPTION_INT_OVERFLOW。一个整数操作的结果超过了整数值规定的范围。
5.与浮点数有关的异常
•EXCEPTION_FLT_DENORMAL_OPERAND。浮点操作中的一个操作数不正常。不正常的值是一个太小的值，不能表示标准的浮点值。
•EXCEPTION_FLT_DIVIDE_BY_ZERO。线程试图用浮点数0来除一个浮点。
•EXCEPTION_FLT_INEXACT_RESULT。浮点操作的结果不能精确表示成十进制小数。
•EXCEPTION_FLT_INVALID_OPERATION。表示任何没有在此列出的其他浮点数异常。
•EXCEPTION_FLT_OVERFLOW。浮点操作的结果超过了允许的值。
•EXCEPTION_FLT_STACK_CHECK。由于浮点操作造成栈溢出或下溢。
•EXCEPTION_FLT_UNDERFLOW。浮点操作的结果小于允许的值。
内部函数GetExceptionCode只能在一个过滤器中调用（--except之后的括号里），或在一个异常处理程序中被调用。
但是，不能在一个异常过滤器函数里面调用GetExceptionCode。编译程序会捕捉这样的错误。
异常代码遵循在WinError.h文件中定义的有关错误代码的规则。每个DWORD被划分如表24-2所示。
表24-2一个错误代码的构成
位       31-30           29         28         27-16         15-0
内容    严重性系数   微软/客户    保留        设备代码      异常代码
意义     0＝成功     0=微软定义   必须为0     微软定义      微软／客户定义
1＝信息    的代码              (见表24-3)
2＝警告    1=客户定义
3＝错误    的代码
目前，微软定义了下面一些设备代码(见表24-3)。
表24-3设备代码及其值
设备代码                                   值
FACILITY_NULL                            0
FACILITY_RPC                            1
FACILITY_DISPATCH                        2
FACILITY_STORAGE                        3
FACILITY_ITF                            4
FACILITY_WIN32                            7
FACILITY_WINDOWS                        8
FACILITY_SECURITY                        9
FACILITY_NULL0FACILITY_CONTROL            10
FACILITY_RPC1FACILITY_CERT                11
FACILITY_DISPATCH2FACILITY_INTERNET        12
FACILITY_STORAGE3FACILITY_MEDIASERVER    13
FACILITY_ITF4FACILITY_MSMQ                14
FACILITY_WIN327FACILITY_SETUPAPI        15
FACILITY_WINDOWS8FACILITY_SCARD            16
FACILITY_SECURITY9FACILITY_COMPLUS        17
我们将EXCEPTION_ACCESS_VIOLATION异常代码拆开来，看各位(bit)都是什么。在WinBase.h中找到EXCEPTION_ACCESS_VIOLATION，它的值为0xC0000005：
C    0    0    0    0    0    0    5   （十六进制）
1100 0000 0000 0000 0000 0000 0000 0101（二进制）
第30位和第31位都是1，表示存取异常是一个错误（线程不能继续运行）。第29位是0，表示Microsoft已经定义了这个代码。第28位是0，留待后用。第16位至27位是0，代表FACILITY_NULL（存取异常可发生在系统中任何地方，不是使用特定设备才发生的异常）。第0位到第15位包含一个数5，表示微软将存取异常这种异常的代码定义成5。
24.6GetExceptionInformation
当一个异常发生时，操作系统要向引起异常的线程的栈里压入三个结构，这三个结构是：EXCEPTION_RECORD结构、CONTEXT结构和EXCEPTION_POINTERS结构。
EXCEPTION_RECORD结构包含有关已发生异常的独立于CPU的信息，CONTEXT结构包含已发生异常的依赖于CPU的信息。EXCEPTION_POINTERS结构只有两个数据成员，二者都是指针，分别指向被压入栈的EXCEPTION_RECORD和CONTEXT结构：
Typedef struct _EXCEPTION_POINTERS{
Pexception_record exceptionrecord;
Pcontext contextrecord;
}EXCEPTON_POINTERS,*PEXCEPTION_POINTERS;
为了取得这些信息并在你自己的程序中使用这些信息，需要调用GetExceptionInformation函数：PEXCEPTION_POINTERS GetExceptionInformation();
这个内部函数返回一个指向EXCEPTION_POINTERS结构的指针。
关于GetExceptionInformation函数，要记住的最重要事情是它只能在异常过滤器中调用，因为仅仅在处理异常过滤器时，CONTEXT、EXCEPTION_RECORD和EXCEPTION_POINTERS才是有效的。一旦控制被转移到异常处理程序，栈中的数据就被删除。
如果需要在你的异常处理程序块里面存取这些异常信息（虽然很少有必要这样做），必须将EXCEPTION_POINTERS结构所指向的CONTEXT数据结构和／或EXCEPTION_RECORD数据结构保存在你所建立的一个或多个变量里。
24.7软件异常
迄今为止，我们一直在讨论硬件异常，也就是CPU捕获一个事件并引发一个异常。在代码中也可以强制引发一个异常。这也是一个函数向它的调用者报告失败的一种方法。传统上，失败的函数要返回一些特殊的值来指出失败。函数的调用者应该检查这些特殊值并采取一种替代的动作。通常，这个调用者要清除所做的事情并将它自己的失败代码返回给它的调用者。这种错误代码的逐层传递会使源程序的代码变得非常难于编写和维护。
另外一种方法是让函数在失败时引发异常。用这种方法，代码更容易编写和维护，而且也执行得更好，因为通常不需要执行那些错误测试代码。实际上，仅当发生失败时也就是发生异常时才执行错误测试代码。
但令人遗憾的是，许多开发人员不习惯于在错误处理中使用异常。这有两方面的原因。第一个原因是多数开发人员不熟悉SEH。即使有一个程序员熟悉它，但其他程序员可能不熟悉它。如果一个程序员编写了一个引发异常的函数，但其他程序员并不编写SEH框架来捕获这个异常，那么进程就会被操作系统结束。
开发人员不使用SEH的第二个原因是它不能移植到其他操作系统。许多公司的产品要面向多种操作系统，因此希望有单一的源代码作为产品的基础，这是可以理解的。SEH是专门针对Windows的技术。
本段讨论通过异常返回错误有关的内容。首先，让我们看一看WindowsHeap函数，例如HeapCreate、heapAlloc等。回顾第18章的内容，我们知道这些函数向开发人员提供一种选择。通常当某个堆（heap）函数失败，它会返回NULL来指出失败。然而可以对这些堆函数传递HEAP_GENERATE_EXCEPTIONS标志。如果使用这个标志并且函数失败，函数不会返回NULL，而是由函数引发一个STATUS_NO_MEMORY软件异常，程序代码的其他部分可以用SEH框架来捕获这个异常。
如果想利用这个异常，可以编写你的try块，好像内存分配总能成功。如果内存分配失败，可以利用except块来处理这个异常，或通过匹配try块与finally块，清除函数所做的事。这非常方便。
程序捕获软件异常采取的方法与捕获硬件异常完全相同。也就是说，前一章介绍的内容可以同样适用于软件异常。
本节重讨论如何让你自己的函数引发软件异常，作为指出失败的方法。实际上，可以用类似于微软实现堆函数的方法来实现你的函数：让函数的调用者传递一个标志，告诉函数如何指出失败。
引发一个软件异常很容易，只需要调用RaiseException函数：
VOID RaiseException{
DWORD dwExceptionCode,
DWORD dwExceptionFlags,
DWORD nNumberOfarguments,
CONST ULONG_PTR *parguments);
第一个参数dwExceptionCode是标识所引发异常的值。HeapAlloc函数对这个参数设定STATUS_NO_MEMORY。如果程序员要定义自己的异常标识符，应该遵循标准Windows错误代码的格式，像WinError.h文件中定义的那样。参阅表24-1。
如果要建立你自己的异常代码，要填充DWORD的4个部分：
•第31位和第30位包含严重性系数(severity)。
•第29位是1（0表示微软建立的异常，如HeapAlloc的STATUS_NO_MEMORY）。
•第28位是0。
•第27位到16位是某个微软定义的设备代码。
•第15到0位是一个任意值，用来标识引起异常的程序段。
RaiseException的第二个参数dwExceptionFlags，必须是0或EXCEPTION_NONCONTINUABLE。本质上，这个标志是用来规定异常过滤器返回EXCEPTION_CONTINUE_EXECUTION来响应所引发的异常是否合法。如果没有向RaiseException传递EXCEPTION_NONCONTINUABLE参数值，则过滤器可以返回EXCEPTION_CONTINUE_EXECUTION。正常情况下，这将导致线程重新执行引发软件异常的同一CPU指令。但微软已做了一些动作，所以在调用RaiseException函数之后，执行会继续进行。
如果你向RaiseException传递了EXCEPTION_NONCONTINUABLE标志，你就是在告诉系统，你引发异常的类型是不能被继续执行的。这个标志在操作系统内部被用来传达致命（不可恢复）的错误信息。另外，当HeapAlloc引发STATUS_NO_MEMORY软件异常时，它使用EXCEPTION_NONCONTINUABLE标志来告诉系统，这个异常不能被继续。意思就是没有办法强制分配内存并继续运行。
如果一个过滤器忽略EXCEPTION_NONCONTINUABLE并返回EXCEPTION_CONTINUE_EXECUTION，系统会引发新的异常：EXCEPTION_NONCONTINUABLE_EXCEPTION。
当程序在处理一个异常的时候，有可能又引发另一个异常。比如说，一个无效的内存存取有可能发生在一个finally块、一个异常过滤器、或一个异常处理程序里。当发生这种情况时，系统压栈异常。回忆一下GetExceptionInformation函数。这个函数返回EXCEPTION_POINTERS结构的地址。EXCEPTION_POINTERS的ExceptionRecord成员指向一个EXCEPTION_RECORD结构，这个结构包含另一个ExceptionRecord成员。这个成员是一个指向另外的EXCEPTION_RECORD的指针，而这个结构包含有关以前引发异常的信息。
通常系统一次只处理一个异常，并且ExceptionRecord成员为NULL。然而如果处理一个异常的过程中又引发另一个异常，第一个EXCEPTION_RECORD结构包含有关最近引发异常的信息，并且这个EXCEPTION_RECORD结构的ExceptionRecord成员指向以前发生的异常的EXCEPTION_RECORD结构。如果增加的异常没有完全处理，可以继续搜索这个EXCEPTION_RECORD结构的链表，来确定如何处理异常。
RaiseException的第三个参数nNumberOfArguments和第四个参数pArguments，用来传递有关所引发异常的附加信息。通常，不需要附加的参数，只需对pArguments参数传递NULL，这种情况下，RaiseException函数忽略nNumberOfArguments参数。如果需要传递附加参数，nNumberOfArguments参数必须规定由pArguments参数所指向的ULONG_PTR数组中的元素数目。这个数目不能超过EXCEPTION_MAXIMUM_PARAMETERS，EXCEPTION_MAXIMUM_PARAMETERS在WinNT.h中定义成15。
在处理这个异常期间，可使异常过滤器参照EXCEPTION_RECORD结构中的NumberParameters和ExceptionInformation成员来检查nNumberOfArguments和pArguments参数中的信息。
你可能由于某种原因想在自己的程序中产生自己的软件异常。例如，你可能想向系统的事件日志发送通知消息。每当程序中的一个函数发现某种问题，你可以调用RaiseException并让某些异常处理程序上溯调用树查看特定的异常，或者将异常写到日志里或弹出一个消息框。你还可能想建立软件异常来传达程序内部致使错误的信息。

第25章未处理异常和C++异常
前一章讨论了当一个异常过滤器返回EXCEPTION_CONTINUE_SEARCH时会发生什么事情。返回EXCEPTION_CONTINUE_SEARCH是告诉系统继续上溯调用树，去寻找另外的异常过滤器。但是当每个过滤器都返回EXCEPTION_CONTINUE_SEARCH时会出现什么情况呢？
在这种情况下，就出现了所谓的“未处理异常”（Unhandledexception）。
在第6章里我们已经知道，每个线程开始执行，实际上是利用Kernel32.dll中的一个函数来调用BaseProcessStart或BaseThreadStart。这两个函数实际是一样的，区别在于一个函数用于进程的主线程（Primarythread）：
VOID BaseProcessStart(PPROCESS_START_ROUTINE pfnStartAddr){
__try{
ExitThread((pfnStartAddr)());
}
__except(UnhandledExceptionFilter(GetExceptionInformation())){
ExitProcess(GetExceptionCode());
}
//NOTE:we never get here
}
另一个函数用于进程的所有辅助线程（Secondarythread）：
VOID BaseThreadStart(PPROCESS_START_ROUTINE pfnStartAddr,PVOID pvParam){
__try{
ExitThread((pfnStartAddr)(pvParam));
}
__except(UnhandledExceptionFilter(GetExceptionInformation())){
ExitProcess(GetExceptionCode());
}
//NOTE:we never get here
}
注意这两个函数都包含一个SEH框架。每个函数都有一个try块，并从这个try块里调用主线程或辅助线程的进入点函数。所以，当线程引发一个异常，所有过滤器都返回EXCEPTION_CONTINUE_SEARCH时，将会自动调用一个由系统提供的特殊过滤器函数：UnhandledExceptionFilter。这个函数负责显示一个消息框，指出有一个进程的线程存在未处理的异常，并且能让用户结束或调试这个进程。
在消息框中的异常描述之后，又提供给用户两种选择。第一种选择是点击OK按钮，这将导致UnhandledExceptionFilter返回EXCEPTION_EXECUTE_HANDLER。这又引起全局展开的发生，所有的finally块都要执行，然后BaseProcessStart或BaseThreadStart中的处理程序执行。
这两个处理程序都叫ExitProcess，意思是退出进程，这就是程序结束的原因。注意进程的退出代码就是异常代码。还要注意是进程的线程废（kill）了进程本身，而不是操作系统。这也意味着程序员可以控制这种行为并可以改变它。
第二种选择是点击Cancel按钮。在这里程序员的梦想成真。当点击Cancel按钮时，UnhandledExceptionFilter试图加载一个调试程序，并将这个调试程序挂接在进程上。通过将调试程序附在进程上，可以检查全局、局部和静态变量的状态，设置断点，检查调用树，重新启动进程，以及调试一个进程时可以做的任何事情。
这里真正的好处是，你可以在程序运行当中错误发生时就处理错误。在其他操作系统中，必须在调试程序调用程序时才能对其进行调试。在那些操作系统中，如果一个进程中发生了一个异常，必须结束这个进程，启动调试程序，调用程序，再使用调试程序。这样，你必须重新产生这个错误，才有可能去修正它。但谁能记住问题最初发生时的各种条件和各变量的值？按这种方式解决程序错误问题非常困难。将调试程序动态挂接在运行中的进程上，是Windows最好的特性之一。
Windows2000本书着重讨论用户方式（user-mode）的程序开发。但对于运行在内核方式（kernel-mode）的线程引发的未处理异常会造成什么情况，读者也许会感兴趣。内核方式中的异常同用户方式中的异常是按同样方式处理的。如果一个低级虚拟内存函数产生一个异常，系统查找是否有内核方式异常过滤器准备处理这个异常。如果系统找不到一个异常过滤器来处理这个异常，则异常就是未处理的。对于内核方式的异常，未处理异常是在操作系统中或（更可能）在设备驱动程序中，而不是在应用程序中。这样一个未处理异常表示一个严重的程序错误（bug）！
如果一个未处理异常发生在内核方式，让系统继续运行是不安全的。所以系统在这种情况下不去调用UnhandledExceptionFilter函数，而是显示所谓的蓝屏死机（BlueScreenofDeath）。显示画屏切换到只包含文本的蓝屏视频方式，并且计算机被停机（balt）。显示的文本告诉是哪个设备驱动程序被加载，并且该模块中包含有引发未处理异常的代码。用户应该记下这些信息并送交微软或设备驱动程序的厂商，以便修复这个错误。因为计算机被挂起，要想再做其他事情就必须重新启动计算机，所有未保存的工作都丢失了。
25.1即时调试
随时将调试程序连接到任何进程的能力称为即时调试（Just-in-timeDebugging）。这里我们对它如何工作稍加说明：当程序员点击Cancel按钮，就是告诉UnhandledExceptionFilter函数对进程进行调试。
在内部，UnhandledExceptionFilter调用调试程序，这需要查看下面的注册表子关键字：
HKEY_LOCAL_MACHINE/SOFTWARE/Microsoft/Windows NT/CurrentVersion/AeDebug
在这个子关键字里，有一个名为Debugger的数值，在安装VisualStudio时被设置成下面的值：
"C:/Program Files/Microsoft Visual Studio/Common/MSDev98/Bin/msdev.exe" -p %ld -e %ld
Windows98在Windows98中，这些值不是存放在注册表中，而是存放在Win.ini文件中。
这一行代码是告诉系统要将哪一个程序（这里是MSDev.exe）作为调试程序运行。当然也可以选择其他调试程序。UnhandledExceptionFilter还在这个命令行中向调试程序传递两个参数。第一个参数是被调试进程的ID。第二个参数规定一个可继承的手工复位事件，这个事件是由UnhandledExceptionFilter按无信号状态建立的。厂商必须实现他们的调试程序，这样才能认识指定进程ID和事件句柄的-p和-e选项。
在进程ID和事件句柄都合并到这个串中之后，UnhandledExceptionFilter通过调用CreateProcess来执行调试程序。这时，调试程序进程开始运行并检查它的命令行参数。如果存在-p选项，调试程序取得进程ID，并通过调用DebugActiveProcess将自身挂接在该进程上。
BOOL DebugActiveProcess(DWORD dwProcessID);
一旦调试程序完成自身的挂接，操作系统将被调试者（debuggee）的状态通报给调试程序。
在调试程序完全初始化之后，它要再检查它的命令行，找-e选项。如果该选项存在，调试程序取得相应的事件句柄并调用SetEvent。调试程序可以直接使用事件的句柄值，因为事件句柄具有创建的可继承性，并且被调试进程对UnhandledExceptionFilter函数的调用也使调试程序进程成为一个子进程。
设定这个事件将唤醒被调试进程的线程。被唤醒的线程将有关未处理异常的信息传递给调试程序。调试程序接收这些通知并加载相应的源代码文件，再将自身放在引发异常的指令位置上。
还有，不必在调试进程之前等待异常的出现。可以随时将一个调试程序连接在任何进程上，只需运行“MSDEV-pPID”，其中PID是要调试的进程的ID。实际上，利用Windows2000 TaskManager，做这些事很容易。当观察Process标记栏时，可以选择一个进程，点击鼠标右键，并选择Debug菜单选项。这将引起TaskManager去查看前面讨论过的注册表子关键字，调用CreateProcess，并传递所选定的进程的ID作为参数。在这里，TaskManager为事件句柄传送0值。
25.2关闭异常消息框
有时候，在异常发生时，你可能不想在屏幕上显示异常消息框。可以使用几种不同的方法来防止这种消息框的出现。
25.2.1强制进程终止运行
为防止UnhandledExceptionFilter显示异常消息框，可以调用下面的SetErrorModel函数，并向它传递一个SEM_NOGPFAULTERRORBOX标识符：
UINT SetErrorMode(UINT fuErrorMode);
然后，当调用UnhandledExceptionFilter函数来处理异常时，看到已经设置了这个标志，就会立即返回EXCEPTION_EXECUTE_HANDLER。这将导致全局展开并执行BaseProcessStart或BaseThreadStart中的处理程序。该处理程序结束进程。
在这种方式下，用户完全没有得到警告，程序只是自行消失。
25.2.2包装一个线程函数
使用另外一种办法也可以避免出现这个消息框，就是针对主线程进入点函数（main、wmain、WinMain或wWinMain）的整个内容安排一个try-except块。保证异常过滤器的结果值总是EXCEPTION_EXECUTE_HANDLER，这样就保证异常得到处理，防止了系统再调用UnhandledExceptionFilter函数。
在你的异常处理程序中，你可以显示一个对话框，在上面显示一些有关异常的诊断信息。用户可以记录下这些信息，并通报给你公司的客户服务部门，以便能够找到程序的问题根源。你应该建立这个对话框，这样用户只能结束程序而不能调用调试程序。
这种方法的缺点是它只能捕捉进程的主线程中发生的异常。如果其他线程在运行，并且其中有一个线程发生了一个未处理异常，系统就要调用内部的UnhandledExceptionFilter函数。为了改正这一点，需要在所有的辅助线程进入点函数中包含try-except块。
25.2.3包装所有的线程函数
Windows还提供另外一个函数，SetUnhandledExceptionFilter，利用它可以按SEH格式包装所有的线程函数：
PTOP_LEVEL_EXCEPTION_FILTER SetUnhandledExceptionFilter(
PTOP_LEVEL_EXCEPTION_FILTER pTopLevelExceptionFilter);
在进程调用这些函数之后，进程的任何线程中若发生一个未处理的异常，就会导致调用程序自己的异常过滤器。需要将这个过滤器的地址作为参数传递给SetUnhandledExceptionFilter。
过滤器函数原型必须是下面的样子：
LONG UnhandledExceptionFilter(PEXCEPTION_POINTERS     pExceptionInfo);
你可能会注意到这个函数同UnhandledExceptionFilter函数的形式是一样的。程序员可以在自己的异常过滤器中执行任何想做的处理，但要返回三个EXCEPTION_*标识符中的一个。表25-1给出了当返回各标识符时所发生的事。
表25-1返回标识符时的情况
标识符                                        出现的情况
EXCEPTION_EXECUTE_HANDLER        进程只是结束，因系统在其异常处理程序块中没有执行任何动作
EXCEPTION_CONTINUE_EXECUTION    从引起异常的指令处继续执行。可以参照PEXCEPTION_POINTERS参数修改异常信息
EXCEPTION_CONTINUE_SEARCH        执行正规的WindowsUnhandledExceptionFilter函数

为了使UnhandledExceptionFilter函数再成为默认的过滤器，可以调用SetUnhandledExceptionFilter并传递NULL给它。而且，每当设置一个新的未处理的异常过滤器时，SetUnhandledExceptionFilter就返回以前安装的异常过滤器的地址。如果UnhandledExceptionFilter是当前所安装的过滤器，则这个返回的地址就是NULL。如果你自己的过滤器要返回EXCEPTION_CONTINUE_SEARCH，你就应该调用以前安装的过滤器，其地址通过SetUnhandledExceptionFilter函数返回。
25.2.4自动调用调试程序
现在再介绍关闭UnhandledExceptionFilter消息框的最后一种方法。在前面提到的同一个注册表子关键字里，还有另外一个数据值，名为Auto。这个值用来规定UnhandledExceptionFilter是应该显示消息框，还是仅启动调试程序。如果Auto设置成1，UnhandledExceptionFilter就不显示消息框向用户报告异常，而是立即调用调试程序。如果Auto子关键设置成0，UnhandledExceptionFilter就显示异常消息框，并按前面描述的那样操作。
25.3程序员自己调用UnhandledExceptionFilter
UnhandledExceptionFilter函数是一个公开的、文档完备的Windows函数，程序员可以直接在自己的代码中调用这个函数。
25.4UnhandledExceptionFilter函数的一些细节
下面所列的步骤详细描述了UnhandledExceptionFilter函数的内部执行情况：
1)如果发生一个存取违规并且是由于试图写内存（相对于读内存）引起的，系统要查看你是不是要修改一个.exe模块或DLL模块中的资源。默认时，资源是（而且应该是）只读的。试图修改资源会引起存取异常。然而16位Windows允许修改资源，从兼容性考虑，32位和64位Windows也应该允许修改资源。所以当想要修改资源的时候，UnhandledExceptionFilter调用VirtualProtect，将资源页上的保护改成PAGE_READWRITE，并返回EXCEPTION_CONTINUE_EXECUTION。
2)如果你已经调用SetUnhandledExceptionFilter指定了你自己的过滤器，UnhandledExceptionFilter就调用你自己的过滤器函数。如果你自己的过滤器函数返回EXCEPTION_EXECUTE_HANDLER或EXCEPTION_CONTINUE_EXECUTION，UnhandledExceptionFilter就将这个值返回给系统。如果你没有设置你自己的未处理异常过滤器，或者你的未处理异常过滤器返回EXCEPTION_CONTINUE_SEARCH，转到第3步继续处理。
Windows98Windows98有这样的错误（bug）：如果进程不在调试中，系统只调用程序自己的未处理异常过滤器函数。
3)如果你的进程是在调试程序下运行的，就返回EXCEPTION_CONTINUE_SEARCH。你可能会对此感到不解，因系统已经为线程执行了最高层的try或except框架，再往高层已经没有其他的异常过滤器可搜索。当系统看到最高层过滤器返回EXCEPTION_CONTINUE_SERCH时，系统知道要同调试程序联系并告诉调试程序，被调试程序只是有一个未处理异常。作为回答，调试程序显示一个消息框并允许你调试进程（注意，IsDebuggerPresent函数用来确定一个进程是否正在被调试）。
4)如果进程中的一个线程以SEM_NOGPFAULTERRORBOX标志为参数调用SetErrorMode，UnhandledExceptionFilter就返回EXCEPTION_EXECUTE_HANDLER。
5)如果进程在一个作业（job）里并且作业的限制信息设定了JOB_OBJECT_LIMIT_DIE_ON_UNHANDLED_EXCEPTION标志，则UnhandledExceptionFilter返回EXCEPTION_EXECUTE_HANDLER。
Windows98Windows98不支持作业，所以这一步略去。
6)UnhandledExceptionFilter查阅注册表并取出Auto值。如果这个值是1，跳到第7步。如果这个值是0，向用户显示一个消息框。这个消息框指出引发了什么异常。如果注册表子关键字也包含这个Debugger值，消息框有OK按钮和Cancel按钮。如果注册表子关键字没有Debugger值，消息框只包含OK按钮。如果用户点击OK按钮，UnhandledExceptionFilter返回EXCEPTION_EXECUTE_HANDLER。如果Cancel按钮可用并且用户按了这个按钮，转到第7步继续处理。
Windows98 在Windows98中，这些值不是保存在注册表里，而是保存在Win.ini文件里。
7)UnhandledExceptionFilter现在要产生调试程序。它首先调用CreateEvent建立一个无信号的、手工复位的事件。这个事件的句柄是可继承的。然后它从注册表中取出Debugger值，调用sprintf把它粘贴到进程ID（通过调用GetCurrentProcessID函数得到）和事件句柄里。
STARTUPINFO的lpDesktop成员也设置成“Winsta0//Default”，这样调试程序就出现在交互式桌面上。调用CreateProcess，其中fInheritHandles参数设置成TRUE，这个函数再调用调试程序进程并允许它继承事件对象的句柄。UnhandledExceptionFilter通过以事件句柄为参数调用WaitForSingleObjectEx，等待调试程序初始化。注意这里是用WaitForSingleObjectEx函数而不是WaitForSingleObject函数，所以线程是在可报警状态等待。这样就可以处理线程的任何排队的同步过程调用（APC）。
8)当调试程序完成初始化时，就设置事件句柄，这将唤醒UnhandledExceptionFilter中的线程。现在进程就在调试程序之下运行，UnhandledExceptionFilter返回EXCEPTION_CONTINUE_SEARCH。注意这就是第3步所发生的事。
25.5异常与调试程序
MicrosoftVisualC++调试程序支持调试异常。当一个进程的线程引起一个异常，操作系统立即通知调试程序（如果挂接了一个调试程序）。这个通知被称作最初机会通知（first-chancenotificontion）。正常情况下，调试程序要响应最初机会通知，告诉线程去搜索异常过滤器。如果所有的异常过滤器都返回EXCEPTION_CONTINUE_SEARCH，操作系统再通知调试程序，称为最后机会通知(last-chancenotification)。使用这两种通知是为了使软件开发人员对调试异常有更多的控制能力。
25.6C++异常与结构性异常的对比
笔者常碰到开发人员询问，在开发程序时应该使用结构化异常处理，还是使用C++异常处理？本节对此作出回答。
首先要提醒读者，SEH是可用于任何编程语言的操作系统设施，而异常处理只能用于编写C++代码。如果你在编写C++程序，你应该使用C++异常处理而不是结构化异常处理。理由是C++异常处理是语言的一部分，编译器知道C++类对象是什么。也就是说编译器能够自动生成代码来调用C++对象析构函数，保证对象的清除。
但是也应该知道，MicrosoftVisualC++编译器已经利用操作系统的结构化异常处理实现了C++异常处理。所以当你建立一个C++try块时，编译器就生成一个SEH__try块。一个C++catch测试变成一个SEH异常过滤器，并且catch中的代码变成SEH__except块中的代码。实际上，当你写一条C++throw语句时，编译器就生成一个对Windows的RaiseException函数的调用。用于throw语句的变量传递给RaiseException作为附加的参数。
注意由于C++异常在内部是由结构性异常实现的，所以可以在一个程序中使用两种机制。例如，当引起存取违规时，我乐意使用虚拟内存来提交存储区。而C++语言完全不支持这种类型的再恢复异常处理（resumptiveexceptionhandling）。我可以在我的代码的某些部分使用结构化处异常处理。在这些部分需要利用这种结构的优点，可使我自己的__except过滤器返回EXCEPTION_CONTINUE_EXECUTION。对于代码的其他部分，如果不要求这种再恢复异常处理，我还是使用C++异常处理。

第26章窗口消息
本章介绍MicrosoftWindows的消息系统是如何支持带有图形用户界面的应用程序的。在设计Windows2000或Windows98所使用的窗口系统时，微软有两个主要目标：
•尽可能保持与过去16位Windows的兼容性，便于开发人员移植他们已有的16位Windows程序。
•使窗口系统强壮，一个线程不会对系统中其他线程产生不利影响。
但是，这两个目标是直接相互冲突的。在16位Windows系统中，向窗口发送一个消息总是按同步方式执行的：发送程序要在接受消息的窗口完全处理完消息之后才能继续运行。这通常是一个所期望的特性。但是，如果接收消息的窗口花很长的时间来处理消息或者出现挂起，则发送程序就不能再执行。这意味着系统是不强壮的。
这种冲突给微软的设计人员带来了一定的困难。他们的解决方案是两个相互冲突目标之间的出色折衷方案。如果在阅读本章时记住这两个目标，你就会更多地理解微软为什么会做出这样的设计。
26.1线程的消息队列
前面已经说过，Windows的一个主要目标是为程序的运行提供一个强壮的环境。为实现这个目标，要保证每个线程运行在一个环境中，在这个环境中每个线程都相信自己是唯一运行的线程。更确切地说，每个线程必须有完全不受其他线程影响的消息队列。而且，每个线程必须有一个模拟环境，使线程可以维持它自己的键盘焦点（keyboardfocus）、窗口激活、鼠标捕获等概念。
当一个线程第一次被建立时，系统假定线程不会被用于任何与用户相关的任务。这样可以减少线程对系统资源的要求。但是，一旦这个线程调用一个与图形用户界面有关的函数（例如检查它的消息队列或建立一个窗口），系统就会为该线程分配一些另外的资源，以便它能够执行与用户界面有关的任务。特别是，系统分配一个THREADINFO结构，并将这个数据结构与线程联系起来。
这个THREADINFO结构包含一组成员变量，利用这组成员，线程可以认为它是在自己独占的环境中运行。THREADINFO是一个内部的、未公开的数据结构，用来指定线程的登记消息队列（posted-messagequeue）、发送消息队列（send-messagequeue）、应答消息队列（reply-messagequeue）、虚拟输入队列（virtualized-inputqueue）、唤醒标志（wakeflag）、以及用来描述线程局部输入状态的若干变量。图26-1描述了THREADINFO结构和与之相联系的三个线程。
![26 26.JPG](http://copathway.com/ASP/ibr_upload/1266.jpg)
图26-1三个线程及相应的THREADINFO结构
这个THREADINFO结构是窗口消息系统的基础，在阅读下面各节内容时，应该参考该图。
26.2将消息发送到线程的消息队列中
当线程有了与之相联系的THREADINFO结构时，线程就有了自己的消息队列集合。如果一个进程建立了三个线程，并且所有这些线程都调用CreateWindow，则将有三个消息队列集合。消息被放置在线程的登记消息队列中，这要通过调用PostMessage函数来完成：
BOOL PostMessage(
HWND hwnd,
UINT uMsg,
WPARAM wParam,
LPARAM lParam);
当一个线程调用这个函数时，系统要确定是哪一个线程建立了用hwnd参数标识的窗口。然后系统分配一块内存，将这个消息参数存储在这块内存中，并将这块内存增加到相应线程的登记消息队列中。并且，这个函数还设置QS_POSTMESSAGE唤醒位（后面会简单讨论）。函数PostMessage在登记了消息之后立即返回，调用该函数的线程不知道登记的消息是否被指定窗口的窗口过程所处理。实际上，有可能这个指定的窗口永远不会收到登记的消息。如果建立这个特定窗口的线程在处理完它的消息队列中的所有消息之前就结束了，就会发生这种事。
还可以通过调用PostThreadMessage将消息放置在线程的登记消息队列中。
BOOL PostThreadMessage(
DWORD,dwThreadId,
UINT uMsg,
WPARAM wParam,
LPARAM lParam);
注意可以通过调用GetWindowsThreadProcessId来确定是哪个线程建立了一个窗口。
DWORD GetWindowsThreadProcessId(
  HWND hwnd,
  PDWORD pdwProcessid);
这个函数返回线程的ID，这个线程建立了hwnd参数所标识的窗口。线程ID在全系统范围内是唯一的。还可以通过对pdwProcessId参数传递一个DWORD地址来获取拥有该线程的进程ID，这个进程ID在全系统范围内也是唯一的。通常，我们不需要进程ID，只须对这个参数传递一个NULL。
PostThreadMessage函数所期望的线程由第一个参数dwThreadId所标记。当消息被设置到队列中时，MSG结构的hwnd成员将设置成NULL。当程序要在主消息循环中执行一些特殊处理时要调用这个函数。
要对线程编写主消息循环以便在GetMessage或PeekMessage取出一个消息时，主消息循环代码检查hwnd是否为NULL，并检查MSG结构的msg成员来执行特殊的处理。如果线程确定了该消息不被指派给一个窗口，则不调用DispatchMessage，消息循环继续取下一个消息。像PostMessage函数一样，PostThreadMessage在向线程的队列登记了消息之后就立即返回。调用该函数的线程不知道消息是否被处理。
向线程的队列发送消息的函数还有PostQuitMessage：
VOID PostQuitMessage(int nExitCode);
为了终止线程的消息循环，可以调用这个函数。调用PostQuitMessage类似于调用：
PostThreadMessage(GetCurrentThreadId(),WM_QUIT,nExitCode,0);
但是，PostQuitMessage并不实际登记一个消息到任何一个THREADINFO结构的队列。只是在内部，PostQuitMessage设定QS＿QUIT唤醒标志（后面将要讨论），并设置THREADINFO结构的nExitCode成员。因为这些操作永远不会失败，所以PostQuitMessage的原型被定义成返回VOID。
26.3向窗口发送消息
使用SendMessage函数可以将窗口消息直接发送给一个窗口过程：
LRESULT SendMessage(
HWND hwnd,
UINT uMsg,
WPARAM wParam,
LPARAM lParam);
窗口过程将处理这个消息。只有当消息被处理之后，SendMessage才能返回到调用程序。由于具有这种同步特性，比之PostMessage或PostThreadMessage，SendMessage用得更频繁。调用这个函数的线程在下一行代码执行之前就知道窗口消息已经被完全处理。
SendMessage是如何工作的呢？如果调用SendMessage的线程向该线程所建立的一个窗口发送一个消息，SendMessage就很简单：它只是调用指定窗口的窗口过程，将其作为一个子例程。当窗口过程完成对消息的处理时，它向SendMessage返回一个值。SendMessage再将这个值返回给调用线程。
但是，当一个线程向其他线程所建立的窗口发送消息时，SendMessage的内部工作就复杂得多（即使两个线程在同一进程中也是如此）。Windows要求建立窗口的线程处理窗口的消息。
所以当一个线程调用SendMessage向一个由其他进程所建立的窗口发送一个消息，也就是向其他的线程发送消息，发送线程不可能处理窗口消息，因为发送线程不是运行在接收进程的地址空间中，因此不能访问相应窗口过程的代码和数据。实际上，发送线程要挂起，而由另外的线程处理消息。所以为了向其他线程建立的窗口发送一个窗口消息，系统必须执行下面将讨论的动作。
首先，发送的消息要追加到接收线程的发送消息队列，同时还为这个线程设定QS_SENDMESSAGE标志（后面将讨论）。其次，如果接收线程已经在执行代码并且没有等待消息（如调用GetMessage、PeekMessage或WaitMessage），发送的消息不会被处理，系统不能中断线程来立即处理消息。当接收进程在等待消息时，系统首先检查QS_SENDMESSAGE唤醒标志是否被设定，如果是，系统扫描发送消息队列中消息的列表，并找到第一个发送的消息。
有可能在这个队列中有几个发送的消息。例如，几个线程可以同时向一个窗口分别发送消息。当发生这样的事时，系统只是将这些消息追加到接收线程的发送消息队列中。
当接收线程等待消息时，系统从发送消息队列中取出第一个消息并调用适当的窗口过程来处理消息。如果在发送消息队列中再没有消息了，则QS_SENDMESSAGE唤醒标志被关闭。当接收线程处理消息的时候，调用SendMessage的线程被设置成空闲状态（idle），等待一个消息出现在它的应答消息队列中。在发送的消息处理之后，窗口过程的返回值被登记到发送线程的应答消息队列中。发送线程现在被唤醒，取出包含在应答消息队列中的返回值。这个返回值就是调用SendMessage的返回值。这时，发送线程继续正常执行。
当一个线程等待SendMessage返回时，它基本上是处于空闲状态。但它可以执行一个任务：如果系统中另外一个线程向一个窗口发送消息，这个窗口是由这个等待SendMessage返回的线程所建立的，则系统要立即处理发送的消息。在这种情况下，系统不必等待线程去调用GetMessage、PeekMessage或WaitMessage。
由于Windows使用上述方法处理线程之间发送的消息，所以有可能造成线程挂起（hang）。
例如，当处理发送消息的线程含有错误时，会导致进入死循环。那么对于调用SendMessage的线程会发生什么事呢？它会恢复执行吗？这是否意味着一个程序中的bug会导致另一个程序挂起？答案是确实有这种可能。
利用4个函数——SendMessageTimeout、SendMessageCallback、SendNotifyMessage和ReplyMessage，可以编写保护性代码防止出现这种情况。第一个函数是SendMessageTimeout：利用SendMessageTimeout函数，可以规定等待其他线程答回你消息的时间最大值。
用来在线程间发送消息的第二个函数是SendMessageCallback：当一个线程调用SendMessageCallback时，该函数发送消息到接收线程的发送消息队列，并立即返回使发送线程可以继续执行。
线程间发送消息的第三个函数是SendNotifyMessage：SendNotifyMessage将一个消息置于接收线程的发送消息队列中，并立即返回到调用线程。
第四个用于线程发送消息的函数是ReplyMessage：这个函数与前面讨论过的三个函数不同。线程使用SendMessageTimeout、SendMessageCallback和SendNotifyMessage发送消息，是为了保护自己以免被挂起。而线程调用ReplyMessage是为了接收窗口消息。当一个线程调用ReplyMessage时，它是要告诉系统，为了知道消息结果，它已经完成了足够的工作，结果应该包装起来并登记到发送线程的应答消息队列中。这将使发送线程醒来，获得结果，并继续执行。
26.4唤醒一个线程
当一个线程调用GetMessage或WaitMessage，但没有对这个线程或这个线程所建立窗口的消息时，系统可以挂起这个线程，这样就不再给它分配CPU时间。当有一个消息登记或发送到这个线程，系统要设置一个唤醒标志，指出现在要给这个线程分配CPU时间，以便处理消息。正常情况下，如果用户不按键或移动鼠标，就没有消息发送给任何窗口。这意味着系统中大多数线程没有被分配给CPU时间。
26.4.1队列状态标志
当一个线程正在运行时，它可以通过调用GetQueueStatus函数来查询队列的状态：
DWORD GetQueueStatus(UINT fuFlags);
参数fuFlags是一个标志或一组由OR连接起来的标志，可用来测试特定的唤醒位。表26-2给出了各个标志取值及含义。
表26-2标志取值及含义
标志                队列中的消息
QS_KEY         WM_KEYUP、WM_KEYDOWN、WM_SYSKEYUP或WM_SYSKEYDOWN
QS_MOUSEMOVE      WM_MOUSEMOVE
QS_MOUSEBUTTON    WM_?BUTTON*（其中？代表L、M或R、*代表DOWN、UP或DBLCLK)
QS_MOUSE  同QS_MOUSEMOVE|QS_MOUSEBUTTON
QS_INPUT  同QS_MOUSE|QS_KEY
QS_PAINT  WM_PAINT
QS_TIMER  WM_TIMER
QS_HOTKEY WM_HOTKEY
QS_POSTMESSAGE  登记的消息（不同于硬件输入事件）。当队列在期望的消息过滤器范围内没有登记的消息时，这个标志要消除。除此之外，这个标志与QS_ALLPOSTMESSAGE相同
QS_ALLPOSTMESSAGE 登记的消息（不同于硬件输入事件）。当队列完全没有登记的消息时（在任何消息过滤器范围），该标志被清除。除此之外，该标志与QS_POSTMESSAGE相同
QS_ALLEVENTS  同QS_INPUT|QS_POSTMESSAGE|QS_TIMER|QS_PAINT|QS_HOTKEY
QS_QUIT  已调用PostQuitMessage。注意这个标志没有公开，所以在WinUser.h文件中没有。它由系统在内部使用
QS_SENDMESSAGE 由另一个线程发送的消息
QS_ALLINPUT  同QS_ALLEVENTS|QS_SENDMESSAGE
当调用GetQueueStatus函数时，fuFlags将队列中要检查的消息的类型告诉GetQueueStatus。
用OR连接的QS_*标识符的数量越少，调用执行的就越快。当GetQueueStatus返回时，线程的队列中当前消息的类型在返回值的高字（两字节）中。这个返回的标志的集合总是所想要的标志集的子集。
不是所有的唤醒标志都由系统平等对待。对于QS_MOUSEMOVE标志，只要队列中存在一个未处理的WM_MOUSEMOVE消息，这个标志就要被设置。当GetMessage或PeekMessage(利用PM_REMOVE)从队列中放入新的WM_MOUSEMOVE消息之前，这个标志被关闭。QS_KEY、QS_MOUSEBUTTON和QS_HOTKEY标志都根据相应的消息按与此相同的方式处理。
QS_PAINT标志的处理与此不同。如果线程建立的一个窗口有无效的区域，QS_PAINT标志被设置。当这个线程建立的所有窗口所占据的区域变成无效时（通常由于对ValidateRect、ValidateRegion或BeginPaint的调用而引起），QS_PAINT标志就被关闭。只有当线程建立的所有窗口都无效时，这个标志才关闭。调用GetMessage或PeekMessage对这个唤醒标志没有影响。
当线程的登记消息队列中至少有一个消息时，QS_POSTMESSAGE标志就被设置。这不包括线程的虚拟输入队列中的硬件事件消息。当线程的登记消息队列中的所有消息都已经处理，队列变空时，这个标志被复位。
每当一个定时器（由线程所建立）报时（gooff），QS_TIMER标志就被设置。在GetMessage或PeekMessage返回WM_TIMER事件之后，QS_TIMER标志被复位，直到定时器再次报时。QS_SENDMESSAGE标志指出有一个消息在线程的发送消息队中。系统在内部使用这个标志，用来确认和处理线程之间发送的消息。对于一个线程向自身发送的消息，不设置这个标志。虽然可以使用QS_SENDMESSAGE标志，但很少需要这样做。笔者还从未见到一个程序使用这个标志。
还有一个未公开的队列状态标志QS_QUIT。当一个线程调用PostQuitMessage时，QS_QUIT标志就被设置。系统并不实际向线程的消息队列追加一个WM_QUIT消息。GetQueueStatus函数也不返回这个标志的状态。
26.4.2从线程的队列中提取消息的算法
当一个线程调用GetMessage或PeekMessage时，系统必须检查线程的队列状态标志的情况，并确定应该处理哪个消息。下面叙述的步骤说明了系统是如何确定线程应该处理的下一个消息的情况。
1)如果QS_SENDMESSAGE标志被设置，系统向相应的窗口过程发送消息。GetMessage或PeekMessage函数在内部进行这种处理，并且在窗口过程处理完消息之后不返回到线程，这些函数要等待其他要处理的消息。
2)如果消息在线程的登记消息队列中，函数GetMessage或PeekMessage填充传递给它们的MSG结构，然后函数返回。这时，线程的消息循环通常调用DispatchMessage，让相应的窗口过程来处理消息。
3)如果QS_QUIT标志被设置。GetMessage或PeekMessage返回一个WM_QUIT消息（其中wParam参数是规定的退出代码）并复位QS_QUIT标志。
4)如果消息在线程的虚拟输入队列，函数GetMessage或PeekMessage返回硬件输入消息。
5)如果QS_PAINT标志被设置，GetMessage或PeekMessage为相应的窗口返回一个WM-PAINT消息。
6)如果QS_TIMER标志被设置，GetMessage或PeekMessage返回一个WM_TIMER消息。
尽管很难令人相信，但确有理由这样做。微软在设计这个算法时有一个大前提，就是应用
程序应该是用户驱动的，用户通过建立硬件输入事件（键盘和鼠标操作）来驱动应用程序。在使用应用程序时，用户可能按一个鼠标按钮，引起一系列要发生的事件。应用程序通过向线程的消息队列中登记消息使每个个别的事件发生。
所以如果按鼠标按钮，处理WM_LBUTTONDOWN消息的窗口可能向不同的窗口投送三个消息。由于是硬件事件引发三个软件事件，所以系统要在读取用户的下一个硬件事件之前，先处理这些软件事件。这也说明了为什么登记消息队列要在虚拟输入队列之前检查。
注意要记住GetMessage或PeekMessage函数只检查唤醒标志和调用线程的消息队列。这意味着一个线程不能从与其他线程挂接的队列中取得消息，包括同一进程内其他线程的消息。
26.4.3利用内核对象或队列状态标志唤醒线程
GetMessage或PeekMessage函数导致一个线程睡眠，直到该线程需要处理一个与用户界面（UI）相关的任务。有时候，若能让线程被唤醒去处理一个与UI有关的任务或其他任务，就会带来许多方便。例如，一个线程可能启动一个长时间运行的操作，并可以让用户取消这个操作。
这个线程需要知道何时操作结束（与UI无关的任务），或用户是否按了Cancel按钮（与UI相关的任务）来结束操作。
一个线程可以调用MsgWaitForMultipleObjects或MsgWaitForMultipleObjectsEx函数，使线程等待它自已的消息。
这两个函数类似于WaitForMultipleObjects函数（在第9章讨论过）。不同之处是，当一个内核对象变成有信号状态（signaled）或当一个窗口消息需要派送到调用线程建立的窗口时，这两个函数用于线程调度。
26.5通过消息发送数据
本节将讨论系统如何利用窗口消息在进程之间传送数据。一些窗口消息在其lParam参数中指出了一个内存块的地址。例如，WM_SETTEXT消息使用lParam参数作为指向一个以零结尾的字符串的指针，这个字符串为窗口规定了新的文本标题串。考虑下面的调用：
SendMessage(FindWindow(NULL,”Calculator”),WM_SETTEXT,0,(LPARAM)”A Test Caption”);
这个调用看起来不会有害。它确定Calculator程序窗口的窗口句柄，并试图将窗口的标题改成“ATestCaption”。但我们要仔细看一看究竟会发生什么。
新标题的字符串包含在调用进程的地址空间里。所以这个在调用进程空间的字符串的地址将传递给lParam参数。当Calculator的窗口的窗口过程收到这个消息时，它要查看lParam参数，并要读取这个以零结尾的字符串，使其成为新的标题。
但lParam中的地址指向调用进程的地址空间中的字符串，而不是Calculator的地址空间。这会发生内存存取违规这种严重问题。但当你执行上面的代码时，你会看到执行是成功的，为什么会是这样？
答案是系统特别要检查WM_SETTEXT消息，并用与处理其他消息不同的方法来处理这个消息。当调用SendMessage时，函数中的代码要检查是否要发送一个WM_SETTEXT消息。如果是，就将以零结尾的字符串从调用进程的地址空间放入到一个内存映像文件中，该内存映像文件可在进程间共享。然后再发送消息到其他进程的线程。当接收线程已准备好处理WM_SETTEXT消息时，它在自己的地址空间中确定包含新的窗口文本标题的共享内存映像文件的位置，再将WM_SETTEXT消息派送到相应的窗口过程。在处理完消息之后，内存映像文件被删除。这样做看起来是不是太麻烦了一些。
幸而大多数消息不要求这种类型的处理。仅当这种消息是程序在进程间发送的消息，特别是消息的wParam或lParam参数表示一个指向数据结构的指针时，要做这样的处理。
我们再来看另外一个要求系统特殊处理的例子——WM_GETTEXT消息。假定一个程序包含下面的代码：
Char szBuf[200];
sendMessage(findWindow(NULL,”calculator”),WM_GETTEXT,sizeof(szBuf),(lparam)szBuf);
WM_GETTEXT消息请求Calculator的窗口过程用该窗口的标题填充szBuf所指定的缓冲区。当一个进程向另一个进程的窗口发送这个消息时，系统实际上必须发送两个消息。首先，系统要向那个窗口发送一个WM_GETTEXTLENGTH消息。窗口过程通过返回窗口标题的字符数来响应这个消息。系统利用这个数字来建立一个内存映像文件，用于在两个进程之间共享。
当内存映像文件被建立时，系统就发送消息来填充它。然后系统再转回到最初调用SendMessage的进程，从共享内存映像文件中将数据复制到szBuf所指定的缓冲区中，然后从SendMessage调用返回。
对于系统已经知道的消息，发送消息时都可以按相应的方式来处理。如果你要建立自己的（WM_USER＋x）消息，并从一个进程向另一个进程的窗口发送，那又会怎么样？系统不知道你要用内存映像文件并在发送消息时改变指针。为此，微软建立了一个特殊的窗口消息，WM_COPYDATA以解决这个问题：
COPYDATASTRUCT cds;
sendMessage(hwndReceiver,WM_COPYDATA,
   (WPARAM)hwndSender,(LPARAM)&cds);
COPYDATASTRUCT是一个结构，定义在WinUser.h文件中，形式如下面的样子：
Typedef struct tagCOPYDATASTRUCT{
ULONG_PTR dwData;
DWORD cbData;
PVOID lpData;
}COPYDATASTRUCT;
当一个进程要向另一个进程的窗口发送一些数据时，必须先初始化COPYDATASTRUCT结构。数据成员dwData是一个备用的数据项，可以存放任何值。例如，你有可能向另外的进程发送不同类型或不同类别的数据。可以用这个数据来指出要发送数据的内容。
cbData数据成员规定了向另外的进程发送的字节数，lpData数据成员指向要发送的第一个字节。lpData所指向的地址，当然在发送进程的地址空间中。
当SendMessage看到要发送一个WM_COPYDATA消息时，它建立一个内存映像文件，大小是cbData字节，并从发送进程的地址空间中向这个内存映像文件中复制数据。然后再向目的窗口发送消息。在接收消息的窗口过程处理这个消息时，lParam参数指向已在接收进程地址空间的一个COPYDATASTRUCT结构。这个结构的lpData成员指向接收进程地址空间中的共享内存映像文件的视图。
关于WM_COPYDATA消息，应该注意三个重要问题：
•只能发送这个消息，不能登记这个消息。不能登记一个WM_COPYDATA消息，因为在接收消息的窗口过程处理完消息之后，系统必须释放内存映像文件。如果登记这个消息，系统不知道这个消息何时被处理，所以也不能释放复制的内存块。
•系统从另外的进程的地址空间中复制数据要花费一些时间。所以不应该让发送程序中运
行的其他线程修改这个内存块，直到SendMessage调用返回。
•利用WM_COPYDATA消息，可以实现16位和32位之间的通信。它也能实现32位与64位之间的通信。这是使新程序同旧程序交流的便捷方法。注意在Windows2000和Windows98上完全支持WM_COPYDATA。但如果你依然在编写16位Windows程序，Microsofe VisualC++1.52没有WM_COPYDATA消息的定义，也没有COPYDATASTRUCT结构的定义。需要手工添加些代码。
26.6Windows如何处理ANSI／Unicode字符和字符串
Windows98Windows98只支持ANSI窗口类和ANSI窗口过程。
当你注册一个新的窗口类时，必须将负责为这个类处理消息的窗口过程的地址告诉系统。
对某些消息（如WM_SETTEXT），消息的lParam参数指向一个字符串。在此之前，为了派送消息，使它被正确地处理，系统需要知道窗口过程要求该字符串是ANSI字符串还是Unicode字符串。
告诉系统一个窗口过程是要求ANSI字符串还是Unicode字符串，实际上取决于注册窗口类时所使用的函数。如果构造WNDCLASS结构，并调用RegisterClassA，系统就认为窗口过程要求所有的字符串和字符都属于ANSI。而用RegisterClassW注册窗口类，则系统就向窗口过程派送Unicode字符串和字符。宏RegisterClass对RegisterClassA和RegisterClassW都做了扩展，究竟代表哪一个要看在编译源模块时是否定义了UNICODE。
如果有了一个窗口句柄，就可以确定窗口过程所要求的字符和字符串类型。这可以通过调用下面的函数实现：
BOOL IswindowUnicode(HWND hwnd);
如果这个窗口的窗口过程要求Unicode，这个函数返回TRUE，否则返回FALSE。
如果你建立一个ANSI串，并向一个窗口过程要求Unicode串的窗口发送WM_SETTEXT消息，则系统在发送消息之前，为你自动地转换字符串。很少需要调用IsWindowUnicode函数。
如果你对窗口过程派生子类，系统也会为你执行自动的转换。假定一个编辑控制框的窗口过程要求字符和字符串是Unicode。在你的程序的某处建立了一个编辑控制框，并建立窗口过程的子类，这可以调用
LONG_PTR SetWindowLongPtrA(
HWND hwnd,
Int nIndex,
LONG_PTR dwNewLong);
或
LONG_PTR SetWindowLongPtrW(
HWND hwnd,
Int nIndex,
LONG_PTR dwNewLong);
并将nIndex参数设置成GCLP_WNDPROC，dwNewLong参数设置成子类过程的地址。如果这个子类过程要求ANSI字符和字符串会出现什么情况？这可能引起严重的问题。系统决定怎样转换字符串和字符，要取决于究竟是用上面两个函数中的哪一个来建立子类。如果是调用SetWindowLongPtrA，就是告诉系统新的窗口过程（即子类过程）要接收ANSI字符和字符串。
实际上，如果在调用SetWindowLongPtrA之后调用IsWindowUnicode函数，将返回FALSE，表
示这个子类的编辑窗口过程不再要求Unicode字符和字符串。
但现在又有一个新的问题：如何能够保证原来的窗口过程得到正确的字符和字符串类型？系统需要有两条信息，才能正确地转换字符和字符串。第一条信息就是字符和字符串当前所具有的形式。这可以通过调用CallWindowProcA或CallWindowProcW来告诉系统：
LRESULT CallWindowProcA(
WNDPROC wndprcPrev,
HWND hwnd,
UINT uMsg,
WPARAM wParam,
LPARAM lParam);

LRESULT CallWindowProcW(
WNDPROC wndprcPrev,
HWND hwnd,
UINT uMsg,
WPARAM wParam,
LPARAM lParam);
如果子类过程要把ANSI字符串传递给原来的窗口过程，子类过程必须调用CallWindowProcA。如果子类过程要把Unicode字符串传递给原来的窗口过程，则子类过程必须调用CallWindowProcW。
系统需要的第二条信息是原来的窗口过程所要求的字符和字符串类型。系统从原来窗口过程的地址获取这个信息。当调用SetWindowLongPtrA或SetWindowLongPtrW函数时，系统要查看是否使用了一个ANSI子类过程派生了一个Unicode窗口过程，或用一个Unicode子类过程派生了一个ANSI窗口过程。如果没有改变所要求的字符串类型，则SetWindowLongPtr只返回原先窗口过程的地址。如果改变了窗口过程要求的字符和字符串类型，SetWindowLongPtr不是返回原先窗口过程的实际地址，而是返回一个内部子系统数据结构的句柄。
这个数据结构包含原先窗口过程的地址及一个数值，用来指示窗口过程是要求ANSI还是要求Unicode字符串。当调用CallWindowProc时，系统要查看是传递了某个内部数据结构的地址，还是传递了一个窗口过程的地址。如果传递了一个窗口过程的地址，则调用原先的窗口过程，不需要执行字符和字符串转换。
如果传递了一个内部数据结构的句柄，则系统要将字符和字符串转换成适当的类型（ANSI或Unicode），然后调用原先的窗口过程。

第27章硬件输入模型和局部输入状态
本章将讨论系统的硬件输入模型。重点将考察按键和鼠标事件是如何进入系统并发送给适当的窗口过程的。微软设计输入模型的一个主要目标就是为了保证一个线程的动作不要对其他线程的动作产生不好的影响。这里是一个16位Windows中的例子：如果一个任务引起一个死循环，所有的任务都被挂起，不能再响应用户。用户只能重新启动计算机。这样就给一个单个的任务太多的控制。强壮的操作系统，例如Windows2000和Windows98，不会使一个挂起的线程妨碍系统中其他线程接收硬件输入。
27.1原始输入线程
当系统初始化时，要建立一个特殊的线程，即原始输入线程（rawinputthread，RIT）。此外，系统还要建立一个队列，称为系统硬件输入队列（Systemhardwareinputqueue,SHIQ）。RIT和SHIQ构成系统硬件输入模型的核心。
  RIT通常处于睡眠状态，等待有一个项出现在SHIQ中。当用户按下和放开一个键．按下和放开一个鼠标按钮或移动鼠标的时候，这些硬件设备的设备驱动程序就向SHIQ中增加一个硬件事件，这将唤醒RIT。然后RIT从SHIQ中提取这个项，并将这个事件转换成适当的WM—KEY*、WM—?BUTTON*或WM_MOUSEMOVE消息。转换成的消息再添加到适当线程的虚拟输入队列(vIQ)中(这在第26章讨论过)。然后RIT再循环等待更多的消息出现在SHIQ中，由于RIT只执行微软编写的代码，并且，这些代码又经过严格测试．所以RIT不合停止对硬件输入事件的响应。
那么RIT怎么才能知道要向哪一个线程的虚拟输入队列里增加硬件输入消息？对鼠标消息，RIT只是确定是哪一个窗口在鼠标光标之下。利用这个窗口，RIT调用GetWindowThreadProcessId来确定是哪个线程建立了这个窗口。返回的线程ID指出哪一个线程应该得到这个鼠标消息。
对按键硬件事件的处理稍有不同。在任何给定的时刻，只有一个线程同RIT“连接”。这个线程称为前景线程（foregroundthread），因为它建立了正在与用户交互的窗口，并且这个线程的窗口相对于其他线程所建立的窗口来说处在画面中的前景。
当一个用户在系统上登录时，WindowsExplorer进程让一个线程建立相应的任务栏（taskbar）和桌面。这个线程连接到RIT。如果你又要产生Calculator，那么就又有一个线程来建立一个窗口，并且这个线程变成连接到RIT的线程。注意现在WindowsExplorer的线程不再与RIT连接，因为在一个时刻只能有一个线程同RIT连接。当一个按键消息进入SHIQ时，RIT就被唤醒，将这个事件转换成适当的按键消息，并将消息放入与RIT连接的线程的虚拟输入队列。
不同的线程是如何连接到RIT的呢？我们已经说过，当产生一个进程时，这个进程的线程可以建立一个窗口。这个窗口处于前景，其建立窗口的线程同RIT相连接。另外，RIT还要负责处理特殊的键组合，如Alt+Tab、Alt+Esc和Ctrl+Alt+Del等。因为RIT在内部处理这些键组合，就可以保证用户总能用键盘激活窗口。应用程序不能够拦截和废弃这些键组合。当用户按动了某个特殊的键组合时，RIT激活选定的窗口，并将窗口的线程连接到RIT。Windows也提供激活窗口的功能，使窗口的线程连接到RIT。这些功能在本章后面讨论。
从图27-1中可以看到如何保护线程，避免相互影响的。如果RIT向窗口B1或窗口B2发送一个消息，消息到达线程B的虚拟输入队列。在处理消息时，线程B在与五个内核对象同步时可能会进入死循环或死锁。如果发生这种情况，线程仍然同RIT连接在一起，并且可能有更多的消息要增加到线程的虚拟输入队列中。
这种情况下，用户会发现窗口B1和B2都没有反应，可能想切换到窗口A1。为了做这种切换，用户按Alt+Tab。因为是RIT处理Alt+Tab按键组合，所以用户总能切换到另外的窗口，不会有什么问题。在选定窗口A1之后，线程A就连接到RIT。这个时候，用户就可以对窗口A1进入输入，尽管线程及其窗口都没有响应。
27.2局部输入状态
  让线程独立地处理输入、防止线程之间相互有不利的影响，这些只是实现强壮输入模型工作的一部分，如果仅有这些特性．不足以保证线程相互之间的隔离。所以系统还要有另外的基础设施。这个基础设施称作局部输入状念(1ocal input state)。
    每个线段都有自已的局部输入状态、并在线程的THREADINF0结构内进行管理(THREADINFo结构在第26章讨论过)。这个局部输人状态由线程的虚拟输入队列和一组变量构成。这些变量用来跟踪下面的输入状念管理信息：
键盘输入和窗口焦点信息．例如：
•哪一个窗口有键盘焦点：
．哪一个窗口是活动的。
．哪一个键被认为是按下的。
．插入符号的状态
这些变量还跟踪鼠标光标的管理信息，例如：
•哪一个窗口有鼠标捕获。
•鼠标光标的形状。
•鼠标光标的可见性。
由于每个线程都有自己的输入状态变量，每个线程都有不同的焦点窗口、鼠标捕获窗口等概念。从一个线程的角度来看，或者它的某个窗口拥有键盘焦点，或者系统中没有窗口拥有键盘焦点；或者它的某个窗口拥有鼠标捕获，或者系统中没有窗口拥有鼠标捕获，等等。读者会想到，这种隔离应该有一些细节，对此我们将在后面讨论。
27.2.1键盘输入与焦点
我们已经知道，RIT使用户的键盘输入流向一个线程的虚拟输入队列，而不是流向一个窗口。RIT将键盘事件放入线程的虚拟输入队列时不用涉及具体的窗口。当这个线程调用GetMessage时，键盘事件从队列中移出并分派给当前有输入焦点的窗口。（由该线程所建立）。
图27-2说明了这个处理过程。

图27-2RIT将用户的键盘输入导向一个线程的虚拟输入队列
线程1当前正在从RIT接收输入，用窗口A、窗口B或窗口C的句柄作参数调用SetFocus会引起焦点改变。失去焦点的窗口除去它的焦点矩形或隐藏它的插入符号，获得焦点的窗口画出焦点矩形或显示它的插入符号。
假定线程1仍然从RIT接收输入，并用窗口E的句柄作为参数调用SetFocus。这种情况下，系统阻止执行这个调用．因为想要设置焦点的窗口不使用当前连接RIT的虚拟输人队列。在线程1执行这个调用之后，焦点没有发生变化，屏幕的样子也没有变化。
    在另一种情况下，线程1同RIT连接，线程用窗口E的句柄作为参数调用setFocus。这时，线程2的局部输入状态要更新．表示下一次当RIT将按键消息导向线程2时，窗口E是接收键盘输入的窗口。
    因窗口E现在拥有线程的焦点，它接收一个WM_SETFOCUS消息。如果窗口E是—个按钮(PushbuKon)，它为自己画一个焦点矩形．所以屏幕上会出现两个有焦点矩形的窗口(窗口A和窗口E)。最终用户对此可能感到困惑。程序员应该小心地使用setFoucs，避免出现这种情况。仅当线程同RIT连接时才调用setFoucs。
    如果一个窗口在接收WM—SETFocus消息时显示一个插入符号，给它焦点时会造成屏幕上几个窗口同步地显示闪烁的插入符号。这可能会使用户感到不解：
    当使用传统的方法(如用鼠标点击窗口3将焦点从一个窗口转移到另一个窗口时，失去焦点的窗口接收一个WM—KILFOUCS消息。如果接收焦点的窗口所属的线程同失去焦点的窗口的线程不一样，那么，对于建立失去焦点窗口的线程，要更新它的局部输入状态变量，说明它没有窗口拥有焦点。这时调用GetFocus将返回NULL，这会使线程知道当前没有窗口拥有焦点。
函数SetActiveWindow激活系统中一个最高层（top-level）的窗口，并对这个窗口设定焦点：
同SetFocus函数一样，如果调用线程没有创建作为函数参数的窗口，则这个函数什么也不做。
与SetActiveWindow相配合的函数是GetActiveWindow函数：这个函数的功能同GetFocus函数差不多，不同之处是它返回由调用线程的局部输入状态变量所指出的活动窗口的句柄。当活动窗口属于另外的线程时，GetActiveWindow返回NULL。
其他可以改变窗口的Z序（Z-order）、活动状态和焦点状态的函数还包括BringWindowToTop和SetWindowPos：
这两个函数功能相同（实际上，BringWindowToTop函数在内部调用SetWindowPos，以HWND_TOP作为第二个参数）。如果调用这两个函数的线程没有连接到RIT，则函数什么也不做。如果调用这些函数的线程同RIT相连接，系统就会激活相应的窗口。注意即使调用线程不是建立这个窗口的线程，也同样有效。这意味着，这个窗口变成活动的，并且建立这个窗口的线程被连接到RIT。这也引起调用线程和新连接到RIT的线程的局部输入状态变量被更新。
有时候，一个线程想让它的窗口成为屏幕的前景。例如，有可能会利用MicrosoftQutlook安排一个会议。在会议开始前的半小时，Outlook弹出一个对话框提醒用户会议将要开始。如果Qutlook的线程没有连接到RIT，这个对话框就会藏在其他窗口的后面，有可能看不见它。因此需要有某种办法把用户的注意力吸引到窗口，即使用户正在使用其他程序的窗口。
    下面的函数SetForegroundWindow窗口放到前景，并将它的线程连接到RIT：
    通过调用这个函数．系统将激活相应的窗口并给它焦点。与这个函数相配合的函数是GetForegroundWindow。
    这个函数返回当前处于前景的窗口的句柄。
在Window98的早期版本里．SetForegroundwindow总是有效的。也就是说．调用这个函数的线程总能把相应的窗口拉到前景(即使这个窗口不是调用线程所建立的)。但是后来有些开发人员开始滥用这种能力，相互之间乱弹窗口。例如，你正在写—篇稿子或录入一篇文章，突然屏幕上跳出一个Print Job Finished对话框。如果你没有在看屏幕也就没有看见对话框，这时键人的文字就到了对话框里，而不是你的文件里。更令人恼火的是当你正在选择一个菜单项时、突然另一个窗口跳出来．关闭了你选的菜单。
为了制止这种现象，微软对SetForegroundWindow函数增加了更多的智能。特别规定，仅当调用一个函数的线程已经连接到RIT或者当前与RIT相连接的线程在一定的时间内（这个时间量由SystemParametersInfo函数和SPI_SETFOREGROUND_LOCKTIMEOUT值来控制）没有收到任何输入，这个函数才有效。另外，如果有一个菜单是活动的，这个函数就失效。
如果不允许SetForegroundWindow将窗口移到前景，它会闪烁该窗口的标题栏和任务条上该窗口的按钮。用户看到任务条按钮闪烁，就知道该窗口想得到用户的注意。用户应该手工激活这个窗口，看一看要报告什么信息。还可以用SystemParametersInfo函数和SPI_SETFOREGROUND-FLASHCOUNT值来控制闪烁。
由于这些新的内容，系统又提供了另外一些函数。如果调用AllowSetForegroundWindow的线程能够成功调用SetForegroundWindow，第一个函数（见下面所列）可使指定进程的一个线程成功调用SetForegroundWindow。为了使任何进程都可以在你的线程的窗口上弹出一个窗口，指定ASFW_ANY(定义为－1)作为dwProcessId参数：
此外，线程可以锁定SetForegroundWindow函数，使它总是失效的。方法是调用LockSetForegroundWindow。
对uLockCode参数可以指定LSFW_LOCK或者LSFW_UNLOCK。当一个菜单被激活时，系统在内部调用这个函数，这样一个试图跳到前景的窗口就不能关闭这个菜单。Windows Explorer在显示Start菜单时，需要明确地调用这些函数，因为Start菜单不是一个内置菜单。当用户按了Alt键或者将一个窗口拉到前景时，系统自动解锁SetForegroundWindow函数。这可以防止一个程序一直对SetForegroundWindow函数封锁。
关于键盘管理和局部输入状态，其他的内容是同步键状态数组。每个线程的局部输入状态变量都包含一个同步键状态数组，但所有的线程要共享一个同步键状态数组。这些数组反映了在任何给定时刻键盘所有键的状态。利用GetAsyncKeyState函数可以确定用户当前是否按下了键盘上的一个键：
参数nVirtKey指出要检查键的虚键代码。结果的高位指出该键当前是否被按下（是为1，否为0）。笔者在处理一个消息时，常用这个函数来检查用户是否释放了鼠标主按钮。为函数参数赋一个虚键值VK_LBUTTON，并等待返回值的高位成为0。注意，如果调用函数的线程不是建立当时拥有输入焦点窗口的线程．则GetAsyncKeyState总是返回0：
    另一个函数GetKeyState与GetAsyncKeyState函数不同．它返回最近的键盘消息从线程的队列中移出时的键盘状态。
不管哪个窗口拥有输入焦点．这个函数都不受影响。在任何时候都可调用：
27.2.2 鼠标光标管理
鼠标光标管理是局部输入状态的另一部分内容。鼠标同键盘一样，必须在所有不同线程之间共享．Windows系统不能允许某—个线程通过改变鼠标光标形状或将其配置到屏幕的一个小区域来独占鼠标光标，本节讨论系统如何管理鼠标光标。
    鼠标光标管理的一个方画是鼠标光标的隐藏／显示能力。如果一个线程调用showcousor(FALsE)．则鼠标光标在该线程建立的任何窗口之—都被隐藏。如果鼠标光标移到其他线程所建立的窗口上，鼠标光标就可见了。
鼠标光标管理的另一个方面是使用ClipCursor函数将鼠标光标剪贴到一个矩形区域。
这个函数使鼠标被限制在一个由prc参数指定的矩形区域内。当一个程序调用ClipCursor函数时，系统该做些什么呢？允许剪贴鼠标光标可能会对其他线程产生不利影响，而不允许剪贴鼠标光标又会影响调用线程。微软实现了一种折衷的方案。当一个线程调用这个函数时，系统将鼠标光标剪贴到指定的矩形区域。但是，如果同步激活事件发生（当用户点击了其他程序的窗口，调用了SetForegroundWindow，或按了Ctrl+Esc组合键），系统停止剪贴鼠标光标的移动，允许鼠标光标在整个屏幕上自由移动。
现在我们再讨论鼠标捕获。当一个窗口“捕获”鼠标（通过调用SetCapture）时，它要求所有的鼠标消息从RIT发到调用线程的虚拟输入队列，并且所有的鼠标消息从虚拟输入队列发到设置捕获的窗口。在程序调用ReleaseCapture之前，要一直继续这种鼠标消息的捕获。
鼠标的捕获必须同系统的强壮性折衷，也只能是一种折衷。当一个程序调用SetCapture时，RIT将所有鼠标消息放入线程的虚拟输入队列。SetCapture还要为调用SetCapture的线程设置局部输入状态变量。
通常一个程序在用户按一个鼠标按钮时调用SetCapture。但是即使鼠标按钮没有被按下，也没有理由说一个线程不能调用SetCapture。如果当一个鼠标按下时调用SetCapture，捕获在全系统范围内执行。但当系统检测出没有鼠标按钮按下时，RIT不再将鼠标消息只发往线程的虚拟输入队列，而是将鼠标消息发往与鼠标光标所在的窗口相联系的输入队列。这是不做鼠标捕获时的正常行为。
但是，最初调用SetCapture的线程仍然认为鼠标捕获有效。因此，每当鼠标处于有捕获设置的线程所建立的窗口时，鼠标消息将发往这个线程的捕获窗口。换言之，当用户释放了所有的鼠标按钮时，鼠标捕获不再在全系统范围内执行，而是在一个线程的局部范围内执行。
此外，如果用户想激活一个其他线程所建立的窗口，系统自动向设置捕获的线程发送鼠标按钮按下和鼠标按钮放开的消息。然后系统更新线程的局部输入状态变量，指出该线程不再具有鼠标捕获。很明显，通过这种实现方式，微软希望鼠标点击和拖动是使用鼠标捕获的最常见理由。
  与鼠标相关的最后一个局部输入状态变量是鼠标光标的形状。每当一个线程调用setcaPture来改变鼠标光标的形状时．局部输入状态变量都要更新，以反映鼠标光标的形状。换句话说，局部输人状态变量总是记住线程所设置的最新的鼠标光标形状。
    我们假定用户在你的窗口上移动鼠标，你的窗口接收到一个wM_SETCursor消息,然后你调用setcapture将鼠标光标的形状改变成一个沙漏。在setcapture调用之后．你的代码进入一个长时间进程(死循环是长时间进程的例子)。现在用户将鼠标光标移出你的窗口，到达其他程序的窗口，当鼠标在其他某个窗口上移动时，鼠标形状就由这个窗口来控制。
    完成这种变化不要求局部输入状态变量。我们现在再将鼠标光标移回到他的窗口上．它仍然在运行长时间的进程。系统要向这个窗口发送wM_setcursor消息．但这个窗口不能取得这个消息．因为他仍然在循环。所以系统查看最近设置的鼠标光标形状(包含在线程的局部输入状态变量里)，并自动地将鼠标光标形状设置成这个形状(本例中是沙漏)。这样就给了用户可见的反馈消息．告诉用户．处理仍在进行，用户需要等待。
27.3将虚拟输入队列同局部输入状态挂接在一起
从上面的讨论我们可以看出这个输入模型是强壮的，因为每个线程都有自己的局部输入状态环境，并且在必要时每个线程可以连接到RIT或从RIT断开。有时候，我们可能想让两个或多个线程共享一组局部输入状态变量及一个虚拟输入队列。
可以利用AttachThreadInput函数来强制两个或多个线程共享同一个虚拟输入队列和一组局部输入状态变量。
可以通过多次调用AttachThreadInput函数让多个线程共享同一个虚拟输入队列和局部输入状态变量。
我们再考虑前面的例子，假定线程A调用AttachThreadInput，传递线程A的ID作为第一个参数，线程B的ID作为第二个参数，TRUE作为最后一个参数：
现在每个发往窗口A1、窗口B1或窗口B2的硬件输入事件都将添加到线程B的虚拟输入队列中。线程A的虚拟输入队列将不再接收输入事件，除非再一次调用AttachThreadInput并传递FALSE作为最后一个参数，将两个线程的输入队列分开。
当将两个线程的输入都挂接在一起时，就使线程共享单一的虚拟输入队列和同一组局部输入状态变量。但线程仍然使用自己的登记消息队列、发送消息队列、应答消息队列和唤醒标志（见第26章的讨论）。
如果让所有的线程都共享一个输入队列，就会严重削弱系统的强壮性。如果某一个线程接收一个按键消息并且挂起，其他的线程就不能接收任何输入了。所以应该尽量避免使用AttachThreadInput函数。
在某些情况下，系统隐式地将两个线程挂接在一起。第一种情况是当一个线程安装一个日志记录挂钩（journalrecordhook）或日志播放挂钩（journalplaybackhook）的时候。当挂钩被卸载时，系统自动恢复所有线程，这样线程就可以使用挂钩安装前它们所使用的相同输入队列。
当一个线程安装一个日志记录挂钩时，它是让系统将用户输入的所有硬件事件都通知它。
这个线程通常将这些信息保存或记录在一个文件上。因用户的输入必须按进入的次序来记录，所以系统中每个线程要共享一个虚拟输入队列，使所有的输入处理同步。
还有一些情况，系统会代替你隐式地调用AttachThreadInput。假定你的程序建立了两个线程。第一个线程建立了一个对话框。在这个对话框建立之后，第二个线程调用GreatWindow，使用WS_CHILD风格，并向这个子窗口的双亲传递对话框的句柄。系统用子窗口的线程调用AttachThreadInput，让子窗口的线程使用对话框线程所使用的输入队列。

