# sh和bash的区别 - 星爷2401 - CSDN博客
2016年12月05日 09:00:08[星爷2401](https://me.csdn.net/leifukes)阅读数：4100标签：[bash sh shell UNIX](https://so.csdn.net/so/search/s.do?q=bash sh shell UNIX&t=blog)
个人分类：[UNIX环境高级编程](https://blog.csdn.net/leifukes/article/category/6556556)
sh 名称：bourne shell               [bɔ:n]
bash 名称：bourne-again shell
sh 是 bash的子集，bash是sh的进一步扩展。在linux中sh是bash的一个软连接。
`**[root@localhost /]# ls -l /bin/shlrwxrwxrwx 1 root root 4 Nov 20 05:56 /bin/sh -> bash**`
/bin/sh 相当于 /bin/bash --posix，因为sh和bash的区别就在于POSIX标准模式和bash特性的差异，执行sh命令，就相当于bash 加上 --posix 参数执行命令一样。
POSIX介绍：
[POSIX](http://baike.baidu.com/view/209573.htm)表示[可移植操作系统接口](http://baike.baidu.com/view/11757945.htm)（Portable
 Operating System Interface ，缩写为 POSIX ），POSIX标准定义了操作系统应该为应用程序提供的接口标准，是[IEEE](http://baike.baidu.com/view/3933.htm)为要在各种UNIX操作系统上运行的软件而定义的一系列API标准的总称，其正式称呼为IEEE
 1003，而国际标准名称为ISO/IEC 9945。
[bɔ:n]
