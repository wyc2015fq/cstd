# Modern C++(四)左值与右值 - xcyl的口袋 - CSDN博客
2016年10月30日 01:27:14[xcyl](https://me.csdn.net/cai6811376)阅读数：365
在之前的文章里，我们提到了lvalue、rvalue、xvalue。在C++11之前，左值指的是表达式左边的值，右值指的是表达式右边的值。比如
```
//a为左值，(b + c)为右值
a = b + c;
```
这是一个很简单的辨别方法。但C++中有个辨别左值右值的更准确的方法：
> 
可以对其取地址的值为左值，否则为右值
在上面的示例中我们可以使用`auto d = &a;`，但是你无法使用`auto e = &(b + c);`
C++11中将表达式进行划分
![这里写图片描述](https://img-blog.csdn.net/20161030013352767)
新标准中，将右值细分，就是prvalue纯右值和xvalue将亡值。并且，左值与将亡值合称为泛左值(generalized lvalue)，将亡值与纯右值合称为右值。
纯右值与C++11以前标准中的右值是同一个概念，我们来看看哪些类型的值为纯右值
```
//字面值常量，但是字符串字面值为左值
//5、5.5为纯右值，a、b为左值
int a = 5;
float b = 5.5;
//getIntValue()函数的返回的临时变量为纯右值，c为左值
int getIntValue();
auto c = getIntValue();
//(1 + 2)为纯右值，d为左值
auto d = 1 + 2;
//等等
```
C++11标准新加入的xvalue将亡值是什么呢？
- 
将要被移动的对象
- 
返回类型为T&&的函数返回值
- 
std::move的返回值
- 
可转换为T&&的类型转换函数的返回值
C++引入将亡值是为了使用右值引用实现移动语义与完美转发。
关于移动、T&&、std::move我们会在以后的文章中介绍，具体的示例会在以后的文章中讲解。
最后
C++11规定
> 
在C++程序中，所有的值必须属于左值、纯右值、将亡值中的一种
参考：[http://en.cppreference.com/w/cpp/language/value_category](http://en.cppreference.com/w/cpp/language/value_category)
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.easydarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
