# 什么是SDK - ljx0305的专栏 - CSDN博客
2008年07月27日 18:48:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：482标签：[api																[windows																[dos																[dll																[编程																[mfc](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=dos&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)
个人分类：[其它](https://blog.csdn.net/ljx0305/article/category/431756)
                 SDK是 Software Development Kit 的缩写，即“软件开发工具包”。
        SDK是一个覆盖面相当广泛的名词，可以这么说：辅助开发某一类软件的相关文档、范例和工具的集合都可以叫做“SDK”。具体到Windows程序开发，我们下面只讨论广义 SDK 的一个子集——即开发Windows 平台下的应用程序所使用的SDK。
        为了解释什么是 SDK 我们不得不引入 API、动态链接库、导入库等等概念。
API ，也就是 Application Programming Interface，即操作系统留给应用程序的一个调用接口， 应用程序通过调用操作系统的API 而使操作系统去执行应用程序的命令。其实早在 DOS 时代就有 API 的概念，只不过那个时候的 API 是以中断调用的形式（INT 21h）提供的，在 DOS 下跑的应用程序都直接或间接的通过中断调用来使用操作系统功能，比如将 AH 置为 30h 后调用 INT 21h 就可以得到 DOS 操作系统的版本号。而在 Windows 中，系统 API 是以函数调用的方式提供的。同样是取得操作系统的版本号，在 Windows 中你所要做的就是调用 GetVersionEx() 函数。可以这么说，DOS API 是“Thinking in 汇编语言”的，而 Windows API 则是“Thinking in 高级语言”的。DOS API 是系统程序的一部分，他们与系统一同被载入内存并且可以通过中断矢量表找到他们的入口，那么 Windows API 呢？要说明白这个问题就不得不引入我们下面要介绍得这个概念——DLL。
        DLL 即 Dynamic Link Library（动态链接库）。我们经常会看到一些 .dll 格式的文件，这些文件就是动态链接库文件，其实也是一种可执行文件格式。跟 .exe 文件不同的是，.dll 文件不能直接执行，他们通常由 .exe 在执行时装入，内含有一些资源以及可执行代码等。其实 Windows 的三大模块就是以 DLL 的形式提供的（Kernel32.dll，User32.dll，GDI32.dll），里面就含有了 API 函数的执行代码。为了使用 DLL 中的 API 函数，我们必须要有 API 函数的声明（.H）和其导入库（.LIB），函数的原型声明不难理解，那么导入库又是做什么用的呢？我们暂时先这样理解：导入库是为了在 DLL 中找到 API 的入口点而使用的。 
        所以，为了使用 API 函数，我们就要有跟 API 所对应的 .H 和 .LIB 文件，而 SDK 正是提供了一整套开发Windows应用程序所需的相关文件、范例和工具的“工具包”。 到此为止，我们才真正的解释清楚SDK 的含义。
        由于 SDK 包含了使用 API 的必需资料，所以人们也常把仅使用 API 来编写 Windows 应用程序的开发方式叫做“SDK 编程”。而 API 和 SDK 是开发 Windows 应用程序所必需的东西，所以其它编程框架和类库都是建立在它们之上的，比如 VCL 和 MFC，虽然他们比起“SDK 编程”来有着更高的抽象度，但这丝毫不妨碍它们在需要的时候随时直接调用 API 函数。
SDK的编程方式就是传统的C的编程方式,
通过调用库函数(在这里是win32API), 来实现程序的搭建.  所以程序比较小, 效率高, 在一些底层的, 和对效率要求很高的场合用.
MFC就是把SDK函数组织成C++的class提供给程序员使用,  接口简单了, 但是有利就有弊, 它的效率根纯SDK的程序相比, 要稍微逊色一些.
但是在实际的开发中, 我们还是经常要在MFC base的程序里边调用API函数的.  所以, 这两者都是需要熟悉地。
引用:http://blog.csdn.net/samehai/archive/2007/02/11/1507302.aspx
Trackback: http://tb.donews.net/TrackBack.aspx?PostId=183788
