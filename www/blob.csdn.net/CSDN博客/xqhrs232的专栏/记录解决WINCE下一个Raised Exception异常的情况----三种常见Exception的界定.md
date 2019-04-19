# 记录解决WINCE下一个Raised Exception异常的情况----三种常见Exception的界定 - xqhrs232的专栏 - CSDN博客
2013年03月13日 11:03:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2354
先对WINCE下各种常见Exception进行必要的认识
1.WINCE系统报错总结----[http://blog.chinaunix.net/uid-22597935-id-3290516.html](http://blog.chinaunix.net/uid-22597935-id-3290516.html)
1> Exception 'Raised Exception' (-1):   数组越界
2> Exception 'Data Abort' (4):          栈出问题了/空指针/访问的数据块无效了(如内存已经回收无效了)
3> Exception 'Prefetch Abort' (3):      空指针
我的情况是使用std::wstring 类型的字符串变量，在对这个字符串变量进行用substr( )函数去截取的时候，起始的offset值没有初始化（用的是一个临时变量，在有些情况下这个临时变量使用时没被赋初始值），导致截取的时候报Raised Exception异常！！！找了个老半天！！！
2013--3--14 今天是遇到std::vector<std::wstring>向量数组访问越界，超出了数组的范围！
