# MultiByteToWideChar的与WideCharToMultiByte的参数详解  - ljx0305的专栏 - CSDN博客
2009年11月07日 21:54:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：814标签：[null																[translation																[windows																[thread																[制造](https://so.csdn.net/so/search/s.do?q=制造&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=translation&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[VC](https://blog.csdn.net/ljx0305/article/category/401790)
# [MultiByteToWideChar的与WideCharToMultiByte的参数详解](http://www.cnblogs.com/wanghao111/archive/2009/05/25/1489021.html)
第一个就是宽字符到多字节字符转换函数，函数原型如下： 
```
Code
int WideCharToMultiByte( 
UINT CodePage, 
DWORD dwFlags, 
LPCWSTR lpWideCharStr, 
int cchWideChar, 
LPSTR lpMultiByteStr, 
int cbMultiByte, 
LPCSTR lpDefaultChar, 
LPBOOL lpUsedDefaultChar 
);
```
此函数把宽字符串转换成指定的新的字符串，如ANSI，UTF8等，新字符串不必是多字节字符集。参数： 
```
Code
CodePage： 指定要转换成的字符集代码页，它可以是任何已经安装的或系统自带的字符集，你也可以使用如下所示代码页之一。 
CP_ACP 当前系统ANSI代码页 
CP_MACCP 当前系统Macintosh代码页 
CP_OEMCP 当前系统OEM代码页，一种原始设备制造商硬件扫描码 
CP_SYMBOL Symbol代码页，用于Windows 2000及以后版本，我不明白是什么 
CP_THREAD_ACP 当前线程ANSI代码页，用于Windows 2000及以后版本，我不明白是什么 
CP_UTF7 UTF-7，设置此值时lpDefaultChar和lpUsedDefaultChar都必须为NULL 
CP_UTF8 UTF-8，设置此值时lpDefaultChar和lpUsedDefaultChar都必须为NULL
```
我想最常用的应该是CP_ACP和CP_UTF8了，前者将宽字符转换为ANSI，后者转换为UTF8。 
```
Code
dwFlags： 指定如何处理没有转换的字符， 但不设此参数函数会运行的更快一些，我都是把它设为0。 可设的值如下表所示： 
WC_NO_BEST_FIT_CHARS 把不能直接转换成相应多字节字符的Unicode字符转换成lpDefaultChar指定的默认字符。也就是说，如果把Unicode转换成多字节字符，然后再转换回来，你并不一定得到相同的Unicode字符，因为这期间可能使用了默认字符。此选项可以单独使用，也可以和其他选项一起使用。 
WC_COMPOSITECHECK 把合成字符转换成预制的字符。它可以与后三个选项中的任何一个组合使用，如果没有与他们中的任何一个组合，则与选项WC_SEPCHARS相同。 
WC_ERR_INVALID_CHARS 此选项会致使函数遇到无效字符时失败返回，并且GetLastError会返回错误码ERROR_NO_UNICODE_TRANSLATION。否则函数会自动丢弃非法字符。此选项只能用于UTF8。 
WC_DISCARDNS 转换时丢弃不占空间的字符，与WC_COMPOSITECHECK一起使用 
WC_SEPCHARS 转换时产生单独的字符，此是默认转换选项，与WC_COMPOSITECHECK一起使用 
WC_DEFAULTCHAR 转换时使用默认字符代替例外的字符，(最常见的如’?’)，与WC_COMPOSITECHECK一起使用。
```
当指定WC_COMPOSITECHECK时，函数会将合成字符转换成预制字符。合成字符由一个基字符和一个不占空间的字符(如欧洲国家及汉语拼音的音标)组成，每一个都有不同的字符值。预制字符有一个用于表示基字符和不占空间字符的合成体的单一的字符值。 
当指定WC_COMPOSITECHECK选项时，也可以使用上表列出的最后3个选项来定制预制字符的转换规则。这些选项决定了函数在遇到宽字符串的合成字符没有对应的预制字符时的行为，他们与WC_COMPOSITECHECK一起使用，如果都没有指定，函数默认WC_SEPCHARS。 
对于下列代码页，dwFlags必须为0，否则函数返回错误码ERROR_INVALID_FLAGS。 
50220 50221 50222 50225 50227 50229 52936 54936 57002到57011 65000(UTF7) 42(Symbol) 
对于UTF8，dwFlags必须为0或WC_ERR_INVALID_CHARS，否则函数都将失败返回并设置错误码ERROR_INVALID_FLAGS，你可以调用GetLastError获得。 
```
Code
lpWideCharStr： 待转换的宽字符串。 
cchWideChar： 待转换宽字符串的长度，-1表示转换到字符串结尾。 
lpMultiByteStr： 接收转换后输出新串的缓冲区。 
cbMultiByte： 输出缓冲区大小，如果为0，lpMultiByteStr将被忽略，函数将返回所需缓冲区大小而不使用lpMultiByteStr。 
lpDefaultChar： 指向字符的指针， 在指定编码里找不到相应字符时使用此字符作为默认字符代替。 如果为NULL则使用系统默认字符。对于要求此参数为NULL的dwFlags而使用此参数，函数将失败返回并设置错误码ERROR_INVALID_PARAMETER。 
lpUsedDefaultChar：开关变量的指针，用以表明是否使用过默认字符。对于要求此参数为NULL的dwFlags而使用此参数，函数将失败返回并设置错误码ERROR_INVALID_PARAMETER。lpDefaultChar和lpUsedDefaultChar都设为NULL，函数会更快一些。 
返回值： 如果函数成功，且cbMultiByte非0，返回写入lpMultiByteStr的字节数(包括字符串结尾的null)；cbMultiByte为0，则返回转换所需 
字节数。函数失败，返回0。
```
注意：函数WideCharToMultiByte使用不当，会给影响程序的安全。调用此函数会很容易导致内存泄漏，因为lpWideCharStr指向的输入缓冲区大小是宽字符数，而lpMultiByteStr指向的输出缓冲区大小是字节数。为了避免内存泄漏，应确保为输出缓冲区指定合适的大小。我的方法是先使cbMultiByte为0调用WideCharToMultiByte一次以获得所需缓冲区大小，为缓冲区分配空间，然后再次调用WideCharToMultiByte填充缓冲区，详见下面的代码。另外，从Unicode UTF16向非Unicode字符集转换可能会导致数据丢失，因为该字符集可能无法找到表示特定Unicode数据的字符。 
```
Code
wchar_t* pwszUnicode = "Holle, word! 你好，中国！ "; 
int iSize; 
char* pszMultiByte; 
iSize = WideCharToMultiByte(CP_ACP, 0, pwszUnicode, -1, NULL, 0, NULL, NULL); 
pszMultiByte = (char*)malloc((iSize+1)/**sizeof(char)*/); 
WideCharToMultiByte(CP_ACP, 0, pwszUnicode, -1, pszMultiByte, iSize, NULL, NULL);
```
第二个是多字节字符到宽字符转换函数，函数原型如下： 
```
int MultiByteToWideChar( 
UINT CodePage, 
DWORD dwFlags, 
LPCSTR lpMultiByteStr, 
int cbMultiByte, 
LPWSTR lpWideCharStr, 
int cchWideChar 
);
```
此函数把多字节字符串转换成宽字符串(Unicode)，待转换的字符串并不一定是多字节的。 
此函数的参数，返回值及注意事项参见上面函数WideCharToMultiByte的说明，这里只对dwFlags做简单解释。 
```
Code
dwFlags： 指定是否转换成预制字符或合成的宽字符，对控制字符是否使用像形文字，以及怎样处理无效字符。 
MB_PRECOMPOSED 总是使用预制字符，即有单个预制字符时，就不会使用分解的基字符和不占空间字符。此为函数的默认选项，不能和MB_COMPOSITE合用 
MB_COMPOSITE 总是使用分解字符，即总是使用基字符+不占空间字符的方式 
MB_ERR_INVALID_CHARS 设置此选项，函数遇到非法字符就失败并返回错误码ERROR_NO_UNICODE_TRANSLATION，否则丢弃非法字符 
MB_USEGLYPHCHARS 使用像形字符代替控制字符
```
对于下列代码页，dwFlags必须为0，否则函数返回错误码ERROR_INVALID_FLAGS。 
50220 50221 50222 50225 50227 50229 52936 54936 57002到57011 65000(UTF7) 42(Symbol) 
对于UTF8，dwFlags必须为0或MB_ERR_INVALID_CHARS，否则函数都将失败并返回错误码ERROR_INVALID_FLAGS。 
以下函数我没用过，只简要说明之。 
int GetTextCharset( HDC hdc ); 
此函数获取当前选进的设备描述表的字符集，等同于GetTextCharsetInfo(hdc, NULL, 0)。 
返回值： 成功返回字符集标识，失败返回DEFAULT_CHARSET。
