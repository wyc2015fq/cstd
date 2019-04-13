
# Linux Shell 重定向到文件以当前时间命名 - 嵌入式Linux - CSDN博客

2018年11月01日 14:48:37[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：192


我们经常在编译的时候，需要把编译的过程日志保留下来，这时候这个命令就非常重要了。
# make |tee xxx_$(date +%y%m%d%H%M%S).txt

