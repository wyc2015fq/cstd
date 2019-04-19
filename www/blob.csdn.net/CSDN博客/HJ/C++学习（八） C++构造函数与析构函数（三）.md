# C++学习（八） C++构造函数与析构函数（三） - HJ - CSDN博客
2017年12月04日 13:35:02[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：165
所属专栏：[C++学习](https://blog.csdn.net/column/details/18257.html)
**拷贝构造函数的调用**
拷贝构造函数的功能是用一个已有的对象来初始化一个被创建的同类对象，是一种特殊的构造函数，具有一般构造函数的所有特性，当创建一个新对象时系统会自动调用它；其形参是对类对象的引用，它的特殊功能是将参数代表的对象逐域拷贝到新创建的对象中。
用户刻意根据实际问题需要定义特定的拷贝构造函数，以实现同类对象之间的数据成员的传递。如果用户没有声明类的拷贝构造函数，系统就会自动生成一个默认拷贝构造函数，这个默认拷贝构造函数的功能是把初始对象的每个数据成员的值都复制到新建立的对象中。拷贝构造函数的声明形式为：
```
类名(类名 & 对象名)
```
下面定义了一个cat类和cat类的拷贝构造函数：
```
class cat{
private:
    int age;
    float weight;
    char *color;
public:
    cat();
    cat(cat &);
    void play();
    void hunt(); 
};
cat::cat(cat &other)
{
    age = other.age;
    weight = other.weight;
    color = other.color;
}
```
以下四种情况下系统还会自动调用拷贝构造函数：
①用类的一个对象去初始化另一个对象；
```matlab
cat cat1;
cat cat2(cat1);
```
②用类的一个对象去初始化另一个对象的另外一种形式；
`cat cat2 = cat1;`
③对象作为函数参数传递时，调用拷贝构造函数；
```matlab
f(cat a){ }
cat b;
f(b);
```
④如果函数的返回值是类的对象，函数调用返回时，调用拷贝构造函数；
```
cat f(){
    cat a;
    ...
    return a;
}
cat b;
b = f()；
```
**深拷贝构造函数和浅拷贝构造函数**
拷贝构造函数分为深拷贝和浅拷贝两种构造函数。由C++语言提供的默认拷贝构造函数只是对对象进行浅拷贝（逐个成员值依次拷贝），即只复制对象空间而不复制资源。
一般情况下，只需要使用系统提供的浅拷贝构造函数即可，但是如果对象的数据成员包括指向堆空间的指针，就不能使用这种拷贝方式，因为两个对象都都拥有同一个资源，如果在给其中一个对象赋值的时候，另一个对象的该成员变量值同样会被改变。而且在对象析构时，该资源将经历两次资源返还，此时必须定义深拷贝构造函数，为创建的对象分配堆空间，否则会出现动态分配的指针变量悬空的情况。深拷贝需要同时复制对象空间和资源。
**说明：**
在同时满足以下两个条件时，就必须要定义深拷贝构造函数：
①肯定要调用拷贝构造函数；
②数据成员包括指向堆内存的指针变量；
```cpp
#include<iostream>
#include<cstring>
using namespace std;
class Person{
    public:
        Person(char *na)          //构造函数 
        {
            cout<<"call constructor"<<endl;
            name = new char[strlen(na) + 1];      //使用new进行动态内存分配 
            if(name != 0)
              strcpy(name,na);
        }
        Person(Person &p)            //深拷贝构造函数 
        {
            cout<<"call copy constructor"<<endl;
            name = new char[strlen(p.name) + 1];  //复制资源 
            if(name != 0)
              strcpy(name,p.name);     //复制对象空间 
            }
        void printname()
        {
            cout<<name<<endl;
        } 
        ~Person()          //析构函数的定义 
        {
            delete name;
        } 
    private:
        char *name;
}; 
int main()
{
    Person wang("wang");
    Person li(wang);
    wang.printname();
    li.printname();
    return 0;
}
/*
    程序运行的结果为：
    call constructor
    call copy constructor
    wang 
    wang
*/
```
**注意，当有使用new动态分配内存空间的数据成员，在析构函数中使用delete进行动态内存空间的释放以及对赋值=运算符进行重载时，应该自定义深拷贝构造函数。**
