# QT4使用HDF5 类型错误 - wishchinYang的专栏 - CSDN博客
2016年05月25日 10:19:06[wishchin](https://me.csdn.net/wishchin)阅读数：923
个人分类：[C+/代码迁移																[C++编程](https://blog.csdn.net/wishchin/article/category/1508333)](https://blog.csdn.net/wishchin/article/category/1552163)
使用HDF5 ：HDF5_1.10.0
出现： fatal error C1083: 无法打开包括文件:"stdbool.h": No such file or directory
fatal error C1083: 无法打开包括文件:"inttype.h": No such file or directory
      C++11的问题，MSVC10不能使用C++11,。
      处理方法： 滚回 HDF5_1.8.11 版本
