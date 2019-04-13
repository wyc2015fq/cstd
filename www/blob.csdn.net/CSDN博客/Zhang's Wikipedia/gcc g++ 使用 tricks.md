
# gcc-g++ 使用 tricks - Zhang's Wikipedia - CSDN博客


2018年05月14日 23:08:24[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：113



## 0. 优化级别
-O0，不进行优化的编译后的文件大小反而更小，小于 -O2 的；
## 1. -std 指定 C 语言标准
-ansi
-ansi == -std=c90
-std=c99：（std：standard）
循环变量在 for 语句中定义，也即for (int i = 0; i < 10; ++i)，c99 语法才支持，在ansi也即c90的语法中是不支持的，循环变量必须事先定义；
2. 预处理（preprocessing）
编译器的工作分为两个阶段，
预处理（preprocess）
编译阶段
gcc -E：可以看到预处理之后，编译之前的程序；
预处理会做两件事情：
把头文件在代码中展开；
宏定义原样替换；
C 语言中 \# 开头的行称为预处理指示（preprocessing directive），
gcc -E main.c 等价于 cpp main.c
cpp：c preprocessor，只做预处理而不编译；


