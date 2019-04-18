# char* 转 LPCWSTR 解决方案 - 文章 - 伯乐在线
原文出处： [周旭光，2013-04-05](http://blog.csdn.net/zhouxuguang236/article/details/8761497)
在Windows编程中，经常会碰到字符串之间的转换，char*转LPCWSTR也是其中一个比较常见的转换。下面就列出几种比较常用的转换方法。
**1、通过 MultiByteToWideChar 函数转换**
MultiByteToWideChar函数是将多字节转换为宽字节的一个API函数，它的原型如下：


```
int MultiByteToWideChar(  
  UINT CodePage,         // code page  
  DWORD dwFlags,         // character-type options  
  LPCSTR lpMultiByteStr, // string to map  
  int cbMultiByte,       // number of bytes in string  
  LPWSTR lpWideCharStr,  // wide-character buffer  
  int cchWideChar        // size of buffer  
);
```
LPCWSTR实际上也是CONST WCHAR *类型


```
char* szStr = "测试字符串";  
WCHAR wszClassName[256];  
memset(wszClassName,0,sizeof(wszClassName));  
MultiByteToWideChar(CP_ACP,0,szStr,strlen(szStr)+1,wszClassName,  
sizeof(wszClassName)/sizeof(wszClassName[0]));
```
**2、通过 T2W 转换宏**


```
char* szStr = "测试字符串";     
CString str = CString(szStr);  
USES_CONVERSION;  
LPCWSTR wszClassName = new WCHAR[str.GetLength()+1];  
wcscpy((LPTSTR)wszClassName,T2W((LPTSTR)str.GetBuffer(NULL)));  
str.ReleaseBuffer();
```
**3、通过 A2CW 转换**


```
char* szStr = "测试字符串";     
CString str = CString(szStr);  
USES_CONVERSION;  
LPCWSTR wszClassName = A2CW(W2A(str));  
str.ReleaseBuffer();
```
上述方法都是UniCode环境下测试的。
