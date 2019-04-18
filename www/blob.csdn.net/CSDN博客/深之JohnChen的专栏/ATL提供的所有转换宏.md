# ATL提供的所有转换宏 - 深之JohnChen的专栏 - CSDN博客

2017年05月05日 13:19:24[byxdaz](https://me.csdn.net/byxdaz)阅读数：673


在头文件<atlconv.h>中定义了ATL提供的所有转换宏，如：

A2CW(LPCSTR)-> (LPCWSTR)

A2W(LPCSTR)-> (LPWSTR)

W2CA(LPCWSTR) -> (LPCSTR)

W2A(LPCWSTR) -> (LPSTR)

所有的宏如下表所示：
|A2BSTR|OLE2A|T2A|W2A|
|----|----|----|----|
|A2COLE|OLE2BSTR|T2BSTR|W2BSTR|
|A2CT|OLE2CA|T2CA|W2CA|
|A2CW|OLE2CT|T2COLE|W2COLE|
|A2OLE|OLE2CW|T2CW|W2CT|
|A2T|OLE2T|T2OLE|W2OLE|
|A2W|OLE2W|T2W|W2T|

上表中的宏函数，非常的有规律，每个字母都有确切的含义如下：
|2|to 的发音和 2一样，所以借用来表示“转换为、转换到”的含义。|
|----|----|
|A|ANSI 字符串，也就是 MBCS。|
|W、OLE|宽字符串，也就是 UNICODE。|
|T|中间类型T。如果定义了 _UNICODE，则T表示W；如果定义了 _MBCS，则T表示A|
|C|const 的缩写|

利用这些宏，可以快速的进行各种字符间的转换。使用前必须包含头文件，并且申明USER_CONVERSION；

使用 ATL
转换宏，由于不用释放临时空间，所以使用起来非常方便。

但是考虑到栈空间的尺寸（VC
默认2M），使用时要注意几点：

    1、只适合于进行短字符串的转换；

    2、不要试图在一个次数比较多的循环体内进行转换；

    3、不要试图对字符型文件内容进行转换，因为文件尺寸一般情况下是比较大的；

    4、对情况 2
和 3，要使用
 MultiByteToWideChar() 和 WideCharToMultiByte()；

