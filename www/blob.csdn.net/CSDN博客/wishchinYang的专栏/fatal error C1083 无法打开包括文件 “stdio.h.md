# fatal error C1083: 无法打开包括文件:“stdio.h - wishchinYang的专栏 - CSDN博客
2016年07月07日 15:37:12[wishchin](https://me.csdn.net/wishchin)阅读数：7655
# 现象：
#     vs2012一直fatal error C1083: 无法打开包括文件:“stdio.h”
不知道配置太多，动到了什么地方，出现了这个问题；
在：
    解决方案--调试源文件  
    里面添加 包含   C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\include
   没有效果！！！
 在：
   属性页--VC++目录—包含目录，
   里面添加 C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\include
   可以编译成功。
