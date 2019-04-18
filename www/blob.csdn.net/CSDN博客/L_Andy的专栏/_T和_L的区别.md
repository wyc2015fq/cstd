# _T和_L的区别 - L_Andy的专栏 - CSDN博客

2013年05月14日 10:55:26[卡哥](https://me.csdn.net/L_Andy)阅读数：1292
个人分类：[C/C++编程](https://blog.csdn.net/L_Andy/article/category/1925955)



问：

#define ABC L"ABC"

L 宏是干什么用的,和Unicode相关吗?

如果这样,这和

#define ABC _T("ABC")

有分别吗?

答：

L表示UNICODE串，比如wchar_t* str = L"yangsongx";

_T在ANSI编译模式下表示ANSI串，在UNICODE下表示UNICODE串，比如

TCHAR* str = _T("yangsongx");

在ANSI下编译就是 char* str = "yangsongx";

在UNICODE下编译就是 wchar_t* str = L"yangsongx";

http://blog.csdn.net/Awey_001/archive/2011/01/12/6130795.aspx

字符串前面加L表示该字符串是Unicode字符串。

_T是一个宏，如果项目使用了Unicode字符集（定义了UNICODE宏），则自动在字符串前面加上L，否则字符串不变。因此，Visual C++里边定义字符串的时候，用_T来保证兼容性。VC支持ascii和unicode两种字符类型，用_T可以保证从ascii编码类型转换到unicode编码类型的时候，程序不需要修改。

以下是别人的总结：

一、在字符串前加一个L作用:  

注释：GDI+中只能使用宽字符L。eg：

Graphics graphics( pDC->m_hDC );

Image image(L"log.gif"); //使用宽字符L

UINT width = image.GetWidth();

UINT height = image.GetHeight();

graphics.DrawImage( &image,10,10);

/////////////////////////////////////

  如 L"我的字符串" 表示将ANSI字符串转换成unicode的字符串，就是每个字符占用两个字节。  

  strlen("asd") = 3;   

  strlen(L"asd") = 6;  

  二、 _T宏可以把一个引号引起来的字符串，根据你的环境设置，使得编译器会根据编译目标环境选择合适的（Unicode还是ANSI）字符处理方式  

  如果你定义了UNICODE，那么_T宏会把字符串前面加一个L。这时 _T("ABCD") 相当于 L"ABCD" ，这是宽字符串。 

  如果没有定义，那么_T宏不会在字符串前面加那个L，_T("ABCD") 就等价于 "ABCD"  

三、TEXT,_TEXT 和_T 一样的  

如下面三语句：   

  TCHAR szStr1[] = TEXT("str1");   

  char szStr2[] = "str2";   

  WCHAR szStr3[] = L("str3");   

  那么第一句话在定义了UNICODE时会解释为第三句话，没有定义时就等于第二句话。   

  但二句话无论是否定义了UNICODE都是生成一个ANSI字符串，而第三句话总是生成UNICODE字符串。 

  为了程序的可移植性，建议都用第一种表示方法。但在某些情况下，某个字符必须为ANSI或UNICODE，那就用后两种方法。

别人的总结2：

你要确定你需要的字符串是宽字符还是窄字符。_T("")是说如果你定义了UNICODE 那么就是L"",没有定义就是"";以下情况用_T（）比较好，其他情况最好别用：

1，用THCAR，LPTSTR，LPCTSTR等tchar数据类型的时候

2，用_tprintf之类的_t版本运行时函数时候

3，像下面这样有w和a版本的api，调用CreateFile的时候

#ifdef UNICODE

#define CreateFile CreateFileW

#else

#define CreateFile CreateFileA

#endif // !UNICODE

