# Python 调用C++ - weixin_33985507的博客 - CSDN博客
2019年01月14日 08:43:05[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
1、C++代码提供Python需要的接口：
```
1 #include "stdafx.h"
 2 #include <boost/python.hpp>
 3 #include <string>
 4 using namespace std;
 5 
 6 string greet()
 7 {
 8     for(int i=0;i<5;++i)
 9     {
10         return "hello";
11     }    
12 }
13 
14 BOOST_PYTHON_MODULE(hello)
15 {
16     boost::python::def("greet", greet);
17 }
```
**注意事项：导出模块名为hello，后缀名为pyd**
2、Python调用接口
```
1 Python 3.2.2 (default, Sep  4 2011, 09:51:08) [MSC v.1500 32 bit (Intel)] on win32
2 Type "copyright", "credits" or "license()" for more information.
3 >>> import hello
4 >>> hello.greet()
5 'hello'
6 >>>
```
