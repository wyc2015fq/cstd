
# stringstream 的.str()正确用法和清空操作 - Augusdi的专栏 - CSDN博客


2015年10月26日 23:44:31[Augusdi](https://me.csdn.net/Augusdi)阅读数：1363


转载自：[http://dewei.iteye.com/blog/1940786](http://dewei.iteye.com/blog/1940786)
streamstring在调用str()时，会返回临时的string对象。而因为是临时的对象，所以它在整个表达式结束后将会被析构。
如果需要进一步操作string对象，先把其值赋给一个string变量后再操作。
```python
stringstream ss("012345678901234567890123456789012345678901234567890123456789");  
//错误用法  
const char* cstr2 = ss.str().c_str();  
//正确用法  
const string& str2 = ss.str();  
const char* cstr2 = str2.c_str();
```

**使用stringstream时的清空操作**
在C++中可以使用stringstream来很方便的进行类型转换，字符串串接，不过注意重复使用同一个stringstream对象时要 先继续清空，而清空很容易想到是clear方法，而在stringstream中这个方法实际上是清空stringstream的状态（比如出错等），真 正清空内容需要使用.str("")方法。

在C++中可以使用stringstream来很方便的进行类型转换，字符串串接，不过注意重复使用同一个stringstream对象时要先继续清空，而清空很容易想到是clear方法，而在stringstream中这个方法实际上是清空stringstream的状态（比如出错等），真正清空内容需要使用.str(“”)方法。
﻿﻿

