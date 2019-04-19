# Modern C++(五)nullptr关键字 - xcyl的口袋 - CSDN博客
2016年10月30日 02:05:30[xcyl](https://me.csdn.net/cai6811376)阅读数：245
个人分类：[C++](https://blog.csdn.net/cai6811376/article/category/6318203)
我们都知道在C++11标准之前使用NULL对指针进行置空。但这是不严谨的使用方式。
因为
在C语言中
`#define NULL ((void*)0)`
并且支持
`#define NULL 0`
但在C++中
```
#ifndef NULL
    #ifdef __cplusplus
        #define NULL 0
    #else
        #define NULL ((void *)0)
    #endif
#endif
```
这是因为C++支持重载函数，若NULL可以定义为((void*)0)和0，则会在重载函数时造成二义性问题
```
void func(int a);
void func(char* b);
func(NULL);    //ambiguity error
```
为此C++11中添加了nullptr关键字，它指代的是空指针类型，我们使用nullptr将改善我们上例的函数调用方式
```cpp
void func(int a);
void func(char* b);
func(0);        //func(int a);
func(nullptr)   //func(char* b);
```
对于空指针的判断我们可以
```cpp
if (ptr == nullptr)
if (ptr == 0)
if (ptr)
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.easydarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
