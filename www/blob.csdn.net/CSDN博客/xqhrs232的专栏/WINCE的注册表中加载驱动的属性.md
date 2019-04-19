# WINCE的注册表中加载驱动的属性 - xqhrs232的专栏 - CSDN博客
2013年09月16日 11:47:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：721
原文地址::[http://blog.csdn.net/armeasy/article/details/5694783](http://blog.csdn.net/armeasy/article/details/5694783)
相关网帖
1、Wince6.0驱动开发----[http://wenku.baidu.com/view/2560b76a7e21af45b307a82f.html](http://wenku.baidu.com/view/2560b76a7e21af45b307a82f.html)
2、[图解WinCE6.0下的内核驱动和用户驱动](http://blog.csdn.net/xqhrs232/article/details/14000721)----[http://blog.csdn.net/xqhrs232/article/details/14000721](http://blog.csdn.net/xqhrs232/article/details/14000721)
加载驱动的属性：
    SHMK
    S: system,即指定驱动为系统文件；
    H: hide,  即指定驱动为隐藏文件；
    M: module，即加载整个模块，而不是按需分页；
    K: Kernel, 即指定驱动工作在kernel模式。CE6.0必须工作在该模式。
