# C++静态多态CRTP - xcyl的口袋 - CSDN博客
2016年07月18日 21:39:11[xcyl](https://me.csdn.net/cai6811376)阅读数：2244
CRTP指的是Curiously Recurring Template Prattern，这是一种可以实现静态多态的惯用法。
我们知道C++中，你不能将基类的虚函数定义为模板函数。这时你就可以考虑使用CRTP。
在以后的Modern C++改进设计模式中会有所使用。
并且，CRTP不需要像动态多态那样携带虚表和虚指针，拥有更快的运行效率[http://eli.thegreenplace.net/2013/12/05/the-cost-of-dynamic-virtual-calls-vs-static-crtp-dispatch-in-c/](http://eli.thegreenplace.net/2013/12/05/the-cost-of-dynamic-virtual-calls-vs-static-crtp-dispatch-in-c/)
```cpp
#include <iostream>
using namespace std;
template<typename T>
class BaseClass
{
public:
    void Func()
    {
        static_cast<T&>(*this).funcImpl();
    }
private:
    void funcImpl()
    {
        cout << "Base class" << endl;
    }
};
class Base : public BaseClass<Base>
{
};
class Derive : public BaseClass<Derive>
{
public:
    void funcImpl()
    {
        cout << "Derive class" << endl;
    }
};
int main(int argc, char* argv[])
{
    BaseClass<Derive> der;
    der.Func();
    BaseClass<Base> bas;
    bas.Func();
    return 0;
}
```
```
//结果
Derive class
Base class
请按任意键继续. . .
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：288214068
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
