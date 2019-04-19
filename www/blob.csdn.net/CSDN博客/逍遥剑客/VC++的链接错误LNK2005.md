# VC++的链接错误LNK2005  - 逍遥剑客 - CSDN博客
2008年01月09日 13:54:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1743标签：[vc++																[多线程																[iostream																[c++																[mfc																[library](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=iostream&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=vc++&t=blog)
个人分类：[基础知识](https://blog.csdn.net/xoyojank/article/category/259553)
作者: Jerry Cat
时间: 2006/04/19
链接: http://www.cppblog.com/jerysun0818/archive/2006/04/19/5913.html
编程中经常能遇到LNK2005错误——重复定义错误，其实LNK2005错误并不是一个很难解决的错误。弄清楚它形成的原因，就可以轻松解决它了。
造成LNK2005错误主要有以下几种情况：
1．重复定义全局变量。可能存在两种情况：
A、对于一些初学编程的程序员，有时候会以为需要使用全局变量的地方就可以使用定义申明一下。其实这是错误的，全局变量是针对整个工程的。正确的应该是在一个CPP文件中定义如下：int g_Test;那么在使用的CPP文件中就应该使用：extern int g_Test即可，如果还是使用int g_Test，那么就会产生LNK2005错误，一般错误错误信息类似：AAA.obj error LNK2005 int book c？[book@@3HA](mailto:book@@3HA)already defined in BBB.obj。切记的就是不能给变量赋值否则还是会有LNK2005错误。
       这里需要的是“声明”，不是“定义”！根据C++标准的规定，一个变量是声明，必须同时满足两个条件，否则就是定义：
(1)声明必须使用extern关键字；(2)不能给变量赋初值
所以，下面的是声明:
extern int a;
下面的是定义
int a; int a = 0; extern int a =0;
B、对于那么编程不是那么严谨的程序员，总是在需要使用变量的文件中随意定义一个全局变量，并且对于变量名也不予考虑，这也往往容易造成变量名重复，而造成LNK2005错误。
2．头文件的包含重复。往往需要包含的头文件中含有变量、函数、类的定义，在其它使用的地方又不得不多次包含之，如果头文件中没有相关的宏等防止重复链接的措施，那么就会产生LNK2005错误。解决办法是在需要包含的头文件中做类似的处理：#ifndef MY_H_FILE   //如果没有定义这个宏
#define MY_H_FILE   //定义这个宏
…….   //头文件主体内容
…….
#endif
上面是使用宏来做的，也可以使用预编译来做，在头文件中加入：
#pragma once
//头文件主体
3．使用第三方的库造成的。这种情况主要是C运行期函数库和MFC的库冲突造成的。具体的办法就是将那个提示出错的库放到另外一个库的前面。另外选择不同的C函数库，可能会引起这个错误。微软和C有两种C运行期函数库，一种是普通的函数库：LIBC.LIB，不支持多线程。另外一种是支持多线程的：msvcrt.lib。如果一个工程里，这两种函数库混合使用，可能会引起这个错误，一般情况下它需要MFC的库先于C运行期函数库被链接，因此建议使用支持多线程的msvcrt.lib。所以在使用第三方的库之前首先要知道它链接的是什么库，否则就可能造成LNK2005错误。如果不得不使用第三方的库，可以尝试按下面所说的方法修改，但不能保证一定能解决问题，前两种方法是微软提供的：
A、选择VC菜单Project->Settings->Link->Catagory选择Input，再在Ignore libraries 的Edit栏中填入你需要忽略的库，如：Nafxcwd.lib;Libcmtd.lib。然后在Object/library Modules的Edit栏中填入正确的库的顺序，这里需要你能确定什么是正确的顺序，呵呵，God bless you！
B、选择VC菜单Project->Settings->Link页，然后在Project Options的Edit栏中输入/verbose:lib，这样就可以在编译链接程序过程中在输出窗口看到链接的顺序了。
C、选择VC菜单Project->Settings->C/C++页，Catagory选择Code Generation后再在User Runtime libraray中选择MultiThread DLL等其他库，逐一尝试。
这就是我所遇到过的LNK2005错误的几种情况，肯定还有其他的情况也可能造成这种错误，所以我不希望你在看完这篇文章以后，再遇到LNK2005错误时候，不动脑筋的想对号入座的排除错误。编程的过程就是一个思考的过程，所以还是多多开动你的头脑，那样收获会更多！
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
支持,我在社区里也看到了许多LINK 2005错
补充一点,就是一次在用第三方库时,由于errno被重定义,用多种方法都不能解决,后查找MSDN,发现link有个选项/FORCE可以解决,在IDE下
Project->Settings->Link页,选categroy为custom,将force file output前打勾
但会有警告
warning LNK4088: image being generated due to /FORCE option; image may not run
但的确解决了问题,这是由于VC对重定义比较严格,像BCB或GCC在库中的重定义不会有任何警告或错误
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
我发现的另外一个出现LINK2005的现象，好像是由于名称空间而引起的。我在dos下写的程序没有问题，但是放在mfc中就出现了这个链接错误。因为起初图省事，我在一个头文件中写了using namespace std,并且这个头文件我多处使用，另外，我还使用了boost库。后来，问题解决的方法非常奇怪，在一个头文件中引用其他头文件，这些头文件的顺序换一下就通过了，那个出现问题的头文件中我使用了std::map，当我把这种容器使用模板代替后，链接就有没事了。（例如：template<class coll>）,后来感到模板技术还有这种效果,赚了!哈哈
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
What are the C and C++ libraries my program would link with?
                      Summary Table
 /MDd      Yes               No                   MSVCRTD.LIB, MSVCIRTD.LIB
你的程序使用了/ML编译选项,而程序依赖的.lib可能使用/MDd选项编译,造成链接冲突.
统一编译选项可回避此错误
Project Settings->C/C++ Tab->Category:CodeGeneration
Use run-time library组合框中选择Multithread Dll(或Debug Multithread Dll )
Summary Table for CRT DLLs Used
|Import Library Linked With|DLLs Used (Visual C++ 5.0|6.0)|DLLs Used (Visual C++ 4.2)|DLLs Used (Visual C++ .NET 2002| Visual C++ .NET 2003)|
|----|----|----|----|
|MSVCRT.LIB|MSVCRT.DLL|MSVCRT.DLL|MSVCRT.DLL|
|MSVCRTD.LIB|MSVCRTD.DLL|MSVCRTD.DLL|MSVCRTD.DLL|
|MSVCPRT.LIB|MSVCP(5|6)0.DLL|MSVCP7(0|1).DLL| |
|MSVCPRTD.LIB|MSVCP(5|6)0D.DLL|MSVCP7(0|1)D.DLL| |
|MSVCIRT.LIB|MSVCIRT.DLL|MSVCIRT.DLL| |
|MSVCIRTD.LIB|MSVCIRTD.DLL|MSVCIRTD.DLL| |
注意：所有相关工程都应该选择相同编译选项
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
微软的MSDN中查到信息的 
可能的原因
不慎同时与单线程库和多线程库链接。确保应用程序项目文件只包括适当的库，并且任何第三方库已适当创建了单线程或多线程版本。
该符号为封装函数（通过用 /Gy 编译创建），包含在多个文件中，但在各编译间已改变。重新编译所有包含 symbol 的文件。
以不同的形式在不同库中的两个成员对象中定义了该符号，并且使用了这两个成员对象。
某个绝对符号被定义两次，而每次定义的值不同。
头文件声明并定义了变量。可能的解决方案有：
在 .h 中声明变量：extern BOOL MyBool;，然后在 .c 或 .cpp 文件中向它分配：BOOL MyBool = FALSE;。
将变量声明为 Static。
将变量声明为 selectany。
当将 uuid.lib 与定义 GUID 的其他 .lib 文件（例如 oledb.lib 和 adsiid.lib）一起使用时。例如：
oledb.lib(oledb_i.obj) : error LNK2005: _IID_ITransactionObject
already defined in uuid.lib(go7.obj)
若要修复，请将 /FORCE:MULTIPLE 添加到链接器命令行选项，并确保 uuid.lib 是引用的第一个库。
有关更多信息，请参阅知识库文章：
Q148652，PRB: LNK2005 Errors When Link C Run-Time Libraries Are Linked Before MFC Libraries。
Q140440，FIX: Global Overloaded Delete Operator Causes LNK2005。
Q184235，PRB: LNK2005 Errors on New and Delete When Defining _ATL_MIN_CRT。
该错误之后为致命错误 LNK1169。
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
有时候因为工程使用了预编译头文件并且是增量编译，所以当你改动以后可能也会出现LNK2005错误，提示“XXXX已经在XXXX.obj文件中定义”的消息，这时候只要Rebuild All一般都能解决问题。这是因为头文件的顺序被改动等等操作造成的。
最后要说明的：事物是在不断变化中的，C++的标准在变化，编译器也在变化，所以并不是所有的LNK2005错误都可以在这里找到答案，但是至少它能给你以提示。学习并思考才是正确的！
