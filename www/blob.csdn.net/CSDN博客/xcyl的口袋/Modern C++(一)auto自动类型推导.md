# Modern C++(一)auto自动类型推导 - xcyl的口袋 - CSDN博客
2016年10月28日 23:34:33[xcyl](https://me.csdn.net/cai6811376)阅读数：352
从今天开始，我会将自己一直以来使用C++11的新特性记录下来。作为自己职业生涯的记录以及经验分享。
第一篇我给了auto关键字，这是因为实际使用中，确实auto给我们开发效率提升了很多，确实使用的也算是最广。在C++11之前已经有了auto，它指代的是“一个自动存储变量的关键字，也就是申明一块临时的变量内存”，比如：auto int i = 5; 我们很少使用，因为非static的变量本身就是自动存储变量。
在C++11中，auto关键字被定义为：自动变量类型推导。
我们来看看它的用法。
```
//后面注释的类型为变量类型
auto a = 5;                     //int
auto b = 0.5;                   //double
int c = 10;
auto d = c;                     //int
auto e = b;                     //double
auto f;                         //error
auto g = new int(5);            //int*
auto* h = &a;                   //int*
```
我们看到auto可以自动推导出变量的类型，并且当使用auto定义变量时需要进行初始化，否则auto将无从推导类型。
我们再看几个示例
```
//后面注释的类型为变量类型
int a = 5;
auto* b = &a;                   //int*
auto& c = a;                    //int&
const auto d = a;               //const int
auto e = d;                     //int
const auto& f = a;              //const int&
auto& g = f;                    //const int&
const auto* h = &a;             //const int*
auto* i = h;                    //const int*
```
由示例我们可以得出以下结论：
- 
当声明的变量不为指针或者引用时（e），推导将忽略const限定；
- 
当声明变量为指针或者引用时（g，i），推导将保留const限定；
- 
volatile限定同const
auto的典型应用，在我们需要使用迭代器遍历C++容器时，通常的写法非常繁琐。
```cpp
std::map<int, string> testMap;
std::map<int, string>::iterator it = testMap.begin();
for (; it != testMap.end(); ++it)
{
    cout << it->first << "-------" << it->second << endl;
}
```
在定义it的时候需要写出具体的类型，而使用auto的写法是这样
```cpp
std::map<int, string> testMap;
for (auto it = testMap.begin(); it != testMap.end(); ++it)
{
    cout << it->first << "-------" << it->second << endl;
}
```
是不是感觉清爽很多？当然遍历容器还有更简洁的写法，以及auto的更多便利用法我们后面再说~
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.easydarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
