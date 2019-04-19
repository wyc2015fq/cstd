# Qt连接Sqlserv数据库设置端口号问题-----测试OK但想不明白，难道是qt的一个bug!!! - xqhrs232的专栏 - CSDN博客
2018年08月31日 18:16:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：58
原文地址::[https://blog.csdn.net/dyb_k3/article/details/59110610](https://blog.csdn.net/dyb_k3/article/details/59110610)
相关文章
1、数据库连接不成功的问题----[https://bbs.csdn.net/topics/390406730](https://bbs.csdn.net/topics/390406730)
2、使用QT连接SqlServer数据库----[https://blog.csdn.net/crush____/article/details/52577305](https://blog.csdn.net/crush____/article/details/52577305)
前一阵自己数据库的端口号需要改动，默认是1433，改成1434以后，数据库突然连接不上了，看文档上写的需要设置端口号，setport以后还是不行，折腾了半天发现设置数据库名称的时候数据库IP地址后面要加上端口号而且必须这么加“127.0.0.1,1434”才可以，注意是逗号不是冒号。
写出来，让大家少走弯路，大家有知道的说说怎么设置端口号才有效？
