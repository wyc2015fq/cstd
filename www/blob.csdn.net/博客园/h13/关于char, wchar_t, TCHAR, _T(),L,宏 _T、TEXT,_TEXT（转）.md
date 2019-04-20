# 关于char, wchar_t, TCHAR, _T(),L,宏 _T、TEXT,_TEXT（转） - h13 - 博客园
char :单字节变量类型，最多表示256个字符，
wchar_t :宽字节变量类型，用于表示Unicode字符，
它实际定义在<string.h>里：typedef unsigned short wchar_t。
为了让编译器识别Unicode字符串，必须以在前面加一个“L”,定义宽字节类型方法如下：
  wchar_t c = `A' ; 
wchar_t * p = L"Hello!" ; 
wchar_t a[] = L"Hello!" ;
其中，宽字节类型每个变量占用2个字节，故上述数组a的sizeof(a) = 14
TCHAR / _T( ) : 
如果在程序中既包括ANSI又包括Unicode编码，需要包括头文件tchar.h。TCHAR是定义在该头文件中的宏，它视你是否定义了_UNICODE宏而定义成： 
定义了_UNICODE：    typedef wchar_t TCHAR ; 
没有定义_UNICODE： typedef char TCHAR ;
#ifdef UNICODE 
typedef char TCHAR; 
#else 
typede wchar_t TCHAR; 
#endif 
_T( )也是定义在该头文件中的宏，视是否定义了_UNICODE宏而定义成： 
定义了_UNICODE：    #define _T(x) L##x 
没有定义_UNICODE： #define _T(x) x 
注意：如果在程序中使用了TCHAR，那么就不应该使用ANSI的strXXX函数或者Unicode的wcsXXX函数了，而必须使用tchar.h中定义的_tcsXXX函数。
以strcpy函数为例子，总结一下：
```
//如果你想使用ANSI字符串，那么请使用这一套写法： 
char szString[100]; 
strcpy(szString,"test"); 
//如果你想使用Unicode字符串，那么请使用这一套： 
wchar_t szString[100]; 
wcscpy(szString,L"test"); 
//如果你想通过定义_UNICODE宏，而编译ANSI或者Unicode字符串代码： 
TCHAR szString[100]; 
_tcscpy(szString,_TEXT("test"));
//如果你想使用ANSI字符串，那么请使用这一套写法： 
char szString[100]; 
strcpy(szString,"test"); 
//如果你想使用Unicode字符串，那么请使用这一套： 
wchar_t szString[100]; 
wcscpy(szString,L"test"); 
//如果你想通过定义_UNICODE宏，而编译ANSI或者Unicode字符串代码： 
TCHAR szString[100]; 
_tcscpy(szString,_TEXT("test"));
```
CSDN:superarhow说： 不要再使用TCHAR和_T了！他分析了原因后总结：如 果您正开始一个新的项目，请无论如何也要顶住压力，直接使用UNICODE编码！切记！您只需要对您的组员进行10分钟的培训，记住strcpy用 wcscpy，sprintf用swprintf代替，常数前加L，就可以了！它不会花您很多时间的，带给您的是稳定和安全！相信偶，没错的！！
一、 在字符串前加一个L作用: 
如  L"我的字符串"    表示将ANSI字符串转换成unicode的字符串，就是每个字符占用两个字节。 
strlen("asd")   =   3;   
strlen(L"asd")   =   6;
二、  _T宏可以把一个引号引起来的字符串，根据你的环境设置，使得编译器会根据编译目标环境选择合适的（Unicode还是ANSI）字符处理方式 
如果你定义了UNICODE，那么_T宏会把字符串前面加一个L。这时 _T("ABCD") 相当于 L"ABCD" ，这是宽字符串。 
如果没有定义，那么_T宏不会在字符串前面加那个L，_T("ABCD") 就等价于 "ABCD"
三、TEXT,_TEXT 和_T 一样的 
如下面三语句：   
TCHAR   szStr1[]   =   TEXT("str1");   
char   szStr2[]   =   "str2";   
WCHAR   szStr3[]   =   L("str3");
 那么第一句话在定义了UNICODE时会解释为第三句话，没有定义时就等于第二句话。   
但二句话无论是否定义了UNICODE都是生成一个ANSI字符串，而第三句话总是生成UNICODE字符串。   
为了程序的可移植性，建议都用第一种表示方法。   
但在某些情况下，某个字符必须为ANSI或UNICODE，那就用后两种方法
