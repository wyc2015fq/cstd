
# 代码静态检查-cppcheck - 3-Number - CSDN博客


置顶2015年08月10日 17:22:12[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：1094


### 1
### 安装
解压：
tar xzvf cppcheck-1.69.tar.gz
进入源码，方法1：make & sudo make install
如果方法1运行报错（找不到配置文件），则按方法2：
make SRCDIR=build CFGDIR=/usr/share/cppcheck/
make install CFGDIR=/usr/share/cppcheck/
### 2
### 功能
Cppcheck是一种C/C++代码缺陷静态检查工具。不同于C/C++编译器及其它分析工具，Cppcheck只检查编译器检查不出来的bug，不检查语法错误。
cppcheck是一个静态代码检查工具，支持c,c++代码；作为编译器的一种补充检查，cppcheck对产品的源代码执行严格的逻辑检查。 执行的检查包括：
1.自动变量检查
2.数组的边界检查
3.class类检查
4.过期的函数，废弃函数调用检查
5.异常内存使用，释放检查
6.内存泄漏检查，主要是通过内存引用指针
7.操作系统资源释放检查，中断，文件描述符等
8.异常STL 函数使用检查
9.代码格式错误，以及性能因素检查
### 3
### 使用案例
1、检查一个目录中的所有文件 cppcheck path
2、通过-enable控制输出信息的严重等级
只显示警告信息：Cppcheck --enable = warning file.c
建议写出更高效的代码：cppcheck --enable = performance file.c
输出检查的信息：cppcheck --enable=information file.c
输出一些无用的函数、冗余代码等：cppcheck --enable=style file.c
输出一些无用函数：Cppcheck --enable=unusedFunction file.c
所有选项都启动：cppcheck--enable=all file.c
大量的信息参照英文帮助文档：
http://wenku.baidu.com/link?url=Yu-vnK0kFyZ_XBSb8k82kTy_Thz3NiEhXRyxTJ6l8r3HIV3EC6OaxitLALXmnoX_ve4ai2bHcsGPmxXniH0vw1yBjfDmVGAw8cu9Kbf2xdW
使用例子：
cppcheck --enable=all . 2 >err.txt
这个工具能检查一些大家不容易发现的问题。
### 4
### 输出信息
Buffer is accessed out of bounds
Memory leak
Resource leak
Uninitialized struct member
Memory is allocated but not initialized
Memory pointed to by 'crypt' is freed twice
Uninitialized variable

