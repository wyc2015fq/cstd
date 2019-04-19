# C和C++中函数传参常见方式总结 - fanyun的博客 - CSDN博客
2016年03月24日 16:09:13[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：832
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**在我们的C和C++中函数传参中，我们常见方式主要有三种：值传参、指针传参、引用传参，现在加以总结，希望对各位想了解相关知识的朋友有帮助：**
1.值传参
按值传递的过程为：首先计算出实参表达式的值，接着给对应的形参变量分配一个存储空间，该空间的大小等于该形参类型的，然后把以求出的实参表达式的值一一存入到形参变量分配的存储空间中，成为形参变量的初值，供被调用函数执行时使用。这种传递是把实参表达式的值传送给对应的形参变量，也叫直接传参。使用这种方式，调用函数本省不对实参进行操作，也就是说，即使形参的值在函数中发生了变化，实参的值也完全不会受到影响，仍为调用前的值。
示例：
```cpp
#include <iostream>  
using namespace std;  
void swap(int,int);  
int main()  
{  
    int a = 3, b = 4;  
    cout << "a = " << a << ", b = "  
    << b << endl;  
    swap(a,b);  
    cout << "a = " << a << ", b = "  
    << b << endl;  
    return 0;  
}  
void swap(int x, int y)  
{  
     int t = x;  
     x = y;  
     y = t;  
}
```
2.指针传参
指针传参也叫地值传参，在函数定义时将形参说明成指针，对这样的函数进行调用时就需要指定地址值形式的实参。这时的参数传递方式就是地址传递方式。地址传递与按值传递的不同在于，它把实参的存储地址传送给对应的形参，从而使得形参指针和实参指针指向同一个地址。因此，被调用函数中对形参指针所指向的地址中内容的任何改变都会影响到实参。
示例：
```cpp
#include <iostream>  
using namespace std;  
void swap(int*,int*);  
int main()  
{  
    int a = 3, b = 4;  
    cout << "a = " << a << ", b = "  
    << b << endl;  
    swap(&a,&b);  
    cout << "a = " << a << ", b = "  
    << b << endl;  
    system("pause");  
    return 0;  
}  
void swap(int *x,int *y)  
{  
     int t = *x;  
     *x = *y;  
     *y = t;  
}
```
3.引用传参
引用传递方式是在函数定义时在形参前面加上引用运算符“&”。
示例：
```cpp
#include <iostream>  
using namespace std;  
void swap(int&,int&);  
int main()  
{  
    int a = 3, b = 4;  
    cout << "a = " << a << ", b = "  
    << b << endl;  
    swap(a,b);  
    cout << "a = " << a << ", b = "  
    << b << endl;  
    system("pause");  
    return 0;  
}  
void swap(int &x,int &y)  
{  
     int t = x;  
     x = y;  
     y = t;  
}
```
总结：
值传递方式容易理解，但形参值的改变不能对实参产生影响。指针传递方式虽然可以使得形参的改变对相应的实参有效，但如果在函数中反复利用指针进行间接访问，会使程序容易产生错误且难以阅读。如果以引用为参数，则既可以使得对形参的任何操作都能改变相应的数据，又使得函数调用显得更加简单易用。
下面有篇文章，个人觉得写得不错，可以参考去理解C和C++中参数的传递。
http://www.cnblogs.com/Romi/archive/2012/08/09/2630014.html
