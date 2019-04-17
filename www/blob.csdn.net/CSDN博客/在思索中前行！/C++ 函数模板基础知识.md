# C++ 函数模板基础知识 - 在思索中前行！ - CSDN博客





2015年03月30日 22:29:44[_Tham](https://me.csdn.net/txl16211)阅读数：549标签：[C++ 函数模板																[C++ 模板																[C++ template																[template](https://so.csdn.net/so/search/s.do?q=template&t=blog)
个人分类：[数据结构																[C/C++](https://blog.csdn.net/txl16211/article/category/2284665)](https://blog.csdn.net/txl16211/article/category/2409063)





为什么要引入模板：为了避免代码重复，程序员可以编写脱离数据类型通用模板。

模板的分类：函数模板 + 类模板

注意：模板的声明或定义只能在全局，命名空间或类范围内进行。不能在函数内进行，比如不能在main函数中声明或定义一个模板。

函数模板:

定义：

函数模板：关键词在后两个字，模板：提供一类函数的抽象，以任意类型T为参数把具有相同程序正文的一类函数抽象出来，可以适合任意类型T的参数。

模板函数：重点在函数，是对函数模板进行参数实例化后的结果，是一个具体的函数。

什么时候使用函数模板：会有一类函数，它们几乎相同，唯一的区别就是形参类型不同。这时我们可以撇开不同的数据类型，创建一个模板，并把数据类型也当作一个参数而设计一个模板

举例说明：编写比较两个整型数据大小的函数，它只适合于整型数据。如果是比较两个浮点型数据，就需要重新编写另外一个函数。如果我们使用函数模板的话，可以把数据类型忽略，编写一个max函数，既可以实现int，也可以实现浮点型。

语法：



```cpp
template <class T>   //模板声明格式  
返回值类型 函数名(模板形参表)   //模板函数形参表  
{  
    //函数定义体  
}  
函数调用：函数名(模板实参表) //和普通的函数调用没有区别，只不过运行时，系统会首先根据参数类型确定数据类型，生成一个模板函数。
```




说明：

        1、这里的class指一类的意思

        2、参数有两种：类型参数 + 函数参数

        3、T 为类型参数，可以是预定义的数据类型，可以是用户自定义的类型（类，结构体等）。在运行中必须用实际的数据类型替代它

        4、template 语句 和 函数模板定义语句不允许有别的语句

如：



```cpp
template <class T>  
    int i;            //这是不允许的.  
    T max(T a , T b)  
    {  
        return (a>b)?a:b;  
    }
```

举例：求两个数最大值，使用模板



```cpp
template <class T>   
    T max(T a , T b)  
    {  
        return (a>b)?a:b;  
    }  
    调用：  
    int i1,i2;  
    flaot f1,f2;  
    max(i1,i2);  
    max(f1,f2);
```



运行过程可分两部分：

      第一、传递参数类型：用模板实参int将 类型参数T进行实例化。即用 int  代替T

      第二、传递参数：把参数传递给函数

函数模板的重载：

引入函数模板重载的原因：函数模板要求要传入参数类型必须全都相同，不同则报错。使用函数模板重载就是要解决这个问题

引入两种方法解决这个问题：

        1、函数模板可以使用多个模板参数：有几个不同的类型，就是用不同的模板参数来代表它。

        2、超载一个函数模板：定义一个模板后，在定义一个超载函数

具体来说：

       1、定义一个多参的函数模板：

```cpp
template <class T,class D>  
    T max(T a,D b)  
    {  
        return (a>b)?a:b;  
    }
```
调用：

```cpp
int a；float b；  
max(a,b)
```
2、超载一个函数模板

```cpp
template <class T>  
    T max(T a,T b)  
    {  
        return (a>b)?a:b;  
    }  
    int max(int,int);//用户自己定义一个同名函数
```

```cpp
参数调用：int m,n;  
    char a,b;  
    char s[10];  
    max(m,n);//调用函数 实参和函数的参数类型完全一致，直接调用函数  
    max(a,b);//调用模板 实参和函数参数类型不同，直接掉模板  
    max(m,a);//调用函数 实参的类型不同，模板不适合，则再次调用函数  
    max(m,s);//报错-函数也不适合，直接报错
```

分析：调用函数时的顺序：先调用函数 ---  其次套用模板  --- 再调用函数 --- 报错

运行步骤：

如果调用语句的实参类型和函数类型完全一致，这时不找模板，而优先使用函数。

如果调用语句的实参类型和函数类型不一致，应该找模板

如果调用语句的实参各自类型不同，应重新调用函数，并试着把实参类型转换为形参类型，成功则调用，失败，则报错


函数模板和类结合

举例：

```cpp
#include <iostream>  
    using namespace std;  
    class Point  
    {  
    private:  
        int x;  
    public:  
        Point(int x1);  
        bool operator>(const Point& p);  
        friend ostream& operator<<(ostream& cout,const Point& p);  
    };  
    Point::Point(int x1)  
    {  
        x=x1;  
    }  
    bool Point::operator>(const Point& p)  
    {  
        return (x>p.x)?true:false;  
    }  
    ostream& operator<<(ostream& cout,const Point& p)  
    {  
        cout<<p.x;  
        return cout;  
    }  
    template <class T>  
    T& max(T& a,T& b)  
    {  
        cout<<"调用函数模板"<<endl;  
        return (a>b)?a:b;  
    }  
    void main()  
    {  
        Point a(1);  
        Point b(2);  
        cout<<max(a,b);  
        system("pause");  
    }
```
什么时候使用模板：
1、创建一个类型安全的集合类(如，堆栈)用来处理各种类型的数据

2、为函数添加额外的类型检查以避免获得空指针

模板的优势：为函数大多数以上应用可以不用模板实现

模板具有以下几个优势：

1、开发容易。你可以只为你的类或函数创建一个普通的版本代替手工创建特殊情况处理。

2、理解容易。模板为抽象类型信息提供了一个直截了当的方法。

3、类型安全。模板使用的类型在编译时是明确的，编译器可以在发生错误之前进行类型检查。](https://so.csdn.net/so/search/s.do?q=C++ template&t=blog)](https://so.csdn.net/so/search/s.do?q=C++ 模板&t=blog)](https://so.csdn.net/so/search/s.do?q=C++ 函数模板&t=blog)




