
# Makefile编写详解--项目开发 - 高科的专栏 - CSDN博客

2012年08月22日 21:22:56[高科](https://me.csdn.net/pbymw8iwm)阅读数：2122


预备知识：
gcc 的3个参数：
1. -o 指定目标文件
gcc sources/main.c -o bin/main
2. -c 编译的时候只生产目标文件不链接
gcc -c sources/main.c -o obj/main.o
3. -I 主要指定头文件的搜索路径
gcc -I headers -c main.c -o main.o
4. -l 指定静态库
gcc -lpthread ...
**3个预定义变量介绍：**
1.  $@     表示要生成的目标
2.  $^     表示全部的依赖文件
3.  $<     表示第一个依赖文件
bin/st_work : obj/main.o  obj/st_work.o  obj/fun.o
gcc  $^  -o $@  (命令一定要用以Tab开头)
obj/st_work.o : sources/st_work.c
gcc  -I  headers   -c$<-o  $@
obj/main.o :sources/main.c
gcc  -I  headers   -c$<-o  $@
obj/fun.o  :sources/fun.c
gcc  -I  headers   -c$<-o  $@
clean：
rm -f bin/st_work obj/*.o

