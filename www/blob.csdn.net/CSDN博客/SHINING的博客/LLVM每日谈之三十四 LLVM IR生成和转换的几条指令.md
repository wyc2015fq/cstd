# LLVM每日谈之三十四  LLVM IR生成和转换的几条指令 - SHINING的博客 - CSDN博客
2018年07月14日 11:27:31[snsn1984](https://me.csdn.net/snsn1984)阅读数：954
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
本文将罗列几条关于LLVM IR生成和转换的几条指令，并没有技术含量可言，只是让刚接触LLVM IR的同学，有一个检索和参考作用。文中min.c作为输入。
```cpp
min.c
int min(int a , int b) {
  if (a <b )
    return a;
  return b:
}
```
1、从源码生成.bc
clang min.c -emit-llvm -c -o min.bc
2、从源码生成.ll
clang min.c -emit-llvm -S -c -o min.ll
3、.ll转换为.bc
llvm-as min.ll -o min.bc
4、.bc转换为.ll
llvm-dis min.bc -o min.ll
5、从.bc中抽取元素构建成新的.bc
llvm-extract -func=min min.bc -o min-fn.bc
llvm-extract可以从.bc中抽取函数、全局变量等，然后构建成为一个新的.bc文件。
此处的例子是从中抽取出函数名为min的函数，由于该文件中只有这一个函数，所以
抽取之后所形成的文件与原文件并无区别。
参考文献：《Getting Started with LLVM Core Libraries》
