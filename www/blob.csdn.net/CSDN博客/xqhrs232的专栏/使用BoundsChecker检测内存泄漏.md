# 使用BoundsChecker检测内存泄漏 - xqhrs232的专栏 - CSDN博客
2013年03月19日 17:30:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：783
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://www.51testing.com/?uid-65703-action-viewspace-itemid-65101](http://www.51testing.com/?uid-65703-action-viewspace-itemid-65101)
相关网帖
1.[C/C++内存泄露及其检测工具](http://blog.csdn.net/looob/article/details/5734606)----[http://blog.csdn.net/looob/article/details/5734606](http://blog.csdn.net/looob/article/details/5734606)
使用BoundsChecker检测内存泄漏：
   BoundsChecker采用一种被称为 Code Injection的[**技术**]()，来截获对分配内存和释放内存的函数的调用。简单地说，当你的程序开始运行时，BoundsChecker的DLL被自动载入进程的地址空间（这可以通过system-level的Hook实现），然后它会修改进程中对内存分配和释放的函数调用，让这些调用首先转入它的代码，然后再执行原来的代码。BoundsChecker在做这些动作的时，无须修改被调试程序的源代码或工程配置文件，这使得使用它非常的简便、直接。
   这里我们以malloc函数为例，截获[**其他**]()的函数方法与此类似。
   需要被截获的函数可能在DLL中，也可能在程序的代码里。比如，如果静态连结C-Runtime Library，那么malloc函数的代码会被连结到程序里。为了截获住对这类函数的调用，BoundsChecker会动态修改这些函数的指令。
   以下两段汇编代码，一段没有BoundsChecker介入，另一段则有BoundsChecker的介入：
   126: _CRTIMP void * __cdecl malloc (
   127: size_t nSize
   128: )
   129: {
   00403C10 push ebp
   00403C11 mov ebp,esp
   130: return _nh_malloc_dbg(nSize, _newmode, _NORMAL_BLOCK, NULL, 0);
   00403C13 push 0
00403C15 push 0
   00403C17 push 1
   00403C19 mov eax,[__newmode (0042376c)]
   00403C1E push eax
   00403C1F mov ecx,dword ptr [nSize]
   00403C22 push ecx
   00403C23 call _nh_malloc_dbg (00403c80)
   00403C28 add esp,14h
   131: }
   以下这一段代码有BoundsChecker介入：
   126: _CRTIMP void * __cdecl malloc (
   127: size_t nSize
   128: )
   129: {
   00403C10 jmp 01F41EC8
   00403C15 push 0
   00403C17 push 1
   00403C19 mov eax,[__newmode (0042376c)]
   00403C1E push eax
00403C1F mov ecx,dword ptr [nSize]
   00403C22 push ecx
   00403C23 call _nh_malloc_dbg (00403c80)
   00403C28 add esp,14h
   131: }
   当BoundsChecker介入后，函数malloc的前三条汇编指令被替换成一条jmp指令，原来的三条指令被搬到地址01F41EC8处了。当程序进入malloc后先jmp到01F41EC8，执行原来的三条指令，然后就是BoundsChecker的天下了。大致上它会先记录函数的返回地址（函数的返回地址在stack上，所以很容易修改），然后把返回地址指向属于BoundsChecker的代码，接着跳到malloc函数原来的指令，也就是在00403c15的地方。当malloc函数结束的时候，由于返回地址被修改，它会返回到BoundsChecker的代码中，此时BoundsChecker会记录由malloc分配的内存的指针，然后再跳转到到原来的返回地址去。
   如果内存分配/释放函数在DLL中，BoundsChecker则采用另一种方法来截获对这些函数的调用。BoundsChecker通过修改程序的DLL Import Table让table中的函数地址指向自己的地址，以达到截获的目的。关于如何拦截[**Windows**]()的系统函数，《程序员》杂志2002年8期，《API钩子揭密（下）》，对修改导入地址表做了概要的描述。我就不再赘述。
   截获住这些分配和释放函数，BoundsChecker就能记录被分配的内存或资源的生命周期。接下来的问题是如何与源代码相关，也就是说当BoundsChecker检测到内存泄漏，它如何报告这块内存块是哪段代码分配的。答案是调试信息（Debug Information）。当我们编译一个Debug版的程序时，编译器会把源代码和二进制代码之间的对应关系记录下来，放到一个单独的文件里(.pdb)或者直接连结进目标程序中。有了这些信息，调试器才能完成断点设置，单步执行，查看变量等功能。BoundsChecker支持多种调试信息格式，它通过直接读取调试信息就能得到分配某块内存的源代码在哪个文件，哪一行上。使用Code
 Injection和Debug Information，使BoundsChecker不但能记录呼叫分配函数的源代码的位置，而且还能记录分配时的Call Stack，以及Call Stack上的函数的源代码位置。这在使用像MFC这样的类库时非常有用，以下我用一个例子来说明：
void ShowXItemMenu()
   {
   …
   CMenu menu;
   menu.CreatePopupMenu();
   //add menu items.
   menu.TrackPropupMenu();
   …
   }
   void ShowYItemMenu( )
   {
   …
   CMenu menu;
   menu.CreatePopupMenu();
   //add menu items.
   menu.TrackPropupMenu();
   menu.Detach();//this will cause HMENU leak
   …
   }
   BOOL CMenu::CreatePopupMenu()
   {
…
   hMenu = CreatePopupMenu();
   …
   }
   当调用ShowYItemMenu()时，我们故意造成HMENU的泄漏。但是，对于BoundsChecker来说被泄漏的HMENU是在class CMenu::CreatePopupMenu()中分配的。假设的你的程序有许多地方使用了CMenu的CreatePopupMenu()函数，如果只是告诉你泄漏是由CMenu::CreatePopupMenu()造成的，你依然无法确认问题的根结到底在哪里，在ShowXItemMenu()中还是在ShowYItemMenu()中，或者还有其它的地方也使用了CreatePopupMenu()？有了Call
 Stack的信息，问题就容易了。BoundsChecker会如下报告泄漏的HMENU的信息：
|Function|File|Line|
|----|----|----|
|CMenu::CreatePopupMenu|E:\8168\vc98\mfc\mfc\include\afxwin1.inl|1009|
|ShowYItemMenu|E:\testmemleak\mytest.cpp|100|
|这里省略了其他的函数调用| | |
   如此，我们很容易找到发生问题的函数是ShowYItemMenu()。当使用MFC之类的类库编程时，大部分的API调用都被封装在类库的class里，有了Call Stack信息，我们就可以非常容易的追踪到真正发生泄漏的代码。
   记录Call Stack信息会使程序的运行变得非常慢，因此默认情况下BoundsChecker不会记录Call Stack信息。可以按照以下的步骤打开记录Call Stack信息的选项开关：
   1. 打开菜单：BoundsChecker|Setting…
   2. 在Error Detection页中，在Error Detection Scheme的List中选择Custom
   3. 在Category的Combox中选择 Pointer and leak error check
   4. 钩上Report Call Stack复选框
   5. 点击Ok
基于Code Injection，BoundsChecker还提供了API Parameter的校验功能，memory over run等功能。这些功能对于程序的开发都非常有益。由于这些内容不属于本文的主题，所以不在此详述了。
   尽管BoundsChecker的功能如此强大，但是面对隐式内存泄漏仍然显得苍白无力。所以接下来我们看看如何用Performance Monitor检测内存泄漏。
   使用Performance Monitor检测内存泄漏
   NT的内核在设计过程中已经加入了系统监视功能，比如CPU的使用率，内存的使用情况，I/O操作的频繁度等都作为一个个Counter，应用程序可以通过读取这些Counter了解整个系统的或者某个进程的运行状况。Performance Monitor就是这样一个应用程序。
   为了检测内存泄漏，我们一般可以监视Process对象的Handle Count，Virutal Bytes 和Working Set三个Counter。Handle Count记录了进程当前打开的HANDLE的个数，监视这个Counter有助于我们发现程序是否有Handle泄漏；Virtual Bytes记录了该进程当前在虚地址空间上使用的虚拟内存的大小，NT的内存分配采用了两步走的方法，首先，在虚地址空间上保留一段空间，这时[**操作系统**]()并没有分配物理内存，只是保留了一段地址。然后，再提交这段空间，这时操作系统才会分配物理内存。所以，Virtual
 Bytes一般总大于程序的Working Set。监视Virutal Bytes可以帮助我们发现一些系统底层的问题; Working Set记录了操作系统为进程已提交的内存的总量，这个值和程序申请的内存总量存在密切的关系，如果程序存在内存的泄漏这个值会持续增加，但是Virtual Bytes却是跳跃式增加的。
   监视这些Counter可以让我们了解进程使用内存的情况，如果发生了泄漏，即使是隐式内存泄漏，这些Counter的值也会持续增加。但是，我们知道有问题却不知道哪里有问题，所以一般使用Performance Monitor来验证是否有内存泄漏，而使用BoundsChecker来找到和解决问题。
   当Performance Monitor显示有内存泄漏，而BoundsChecker却无法检测到，这时有两种可能：第一种，发生了偶发性内存泄漏。这时你要确保使用Performance Monitor和使用BoundsChecker时，程序的运行环境和操作方法是一致的。第二种，发生了隐式的内存泄漏。这时你要重新审查程序的设计，然后仔细研究Performance Monitor记录的Counter的值的变化图，分析其中的变化和程序运行逻辑的关系，找到一些可能的原因。这是一个痛苦的过程，充满了假设、猜想、验证、失败，但这也是一个积累经验的绝好机会。
   总结
   内存泄漏是个大而复杂的问题，即使是Java和.Net这样有Gabarge Collection机制的环境，也存在着泄漏的可能，比如隐式内存泄漏。由于篇幅和能力的限制，本文只能对这个主题做一个粗浅的研究。其他的问题，比如多模块下的泄漏检测，如何在程序运行时对内存使用情况进行分析等等，都是可以深入研究的题目。如果您有什么想法，建议或发现了某些错误，欢迎和我交流。 
