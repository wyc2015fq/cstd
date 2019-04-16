# Unix学习笔记（四） - 彩虹糖的博客 - CSDN博客





2018年05月17日 21:00:27[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：46
个人分类：[Unix](https://blog.csdn.net/caozixuan98724/article/category/7630652)









继续我们的Unix学习之旅，今天依旧把重点放到进程和内存管理上。

1. size

![](https://img-blog.csdn.net/20180517205333635)


重点在于这张图，通过这张图，我们要了解到在内存构造中，低地址到高地址分别放着哪些内容，了解程序运行中有关于内存的相关机制。

我们可以执行如下的命令：

size a.out

观察输出：

![](https://img-blog.csdn.net/20180517205707128)


可以看到，通过这个命令我们只能看到text(代码段)，data(initialized data)，bss(uninitialized data)的数据大小，显然，我们是不可能看到堆和栈的大小的，因为这些在编译的时候显然无法确定（比如递归程序对栈的占用）。



