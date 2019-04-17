# C++ 类的析构函数 - DumpDoctorWang的博客 - CSDN博客





2018年04月16日 21:23:00[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：315








很多博客已经就析构函数作了非常详细的讨论，我在这篇博客中仅仅对发生异常时析构函数的行为做讨论。

## 一、析构函数基础

1. [C++ 析构函数](https://blog.csdn.net/github_35160620/article/details/52602332)

2.[C++之类的析构函数](http://www.cnblogs.com/MrListening/p/5567762.html)

## 二、程序发生异常时析构函数的行为

在C++中，对资源应该使用类来管理，在构造函数中获得资源，在析构函数中释放资源。但是，当程序发生异常时(调用exit、抛出异常)，析构函数是否能被正常的调用？答案是否定的。

1、调用exit()函数

观察如下代码

```cpp
#include <iostream>

using namespace std;

class A{
public:
    A(){
        cout<<"构造函数"<<endl;
    }
    ~A(){
        cout<<"析构函数"<<endl;
    }
};

void quit(){
    exit(-1);
}
void exec(){
    throw std::runtime_error("test");
}

int main() {
    A a;
    quit();
    return 0;
}
```

它的输出为:

```
构造函数

Process finished with exit code 255
```

可以看到，在main函数之前调用exit()，析构函数是不会被调用的，这样就会造成资源释放失败。

2、程序抛出异常

观察如下代码

```cpp
#include <iostream>

using namespace std;

class A{
public:
    A(){
        cout<<"构造函数"<<endl;
    }
    ~A(){
        cout<<"析构函数"<<endl;
    }
};

void quit(){
    exit(-1);
}
void exec(){
    throw std::runtime_error("test");
}

int main() {
    A a;
    exec();
    return 0;
}
```

它的输出为：

```
构造函数
terminate called after throwing an instance of 'std::runtime_error'
what():  test

Process finished with exit code 134 (interrupted by signal 6: SIGABRT)
```

同样可知，析构函数并未被调用。



