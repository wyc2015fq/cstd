# UNICODE编码细节与个人使用总结 - 深之JohnChen的专栏 - CSDN博客

2009年07月01日 16:06:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3230


最近做了一个需要支持Unicode的项目，现在把编程中一些心得总结一下。

1、TCHAR ,UNICODE,CHAR,wchar_t 之间的关系

经常发现有的人爱用strcpy等标准ANSI函数，有的人爱用_tXXXX函数，这个问题曾经搞的很混乱。为了统一，有必要搞清楚它们之间的关系。

为了搞清这些函数，就必须理请几种字符类型的写法。char就不用说了，先说一些wchar_t。wchar_t是Unicode字符的数据类型，它实际定义在<string.h>里：

typedef unsigned short wchar_t;

不能使用类似strcpy这样的ANSI C字符串函数来处理wchar_t字符串，必须使用wcs前缀的函数，例如wcscpy。为了让编译器识别Unicode字符串，必须以在前面加一个“L”,例如:

wchar_t *szTest=L"This is a Unicode string.";

下面在看看TCHAR。如果你希望同时为ANSI和Unicode编译的源代码，那就要include TChar.h。TCHAR是定义在其中的一个宏，它视你是否定义了_UNICODE宏而定义成char或者wchar_t。如果你使用了TCHAR，那么就不应该使用ANSI的strXXX函数或者Unicode的wcsXXX函数了，而必须使用TChar.h中定义的_tcsXXX函数。另外，为了解决刚才提到带“L”的问题，TChar.h中定义了一个宏：“_TEXT”。

以strcpy函数为例子，总结一下:

.如果你想使用ANSI字符串，那么请使用这一套写法：

char szString[100];

strcpy(szString,"test");

.如果你想使用Unicode字符串，那么请使用这一套：

wchar_t szString[100];

wcscpyszString,L"test");

.如果你想通过定义_UNICODE宏，而编译ANSI或者Unicode字符串代码：

TCHAR szString[100];

_tcscpy(szString,_TEXT("test"));

2、增加Unicode宏定义UNICODE,_UNICODE

3、如何在调式程序中显示Unicode字符，需要在VC开发工具“Tools”—>“Options”à“Debug”页中勾选“Display Unicode strings”选项。如图

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090701/unicode1.JPG)

4、使用Unicode的问题 wWinMainCRTStartup设定程序入口

project->   settings->Link在category：选择output在Entry   point   symbol：加上wWinMainCRTStartup  

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090701/unicode2.JPG)

5、几种编码之间的转换

//UTF8格式转换成GB格式

CString ConvertUTF8ToGBK(CString strUtf8)

{

int len=MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8.GetBuffer(0), -1, NULL,0); 

unsigned short * wszGBK = new unsigned short[len+1]; 

memset(wszGBK, 0, len * 2 + 2); 

MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8.GetBuffer(0), -1, wszGBK, len); 

len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL); 

char *szGBK=new char[len + 1]; 

memset(szGBK, 0, len + 1); 

WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL); 

CString strGBK;

strGBK = szGBK;

delete[] szGBK; 

delete[] wszGBK; 

return strGBK;

}

//GB格式转换成UTF8格式

CString ConvertGBKToUTF8(CString strGBK)

{

int len=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK.GetBuffer(0), -1, NULL,0); 

unsigned short * wszUtf8 = new unsigned short[len+1]; 

memset(wszUtf8, 0, len * 2 + 2); 

MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK.GetBuffer(0), -1, wszUtf8, len); 

len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL); 

char *szUtf8=new char[len + 1]; 

memset(szUtf8, 0, len + 1); 

WideCharToMultiByte (CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL,NULL);

CString sUTF_8;

sUTF_8=szUtf8;

delete[] szUtf8; 

delete[] wszUtf8; 

return sUTF_8;

}

//字符串转换float

float strtofloat(CString str)

{

char a[MAX_PATH];

memset(a, 0, MAX_PATH);

WideCharToMultiByte(CP_ACP, 0,(LPCWSTR)str, -1, a, MAX_PATH, NULL, NULL);

float f = (float)atof(a);

return f;

}

//UTF8转换成UNICDOE

wchar_t* U8ToUnicode(const char* szU8)

{

int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);

wchar_t* wszString = new wchar_t[wcsLen + 1];

::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);

wszString[wcsLen] = '/0';

return wszString;

}

//UNICODE转换成UTF8

char* UnicodeToU8(wchar_t* wszString)

{

int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);

char* szU8 = new char[u8Len + 1];

::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), szU8, u8Len, NULL, NULL);

szU8[u8Len] = '/0';

return szU8;

}

//UNICODE转换成ANSI

char* UnicodeToAnsi(wchar_t* wszString)

{

int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);

char* szAnsi = new char[ansiLen + 1];

::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), szAnsi, ansiLen, NULL, NULL);

szAnsi[ansiLen] = '/0';

return szAnsi;

}

