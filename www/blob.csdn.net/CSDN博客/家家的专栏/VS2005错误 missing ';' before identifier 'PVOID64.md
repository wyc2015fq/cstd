# VS2005错误 missing ';' before identifier 'PVOID64 - 家家的专栏 - CSDN博客





2010年07月30日 11:26:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2518标签：[buffer																[c																[微软](https://so.csdn.net/so/search/s.do?q=微软&t=blog)




**问题:**

    1>d:/program files/microsoft visual studio 8/vc/platformsdk/include/winnt.h(223) : error C2146: syntax error : missing ';' before identifier 'PVOID64'
1>d:/program files/microsoft visual studio 8/vc/platformsdk/include/winnt.h(223) : error C4430: missing type specifier - int assumed. Note: C++ does not support default-int
1>d:/program files/microsoft visual studio 8/vc/platformsdk/include/winnt.h(5941) : error C2146: syntax error : missing ';' before identifier 'Buffer'
1>d:/program files/microsoft visual studio 8/vc/platformsdk/include/winnt.h(5941) : error C4430: missing type specifier - int assumed. Note: C++ does not support default-int
1>d:/program files/microsoft visual studio 8/vc/platformsdk/include/winnt.h(5941) : error C4430: missing type specifier - int assumed. Note: C++ does not support default-in

解决方法:

     在winnt.h文件中修改：

添加：#define POINTER_64 __ptr64

在typedef void *PVOID;   typedef void * POINTER_64 PVOID64;这两句之前。

    原因opencv的定义与微软的有差异，而引起的。](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)




