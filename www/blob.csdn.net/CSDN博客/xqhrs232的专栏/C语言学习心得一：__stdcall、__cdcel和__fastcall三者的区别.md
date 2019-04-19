# C语言学习心得一：__stdcall、__cdcel和__fastcall三者的区别 - xqhrs232的专栏 - CSDN博客
2017年04月21日 10:31:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：388
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.sina.com.cn/s/blog_701526f40100lcy6.html](http://blog.sina.com.cn/s/blog_701526f40100lcy6.html)
相关文章
1、__stdcall 和 __cdecl 的区别浅析----[http://www.jb51.net/article/34554.htm](http://www.jb51.net/article/34554.htm)
__stdcall、__cdecl和__fastcall是三种函数调用协议，函数调用协议会影响函数参数的入栈方式、栈内数据的清除方式、编译器函数名的修饰规则等。如下图所示，可以在IDE环境中设定所有函数默认的调用协议，还可以在函数定义时单独设定本函数的调用协议。
![image](https://i7vgvq.bay.livefilestore.com/y1md5APrqt38yoiWljOGFLHolet8y4lgFRrgj5ioVe4tMawvihzWytXK9XDnuNgnFxUFdUb41nqkNRJhJ6u3etrLDZSPGy0ScYAlNhKth_OvUJVlbmxf9sec8aQ8DB_aIFUcDFctZxrMnDQy1Tx_7r_Lw/image_thumb%5B4%5D.png?download&psid=1)
- 
调用协议常用场合
- - 
__stdcall：Windows API默认的函数调用协议。
- 
__cdecl：C/C++默认的函数调用协议。
- 
__fastcall：适用于对性能要求较高的场合。
- 
函数参数入栈方式
- - 
__stdcall：函数参数由右向左入栈。
- 
__cdecl：函数参数由右向左入栈。
- 
__fastcall：从左开始不大于4字节的参数放入CPU的ECX和EDX寄存器，其余参数从右向左入栈。
- 
问题一：__fastcall在寄存器中放入不大于4字节的参数，故性能较高，适用于需要高性能的场合。
- 
栈内数据清除方式
- - 
__stdcall：函数调用结束后由被调用函数清除栈内数据。
- 
__cdecl：函数调用结束后由函数调用者清除栈内数据。
- 
__fastcall：函数调用结束后由被调用函数清除栈内数据。
- 
问题一：不同编译器设定的栈结构不尽相同，跨开发平台时由函数调用者清除栈内数据不可行。
- 
问题二：某些函数的参数是可变的，如printf函数，这样的函数只能由函数调用者清除栈内数据。
- 
问题三：由调用者清除栈内数据时，每次调用都包含清除栈内数据的代码，故可执行文件较大。
- 
C语言编译器函数名称修饰规则
- - 
__stdcall：编译后，函数名被修饰为“_functionname@number”。
- 
__cdecl：编译后，函数名被修饰为“_functionname”。
- 
__fastcall：编译后，函数名给修饰为“@functionname@nmuber”。
- 
注：“functionname”为函数名，“number”为参数字节数。
- 
注：函数实现和函数定义时如果使用了不同的函数调用协议，则无法实现函数调用。
- 
C++语言编译器函数名称修饰规则
- - 
__stdcall：编译后，函数名被修饰为“?functionname@@YG******@Z”。
- 
__cdecl：编译后，函数名被修饰为“?functionname@@YA******@Z”。
- 
__fastcall：编译后，函数名被修饰为“?functionname@@YI******@Z”。
- 
注：“******”为函数返回值类型和参数类型表。
- 
注：函数实现和函数定义时如果使用了不同的函数调用协议，则无法实现函数调用。
- 
C语言和C++语言间如果不进行特殊处理，也无法实现函数的互相调用。
