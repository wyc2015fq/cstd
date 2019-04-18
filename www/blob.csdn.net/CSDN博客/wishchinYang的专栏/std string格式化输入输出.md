# std::string格式化输入输出 - wishchinYang的专栏 - CSDN博客
2016年03月08日 09:59:16[wishchin](https://me.csdn.net/wishchin)阅读数：14277
在C语言中：
       C函数有sprintf函数, 比较方便, 但是需要知道所需要的内存空间是多少.
在C++的框架MFC中：
        在MFC中CString 有Format函数来格式化字符串. 很方便.
难过的是：
       std::string没有格式化输入输出的Format函数.
只能通过 
std::strstream进行转换
```cpp
#include <sstream>
std::stringstream ss;
ss << 1234<< "wishchin" << 5678;
std::string  str = ss.str();
```
多写个一行，也算比较简单的.
