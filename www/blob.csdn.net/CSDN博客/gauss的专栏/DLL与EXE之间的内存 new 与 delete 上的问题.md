# DLL与EXE之间的内存 new 与 delete 上的问题 - gauss的专栏 - CSDN博客
2013年01月04日 00:15:10[gauss](https://me.csdn.net/mathlmx)阅读数：453
个人分类：[Window编程](https://blog.csdn.net/mathlmx/article/category/944343)
                
最近在做的一个项目，我们的一个EXE调用了DLL，其中EXE会使用DLL new出来的变量指针，而且会在EXE delete这些指针，同样，DLL也会使用及delete由EXE new出来的变量指针。
开始在做测试的时候没有发现任何问题，当时用的是DEBUG版本。 可是后来发现Release版本运行程序时总是会崩，于是开始疯狂地调试，终于找到了导致崩溃的原因：
我们的Release版的EXE和DLL都是用/MT 编译选项编译的，也就是静态链接C/C++的运行时库,而这时，EXE和DLL各自拥有独自的堆(Heap)空间，所以其中一方在其堆中new出来的内存，只能由其自己来delete，否则就会产生问题（也就是我们碰到的崩溃...).
解决方法有很多种，我采用的方法就是改了改代码，让DLL delete 自己 new 出来的东西，EXE也同样如此。测试后程序没有出现问题。
-------------------------------------------------------------------------------------------
贴上几个相关的链接，很有帮助：
(英文）
[memory allocations in dll](http://www.codeguru.com/forum/showthread.php?t=229394)
[Deleting memory from DLLs (C++)](http://www.gamedev.net/community/forums/topic.asp?topic_id=289896)
（中文）
[DLL和exe里的malloc和free不能混用](http://blog.csdn.net/dotphoenix/archive/2009/07/14/4348686.aspx)
[在DLL中用CRT静态库申请内存,EXE释放是不行的](http://blog.sina.com.cn/s/blog_60d705b10100g4ou.html)
[DLL分配的内存如何在EXE里面释放](http://hi.baidu.com/honey%BC%A6/blog/item/8780d1f918976ed5b58f310f.html)
