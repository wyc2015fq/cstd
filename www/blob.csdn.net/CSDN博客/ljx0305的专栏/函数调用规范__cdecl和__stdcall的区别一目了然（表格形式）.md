# 函数调用规范__cdecl和__stdcall的区别一目了然（表格形式）  - ljx0305的专栏 - CSDN博客
2011年03月18日 10:20:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：659标签：[underscore																[microsoft																[assembly																[winapi																[character																[dll](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=character&t=blog)](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=assembly&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=underscore&t=blog)
个人分类：[C++																[C](https://blog.csdn.net/ljx0305/article/category/394717)](https://blog.csdn.net/ljx0305/article/category/380566)
## [函数调用规范__cdecl和__stdcall的区别一目了然（表格形式）](http://blog.bcchinese.net/happyjet/archive/2005/05/24/22545.aspx)
Posted on Tuesday, May 24, 2005 3:23 PM [#C & C++](http://blog.bcchinese.net/happyjet/category/465.aspx)
|**__cdecl**|**__stdcall**|
|----|----|
|C和C++程序的缺省调用规范|为了使用这种调用规范，需要你明确的加上__stdcall（或WINAPI）文字。即*return-type***__stdcall***function-name*[(*argument-list*)]|
|在**被**调用函数(Callee)返回**后**，由调用方(Caller)调整堆栈。1. 调用方的函数调用2. 被调用函数的执行3. 被调用函数的结果返回4. 调用方清除调整堆栈|在**被**调用函数(Callee)返回**前**，由**被**调用函数(Callee)调整堆栈。图示：1. 调用方的函数调用2. 被调用函数的执行3. 被调用函数清除调整堆栈4. 被调用函数的结果返回|
|因为每个调用的地方都需要生成一段调整堆栈的代码，所以最后生成的文件较大。|因为调整堆栈的代码只存在在一个地方（被调用函数的代码内），所以最后生成的文件较小。|
|函数的参数个数可变（就像printf函数一样），因为只有调用者才知道它传给被调用函数几个参数，才能在调用结束时适当地调整堆栈。|函数的参数个数不能是可变的。|
|对于定义在C程序文件中的输出函数，函数名会保持原样，不会被修饰。对于定义在C++程序文件中的输出函数，函数名会被修饰， MSDN说Underscore character (_) is prefixed to names. 我实际测试（VC4和VC6）下来发现好像不是那么简单。**可通过在前面加上****extern ****“C”****以去除函数名修饰。也可通过****.def****文件去除函数名修饰。**|不论是C程序文件中的输出函数还是C++程序文件中的输出函数，函数名都会被修饰。对于定义在C程序文件中的输出函数，An underscore (_) is prefixed to the name. The name is followed by the at sign (@) followed by the number of bytes (in decimal) in the argument list.对于定义在C++程序文件中的输出函数，好像更复杂，和__cdecl的情况类似。**好像只能通过****.def****文件去除函数名修饰。**|
|_beginthread需要__cdecl的线程函数地址|_beginthreadex和CreateThread需要__stdcall的线程函数地址|
|两者的参数传递顺序都是从右向左。为了让VB可以调用，需要用__stdcall调用规范来定义C/C++函数。请参看Microsoft KB153586 文章：[How To Call C Functions That Use the _cdecl Calling Convention](http://support.microsoft.com/default.aspx?scid=kb;en-us;Q153586)。当你LoadLibrary一个DLL文件后， 把GetProcAddress取得的函数地址传给上面三个线程生成函数时，请务必确认实际定义在DLL文件的输出函数符合调用规范要求。否则，编译成Release版后运行，可能会破坏堆栈，程序行为不可预测。VC中的相关编译开关：/Gd /Gr /Gz。另外，VC6中新增加的 /GZ 编译开关可以帮你检查堆栈问题。我也是初学者，若有不对的地方、可以补充的地方，请指教。谢谢。| |
（补充）汇编语言视点的比较文章：[Intel x86 Function-call Conventions - Assembly View](http://www.unixwiz.net/techtips/win32-callconv-asm.html)
