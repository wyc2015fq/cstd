# Warning:关于_CRT_SECURE_NO_WARNINGS - wishchinYang的专栏 - CSDN博客
2013年08月08日 15:26:49[wishchin](https://me.csdn.net/wishchin)阅读数：1025
Warning 1 warning C4996: 'fopen': This function or variable may be unsafe. Consider using fopen_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
原文链接：[http://blog.csdn.net/xuleilx/article/details/7281499](http://blog.csdn.net/xuleilx/article/details/7281499)
在编程过程中难免会用到一些过时，或者曾经不安全的函数，如上，这是编译器会出现warning提示用某某新函数，如果不想使用新的函数可以使用一下方法：
1. 使用VS提供的 编译器选择性提供warning功能 link: http://msdn.microsoft.com/en-
     us/library/2c8f766e.aspx 例：上面的错误使用 #pragma warning( disable : 4996) 即可消除。
2. 使用提示中的_CRT_SECURE_NO_WARNINGS，以下是使用方法：
        a. Project properties->Configuration Properties->C/C++->Preprocessor->Preprocessor
            Definitions
        b. 点击按钮 (...)
        c. 输入：_CRT_SECURE_NO_WARNINGS。 注：使用 "\n" 相隔
3.#define _CRT_SECURE_NO_WARNINGS
   #include <stdio.h>
[](http://blog.csdn.net/xuleilx/article/details/7281499)
