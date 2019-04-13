
# C,C++宏中\#与\#\#的讲解 - Augusdi的专栏 - CSDN博客


2015年04月25日 13:00:34[Augusdi](https://me.csdn.net/Augusdi)阅读数：1109个人分类：[VC++程序设计																](https://blog.csdn.net/Augusdi/article/category/519968)


C,C++宏中\#与\#\#的讲解
文中__FILE__与示例1可以参见《使用ANSI C and Microsoft C++中常用的预定义宏》
宏中的\#的功能是将其后面的宏参数进行字符串化操作（Stringizing operator），简单说就是在它引用的宏变量的左右各加上一个双引号。
如定义好\#defineSTRING(x)\#x之后，下面二条语句就等价。
char *pChar="hello";
char *pChar=STRING(hello);
还有一个\#@是加单引号（Charizing Operator）
\#definemakechar(x)\#@x
charch=makechar(b);与charch='b';等价。


但有小问题要注意，宏中遇到\#或\#\#时就不会再展开宏中嵌套的宏了。什么意思了？比如使用char *pChar=STRING(__FILE__);虽然__FILE__本身也是一个宏，但编译器不会展开它，所以pChar将指向"__FILE__"而不是你要想的形如"D:\XXX.cpp"的源文件名称。因此要加一个中间转换宏，先将__FILE__解析成"D:\XXX.cpp"字符串。
定义如下所示二个宏：
\#define_STRING(x)\#x
\#defineSTRING(x)_STRING(x)
再调用下面语句将输出带""的源文件路径
char*pChar=STRING(__FILE__);
printf("%s %s\n",pChar, __FILE__);
可以比较下STRING(__FILE__)与__FILE__的不同，前将带双引号，后一个没有双引号。

再讲下\#\#的功能，它可以拼接符号（Token-pasting operator）。
MSDN上有个例子：
\#definepaster(n)printf("token"\#n" = %d\n", token\#\#n)
inttoken9= 100;
再调用paster(9);宏展开后token\#\#n直接合并变成了token9。整个语句变成了
printf("token""9"" = %d", token9 );
在C语言中字符串中的二个相连的双引号会被自动忽略，于是上句等同于
printf("token9 = %d", token9);。
即输出token9 = 100



有了上面的基础后再来看示例1

\#defineWIDEN2(x) L\#\#x
\#defineWIDEN(x)WIDEN2(x)
\#define__WFILE__WIDEN(__FILE__)
wchar_t*pwsz=__WFILE__;
第一个宏中的L是将ANSI字符串转化成unicode字符串。如：wchar_t*pStr= L"hello";
再来看wchar_t*pwsz=__WFILE__;
__WFILE__被首先展开成WIDEN(__FILE__)，再展开成WIDEN2("__FILE__表示的字符串")，再拼接成L"__FILE__表示的字符串"即L"D:\XXX.cpp"从而得到unicode字符串并取字符串地址赋值给pwsz指针。

在VC中_T()，TEXT()也是用的这种技术。
在tchar.h头文件中可以找到：
\#define_T(x)__T(x)
\#define__T(x)L\#\#x
在winnt.h头文件中可以找到
\#defineTEXT(quote)__TEXT(quote)// r_winnt
\#define__TEXT(quote) L\#\#quote// r_winnt
因此不难理解为什么第三条语句会出错error C2065: 'LszText' : undeclared identifier
wprintf(TEXT("%s %s\n"),_T("hello"),TEXT("hello"));
charszText[] ="hello";
wprintf(TEXT("%s %s\n"),_T(szText),TEXT(szText));
而将"hello"定义成宏后就能正确运行。
\#defineSZTEXT"hello"
wprintf(TEXT("%s %s\n"),_T(SZTEXT),TEXT(SZTEXT));
注：由于VC6.0默认是ANSI编码，因此要先设置成unicode编码，在project菜单中选择Setting，再在C/C++标签对话框中的Category中选择Preprocessor。再地Preprocessor definitions编辑框中将_MBCS去掉，加上_UNICODE,UNICODE。
![](http://pic002.cnblogs.com/images/2011/321044/2011081810560395.jpg)


更多内容可以查考MSDN上对Preprocessor Operators的讲解。

转载请标明出处，原文地址：[http://www.cnblogs.com/morewindows/archive/2011/08/18/2144112.html](http://www.cnblogs.com/morewindows/archive/2011/08/18/2144112.html)
[http://www.cnblogs.com/morewindows/archive/2011/08/18/2144112.html](http://www.cnblogs.com/morewindows/archive/2011/08/18/2144112.html)
﻿﻿

