# SDK编程笔记-UNICODE篇 - gauss的专栏 - CSDN博客
2011年12月13日 21:47:17[gauss](https://me.csdn.net/mathlmx)阅读数：183标签：[编程																[windows																[语言																[编译器																[c																[任务](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
个人分类：[Window编程](https://blog.csdn.net/mathlmx/article/category/944343)
**SDK编程笔记-UNICODE篇**
[简单基础但重要]
　　Petzold的Programming Windows的第2章非常详细的讲解了关于Unicode的方方面面。我们需要知道的是它给C语言和Windows编程带来的变化。
**·C语言相关**
　　对应于char, C语言中也有宽字符内型wchar_t。wchar_t被定义为： typedef unsigned short wchar_t ;显然它是16位的。wchar_t类型的常字串应该这样写：
L"hello" 。因此可以这样定义一个宽字符指针 
wchar_t *pwc=L"hello";。
　　对于字符串函数， C语言中也有两个版本。如对应于strlen有wcslen，wprintf是printf的宽字符版。当然这两个版本的函数使用各自类型的参数，原因是显然的，char和unsigned short 怎么能等同呢？编译器是不会放过你的。
　　下表是对char,wchar_t相关内容的比较： 
|（测试环境：VC++6.0）|char类型|wchar_t类型|
|----|----|----|
|类型大小（32位系统）|8位|16位|
|常量字符表示法|'A'|L'A' 或 'A'|
|常量字符串表示法|'hello'|L'hello'|
|一些使用方法|char c='A'； (c的值:0x41)|wchar_t wc='A'； (wc的值:0x0041)|
|char* p='hello'；|wchar_t* pw=L"hello"；| |
|sizeof|sizeof("hello")=6|sizeof(L"hello")=12|
|计算长度|strlen("hello")=5|wcslen(L"hello)=5|
　　既然有两个版本的函数，如果我们想在程序中同时支持它们该怎么办？如果使用VC++,你只要在程序中包含tchar.h头文件，这个文件中提供了一些宏以兼容两种字符集。例如：
#ifdef _UNICODE 
　typedef wchar_t _TCHAR;
　typedef wchar_t TCHAR;
　#define __T(x) L ## x
　#define _tprintf wprintf
　#define _tcslen wcslen
　......
#else
　typedef char _TCHAR;
　typedef char TCHAR;
　#define __T(x) x 
　#define _tprintf printf 
　#define _tcslen strlen
　......
#endif
#define _T(x) __T(x)
#define _TEXT(x) __T(x)
　　我们只要使用TCHAR,_tpirntf,_tcslen,_TEXT等，就可以兼顾两种字符集。 
**·Windows相关**
　　Windows2000/NT完全支持Unicode,Windows98对Unicode支持很少。我们关心的是怎样编写程序，既可以编译为不支持Unicode，又可编译为支持Unicode。Windows SDK的头文件中定义了一些宏以完成这个任务。
typedef char CHAR ; 
typedef wchar_t WCHAR ; // wc
typedef CHAR * PCHAR, * LPCH, * PCH, * NPSTR, * LPSTR, * PSTR ;
typedef CONST CHAR * LPCCH, * PCCH, * LPCSTR, * PCSTR ;
typedef WCHAR * PWCHAR, * LPWCH, * PWCH, * NWPSTR, * LPWSTR, * PWSTR ;
typedef CONST WCHAR * LPCWCH, * PCWCH, * LPCWSTR, * PCWSTR ;
　　和标准C一样，windows C也使用TCHAR作为兼容的类型
#ifdef UNICODE 
　typedef WCHAR TCHAR, * PTCHAR ;
　typedef LPWSTR LPTCH, PTCH, PTSTR, LPTSTR ; 
　typedef LPCWSTR LPCTSTR ;
　#define __TEXT(quote) L##quote 
#else 
　typedef char TCHAR, * PTCHAR ; 
　typedef LPSTR LPTCH, PTCH, PTSTR, LPTSTR ;
　typedef LPCSTR LPCTSTR ;
　#define __TEXT(quote) quote
#endif
　　另外还有一个TEXT宏，等价于__TEXT宏：#define TEXT(quote) __TEXT(quote)。
　　写Win32 SDK程序的时候常常在注册窗口类时使用如下代码：
if (!RegisterClass (&wndclass)) {
　　MessageBox ( NULL, TEXT ("This program requires Winodws 2000/NT"), szAppName, 　　　　　　　　　　　MB_ICONERROR) ;
　　return 0 ;
} 
　　这样，当在Windows98下运行使用了Unicode的程序时，程序可以给出提示并退出。这段代码为什么在Win98下可以运行？因为MessageBox在存在Unicode的定义下，被替换为MessageBoxW，而MessageBoxW是Win98支持的少数宽字符Windows函数之一。
　　总之，写Windows SDK程序时尽可能使用像TCHAR,PTSTR,LPCTSTR,TEXT这样的宏。
本文转载:http://blog.csdn.net/n5/article/details/14270
