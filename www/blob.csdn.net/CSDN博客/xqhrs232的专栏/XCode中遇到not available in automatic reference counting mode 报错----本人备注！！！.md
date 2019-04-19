# XCode中遇到not available in automatic reference counting mode 报错----本人备注！！！ - xqhrs232的专栏 - CSDN博客
2019年02月27日 17:52:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：55
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://blog.csdn.net/helinlin007/article/details/50908333](https://blog.csdn.net/helinlin007/article/details/50908333)
相关文章
1、'release' is unavailable: not available in automatic reference counting modeAutomatic----[https://blog.csdn.net/msn465780/article/details/72424868](https://blog.csdn.net/msn465780/article/details/72424868)
2、在支持ARC工程中编译不支持ARC的文件----[https://blog.csdn.net/duxinfeng2010/article/details/8709697](https://blog.csdn.net/duxinfeng2010/article/details/8709697)
 最近在学习object C的过程中，尝试着编写demo中的程序，结果就出现了很多的莫名其妙的问题。而这些问题又都不是语法本身的问题。那么就是编译器本身的问题了。今天就遭遇到了一个这样的问题。
      在写代码的过程中，没有语法的错误。结果编译无法通过。于是乎报了下面的错误，
     我找了很久，最后当然是在Stackoverflow中获得解决方法，解决方法正是修改编译器的设置。去除引用计数器的设置，具体如下图所示
之所以这么做的原因是由于Xcode的版本更新太快，而这个设置在老版本的xcode中是需要进行手写代码完成的。而如今成了编译器的默认设置。所以有所冲突，故而报错。只需要将上图中的红色圈中部分由YES改为NO即可。
//======================================================================================
本人备注：：
1》意思就是说手动释放内存的代码不可以用，原来IOS有2种编译方式：MRC 和 ARC
