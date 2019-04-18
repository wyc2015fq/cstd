# c++ char* char*&的区别 - writeeee的专栏 - CSDN博客
2015年09月06日 11:04:17[writeeee](https://me.csdn.net/writeeee)阅读数：371
两者都是把地址传到函数中。都可以对传入的指针指向的值进行修改
一旦传入的指针修改后，char*不会影响到函数外的指针指向的值，char*&会影响到函数外指针指向的值，因为此指针已经不是传入的指针了
```cpp
#include<iostream>
using namespace std;
struct point
{
int x;
int y;
};
void changenum1(point *&pnum)
{
pnum = new point;
pnum->x = 4;
}
void changenum2(point *pnum)
{
pnum = new point;
pnum->x = 4;
}
void test1()
{
point *num=new point;
num->x=10;
changenum1(num);
std::cout<<num->x<<endl;
}
void test2()
{
point *num=new point;
num->x=10;
changenum2(num);
std::cout<<num->x<<endl;
}
void main()
{
  test1();
  test2();
}
```
![](https://img-blog.csdn.net/20150906110049994?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果不修改指针的地址，那么这两个应该是有同等的效果。调用方法时对char*&直接传入指针即可。
