# Visual C++常用数据类型转换详解 - 深之JohnChen的专栏 - CSDN博客

2006年03月24日 13:25:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1821标签：[c++																[date																[float																[byte																[ui](https://so.csdn.net/so/search/s.do?q=ui&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=date&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[VC](https://blog.csdn.net/byxdaz/article/category/7211574)


Visual C++常用数据类型转换详解

来源: 天极网

我们先定义一些常见类型变量借以说明：

```
int i = 100;
long l = 2001;
float f=300.2;
double d=12345.119;
char username[]=＂女侠程佩君＂;
char temp[200];
char *buf;
CString str;
_variant_t v1;
_bstr_t v2;
```

　　一、其它数据类型转换为字符串

　　短整型（int）

```
itoa（i,temp,10）;///将i转换为字符串放入temp中,最后一个数字表示十进制
itoa（i,temp,2）; ///按二进制方式转换
```

　　长整型（long）

`ltoa（l,temp,10）;`
　　二、从其它包含字符串的变量中获取指向该字符串的指针

　　CString变量

```
str = ＂2008北京奥运＂;
buf = （LPSTR）（LPCTSTR）str;
```

　　BSTR类型的_variant_t变量

```
v1 = （_bstr_t）＂程序员＂;
buf = _com_util::ConvertBSTRToString（（_bstr_t）v1）;
```

　　三、字符串转换为其它数据类型

`strcpy（temp,＂123＂）; `
　　短整型（int）

`i = atoi（temp）; `
　　长整型（long）

`l = atol（temp）; `
　　浮点（double）

`d = atof（temp）;`
　　四、其它数据类型转换到CString

　　使用CString的成员函数Format来转换,例如:

　　整数（int）

`str.Format（＂%d＂,i）; `
　　浮点数（float）

`str.Format（＂%f＂,i）; `
　　字符串指针（char *）等已经被CString构造函数支持的数据类型可以直接赋值 

`str = username;`
　　五、BSTR、_bstr_t与CComBSTR

　　CComBSTR、_bstr_t是对BSTR的封装,BSTR是指向字符串的32位指针。

char *转换到BSTR可以这样: BSTR b=_com_util::ConvertStringToBSTR（＂数据＂）;///使用前需要加上头文件comutil.h

　　反之可以使用

`char *p=_com_util::ConvertBSTRToString（b）;`
　　六、VARIANT 、_variant_t 与 COleVariant

　　VARIANT的结构可以参考头文件VC98/Include/OAIDL.H中关于结构体tagVARIANT的定义。

　　对于VARIANT变量的赋值：首先给vt成员赋值，指明数据类型，再对联合结构中相同数据类型的变量赋值，举个例子：

```
VARIANT va;
int a=2001;
va.vt=VT_I4;///指明整型数据
va.lVal=a; ///赋值
```

　　对于不马上赋值的VARIANT，最好先用Void VariantInit（VARIANTARG FAR* pvarg）;进行初始化,其本质是将vt设置为VT_EMPTY,下表我们列举vt与常用数据的对应关系:

```
unsigned char bVal; VT_UI1 
short iVal; VT_I2 
long lVal; VT_I4 
float fltVal; VT_R4 
double dblVal; VT_R8 
VARIANT_BOOL boolVal; VT_BOOL 
SCODE scode; VT_ERROR 
CY cyVal; VT_CY 
DATE date; VT_DATE 
BSTR bstrVal; VT_BSTR 
IUnknown FAR* punkVal; VT_UNKNOWN 
IDispatch FAR* pdispVal; VT_DISPATCH 
SAFEARRAY FAR* parray; VT_ARRAY|* 
unsigned char FAR* pbVal; VT_BYREF|VT_UI1 
short FAR* piVal; VT_BYREF|VT_I2 
long FAR* plVal; VT_BYREF|VT_I4 
float FAR* pfltVal; VT_BYREF|VT_R4 
double FAR* pdblVal; VT_BYREF|VT_R8 
VARIANT_BOOL FAR* pboolVal; VT_BYREF|VT_BOOL 
SCODE FAR* pscode; VT_BYREF|VT_ERROR 
CY FAR* pcyVal; VT_BYREF|VT_CY 
DATE FAR* pdate; VT_BYREF|VT_DATE 
BSTR FAR* pbstrVal; VT_BYREF|VT_BSTR 
IUnknown FAR* FAR* ppunkVal; VT_BYREF|VT_UNKNOWN 
IDispatch FAR* FAR* ppdispVal; VT_BYREF|VT_DISPATCH 
SAFEARRAY FAR* FAR* pparray; VT_ARRAY|* 
VARIANT FAR* pvarVal; VT_BYREF|VT_VARIANT 
void FAR* byref; VT_BYREF
```

　　 _variant_t是VARIANT的封装类，其赋值可以使用强制类型转换，其构造函数会自动处理这些数据类型。

　　例如：

```
long l=222;
ing i=100;
_variant_t lVal（l）;
lVal = （long）i;
```

　　COleVariant的使用与_variant_t的方法基本一样，请参考如下例子：

```
COleVariant v3 = ＂字符串＂, v4 = （long）1999;
CString str =（BSTR）v3.pbstrVal;
long i = v4.lVal;
```

　　七、其它

　　对消息的处理中我们经常需要将WPARAM或LPARAM等32位数据（DWORD）分解成两个16位数据（WORD）,例如：

```
LPARAM lParam;
WORD loValue = LOWORD（lParam）;///取低16位
WORD hiValue = HIWORD（lParam）;///取高16位
```

　　对于16位的数据（WORD）我们可以用同样的方法分解成高低两个8位数据（BYTE）,例如:

```
WORD wValue;
BYTE loValue = LOBYTE（wValue）;///取低8位
BYTE hiValue = HIBYTE（wValue）;///取高8位
```


