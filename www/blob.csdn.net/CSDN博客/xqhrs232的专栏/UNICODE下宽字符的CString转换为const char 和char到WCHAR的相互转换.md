# UNICODE下宽字符的CString转换为const char *和char到WCHAR的相互转换 - xqhrs232的专栏 - CSDN博客
2015年01月09日 15:38:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：729
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)
相关文章
1、[CString const char*与char*之间的转换关系](http://blog.csdn.net/tridrop/article/details/2307876)----[http://blog.csdn.net/tridrop/article/details/2307876](http://blog.csdn.net/tridrop/article/details/2307876)
2、[cstring to char*,const char*](http://blog.csdn.net/kingmax54212008/article/details/4465196)----[http://blog.csdn.net/kingmax54212008/article/details/4465196](http://blog.csdn.net/kingmax54212008/article/details/4465196)
3、[CString的GetBuffer用法，GetBuffer本质，GetBuffer常见问题解决方法](http://blog.csdn.net/lewutian/article/details/6787024)----[http://blog.csdn.net/lewutian/article/details/6787024](http://blog.csdn.net/lewutian/article/details/6787024)

一、
使用函数_tcscpy_s：
CString theString( "This is a test" ); 
int    sizeOfString = (theString.GetLength() + 1); 
LPTSTR  lpsz = new TCHAR[ sizeOfString ]; 
_tcscpy_s(lpsz, sizeOfString, theString);
最后再转换一下lpsz为const型的
LPTSTR在UNICODE环境下编译是wchar_t类型
二、
CString str = _T("Hello World!");
char szStr[256] = {0};
wcstombs(szStr, str, str.GetLength());//Unicode转换为ASCII
const char * p = szStr;
 三、
size_t mbstowcs(    wchar_t* *[wcstr](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*,    const char* *[mbstr](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*,
    size_t *[count](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)* );////ASCII转换为Unicode
参数解释如下：
- *wcstr*
The address of a sequence of wide characters.- *mbstr*
The address of a sequence of multibyte characters.- *count*
The number of multibyte characters to convert.
四、
CString IMSI=_T("888888888888888"); 
 char *pBuffer1 = NULL;
  pBuffer1 = new char[IMSI.GetLength()+1];
  ZeroMemory(pBuffer1, IMSI.GetLength()+1);
  USES_CONVERSION;
  pBuffer1=(char*)W2A((LPCTSTR)IMSI);
  使用pBuffer1后，delete[] pBuffer1;但是我调试的时候报错，显示内存崩溃，后来我不用new了，直接初始化一个char数组，示例如下：
  char pBuffer1[16]={0};
  USES_CONVERSION;
  strcpy(pBuffer1,(char*)(W2A((LPCTSTR)registerPak.IMSI)));
这样也不用delete了，不会出现内存问题，如果哪位知道内存崩溃的原因给我留言啊！！
CString是个好东西，有很多好用的成员函数，并且动态分配内存空间。但在MFC学习初期，容易把CString与const char*，char*混淆。遇到三种类型数据转换时，总是得过且过。下面就剖析一下三者之间的转换关系与方法。
1、CString与const char*（LPCTSTR---是在Unicode环境下const char*的宏定义）
CString类提供一个const char*（）把CString类型转换为LPCTSTR类型。
比如AfxMessageBox()的使用，可以采用：
char szMessageText[] = "Unknown error";
AfxMessageBox(szMessageText);
也可以这样：
CString strMessageText("Unknown ;error");
AfxMessageBox(strMessageText);
CString类也提供了一个构造函式把LPCTSTR类型转换为CString类型。比如：
CString strTruth；
strTruth += " is alive";
2、CString与char*
调用CString::GetBuffer在Buffer中开辟一定大小的空间并返回一个char*。注意要在使用完char*后要调用CString::ReleaseBuffer以此保证CString的动态性。例如：
CString strTest("test");
strncpy(strTest.GetBuffer(5), "T", 1);
strTest.ReleaseBuffer();
ASSERT(strTest == "Test");编写以字符串为参数的函数所遵循的规则：a、如果函数不改写字符串的内容并且要调用C Runtime的函数，那么函数要用const char*类型参数;b、如果函数不改写字符串的内容并且要调用CString的成员函数，那么函数要用const CString&类型参数；c、如果函数要改写字符串的内容，那么函数要用CString&类型参数。
如果想把const char* 赋给char*，可以使用strcpy（）函数；
也可以参考MSDN：CString Operations Relating to C-Style Strings。ms-help://MS.VSCC.v80/MS.MSDN.v80/MS.VisualStudio.v80.en/dv_vclib/html/5048de8a-5298-4891-b8a0-c554b5a3ac1b.htm
另一种相互转换char和WCHAR的方法（也就是从ASCII的字符到Unicode的字符的相互转换）：
1，从char到WCHAR的转换：
int MultiByteToWideChar(
  UINT *[CodePage](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*,
  DWORD *[dwFlags](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*,
  LPCSTR *[lpMultiByteStr](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*,
  int *[cbMultiByte](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*,
  LPWSTR *[lpWideCharStr](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*,
  int *[cchWideChar](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*
);
2，从WCHAR到char的转换：
int WideCharToMultiByte(
  UINT *[CodePage](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*,
  DWORD *[dwFlags](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*,
  LPCWSTR *[lpWideCharStr](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*,
  int *[cchWideChar](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*,
  LPSTR *[lpMultiByteStr](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*,
  int *[cbMultiByte](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*,
  LPCSTR *[lpDefaultChar](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*,
  LPBOOL *[lpUsedDefaultChar](http://hailang19821213.blog.163.com/blog/static/306794612009923113044990/)*
);
