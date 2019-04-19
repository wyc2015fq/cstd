# PB5.0下修改OS源代码的编译选项介绍 - xqhrs232的专栏 - CSDN博客
2010年03月05日 18:12:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：947标签：[os																[build](https://so.csdn.net/so/search/s.do?q=build&t=blog)](https://so.csdn.net/so/search/s.do?q=os&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
相关网帖
1、[PB5.0快速编译WINCE](http://blog.csdn.net/xqhrs232/article/details/5348276)----[http://blog.csdn.net/xqhrs232/article/details/5348276](http://blog.csdn.net/xqhrs232/article/details/5348276)
1。BUILD CURRENT PROJECT------------编译当前的文件夹里面的代码，只更新OS部分的LIB/DLL文件
2。BUILD AND SYSGEN CURRENT PROJECT -------编译当前的文件夹里面的代码，并把OS部分修改后的LIB/DLL文件COPY到当前的OS工程目录下（cesysgen/oak/lib）
3。SYSGEN CURRENT PROJECT-------把OS部分修改后的LIB/DLL文件COPY到当前的OS工程目录下（cesysgen/oak/lib）
4。BUILD OS /SYSGEN------基于目前OS里面已经存在的LIB/DLL来组装NK
5。BUILD OS /BUILD AND SYSGEN----把OS里面的源代码全部编译一遍形成LIB/DLL再组装NK
6。BUILD OS /BUILD AND SYSGEN CURRENT BSP----把BSP全部编译一遍再跟OS里面已经存在的LIB/DLL来组装NK
7。BUILD OS /MAKE RUN-TIME IMAGE-----在已经编译出来的OS+BSP的LIB/DLL的基础上来组装NK（修改了PLATFORM。XXX系列文件就要同步更新过去）
LIB/DLL有没更新看文件对应的时间会比较准一些！！！一般要选中CLEAN BEFORE BUILDING。
