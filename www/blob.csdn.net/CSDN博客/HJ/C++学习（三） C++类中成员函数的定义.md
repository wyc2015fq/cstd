# C++学习（三） C++类中成员函数的定义 - HJ - CSDN博客
2017年11月28日 13:18:01[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：944
所属专栏：[C++学习](https://blog.csdn.net/column/details/18257.html)
类的数据成员说明对象的特征，而成员函数决定对象的操作行为。
类的成员函数有两种定义方法：外联定义和内联定义。
（1）**外联成员函数**（外联函数）
外联定义成员函数是指在类定义体中声明成员函数，而在类定义体外定义成员函数。在类中声明成员函数时，它所带的函数参数可以只指出其类型，而省略参数名；在类外定义成员函数时必须在函数名之前缀上类名，在函数名和类名之间加上作用域区分符::，作用域区分符::指明一个成员函数或数据成员所在的类。作用域区分符::前若不加类名，则成为全局数据或全局函数（非成员函数）
在类外定义成员函数的具体形式如下：
```
返回值类型 类名::成员函数名(形式参数表)
{
     //函数体
}
```
如日期类的可以设置三个如下的成员函数：
```
void Tdate::set(int m,int d,int y)
{
    month = m;
    day = d;
    year = y;
} 
int Tdate::isLeapYear()
{
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}
void Tdate::print()
{
    cout<<month<<"/"<<day<<"/"<<year<<endl;
}
```
（2）**内联成员函数**（内联函数、内部函数、内置函数）
程序在编译时将内联成员函数的代码插入在函数的每个调用处，作为函数体的内部扩展，用来避免函数调用机制所带来的开销，提高程序的执行效率。可以将那些仅由少数几条简单语句组成，却在程序中被频繁调用的函数定义为内联成员函数。内联成员函数有两种定义方法，一种方法是在类定义体内定义（**隐式声明**），另一种方法是使用inline关键字（**显式声明**）。
a.在类定义体内定义内联成员函数：
```
class Tdate{
public:
    void Tdate::set(int m,int d,int y)
    {
        month = m;
        day = d;
        year = y;
    } 
    int Tdate::isLeapYear()
    {
        return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
    }
    void Tdate::print()
    {
        cout<<month<<"/"<<day<<"/"<<year<<endl;
    }
private:
    int month;
    int day;
    int year;
};
```
b.使用关键字inline定义内联成员函数：
```
inline void Tdate::set(int m,int d,int y)
{
    month = m;
    day = d;
    year = y;
}
```
或
```
void inline Tdate::set(int m,int d,int y)
{
    month = m;
    day = d;
    year = y;
}
```
