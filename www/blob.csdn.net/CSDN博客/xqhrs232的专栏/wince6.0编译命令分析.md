# wince6.0编译命令分析 - xqhrs232的专栏 - CSDN博客
2013年11月19日 16:48:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：960
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/llf021421/article/details/8485684](http://blog.csdn.net/llf021421/article/details/8485684)
相关网帖
1、WinCE 6.0下的编译选项-“Advanced Build Commands”----[http://blog.sina.com.cn/s/blog_47049b230100wc4e.html](http://blog.sina.com.cn/s/blog_47049b230100wc4e.html)

下面从CSDN帮助文档简要说明vs2005下面的系统编译命令，这些命令位于Build->Advanced Build Commonds
一共有六条命令，分别是：
1.Sysgen
2.Clean Sysgen
3.Build and Sysgen
4.Rebuild and Clean Sysgen
5.Build Current BSP and Subprojects
6.Rebuild Current BSP and Subprojects
现分别讨论一下，各条命令的命令情况：
一、Sysgen
Sysgen 相当于 "blddemo -q"，一般是第一次编译或者改变了Catalog中的Item时，也就是改变了SYSGEN变量，就用这个命令，下面是CSDN中的说明：
![](https://img-my.csdn.net/uploads/201301/09/1357714780_8761.png)
二、Clean Sysgen
下面是CSDN中的说明：
![](https://img-my.csdn.net/uploads/201301/09/1357714800_3896.png)
Clean Sysgen  这条命令相当于"blddemo clean -q",它会清除wince系统内的SYSGEN变量，重新生成SYSGEN变量。当你改变了环境变量或者SYSGEN变量的时候，可以选择此命令。如果只是修改了源代码，则这个命令则不推荐。
三、Build and Sysgen
CSDN说明：
![](https://img-my.csdn.net/uploads/201301/09/1357714817_8287.png)
当你修改了位于%_WINCEROOT%\Public 或者%_WINCEROOT%\<custom_directory>目录下面的文件时，可以使用这个命令。在此处微软是不推荐使用这个命令的，因为这个命令会编译整生OS源代码，然后执行系统生成，这会浪费大量的时间。如果你不是实现wince系统的源代码修改，则这个命令不推荐使用。
四、Rebuild and Sysgen Clean
CSDN说明：
![](https://img-my.csdn.net/uploads/201301/09/1357714844_5747.png)
这个命令也会编译整个OS的源代码，需要发费许多的时候，是不推荐使用的。这个命令的另一个重点是：它会清除wince下面的所有的库，然后重新再生成，这对于没有源代码的库文件，一旦删除了，你就只能重新安装wince了
五、Build and Sysgen Current BSP
![](https://img-my.csdn.net/uploads/201301/09/1357714851_5870.png)
Build and Sysgen Current BSP 这个命令会编译BSP的源代码，即仅编译/platform目录下的代码。当你修改了BSP目录的文件，可以选择这个命令来编译。如果SYSGEN变量也已经改变了，就不要选择这个命令，可以选择Sysgen
六、Rebuild and Clean Sysgen Current BSP
这个命令，在csdn中的blddemo命令介绍时，有如下说明：
![](https://img-my.csdn.net/uploads/201301/09/1357717785_7877.png)
其英文意思大概为：下面的代码说明了你先前在编译\Platform\Common时，发现了一个错误导致了编译中止，当你修改了这个错误的时候，可以通过此命令继续编译。
其中blddemo这条命令的详细使用说明，在csdn中的链接如下：
[http://msdn.microsoft.com/en-us/library/ee478761(v=WinEmbedded.60).aspx](http://msdn.microsoft.com/en-us/library/ee478761(v=WinEmbedded.60).aspx)
//======================================================================================================
备注：：
1》多看PB自带的英文文档，这些编译命令的介绍都有的，在Developing an Operationg System/Platform Builder User's Guide/Platform Builder IDE/Platform Builder User Interface

