# C++“类”小练习 - 心纯净，行致远 - CSDN博客





2017年10月08日 22:33:52[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：727








                编写程序。编写一个关于圆形的C++程序。要求用定义一个圆形类Circle，其中包含如下成员：
1)         1个私有数据成员（半径）。
2)         3个公有函数成员（设置半径、计算面积、计算周长）。
3)         3个构造函数（不带参数的构造函数、带参数的构造函数和拷贝构造函数）。
主函数main使用圆形类Circle创建圆形对象，要求：
1)         定义一个圆对象c1，从键盘输入一个值x并将其设定为c1的半径，计算并显示c1的面积和周长
2)         再定义一个圆对象c2，并将半径初始化为2x，计算并显示c2的面积和周长
3)         再定义一个圆对象c3，并用c1初始化c3，计算并显示c3的面积和周长


```cpp
#include <iostream>

using namespace std;
#define pi 3.14
class Circle
{
    private:          //私有权限
        double x;
    public:          //公有权限
        double getR(){     //从主函数中读取半径 ，并将半径传递给私有权限中的x
            return x;
           }
        void setR(double x){     //接受私有权限的半径，并将半径设置为新的半径
                this->x=x;
        }
        double Area()
        {
            return(pi*x*x);
        }
        double Len()
        {
            return(2*pi*x);
        }
    Circle(){x=0;}        //不带形参的构造函数
    Circle(double x){this->x=x;}     //带形参的构造函数
    Circle(Circle&c){this->x=c.getR();}   //拷贝构造函数
};
    
int main()
{
    double x;
    Circle c1;
    cin>>x;
    c1.setR(x);
    cout<<c1.Area()<<endl;
    cout<<c1.Len()<<endl;

    Circle c2(2*x);
    cout<<c2.Area()<<endl;
    cout<<c2.Len()<<endl;

    Circle c3(c1);
    cout<<c3.Area()<<endl;
    cout<<c3.Len()<<endl;
    return 0;
}
```

这么简单，竟然折腾了快一个上午，气死我了。。。。



