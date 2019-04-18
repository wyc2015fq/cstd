# Windows 下的进程间通讯及数据共享 - ljx0305的专栏 - CSDN博客
2009年09月02日 22:27:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：512标签：[windows																[通讯																[attributes																[security																[64bit																[api](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=64bit&t=blog)](https://so.csdn.net/so/search/s.do?q=security&t=blog)](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)](https://so.csdn.net/so/search/s.do?q=通讯&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[C++																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/380566)
### Windows 下的进程间通讯及数据共享
Windows 下有很多方法实现进程间通讯，比如用 socket，管道(Pipe)，信箱(Mailslot)，等等。但最基本最直接的还是使用内存共享。其他方法最终还是会绕道这里。
可想而知，如果物理内存只有一份，让这份内存在不同的进程中，映射到各自的虚拟地址空间上，每个进程都可以读取同一份数据，是一种最高效的数据交换方法。下面我们就讨论如何实现它。
共享内存在 Windows 中是用 FileMapping 实现的。我们可以用 CreateFileMapping 创建一个内存文件映射对象， CreateFileMapping 这个 API 将创建一个内核对象，用于映射文件到内存。这里，我们并不需要一个实际的文件，所以，就不需要调用 CreateFile 创建一个文件， hFile 这个参数可以填写 INVALID_HANDLE_VALUE 。但是，文件长度是需要填的。Windows 支持长达 64bit 的文件，但是这里，我们的需求一定不会超过 4G ， dwMaximumSizeHigh 一定是 0 ，长度填在 dwMaximumSizeLow 即可。然后调用 MapViewOfFile 映射到当前进程的虚拟地址上即可。一旦用完共享内存，再调用 UnmapViewOfFile 回收内存地址空间。
Windows 把 CreateFileMapping 和 MapViewOfFile 两个 API 分开做是有它的道理的。这是因为允许映射一个超过 4G 的文件，而地址空间最大只有 4G (实际上，一般用户的程序只能用到 2G) ， MapViewOfFile 就可以指定文件的 Offset 而只映射一部分。
在 CreateFileMapping 的最后一个参数 pszName 填写一个名字，那么别的进程就可以用这个名字去调用 OpenFileMapping 来打开这个 FileMapping 对象，在新的进程内作映射。 不过，通过约定字符串的方法似乎不太优雅。
一个优雅的方法是，用 DuplicateHandle 在新进程中复制一份 FileMapping 对象出来，然后想办法把 Handle 通知新进程，比如用消息的方式传递过去。
如果需要共享内存的两个进程是父子关系，那么我们可以不用消息传递的方式来通知 FileMapping 的 Handle 。父进程可以用继承 Handle 的方式直接把 FileMapping 的 Handle 传递到子进程中。当然，在 CreateFileMapping 时就应该设置可以被继承的属性。
大约是这样：
SECURITY_ATTRIBUTES sa;
sa.nLength=sizeof(sa);
sa.lpSecurityDescriptor=NULL;
sa.bInheritHandle=TRUE;
handle=CreateFileMapping(INVALID_HANDLE_VALUE,&sa,PAGE_READWRITE,0,size,NULL);
这样，在 CreateProcess 的时候，如果 bInheritHandles 参数为 TRUE ，所有有可被继承属性的内核对象都会被复制到子进程中。
注：内核对象的继承就是在 CreateProcess 创建子进程，但是子进程的主线程尚未活动之前，内核扫描当前进程中所有内核对象，检查出有可继承属性的那些，再用 DuplicateHandle 复制一份到子进程。由于是内核对象，在内核中实质只有一份，所有只是引用记数加一，父进程和子进程对同一内核对象的 Handle 一定是相同的。
复制内核对象的过程是由 CreateProcess 内部完成的，我们可以放心的把对象 Handle (和子进程相同) 通过命令行传递给子进程。或者，用环境变量传递也可以。
值得注意的是，子进程用完了这个 FileMapping 对象后一样需要 CloseHandle 减去引用计数。
备注：
CreateProcess 调用时，pszCommandLine 不能直接填上一个不可修改的字符串。例如：
CreateProcess("test.exe","test argument",...);
这样就是错误的，因为 "test argument" 会被编译器编译放到不可修改的数据段中。正确的方法是：
char cmdline[]="test argument";
CreateProcess("test.exe",cmdline,...);
这样，命令行的字符串就被放在堆栈上，是可以被读写的。
CreateProcess 的倒数第二个参数需要填写一个 STARTUPINFOW 结构，这个结构很复杂，通常填起来很麻烦。我们可以复制一份父进程的结构，再酌情修改。方法是：
STARTUPINFO si={sizeof(si)};
PROCESS_INFORMATION pi;
GetStartupInfo(&si);
CreateProcess(...,&si,& pi);
这里， STARTUPINFO 结构的第一个长度信息通常应该填上，保证 GetStartupInfo(&si); 的正确执行。 
引用:http://blog.codingnow.com/2005/10/interprocess_communications.html
