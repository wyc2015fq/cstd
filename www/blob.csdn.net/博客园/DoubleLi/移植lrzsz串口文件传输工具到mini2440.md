# 移植lrzsz串口文件传输工具到mini2440 - DoubleLi - 博客园






1、[下载源码](http://www.ohse.de/uwe/software/lrzsz.html)
2、解压源码

tar -xzf lrzsz-0.12.20.tar.gz

3、检查配置

./configure



4、修改Makefile

有三个Makefile需要修改，分别是
**lrzsz-0.12.20/Makefile**,**lrzsz-0.12.20/lib/Makefile**,  
**lrzsz-0.12.20/src/Makefile       **

将这三个Makefile中的所有gcc替代成你开发板所需要的交叉编译器，如mipsel-linux-gcc

  （即：CC = gcc    CPP = gcc -E  都改成CC = mipsel-linux-gcc    CPP = mipsel-linux-gcc -E）

5、编译源码

make

6、得到工具

在src/文件夹下的lrz和lsz工具即是交叉编译成功的lrzsz工具了

7、实用

将lrz和lsz复制到板子/bin下，即可使用，当然要配合secureCRT









