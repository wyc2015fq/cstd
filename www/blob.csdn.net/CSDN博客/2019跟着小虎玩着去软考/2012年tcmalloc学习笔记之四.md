# 2012年tcmalloc学习笔记之四 - 2019跟着小虎玩着去软考 - CSDN博客
2012年07月12日 17:46:43[littletigerat](https://me.csdn.net/littletigerat)阅读数：2554标签：[pointers																[object																[class																[list																[os																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=os&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=pointers&t=blog)
个人分类：[源码学习																[C/C++																[Linux](https://blog.csdn.net/littletigerat/article/category/613914)](https://blog.csdn.net/littletigerat/article/category/666612)](https://blog.csdn.net/littletigerat/article/category/665291)
**2012****年****tcmalloc****学习笔记之四**
# 一、线程缓存的垃圾收集机制
1.什么时候进行线程缓存垃圾收集，需要满足什么条件
某个线程缓存当缓存中所有对象的总共大小超过2MB的时候，会对他进行垃圾收集。
2.线程缓存自由列表与中央自由列表的联系与区别；
我们会遍历缓存中所有的自由列表并且将一定数量的对象从自由列表移到对于得中央列表中。
3.为什么不会因为程序有大量线程而过度浪费内存？
垃圾收集阈值会自动根据线程数量的增加而减少。
4.自由列表中移除的对象的数量是如何确定的？
从某个自由列表中移除的对象的数量是通过使用一个每列表的低水位线`L`来确定的。`L`记录了自上一次垃圾收集以来列表最短的长度。注意，在上一次的垃圾收集中我们可能只是将列表缩短了`L`个对象而没有对中央列表进行任何额外访问。我们利用这个过去的历史作为对未来访问的预测器并将`L/2`个对象从线程缓存自由列表中移到相应的中央自由列表中。这个算法有个很好的特性是，如果某个线程不再使用某个特定的尺寸时，该尺寸的所有对象都会很快从线程缓存被移到中央自由列表，然后可以被其他缓存利用。
# 二、使用环境变量LD_PRELOAD
环境变量LD_PRELOAD指定程序运行时优先加载的动态连接库，这个动态链接库中的符号优先级是最高的。
标准C的各种函数都是存放在libc.so的文件中，在程序运行时自动链接。使用LD_PRELOAD后，自己编写的malloc的加载顺序高于glibc中的malloc，这样就实现了替换。
`g++hack.cpp -shared -fPIC -o hack.so`
`LD_PRELOAD="./hack.so"./checkpasswd abc`
# `三、链接选项`
`只需添加链接选项``-ltcmalloc``即可`
`g++wrap.cpp -o wrap -Wl,-wrap,malloc`
## **方法： 控制链接过程**
ld中有一个选项–wrap，当查找某个符号时，它优先先解析__wrap_symbol,　解析不到才去解析symbol。例如：
|`1`|`void``*__wrap_malloc (size_t``c)`|
|`2`|`{`|
|`3`|`     ``printf``("malloc called with %u\n", c);`|
|`4`|`     ``return``__real_malloc (c);`|
|`5`|`}`|
当其它文件与你实现__wrap_malloc函数的文件链接时使用–wrapmalloc，则所有到malloc的调用都是会链接到__wrap_malloc上。只有调用__reall_malloc时才会调用真正的malloc
|`1`|`#include<stdio.h>`|
|`2`|`#include<stdlib.h>`|
|`3`||
|`4`|`extern``"C"``void``*__real_malloc(size_t);`|
|`5`|`extern``"C"``void``*__real_free(void````*);```|
|`6`||
|`7`|`extern``"C"``void``*__wrap_malloc(size_t``c)`|
|`8`|`{`|
|`9`|`    ``printf("MyMALLOC called: %d\n", c);`|
|`10`|`    ``return``__real_malloc(c);`|
|`11`|`}`|
|`12`|`extern``"C"``void``*__wrap_free(void````*ptr)```|
|`13`|`{`|
|`14`|`    ``printf("MyFREE called: 0x%08X\n", ptr);`|
|`15`|`    ``return``__real_free(ptr);`|
|`16`|`}`|
|`17`||
|`18`|`int``main (int``argc,char``*argv[])`|
|`19`|`{`|
|`20`|`    ``void``*ptr = malloc(12);`|
|`21`|`    ``free(ptr);`|
|`22`|`    ``return``0;`|
|`23`|`}`|
编译：
|`1`|`g++wrap.cpp -o wrap -Wl,-wrap,malloc`|
结果：
MyMALLOC called: 12
My FREE called: 0×00501010
# `四、具体应用场景：`
`mysql`
`nginx`
`squid`
# `五、为``Class``增加内存池的机制`
`专用的‘对象池’可以比通用的‘内存池’性能高两个数量级。`
```
```
`通过宏定义（``DECL_MEM_POOL,IMPL_MEM_POOL``）可以很快速的为``class``增加``pool``能力，还可以在单线程的环境下去掉锁。`
# `六、内存请求`
`tcmalloc``将内存请求分为两类，大对象请求和小对象请求，大对象为``>=32K``的对象。`
```
|
tcmalloc
```
`会为每个线程分配线程局部缓冲`
```
```
`对于小对象请求，可以直接从线程局部缓冲区获取，如果线程局部缓冲区没有空闲内存，则从``centralheap``中一次性获取一连串小对象。`
```
tcmalloc
```
`对于小内存，按``8``的整数次倍分配，对于大内存，按``4K``的整数次倍分配。`
```
```
`这样做有两个好处，一是分配的时候比较快。二是短期的收益比较大，分配的小内存至多浪费``7``个字节，大内存则``4K`
# `七、内存检测工具`
`valgrind`
# `八、直接通过``./configure``来修改参数`
`./configure–enable-frame_pointers`
`./configure--enable-frame_pointers && make && sudo make install`
`sudoldconfig`
`g++.... -ltcmalloc (link static lib)`
`修改参数`
`tcmalloc``每个线程默认最大缓存``16M``空间，所以当线程多的时候其占用的空间还是非常可观的，在``common.h``中有几个参数是控制缓存空间的，可以做合理的修改（只可个人做实验，注意法律问题）：`
`1.``降低每个线程的缓存空间，`可以修改common.h中的kMaxThreadCacheSize，比如2M
2.降低所有线程的缓存空间的总大小，可以修改common.h中的kDefaultOverallThreadCacheSize，比如20M
3.尽快将free的空间还给centrallist，可以将kMaxOverages改小一点，比如1
还可以定期让tcmalloc归还空间给OS，
`#include"google/malloc_extension.h"`
# `九、``tcmalloc``向系统申请空间有三种方式`
`sbrk``，``mmap``，``/dev/mem``文件`
`默认是三种都``try``的，一种不行换另外一种。`
# `十、线程如何释放资源`
`1.``释放某个``object`
2.找到该object所在的span
3.如果该span中所有object都被释放，则释放该span到对应的可用列表，在释放的过程中，尝试将该span跟左右spansmerge成更大的span
4.如果当前threadcache的free空间大于指定预置，归还部分空间给centrallist
5.central list也会试图通过释放可用span列表的最后几个span来将不用的空间归还给OS
tcmalloc向OS申请/释放资源是以span为单位的。
tcmalloc里面不少实现值得称道，比如pagesize到void*的mapping方式，添加/移除链表元素的时候利用结构体内存布局直接赋值，span/page/item的内存层次结构等，值得一看。
# 十一、线程如何申请资源：
1.首先根据申请空间的大小从当前线程的可用内存块里面找（每个进程维护一组链表，每个链表代表一定大小的可用空间）
2.如果step1没有找到，则到centrallist里面查找（centrallist跟线程各自维护的list结构很像，为不同的size各自维护一组可用空间列表）
3.如果step2 central list也没有找到，则计算分配size个字节需要分配多少page（变量：class_to_pages）
4.根据pagemap查找page对应的可用的span列表，如果找到了，则直接返回span，centrallist会将该span切割成合适的大小放入对应的列表中，然后交给threadcache
5.如果step4没有找到可用的span，则向OS直接申请，然后步骤同step4。
# `十二、如何修改``libunwind``的参数`
`1``、``libunwind``的编译参数改为：`
```
CFLAGS=-fPIC./configure
make CFLAGS=-fPIC
make CFLAGS=-fPIC install
```
# `十三、如何指定链接库的路径`
echo“/usr/local/lib” >/etc/ld.so.conf.d/usr_local_lib.conf
/sbin/ldconfig
echo“/usr/local/lib” > /etc/ld.so.conf.d/usr_local_lib.conf
`/sbin/ldconfig`
