# c++中如何在主函数中调用其他文件内的函数？ - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年06月22日 20:52:15[QuJack](https://me.csdn.net/A784586)阅读数：9174








**linux下c++中如何在主函数中调用其他文件内的函数？**

这个作用不巴拉巴拉了，直接上干货！

需要四个文件：a.cpp包含一个被调用的函数，myhead.h预定义这个函数，3.b.cpp主函数，4.Makefile文件。

1.a.cpp


```cpp
#include "myhead.h"
#include <iostream>
using namespace std;
int myfun(int a,int b){
         std::cout<<a+b<<std::endl;
        return 0;
}
```


2.myhead.h


```cpp
#ifndef A_H_
#define A_H_
int myfun(int a,int b);
#endif
```


3.b.cpp


```cpp
#include "myhead.h"
#include <iostream>
int main(int argc, char** argv){
  int a=2,b=3;
  myfun(a,b);
  return 0;
}
```


4.Makefile


```cpp
CXX=g++
Objects=b.o a.o
myrun:$(Objects)
        $(CXX) -o myrun $(Objects)
b.o:b.cpp myhead.h
        $(CXX) -c b.cpp
a.o:a.cpp myhead.h
        $(CXX) -c a.cpp
```



5.linux 命令行下：

make

./myrun






