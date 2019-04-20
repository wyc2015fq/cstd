# TCHAR 认识整理 - sandro_zhang的专栏 - CSDN博客
2011年09月16日 17:15:08[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：384
**定义**
TCHAR是通过define定义的字符串宏
**使用原理**
　　因为C++支持两种字符串，即常规的ANSI编码（使用""包裹）和Unicode编码（使用L""包裹），这样对应的就有了两套字符串字符串处理函数，比如：strlen和wcslen，分别用于处理两种字符串
　　微软将这两套字符集及其操作进行了统一，通过条件编译（通过_UNICODE和UNICODE宏）控制实际使用的字符集，这样就有了_T("")这样的字符串，对应的就有了_tcslen这样的函数
　　为了存储这样的通用字符，就有了TCHAR：
　　当没有定义_UNICODE宏时，TCHAR = char，_tcslen = strlen
　　当定义了_UNICODE宏时，TCHAR = wchar_t ， _tcslen = wcslen
       （_tcscmp）
　　当我们定义了UNICODE宏，就相当于告诉了编译器：我准备采用UNICODE版本。这个时候，TCHAR就会摇身一变，变成了wchar_t。而未定义UNICODE宏时，TCHAR摇身一变，变成了unsigned char 。这样就可以很好的切换宽窄字符集。
　　tchar可用于双字节字符串，使程序可以用于中日韩等国 语言文字处理、显示。使编程方法简化。
另：
tchar可用于双字节字符串，使程序可以用于中日韩等国 语言文字处理、显示。使编程方法简化。
感谢：百度百科
