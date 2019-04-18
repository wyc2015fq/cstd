# c中宏定义的特殊符号 - ljx0305的专栏 - CSDN博客
2010年04月07日 17:37:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：3178
在C／C＋＋中，宏定义是由define完成的，define中有三个特殊的符号值得我们注意：
1. ＃:在宏展开的时候会将#后面的参数替换成字符串，如：
  ＃define p(exp) printf(#exp);
   调用p(asdfsadf)的时候会将#exp换成"asdfsadf"
2. ##:将前后两个的单词拼接在一起。例如《The C Programming Language》中的例子：
  #define cat(x,y) x##y
  调用cat(var, 123)展开后成为var123.
3. #@:将值序列变为一个字符
  #define ch(c) #@c
  调用ch(a)展开后成为'a'.
自己写了一小段测试程序：
#define A(a,b) a##b
#define B(a) #a
#define C(a) [#@a](mailto:#@a)
#include <iostream>
using namespace std;
void main()
{
 int v = 0,v1 = 1,v12 = 2;//v stands for var
 cout << A(v1,2) << endl;
 cout << B(v1) << endl;
 cout << C(v) << endl;
}
结果为：
1
v1
v
在c++面对对象化编程还可以这样用：
#define A
public://此处加一些成员，并且可以在成员名上用##下一些功夫
class Object
{
     A
};
以前没有太在意宏的这些语法，感觉宏这种拼接语法这样的功能很神奇。c++很自由，很强大。。。
