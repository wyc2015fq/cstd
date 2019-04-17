# C++11使用emplace_back代替push_back - DoubleLi - 博客园






最近在写一段代码的时候，突然很好奇C++11中对push_back有没有什么改进以增加效率，上网搜了一些资料，发现果然新增了emplace_back方法，比push_back的效率要高很多。

首先，写了一个类用于计时，

```
//time_interval.h
#pragma once

#include <iostream>
#include <memory>
#include <string>
#ifdef GCC
#include <sys/time.h>
#else
#include <ctime>
#endif // GCC

class TimeInterval
{
public:
    TimeInterval(const std::string& d) : detail(d)
    {
        init();
    }

    TimeInterval()
    {
        init();
    }

    ~TimeInterval()
    {
#ifdef GCC
        gettimeofday(&end, NULL);
        std::cout << detail 
            << 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000 
            << " ms" << endl;
#else
        end = clock();
        std::cout << detail 
            << (double)(end - start) << " ms" << std::endl;
#endif // GCC
    }

protected:
    void init() {
#ifdef GCC
        gettimeofday(&start, NULL);
#else
        start = clock();
#endif // GCC
    }
private:
    std::string detail;
#ifdef GCC
    timeval start, end;
#else
    clock_t start, end;
#endif // GCC
};

#define TIME_INTERVAL_SCOPE(d)   std::shared_ptr<TimeInterval> time_interval_scope_begin = std::make_shared<TimeInterval>(d)
```



使用方法就是在作用域中使用宏TIME_INTERVAL_SCOPE(d)，d为打印用的字符串，输出作用域的耗时情况。

其次，看一下现在push到vector的5种方法的耗时情况对比：

```cpp
#include <vector>
#include <string>
#include "time_interval.h"

int main() {


    std::vector<std::string> v;
    int count = 10000000;
    v.reserve(count);       //预分配十万大小，排除掉分配内存的时间

    {
        TIME_INTERVAL_SCOPE("push_back string:");
        for (int i = 0; i < count; i++)
        {
            std::string temp("ceshi");
            v.push_back(temp);// push_back(const string&)，参数是左值引用
        }
    }

    v.clear();
    {
        TIME_INTERVAL_SCOPE("push_back move(string):");
        for (int i = 0; i < count; i++)
        {
            std::string temp("ceshi");
            v.push_back(std::move(temp));// push_back(string &&), 参数是右值引用
        }
    }

    v.clear();
    {
        TIME_INTERVAL_SCOPE("push_back(string):");
        for (int i = 0; i < count; i++)
        {
            v.push_back(std::string("ceshi"));// push_back(string &&), 参数是右值引用
        }
    }

    v.clear();
    {
        TIME_INTERVAL_SCOPE("push_back(c string):");
        for (int i = 0; i < count; i++)
        {
            v.push_back("ceshi");// push_back(string &&), 参数是右值引用
        }
    }

    v.clear();
    {
        TIME_INTERVAL_SCOPE("emplace_back(c string):");
        for (int i = 0; i < count; i++)
        {
            v.emplace_back("ceshi");// 只有一次构造函数，不调用拷贝构造函数，速度最快
        }
    }
}
```



vs2015 release下编译，运行结果：

> 
push_back string:327 ms 
push_back move(string):213 ms 
push_back(string):229 ms 
push_back(c string):215 ms 
emplace_back(c string):122 ms


第1中方法耗时最长，原因显而易见，将调用左值引用的push_back，且将会调用一次string的拷贝构造函数，比较耗时，这里的string还算很短的，如果很长的话，差异会更大

第2、3、4中方法耗时基本一样，参数为右值，将调用右值引用的push_back，故调用string的移动构造函数，移动构造函数耗时比拷贝构造函数少，因为不需要重新分配内存空间。

第5中方法耗时最少，因为emplace_back只调用构造函数，没有移动构造函数，也没有拷贝构造函数。

为了证实上述论断，我们自定义一个类，并在普通构造函数、拷贝构造函数、移动构造函数中打印相应描述：

```cpp
#include <vector>
#include <string>
#include "time_interval.h"

class Foo {
public:
    Foo(std::string str) : name(str) {
        std::cout << "constructor" << std::endl;
    }
    Foo(const Foo& f) : name(f.name) {
        std::cout << "copy constructor" << std::endl;
    }
    Foo(Foo&& f) : name(std::move(f.name)){
        std::cout << "move constructor" << std::endl;
    }

private:
    std::string name;
};
int main() {


    std::vector<Foo> v;
    int count = 10000000;
    v.reserve(count);       //预分配十万大小，排除掉分配内存的时间

    {
        TIME_INTERVAL_SCOPE("push_back T:");
        Foo temp("ceshi");
        v.push_back(temp);// push_back(const T&)，参数是左值引用
        //打印结果：
        //constructor
        //copy constructor
    }

    v.clear();
    {
        TIME_INTERVAL_SCOPE("push_back move(T):");
        Foo temp("ceshi");
        v.push_back(std::move(temp));// push_back(T &&), 参数是右值引用
        //打印结果：
        //constructor
        //move constructor
    }

    v.clear();
    {
        TIME_INTERVAL_SCOPE("push_back(T&&):");
        v.push_back(Foo("ceshi"));// push_back(T &&), 参数是右值引用
        //打印结果：
        //constructor
        //move constructor
    }

    v.clear();
    {
        std::string temp = "ceshi";
        TIME_INTERVAL_SCOPE("push_back(string):");
        v.push_back(temp);// push_back(T &&), 参数是右值引用
        //打印结果：
        //constructor
        //move constructor
    }

    v.clear();
    {
        std::string temp = "ceshi";
        TIME_INTERVAL_SCOPE("emplace_back(string):");
        v.emplace_back(temp);// 只有一次构造函数，不调用拷贝构造函数，速度最快
        //打印结果：
        //constructor
    }
}
```



**结论：在C++11情况下，果断用emplace_back代替push_back**









