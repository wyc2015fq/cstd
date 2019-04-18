# VC下Debug版和Release版的区别 - 长歌行 - CSDN博客





2012年12月31日 14:31:26[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：724
个人分类：[C、C++](https://blog.csdn.net/iuhsihsow/article/category/1072760)










经常在 CSDN 上看见有人问 Debug 运行正常但 Release 失败的问题。以往的讨论往往是经验性的，并没有指出会这样的真正原因是什么，要想找出真正的原因通常要凭运气。最近我看了一些这方面的书，又参考了 CSDN 上的一些帖子，然后深入研究了一下关于二者的不同。以下是我的一些体会，拿来与大家共享。    

本文主要包含如下内容： 

1. Debug 和 Release 编译方式的本质区别 

2. 哪些情况下 Release 版会出错 

3. 怎样"调试" Release 版的程序 


关于Debug和Release之本质区别的讨论 


一、Debug 和 Release 编译方式的本质区别 


Debug 通常称为调试版本，它包含调试信息，并且不作任何优化，便于程序员调试程序。Release 称为发布版本，它往往是进行了各种优化，使得程序在代码大小和运行速度上都是最优的，以便用户很好地使用。 Debug 和 Release 的真正秘密，在于一组编译选项。下面列出了分别针对二者的选项（当然除此之外还有其他一些，如/Fd /Fo，但区别并不重要，通常他们也不会引起 Release 版错误，在此不讨论） 

Debug 版本： 

/MDd /MLd 或 /MTd 使用 Debug runtime library(调试版本的运行时刻函数库) /Od 关闭优化开关 /D "_DEBUG" 相当于 #define _DEBUG,打开编译调试代码开关(主要针对 assert函数) /ZI 创建 Edit and continue(编辑继续)数据库，这样在调试过 程中如果修改了源代码不需重新编译 /GZ 可以帮助捕获内存错误 /Gm 打开最小化重链接开关，减少链接时间 

Release 版本： 

/MD /ML 或 /MT 使用发布版本的运行时刻函数库 /O1 或 /O2 优化开关，使程序最小或最快 /D "NDEBUG" 关闭条件编译调试代码开关(即不编译assert函数) /GF 合并重复的字符串，并将字符串常量放到只读内存，防止 被修改实际上，Debug 和 Release 并没有本质的界限，他们只是一组编译选项的集合，编译器只是按照预定的选项行动。事实上，我们甚至可以修改这些选项，从而得到优化过的调试版本或是带跟踪语句的发布版本。 


二、哪些情况下 Release 版会出错 


有了上面的介绍，我们再来逐个对照这些选项看看 Release 版错误是怎样产生的 


1. Runtime Library： 

链接哪种运行时刻函数库通常只对程序的性能产生影响。调试版本的 Runtime Library 包含了调试信息，并采用了一些保护机制以帮助发现错误，因此性能不如发布版本。编译器提供的 Runtime Library 通常很稳定，不会造成 Release 版错误；倒是由于 Debug 的 Runtime Library 加强了对错误的检测，如堆内存分配，有时会出现 Debug 有错但 Release 正常的现象。应当指出的是，如果 Debug 有错，即使 Release 正常，程序肯定是有 Bug 的，只不过可能是
 Release 版的某次运行没有表现出来而已。 


2. 优化： 

这是造成错误的主要原因，因为关闭优化时源程序基本上是直接翻译的，而打开优化后编译器会作出一系列假设。这类错误主要有以下几种： 


(1) 帧指针(Frame Pointer)省略（简称 FPO ）： 

在函数调用过程中，所有调用信息（返回地址、参数）以及自动变量都是放在栈中的。若函数的声明与实现不同（参数、返回值、调用方式），就会产生错误 ————但 Debug 方式下，栈的访问通过 EBP 寄存器保存的地址实现，如果没有发生数组越界之类的错误（或是越界"不多"），函数通常能正常执行；Release 方式下，优化会省略 EBP 栈基址指针，这样通过一个全局指针访问栈就会造成返回地址错误是程序崩溃。C++ 的强类型特性能检查出大多数这样的错误，但如果用了强制类型转换，就不行了。你可以在 Release 版本中强制加入
 /Oy- 编译选项来关掉帧指针省略，以确定是否此类错误。此类错误通常有： 

● MFC 消息响应函数书写错误。正确的应为 afx_msg LRESULT OnMessageOwn(WPARAM wparam, LPARAM lparam); ON_MESSAGE 宏包含强制类型转换。防止这种错误的方法之一是重定义ON_MESSAGE 宏，把下列代码加到 stdafx.h 中（在#include "afxwin.h"之后）,函数原形错误时编译会报错 #undef ON_MESSAGE #define ON_MESSAGE(message, memberFxn) \ { message,
 0, 0, 0, AfxSig_lwl, \ (AFX_PMSG)(AFX_PMSGW)(static_cast< LRESULT (AFX_MSG_CALL \ CWnd::*)(WPARAM, LPARAM) > (&memberFxn) }, 


(2) volatile 型变量： 

volatile 告诉编译器该变量可能被程序之外的未知方式修改（如系统、其他进程和线程）。优化程序为了使程序性能提高，常把一些变量放在寄存器中（类似于 register 关键字），而其他进程只能对该变量所在的内存进行修改，而寄存器中的值没变。如果你的程序是多线程的，或者你发现某个变量的值与预期的不符而你确信已正确的设置了，则很可能遇到这样的问题。这种错误有时会表现为程序在最快优化出错而最小优化正常。把你认为可疑的变量加上 volatile 试试。 


(3) 变量优化： 

优化程序会根据变量的使用情况优化变量。例如，函数中有一个未被使用的变量，在 Debug 版中它有可能掩盖一个数组越界，而在 Release 版中，这个变量很可能被优化调，此时数组越界会破坏栈中有用的数据。当然，实际的情况会比这复杂得多。与此有关的错误有： ● 非法访问，包括数组越界、指针错误等。例如 void fn(void) { int i; i = 1; int a[4]; { int j; j = 1; } a[-1] = 1;//当然错误不会这么明显，例如下标是变量 a[4] = 1; } j 虽然在数组越界时已出了作用域，但其空间并未收回，因而
 i 和 j 就会掩盖越界。而 Release 版由于 i、j 并未其很大作用可能会被优化掉，从而使栈被破坏。 


3. _DEBUG 与 NDEBUG ： 

当定义了 _DEBUG 时，assert() 函数会被编译，而 NDEBUG 时不被编译。除此之外，VC++中还有一系列断言宏。这包括： ANSI C 断言 void assert(int expression_r ); C Runtime Lib 断言 _ASSERT( booleanexpression_r ); _ASSERTE( booleanexpression_r ); MFC 断言 ASSERT( booleanexpression_r ); VERIFY( booleanexpression_r
 ); ASSERT_VALID( pObject ); ASSERT_KINDOF( classname, pobject ); ATL 断言 ATLASSERT( booleanexpression_r ); 此外，TRACE() 宏的编译也受 _DEBUG 控制。 所有这些断言都只在 Debug版中才被编译，而在 Release 版中被忽略。唯一的例外是 VERIFY() 。事实上，这些宏都是调用了 assert() 函数，只不过附加了一些与库有关的调试代码。如果你在这些宏中加入了任何程序代码，而不只是布尔表达式（例如赋值、能改变变量值的函数调用等），那么
 Release 版都不会执行这些操作，从而造成错误。初学者很容易犯这类错误，查找的方法也很简单，因为这些宏都已在上面列出，只要利用 VC++ 的 Find in Files 功能在工程所有文件中找到用这些宏的地方再一一检查即可。另外，有些高手可能还会加入 #ifdef _DEBUG 之类的条件编译，也要注意一下。 顺便值得一提的是 VERIFY() 宏，这个宏允许你将程序代码放在布尔表达式里。这个宏通常用来检查 Windows API 的返回值。有些人可能为这个原因而滥用 VERIFY() ，事实上这是危险的，因为
 VERIFY() 违反了断言的思想，不能使程序代码和调试代码完全分离，最终可能会带来很多麻烦。因此，专家们建议尽量少用这个宏。 


4. /GZ 选项：这个选项会做以下这些事 


(1) 初始化内存和变量。包括用 0xCC 初始化所有自动变量，0xCD ( Cleared Data ) 初始化堆中分配的内存（即动态分配的内存，例如 new ），0xDD ( Dead Data ) 填充已被释放的堆内存（例如 delete ），0xFD( deFencde Data ) 初始化受保护的内存（debug 版在动态分配内存的前后加入保护内存以防止越界访问），其中括号中的词是微软建议的助记词。这样做的好处是这些值都很大，作为指针是不可能的（而且 32 位系统中指针很少是奇数值，在有些系统中奇数的指针会产生运行时错误），作为数值也很少遇到，而且这些值也很容易辨认，因此这很有利于在
 Debug 版中发现 Release 版才会遇到的错误。要特别注意的是，很多人认为编译器会用 0 来初始化变量，这是错误的（而且这样很不利于查找错误）。 


(2) 通过函数指针调用函数时，会通过检查栈指针验证函数调用的匹配性。（防止原形不匹配） 


(3) 函数返回前检查栈指针，确认未被修改。（防止越界访问和原形不匹配，与第二项合在一起可大致模拟帧指针省略 FPO ） 通常 /GZ 选项会造成 Debug 版出错而 Release 版正常的现象，因为 Release 版中未初始化的变量是随机的，这有可能使指针指向一个有效地址而掩盖了非法访问。 除此之外，/Gm /GF 等选项造成错误的情况比较少，而且他们的效果显而易见，比较容易发现。 


三、怎样"调试" Release 版的程序 


遇到 Debug 成功但 Release 失败，显然是一件很沮丧的事，而且往往无从下手。如果你看了以上的分析，结合错误的具体表现，很快找出了错误，固然很好。但如果一时找不出，以下给出了一些在这种情况下的策略。 


1. 前面已经提过，Debug 和 Release 只是一组编译选项的差别，实际上并没有什么定义能区分二者。我们可以修改 Release 版的编译选项来缩小错误范围。如上所述，可以把 Release 的选项逐个注：那篇文章到此就完了，好像还有一些没了。 


在VC中当整个工程较大时，软件时常为出现在DEBUG状态下能运行而在RELEASE状态下无法运行的情况。由于开发者通常在DEBUG状态下开发软件，所以这种情况时常是在我们辛苦工作一两个月后，满怀信心的准备将软件发行时发生。为了避免无谓的损失，我们最好进行以下的检查: 1、时常测试软件的两种版本。 


2、不要轻易将问题归结为DEBUG/RELEASE问题，除非你已经充分对两种版本进行了测试。 


3、预处理的不同，也有可能引起这样的问题。出现问题的一种可能性是在不同版本的编译间定义了不同的预处理标记。请对你的DEBUG版本的软件试一下以下改动： 在"Project Setting(ALT-F7)" 中的C/C++项中设置目录(category)为"General"，并且改动"_DEBUG"定义为"NDEBUG". 设置目录为"Preprocessor"并且添加定义"_DEBUG到"Undefined Symbols"输入框. 选择Rebuild ALL,重新编译. 如果经过编译的程序产生了问题，请对代码进行如下改动：
 将ASSERT() 改为 VERIFY(). 找出定义在"#ifdef _DEBUG"中的代码，如果在RELEASE版本中需要这些代码请将他们移到定义外。查找TRACE(...)中代码，因为这些代码在RELEASE中也不被编译。所以请认真检查那些在RELEASE中需要的代码是否并没有被便宜。 


4、变量的初始化所带来的不同，在不同的系统，或是在DEBUG/RELEASE版本间都存在这样的差异，所以请对变量进行初始化。 


5、是否在编译时已经有了警告?请将警告级别设置为3或4,然后保证在编译时没有警告出现. 


6、是否改动了资源文件. 


7、此外对RELEASE版本的软件也可以进行调试，请做如下改动： 在"Project Settings" 中 "C++/C " 项目下设置 "category" 为 "General" 并且将"Debug Info"设置为 "Program Database". 在"Link"项目下选中"Generate Debug Info"检查框。 "Rebuild All" 如此做法会产生的一些限制：无法获得在MFC DLL中的变量的值。 必须对该软件所使用的所有DLL工程都进行改动。 

另： MS BUG：MS的一份技术文档中表明，在VC5中对于DLL的"Maximize Speed"优化选项并未被完全支持，因此这将会引起内存错误并导致程序崩溃。










DEBUG版本包含调试信息，编译器生成的代码方便调试
RELEASE版本算是发布版本，RELEASE下不能调试，生成的文件更小，编译器生成的程序速度更快
说到这个问题，先得说说VC对Debug和Release的定义。
　　实际上，Debug和Release只是VC对编译选项的预定义而已，如果我们愿意，我们完全可以把Debug和Release的行为完全颠倒过来。
　　当然，我们也可以自己定义一组编译选项，然后命名为ABC，DEF等。当然，在习惯上，我们仍然更愿意使用VC已经定义好的名称。
　　既然Debug和Release仅仅是编译选项的不同，那么为什么要区分Debug和Release版本呢？
　　Debug和Release，在我看来主要是针对其面向的目标不同的而进行区分的。
　　Debug通常称为调试版本，通过一系列编译选项的配合，编译的结果通常包含调试信息，而且不做任何优化，以为开发人员提供强大的应用程序调试能力。
　　而Release通常称为发布版本，是为用户使用的，一般客户不允许在发布版本上进行调试。所以不保存调试信息，同时，它往往进行了各种优化，以期达到代码最小和速度最优。为用户的使用提供便利。
　　下面仅就默认的Debug和Release版本的选项进行比较，详细的编译选项可以看MSDN的说明。
　　我们将默认的Debug和Release的选项设置进行比较，过滤掉相同设置，主要的不同如下：
　　编译选项：/Od /D "_DEBUG" /Gm /RTC1 /MDd /Fo"Debug\\" /ZI
　　链接选项：/OUT:"D:\MyProject\logging\Debug\OptionTest.dll" /INCREMENTAL
　　默认的Release设置如下：
　　编译选项：/O2 /GL /D "NDEBUG" /FD /MD /Fo"Release\\" /Zi
　　链接选项：/OUT:"D:\MyProject\logging\Release\OptionTest.dll" /INCREMENTAL:NO
　　MDd
　　与
　　MD
　　首先，Debug版本使用调试版本的运行时库（/MDd选项），Relase版本则使用的是发布版本的运行时库（vcrt.dll）。其区别主要在于运行时的性能影响。调试版本的运行时库包含了调试信息，并采用了一些保护机制以帮助发现错误，也因此，其性能不如发布版本。编译器提供的Runtime Library很稳定，不会造成Release版本错误，倒是由于Debug版本的Runtime Library加强了对错误的检测，如堆内存分配检查等，反而会报告错误，应当指出，如果Debug有错误，而Release版本正常，程序肯定是有Bug的，只是我们还没有发现。
　　ZI
　　与
　　Zi
　　其次，/ZI选项与/Zi选项。通过使用/ZI选项，可以在调试过程修改代码而不需要重新编译。这是个调试的好帮手，可如果我们使用Release版本，这将变得不可行。
　　Od
　　与
　　O2
　　/O2与/Od选项：Od是关闭编译器优化，普遍用于Debug版本。而O2选项是创建最快速代码，这当然是Release版本的不二选择。
　　RTCx
　　选项
　　/RTCx选项，这个选项比较强，它可以让编译器插入动态检测代码以帮助你检测程序中的错误。比如，它会将局部变量初始化为非零值。包括用0xCC初始化所有自动变量，0xCD初始化堆中分配的内存（即new的内存），使用0xDD填充被释放的内存（即delete的内存），0xFD初始化受保护的内存（debug版在动态分配内存的前后加入保护内存以防止越界访问）。这样做的好处是这些值都很大，一般不可能作为指针，青年人网提示作为数值也很少用到，而且这些值很容易辩认，因此有利于在Debug版本中发现Release版才会遇到的错误。
　　另外，通过函数指针调用函数时，会通过检查栈指针验证函数调用的匹配性（防止原型不匹配）。
　　使用/RTCx选项会造成Debug版本出错，而Release版本正常的现象，因为Release版中未初始化的变量是随机的，很可能使指针指向了有效但是错误的地址，从而掩盖了错误。
　　说了这么多好处，这个编译选项有个限制：那就是只能在/Od选项下使用。
　　Gm
　　，
　　INCREMENTAL or NO
　　编译选项中的Gm和链接选项中的INCREMENTAL都只为一个目的，加快编译速度。我们经常遇上这样的问题，只修改了一个头文件，结果却造成所有动态库的重新编译。而这两个选项就是为了解决这样的问题。
　　如果启用了/Gm开关，编译器在项目中的.idb文件中存储了源文件和类定义之间的依赖关系。之后的编译过程中使用.idb文件中的信息确定是否需要编译某个源文件，哪怕是此源文件已经包含了已修改的.h文件。
　　INCREMENTAL开关默认是开启的。使用增量链接生成的可执行文件或者动态链接库会大于非增量链接的程序，因为有代码和数据的填充。另外，增量链接的文件还包含跳转trunk以处理函数重定位到新地址。
　　MSDN上明确指出：为确保最终发布版本不包含填充或者trunk，请非增量链接程序。
　　_DEBUG
　　与
　　NDEBUG
　　这个话题放在本节的最后，却是最重要的一个选项。这两个是编译器的预处理器定义，默认情况下_DEBUG用于Debug版本，而NDEBUG用于Release版本。它们可以说是重要的无以复加。因为，assert系列的断言仅仅在_DEBUG下生效！
　　下面是assert.h文件中摘出来的：
　　#ifdef NDEBUG
　　#define assert(_Expression) ((void)0)
　　#else
　　#ifdef __cplusplus
　　extern "C" {
　　#endif
　　_CRTIMP void __cdecl _wassert(__in_z const wchar_t * _Message, __in_z const wchar_t *_File, __in unsigned _Line);
　　#ifdef __cplusplus
　　}
　　#endif
　　#define assert(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )
　　#endif /* NDEBUG */
　　可以看出在未定义_DEBUG时，assert变成一条空语句不被执行。
　　也就是说，我们现在所有发布的版本无法使用断言机制进行程序调试。
　　assert的作用就不用我细说了吧。如果列位看官不知道assert为何物，那真得好好补补课了。





