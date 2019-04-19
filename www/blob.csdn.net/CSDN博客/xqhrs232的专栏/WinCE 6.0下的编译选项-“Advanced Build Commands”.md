# WinCE 6.0下的编译选项-“Advanced Build Commands” - xqhrs232的专栏 - CSDN博客
2013年11月19日 16:50:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：975
原文地址::[http://blog.sina.com.cn/s/blog_47049b230100wc4e.html](http://blog.sina.com.cn/s/blog_47049b230100wc4e.html)
相关网帖
1、[wince6.0编译命令分析](http://blog.csdn.net/llf021421/article/details/8485684)----[http://blog.csdn.net/llf021421/article/details/8485684](http://blog.csdn.net/llf021421/article/details/8485684)
**在VS2005的"Build"菜单中，有一个“Advanced Build Commands”，其中有如下关于WinCE 6.0的编译选项：**
"Sysgen"：相当于执行命令"blddemo -q”，一般第一次编译或者是改变了"Catalog"中的item的时候，就用这个了。
"Clean Sysgen"：相当于执行命令"blddemo clean -q"，按照文档上的说明，当修改了%_WINCEROOT%\Public\CEBASE\OAK\Misc\Cesysgen.bat的时候，或者改变了以SYSGEN，BSP为前缀的环境变量的时候，需要使用这个来编译。
**一般只有第一次创建完工程的时候，才需要用"Sysgen"命令，以后只要是改变了SYSGEN为前缀的环境变量的设置或者是"Catalog"中的item，就需要使用"Clean Sysgen"，而改变了以BSP为前缀的环境变量要看具体情况，也不一定就要用"Clean Sysgen"。**
"Build and Sysgen"：相当于执行命令"blddemo"，当改变了\public目录下的代码，比如你打了WinCE的patch，你就需要用这个了。
"Rebuild and Sysgen Clean"：相当于执行命令"blddemo clean cleanplat
 -c"，相当于清除上一次编译生成的文件，然后重新编译\public目录和你的工程。
"Build and Sysgen Current BSP"：相当于执行命令"blddemo -qbsp"，仅编译\platform目录下的代码。所以当改变了\platform目录下的代码的时候或者说改变了BSP的代码的时候，可以用这个来编译。
"Rebuild and Clean Sysgen Current BSP"：相当于执行命令"blddemo
 -qbsp -c"，相当于完全重新编译\platform目录下要编译的代码。
![【转】WinCE <wbr>6.0下的编译选项-鈥淎dvanced <wbr>Build <wbr>Commands鈥](http://s12.sinaimg.cn/middle/47049b23gb660b0e692fb&690)
![【转】WinCE <wbr>6.0下的编译选项-鈥淎dvanced <wbr>Build <wbr>Commands鈥](http://s7.sinaimg.cn/middle/47049b23gb660b2ecf666&690)
//======================================================================================================
备注：：
1》多看PB自带的英文文档，这些编译命令的介绍都有的，在Developing an Operationg System/Platform Builder User's Guide/Platform Builder IDE/Platform Builder User Interface
