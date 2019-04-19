# error LNK2005: _DllMain@12 already defined in MSVC - 三少GG - CSDN博客
2011年03月31日 20:57:00[三少GG](https://me.csdn.net/scut1135)阅读数：916标签：[mfc																[microsoft																[library																[linker																[properties																[多线程](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=properties&t=blog)](https://so.csdn.net/so/search/s.do?q=linker&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
Mp4 读头信息时碰到。
libmp4v2 使用时碰到的问题：
1.**Project Properties, Linker, Input, 右边有个Ignore Specific Library,输入"MSVCRTD.lib",确定即可**
2. 附加依赖项：libmp4v260.lib **ws2_32.lib**
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
[http://hi.baidu.com/%CC%A9%BF%C6%CD%DE%CD%DE/blog/item/6dfb1b1b60fab0118718bffa.html](http://hi.baidu.com/%CC%A9%BF%C6%CD%DE%CD%DE/blog/item/6dfb1b1b60fab0118718bffa.html)
编程中经常能遇到LNK2005错误——重复定义错误，其实LNK2005错误并不是一个很难解决的错误。弄清楚它形成的原因，就可以轻松解决它了。 
造成LNK2005错误主要有以下几种情况： 
1．重复定义全局变量。可能存在两种情况： 
A、对于一些初学编程的程序员，有时候会以为需要使 用全局变量的地方就可以使用定义申明一下。其实这是错误的，全局变量是针对整个工程的。正确的应该是在一个CPP文件中定义如下：int g_Test;那么在使用的CPP文件中就应该使用：extern int g_Test即可，如果还是使用int g_Test，那么就会产生LNK2005错误，一般错误错误信息类似：AAA.obj error LNK2005 int book c？book@@3HA already defined in BBB.obj。切记的就是不能给变量赋值否则还是会有LNK2005错误。 
这里需要的是“声明”，不是“定义”！根据C++标准的规定，一个变量是声明，必须同时满足两个条件，否则就是定义： 
(1)声明必须使用extern关键字；(2)不能给变量赋初值 
所以，下面的是声明: 
extern int a; 
下面的是定义 
int a; int a = 0; extern int a =0; 
B、对于那么编程不是那么严谨的程序员，总是在需要使用变量的文件中随意定义一个全局变量，并且对于变量名也不予考虑，这也往往容易造成变量名重复，而造成LNK2005错误。 
2．头文件的包含重复。往往需要包含的头文件中含有变量、函数、类的定义，在其它使用的地方又不得不多次包含之，如果头文件中没有相关的宏等防止重复链接 的措施，那么就会产生LNK2005错误。解决办法是在需要包含的头文件中做类似的处理：#ifndef MY_H_FILE //如果没有定义这个宏 
#define MY_H_FILE //定义这个宏 
……. //头文件主体内容 
……. 
#endif 
上面是使用宏来做的，也可以使用预编译来做，在头文件中加入： 
#pragma once 
//头文件主体 
3． 使用第三方的库造成的。这种情况主要是C运行期函数库和MFC的库冲突造成的。具体的办法就是将那个提示出错的库放到另外一个库的前面。另外选择不同的C 函数库，可能会引起这个错误。微软和C有两种C运行期函数库，一种是普通的函数库：LIBC.LIB，不支持多线程。另外一种是支持多线程的： msvcrt.lib。如果一个工程里，这两种函数库混合使用，可能会引起这个错误，一般情况下它需要MFC的库先于C运行期函数库被链接，因此建议使用支持多线程的msvcrt.lib。所以在使用第三方的库之前首先要知道它链接的是什么库，否则就可能造成LNK2005错误。如果不得不使用第三方的库，可以尝试按下面所说的方法修改，但不能保证一定能解决问题，前两种方法是微软提供的： 
A、选择VC菜单Project->Settings->Link->Catagory选择Input，再在Ignore libraries 的Edit栏中填入你需要忽略的库，如：Nafxcwd.lib;Libcmtd.lib。然后在Object/library Modules的Edit栏中填入正确的库的顺序，这里需要你能确定什么是正确的顺序，呵呵，God bless you！ 
B、选择VC菜单Project->Settings->Link页，然后在Project Options的Edit栏中输入/verbose:lib，这样就可以在编译链接程序过程中在输出窗口看到链接的顺序了。 
C、选择VC菜单Project->Settings->C/C++页，Catagory选择Code Generation后再在User Runtime libraray中选择MultiThread DLL等其他库，逐一尝试。 
关于编译器的相关处理过程，参考： 
http://www.donews.net/xzwenlan/archive/2004/12/23/211668.aspx 
这就是我所遇到过的LNK2005错误的几种情况，肯定还有其他的情况也可能造成这种错误，所以我不希望你在看完这篇文章以后，再遇到LNK2005错误时候，不动脑筋的想对号入座的排除错误。编程的过程就是一个思考的过程，所以还是多多开动你的头脑，那样收获会更多！ 
**4. Grant:  一个类的成员函数(.h)的实现在inline 和  cpp 中的不同表示！！！**
方案二、
链接时出错提示：
LIBCMT.lib(crt0dat.obj) : error LNK2005: __amsg_exit already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(crt0dat.obj) : error LNK2005: __initterm_e already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(crt0dat.obj) : error LNK2005: _exit already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(crt0dat.obj) : error LNK2005: __exit already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(crt0dat.obj) : error LNK2005: __cexit already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(fflush.obj) : error LNK2005: _fflush already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(setlocal.obj) : error LNK2005: __configthreadlocale already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(tidtable.obj) : error LNK2005: __encode_pointer already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(tidtable.obj) : error LNK2005: __decode_pointer already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(invarg.obj) : error LNK2005: __invoke_watson already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(realloc.obj) : error LNK2005: _realloc already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(crt0init.obj) : error LNK2005: ___xi_a already defined in MSVCRTD.lib(cinitexe.obj)
LIBCMT.lib(crt0init.obj) : error LNK2005: ___xi_z already defined in MSVCRTD.lib(cinitexe.obj)
LIBCMT.lib(crt0init.obj) : error LNK2005: ___xc_a already defined in MSVCRTD.lib(cinitexe.obj)
LIBCMT.lib(crt0init.obj) : error LNK2005: ___xc_z already defined in MSVCRTD.lib(cinitexe.obj)
LIBCMT.lib(hooks.obj) : error LNK2005: "void __cdecl terminate(void)" ([?terminate@@YAXXZ](mailto:?terminate@@YAXXZ)) already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(winxfltr.obj) : error LNK2005: __XcptFilter already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(mlock.obj) : error LNK2005: __unlock already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(mlock.obj) : error LNK2005: __lock already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(dosmap.obj) : error LNK2005: __errno already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(crt0.obj) : error LNK2005: _mainCRTStartup already defined in MSVCRTD.lib(crtexe.obj)
LIBCMT.lib(errmode.obj) : error LNK2005: ___set_app_type already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(isctype.obj) : error LNK2005: __isctype already defined in MSVCRTD.lib(MSVCR80D.dll)
LIBCMT.lib(tolower.obj) : error LNK2005: _tolower already defined in MSVCRTD.lib(MSVCR80D.dll)
**解决方案：**
**Project Properties, Linker, Input, 右边有个Ignore Specific Library,输入"MSVCRTD.lib",确定即可。**
方案三、
**error LNK2005: _DllMain@12 already defined in AUDIO.obj 处理方法**
VC++6.0中编译链接过程出错：
mfcs42d.lib(dllmodul.obj) : error LNK2005: _DllMain@12 already defined in AUDIO.obj
症状
当 C 运行时 (CRT) 库和 Microsoft 基础类 (MFC) 库的链接顺序有误时，可能会出现以下 LNK2005 错误之一：
nafxcwd.lib(afxmem.obj) :error LNK2005:
"void * __cdecl operator new(unsigned int)"(??2@YAPAXI@Z) already
defined in LIBCMTD.lib(new.obj) 
nafxcwd.lib(afxmem.obj) :error LNK2005:
"void __cdecl operator delete(void *)"(??3@YAXPAX@Z) already defined
in LIBCMTD.lib(dbgnew.obj) 
nafxcwd.lib(afxmem.obj) :error LNK2005:
"void * __cdecl operator new(unsigned int,int,char const *,int)"
(??2@YAPAXIHPBDH@Z) already defined in LIBCMTD.lib(dbgnew.obj) 
mfcs40d.lib(dllmodul.obj):error LNK2005:_DllMain@12 already defined in
MSVCRTD.LIB (dllmain.obj) 
mfcs42d.lib(dllmodul.obj):error LNK2005:_DllMain@12 already defined in
msvcrtd.lib(dllmain.obj)
原因
CRT 库对 new、delete 和 DllMain 函数使用弱外部链接。MFC 库也包含 new、delete 和 DllMain 函数。这些函数要求先链接 MFC 库，然后再链接 CRT 库。
解决方案一：强制链接器按照正确的顺序链接库
1. 在“项目”菜单上，单击“设置”。 
2. 在“项目设置”对话框的“以下项目的设置”视图中，单击以选中出现链接错误的项目配置。 
3. 在“链接”选项卡上，单击以选中“类别”组合框中的“输入”。 
4. 在“忽略库”框中，插入库名（例如，Nafxcwd.lib;Libcmtd.lib）。
注意：等效的链接器命令行是：/NOD:<library name>。 
5. 在“对象/库模块”框中，插入库名。必须确保这些库按顺序列出，而且是行中的前两个库（例如，Nafxcwd.lib 和 Libcmtd.lib）。 
要在 Visual C++ .NET 中设置该选项，请阅读“设置 Visual C++ 项目属性”联机帮助主题。
解决方案二：找到并纠正出现问题的模块
要查看当前的库链接顺序，请按照下列步骤操作： 1. 在“项目”菜单上，单击“设置”。 
2. 在“项目设置”对话框的“以下项目的设置”视图中，单击以选中出现链接错误的项目配置。 
3. 在“链接”选项卡上的“项目选项”框中键入 /verbose:lib。 
4. 重新生成项目。在链接过程中，这些库将在输出窗口中列出。
状态
这种现象是设计导致的。
更多信息
使用 MFC 库时，务必先链接它们，然后再链接 CRT 库。这可以通过确保项目中的每个文件都首先包 含 Msdev/Mfc/Include/ Afx.h 来完成。直接包含 (#include <Afx.h>) 或间接包含 (#include < Stdafx.h>) 都可以。Afx.h 包含文件会通过使用 #pragma comment (lib,"<libname> ") 指令来强制采用库的正确顺序。
如果源文件的扩展名为 .c，或者该文件的扩展名为 .cpp 但不使用 MFC，则可以创建一个较小的头文件 (Forcelib.h) 并将其放在模块的顶端。这个新的头文件可确保按照正确的顺序搜索库。
Visual C++ 不包含该头文件。要创建此文件，请按照下列步骤操作： 1. 打开 Msdev/Mfc/Include/Afx.h。 
2. 选定 #ifndef _AFX_NOFORCE_LIBS 和 #endif //!_AFX_NOFORCE_LIBS 之间的行。 
3. 将选定部分复制到 Windows 剪贴板。 
4. 创建一个新文本文件。 
5. 将剪贴板的内容粘贴到这个新文件中。  
6. 将该文件另存为 Msdev/Mfc/Include/Forcelib.h。
在 Visual C++ .NET 中重现问题的步骤
1. 启动 Microsoft Visual Studio .NET。 
2. 在“文件”菜单上，指向“新建”，然后单击“项目”。 
3. 单击“项目类型”下的“Visual C++ 项目”，然后单击“模板”下的“MFC 应用程序”。 
4. 在“名称”文本框中，键入 Q148652。 
5. 在“位置”文本框中，键入 C:/Test，然后单击“确定”。 
6. 在“MFC 应用程序向导”对话框中，单击“应用程序类型”。 
7. 单击“应用程序类型”下的“基于对话框”，然后单击“MFC 的使用”下的“在静态库中使用 MFC”。 
8. 单击“完成”。 
9. 在“解决方案资源管理器”中，选择“源文件”下的全部三个 .cpp 文件。 
10. 右键单击三个选定的文件，然后单击“删除”。 
11. 右键单击“源文件”，指向“添加”，然后单击“添加新项”。 
12. 单击“模板”下的“C++ 文件”。在“名称”文本框中，键入 Aa。单击“打开”。 
13. 将以下代码粘贴到 Aa.cpp 中：int test(){new int; return 1;}
14. 右键单击“源文件”，指向“添加”，然后单击“添加现有项”。  
15. 选择以下文件： • Q148652.cpp 
• Q148652Dlg.cpp 
• stdafx.cpp 
16. 单击“打开”。 
17. 您在第 15 步中选择的文件将出现在“源文件”下。 
18. 选择“源文件”下的全部四个 .cpp 文件。 
19. 右键单击选定的四个 .cpp 文件，然后单击“属性”。 
20. 展开“配置属性”，然后展开“C/C++”。 
21. 单击“预编译头”。 
22. 将“创建/使用预编译头”属性设置为“不使用预编译头”。单击“确定”。 
23. 在“生成”菜单上，单击“重新生成解决方案”。
本人Aladdin已采用的方案：
方案三中的解决方案一：强制链接器按照正确的顺序链接库
1. 在“项目”菜单上，单击“设置”。 
2. 在“项目设置”对话框的“以下项目的设置”视图中，单击以选中出现链接错误的项目配置。 
3. 在“链接”选项卡上，单击以选中“类别”组合框中的“输入”。 
4. 在“忽略库”框中，插入库名（例如，Nafxcwd.lib;Libcmtd.lib）。 注意：等效的链接器命令行是：/NOD:<library name>。 
5. 在“对象/库模块”框中，插入库名。必须确保这些库按顺序列出，而且是行中的前两个库（例如，Nafxcwd.lib 和 Libcmtd.lib）。
