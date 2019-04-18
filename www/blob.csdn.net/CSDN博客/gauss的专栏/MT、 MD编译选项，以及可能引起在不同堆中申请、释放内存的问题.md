# /MT、/MD编译选项，以及可能引起在不同堆中申请、释放内存的问题 - gauss的专栏 - CSDN博客
2013年01月16日 12:45:59[gauss](https://me.csdn.net/mathlmx)阅读数：278
个人分类：[Window编程](https://blog.csdn.net/mathlmx/article/category/944343)
# 一、MD(d)、MT(d)编译选项的区别
## 1、编译选项的位置
以VS2005为例，这样子打开：
1）         打开项目的Property Pages对话框
2）         点击左侧C/C++节
3）         点击Code Generation节
4）         右侧第六行Runtime Library项目
## 2、各个设置选项代表的含义
|编译选项|包含|静态链接的lib|说明|
|----|----|----|----|
|/MD|_MT、_DLL|MSVCRT.lib|多线程、Release、DLL版本的运行时库|
|/MDd|_DEBUG、_MT、_DLL|MSVCRTD.lib|多线程、Debug、DLL版本的运行时库|
|/MT|_MT|LIBCMT.lib|多线程、Release版本的运行时库|
|/MTd|_DEBUG、_MT|LIBCMTD.lib|多线程、Debug版本的运行时库|
简单的说：
（1）/MD，表示运行时库由操作系统提供一个DLL，程序里不集成。
（2）/MT，表示运行时库由程序集成。
# 二、/MD、/MT的选择
##       1、为什么选择/MD，不选/MT？
         （1）程序就不需要静态链接运行时库，可以减小软件的大小；
         （2）所有的模块都采用/MD，使用的是同一个堆，不存在A堆申请，B堆释放的问题。
##       2、为什么选择/MT，不选择/MD？
         （1）有些系统可能没有程序所需要版本的运行时库，程序必须把运行时库静态链接上。
（2）减少模块对外界的依赖。
##       3、多个模块，必须选择相同的运行时库。
# 三、选择/MT需要解决的堆空间释放问题
         不同的模块各自有一份C运行时库代码、或者根本没有C运行时库，导致了各个模块会有各自的堆。如果在A堆中申请空间，到B堆中释放就会有崩溃，在模块A申请的空间，必须在模块A中释放。
         附录的DLL以及DLLUser代码，以STL的string为例，通过修改编译选项验证了这个问题。string在赋值的时候需要释放掉原来的空间，然后再申请新的空间存储新的内容。
# 四、选择/MD需要注意多个模块使用不同版本运行时库的问题
     (2012-9-17补充)
     多个dll被一个exe LoadLibrary加载，如果这些dll使用的运行时库是不同的，那么可能出现加载失败，原因可能是旧版本的运行时库已经在了，而某个dll它需要的是新版本的运行时库，旧版本不符合要求。
     如果工程里所有的模块都是自己写的或者可以完全控制的，那么这个问题不难解决，只需要在工程设置里都设置/MD，然后在相同的环境下编译一次就行。但是假如这个模块是外界提供的呢？
     可能存在这种情况：A动态库使用了B静态库，B静态库使用了C动态库，B静态库是外界提供的，我们要使用它，但无法修改它，我们也无法接触到C动态库。如果C动态库使用的运行时库版本跟编译A动态库的本地使用的不一致，那么A动态库里的嵌入信息就会记录两个不同版本的运行时库，它被加载的时候，可能会选择版本新的。假设A动态库被一个exe LoadLibrary加载，而这个exe本身的运行时库是旧的，这样就会导致A动态库加载失败，即便把新的运行时库拷贝到目录下也不行，因为exe这个进程已经加载了那个旧的运行时库。这时候必须使用manifest文件指定嵌入到A动态库里的运行时库为某个版本，忽略掉C动态库使用的运行时库版本。
     这个问题挺复杂的，我心思没去验证windows的PE文件加载会对运行时库做什么样的优先选择、运行时库在静态库里的记录…。只要记住，给外界使用的版本避免使用/MD（会导致膨胀？）。
# 四、参考资料
1、微软关于MT、MD等的详细介绍
[http://msdn.microsoft.com/en-us/library/2kzt1wy3(v=VS.71).aspx](http://msdn.microsoft.com/en-us/library/2kzt1wy3%28v=VS.71%29.aspx)
2、不要出现A模块申请，B模块释放的情况
[http://www.cnblogs.com/minggoddess/archive/2010/12/15/1907179.html](http://www.cnblogs.com/minggoddess/archive/2010/12/15/1907179.html)
3、运行时库有哪些版本
[http://www.cppblog.com/MichaelLiu/articles/10607.html](http://www.cppblog.com/MichaelLiu/articles/10607.html)
4、CSDN上关于堆空间释放的讨论
[http://topic.csdn.net/t/20010112/09/57983.html](http://topic.csdn.net/t/20010112/09/57983.html)
[http://topic.csdn.net/t/20031009/17/2338051.html](http://topic.csdn.net/t/20031009/17/2338051.html)
[http://topic.csdn.net/u/20090502/00/bf1602e3-ddf5-49b0-af81-8a23383f9ddc.html](http://topic.csdn.net/u/20090502/00/bf1602e3-ddf5-49b0-af81-8a23383f9ddc.html)
[http://blog.csdn.net/blz_wowar/article/details/2176536](http://blog.csdn.net/blz_wowar/article/details/2176536)
5、不同模块不同的堆
[http://www.cnblogs.com/WengYB/archive/2011/08/18/2144727.html](http://www.cnblogs.com/WengYB/archive/2011/08/18/2144727.html)
6、因为运行时库版本问题导致加载失败的分享
[http://blog.csdn.net/dev_yarin/article/details/6768373](http://blog.csdn.net/dev_yarin/article/details/6768373)
[http://blog.163.com/henan_lujun/blog/static/19538333200611485511640/](http://blog.163.com/henan_lujun/blog/static/19538333200611485511640/)
附录：
下载地址：http://files.cnblogs.com/cswuyg/Test_MD_and_MT.rar
来自：http://www.cnblogs.com/cswuyg/archive/2012/02/03/2336424.html
