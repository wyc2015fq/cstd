# java dump文件怎么生成和分析-JMAP用法 - z69183787的专栏 - CSDN博客
2018年06月26日 10:49:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1062
[https://blog.csdn.net/hemin1003/article/details/71425209](https://blog.csdn.net/hemin1003/article/details/71425209)
jmap是java自带的工具
1. 查看整个JVM内存状态 
jmap -heap [pid]
pid 为 java 进程号，可使用jps 或者 linux 命令行查看，
通过 ps -ef | grep java  得到如上线程将某线程终止时用 
kill -9 XXXXX     XXXXX为上述查出的序号  如： 19979线程终止为： kill -9 4834 
![](https://img-blog.csdn.net/20170508171142218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVtaW4xMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2. 查看JVM堆中对象详细占用情况
jmap -histo [pid]
3. 导出整个JVM 中内存信息，可以利用其它工具打开dump文件分析，例如jdk自带的visualvm工具
jmap -dump:file=文件名.dump [pid]
![](https://img-blog.csdn.net/20170508171157703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVtaW4xMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
