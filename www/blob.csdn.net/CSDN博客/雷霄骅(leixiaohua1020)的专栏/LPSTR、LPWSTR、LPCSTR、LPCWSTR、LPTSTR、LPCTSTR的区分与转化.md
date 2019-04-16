# LPSTR、LPWSTR、LPCSTR、LPCWSTR、LPTSTR、LPCTSTR的区分与转化 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年07月11日 12:20:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：50
个人分类：[纯编程](https://blog.csdn.net/leixiaohua1020/article/category/1647545)










首先在编译程序时经常会遇到这种问题：


`错误 1 error C2664: “CWnd::MessageBoxW”: 不能将参数 1 从“const char [3]”转换为“LPCTSTR”`

1、觉得很烦，一般的做法就是不管三七二十一，在字符串前面加一个‘L’：
如调用函数FUN(LPCTSTR str)时，不能 FUN("HELLO"); 而是FUN(L"HELLO");
通常这样做都比较能解决问题。


2、或者还可以用_T()， 即 FUN(_T("HELLO")); _T() 的别名还有 _TEXT(), TEXT()。


稍微研究了下，BSTR,LPSTR,LPWSTR,LPCTSTR,LPTSTR等这些让人头晕的东东。（还是C#里简单啊，直接tostring）

```
BSTR：是一个OLECHAR*类型的Unicode字符串，是一个COM字符串，带长度前缀，与VB有关，没怎么用到过。
LPSTR：即 char *，指向以'/0'结尾的8位（单字节）ANSI字符数组指针
LPWSTR：即wchar_t *，指向'/0'结尾的16位（双字节）Unicode字符数组指针
LPCSTR：即const char *
LPCWSTR：即const wchar_t *
LPTSTR：LPSTR、LPWSTR两者二选一，取决于是否宏定义了UNICODE或ANSI
LPCTSTR： LPCSTR、LPCWSTR两者二选一，取决于是否宏定义了UNICODE或ANSI。
```



如下是从MFC库中拷来的：

```cpp
#ifdef UNICODE
    typedef LPWSTR LPTSTR;
    typedef LPCWSTR LPCTSTR;
#else
    typedef LPSTR LPTSTR;
    typedef LPCSTR LPCTSTR;
#endif
```



3、相互转换方法：
前面需要定义：

```cpp
USES_CONVERSION;
```

转换方法：

```cpp
LPWSTR->LPTSTR:   W2T();
LPTSTR->LPWSTR:   T2W();
LPCWSTR->LPCSTR: W2CT();
LPCSTR->LPCWSTR: T2CW();
ANSI->UNICODE:     A2W();
UNICODE->ANSI:     W2A();
```


另外，CString转为CStringW方法(通过一个wchar_t数组来转)

```cpp
CString str;
 CStringW strw;
 wchar_t *text = new wchar_t[sizeof(wchar_t) * str.GetLength()];
 MultiByteToWideChar(CP_ACP,0,str,-1,text,str.GetLength());
 strw = text;
```


4、另外，还可以强行转换，不过不一定能成功




5、还有_bstr_t ( 对BTSTR的封装，需要#include<comdef.h> )，也可将单字节字符数组指针转为双字节字符数组指针，还没怎么没用到过。



原文链接：[http://blog.csdn.net/jize00/article/details/4379478](http://blog.csdn.net/jize00/article/details/4379478)




