# C++11  lambda - u013366022的专栏 - CSDN博客
2016年11月15日 10:23:23[slitaz](https://me.csdn.net/u013366022)阅读数：172
**为什么需要lambda函数**
匿名函数是许多编程语言都支持的概念，有函数体，没有函数名。1958年，lisp首先采用匿名函数，匿名函数最常用的是作为回调函数的值。正因为有这样的需求，c++引入了lambda 函数，你可以在你的源码中内联一个lambda函数，这就使得创建快速的，一次性的函数变得简单了。例如，你可以把lambda函数可在参数中传递给std::sort函数
#include <algorithm>
#include <cmath>
void abssort(float* x, unsigned N) {
    std::sort(x, x + N,
        // Lambda expression begins
        [](float a, float b) {
            return std::abs(a) < std::abs(b);
        });
}
你可能会问，使用函数对象不是也可以吗？是的，函数对象当然没问题，自己写的回调函数，你可以传个函数指针也没有问题。他们有优点也有缺点。函数对象能维护状态，但语法开销大，而函数指针语法开销小，却没法保存范围内的状态。如果你觉得鱼和熊掌不可兼得，那你可错了。lambda函数结合了两者的优点，让你写出优雅简洁的代码。
**基本lambda语法**
基本形式如下：
[capture](parameters)->return-type {body}
[]叫做捕获说明符，表示一个lambda表达式的开始。接下来是参数列表，即这个匿名的lambda函数的参数，->return-type表示返回类型，如果没有返回类型，则可以省略这部分。想知道为什么返回类型可以这么表示，这涉及到c++11的另一特性，参见[自动类型推导](http://blog.csdn.net/booirror/article/details/25751577)，最后就是函数体部分了。
我们可以这样输出"hello,world"
auto func = [] () { cout << "hello,world"; };
func(); // now call the function
**变量捕获与lambda闭包实现**
string name;
cin >> name;
[&](){cout << name;}();
lambda函数能够捕获lambda函数外的具有自动存储时期的变量。函数体与这些变量的集合合起来叫闭包。
- [] 不截取任何变量
- [&} 截取外部作用域中所有变量，并作为引用在函数体中使用
- [=] 截取外部作用域中所有变量，并拷贝一份在函数体中使用
- [=, &foo] 截取外部作用域中所有变量，并拷贝一份在函数体中使用，但是对foo变量使用引用
- [bar] 截取bar变量并且拷贝一份在函数体重使用，同时不截取其他变量
- [x, &y] x按值传递，y按引用传递
- [this] 截取当前类中的this指针。如果已经使用了&或者=就默认添加此选项。
看到这，不禁要问，这魔法般的变量捕获是怎么实现的呢？原来，lambda是通过创建个小类来实现的。这个类重载了操作符()，一个lambda函数是该类的一个实例。当该类被构造时，周围的变量就传递给构造函数并以成员变量保存起来。看起来跟函数对象很相似。
最后，lambda函数的类型是什么呢，答案是std:function。
