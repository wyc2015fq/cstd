# 在WinCE5.0和WinCE6.0下，编译选项介绍 - xqhrs232的专栏 - CSDN博客
2013年01月28日 16:53:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：648
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/nanjianhui/article/details/2010870](http://blog.csdn.net/nanjianhui/article/details/2010870)

作者：ARM-WinCE
**在WinCE5.0中，在"Build OS”菜单中，我们会看到不同的编译选项：**
"Sysgen"，"Build and Sysgen"和"Build and Sysgen current BSP"
Sysgen：不用多说，当你在"Catalog"中添加或删除了新的item的时候，就用这个吧。
Build and Sysgen：当你更新了/public目录下的源代码的时候，你就需要用这个了。一般比如在打patch以后，可能就需要进行Build and Sysgen了。
Build and Sysgen current BSP： 当你只改变了你的BSP部分的代码，就可以用这个选项。据说，当你改变了/platform目录下的代码，也可以用这个，具体没有试过。
**在WinCE6.0中，在VS2005的"Build"菜单中，有一个“Advanced Build Commands”，其中有很多编译选项：**
"Sysgen"：相当于执行命令"blddemo -q”，一般第一次编译或者是改变了"Catalog"中的item的时候，就用这个了。
"Clean Sysgen"：相当于执行命令"blddemo clean -q"，按照文档上的说明，当修改了 %_WINCEROOT%/Public/CEBASE/OAK/Misc/Cesysgen.bat的时候，或者改变了以SYSGEN，BSP为前缀的环境变量的时候，需要使用这个来编译。
在此我说一下我个人的经验，一般只有第一次创建完工程的时候，我会用"Sysgen"命令，以后只要是改变了SYSGEN为前缀的环境变量的设置或者是"Catalog"中的item，我就会使用"Clean Sysgen"，而改变了以BSP为前缀的环境变量要看具体情况，也不一定就要用"Clean Sysgen"。继续...
"Build and Sysgen"：相当于执行命令"blddemo"，当改变了/public目录下的代码，比如你打了WinCE的patch，你就需要用这个了。
"Rebuild and Sysgen Clean"：相当于执行命令"blddemo clean cleanplat -c"，相当于清除上一次编译生成的文件，然后重新编译/public目录和你的工程。
"Build and Sysgen Current BSP"：相当于执行命令"blddemo -qbsp"，仅编译/platform目录下的代码。所以当改变了/platform目录下的代码的时候或者说改变了BSP的代码的时候，可以用这个来编译。
 "Rebuild and Clean Sysgen Current BSP"：相当于执行命令"blddemo -qbsp -c"，相当于完全重新编译/platform目录下要编译的代码。
