# ubuntu部署eclipse兼容性问题 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年01月06日 16:49:04[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1977
个人分类：[Linux](https://blog.csdn.net/fjssharpsword/article/category/6480442)









好几天一直耗在eclipse解压后无法启动，毫无反应。

今天突然想起是不是因为32位和64位兼容问题。

于是ubuntu一查（sudo uname --m）是64位，下载的eclipse是32位，不能启动，重新下载64位eclipse可以启动。



