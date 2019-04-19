# Keil 链接错误 Error: L6406E: No space in execution regions with .ANY selector matching - xqhrs232的专栏 - CSDN博客
2018年09月17日 15:59:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：172
原文地址::[https://www.cnblogs.com/ironx/p/5474935.html](https://www.cnblogs.com/ironx/p/5474935.html)
相关文章
1、MDK在链接时提示空间不够(No space in execution regions with .ANY selector... )的解决方案总结----[https://blog.csdn.net/gengyiping18/article/details/52640806/](https://blog.csdn.net/gengyiping18/article/details/52640806/)
2、MDK在链接时提示空间不够(No space in execution regions with .ANY selector... )解决方案----[https://blog.csdn.net/y1757655788/article/details/78231903](https://blog.csdn.net/y1757655788/article/details/78231903)
在开发过程当中遇到Keil build错误，检查发现时是链接时出错了。
错误内容为：
Error: L6406E: No space in execution regions with .ANY selector matching
Error: L6407E: Sections of aggregate size 0x44 bytes could not fit into .ANY selector
怀疑有可能某些段的size溢出了。
因为使用的ARM芯片内置flash为64KB，RAM = 8KB，因此不太可能是flash溢出了，很可能是RAM溢出了。
所以尝试了两种解决方法：
第一种是将keil option里面的编译优化选项，从level 0 调高到level 2，看看经过优化还会不会有溢出的问题。
实验的结果是没有溢出了，所以怀疑某些段溢出这个定论是对的，下一步就是要区分是RAM还是Flash溢出了。
第二种，是将一个比较大的函数挂载数组定义成const，也就是放在flash里面，再次build，发现错误也消失了。
看来果然是RAM溢出了。
普及下Keil编译结果的参数含义：
请参考以下资料，说明RO/RW/ZI-DATA的含义：
http://www.keil.com/forum/10624/
