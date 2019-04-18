# Win32常用的 类型和宏 - gauss的专栏 - CSDN博客
2013年01月08日 22:24:21[gauss](https://me.csdn.net/mathlmx)阅读数：348
Win32常用的 类型和宏
COLORREF
1、结构定义
一个COLORREF 型颜色值是定义了一种颜色的长整数。要求一个颜色参数的GDI函数（比如CreatePen和FloodFill）接受COLORREF值作为参数。
2、结构说明
根据应用程序对COLORREF类型值的不同使用方式，该类型值具有三种不同的形式。它可以定义为下列三种方式之一：
(1)直接定义RGB值
其中低位字节包含了定义红色的相对强度值：第二个字节包含了定义绿色的相对强度值，第三个字节包含了定义蓝色的值，高字节必须为零，单字节的最大值的最大值为FF（十六进制）。下表说明了某些颜色的十六进制值：
----0x000000FF        纯红色
----0x0000FF00        纯绿色
----0x00FF0000        纯蓝色
----0x00000000        黑  色
----0x00FFFFFF       白  色
----0x00808080        淡灰色
RGB宏指令接受红、绿、蓝三种颜色的值，返回一个显示的RGB_COLORREF值。
(2)调色板索引
当指定逻辑调色板的索引时，COLORREF值有如下的16进制形式：0x0100iiii
低位上的两个字节组成了一个16位的整数，它定义了一个逻辑调色板的索引。第三个字节不用，并且必须设置为0，第四个字节（即高位上的字节）必须设置为1。
例如，16进制值0x10000定义了一个索引为0的调色板所代表的颜色；0x010000C定义了一个索引为12的元素所代表的颜色，以此类推。
PALETTEINDEX宏指令接收一个表示逻辑调色板的整数，并返回一个定义为调色板索引的COLORREF值
(3)与调色板相关的RGB
当COLORREF值定义为一个与调色板相关的RGB值时，该值具有如下形式：
0x02bbggrr
与定义为直接的RGB值一样，该值低位上的三个字节分别包含了红色，绿色，蓝色的强度色，其高位上的字节必须为2。
对于支持逻辑调色板的输出设备，Windows将把与调色板相关的RGB值和设备描述表中的逻辑调色板中最接近的颜色匹配，就好象应用程序定义了一个对应于该调色板索引的表项。如果输出设备不支持逻辑调色板，那么Windows就把与调色板相关的RGB值当作直接的RGB值使用。
PALETTERGB宏指令接收表示红色、绿色、蓝色三种颜色的值，并返回一个表示与调色板索引相关的COLORREF值。
3、说明
在将一个表示调色板索引或表示调色板相关的COLORREF值传递给一个需要设备描述表参数的函数之前，使用自身调色板的应用程序必须把调色板放入设备描述表（通过调用RealizePalette函数）中并且使用该调色板成为当前调色板（通过调用SelectPalette函数）,这样做可以保证函数将使用正确的调色板表项的颜色。对于画一个物体的函数（比如CreatePen）,应用程序在为设备描述表选择一个对象之前必须先将调色板选入描述表并使用之成为当前调色板。
4、相关函数
参见CreatePen,RealizePalette,RGB,SelectPalette.
----------------------------------------------------------------------
HIBYTE
1、定义结构
BYTE HIBYTE（nInteger）
此宏指令以nInteger参数所指定的整数值中获取其高位字节
2、参数
nInteger 指定用于转换的值
3、说明
返回值为给定值的高位字节。
HIBYTE宏定义在WINDOWS.H中，如下所示：
#define HIBYTE (w) ((BYTE)((WORD)(W)>>8)&0xFF))
HIWORD
1、结构定义
WORD HIWORD (dwInteger)
此宏指令从dwInteger参数指定的32位整数值中获取其高位字。
2、参数
dwInteger：指定用于转换的值。
3、说明
返回值指定给定的32位整数的高位字。
HIWORD宏定义在WINDOWS.H中，如下所示：
#define HIWORD (i) ((WORD)(((DWORD)(i)>>16)&0xFFFF))
LOBYTE
1、结构定义
BYTE LOBYTE(wVal)
此宏指令从wVal指定的短整数值中抽取其低位字节。
2、参数
wVal：指定用于转换的值。
3、说明
返回值指定给定整数值的低字节。
LOBYTE宏定义于WINDOWS.H中，如下所示：
#define LOBYTE(w)  ((BYTE)(w))
LOWORD
1、结构定义
WORD LOWORD (dwVal)
此宏指令从一个DWORD类型的变量dwVal中抽取齐低位字。
2、参数
(1)dwVal　指定一个用于转换的值
3、说明
返回值指定32全整数值的低位字．
LOWORD宏定义在WINDOWS.H中，如下所示:
#define LOWORD(i) ((WORD)(i))
MAKELONG
1、结构定义
DWORD MAKELONG (wLow,wHing)
WORD wLow;
WORD wHing;
此宏指令将两个WORD类型的变量wLow,wHeight串接起来创建一个无符号长整数。
2、参数
（1）wLow 指定新的长整数的低位字。
（2）wHigh 指定新的长整数的高位字。
3、说明
返回值指定一个无符号长整数。
MAKELONG宏定义在WINDOWS.H中，如下所示：
#define MAKELONG(a,b) ((LONG)(((WORD)(a))|((DWORD)((DWORD)(b)))<<16))
max
1、结构定义
max(value1,value2)
max宏指令比较两个值，返回较大的值。数据类型可以为任意的数字数据类型，有符号的或无符号的。参数的类型和返回值的类型相同。
2、参数
（1）value1：指定第一个值
（2）value2：指定第二个值
3、说明
返回值value1，value2中较大的一个。
max宏定义在WINDOWS.H中，如下：
#define max(a,b) (((a>b))?(a):(b))
4、相关函数
 参见min
min
1、定义结构
min（value1,value2）
min宏指令比较两个值，返回较小的值。数据类型可以为任意的数字数据类型，有符号的或无符号的。参数的类型和返回值的类型相同。
2、参数
（1）value1：指定第一个值
（2）value2：指定第二个值
3、说明
返回值value1，value2中较大的一个。
max宏定义在WINDOWS.H中，如下：
#define min(a,b) (((a<b))?(a):(b))
4、相关函数
 参见max
MAKEINTATOM
1、结构定义
LPSTR MAKEINTATOM（wInteger）
WORD wInteger
此宏指令创建一个代表十进制数字字符串的整数原子。
尽管MAKEINTATOM宏的返回值是LPSTR类型，但不能用做字符串指针，除非将它传递给需要LPSTR参数的原子管理函数。
DeleteAtom函数对整数原子的操作总是成功的，尽管它什么也不做。GetAtomName函数返回的字符串是一个NULL结尾的字符串，其第一个字符为“#”，其余的字符即为MAKEINTATOM宏中使用的WORD。
该宏定义在winbase.h中，如下：
#define MAKEINTATOM(i) (LPSTR)(DWORD)((WORD)(i)))
MAKEINTRESOURCE
1、结构定义
LPSTR MAKEINTRESOURCE(nInteger)
MAKEINTRESOURCE宏将一个整数项转化为一个和Windows资源管理函数兼容的资源类型。该宏一般用来代替一个包含资源名字的字符串。
2、参数
nInteger：指定用于转换的整数值
3、说明
返回值只能作为lpType参数传给Windows资源管理函数。
MAKEINTRESOURCE宏定义于WINDOWS.H中，如下：
#define MAKEINTRESOURCE(i) (LPTSTR)((DWORD)((WORD)(i)))
MAKELPARAM
1、结构定义
LPARAM MAKELONG(wLow,wHigh)
WORD wLow;
WORD wHigh;
此宏指令将两个WORD类型的变量wLow,wHigh串接起来创建一个LPARAM类型的值（长整数），该类型的值用于消息的参数lParam中。
2、参数
（1）wLow：指定新的LPARAM的低位字。
（2）wHigh:指定新的LPARAM的高位字。
3、说明
返回值指定一个LPARAM宏定义在WINUSER.H中，如下所示:
#define MAKELPARAM(l,h) ((LPARAM)MAKELONG(L，H))
MAKELRESULT
1、结构定义
LRESULT MAKELRESULT(wLow,wHigh)
WORD wLow;
WORD wHigh;
此宏指令将两个WORD类型的变量wLow,wHigh串接起来创建一个WPARAM类型的值（长整数），该类型的值用于消息的参数wParam中。
2、参数
（1）wLow：指定新的LRESULT的低位字。
（2）wHigh：指定新的LRESULT的高位字。
3、说明
返回值指定一个LRESULT值。
MAKELRESULT宏定义在WINUSER.H中，如下：
#define MAKELRESULT(l,h) ((LRESULT)MAKELONG(l,h))
MAKEWORD
1、结构定义
WORD MAKEWORD(bLow,bHigh)
BYTE bLow;
BYTE bHigh;
此宏指令将两个BYTE类型的变量bLow，bHihg串接起来创建一个无符号整数。
2、参数
（1）bLow 指定一个新的整数的低位字节
（2）bHigh 指定一个新的整数的高位字节
3、说明
返回值指定一个无符号整数。
MAKEWORD宏定义在WINDOWS.H中，如下：
#define MAKEWORD(a,b) ((WORD)(((BYTE)(a))|((WORD)((BYTE)(b)))<<8))
MAKEWPARAM
1、结构定义
WPARAM MAKEWPARAM(wLow,wHigh)
WORD wLow;
WORD wHigh;
此宏指令将两个WORD类型的变量wLow,wHigh串接起来创建一个WPARAM类型的值（长整数），该类型的值用于消息的参数WParam中。
2、参数
（1）wLow：指定新的WPARAM的低位字。
（2）wHigh:指定新的WPARAM的高位字。
3、说明
返回值指定一个WPARAM宏定义在WINUSER.H中，如下所示:
#define MAKEWPARAM(l,h) ((LPARAM)MAKELONG(L，H))
PALETTEINDEX
1、结构定义
COLORREF PALETTEINDEX(nPaletteIndex)
此宏接受一个逻辑调色板表项的索引作参量，返回一个其高字节为1，低字节为调色板表项索引的值，它称为逻辑调色板表项的索引指定符，使用调色板的应用程序可以传递此指定符，而不必直接指定RGB值。这使得应用程序可以使用此调色板表项中颜色。
2、参数
nPaletteIndex：指定调色板表项的索引，该调色板包含了图形操作要使用的颜色。
3、说明
返回值是一个逻辑调色板索引指示符，当使用此逻辑调色板时，应用程序可以让需要颜色的GDI函数使用此指定符来代替RGB值。
PALETTEINDEX宏定义于WINDOWS.H中，如下
#define PALETTEINDEX(i) ((COLORREF)(0X01000000|(DWORD)(WORD)(i)))
PALETTERGB
1、结构定义
COLORREF PALETTERGB（cRed，cGreen，cBlue）
本宏指令接受三个表示红，绿，蓝相对强度的三个值，然后返回高位为2，低位的三个字节为RGB值的返回值，这称为相对调色板的RGB指定符，使用调色板的应用程序可通过此指定符来代替精确的RGB值以传送给需要颜色的函数。
2、参数
（1）cRed    指定红色强度
（2）cGreen  指定绿色强度
（3）cBlue   指定蓝色强度
3、说明
对支持逻辑调色板的输出设备来说，WINDOWS把调色板相对RGB值在设备描述表中逻辑调色板上最相近的颜色相匹配；如果输出设备不支持系统调色板，那么 WINDOWS就按照调色板相对RGB值RGB宏指令返回的常规RGB DWORD时一样的方式来使用调色板相对RGB值。
返回值指定调色板相对RGB值。
PALETTERGB宏定义于WINGDI.H中，如下：
#define PALETTERGB(r, g, b)  (0x02000000 | RGB(r, g, b))
RGB
1、结构定义
COLORREF RGB(BYTE bRed,BYTE bGreen,BYTE bBlue)
此宏指令在基于由三个参数提供的颜色种类及输出设备拥有的色彩功能这些条件范围内，选择一种RGB颜色。
2、参数
（1）cRed    指定红色强度
（2）cGreen  指定绿色强度
（3）cBlue   指定蓝色强度
3、说明
返回值指定被选中的RGB颜色。每个参数都可以从0取到255。如果三个参数都为0，则返回结果为黑色；若桑饿参数都为255，则返回结果为白色。关于调色板中使用的颜色值的具体细节介绍可参见宏PALETTEINDEX和PALETTERGB。
RGB宏定义在wingdi.h中，如下：
#define RGB(r, g ,b)  ((DWORD) (((BYTE) (r) | /
    ((WORD) (g) << 8)) | /
    (((DWORD) (BYTE) (b)) << 16)))
