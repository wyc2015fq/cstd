# wchar_t类型的几个函数 - DoubleLi - 博客园






wchar_t是C/C++的字符数据类型，是一种扩展的字符存储方式。

在Windows下，wchar_t占2个字节（byte）；在[Linux](http://lib.csdn.net/base/linux)下，wchar_t占4个字节

wchar_t类型主要用在国际化程序的实现中，但它不等同于Unicode编码。Unicode编码的字符一般以wchar_t类型存储。

char是8位字符类型，最多只能包含256种字符，许多外文字符集所含的字符数目超过256个，char型无法表示。

标准C++中的wprintf()函数以及iostream类库中的类和对象能提供wchar_t宽字符类型的相关操作。

**函数介绍：**

wcslen()   类似与char*类型作参数的strlen()函数      用来获取wchar_t*变量的长度（不包含终结符）

wcscpy()  类似与char*类型作参数的strcpy()函数      用来进行wchar_t*变量之间的复制

同样的还有  wcsncpy()

wcscmp()  类似与char*类型的strcmp()函数      用来对比两个wchar_t*变量的大小

wmemset()  类似与char*类型的memset()函数     用来初始化内存

wprintf()  类似与char*类型的printf()函数        用来输出wchar_t*字符串

**简单记忆：**

一般把char类型函数中的" str "替换为" wcs "即可

wcs：wide char string









