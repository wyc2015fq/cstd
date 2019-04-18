# C++11 新特性之 lambda - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [usher2007](http://www.jobbole.com/members/usher2007) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
前两篇在这里：
C++11新特性之新类型与初始化:[http://blog.jobbole.com/102728/](http://blog.jobbole.com/102728/)
C++11新特性之类型推断与类型获取: [http://blog.jobbole.com/104559/](http://blog.jobbole.com/104559/)
这是C++11新特性介绍的第三部分，涉及到C++11这次更新中较为重要的lambda表达式。
不想看toy code的读者可以直接拉到文章最后看这部分的总结。
# lambda 简介
熟悉Python的程序员应该对lambda不陌生。简单来说，lambda就是一个匿名的可调用代码块。在C++11新标准中，lambda具有如下格式：


```
[capture list] (parameter list) -> return type { function body }
```
可以看到，他有四个组成部分：
- capture list： 捕获列表
- parameter list： 参数列表
- return type： 返回类型
- function body： 执行代码
其中，参数列表和返回类型可以忽略。
下面，具体看几个简单的例子：


```
auto f1 = [] { return 1; };
auto f2 = [] () { return 2; };
cout<<f1()<<'\t'<<f2()<<endl;
```
# 捕获列表
lambda中的捕获列表既可以捕获值，也可以捕获引用。
捕获值：


```
int test_data[] = {1, 5, 9, 7, 3, 19, 13, 17};
int border = 8;
auto f3 = [border](const int &i){ if(i > border) cout<<i<<'\t'; };
for_each(begin(test_data), end(test_data), f3);
cout<<endl;
```
捕获引用：


```
auto f4 = [&border](const int &i){ if(i > border) cout<<i<<'\t'; };
border = 6;
for_each(begin(test_data), end(test_data), f4);
cout<<endl;
```
通过输出可以看出，lambda中起作用的border是修改后的6，证实了捕获的确是是引用。
需要注意的是，在捕获引用时，需要保证当lambda被调用时，此引用仍然有效。
捕获列表还可以采用隐式捕获的方式，即让编译器通过lambda的执行代码来判断需要捕获哪些局部变量。
隐式捕获可以捕获值、引用或者两者混合：


```
char space = ' ';
auto f5 = [=](const int &i){ if(i > border) cout<<i<<'\t'; };
auto f6 = [&](const int &i){ if(i > border) cout<<i<<'\t'; };
auto f7 = [&, space](const int &i){ if(i > border) cout<<i<<space; };
border = 0;
for_each(begin(test_data), end(test_data), f5);
cout<<endl;
for_each(begin(test_data), end(test_data), f6);
cout<<endl;
for_each(begin(test_data), end(test_data), f7);
cout<<endl;
```
这里的f7使用的混合形式，可以读作“除了space捕获值之外，其他变量均捕获引用”。
# 可变 lambda
当lambda需要在其中修改被值捕获的变量的值时，需要给lambda加上mutable关键字。否则会有编译错误。


```
auto f8 = [&, space](const int &i) mutable { if(i > border) {cout<<i<<space; space='\t';} };
for_each(begin(test_data), end(test_data), f8);
cout<<endl;
cout<<1<<space<<2<<endl;
```
从输出中可以看出，space在lambda f8中的值，在第一次调用之后，就被变成了制表符Tab;但是在lambda之外，space仍然是空格。
# 返回类型
lambda的返回类型采用尾置返回类型的方式。一般的：
- 
> 
lambda如果只包含return语句，则编译器可以推断其返回类型，此时可以不显示指定返回类型；
- 
> 
否则，编译器假定lambda返回void，而返回void的函数不可以反悔任何具体值，这在大多数情况下是个矛盾，因此需要显示指定返回类型。
但是，经过实际测试，目前的g++编译器更聪明了：对于第2点，目前只要编译器可以从lambda函数体中推断出函数的返回类型，就不需要显式指定返回类型，例如：


```
auto f9 = [](const int i){if(i % 3) return i * 3; else return i;};
transform(begin(test_data), end(test_data), begin(test_data), f9);
border = 0;
for_each(begin(test_data), end(test_data), f6);
cout<<endl;
```
lambda代码块中有多个return语句，并且还有if/else语句，但是编译器可以根据return语句推断出，其返回值应该是一个int类型，所以可以省略尾置返回类型。
但是，像下面这种形式，由于编译器在推断返回类型时发现了不一致，所以必须显式的指定返回类型：


```
auto f10 = [](const int i) -> double
{if(i % 5) return i * 5.0; else return i;};
transform(begin(test_data), end(test_data), begin(test_data), f10);
for_each(begin(test_data), end(test_data), f6);
cout<<endl;
```
# 总结
- lambda表达式形式： [capture list] (parameter list) -> return type { function body }，其中parameter list和return type可以省略。
- 捕获列表可以捕获值[val]，也可以捕获引用[&ref]。
- 捕获列表还可以隐式捕获局部变量，同样有捕获值[=]和捕获引用[&]两种方式，初次之外还可以混合捕获[&, val]或者[=, &ref]。
- 当lambda需要修改捕获的值时，需要加上mutable关键字。
- 当lambda无法自动推断出返回值类型时，需要通过尾置返回类型的方式显示指定。
完整代码详见[lambda_expr.cpp](https://github.com/yubo1911/saber/blob/master/learncpp11/lambda_expr.cpp)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/08/a41e08fcaca939c581c931c115da0a8d.png)![](http://jbcdn2.b0.upaiyun.com/2016/08/c07df7ab8dad3a49f6ce602eb7d62949.jpg)
