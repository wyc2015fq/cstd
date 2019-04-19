# register关键字-1 - Koma Hub - CSDN博客
2019年02月11日 19:54:40[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：44
```cpp
#include<iostream>
using namespace std;
int main(int argc, char*argv[])
/* Rong Tao 2018.03.19 */ 
{   
    /*
    在函数中频繁被使用的自动变量可以用register 声明如果可能的话编译器会把该对
    象装载到机器的寄存器中如果不能够的话则对象仍位于内存中出现在循环语句中的数
    组索引和指针是寄存器对象的很好例子
    */
    register int a;
    return 0;
}
```
