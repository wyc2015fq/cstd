# const int*和int* const的含义 - writeeee的专栏 - CSDN博客
2016年08月26日 17:37:28[writeeee](https://me.csdn.net/writeeee)阅读数：948
const int*和int* const中的const修饰的对象不同，const int*修饰值，不能改变指针所指的值；int* const修饰指针，不能改变指针的地址。举个例子：
```
#include <iostream>
using namespace std;
///////////////////////////SubMain//////////////////////////////////
int main(int argc, char *argv[])
{
    int n = 0;
    int* p1 = &n;
    const int* p2 = &n;     // 底层const，可以改变指针地址，不能改变指针指向的值
    int* const p3 = &n;     // 顶层const，可以改变指针指向的值，不能改变指针地址
    (*p1)++;
    cout << n << endl;
    // error C3892: “p2”: 不能给常量赋值
//  (*p2)++;
//  cout << n << endl;
    (*p3)++;
    cout << n << endl;
    p1 = nullptr;
    p2 = nullptr;
    // error C3892: “p3”: 不能给常量赋值
// p3 = nullptr;
    system("pause");
    return EXIT_SUCCESS;
}
///////////////////////////End Sub//////////////////////////////////
/************************************************************************/
/* 
1
2
请按任意键继续. . .
*/
/************************************************************************/
```
