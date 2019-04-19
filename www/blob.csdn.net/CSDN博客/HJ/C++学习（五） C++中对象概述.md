# C++学习（五） C++中对象概述 - HJ - CSDN博客
2017年11月30日 13:20:51[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：213
个人分类：[C++](https://blog.csdn.net/feizaoSYUACM/article/category/7287457)
所属专栏：[C++学习](https://blog.csdn.net/column/details/18257.html)
**对象的定义**
对象的定义有两种方法：
```
a. 在定义类的同时直接定义；
b. 在使用时通过类进行定义；
```
（可以类比结构体变量定义）
**成员的访问**
定义了类及其对象，就可以调用公有成员函数实现对对象内部属性的访问。当然，不论是数据成员，还是成员函数，只要是公有的（public），在类的外部就可以通过类的对象进行访问，对公有成员的调用可以通过以下几种方法来实现：
```
a. 通过对象调用成员  对象名.公有成员  （其中，“.”称为对象选择符，简称点运算符）
b. 通过指向对象的指针调用成员  指向对象的指针 -> 成员 或  (*对象指针名).公有成员
c. 通过对象的引用调用成员  对象的引用.成员
```
需要注意，只有用public定义的公有成员才能使用圆点操作符访问。对象中的私有成员是类中隐藏的数据，类的外部不能访问对象的私有成员，只能通过该类的公有成员函数来访问他们。例如定义时钟类：
```cpp
#include<iostream>
using namespace std;
class Myclock{
private:
    int hour,minute,second;
public:
    void init();
    void update();
    void display();
}; 
int main()
{
    Myclock clock,*pclock; 、 //定义对象clock和指向Myclock类对象的指针pclock 
    clock.init();             //通过对象访问公有成员函数 
    pclock = &clock();        //指针pclock指向对象clock 
    pclock->display();        //通过指针访问成员函数 
    clock.hour = 4;           //错误，因为对象不能访问其私有成员 
    return 0;
}
```
举例：堆栈Cstack类的实现，该类用于存储字符
```cpp
#include<iostream>
using namespace std;
const int SIZE = 10; 
class Cstack{
private:
    char stk[SIZE];
    int position;
public:
    void init()
    {
        position = 0;
    }   
    char push(char ch);
    char pop();
};
char Cstack::push(char ch)
{
    if(position == SIZE)
    {
        cout<<"\n栈已满\n";
        return 0; 
    }
    stk[position++] = ch;
    return ch;
}
char Cstack::pop()
{
    if(position == 0)
    {
        cout<<"\n栈已空\n";
        return 0;
    }
    return stk[--position];
}
int main()
{
    Cstack s;
    s.init();
    char ch;
    cout<<"请输入字符："<<endl;
    cin>>ch;
    while(ch != '#' && s.push(ch))
    {
        cin>>ch;
    } 
    cout<<"\n现在输出栈内数据\n";
    while(ch = s.pop())
    {
        cout<<ch;
    }
    return 0;
}
/*
   程序的运行结果为：
   请输入字符：
   01234567896
   栈已满
   现在输出栈内数据
   9876543210
   栈已空 
*/
```
**名字解析**
在调用成员函数时，通常使用缩写形式，如Tdate例中的表达式s.set(2,15,1998)就是s.Tdate::set(2,15,1998)的缩写，因此可以定义两个或多个类的具有相同名字的成员而不会产生二义性。
**this指针**
当一个成员函数被调用时，C++语言自动向它传递一个隐含的参数，该参数是一个指向接受该函数的对象的指针，在程序中可以使用关键字this来引用该指针，因此称该指针为this指针。this指针是C++语言实现封装的一种机制，它将成员和用于操作这些成员的成员函数连接在一起。例如Tdate类的成员函数set被定义为：
```
void Tdate::set(int m,int d,int y)
{
    month = m; day = d; year = y; 
}
```
其中，对month、day和year的引用，表示的是在该成员函数被调用时，引用接受该函数调用的对象中的数据成员month、day和year。例如，对于下面的语句：
```
Tdate dd;
dd.set(5,16,1991);
```
当调用成员函数set时，该成员函数的this指针指向类Tdate的对象dd。成员函数set中对month、day和year的引用表示引用对象dd的数据成员。C++语言编译器所认识的成员函数set的定义形式为：
```
void Tdate::set(int m,int d,int y)
{
    this -> month = m; this -> day = d; this -> year = y; 
}
```
即对于该成员函数中访问的类的任何数据成员，C++语言编译器都认为是访问this指针所指向对象的成员。由于不同的对象调用成员函数set()时，this指针指向不同的对象，因此成员函数set()可以为不同对象的month、day和year赋初值。使用this指针，保证了每个对象可以拥有不同的数据成员值，但处理这些数据成员的代码可以被所有的对象共享。
**带默认参数的成员函数**
其调用规则与普通函数相同
```cpp
#include<iostream>
using namespace std;
class Tdate{
public:
    void set(int m = 5,int d = 16,int y = 1991)
    {
        month = m; day = d; year = y;
    }
    void print()
    {
        cout<<month<<"/"<<day<<"/"<<year<<endl; 
    }
private:
    int month;
    int day;
    int year;
};
int main()
{
    Tdate a,b,c;
    a.set(4,12,1996);
    b.set(3);
    c.set(8,10);
    a.print();
    b.print();
    c.print();
    return 0;
}
/*
  程序运行结果为：
  4/12/1996
  3/16/1991
  8/10/1991
  */
```
**重载函数**
类的成员函数的重载与全局函数的重载方法相同
```cpp
#include<iostream>
using namespace std;
class cube{
public:
    int volume(int ht,int wd)
    {
        return ht * wd;
    }
    int volume(int ht,int wd,int dp)
    {
        height = ht;
        width = wd;
        depth = dp;
        return height * width * depth;
    }
private:
    int height,width,depth;
};
int main()
{
    cube cube1;
    cout<<cube1.volume(10,20)<<endl;   //调用带2个参数的成员函数
    cout<<cube1.volume(10,20,30)<<endl;   //调用带3个参数的成员函数
    return 0; 
}
/*
    程序的运行结果为：
    200
    6000
*/
```
