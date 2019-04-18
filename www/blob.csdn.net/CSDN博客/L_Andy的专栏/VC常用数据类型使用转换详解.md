# VC常用数据类型使用转换详解 - L_Andy的专栏 - CSDN博客

2013年01月07日 10:25:06[卡哥](https://me.csdn.net/L_Andy)阅读数：1536
个人分类：[VC/MFC编程](https://blog.csdn.net/L_Andy/article/category/1099539)



VC常用数据类型使用转换详解

 std::string str;

LPCTSTR lpstr = (LPCTSTR)str.c_str();

//c_str函数的返回值是const char*的，不能直接赋值给char*，所以就需要我们进行相应的操作转化。

CString ,BSTR ,LPCTSTR之间关系和区别

CString是一个动态TCHAR数组，BSTR是一种专有格式的字符串（需要用系统提供的函数来操纵，LPCTSTR只是一个常量的TCHAR指针。

CString 是一个完全独立的类，动态的TCHAR数组，封装了 + 等操作符和字符串操作方法。

typedef OLECHAR FAR* BSTR;

typedef const char * LPCTSTR;

vc++中各种字符串的表示法 

首先char* 是指向ANSI字符数组的指针，其中每个字符占据8位（有效数据是除掉最高位的其他7位），这里保持了与传统的C,C++的兼容。

LP的含义是长指针(long pointer)。LPSTR是一个指向以‘\0’结尾的ANSI字符数组的指针，与char*可以互换使用，在win32中较多地使用LPSTR。

而LPCSTR中增加的‘C’的含义是“CONSTANT”（常量），表明这种数据类型的实例不能被使用它的API函数改变，除此之外，它与LPSTR是等同的。

1.LP表示长指针,在win16下有长指针(LP)和短指针(P)的区别,而在win32下是没有区别的,都是32位.所以这里的LP和P是等价的.

2.C表示const

3.T是什么东西呢,我们知道TCHAR在采用Unicode方式编译时是wchar_t,在普通时编译成char.

　　//P表示这是一个指针 

　　//C表示是一个常量 

　　//T表示在Win32环境中， 有一个_T宏 

　　//STR表示这个变量是一个字符串

为了满足程序代码国际化的需要，业界推出了Unicode标准，它提供了一种简单和一致的表达字符串的方法，所有字符中的字节都是16位的值，其数量也可以满足差不多世界上所有书面语言字符的编码需求，开发程序时使用Unicode（类型为wchar_t)是一种被鼓励的做法。

LPWSTR与LPCWSTR由此产生，它们的含义类似于LPSTR与LPCSTR，只是字符数据是16位的wchar_t而不是char。

然后为了实现两种编码的通用，提出了TCHAR的定义：

如果定义_UNICODE，声明如下：

typedef wchar_t TCHAR;

如果没有定义_UNICODE，则声明如下：

typedef char TCHAR;

LPTSTR和LPCTSTR中的含义就是每个字符是这样的TCHAR。

CString类中的字符就是被声明为TCHAR类型的，它提供了一个封装好的类供用户方便地使用。

LPCTSTR:

      #ifdef _UNICODE

         typedef const wchar_t * LPCTSTR;

      #else

         typedef const char * LPCTSTR;

      #endif

VC常用数据类型使用转换详解

先定义一些常见类型变量借以说明

int i = 100;

long l = 2001;

float f=300.2;

double d=12345.119;

char username[]="女侠程佩君";

char temp[200];

char *buf;

CString str;

_variant_t v1;

_bstr_t v2;

一、其它数据类型转换为字符串

短整型(int)

        itoa(i,temp,10);      //将i转换为字符串放入temp中,最后一个数字表示十进制

        itoa(i,temp,2);       //按二进制方式转换

长整型(long)

        ltoa(l,temp,10);

二、从其它包含字符串的变量中获取指向该字符串的指针

CString变量

str = "2008北京奥运";

buf = (LPSTR)(LPCTSTR)str;

BSTR类型的_variant_t变量

v1 = (_bstr_t)"程序员";

buf = _com_util::ConvertBSTRToString((_bstr_t)v1);

三、字符串转换为其它数据类型

strcpy(temp,"123");

短整型(int)

       i = atoi(temp);

长整型(long)

       l = atol(temp);

浮点(double)

       d = atof(temp);

四、其它数据类型转换到CString

使用CString的成员函数Format来转换,例如:

整数(int)

       str.Format("%d",i);

浮点数(float)

       str.Format("%f",i);

字符串指针(char *)等已经被CString构造函数支持的数据类型可以直接赋值

       str = username;

五、BSTR、_bstr_t与CComBSTR

CComBSTR、_bstr_t是对BSTR的封装,BSTR是指向字符串的32位指针。

char *转换到BSTR可以这样: BSTR b=_com_util::ConvertStringToBSTR("数据");      //使用前需要加上头文件comutil.h

反之可以使用char *p=_com_util::ConvertBSTRToString(b);

六、VARIANT 、_variant_t 与 COleVariant

VARIANT的结构可以参考头文件VC98\Include\OAIDL.H中关于结构体tagVARIANT的定义。

对于VARIANT变量的赋值：首先给vt成员赋值，指明数据类型，再对联合结构中相同数据类型的变量赋值，举个例子：

VARIANT va;

int a=2001;

va.vt=VT_I4;     //指明整型数据

va.lVal=a;       //赋值

对于不马上赋值的VARIANT，最好先用Void VariantInit(VARIANTARG FAR* pvarg);进行初始化,其本质是将vt设置为VT_EMPTY,下表我们列举vt与常用数据的对应关系:

unsigned char bVal; VT_UI1

short iVal; VT_I2

long lVal;   VT_I4  

float fltVal;   VT_R4

double dblVal;   VT_R8  

VARIANT_BOOL boolVal;   VT_BOOL

SCODE scode;   VT_ERROR

CY cyVal;   VT_CY

DATE date;   VT_DATE

BSTR bstrVal;   VT_BSTR

IUnknown FAR* punkVal;   VT_UNKNOWN

IDispatch FAR* pdispVal;   VT_DISPATCH

SAFEARRAY FAR* parray;   VT_ARRAY|*

unsigned char FAR* pbVal;   VT_BYREF|VT_UI1

short FAR* piVal;   VT_BYREF|VT_I2

long FAR* plVal;   VT_BYREF|VT_I4

float FAR* pfltVal;   VT_BYREF|VT_R4

double FAR* pdblVal; VT_BYREF|VT_R8

VARIANT_BOOL FAR* pboolVal; VT_BYREF|VT_BOOL

SCODE FAR* pscode;   VT_BYREF|VT_ERROR

CY FAR* pcyVal;   VT_BYREF|VT_CY

DATE FAR* pdate; VT_BYREF|VT_DATE

BSTR FAR* pbstrVal;   VT_BYREF|VT_BSTR

IUnknown FAR* FAR* ppunkVal;   VT_BYREF|VT_UNKNOWN

IDispatch FAR* FAR* ppdispVal; VT_BYREF|VT_DISPATCH

SAFEARRAY FAR* FAR* pparray;   VT_ARRAY|*

VARIANT FAR* pvarVal;   VT_BYREF|VT_VARIANT

void FAR* byref;   VT_BYREF

_variant_t是VARIANT的封装类，其赋值可以使用强制类型转换，其构造函数会自动处理这些数据类型。

例如：

long l=222;

ing i=100;

_variant_t lVal(l);

lVal = (long)i;

COleVariant的使用与_variant_t的方法基本一样，请参考如下例子：

COleVariant v3 = "字符串", v4 = (long)1999;

CString str =(BSTR)v3.pbstrVal;

long i = v4.lVal;

七、其它

对消息的处理中我们经常需要将WPARAM或LPARAM等32位数据(DWORD)分解成两个16位数据(WORD),例如：

LPARAM lParam;

WORD loValue = LOWORD(lParam);      //取低16位

WORD hiValue = HIWORD(lParam);      //取高16位

对于16位的数据(WORD)我们可以用同样的方法分解成高低两个8位数据(BYTE),例如:

WORD wValue;

BYTE loValue = LOBYTE(wValue);      //取低8位

BYTE hiValue = HIBYTE(wValue);      //取高8位

如何将CString类型的变量赋给char*类型的变量

1、GetBuffer函数：

使用CString::GetBuffer函数。

char *p;

CString str="hello";

p=str.GetBuffer(str.GetLength());

str.ReleaseBuffer();

将CString转换成char * 时

CString str("aaaaaaa");

strcpy(str.GetBuffer(10),"aa");

str.ReleaseBuffer();

当我们需要字符数组时调用GetBuffer(int n),其中n为我们需要的字符数组的长度.使用完成后一定要马上调用ReleaseBuffer();

还有很重要的一点就是,在能使用const char *的地方,就不要使用char *

2、memcpy：

CString mCS=_T("cxl");

char mch[20];

memcpy(mch,mCS,20);

3、用LPCTSTR强制转换： 尽量不使用

char *ch;

CString str;

ch=(LPSTR)(LPCTSTR)str;

CString str = "good";

char *tmp;

sprintf(tmp,"%s",(LPTSTR)(LPCTSTR)str);

4、

CString Msg;

Msg=Msg+"abc";

LPTSTR lpsz;

lpsz = new TCHAR[Msg.GetLength()+1];

_tcscpy(lpsz, Msg);

char * psz;

strcpy(psz,lpsz);

CString类向const char *转换

char a[100];

CString str("aaaaaa");

strncpy(a,(LPCTSTR)str,sizeof(a));

或者如下:

strncpy(a,str,sizeof(a));

以上两种用法都是正确地. 因为strncpy的第二个参数类型为const char *.所以编译器会自动将CString类转换成const char *.

CString转LPCTSTR (const char *)

CString cStr;

const char *lpctStr=(LPCTSTR)cStr;

LPCTSTR转CString

LPCTSTR lpctStr;

CString cStr=lpctStr;

将char*类型的变量赋给CString型的变量

可以直接赋值，如：

CString myString = "This is a test";

也可以利用构造函数，如：

CString s1("Tom");

将CString类型的变量赋给char []类型(字符串)的变量

1、sprintf()函数

CString str = "good";

char tmp[200] ;

sprintf(tmp, "%s",(LPCSTR)str);  

(LPCSTR)str这种强制转换相当于(LPTSTR)(LPCTSTR)str

CString类的变量需要转换为(char*)的时,使用(LPTSTR)(LPCTSTR)str

然而，LPCTSTR是const char *,也就是说，得到的字符串是不可写的！将其强制转换成LPTSTR去掉const，是极为危险的！

一不留神就会完蛋！要得到char *,应该用GetBuffer()或GetBufferSetLength()，用完后再调用ReleaseBuffer()。

2、strcpy()函数

CString str;

char c[256];

strcpy(c, str);

char mychar[1024];

CString source="Hello";

strcpy((char*)&mychar,(LPCTSTR)source);

关于CString的使用

1、指定 CString 形参

     对于大多数需要字符串参数的函数，最好将函数原型中的形参指定为一个指向字符 (LPCTSTR) 而非 CString 的 const 指针。

当将形参指定为指向字符的 const 指针时，可将指针传递到 TCHAR 数组（如字符串 ["hi there"]）或传递到 CString 对象。

CString 对象将自动转换成 LPCTSTR。任何能够使用 LPCTSTR 的地方也能够使用 CString 对象。

2、如果某个形参将不会被修改，则也将该参数指定为常数字符串引用（即 const CString&）。如果函数要修改该字符串，

则删除 const 修饰符。如果需要默认为空值，则将其初始化为空字符串 [""]，如下所示：

void AddCustomer( const CString& name, const CString& address, const CString& comment = "" );

3、对于大多数函数结果，按值返回 CString 对象即可。

串的基本运算

     对于串的基本运算，很多高级语言均提供了相应的运算符或标准的库函数来实现。

为叙述方便，先定义几个相关的变量：

     char s1[20]="dir/bin/appl",s2[20]="file.asm",s3[30],*p;

     int result;

     下面以C语言中串运算介绍串的基本运算

1、求串长

         int strlen(char *s);          //求串s的长度

     【例】printf("%d",strlen(s1));     //输出s1的串长12

2、串复制

     char *strcpy(char *to,*from)；//将from串复制到to串中，并返回to开始处指针

     【例】strcpy(s3,s1);   //s3="dir/bin/appl",s1串不变

3、联接

     char *strcat(char *to,char *from);//将from串复制到to串的末尾，

                                       //并返回to串开始处的指针

     【例】strcat(s3,"/");     //s3="dir/bin/appl/"

          strcat(s3,s2);      //s3="dir/bin/appl/file.asm"

4、串比较

     int strcmp(char *s1,char *s2);//比较s1和s2的大小，

      //当s1<s2、s1>s2和s1=s2时，分别返回小于0、大于0和等于0的值

     【例】result=strcmp("baker","Baker");     //result>0

             result=strcmp("12","12");        //result=0

             result=strcmp("Joe","joseph")    //result<0

5、字符定位

     char *strchr(char *s,char c);//找c在字符串s中第一次出现的位置，

                                  //若找到，则返回该位置，否则返回NULL

     【例】p=strchr(s2,'.');       //p指向"file"之后的位置

if(p) strcpy(p,".cpp");      //s2="file.cpp"

   注意：

     　①上述操作是最基本的，其中后 4个操作还有变种形式：strncpy，strncath和strnchr。

     　②其它的串操作见C的<string.h>。在不同的高级语言中，对串运算的种类及符号都不尽相同

     　③其余的串操作一般可由这些基本操作组合而成     【例】求子串的操作可如下实现：

     void substr(char *sub,char *s,int pos,int len){

          //s和sub是字符数组，用sub返回串s的第pos个字符起长度为len的子串

          //其中0<=pos<=strlen(s)-1,且数组sub至少可容纳len+1个字符。

         if (pos<0||pos>strlen(s)-1||len<0)

             Error("parameter error!");

         strncpy(sub,&s[pos],len);       //从s[pos]起复制至多len个字符到sub

