# c++容易混淆的概念 - KinboSong的博客 - CSDN博客
2017年07月31日 23:11:24[songjinbo3](https://me.csdn.net/KinboSong)阅读数：230
## 1、c++标准库和STL之间的关系
简单地说，C++ 标准库包含了三个部分：C 标准库的 C++ 版本；C++ IO 库；C++ STL 
IO 库最常用的 HEADER 是 <IOSTREAM> 头文件 
STL 包括了很多容器类(vector, list, deque, stack...)，还有 functinal, algorithm, iterator 等 
C 标准库的 C++ 版本：设原来头文件是 <*.h> 则 C++ 标准头文件是 <c*> 
C++ 98 STD 版本的标准库头文件一概没有 .h 后缀，并且把几乎所有内容都加入了 namespace std，需要 unsing 指令才能使用
# 2、动态链接和静态链接
动态链接和静态链接都需要连接静态库，具体程序是动态链接还是静态链接是由静态库内部来决定的，不需要开发人员指定。
