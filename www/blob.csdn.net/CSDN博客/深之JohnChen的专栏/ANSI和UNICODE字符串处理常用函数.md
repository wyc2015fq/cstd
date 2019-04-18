# ANSI和UNICODE字符串处理常用函数 - 深之JohnChen的专栏 - CSDN博客

2018年05月30日 11:22:29[byxdaz](https://me.csdn.net/byxdaz)阅读数：956


ANSI和UNICODE字符串处理函数

在windows中ANSI和UNICODE字符串操作分别提供了相应的函数，也提供了一套兼容两者的函数。比如：

#ifdef  UNICODE  

    #define _tcscpy     wcscpy 

#else 

    #define _tcscpy     strcpy 

#endif

字符串处理函数常用函数对照
|**ANSI**|**UNICODE**|**通用**|**说明**|
|----|----|----|----|
|数据类型| | | |
|(char.h)|(wchar.h)|(tchar.h)||
|char|wchar_t|TCHAR||
|char *|wchar_t *|TCHAR*||
|LPSTR|LPWSTR|LPTSTR||
|LPCSTR|LPCWSTR|LPCTSTR||
|||||
|字符串转换| | | |
|atoi|_wtoi|_ttoi|把字符串转换成整数（int）|
|atol|_wtol|_ttol|把字符串转换成长整型数（long）|
|atof|_wtof|_tstof|把字符串转换成浮点数（double）|
|itoa|_itow|_itot|将任意类型的数字转换为字符串|
|||||
|字符串操作| | | |
|strlen|wcslen|_tcslen|获得字符串的数目|
|strcpy|wcscpy|_tcscpy|拷贝字符串|
|strncpy|wcsncpy|_tcsncpy|类似于strcpy/wcscpy，同时指定拷贝的数目|
|strcmp|wcscmp|_tcscmp|比较两个字符串|
|strncmp|wcsncmp|_tcsncmp|类似于strcmp/wcscmp，同时指定比较字符字符串的数目|
|strcat|wcscat|_tcscat|把一个字符串接到另一个字符串的尾部|
|strncat|wcsncat|_tcsnccat|类似于strcat/wcscat，而且指定粘接字符串的粘接长度.|
|strchr|wcschr|_tcschr|查找子字符串的第一个位置|
|strrchr|wcsrchr|_tcsrchr|从尾部开始查找子字符串出现的第一个位置|
|strpbrk|wcspbrk|_tcspbrk|从一字符字符串中查找另一字符串中任何一个字符第一次出现的位置|
|strstr|wcsstr/wcswcs|_tcsstr|在一字符串中查找另一字符串第一次出现的位置|
|strcspn|wcscspn|_tcscspn|返回不包含第二个字符串的的初始数目|
|strspn|wcsspn|_tcsspn|返回包含第二个字符串的初始数目|
|strtok|wcstok|_tcstok|根据标示符把字符串分解成一系列字符串|
||wcswidth||获得宽字符串的宽度|
||wcwidth||获得宽字符的宽度|
|||||
|字符串测试| | | |
|isascii|iswascii|_istascii|测试字符是否为ASCII 码字符, 也就是判断c 的范围是否在0 到127 之间|
|isalnum|iswalnum|_istalnum|测试字符是否为数字或字母|
|isalpha|iswalpha|_istalpha|测试字符是否是字母|
|iscntrl|iswcntrl|_istcntrl|测试字符是否是控制符|
|isdigit|iswdigit|_istdigit|测试字符是否为数字|
|isgraph|iswgraph|_istgraph|测试字符是否是可见字符|
|islower|iswlower|_istlower|测试字符是否是小写字符|
|isprint|iswprint|_istprint|测试字符是否是可打印字符|
|ispunct|iswpunct|_istpunct|测试字符是否是标点符号|
|isspace|iswspace|_istspace|测试字符是否是空白符号|
|isupper|iswupper|_istupper|测试字符是否是大写字符|
|isxdigit|iswxdigit|_istxdigit|测试字符是否是十六进制的数字|
|大小写转换| | | |
|tolower|towlower|_totlower|把字符转换为小写|
|toupper|towupper|_totupper|把字符转换为大写|
|字符比较| | | |
|strcoll|wcscoll|_tcscoll|比较字符串|
|日期和时间转换| | | |
|strftime|wcsftime|_tcsftime|根据指定的字符串格式和locale设置格式化日期和时间|
|strptime|||根据指定格式把字符串转换为时间值， 是strftime的反过程|
|打印和扫描字符串| | | |
|printf|wprintf|_tprintf|使用vararg参量的格式化输出到标准输出|
|fprintf|fwprintf|_ftprintf|使用vararg参量的格式化输出|
|scanf|wscanf|_tscanf|从标准输入的格式化读入|
|fscanf|fwscanf|_ftscanf|格式化读入|
|sprintf|swprintf|_stprintf|根据vararg参量表格式化成字符串|
|sscanf|swscanf|_stscanf|以字符串作格式化读入|
|vfprintf|vfwprintf|_vftprintf|使用stdarg参量表格式化输出到文件|
|vprintf|||使用stdarg参量表格式化输出到标准输出|
|vsprintf|vswprintf|_vstprintf|格式化stdarg参量表并写到字符串|
|sprintf_s|swprintf_s|_stprintf_s|格式化字符串|
|数字转换| | | |
|strtod|wcstod|_tcstod|把字符串的初始部分转换为双精度浮点数|
|strtol|wcstol|_tcstol|把字符串的初始部分转换为长整数|
|strtoul|wcstoul|_tcstoul|把字符串的初始部分转换为无符号长整数|
|_strtoi64|_wcstoi64|_tcstoi64||
|||||
|输入和输出| | | |
|fgetc|fgetwc|_fgettc|从流中读入一个字符并转换为宽字符|
|fgets|fgetws|_fgetts|从流中读入一个字符串并转换为宽字符串|
|fputc|fputwc|_fputtc|把宽字符转换为多字节字符并且输出到标准输出|
|fputs|fputws|_fputts|把宽字符串转换为多字节字符并且输出到标准输出串|
|getc|getwc|_gettc|从标准输入中读取字符， 并且转换为宽字符|
|getchar|getwchar|_gettchar|从标准输入中读取字符|
|putc|putwc|_puttc|标准输出|
|putchar|putwchar|_puttchar|标准输出|
|ungetc|ungetwc|_ungettc|把一个字符放回到输入流中|

多字节和宽字节字符串的转换

使用MultiByteToWideChar函数和WideCharToMultiByte函数可以实现多字节宽字节之间的转换。ATL的一个很好的字符串的转换宏：A2W和W2A，使用起来更加简单。

一、MultiByteToWideChar和WideCharToMultiByte函数

```cpp
#include <string> 
#include <windows.h> 
using namespace std; 
//Converting a WChar string to a Ansi string 
std::string WChar2Ansi(LPCWSTR pwszSrc) 
{ 
         int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL); 
         if (nLen<= 0) return std::string(""); 
         char* pszDst = new char[nLen]; 
         if (NULL == pszDst) return std::string(""); 
         WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL); 
         pszDst[nLen -1] = 0; 
         std::string strTemp(pszDst); 
         delete [] pszDst; 
         return strTemp; 
}
string ws2s(wstring& inputws) 
{ 
        return WChar2Ansi(inputws.c_str()); 
}
//Converting a Ansi string to WChar string 
std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen) 
{ 
    int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0); 
    if(nSize <= 0) return NULL; 
         WCHAR *pwszDst = new WCHAR[nSize+1]; 
    if( NULL == pwszDst) return NULL; 
    MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc, nLen, pwszDst, nSize); 
    pwszDst[nSize] = 0; 
    if( pwszDst[0] == 0xFEFF)                    // skip Oxfeff 
        for(int i = 0; i < nSize; i ++) 
                            pwszDst[i] = pwszDst[i+1]; 
    wstring wcharString(pwszDst); 
         delete pwszDst; 
    return wcharString; 
}
std::wstring s2ws(const string& s) 
{ 
     return Ansi2WChar(s.c_str(),s.size()); 
}
```

二、MultiByteToWideChar和WideCharToMultiByte函数

```
std::wstring AsciiToUnicode(const std::string& str)
{
	// 预算-缓冲区中宽字节的长度  
	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	// 给指向缓冲区的指针变量分配内存  
	wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
	// 开始向缓冲区转换字节  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, unicodeLen);
	std::wstring ret_str = pUnicode;
	free(pUnicode);
	return ret_str;
}

std::string UnicodeToUtf8(const std::wstring& wstr)
{
	// 预算-缓冲区中多字节的长度  
	int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	// 给指向缓冲区的指针变量分配内存  
	char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
	// 开始向缓冲区转换字节  
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
	std::string ret_str = pAssii;
	free(pAssii);
	return ret_str;
}

//ANSI转UTF8
std::string AsciiToUtf8(const std::string& str)
{
	return UnicodeToUtf8(AsciiToUnicode(str));
}

//UTF8转ANSI
std::string Utf8toAscii(const std::string strUTF8)
{
	std::string  strAnsi = "";
	//获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区
	UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, NULL, NULL);
	WCHAR *wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR *szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;
	strAnsi = szBuffer;
	//清理内存
	delete[]szBuffer;
	delete[]wszBuffer;
	return strAnsi;
}
```

三、A2W、W2A、A2T、T2A的使用方法

类型T。如果定义了 _UNICODE，则T表示W；如果定义了 _MBCS，则T表示A

头文件包含

#include <atlbase.h>

（1）A2W的用法：

```cpp
USES_CONVERSION;   // 只需要调用一次，就可以在函数中进行多次转换
CString tmpStr;  
char*LineChar="fdsfdsa";  
const WCHAR * cLineChar = A2W(LineChar);  
tmpStr=cLineChar;  
//获取字符个数
wcslen(cLineChar );
```

（2）W2A的用法：

```cpp
USES_CONVERSION;  
CString tmpStr;  
WCH LineChar="fdsfdsa";  
const char* cLineChar = A2W(LineChar);
```

（3）A2T的用法：

```cpp
USES_CONVERSION;  
char * pChar="char to cstring";  
CString cTemp=A2T(pChar);
```

（4）T2A的用法：

```cpp
USES_CONVERSION;  
CString cTemp =_T("char to cstring");  
char * pChar=T2A(cTemp.GetBuffer(cTemp.GetLength()));
```

```cpp

```

四、几种ANSI和UNICODE的宏定义

L 在字符串前加一个L，如 L”我的字符串” 表示将ANSI字符串转换成unicode的字符串，就是每个字符占用两个字节。

strlen(L”asd”) = 6;

_T宏可以把一个引号引起来的字符串，根据你的环境设置，使得编译器会根据编译目标环境选择合适的（Unicode还是ANSI）字符处理方式。

TEXT,_TEXT 和_T 一样的。

std兼容字符串定义

```cpp
#ifdef _UNICODE  
#define tstring std::wstring  
#else  
#define tstring std::string  
#endif
```

long int strtol(const char *str, char **endptr, int base) 把参数 str 所指向的字符串根据给定的 base 转换为一个长整数（类型为 long int 型），base 必须介于 2 和 36（包含）之间，或者是特殊值 0。

实例：

```cpp
#include <stdio.h>
#include <stdlib.h>
int main()
{
   char str[30] = "2030300 This is test";
   char *ptr;
   long ret;
   ret = strtol(str, &ptr, 10);
   printf("数字（无符号长整数）是 %ld\n", ret);
   printf("字符串部分是 |%s|", ptr);
   return(0);
}
```

结果：

数字（无符号长整数）是 2030300

字符串部分是 | This is test|

开源字符转换iconv

http://www.gnu.org/software/libiconv/

https://blog.csdn.net/langresser_king/article/details/7459367

