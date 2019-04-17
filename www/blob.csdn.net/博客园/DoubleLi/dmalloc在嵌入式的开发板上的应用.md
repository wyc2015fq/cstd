# dmalloc在嵌入式的开发板上的应用 - DoubleLi - 博客园






下面是我实际在开发环境里面做的dmalloc移植时候的一些随笔



配置PC的dmalloc环境
1. 首先把源码包打开，进入dmalloc文件夹
2. ./configure 配置Makefile，我是加了线程选项的，所以我的是./configure --enable-threads
3. 生成Makefile之后，需要修改makefile里面的编译选项 cc和cxx
   我用的是DM6467的开发环境，所以我的改成了arm_v5t_le-gcc和arm_v5t_le-g++，这里按照各自需要改成交叉编译的命令就好。
4. 然后make
5. make install
到此，环境配置完成

进到我们需要测试内存漏洞的代码，例如我们的代码在test文件夹里，名字叫mem.c。
1. 进入test
2. 用交叉编译工具编译arm_v5t_le-gcc mem.c -o mem -ldmalloc，这里记得加-ldmalloc这个编译选项，这样才能使用dmalloc。
执行成功后，生成的mem拷贝到开发板的某个目录，等待执行。

配置开发板的dmalloc执行环境
1. 从PC的/usr/local/bin 拷贝dmalloc 文件到开发板的目录/bin
2. 从PC的/usr/local/lib 拷贝相关的.a文件到开发板的目录/usr/lib
3. 从PC的/usr/local/include 拷贝dmalloc.h 文件到开发板的目录/usr/include
4. 输入配置文件命令 export DMALLOC_OPTIONS=log=logname,debug=0x3
   这里的logname是一个我们输入的文件，以后的编译debug信息会打印进去，0x3是debug的选项，详细的可以看[http://www.linuxfordevices.com/c/a/Linux-For-Devices-Articles/ELJonline-BRMemory-Leak-Detection-in-Embedded-Systems/](http://www.linuxfordevices.com/c/a/Linux-For-Devices-Articles/ELJonline-BRMemory-Leak-Detection-in-Embedded-Systems/)  我下面也把编译选项贴过来了。


将上面编译好的mem可执行程序拷贝到我们的开发板上，然后运行。

--------------------------
The following is a list of the tests and the corresponding bits to set in ``debug'':

    * none (nil): no functionality (0)

    * log-stats (lst): log general statistics (0x1)

    * log-non-free (lnf): log non-freed pointers (0x2)

    * log-known (lkn): log only known non-freed (0x4)

    * log-trans (ltr): log memory transactions (0x8)

    * log-admin (lad): log administrative info (0x20)

    * log-blocks (lbl): log blocks when heap-map (0x40)

    * log-bad-space (lbs): dump space from bad pointers (0x100)

    * log-nonfree-space (lns): dump space from non-freed pointers (0x200)

    * log-elapsed-time (let): log elapsed time for allocated pointer (0x40000)

    * log-current-time (lct): log current time for allocated pointer (0x80000)

    * check-fence (cfe): check fencepost errors (0x400)

    * check-heap (che): check heap adm structs (0x800)

    * check-lists (cli): check free lists (0x1000)

    * check-blank (cbl): check mem overwritten by alloc-blank, free-blank (0x2000)

    * check-funcs (cfu): check functions (0x4000)

    * force-linear (fli): force heap-space to be linear (0x10000)

    * catch-signals (csi): shut down program on SIGHUP, SIGINT, SIGTERM (0x20000)

    * realloc-copy (rco): copy all re-allocations (0x100000)

    * free-blank (fbl): overwrite freed memory space with BLANK_CHAR (0x200000)

    * error-abort (eab): abort immediately on error (0x400000)

    * alloc-blank (abl): overwrite newly alloced memory with BLANK_CHAR (0x800000)

    * heap-check-map (hcm): log heap-map on heap-check (0x1000000)

    * print-messages (pme): write messages to stderr (0x2000000)

    * catch-null (cnu): abort if no memory available (0x4000000)

    * never-reuse (nre): never reuse freed memory (0x8000000)

    * allow-free-null (afn): allow the frees of NULL pointers (0x20000000)

    * error-dump (edu): dump core on error and then continue (0x40000000)









