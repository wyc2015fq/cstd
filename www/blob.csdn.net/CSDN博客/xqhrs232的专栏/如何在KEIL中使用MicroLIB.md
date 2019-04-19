# 如何在KEIL中使用MicroLIB - xqhrs232的专栏 - CSDN博客
2015年11月27日 17:26:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：428
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://blog.csdn.net/woshi_ziyu/article/details/12145473](http://blog.csdn.net/woshi_ziyu/article/details/12145473)
3.5. microlib 中缺少的 ISO C 特性
本节提供了 microlib 不支持的主要 ISO C90 特性的列表。
宽字符和多字节支持
microlib 不支持所有处理宽字符或多字节字符串的函数。 如果使用这些函数，则会产生链接器错误。 例如，mbtowc()、wctomb()、mbstowcs() 和wcstombs()。 microlib 不支持在标准附录 1 中定义的所有函数。
操作系统交互
microlib 不支持与操作系统交互的所有函数。 例如，abort()、exit()、atexit()、clock()、time()、system() 和getenv()。
文件 I/O
与文件指针交互的所有 stdio 函数将返回错误（如果已实现）。 唯一的例外情况是以下三个标准流：stdin、stdout和stderr。
可配置的区域设置
缺省 C 区域设置是唯一可用的区域设置。
信号
虽然提供了 signal() 和 raise() 函数，但microlib 不会生成信号。唯一的例外情况是程序显式地调用raise()。
浮点支持
浮点支持不符合 IEEE 754 标准。
产生不可预测的输出的运算是指：
涉及 NaN、无穷大或非正规数
依照正确的 IEEE 754 规则，并非通过不精确结果产生 IEEE 异常。 但是，microlib不会产生IEEE 异常，而是返回不可预测的结果。
另外，microlib 不会将零的符号视为有效位，并且会产生不可预测的输出。
与位置无关且线程安全的代码
microlib 没有可重入变体。 microlib 不提供互斥锁来防止非线程安全的代码。 microlib 的使用与 FPIC 或 RWPI 编译模式不兼容，但可以将 ROPI 代码与 microlib 进行链接，生成的二进制文件总体上与 ROPI 不兼容。
//============================================================================================
备注::
1>microlib 不支持与操作系统交互的所有函数。 例如，abort()、exit()、atexit()、clock()、time()、system() 和getenv()。
