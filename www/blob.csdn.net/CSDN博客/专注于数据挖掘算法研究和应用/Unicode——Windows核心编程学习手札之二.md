# Unicode——Windows核心编程学习手札之二 - 专注于数据挖掘算法研究和应用 - CSDN博客





2008年11月24日 15:21:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2226








# Unicode

### ——Windows核心编程学习手札之二

处理软件本地化的核心在于处理不同的字符集。文本串一直作为一系列单字节字符进行编码，并在结尾处放上一个零，当调用strlen函数时，获取以/0结尾的单字节字符数组中返回字符的数目。但各地的文字和书写规则单字节（256个字符）可能不够使用，因此需要双字节字符集（DBCS）的支持。双字节字符集，字符串中的每个字符可以包含一个字节或两个字节，对于程序员来说是个难题。

为此，Unicode标准被提出来。Unicode字符串中的所有字符都是16位的（两个字节）。也就是说固定了双字节来表示一个字符。Unicode用16位值表示每个字符，总共可以得到65000个字符，能够满足对各地不同的文字和书写规则字符所需。Unicode字符集具有：容易在不同语言之间进行数据交换；能够分配支持所有语言的单个二进制EXE文件或DLL文件；提高应用程序的效率。

Windows2000是使用Unicode从头开发进行，用于创建窗口、显示文本、进行字符串操作等的所有核心函数都需要Unicode字符串。如果调用任何一个Windows函数并给它传递一个ANSI字符串，系统首先会先该字符串转换成Unicode，然后将Unicode传递给操作系统。如调用CreateWindowEx函数，并传递类名字和窗口标题文本的非Unicode字符串，那么CreateWindowEx必须分配内存块（在进程默认堆中），将非Unicode字符串转换成Unicode字符串，并将结果存储到分配的内存块中，然后调用Unicode版的CreateWindowEx函数。由于系统必须执行这些转换操作，这些涣涣操作是在你看不见的情况下完成，需要更多的内存，并且运行速度较慢，因此开发基于Windows2000操作系统的应用程序最后从头开始就使用Unicode，可以使应用程序更加有效地运行。

Windows98继承16位Windows操作系统特性，不是用Unicode来处理，不过同样可以处理Unicode字符和字符串，如同Windows2000处理非Unicode字符串一样。如调用CreateWindowEx函数并将Unicode字符串传递给它，就必须明确分配缓存以便执行从Unicode到ANSI字符串的转换操作，当调用CreateWindowEx函数返回时，就释放临时缓存。

Windows2000既支持Unicode也支持ANSI，可以为任意一种开发应用程序；而Windows98只支持ANSI，只能为ANSI开发应用程序。Microsoft在将COM从16位Windows转换成Win32时，将需要字符串的所有COM接口方法改成只能接受Unicode字符串，因为COM通常用于不同组件间进行通信，Unicode是传递字符串最佳手段。Microsoft为Unicode设计了Windows API可以尽量减少对代码的影响。在编写单个源代码文件，可以通过定义宏（UNICODE和_UNICODE）确定使用或不使用Unicode来进行编译。

为了利用Unicode字符串，C运行期库定义了一些数据类型。标准的C头文件String.h中定义了一个Unicode字符数据类性，名字为wchar_t的数据类型：

Typedef unsigned short wchar_t；

如你要创建一个缓存，用于存放最多为99个字符的Unicode字符串和一个结尾为零，可以使用下面语句：

wchar_t szBuffer[100]；

该语句创建了一个由100个16位值组成的数组。标准的C运行期库字符串函数，如strcpy、strchr、strcat等只能对ANSI字符串进行操作，不能正确处理Unicode字符串。因此，ANSI C也设计了一组补充函数用于出来Unicode，如char *strcat(char *,const char*)处理ANSI字符串，对应的Unicode字符串函数是wchar_t *wcscat(wchar_t *,const wchar_t)，一般所有的Unicode函数均以wcs开头。ANSI C规定，C运行期库支持Unicode字符和字符串。若要建立支持ANSI/UNICODE通用代码，需要包含TChar.h文件而非String.h文件。TChar.h包含用在源代码中的一组宏，而不直接调用str函数或wcs函数。如果在编译文件中定义了_UNICODE，这些宏就用wcs组函数，如没有定义，则引用str这组宏。如在TChar.h中有一个宏_tcscpy，如在头文件中没有定义_UNICODE，那_tcscpy就扩展为ANSI的strcpy函数，如定义了，则扩展为Unicode的wcscpy函数。

Windows头文件定义的Unicode数据类型有：WCHAR Unicode字符串；PWSTR 指向Unicode字符串的指针；PCWSTR 指向一个恒定的Unicode字符串的指针。Windows头文件也定义了ANSI和UNICODE通用数据类型PTSTR和PCTSTR，这些数据类型既可以指ANSI字符串，也可以指Unicode字符串，取决于编译程序模块是否定义了UNICODE宏，这里宏和C运行期头文件定义的_UNICODE宏不一样。

Windows还提供一组字符串操作函数，与C运行期字符串函数（如strcpy和wcscpy）相似，但这些函数是操作系统的组成部分，操作系统的许多组件都使用这些函数而不使用C运行期库。建立最好使用操作系统函数而不使用C运行期库字符串函数，这有助于提高应用程序的运行性能，因为操作系统字符串函数常常被大型应用程序如操作系统的外壳进程Explorer.exe所使用，由于这些函数使用多，在你应用程序运行时，它们很可能已经被装入了RAM中。

开发符合ANSI和Unicode的应用程序，需要遵循的原则：将文本串视为字符数组而不是chars数组或字节数组；将通用数据类型（如TCHAR和PTSTR）用于文本字符和字符串；将显示数据类型（如BYTE和PBYTE）用于字节、自己指针和数据缓存；将TEXT宏用于原义字符和字符串；执行全局性替换（例如用PTSTR替换PSTR）；修改字符串运算问题。如果开发包含C运行期库头文件，Unicode宏定义为_UNICODE，如果定义了则编译时如果是通用数据类型和函数则选择执行Unicode函数，同样如果在Windows头文件中定义UNICODE宏处理是一样。要开发出符合ANSI和Unicode的应用程序需要在代码中用通用数据类型和函数来处理字符和字符串，如果在代码中直接使用了ANSI的函数和数据类型，那么就算宏定义了Unicode也将只能处理ANSI字符和字符串，对于Unicode是一样。考虑到利用宏定义就可以实现ANSI和Unicode的应用程序切换。

这里做了个代码测试，在Visual C++6.0开发环境中，建立一个Win32 Console Application，并在Project Settings中的C/C++页Preprocessor definitions里添加UNICODE的宏，同时在主文件中加入头文件#include <windows.h>并执行如下代码观察：

#include "stdafx.h"

#include <windows.h>

int main(int argc, char* argv[])

{

TCHAR ch[]=TEXT("ANSI or Unicode");

int it=sizeof(ch);

int it1=sizeof(TCHAR);

intilen=sizeof(ch)/sizeof(TCHAR);

printf("%d,%d,%d/n",it,it1,ilen);

return 0;

}

可发现it=32，it1=2，ilen=16；回到Project Settings中的C/C++页Preprocessor definitions中取消UNICODE宏，再次执行代码发现it=16，it1=1，ilen=16；这就说明了宏定义对ANSI/UNICODE通用数据类型的取决作用，如果定义了UNICODE宏，那么TCHAR就是char类型，单字节，没有定义UNICODE宏则是wchar_t，双字节。

如非

<?xml:namespace prefix = st1 ns = "urn:schemas-microsoft-com:office:smarttags" />2008-11-25



