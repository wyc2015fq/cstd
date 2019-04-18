# 在DLL中用CRT静态库申请内存,EXE释放是不行的 - gauss的专栏 - CSDN博客
2013年01月04日 00:19:13[gauss](https://me.csdn.net/mathlmx)阅读数：485
因为malloc/free,new/delete都是调用HeapAlloc/HeapFree来实现来实现内存分配是释放的。
查看Windows的API可以看到，这两个函数都需要一个Heap的HANDLE做为参数。CRT库采用了全局变量来保存这个HANDLE。如果是CRT静态链接，CRT库的代码会链接到各个DLL中去，也包括这个全局变量。
也就是说，每个使用CRT静态链接的dll中都有一个自己的全局堆句柄，他们自己都在这个句柄上使用内存。当释放dll中分配的内存时由于使用的堆句柄不一致于是出错。
当使用CRT动态链接时，有于每个dll都是去调用CRT库的dll函数来分配和释放内存的，使用的是同一个句柄，所以就没有这个问题。
在Windows via C/C++的chap17中讲到，在通常情况下，一个dll文件同时被多处关联使用或exe文件同时有多个实例在运行，通过memory mapping在内存中实际只存在一份dll或exe的数据。如果dll或exe中的全局变量被修改，则通过copy-on-write，将被修改的内容所在page在内存中复制出一份，并将引用关联改为新的page。这样就不会影响到其他使用同一份page的程序，发生修改的进程中对应内容的地址修改为新的page地址。这样在设计dll时应尽量避免使用全局变量，否则根据copy-on-write的规则，很可能造成内存中存在多份内容大体相同，而仅有少量修改所导致的内存复制。
如果需要将dll或exe中的全局变量在多个实例中共享，可以采用以下方法。
// Tell the compiler to put this initializedvariable in its own Shared // section so it is shared by all instances of this application. #pragma data_seg("Shared") volatile LONG g_lApplicationInstances= 0; #pragma data_seg() // Tell the linker to make the Shared section // readable, writable, and shared. #pragma comment(linker, "/Section:Shared,RWS")本文转自：[http://www.mindair.net/2011/03/02/dll及多个相同exe之间共享静态数据/](http://www.mindair.net/2011/03/02/dll%E5%8F%8A%E5%A4%9A%E4%B8%AA%E7%9B%B8%E5%90%8Cexe%E4%B9%8B%E9%97%B4%E5%85%B1%E4%BA%AB%E9%9D%99%E6%80%81%E6%95%B0%E6%8D%AE/)
