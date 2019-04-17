# 【mem_fun/mem_fun_ref】成员函数作为函数对象使用 - xiaxzhou的博客 - CSDN博客





2017年05月09日 11:22:09[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：488








# 如何将成员函数作为函数对象使用

> 
今天，我的**2B**队友提出了一个有意思的问题：


下图代码中，solution类成员函数中使用了sort()，sort()使用了自定义的比较方法isless()，isless函数也是solution类的成员函数。
- 
出现的问题是：
- 当将isless()定义为**static**时，代码可以运行。
- 但若将isless()定义为**普通函数**时，则编译出错。


![问题代码图片](https://img-blog.csdn.net/20170509103710022?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlheHpob3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 
问题的原因：

- 成员函数

普通的C++成员函数都隐含了一个传递函数作为参数，亦即“**this**”指针，C++通过传递一个指向自身的指针给其成员函数从而实现程序函数可以访问C++的数据成员。所以普通的C++成员函数不能直接作为函数对象使用。
- STL

STL中的一种惯例是：函数或者函数对象被调用的时候，总是使用**非成员函数**的语法形式。（effective STL 第41条）

> 
解决方法

- 首先isless函数需要修改为如下形式：
`bool isless(const solution & rhs){ return value>rhs.value; }`
函数只接受一个参数，另一个参数为隐式的this指针

注：对于只接受一个参数的函数对象，则改为：
`void dosomething(){ ++value; }`- 使用**mem_fun_ref()**函数：在非类成员函数中使用类的成员函数作为函数对象，代码如下：

```cpp
class solution{
public:
    bool isless(const solution & rhs){ return value>rhs.value; }
    void dosomething(){ ++value; }
    void fun(){}
public:
    int value;
};

void main()
{
    vector<solution> vec;
    solution sol;
    sol.value = 0;
    vec.push_back(sol);
    sol.value = 2;
    vec.push_back(sol);
    sol.value = 1;
    vec.push_back(sol); 
    sort(vec.begin(), vec.end(), mem_fun_ref(&solution::isless));
    for_each(vec.begin(), vec.end(), mem_fun_ref(&solution::dosomething));
}
```

代码运行结果：

> 
sort： 
![这里写图片描述](https://img-blog.csdn.net/20170509110942804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlheHpob3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

  for_each: 
![这里写图片描述](https://img-blog.csdn.net/20170509111722043?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlheHpob3U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- 对于在类成员函数中使用，用法一样：

```cpp
class solution{
public:
    bool isless(const solution & rhs){ return value>rhs.value; }
    void dosomething(){ ++value; }
    void fun(){
        vector<solution> vec;
        solution sol;
        sol.value = 0;
        vec.push_back(sol);
        sol.value = 2;
        vec.push_back(sol);
        sol.value = 1;
        vec.push_back(sol);
        sort(vec.begin(), vec.end(), mem_fun_ref(&solution::isless));
        for_each(vec.begin(), vec.end(), mem_fun_ref(&solution::dosomething));
    }
public:
    int value;
};

void main()
{
    solution sol;
    sol.fun();
}
```

> 
本以为这样问题就搞定了！结果2B队友说他的容器中放的不是solution类的对象。。。。。。。。。。。我去，不带这么坑人的

- 为了解决这个问题，需要使用**bind（）**函数，将this指针手动传入isless（）函数中，代码如下：

```cpp
class solution{
public:
    bool isless(const int & lhs,const int & rhs){ return lhs>rhs; }
    void dosomething(){ ++value; }
    void fun(){
        vector<int> vec;
        vec.push_back(0);
        vec.push_back(2);
        vec.push_back(1);

        using namespace std::placeholders;
        auto iter = &solution::isless;
        sort(vec.begin(), vec.end(), bind(iter, this, _1, _2));
    }
public:
    int value;
};

void main()
{
    solution sol;
    sol.fun();
}
```

> 
问题终于解决了。**但是**。。。。。。一开始用的static不是挺好的吗？干嘛费这么大劲？






