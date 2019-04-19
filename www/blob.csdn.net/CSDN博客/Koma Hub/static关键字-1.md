# static关键字-1 - Koma Hub - CSDN博客
2019年02月11日 19:53:39[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：46
个人分类：[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)
```cpp
#include<iostream>
using namespace std;
void f(){
    /*
    当一个局部变量的值必须在多个函数调用之间保持有效时我们不能使用普
    通的自动对象自动对象的值在函数结束时被丢弃
    初始化只在f()首次被调用时执行一次;
    未经初始化的静态局部对象会被程序自动初始化为0;
    */
    static int a;
    a++;
    cout<<a<<endl;
}
int main(int argc, char*argv[])
/* Rong Tao 2018.03.19 */ 
{   
    f();
    f();
    return 0;
}
```
```
$ make
g++ Demo.cpp  -w
./a.out
2
3
```
