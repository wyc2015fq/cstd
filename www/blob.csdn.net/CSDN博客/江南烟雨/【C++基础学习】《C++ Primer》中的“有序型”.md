# 【C++基础学习】《C++ Primer》中的“有序型” - 江南烟雨 - CSDN博客
2011年11月10日 11:30:15[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：1562标签：[c++																[integer																[types																[class																[ibm																[object](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=ibm&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=types&t=blog)](https://so.csdn.net/so/search/s.do?q=integer&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C/C++](https://blog.csdn.net/xiajun07061225/article/category/723127)
本人在看《C++ Primer（第三版）》中文版看到类一章时，遇到了“有序型”这个概念。（Page527）
原文摘录如下：
作为特例，有序型的const静态数据成员可以在类体中用一常量值初始化。例如，如果决定用一个字符数组而不是string来存储账户的姓名，那么我们可以用int型的const数据成员指定数组的长度。例如：
```cpp
//头文件
class Account{
	//...
private:
	static const int namesize = 16;
	static const char name[namesize];
};
//文本文件
const int Account::namesize;//必须的成员定义
const char Account::name[namesize] = "Savings Account";
```
因为name是一个数组（不是有序类型），所以它不能在类体内被初始化。任何试图这么做的行为都将导致编译错误。例如：
```cpp
//头文件
class Account{
	//...
private:
	static const int namesize = 16;//OK:有序类型
	static const char name[namesize] = "Savings Account";//错误
};
```
那么这里所说的“有序类型”到底是什么呢？
我查了一下英文原版，它原版上是：integral type。
又在网上查了查，发现这个地方应该指的是“整值类型”。指char、short、int、long和它们的unsigned版本，不含float和double。
Google一下integral type可以搜索到一些资料。
这是从IBM网站上的内容：
Integral types
Integer types fall into the following categories:
Signed integer types:
signed char
short int
int
long int
long long int
Unsigned integer types:
unsigned char
unsigned short int
unsigned int
unsigned long int
unsigned long long int
The unsigned prefix indicates that the object is a nonnegative integer. Each unsigned type provides the same size storage as its signed equivalent. For example, int reserves the same storage as unsigned int. Because a signed type reserves a sign bit, an unsigned
 type can hold a larger positive integer value than the equivalent signed type.
一些翻译过来的书籍有些地方翻译得不好还真是伤不起。看来最好还是阅读英文原版的书籍~~~
