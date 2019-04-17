# C++ int，char，string，CString类型转换 [转] - 战斗蜗牛的专栏 - CSDN博客





2009年12月12日 13:11:00[vbskj](https://me.csdn.net/vbskj)阅读数：757








#include <string> //使用C++标准库的string类时

using namespace std; //同上

#include <sstream>  

#include <iostream>

#include <stdlib.h>    //要将string类和int类型直接转换最好有这些包含，

                       //因为自己写一个转换函数比较方便，函数定义参考如下

string getstring ( const int n )

{

std::stringstream newstr;
    newstr<<n;
    return newstr.str();

}

////////////////// 以下转载自[http://successfulfortune.com/icode/?p=4](http://successfulfortune.com/icode/?p=4)

string 转 CString
CString.format(”%s”, string.c_str());

char 转 CString
CString.format(”%s”, char*);

char 转 string
string s(char *);

string 转 char *
char *p = string.c_str();

CString 转 string
string s(CString.GetBuffer());

1，string -> CString
CString.format(”%s”, string.c_str());
用c_str()确实比data()要好.
2，char -> string
string s(char *);
只能初始化，在不是初始化的地方最好还是用assign().
3,CString -> string
string s(CString.GetBuffer());
GetBuffer()后一定要ReleaseBuffer(),否则就没有释放缓冲区所占的空间.

《C++标准函数库》中说的
有三个函数可以将字符串的内容转换为字符数组和C―string
1.data(),返回没有”/0“的字符串数组
2,c_str()，返回有”/0“的字符串数组
3，copy()

―――――――――――――――――――――

CString与int、char*、char[100]之间的转换- -

CString与int、char*、char[100]之间的转换- -

CString互转int

将字符转换为整数，可以使用atoi、_atoi64或atol。
而将数字转换为CString变量，可以使用CString的Format函数。如
CString s;
int i = 64;
s.Format(”%d”, i)
Format函数的功能很强，值得你研究一下。

void CStrDlg::OnButton1()
{
// TODO: Add your control notification handler code here
CString
ss=”1212.12″;
int temp=atoi(ss);
CString aa;
aa.Format(”%d”,temp);
AfxMessageBox(”var is ” + aa);
}

sart.Format(”%s”,buf);

CString互转char*

///char * TO cstring          直接赋值或使用Cstring::Format
CString strtest;
char * charpoint;
charpoint=”give string a value”;
strtest=charpoint;

char chArray[] = "Char  test";
TCHAR * p = _T("Char  test");( 或LPTSTR p = _T("Char  test");)
CString theString = chArray;
theString.Format(_T("%s"), chArray);
theString = p;

///cstring TO char *
**方法一，使用强制转换。**例如：

       CString theString( (_T("Char test "));
        LPTSTR lpsz =(LPTSTR)(LPCTSTR)theString;

**方法二，使用strcpy**。例如：

       CString theString( (_T("Char test "));
       LPTSTR lpsz = new TCHAR[theString.GetLength()+1];
        _tcscpy(lpsz, theString);
**方法三，使用CString::GetBuffer。**

       charpoint=strtest.GetBuffer(strtest.GetLength());

CString s(_T("Char test "));
        LPTSTR p = s.GetBuffer(); 

        LPTSTR dot = strchr(p, ''.''); 

         // 在这里添加使用p的代码

          if(p != NULL) 

         *p = _T('');
         s.ReleaseBuffer();                     // 使用完后及时释放，以便能使用其它的CString成员函数



