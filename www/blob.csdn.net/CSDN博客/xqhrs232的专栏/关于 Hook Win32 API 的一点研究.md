# 关于 Hook Win32 API 的一点研究 - xqhrs232的专栏 - CSDN博客
2016年12月01日 14:27:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：305
原文地址::[http://blog.csdn.net/eagletian/article/details/1374028](http://blog.csdn.net/eagletian/article/details/1374028)
相关文章
1、
# [Hook Win32 API 的应用研究之一：网络监控](http://blog.csdn.net/eagletian/article/details/1374033)----[http://blog.csdn.net/eagletian/article/details/1374033](http://blog.csdn.net/eagletian/article/details/1374033)
# 2、
# [Hook Win32 API 的应用研究之二：进程防杀](http://blog.csdn.net/eagletian/article/details/1374034)----[http://blog.csdn.net/eagletian/article/details/1374034](http://blog.csdn.net/eagletian/article/details/1374034)
# 3、
# [Hook Win32 API 的应用研究之三：变速控制](http://blog.csdn.net/eagletian/article/details/1374042)----[http://blog.csdn.net/eagletian/article/details/1374042](http://blog.csdn.net/eagletian/article/details/1374042)
4、
# [Hook Win32 API 的应用研究之四：屏幕取词](http://blog.csdn.net/eagletian/article/details/1374048)----[http://blog.csdn.net/eagletian/article/details/1374048](http://blog.csdn.net/eagletian/article/details/1374048)
# Hook Win32 API 是一项有趣而实用的WINDOWS系统编程技术，应用领域十分广泛。虽然已经有不少的文章介绍过 Hook Win32 API 的方法了，我还是来作些简单的介绍，以便大家了解其工作原理。    Hook Win32 API 是什么意思？就是钩住Win32 API；那又何谓“钩”呢？就是绕弯的意思，让Win32 API函数的调用先绕一个弯路，在它执行实际功能之前，我们可以先做一些“预处理”，这样我们可以监视或定制某个Win32 API的调用，以实现一些特殊的功能。至于具体可以实现些什么样的功能，那就取决于程序设计者的想象力了。    为什么要Hook Win32 API呢？因为在很多情况下，我们想监视或改变某个应用程序的一些特定的行为，但是那个应用程序却没有提供相应的接口，而我们又几乎不可能得到其源代码，怎么办呢？因为大多数WINDOWS引用程序的行为很大程度上依赖于Win32 API，所以我们可以采用Hook Win32 API的方式来试图监视和改变应用程序的行为。    如何Hook Win32 API呢？实际上Win32 API是由一组动态链接库实现的，使用动态链接库是为了尽可能的共享内存。由于动态链接库是动态装入的，所以Win32 API函数的入口点也是动态确定的。当WINDOWS应用程序在调用Win32 API的时候，并不是直接调用某个函数地址，而是调用某处所存储的一个动态确定的函数地址来实现间接调用地，该处被命名为Import Address Table(简称IAT)。知道了这一点，接下来要做的就是想办法找到这个存储单元的位置，然后将其内容替换为接管函数的入口地址，不过得事先保存原函数的入口地址，以便执行了接管函数的代码后，可以在适当的地方以适当的方式再调用原函数。最后退出的时候或是不想再钩着它的时候，再将其恢复为原函数的入口地址。这就是Hook Win32 API的基本步骤，具体实现过程这里就不赘述了，可以参阅《WINDOWS 核心编程》(Jeffrer Richter著)    向大家强烈推荐由微软开发的一个用于Hook Win32 API的库(有源代码)：Detours 1.5 - 微软对自家的[操作系统](http://lib.csdn.net/base/operatingsystem)当然是最了解的了，相信Detours能在WINDOWS平台上运行得很好。点击这里下载：[Detours 1.5](http://ahzhuo.diy.myrice.com/download/detours.zip) (538KB)
