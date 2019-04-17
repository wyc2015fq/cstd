# 类名::function() - Keith - CSDN博客





2014年02月15日 22:21:07[ke1th](https://me.csdn.net/u012436149)阅读数：916








```cpp
#include <iostream>
#include<string>
class People
{
public:
    void eat();
private:
    std::string name;
    int age;
};

class Chinese:public People
{
public:
    void eat();//override
};

void People::eat()
{
    std::cout<<"i'm eating!"<<std::endl;
}

void Chinese::eat()
{
    People::eat();//类中的方法应该是当类实例化是被对象调用，，这样可以使用的理论依据是什么
    std::cout<<"Chinese is eating!"<<std::endl;
}
int main()
{
    Chinese ch;
    ch.eat();
 //   People::eat(); 这种调用非静态，如果上面调用可以进行的话，这种函数调用的方式也可以进行。。但却是错的
    return 0;
}
```

```cpp
这里谈谈的我的想法，，类中的非静态方法是属于对象的，不能用类名：：方法名的方式调用。。但是当我们创建Chinese对象时，实际上是先调用的父类的构造函数（及创建了父类的对象）而  People则用于决定调用哪个基类的eat（）函数，，ch.eat()实际上就是基类对象调用基类eat(),然后再std::cout<<"Chinese is eating!"<<std::endl;
```

```cpp

```



