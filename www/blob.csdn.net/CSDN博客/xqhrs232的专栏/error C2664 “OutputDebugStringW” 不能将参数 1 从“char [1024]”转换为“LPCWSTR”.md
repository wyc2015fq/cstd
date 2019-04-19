# error C2664: “OutputDebugStringW”: 不能将参数 1 从“char [1024]”转换为“LPCWSTR” - xqhrs232的专栏 - CSDN博客
2013年03月27日 22:39:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3259
原文地址::[http://www.verydemo.com/demo_c230_i4355.html](http://www.verydemo.com/demo_c230_i4355.html)
错误信息：error C2664: 'OutputDebugStringW' : cannot convert parameter 1 from 'char [100]' to 'LPCWSTR'
1>        Types pointed to are unrelated; conversion requires reinterpret_cast, C-style cast or function-style cast
相关代码：
  if(i>1){ 
   sprintf(msg,"1+...+%d=%d\n",i,sum); 
   OutputDebugString(msg); 
  }
找了一天资料，原因锁定在字符的转换问题，搞版本的vs中，OutputDebugString要求一个wchar_t
而不是char，而sprintf则需要char参数，那我们是不是一定要通过字符转换解决问题呢？
答案就是 OutputDebugStringA（）
原因：
默认是Unicode方式，OutputDebugString会变成OutputDebugStringW。如果想用ANSI版本的，直接写OutputDebugStringA，或者设置工程属性，使用MBCS的编码集。
