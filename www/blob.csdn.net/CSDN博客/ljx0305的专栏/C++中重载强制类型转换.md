# C++中重载强制类型转换 - ljx0305的专栏 - CSDN博客
2009年04月09日 17:07:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：3742
C++中重载强制类型转换
例1  在类中直接定义成员函数
将Conver类对象强制转换成int型
#include <iostream>
using namespace std;
class Conver
{
      public:
             int b;
operator int(){return b;} //注：这里是把函数的实现直接写出来
};
int main()
{
    Conver c;
    c.b=100;
    //printf("%d",(int)c);
    cout<<(int)c<<endl
    return 0;
}
例2  在类定义外定义成员函数
将Conver类对象强制转换成int型
#include <iostream>
using namespace std;
class Conver
{
      public:
             int b;
operator int(); //注：这里只是声明强制转换函数
};
//注：下边是定义强制转换函数
Cover::operator int()
{
    return b;
}
int main()
{
    Conver c;
    c.b=100;
    //printf("%d",(int)c);
    cout<<(int)c<<endl;
    return 0;
}
上边两个例子在Dev c++下编译运行通过
输出100
