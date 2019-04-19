# const int *， int const *， int * const - Koma Hub - CSDN博客
2019年01月31日 22:14:13[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：33
个人分类：[ANSI C																[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)](https://blog.csdn.net/Rong_Toa/article/category/8653945)
```cpp
const int * grape;
int const * grape;
int * const grape_jelly;
```
The last of these cases makes the pointer read-only, whereas the other two make the object that it points at read-only; and of course, both the object and what it points at might be constant. Either of the following equivalent declarations will accomplish this:
```cpp
const int * const grape_jam;
int const * const grape_jam;
```
关键问题点：const 属于修饰符 ，关键是看const 修饰的位置在那里
## 1、const int *a
这里const 修饰的是int，而int定义的是一个整值
因此*a 所指向的对象 值 不能通过 *a 来修改，但是 可以重新给 a 来赋值，使其指向不同的对象
eg:
```cpp
const int *a = 0;
const int b = 1;
int c = 1;
a = &b  //ok！  额外：注意不能通过a 来修改 b值
a = &c  //ok！   额外：虽然c本身不是一个常量
*a = 2  //erro！ 为题就在这里，不能修改通过 *a 所指向的对象值，最后赋值得对象是c，因此不能通过*a 来修改c值。
```
## 2、int  *const a       
这里const修饰的是 a ，a代表的是一个指针地址，因此不能赋给a其他的地址值，但可以修改a指向的值，这有点和cont int *a相反的意味。
```cpp
#include <stdio.h>
int main()
{
        int b = 1;
        int c = 3;
        int *const a = &b;
        *a = 2;
        printf("*a = %d\n", *a);
        a = &c; //错误：向只读变量‘a’赋值
        return 0;
}
```
## 3、int const *a
和 const int *a 的意义是相同的 他们两个的作用等价
```cpp
#include <stdio.h>
int main()
{
        int b = 1;
        int c = 3;
        const int *a = &b;
        int const *d = &c;
        *a = 2;//错误：向只读位置‘*a’赋值
        b = 2;//正确
        printf("*a = %d\n", *a);
        *d = 4;//错误：向只读位置‘*d’赋值
        c = 4;
        printf("*d = %d\n", *d);
        return 0;
}
```
## 4、const int * const a 
这个代表a所指向的对象的值以及它的地址本身都不能被改变
关于const的点滴补充:
- 1、const 对象的地址只能赋值给指向const 对象的指针
- 2、指向const 对象的指针可以 被赋 以 一个非const 对象的地址 
- 3、指向const 得指针常被用作函数的形式参数，保证被传递给函数的实际对象在函数得实际对象在函数中不会被修改
- 4、常量在定义后就不能被修改,所以它必须被初始化。未初始化的常量定义将导致编译错误（上面都是在说明const得问题，所以没有赋值，实际语句中要赋值的）
[https://blog.csdn.net/zhangheng837964767/article/details/33783511/](https://blog.csdn.net/zhangheng837964767/article/details/33783511/)
