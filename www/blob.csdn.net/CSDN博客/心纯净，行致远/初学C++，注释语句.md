# 初学C++，注释语句 - 心纯净，行致远 - CSDN博客





2017年10月08日 22:32:03[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：390








                阅读程序。阅读下列C++程序，对每条语句进行注释，说明其作用。


```cpp
#include <iostream>    //包含输入输出流头文件，作用是将下面用到的输入输出流函数的头文件包含在内
using namespace std;   //声明命名空间 std,作用是定义函数和全局变量
class CTest   //定义类CTest,作用是将程序中的数据和函数进行封装，将可开放给用户的定义为public，不能开放的定义为private
{
private:  int x, y;     //定义类中的私有成员，作用是包含两个int型变量，用户不能修改变量类型
public:                   //定义类中的共有函数成员，作用是将该共有成员开放给用户，主函数可以调用来处理数据
    CTest(int p1 = 0, int p2 = 0)  {  x = p1;  y = p2;  }  //带形参的构造函数，作用是初始化对象和构造时申请额外的内存
    CTest(CTest &p)  {  x = p.x;  y = p.y;  }      //拷贝构造函数，作用是用已存在的对象初始化当前新对象
    void Show( )  {  cout << x << “, “ << y << endl;  }    //类中公有成员中的显示函数，作用是输出变量值
};
int main( )     //主函数
{
    CTest obj1;    //使用类定义obj1数据类型
    obj1.Show( );        /调用obj1中公有成员的show函数，默认情况下使用默认形参值
    CTest obj2(2, 5);     //使用类定义obj2数据类型，并将2,5传递给形参值
    obj2.Show( );         //使用obj2中具有公有权限的show函数
    CTest obj3(obj2);     //拷贝构造函数，使用已经存在的obj2对象初始化当前新对象obj3
    obj3.Show( );         //使用obj3中具有公有权限的show函数
    return 0;            //返回值0
}
```



