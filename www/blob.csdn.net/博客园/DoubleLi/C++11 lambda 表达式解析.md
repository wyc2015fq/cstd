# C++11 lambda 表达式解析 - DoubleLi - 博客园






C++11 新增了很多特性，lambda 表达式是其中之一，如果你想了解的 C++11 完整特性，建议去[这里](http://www.open-std.org/jtc1/sc22/wg21/)，[这里](http://www.stroustrup.com/C++11FAQ.html)，[这里](http://en.cppreference.com/w/cpp)，还有[这里](http://en.wikipedia.org/wiki/C%2B%2B11)看看。本文作为 5 月的最后一篇博客，将介绍 C++11 的 lambda 表达式。

很多语言都提供了 lambda 表达式，如 Python，Java 8。lambda 表达式可以方便地构造匿名函数，如果你的代码里面存在大量的小函数，而这些函数一般只被调用一次，那么不妨将他们重构成 lambda 表达式。

C++11 的 lambda 表达式规范如下：
|`**[**`capture `**]**``**(**`params `**)**`mutable exception attribute `**->**`ret `**{**`body `**}**`|(1)||
|----|----|----|
|`**[**`capture `**]**``**(**`params `**)**``**->**`ret `**{**`body `**}**`|(2)||
|`**[**`capture `**]**``**(**`params `**)**``**{**`body `**}**`|(3)||
|`**[**`capture `**]**``**{**`body `**}**`|(4)||

其中
- (1) 是完整的 lambda 表达式形式，
- (2) const 类型的 lambda 表达式，该类型的表达式不能改捕获("capture")列表中的值。
- (3)省略了返回值类型的 lambda 表达式，但是该 lambda 表达式的返回类型可以按照下列规则推演出来：
- 如果 lambda 代码块中包含了 return 语句，则该 lambda 表达式的返回类型由 return 语句的返回类型确定。
- 如果没有 return 语句，则类似 void f(...) 函数。

- 省略了参数列表，类似于无参函数 f()。

**mutable** 修饰符说明 lambda 表达式体内的代码可以修改被捕获的变量，并且可以访问被捕获对象的 non-const 方法。

**exception** 说明 lambda 表达式是否抛出异常(`**noexcept**`)，以及抛出何种异常，类似于void f() throw(X, Y)。

**attribute** 用来声明属性。


另外，**capture** 指定了在可见域范围内 lambda 表达式的代码内可见得外部变量的列表，具体解释如下：
- `**[a,&b]**` a变量以值的方式呗捕获，b以引用的方式被捕获。
- `**[this]**` 以值的方式捕获 this 指针。
- `**[&]**` 以引用的方式捕获所有的外部自动变量。
- `**[=]**` 以值的方式捕获所有的外部自动变量。
- `**[]**` 不捕获外部的任何变量。

此外，**params **指定 lambda 表达式的参数。

**一个具体的 C++11 lambda 表达式例子：**

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>
 
int main()
{
    std::vector<int> c { 1,2,3,4,5,6,7 };
    int x = 5;
    c.erase(std::remove_if(c.begin(), c.end(), [x](int n) { return n < x; } ), c.end());
 
    std::cout << "c: ";
    for (auto i: c) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
 
    // the type of a closure cannot be named, but can be inferred with auto
    auto func1 = [](int i) { return i+4; };
    std::cout << "func1: " << func1(6) << '\n'; 
 
    // like all callable objects, closures can be captured in std::function
    // (this may incur unnecessary overhead)
    std::function<int(int)> func2 = [](int i) { return i+4; };
    std::cout << "func2: " << func2(6) << '\n'; 
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

(完)









