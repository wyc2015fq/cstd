# Linux 内存泄露检查工具valgrind简析 - fanyun的博客 - CSDN博客
2019年02月23日 22:09:38[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：29
valgrind是linux检查内存泄露的常用工具，使我们在linux下编程c/c++程序的内存泄露检查变得方便多了
valgrind包含几个标准的工具，它们是：
1、memcheck
memcheck探测程序中内存管理存在的问题。它检查所有对内存的读/写操作，并截取所有的malloc/new/free/delete调用。因此memcheck工具能够探测到以下问题：
1）使用未初始化的内存
2）读/写已经被释放的内存
3）读/写内存越界
4）读/写不恰当的内存栈空间
5）内存泄漏
6）使用malloc/new/new[]和free/delete/delete[]不匹配。
2、cachegrind
cachegrind是一个cache剖析器。它模拟执行CPU中的L1, D1和L2 cache，因此它能很精确的指出代码中的cache未命中。如果你需要，它可以打印出cache未命中的次数，内存引用和发生cache未命中的每一行 代码，每一个函数，每一个模块和整个程序的摘要。如果你要求更细致的信息，它可以打印出每一行机器码的未命中次数。在x86和amd64 上，cachegrind通过CPUID自动探测机器的cache配置，所以在多数情况下它不再需要更多的配置信息了。
3、helgrind
helgrind查找多线程程序中的竞争数据。helgrind查找内存地址，那些被多于一条线程访问的内存地址，但是没有使用一致的锁就会被查出。这表示这些地址在多线程间访问的时候没有进行同步，很可能会引起很难查找的时序问题。
检查内存泄漏只需要用到第一个功能memcheck
命令如下：valgrind --tool=memcheck --leak-check=full 执行程序命令
比如执行二进制可执行文件a
valgrind --tool=memcheck --leak-check=full ./a
a最好是用-g选项编译的，便于在调试时发现哪一步出现了内存泄漏的问题。
