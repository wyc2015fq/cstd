# 使用OutputDebugString帮助调试 - maopig的专栏 - CSDN博客
2011年08月29日 23:33:35[maopig](https://me.csdn.net/maopig)阅读数：946
**使用OutputDebugString帮助调试**
前面我已经介绍了使用TRACE来帮助我们调试，但使用TRACE有一个限制，只能在将程序DEBUG编译状态下才能使用，下面我们介绍OutputDebugString函数，通过它，可以在在DEBUG或RELEASE情况也可以输出调试日志，从而对那些要求编译为RELEASE的程序可以方便的调试。
OutputDebugString的函数原型：
void **OutputDebugString(**
**LPCTSTR***[lpOutputString]()*
**);**
参数就是你需要输出的字符串了，该字符串以\0结尾。如果你需要显示数字等，你需要事先格式化字符串。注意WINDOWS CE仅仅支持UNICODE版本，所以你提供的字符串应该转换为UNICODE。使用_tstrcat等来格式化字符串。
如果你的应用程序有一个DEBUGER，那么该函数输出到DEBUGER，否则就输出到系统的DEBUGER，如果两个都没有，该函数什么也不做。
注意，输出到系统DEBUGER的，你可以使用DEBUGVIEW工具查看日志，这样你在程序运行的时候也可以看到日志了。很多程序都使用这种方式来写日志，包括MICROSOFT本身也使用这种方法。
