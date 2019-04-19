# 查询linux系统中空闲内存/内存使用状态查看/剩余内存查看 - 三少GG - CSDN博客
2014年06月03日 17:03:26[三少GG](https://me.csdn.net/scut1135)阅读数：10362
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
查询linux系统中空闲内存/内存使用状态查看/剩余内存查看
本文作者：宗国锋 宗国锋官方网站 转载请注明原文链接[http://www.zongguofeng.cn/2010/0223/272.html](http://www.zongguofeng.cn/2010/0223/272.html)
下面介绍使用top和free命令查看系统中空闲内存
所以你执行top命令看到的
[root@linuxzgf ~]# top
![\](http://www.zongguofeng.cn/uploadfile/2010/0223/20100223115625128.png)
Mem: 8174492k total, 7124268k used,并不是代表你的应用程序已经使用了7.1的内存,这7.1G是包含了:应用程序内存 + 缓冲 + 缓存的内存的,需要用free命令查看.
下面是一个例子(单位是MB): 
[root@linuxzgf ~]# free -m
                    total       used       free     shared    buffers     cached
Mem:          7982       6811       1171          0        350       5114
-/+ buffers/cache:       1346       6636
Swap:        16935         11      16924
[root@linuxzgf ~]#
在这里例子中,应用程序只使用了1346MB内存,还有6636MB空闲内存可以使用. 
一些简单的计算方法： 
物理已用内存 = 实际已用内存 - 缓冲 - 缓存 
= 6811M - 350M - 5114M
物理空闲内存 = 总物理内存 - 实际已用内存 + 缓冲 + 缓存 
应用程序可用空闲内存 = 总物理内存 - 实际已用内存 
应用程序已用内存 = 实际已用内存 - 缓冲 - 缓存
原始解释：转至互联网：
Linux的基本原则是没有资源应该被浪费.因此核心会使用尽可能多的RAM,来缓存来自本地和远程的文件系统的信息.系统做读写操作的时候,会将与当前运行的进程相关的数据尽量存储在RAM里.系统报告的缓存是缓冲和页缓存两者之和.缓存并不是在进程结束的时候被回收(你可能很快会启动另外一个进程,需要同样的数据),而是随需回收–比如,当你启动一个需要大量内存的进程时,Linux核心会从内存中回收缓存,将得到的内存分配给新的进程. 
有些区域,比如匿名内存映射(mmps)和共享内存区域,它们被报告为缓存,但不是被核心直接释放.一般的缓存不映射到进程的地址空间,仅仅是简单的核心映射,而这些特别的缓存映射到所有挂接到它们上面的进程. 

### linux命令free详解
[houfeng](http://www.taobaotesting.com/users/qa/blogs?uid=67)发表于：2009-06-29浏览：18741次评论：[3次](http://www.taobaotesting.com/blogs/qa?bid=2265#reply-comment)所属分类： [性能测试](http://www.taobaotesting.com/categories/qa/blogs?cid=74)
前段时间有个项目的用C写的，性能测试时发现内存泄露问题。关于怎么观察内存使用问题，free是很好用的一个命令。
**bash-3.00$ free**
total       used       free     shared    buffers     cached
Mem:       1572988    1509260      63728          0      62800     277888
-/+ buffers/cache:    1168572     404416
Swap:      2096472      16628    2079844
Mem：表示物理内存统计
-/+ buffers/cached：表示物理内存的缓存统计
Swap：表示硬盘上交换分区的使用情况，这里我们不去关心。
系统的总物理内存：255268Kb（256M），但系统当前真正可用的内存b并不是第一行free 标记的 16936Kb，它仅代表未被分配的内存。
第1行 Mem：total：表示物理内存总量。
used：表示总计分配给缓存（包含buffers 与cache ）使用的数量，但其中可能部分缓存并未实际使用。
free：未被分配的内存。
shared：共享内存，一般系统不会用到，这里也不讨论。
buffers：系统分配但未被使用的buffers 数量。
cached：系统分配但未被使用的cache 数量。buffer 与cache 的区别见后面。 total = used + free 第2行 -/+ buffers/cached：used：也就是第一行中的used - buffers-cached 也是实际使用的内存总量。
free：未被使用的buffers 与cache 和未被分配的内存之和，这就是系统当前实际可用内存。 free 2= buffers1 + cached1 + free1 //free2为第二行、buffers1等为第一行
buffer 与cache 的区别
A buffer is something that has yet to be “written” to disk. A cache is something that has been “read” from the disk and stored for later use第3行：第三行所指的是从应用程序角度来看，对于应用程序来说，buffers/cached
 是等于可用的，因为buffer/cached是为了提高文件读取的性能，当应用程序需在用到内存的时候，buffer/cached会很快地被回收。
所以从应用程序的角度来说，可用内存=系统free memory+buffers+cached.
接下来解释什么时候内存会被交换，以及按什么方交换。
当可用内存少于额定值的时候，就会开会进行交换.
如何看额定值（RHEL4.0）：
#cat /proc/meminfo
交换将通过三个途径来减少系统中使用的物理页面的个数：
1.减少缓冲与页面cache的大小，
2.将系统V类型的内存页面交换出去，
3.换出或者丢弃页面。(Application 占用的内存页，也就是物理内存不足）。
事实上，少量地使用swap是不是影响到系统性能的。
下面是buffers与cached的区别。
buffers是指用来给块设备做的缓冲大小，他只记录文件系统的metadata以及 tracking in-flight pages.
cached是用来给文件做缓冲。
那就是说：buffers是用来存储，目录里面有什么内容，权限等等。
而cached直接用来记忆我们打开的文件，如果你想知道他是不是真的生效，你可以试一下，先后执行两次命令#man X ,你就可以明显的感觉到第二次的开打的速度快很多。
实验：在一台没有什么应用的机器上做会看得比较明显。记得实验只能做一次，如果想多做请换一个文件名。
#free
#man X
#free
#man X
#free
你可以先后比较一下free后显示buffers的大小。
另一个实验：
#free
#ls /dev
#free
你比较一下两个的大小，当然这个buffers随时都在增加，但你有ls过的话，增加的速度会变得快，这个就是buffers/chached的区别。
因为Linux将你暂时不使用的内存作为文件和数据缓存，以提高系统性能，当你需要这些内存时，系统会自动释放（不像windows那样，即使你有很多空闲内存,他也要访问一下磁盘中的pagefiles）
使用free命令
将used的值减去   buffer和cache的值就是你当前真实内存使用 ————–对**操作系统**来讲是Mem的参数.buffers/cached 都是属于被使用,所以它认为free只有**16936**.
对**应用程序**来讲是(-/+ buffers/cach).buffers/cached 是等同可用的，因为buffer/cached是为了提高程序执行的性能，当程序使用内存时，buffer/cached会很快地被使用。所以,以应用来看看,以(-/+
 buffers/cache)的free和used为主.所以我们看这个就好了.另外告诉大家一些常识.Linux为了提高磁盘和内存存取效率, Linux做了很多精心的设计, 除了对dentry进行缓存(用于VFS,加速文件路径名到inode的转换), 还采取了两种主要Cache方式：Buffer Cache和Page Cache。前者针对磁盘块的读写，后者针对文件inode的读写。这些Cache能有效缩短了
 I/O系统调用(比如read,write,getdents)的时间。**记住内存是拿来用的,不是拿来看的.**不象windows,无论你的真实物理内存有多少,他都要拿硬盘交换文件来读.这也就是windows为什么常常提示虚拟空间不足的原因.你们想想,多无聊,在内存还有大部分的时候,拿出一部分硬盘空间来充当内存.硬盘怎么会快过内存.所以我们看linux,只要不用swap的交换空间,就不用担心自己的内存太少.如果常常swap用很多,可能你就要考虑加物理内存了.这也是linux看内存是否够用的标准哦.
———————————————
语　　法： free [-bkmotV][-s <间隔秒数>]
补充说明：free指令会显示内存的使用情况，包括实体内存，虚拟的交换文件内存，共享内存区段，以及系统核心使用的缓冲区等。
参　　数：
-b 　以Byte为单位显示内存使用情况。
-k 　以KB为单位显示内存使用情况。
-m 　以MB为单位显示内存使用情况。
-o 　不显示缓冲区调节列。
-s<间隔秒数> 　持续观察内存使用状况。
-t 　显示内存总和列。
-V 　显示版本信息。
### linux命令free详解
[houfeng](http://www.taobaotesting.com/users/qa/blogs?uid=67)发表于：2009-06-29浏览：18741次评论：[3次](http://www.taobaotesting.com/blogs/qa?bid=2265#reply-comment)所属分类： [性能测试](http://www.taobaotesting.com/categories/qa/blogs?cid=74)
前段时间有个项目的用C写的，性能测试时发现内存泄露问题。关于怎么观察内存使用问题，free是很好用的一个命令。
**bash-3.00$ free**
total       used       free     shared    buffers     cached
Mem:       1572988    1509260      63728          0      62800     277888
-/+ buffers/cache:    1168572     404416
Swap:      2096472      16628    2079844
Mem：表示物理内存统计
-/+ buffers/cached：表示物理内存的缓存统计
Swap：表示硬盘上交换分区的使用情况，这里我们不去关心。
系统的总物理内存：255268Kb（256M），但系统当前真正可用的内存b并不是第一行free 标记的 16936Kb，它仅代表未被分配的内存。
第1行 Mem：total：表示物理内存总量。
used：表示总计分配给缓存（包含buffers 与cache ）使用的数量，但其中可能部分缓存并未实际使用。
free：未被分配的内存。
shared：共享内存，一般系统不会用到，这里也不讨论。
buffers：系统分配但未被使用的buffers 数量。
cached：系统分配但未被使用的cache 数量。buffer 与cache 的区别见后面。 total = used + free 第2行 -/+ buffers/cached：used：也就是第一行中的used - buffers-cached 也是实际使用的内存总量。
free：未被使用的buffers 与cache 和未被分配的内存之和，这就是系统当前实际可用内存。 free 2= buffers1 + cached1 + free1 //free2为第二行、buffers1等为第一行
buffer 与cache 的区别
A buffer is something that has yet to be “written” to disk. A cache is something that has been “read” from the disk and stored for later use第3行：第三行所指的是从应用程序角度来看，对于应用程序来说，buffers/cached
 是等于可用的，因为buffer/cached是为了提高文件读取的性能，当应用程序需在用到内存的时候，buffer/cached会很快地被回收。
所以从应用程序的角度来说，可用内存=系统free memory+buffers+cached.
接下来解释什么时候内存会被交换，以及按什么方交换。
当可用内存少于额定值的时候，就会开会进行交换.
如何看额定值（RHEL4.0）：
#cat /proc/meminfo
交换将通过三个途径来减少系统中使用的物理页面的个数：
1.减少缓冲与页面cache的大小，
2.将系统V类型的内存页面交换出去，
3.换出或者丢弃页面。(Application 占用的内存页，也就是物理内存不足）。
事实上，少量地使用swap是不是影响到系统性能的。
下面是buffers与cached的区别。
buffers是指用来给块设备做的缓冲大小，他只记录文件系统的metadata以及 tracking in-flight pages.
cached是用来给文件做缓冲。
那就是说：buffers是用来存储，目录里面有什么内容，权限等等。
而cached直接用来记忆我们打开的文件，如果你想知道他是不是真的生效，你可以试一下，先后执行两次命令#man X ,你就可以明显的感觉到第二次的开打的速度快很多。
实验：在一台没有什么应用的机器上做会看得比较明显。记得实验只能做一次，如果想多做请换一个文件名。
#free
#man X
#free
#man X
#free
你可以先后比较一下free后显示buffers的大小。
另一个实验：
#free
#ls /dev
#free
你比较一下两个的大小，当然这个buffers随时都在增加，但你有ls过的话，增加的速度会变得快，这个就是buffers/chached的区别。
因为Linux将你暂时不使用的内存作为文件和数据缓存，以提高系统性能，当你需要这些内存时，系统会自动释放（不像windows那样，即使你有很多空闲内存,他也要访问一下磁盘中的pagefiles）
使用free命令
将used的值减去   buffer和cache的值就是你当前真实内存使用 ————–对**操作系统**来讲是Mem的参数.buffers/cached 都是属于被使用,所以它认为free只有**16936**.
对**应用程序**来讲是(-/+ buffers/cach).buffers/cached 是等同可用的，因为buffer/cached是为了提高程序执行的性能，当程序使用内存时，buffer/cached会很快地被使用。所以,以应用来看看,以(-/+
 buffers/cache)的free和used为主.所以我们看这个就好了.另外告诉大家一些常识.Linux为了提高磁盘和内存存取效率, Linux做了很多精心的设计, 除了对dentry进行缓存(用于VFS,加速文件路径名到inode的转换), 还采取了两种主要Cache方式：Buffer Cache和Page Cache。前者针对磁盘块的读写，后者针对文件inode的读写。这些Cache能有效缩短了
 I/O系统调用(比如read,write,getdents)的时间。**记住内存是拿来用的,不是拿来看的.**不象windows,无论你的真实物理内存有多少,他都要拿硬盘交换文件来读.这也就是windows为什么常常提示虚拟空间不足的原因.你们想想,多无聊,在内存还有大部分的时候,拿出一部分硬盘空间来充当内存.硬盘怎么会快过内存.所以我们看linux,只要不用swap的交换空间,就不用担心自己的内存太少.如果常常swap用很多,可能你就要考虑加物理内存了.这也是linux看内存是否够用的标准哦.
———————————————
语　　法： free [-bkmotV][-s <间隔秒数>]
补充说明：free指令会显示内存的使用情况，包括实体内存，虚拟的交换文件内存，共享内存区段，以及系统核心使用的缓冲区等。
参　　数：
-b 　以Byte为单位显示内存使用情况。
-k 　以KB为单位显示内存使用情况。
-m 　以MB为单位显示内存使用情况。
-o 　不显示缓冲区调节列。
-s<间隔秒数> 　持续观察内存使用状况。
-t 　显示内存总和列。
-V 　显示版本信息。
