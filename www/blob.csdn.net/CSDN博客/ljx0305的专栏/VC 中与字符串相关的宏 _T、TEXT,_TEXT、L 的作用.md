# VC 中与字符串相关的宏 _T、TEXT,_TEXT、L 的作用  - ljx0305的专栏 - CSDN博客
2009年08月13日 10:31:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：645标签：[编译器](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)
个人分类：[C++																[VC																[MFC](https://blog.csdn.net/ljx0305/article/category/403913)](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/380566)
一、 在字符串前加一个L作用: 
   如  L"我的字符串"    表示将ANSI字符串转换成unicode的字符串，就是每个字符占用两个字节。
  strlen("asd")   =   3;   
  strlen(L"asd")   =   6;
  二、  _T宏可以把一个引号引起来的字符串，根据你的环境设置，使得编译器会根据编译目标环境选择合适的（Unicode还是ANSI）字符处理方式
   如果你定义了UNICODE，那么_T宏会把字符串前面加一个L。这时 _T("ABCD") 相当于 L"ABCD" ，这是宽字符串。 
   如果没有定义，那么_T宏不会在字符串前面加那个L，_T("ABCD") 就等价于 "ABCD"
三、TEXT,_TEXT 和_T 一样的
如下面三语句：   
  TCHAR   szStr1[]   =   TEXT("str1");   
  char   szStr2[]   =   "str2";   
  WCHAR   szStr3[]   =   L("str3");   
  那么第一句话在定义了UNICODE时会解释为第三句话，没有定义时就等于第二句话。  
  但二句话无论是否定义了UNICODE都是生成一个ANSI字符串，而第三句话总是生成UNICODE字符串。   
  为了程序的可移植性，建议都用第一种表示方法。   
  但在某些情况下，某个字符必须为ANSI或UNICODE，那就用后两种方法。
引用:http://www.cnblogs.com/txwsh1/archive/2008/03/06/1093335.html
