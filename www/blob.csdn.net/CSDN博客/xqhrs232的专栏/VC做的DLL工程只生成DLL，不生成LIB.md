# VC做的DLL工程只生成DLL，不生成LIB - xqhrs232的专栏 - CSDN博客
2013年10月16日 10:40:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：805
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://hi.baidu.com/gaogaf/item/552a1598be7e30895914612a](http://hi.baidu.com/gaogaf/item/552a1598be7e30895914612a)
相关网帖
1、Lib和Dll如何生成及联系----[http://blog.sina.com.cn/s/blog_50af1bc80100bhf1.html](http://blog.sina.com.cn/s/blog_50af1bc80100bhf1.html)
2、[从DLL生成LIB](http://blog.csdn.net/cheungmine/article/details/8927173)----[http://blog.csdn.net/cheungmine/article/details/8927173](http://blog.csdn.net/cheungmine/article/details/8927173)
必须至少导出一个函数或者变量或者类。
没有导出的话就不生成LIB。
--------------------------------------------------------
如果LIB是静态库则不需要DLL 。
如果LIB是动态链接的，那么就必须找到DLL 。
动态链接时，LIB只包含简单的DLL导出定义和辅助链接，并不包含可执行实体。
--------------------------------------------------------
动态链接与静态链接的不同之处在于：
动态链接允许可执行模块（.dll 文件或 .exe 文件）仅包含在运行时定位 DLL 函数的可执行代码所需的信息。
在静态链接中，链接器从静态链接库获取所有被引用的函数，并将库同代码一起放到可执行文件中。
--------------------------------------------------------
lib后缀的库有两种：
一种为静态链接库(Static Libary，简称“静态库”)；
另一种为动态连接库(DLL，简称“动态库”)的导入库(Import Libary，简称“导入库”)。
