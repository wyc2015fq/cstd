# Linux c 开发 - 内存管理器ptmalloc - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年03月09日 10:07:11[initphp](https://me.csdn.net/initphp)阅读数：5227
所属专栏：[Linux c 开发系列](https://blog.csdn.net/column/details/mylinuxc.html)









### 内存布局

了解ptmalloc内存管理器，就必须得先了解操作系统的内存布局方式。通过下面这个图，我很很清晰的可以看到堆、栈等的内存分布。


X86平台LINUX进程内存布局：

![](https://img-blog.csdn.net/20160309100319232?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

上图就是linux操作系统的内存布局。内存从低到高分别展示了操作系统各个模块的内存分布。


Test Segment：存放程序代码，只读，编译的时候确定

Data Segment：存放程序运行的时候就能确定的数据，可读可写

BBS Segment：定义而没有初始化的全局变量和静态变量

Heap：堆。堆的内存地址由低到高。

Mmap：映射区域。

Stack：栈。编译器自动分配和释放。内存地址由高到低




### ptmalloc内存管理器

ptmalloc是glibc默认的内存管理器。我们常用的malloc和free就是由ptmalloc内存管理器提供的基础内存分配函数。ptmalloc有点像我们自己写的内存池，当我们通过malloc或者free函数来申请和释放内存的时候，ptmalloc会将这些内存管理起来，并且通过一些策略来判断是否需要回收给操作系统。这样做的最大好处就是：让用户申请内存和释放内存的时候更加高效。

为了内存分配函数malloc的高效性，ptmalloc会预先向操作系统申请一块内存供用户使用，并且ptmalloc会将已经使用的和空闲的内存管理起来；当用户需要销毁内存free的时候，ptmalloc又会将回收的内存管理起来，根据实际情况是否回收给操作系统




#### 设计假设

ptmalloc在设计时折中了高效率，高空间利用率，高可用性等设计目标。所以有了下面一些设计上的假设条件：


1. 具有长生命周期的大内存分配使用mmap。

2. 特别大的内存分配总是使用mmap。

3. 具有短生命周期的内存分配使用brk。

4. 尽量只缓存临时使用的空闲小内存块，对大内存块或是长生命周期的大内存块在释放时都直接归还给操作系统。

5. 对空闲的小内存块只会在malloc和free的时候进行合并，free时空闲内存块可能放入pool中，不一定归还给操作系统。

6. 收缩堆的条件是当前free的块大小加上前后能合并chunk的大小大于64KB、，并且堆顶的大小达到阈值，才有可能收缩堆，把堆最顶端的空闲内存返回给操作系统。

7. 需要保持长期存储的程序不适合用ptmalloc来管理内存。

8. 不停的内存分配ptmalloc会对内存进行切割和合并，会导致一定的内存碎片




#### 主分配区和非主分配区

ptmalloc的内存分配器中，为了解决多线程锁争夺问题，分为主分配区main_area和非主分配区no_main_area。

1. 每个进程有一个主分配区，也可以允许有多个非主分配区。

2. 主分配区可以使用brk和mmap来分配，而非主分配区只能使用mmap来映射内存块

3. 非主分配区的数量一旦增加，则不会减少。

4. 主分配区和非主分配区形成一个环形链表进行管理。





#### chunk 内存块的基本组织单元

ptmalloc通过chunk的数据结构来组织每个内存单元。当我们使用malloc分配得到一块内存的时候，这块内存就会通过chunk的形式被记录到glibc上并且管理起来。你可以把它想象成自己写内存池的时候的一个内存数据结构。

chunk的结构可以分为**使用中的chun**k和**空闲的chunk**。

使用中的chunk和空闲的chunk数据结构基本项同，但是会有一些设计上的小技巧，巧妙的节省了内存。


**使用中的chunk：**

![](https://img-blog.csdn.net/20160309202957354?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


1. chunk指针指向chunk开始的地址；mem指针指向用户内存块开始的地址。

2. p=0时，表示前一个chunk为空闲，prev_size才有效

3. p=1时，表示前一个chunk正在使用，prev_size无效    p主要用于内存块的合并操作

4. ptmalloc 分配的第一个块总是将p设为1, 以防止程序引用到不存在的区域

5. M=1 为mmap映射区域分配；M=0为heap区域分配

6. A=1 为非主分区分配；A=0 为主分区分配





**空闲的chunk**

![](https://img-blog.csdn.net/20160309203009527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

1. 空闲的chunk会被放置到**空闲的链表bins**上。当用户申请内存malloc的时候，会先去查找空闲链表bins上是否有合适的内存。

2. fp和bp分别指向前一个和后一个空闲链表上的chunk

3. fp_nextsize和bp_nextsize分别指向前一个空闲chunk和后一个空闲chunk的大小，主要用于在空闲链表上快速查找合适大小的chunk。

4. fp、bp、fp_nextsize、bp_nextsize的值都会存在原本的用户区域，这样就不需要专门为每个chunk准备单独的内存存储指针了。




#### 空闲链表bins

当用户使用free函数释放掉的内存，ptmalloc并不会马上交还给操作系统（这边很多时候我们明明执行了free函数，但是进程内存并没有回收就是这个原因），而是被ptmalloc本身的空闲链表bins管理起来了，这样当下次进程需要malloc一块内存的时候，ptmalloc就会从空闲的bins上寻找一块合适大小的内存块分配给用户使用。这样的好处可以避免频繁的系统调用，降低内存分配的开销。

![](https://img-blog.csdn.net/20160310213639493?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


ptmalloc一共维护了128bin。每个bins都维护了大小相近的双向链表的chunk。

通过上图这个bins的列表就能看出，当用户调用malloc的时候，能很快找到用户需要分配的内存大小是否在维护的bin上，如果在某一个bin上，就可以通过双向链表去查找合适的chunk内存块给用户使用。

**1. fast bins**。fast bins是bins的高速缓冲区，大约有10个定长队列。当用户释放一块不大于max_fast（默认值64）的chunk（一般小内存）的时候，会默认会被放到fast bins上。当用户下次需要申请内存的时候首先会到fast bins上寻找是否有合适的chunk，然后才会到bins上空闲的chunk。ptmalloc会遍历fast bin，看是否有合适的chunk需要合并到bins上。

**2. unsorted bin**。是bins的一个缓冲区。当用户释放的内存大于max_fast或者fast bins合并后的chunk都会进入unsorted bin上。当用户malloc的时候，先会到unsorted bin上查找是否有合适的bin，如果没有合适的bin，ptmalloc会将unsorted bin上的chunk放入bins上，然后到bins上查找合适的空闲chunk。

**3. small bins和large bins**。small bins和large bins是真正用来放置chunk双向链表的。每个bin之间相差8个字节，并且通过上面的这个列表，可以快速定位到合适大小的空闲chunk。前64个为small bins，定长；后64个为large bins，非定长。

**4. Top chunk**。并不是所有的chunk都会被放到bins上。top chunk相当于分配区的顶部空闲内存，当bins上都不能满足内存分配要求的时候，就会来top chunk上分配。


**5. mmaped chunk。**当分配的内存非常大（大于分配阀值，默认128K）的时候，需要被mmap映射，则会放到mmaped chunk上，当释放mmaped chunk上的内存的时候会直接交还给操作系统。




#### 内存分配malloc流程

1. 获取分配区的锁，防止多线程冲突。

2. 计算出需要分配的内存的chunk实际大小。

3. 判断chunk的大小，如果小于max_fast（64b），则取fast bins上去查询是否有适合的chunk，如果有则分配结束。

4. chunk大小是否小于512B，如果是，则从small bins上去查找chunk，如果有合适的，则分配结束。

5. 继续从 unsorted bins上查找。如果unsorted bins上只有一个chunk并且大于待分配的chunk，则进行切割，并且剩余的chunk继续扔回unsorted bins；如果unsorted bins上有大小和待分配chunk相等的，则返回，并从unsorted bins删除；如果unsorted bins中的某一chunk大小 属于small bins的范围，则放入small bins的头部；如果unsorted bins中的某一chunk大小 属于large bins的范围，则找到合适的位置放入。

6. 从large bins中查找，找到链表头后，反向遍历此链表，直到找到第一个大小 大于待分配的chunk，然后进行切割，如果有余下的，则放入unsorted bin中去，分配则结束。

7. 如果搜索fast bins和bins都没有找到合适的chunk，那么就需要操作top chunk来进行分配了（top chunk相当于分配区的剩余内存空间）。判断top chunk大小是否满足所需chunk的大小，如果是，则从top chunk中分出一块来。

8. 如果top chunk也不能满足需求，则需要扩大top chunk。主分区上，如果分配的内存小于分配阀值（默认128k），则直接使用brk()分配一块内存；如果分配的内存大于分配阀值，则需要mmap来分配；非主分区上，则直接使用mmap来分配一块内存。通过mmap分配的内存，就会放入mmap chunk上，mmap chunk上的内存会直接回收给操作系统。




#### 内存释放free流程

1. 获取分配区的锁，保证线程安全。

2. 如果free的是空指针，则返回，什么都不做。

3. 判断当前chunk是否是mmap映射区域映射的内存，如果是，则直接munmap()释放这块内存。前面的已使用chunk的数据结构中，我们可以看到有**M**来标识是否是mmap映射的内存。

4. 判断chunk是否与top chunk相邻，如果相邻，则直接和top chunk合并（和top chunk相邻相当于和分配区中的空闲内存块相邻）。转到步骤8

5. 如果chunk的大小大于max_fast（64b），则放入unsorted bin，并且检查是否有合并，有合并情况并且和top chunk相邻，则转到步骤8；没有合并情况则free。


6. 如果chunk的大小小于 max_fast（64b），则直接放入fast bin，fast bin并没有改变chunk的状态。没有合并情况，则free；有合并情况，转到步骤7

7. 在fast bin，如果当前chunk的下一个chunk也是空闲的，则将这两个chunk合并，放入unsorted bin上面。合并后的大小如果大于64KB，会触发进行fast bins的合并操作，fast bins中的chunk将被遍历，并与相邻的空闲chunk进行合并，合并后的chunk会被放到unsorted bin中，fast bin会变为空。合并后的chunk和topchunk相邻，则会合并到topchunk中。转到步骤8

8. 判断top chunk的大小是否大于mmap收缩阈值（默认为128KB），如果是的话，对于主分配区，则会试图归还top chunk中的一部分给操作系统。free结束。





#### mallopt 参数调优

1. M_MXFAST：用于设置fast bins中保存的chunk的最大大小，默认值为64B。最大80B

2. M_TRIM_THRESHOLD：用于设置mmap收缩阈值，默认值为128KB。

3. M_MMAP_THRESHOLD：M_MMAP_THRESHOLD用于设置mmap分配阈值，默认值为128KB。当用户需要分配的内存大于mmap分配阈值，ptmalloc的malloc()函数其实相当于mmap()的简单封装，free函数相当于munmap()的简单封装。

4. M_MMAP_MAX：M_MMAP_MAX用于设置进程中用mmap分配的内存块的地址段数量，默认值为65536

5. M_TOP_PAD：该参数决定了，当libc内存管理器调用brk释放内存时，堆顶还需要保留的空闲内存数量。该值缺省为 0.





#### 使用注意事项

为了避免Glibc内存暴增，需要注意：

1. 后分配的内存先释放，**因为ptmalloc收缩内存是从top chunk开始，如果与top chunk相邻的chunk不能释放，top chunk以下的chunk都无法释放**。

2. Ptmalloc不适合用于管理长生命周期的内存，特别是持续不定期分配和释放长生命周期的内存，这将导致ptmalloc内存暴增。

3. 多线程分阶段执行的程序不适合用ptmalloc，这种程序的内存更适合用内存池管理

4. 尽量减少程序的线程数量和避免频繁分配/释放内存。**频繁分配，会导致锁的竞争，最终导致非主分配区增加，内存碎片增高，并且性能降低。**

5. 防止内存泄露，ptmalloc对内存泄露是相当敏感的，根据它的内存收缩机制，如果与top chunk相邻的那个chunk没有回收，将导致top chunk一下很多的空闲内存都无法返回给操作系统。

6. 防止程序分配过多内存，或是由于Glibc内存暴增，导致系统内存耗尽，程序因OOM被系统杀掉。预估程序可以使用的最大物理内存大小，配置系统的/proc/sys/vm/overcommit_memory，/proc/sys/vm/overcommit_ratio，以及使用ulimt –v限制程序能使用虚拟内存空间大小，防止程序因OOM被杀掉。




参考整理：http://www.valleytalk.org/wp-content/uploads/2015/02/glibc%E5%86%85%E5%AD%98%E7%AE%A1%E7%90%86ptmalloc%E6%BA%90%E4%BB%A3%E7%A0%81%E5%88%86%E6%9E%901.pdf










