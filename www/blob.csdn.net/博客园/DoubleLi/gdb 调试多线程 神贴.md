# gdb 调试多线程 神贴 - DoubleLi - 博客园






gdb 调试多线程
如果目标进程已经core dump了，那么 gdb -c core xxx   xxx是对应的程序文件。
如果目标进程还在运行，通常此时用于调试线程死锁的情况。有两种方法
一是 gdb -p xxx  xxx是该进程的进程ID
或者用gcore xxx先获取对应进程的core，他会生成一个core文件 core.xxx

进入gdb后
(gdb) info threads
可以列出所有的线程，缺省设为当前的线程前面有一个*号
比如
gdb) info thread
    9 system thread 154262  Priority:154  0xc00000000042f670:0 in __ksleep
   +0x30 () from /usr/lib/hpux64/libc.so.1
    4 system thread 153674  Priority:168  0xc0000000004367d0:0 in _nanosleep2_sys+0x30 () from /usr/lib/hpux64/libc.so.1
    3 system thread 153673  Priority:168  0xc0000000004367d0:0 in _nanosleep2_sys+0x30 () from /usr/lib/hpux64/libc.so.1
    2 system thread 153672  Priority:154  0xc00000000042f670:0 in __ksleep
   +0x30 () from /usr/lib/hpux64/libc.so.1
*   1 system thread 153671  Priority:154  0xc000000000432ef0:0 in _read_sys
   +0x30 () from /usr/lib/hpux64/libc.so.1

这是1个死锁的例子，可以看到线程9 和线程2都停在 __ksleep上。
如果想看各个线程的详细堆栈信息，比如要看9的
gdb)thread 9
把当前线程设成9，然后就可以查看相关信息
比如
gdb)bt
将列出栈的调用情况，以及对应源代码中的位置，此时谨慎察看对应代码，一般必有结果
[http://blog.sina.com.cn/s/blog_62dc94eb0100flyn.html](http://blog.sina.com.cn/s/blog_62dc94eb0100flyn.html)









