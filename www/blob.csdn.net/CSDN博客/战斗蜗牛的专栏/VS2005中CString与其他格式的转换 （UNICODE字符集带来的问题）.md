# VS2005中CString与其他格式的转换 （UNICODE字符集带来的问题） - 战斗蜗牛的专栏 - CSDN博客





2011年06月11日 18:57:00[vbskj](https://me.csdn.net/vbskj)阅读数：779标签：[mfc](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[C/C++/Visual C++](https://blog.csdn.net/vbskj/article/category/630758)









使用MFC的转换宏，很简单的

T2CA()    转成char*

T2CW()    转成wchar_t*

...       类似的转换很多，可以自己查资料

A2T

A2CW

A2W

W2CA

W2A  

...


- C/C++ code



#include "afxpriv.h"


...

USES_CONVERSION;    // 这一句一定要加
printf("这是转化后的字符串:%s", T2CA(strYourstring));

 CString  s("kingfish");   

    USES_CONVERSION;
char*p =T2A(s);
...





