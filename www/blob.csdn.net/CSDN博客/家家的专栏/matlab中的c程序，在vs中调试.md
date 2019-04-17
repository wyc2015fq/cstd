# matlab中的c程序，在vs中调试 - 家家的专栏 - CSDN博客





2011年07月28日 19:02:23[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1058








一、 编译cpp文件：

    mex -O -c -g tld.cpp
    mex -O -g fern.cpp 

。。。

二、VS 中选 Debug/Attach to process，选择matlab。。。。



在c代码中，设置断点，就可调试。。。




