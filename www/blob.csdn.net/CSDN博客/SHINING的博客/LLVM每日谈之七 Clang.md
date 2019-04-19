# LLVM每日谈之七 Clang - SHINING的博客 - CSDN博客
2012年11月28日 09:33:59[snsn1984](https://me.csdn.net/snsn1984)阅读数：20073
个人分类：[编译技术及LLVM](https://blog.csdn.net/snsn1984/article/category/1249418)
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
作者：[snsn1984](http://blog.csdn.net/snsn1984)
        官方介绍Clang是LLVM native的一个面向C/C++/Objective-C的编译器，目标是要提供一个编译非常快的编译器。他们宣称在Debug模式下编译Objective-C比GCC快3倍。至于是否能比GCC快这么多，总有人去做对比，大家可以搜索下，好像确实快了些。
Clang一般被说是LLVM的一个前端。关于前端的定义，大家可以从LLVM每日谈之二里面看到前端在整个LLVM体系中的位置。Clang当初的定位就是要做下一个十年中一个伟大的前端，这也是苹果公司需要的。谈到苹果，现在也是LLVM最大的赞助商。苹果公司认为GCC的前端越来越不好用，并且不能给苹果的IDE提供很好的服务，所以他们转向了LLVM，Clang的定位就是替代GCC的前端。
Clang需要单独下载源码，放到LLVM Source/tools下面，然后整个编译LLVM才行。一般大家按照官方文档编译，都是已经放了Clang了。开始学习Clang文档的地址： http://clang.llvm.org/get_started.html 。这里下载源码的话使用svn下载的是最新的主干版本，大家可以从首页找到最后的发布版本的下载， 地址在： http://llvm.org/releases/  。这里可以看到发布的各个版本，目前最新的是3.1， 马上3.2就要出来了。从这里把编译Clang需要的LLVM，Clang，Compiler-RT（这个容易忘记）都下载好，放到相应的目录（目录怎么放，根据svn命令可以看出来）。
安装好了，大家就可以开始试试效果了。在 http://clang.llvm.org/get_started.html 有基本的命令。
