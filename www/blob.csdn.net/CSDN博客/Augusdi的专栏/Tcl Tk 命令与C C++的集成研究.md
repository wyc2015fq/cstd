
# Tcl-Tk 命令与C-C++的集成研究 - Augusdi的专栏 - CSDN博客


2017年01月13日 16:31:49[Augusdi](https://me.csdn.net/Augusdi)阅读数：2496


﻿﻿
Tcl/Tk 命令与C/C++的集成研究
1. 问题来源
基于虚拟现实的维修性分析评价系统（以下简称 VMSAS）是为适应产品无纸设计、分析、评审的需要，以人机工效商业软件JACK 为平台，由本单位进行二次开发形成的一个维修性设计分析评价系统，主要功能包括样机建模、动作建模、维修仿真与检测、维修性分析与评价等。
VMSAS 的使用全过程涉及大量数据。为确保VMSAS 满足并行设计和集成设计的要求，成为一种实用、先进的维修性设计分析工具，能够与CAD 系统、工程分析系统紧密集成，就必须要解决数据获取的授权、数据一致的保证、数据的集成、提交、审核与批准等统一管理及权限等问题。
以上要求 VMSAS 与产品数据管理（PDM）系统进行集成。二者之间的集成方案如图1 所示。
PDM系统
应用程序接口
系统集成接口
产品配置管理
产品结构管理
变更管理
文档管理
工作流管理
对象管理框架
RMS软件
CAD软件
Microsoft
Office
Adobe
Acorbat
VisView
VMSAS
系统
数据获取
样机准备
虚拟样机成熟
拆卸过程仿真
维修性分
析与评价
仿真过程分析
仿真过程记录
分析与评价结果
人机和谐界面
人机交互界面
虚拟场景生成
虚拟环境管理
拆卸过程控制
Oracle
产品基本信息
产品可靠性数据
产品故障信息
分析评价准则
虚拟设施工具等
产品CAD数据
维修性设计要求
虚拟人体模型
维修知识经验
维修资源模型
维修任务及其
仿真分析数据
图1 VMSAS 系统与PDM 系统集成框架
VMASA 以JACK 系统为平台进行二次开发的，JACK 有一套应用程序开发接口（API）——JACKScript，JACKScript 提供了对JACK 场景进行有效控制的途径，可以使用Tcl、[Python](http://lib.csdn.net/base/11)、Lisp 三种语言进行交互以及二次开发。我们以Tcl/Tk 开发应用程序，以Python 开发仿真脚本。选用UGS PLM Solution 公司的TeamCenter Enterprise 系统进行开发来管理VMASA 中的数据。TeamCenter 系统中的应用程序接口API 函数都是以C/C++语言开发的，以DLL 文件形式提供给用户调用。故要在Tcl/Tk 环境中调用这些API 函数，就要解决Tcl/Tk 命令与C/C++函数之间的接口问题。本文围绕这个问题进行讨论。
基金项目：国家自然科学基金资助项目：并行设计中产品维修性模型研究（编号：50005023），“十五”武器装备预研项目资助。
作者简介：梁伟杰（1980.05～），男，汉族，硕士生，研究方向为维修性理论与应用。通讯地址：石家庄军械工程学院六系维修工程实验中心，050003。联系电话：0311－86879059。蒋科艺（1977～），男，汉族，博士生，主要研究方向为虚拟维修仿真以及应用。吕剑锋（1981～），男，汉族，硕士生，研究方向为维修性理论与应用。
http://www.elecfans.com电子发烧友http://bbs.elecfans.com电子技术论坛
2. Tcl/Tk 设计原理
在控制、仿真、检测等多个领域，涉及大量专用系统的开发以及测试环境的建立，测试环境的建立异常繁琐。人们力图寻找一种新的编程语言，它既要有好的代码可重用性，又要简单易学，这样就促成了Tcl（Tool Command Language，工具命令语言）的产生。其总体结构如图1所示：
扩展层Tcl层应用层
剖析器
初始化
命令循环
扩展命令内建命令应用扩展命令
图1 TCL总体结构简图
Tcl 让应用程序由包含编译代码的大块实体和一小部分用于配置和编写高级命令的Tcl 代码组成，把编程按照基于组件的方法来进行，不同的组件有不同的功能，用于不同的目的。Tcl 有良好的扩展性, 方便用户为其增添新的功能模块。Tcl 是一种集C/C++灵活强大的功能与BASIC 易学高效的风格于一身的通用程序设计语言。
Tcl 是一种可嵌入的命令脚本化语言 (Command Script Language)。“可嵌入”是指把很多应用有效，无缝地集成在一起。“命令”是指每一条 Tcl 语句都可以理解成命令加参数的形式：
Tk（Tool Kit）是基于Tcl 的图形程序开发工具箱，是Tcl 的重要扩展部件。Tcl/Tk 提供了足够的可编程特性（变量、流程控制和过程），使得现有程序可以组装成符合自己需要的复杂的脚本程序。Tcl 解释器可以很容易地添加到已有应用程序中去，这种能力将它与其他Shell 语言区分开来，它是一种扩展语言的语言，用来配置和定制应用程序。Tk 隐藏了许多C/C++ 程序的设计细节,可快速地开发基于图形界面窗口的程序。Tcl/Tk 可快速开发应用程序及应用系统，在自控、仿真、测试系统、网络管理、可视化应用、CAD
等方面都有大量的应用成果，在欧美的许多大学和实验室中都有广泛的应用。
但Tcl/Tk毕竟是一种脚本语言，就象其它的一些脚本语言一样，也有很多事情不能够做或很难做。其解决途径是联合C/C++与Tcl/Tk共同开发。提供C/C++程序调用TCL/TK 的解释器来运行TCL/TK脚本。
3. C/C++函数注册为Tcl/Tk 命令
用 C/C++语言很容易实现Tcl/Tk 命令的扩展，而且用C/C++实现的命令比相应的Tcl 命令执行效率要高。在各种以C/C++实现的仿真检测程序中可方便地调用Tcl/Tk 命令。实现Tcl 命令的C/C++代码被称之为命令过程（command procedure），命令过程的接口非常类似于主程序main 的接口，它的输入是一个数组，包含了与Tcl 脚本命令变元确切对应的值，其结果就是Tcl 命令的结果。
在用户的 C/C++程序中，为了能够访问TCL/TK 库，必须在源代码声明两个调用库的头文件，即"tcl.h"
和"tk.h"两个文件。要建立混合Tcl/Tk 和C/C++应用程序，须按如下方式进行：
3.1 调用"Tcl_Main"函数（Tk_Main 原理相同）
Tcl_Main(argc, argv, Tcl_AppInit)，用来控制整个Tcl 解释器程序，没有返回值。这是一种较为高级的接口，可以替用户创建解释器、处理命令行变元来执行脚本，提供交互式命令循环等。"Tcl_Main"函数有三个变量：第一个变量表示在这个数组的元素个数；第三个变量是指向初始化函数的指针；第二个变量是一个字符串型数组，每个字符串都有一个特殊的含义。字符串数组通"Tcl_Main"来通知Tcl/Tk 解释器应用程序的名称和Tcl/Tk 命令在脚本中的位置。这个数组实际上是传给解释器的命令行参数。数组的第一项给出应用程序名称，第二项给出了运行的脚本位置。
命令 [参数 1] [参数 2] [参数 3] [参数 4] ...... [参数 N]
http://www.elecfans.com电子发烧友http://bbs.elecfans.com电子技术论坛
3.2 初始化函数
"Tcl_Main"的调用控制了程序在Tcl/Tk 中的整个调用，但是在底部初始化之后和Tcl/Tk 脚本运行之前，能够执行用户自定义的函数。在从Tcl/Tk 脚本中使用命令过程之前，必须提供该命令过程的初始化注册过程，为了使用Tcl 解释器，应用程序首先产生一个称之为解释器（interpreter）的对象。此时，如果要将自己的扩展模块创建为共享库，此初始化过程的名称必须以“_Init”来结束，如TclApi_Init，Random_Init等。当Tcl/Tk 脚本加载这个库的时候就会自动调用这个过程。
3.3 C/C++函数注册为Tcl/Tk 过程
C/C++函数在Tcl 中注册时需调用一个特定的原型函数，此函数返回一个整数类型，并设置4 个变量，第一个是Tcl/Tk 库文件类型"ClientData"；第二个变量是指向解释器的指针；最后的两个变量类似于在C/C++ "main"函数中的"argc"和 "argv"这两个变量被用于传递参数给Tcl/Tk 过程。参数"argc"包含了传递给Tcl/Tk 过程的参数个数"argv" 是字符串数组，每个字符串包含了一个参数。如下所示：
int TclApi(ClientData clientData, Tcl_Interp *interp,int argc,char *argv[]);
当一个函数被注册作为Tcl/Tk 过程使用时需一个指针与之联系，指针通过"ClientData"来传递进来。
"ClientData"的概念允许程序员联系数据结构和对象，调用能引用这个对象的过程。3.2 中提到的注册过程需要调用"Tcl_CreateCommand"函数，此函数有5 个参数：第一个参数是指向解释器的指针；第二个参数是在Tcl/Tk 中的过程名；第三个参数是一个指向函数的指针，它在Tcl/Tk 过程被执行时调用；最后两个参数是"ClientData"项和一个指针删除例程。如下面例子所示，Tcl/Tk 将"TclApi"函数注册为" tcl_app"命令：
Tcl_CreateCommand(interp, "tcl_app", TclApi, (ClientData)NULL,(Tcl_CmdDeleteProc *)NULL );
3.4 变量访问
在执行Tcl/Tk 过程时能调用C/C++函数，并可从C/C++中通过一些函数获得Tcl/Tk 的帮助。
"Tcl_GetVar"函数返回一个指向Tcl/Tk 变量的字符串指针。此Tcl/Tk 变量在执行脚本联系到解释器的当前范围被访问，如果在当前范没有局部变量则访问全局变量，如没有匹配的全局变量存在则返报告错误。
下面是Tcl/Tk 脚本中被访问的一部分代码。
set a_prd_tree_name "1966PZ152"
下面是在C/C++代码中访问Tcl/Tk 变量："a_prd_tree_name"
char PTName_001 [10];
strncpy(PTName_001, Tcl_GetVar( pInterp, "a_prd_tree_name", 0 ), 9 );
这样，变量PTName_001 的值就变为了"1966PZ152"。
"Tcl_SetVar"函数允许程序修改Tcl/Tk 变量的值。
综上所述，若要实现Tcl/Tk 命令调用C/C++ API，则被调用的API 需具有以上特定结构，可给此API构造一个具有以上特性的“外壳”，如图2 所示：
拥有Tcl解释器的C/C++函数
函数的Tcl初始化过程
Tcl/Tk脚本中要调用的DLL
中的C/C++函数
C/C++函数注册为Tcl/Tk命令
Tcl/Tk命令C/C++过程
图 2 Tcl/Tk 调用C/C++函数
4. Tcl/Tk 命令调用PDM 系统API 函数的一个实例
如何在 Tcl/Tk 命令实现的系统中调用C/C++ DLL 中的函数是笔者在VMSAS 系统与某PDM 系统进行集成时遇到的问题。有人写过一个可以将C/C++ API 输出为Tcl 命令的工具，叫做SWIG（Simple WrapperInterface Generator）。这个工具的缺点是C/C++的代码必须可知，并且SWIG 生成的C 接口对于脚本编写http://www.elecfans.com电子发烧友http://bbs.elecfans.com电子技术论坛
人员并不友好，而手工编制的Tcl 接口要好得多。
例如，用户要登录 PDM 系统，首先必须要进行验证，此时用到一个baseapi.dll 中的函数：
int clLogin2(char UsrName[], char Password[], int mfail)
要在Tcl/Tk 脚本中调用这个函数。可按如下方式进行：
1）、生成DLL 必要信息
\#include "stdio.h"
\#include "string.h"
\#include "windows.h"
\#include "tcl.h"
\#ifndef DECLSPEC_EXPORT
\#define DECLSPEC_EXPORT __declspec(dllexport)
\#endif
BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved) { return TRUE; }
2）、引用这个函数
\#pragma comment(lib, "/yourPath/baseapi.dll")
extern "C" _declspec(dllimport) int clLogin2(char UsrName[], char Password[], int mfail);
3）、对应的拥有Tcl 解释器的形式
int clLogin2Cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
4）、对此函数进行初始化
EXTERN_C int DECLSPEC_EXPORT clLogin2_Init(Tcl_Interp* interp) // 此处必须加“_Init”后缀
{ // 将此C 过程注册为Tcl 命令
Tcl_CreateCommand(interp, " clloginto",
(Tcl_CmdProc *)ClLogin2Cmd, // 调用第4 步实现
(ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
Tcl_PkgProvide(interp, " clloginto ", "0.1"); // 给此注册的Tcl 命令一个版本号0.1
}
5）、具体的实现
int clLogin2Cmd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[ ] )
{ char usr[]; char psword[];
int error1, error2, error3, mfail, dstat;
clLogin2_Init(interp); // 必须先调用函数初始化过程
if (argc > 4)
{ interp->result = "Usage: clloginto ?range?";
return TCL_ERROR; }
if (argc == 4)
{ // 使用 Tcl_GetString 接受字符串参数：
error1 = Tcl_GetInt(interp, argv[1], usr);
if (error1 != TCL_OK) return error1;
error2 = Tcl_GetString(interp, argv[2], psword);
if (error2 != TCL_OK) return error2;
error3 = Tcl_GetInt(interp, argv[3], mfail);
if (error3 != TCL_OK) return error3;
}
dstat = ClLogin2(usr, psword, mfail); // 在此调用C DLL 中的函数
return dstat; // 返回登录状态值，由此可以进行用户登录验证
}
http://www.elecfans.com电子发烧友http://bbs.elecfans.com电子技术论坛
可将上述实现在C/C++编译器中生成一个DLL 文件，这个DLL 中的函数就可以由Tcl/Tk 经过pkg_mkIndex 和package require 命令处理就可调用了。以上只是一个简单的示例，其它函数可以相应地进行处理，也可以用宏实现大量函数的转化，此处不再赘述。
5. 小结
通过以上的分析，实现了在Tcl/Tk 脚本中调用C/C++ DLL 中的函数。并在VMSAS 与某PDM 系统集成中进行了验证。
此种方法对于其它的以Tcl/Tk 实现的仿真或检测系统中，为提高运行效率或提供新的功能而引入C/C++函数进行扩展的情况，都有一定的参考意义。
参考文献：
[1] 郝建平，蒋科艺等，基于虚拟维修仿真的维修性分析评价及系统实现，数字制造科学，2004.12
[2] Brent B. Welch. Tcl/ tk 组合教程。王道义，乔陶鹏等译。北京：电子工业出版社，2002.6
[3] 王坚，金革，Tcl/Tk 和 C 语言的接口，计算机应用，2000.12
[4] 周波，杨贯中，蔡宇辉，TCL/TK 语言结构分析及其在网络教学中的应用，计算机工程，2002.4
[5]http://www.swing.org
[6]news://comp.lang.tcl
[7]http://www.tclchina.com
[8]http://etude.uwaterloo.ca/~ctrudeau/lessons/tcl_tk/tcl_C.html

