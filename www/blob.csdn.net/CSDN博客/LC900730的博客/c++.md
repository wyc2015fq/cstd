# C++ - LC900730的博客 - CSDN博客
2017年09月24日 21:37:27[lc900730](https://me.csdn.net/LC900730)阅读数：127标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[c++](https://blog.csdn.net/LC900730/article/category/6933816)
## Assert预处理宏
assert预处理宏： 
实际上是一个预处理变量，行为有点类似于内联函数。 
assert宏使用一个表达式作为它的条件。 
assert(expr):如果表达式为假，assert输出信息并且终止程序执行。 
如果为真，则assert什么也不做。 
assert定义在cassert头文件中。
## NDEBUG预处理变量
assert的行为依赖于一个名为NDEBUG的预处理变量的状态。如果定义了NDEBUG，则assert什么也不做。 
默认状态下没有NDEBUG，此时assert将执行运行时候检查。
## 函数名字作为值使用
```
pf=lengthCompare;    //pf指向名字为lengthCompare的函数
pf=&lengthCompare;  //取地址符
//不需要解引用指针就可以调用
bool b1=pf("hello","goodbye");             //调用
bool b2=(*pf)("hello","goodbye");          //等价调用
bool b3=lengthCompare("hello","goodbye");  //等价调用
```
