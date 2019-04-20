# DEFINE_ENUM_FLAG_OPERATORS(x)的作用 - sandro_zhang的专栏 - CSDN博客
2011年10月26日 13:40:52[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：1231
#define DEFINE_ENUM_FLAG_OPERATORS(x)
今天编译XPS相关的Sample，出现以下错误：
```cpp
1>c:\program files\microsoft sdks\windows\v7.1\include\msopc.h(440) : error C4430: missing type specifier - int assumed. Note: C++ does not support default-int
1>c:\program files\microsoft sdks\windows\v7.1\include\msopc.h(447) : error C4430: missing type specifier - int assumed. Note: C++ does not support default-int
1>c:\program files\microsoft sdks\windows\v7.1\include\msopc.h(447) : error C2733: second C linkage of overloaded function 'DEFINE_ENUM_FLAG_OPERATORS' not allowed
1>        c:\program files\microsoft sdks\windows\v7.1\include\msopc.h(447) : see declaration of 'DEFINE_ENUM_FLAG_OPERATORS'
1>Build log was saved at "file://c:\Program Files\Microsoft SDKs\Windows\v7.1\Samples\xps\XpsOMPrint\Debug\BuildLog.htm"
1>XpsOMPrint - 3 error(s), 0 warning(s)
```
经查阅，需在文档的最前面添加如下：
```cpp
#define DEFINE_ENUM_FLAG_OPERATORS(x)
#include <stdio.h>
#include <comdef.h>
#include <windows.h>
```