[UNREFERENCED_PARAMETER的作用](http://blog.csdn.net/li_guotao/archive/2008/03/31/2234325.aspx)
**UNREFERENCED_PARAMETER** 的作用
2007年06月16日 星期六 14:38
我们从 UNREFERENCED_PARAMETER 开始吧。这个宏在 winnt.h 中定义如下：
#define UNREFERENCED_PARAMETER(P) (P)
　 　换句话说 UNREFERENCED_PARAMETER 展开传递的参数或表达式。其目的是避免编译器关于未引用参数的警告。许多程序员，包括我在内，喜欢用最高级别的警告 Level 4（/W4）进行编译。Level 4 属于“能被安全忽略的事件”的范畴。虽然它们可能使你难堪，但很少破坏你的代码。例如，在你的程序中可能会有这样一些代码行：
int x=1;
　　但你从没用到过 x。也许这一行是你以前使用 x 时留下来的，只删除了使用它的代码，而忘了删除这个变量。Warning Level 4 能找到这些小麻烦。所以，为什么不让编译器帮助你完成可能是最高级别的专业化呢？用Level 4 编译是展示你工作态度的一种方式。如果你为公众使用者编写库，Level 4 则是社交礼节上需要的。你不想强迫你的开发人员使用低级选项清洁地编译他们的代码。
　　问题是，Level 4 实在是太过于注意细节，在 Level 4 上，编译器连未引用参数这样无伤大雅的事情也要抱怨（当然，除非你真的有意使用这个参数，这时便相安无事）。假设你有一个函数带来两个参数，但你只使用其中一个：
int SomeFunction(int arg1, int arg2){     return arg1+5;}
使用 /W4，编译器抱怨：
“warning C4100: ''arg2'' : unreferenced formal parameter.”
为了骗过编译器，你可以加上 UNREFERENCED_PARAMETER(arg2)。现在编译器在编译你的引用 arg2 的函数时便会住口。并且由于语句：
arg2;
实际上不做任何事情，编译器不会为之产生任何代码，所以在空间和性能上不会有任何损失。
　　细心的人可能会问：既然你不使用 arg2，那当初为何要声明它呢？通常是因为你实现某个函数以满足某些API固有的署名需要，例如，MFC的 OnSize 处理例程的署名必须要像下面这样：
void OnSize(UINT nType, int cx, int cy);
　　这里 cx/cy 是窗口新的宽/高，nType 是一个类似 SIZE_MAXIMIZED 或 SIZE_RESTORED 这样的编码，表示窗口是否最大化或是常规大小。一般你不会在意 nType，只会关注 cx 和 xy。所以如果你想用 /W4，则必须使用 UNREFERENCED_PARAMETER(nType)。OnSize 只是上千个 MFC 和 Windows 函数之一。编写一个基于 Windows 的程序，几乎不可能不碰到未引用参数。
　　说了这么多关于 UNREFERENCED_PARAMETER 内容。Judy 在她的问题中还提到了另一个 C++ 程序员常用的并且其作用与 UNREFERENCED_PARAMETER 相同的诀窍，那就是注释函数署名中的参数名：
void CMyWnd::OnSize(UINT , int cx, int cy){}
　　现在 nType 是未命名参数，其效果就像你敲入 OnSize(UINT, int cx, int cy)一样。那么现在的关键问题是：你应该使用哪种方法——未命名参数，还是 UNREFERENCED_PARAMETER？
　 　大多数情况下，两者没什么区别，使用哪一个纯粹是风格问题。（你喜欢你的 java 咖啡是黑色还是奶油的颜色？）但我认为至少有一种情况必须使用 UNREFERENCED_PARAMETER。假设你决定窗口不允许最大化。那么你便禁用 Maximize 按钮，从系统菜单中删除，同时阻止每一个用户能够最大化窗口的操作。因为你是偏执狂（大多数好的程序员都是偏执狂），你添加一个 ASSERT （断言）以确保代码按照你的意图运行：
void CMyWnd::OnSize(UINT nType, int cx, int cy){     ASSERT(nType != SIZE_MAXIMIZE);     ... // use cx, cy}
　 　质检团队竭尽所能以各种方式运行你的程序，ASSERT 从没有弹出过，于是你认为编译生成 Release 版本是安全的。但是此时 _DEBUG 定义没有了，ASSERT(nType != SIZE_MAXIMIZE)展开为 ((void)0)，并且 nType 一下子成了一个未引用参数！这样进入你干净的编译。你无法注释掉参数表中的 nType，因为你要在 ASSERT 中使用它。于是在这种情况下——你唯一使用参数的地方是在 ASSERT 中或其它 _DEBUG 条件代码中——只有 UNREFERENCED_PARAMETER
 会保持编译器在 Debug 和 Release 生成模式下都没有问题。知道了吗？
　　结束讨论之前，我想还有一个问题我没有提及，就是你可以象下面这样用 pragma 指令抑制单一的编译器警告：
#pragma warning( disable : 4100 )
4100 是未引用参数的出错代码。pragma 抑制其余文件/模块的该警告。用下面方法可以重新启用这个警告：
#pragma warning( default : 4100 )
　　不管怎样，较好的方法是在禁用特定的警告之前保存所有的警告状态，然后，等你做完之后再回到以前的配置。那样，你便回到的以前的状态，这个状态不一定是编译器的默认状态。
　　所以你能象下面这样在代码的前后用 pragma 指令抑制单个函数的未引用参数警告：
#pragma warning( push ) #pragma warning( disable : 4100 )void SomeFunction(...){}#pragma warning( pop )
　 　当然，对于未引用参数而言，这种方法未免冗长，但对于其它类型的警告来说可能就不是这样了。库生成者都是用 #pragma warning 来阻塞警告，这样他们的代码可以用 /W4 进行清洁编译。MFC 中充满了这样的 pragmas 指令。还有好多的 #pragma warning 选项我没有在本文讨论。有关它们的信息请参考相关文档。
