# C语言相关知识 - BlueSky - CSDN博客
2015年10月16日 18:52:34[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：371
**在Linux系统之中，EOF根本不是一个字符，而是当系统读取到文件结尾，所返回的一个信号值（也就是-1）**
Linux中，在新的一行的开头，按下Ctrl-D，就代表EOF（如果在一行的中间按下Ctrl-D，则表示输出”标准输入”的缓存区，所以这时必须按两次Ctrl-D）；Windows中，Ctrl-Z表示EOF。（顺便提一句，Linux中按下Ctrl-Z，表示将该进程中断，在后台挂起，用fg命令可以重新切回到前台；按下Ctrl-C表示终止该进程。）
**sizeof**
C语言提供一个编译时(compile-time)一元运算符sizeof，它可以用来计算任一对象的长度。 
表达式： sizeof  对象   以及  sizeof  （类型名） 
将返回一个整形数值，它等于指定对象或类型占用的存储空间的字节数（严格说，sizeof的返回值是无符号的整型值，其类型为size_t，该类型在头文件
