# WINCE下error C2061: syntax error : identifier 'PCONTEXT' 处理 - xqhrs232的专栏 - CSDN博客
2014年04月23日 14:44:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1371
原文地址::[http://blog.csdn.net/btkxql/article/details/6607840](http://blog.csdn.net/btkxql/article/details/6607840)
 最近在做个调用摄像头数据进行扫描识别的程序，本人愚钝，环境搭建都要半天时间，之前在windows桌面环境下就弄了半天，现在要将程序移植到wm下，移入之后根本不是想象的那么轻松，由于wm和win桌面操作系统的DirectShow SDK有很多差别，只能是重新搞了。
　　几番周折后又下载了个操作camera的示例源码，是基于windows标准库来建立的，为了想尽快看到是否能成功调用摄像头，赶紧动手进行项目创建，我的wm SDK只有基于MFC的智能应用程序，只能先创建个空的C++项目了，这样的话项目比较“纯净”。
　　项目创建完成，进行编译，本来就不寄予多大的希望一次性成功，出现以下错误：
　　错误 1 error C2146: syntax error : missing ';' before identifier 'ContextRecord' F:Program FilesWindows Mobile 6 SDKPocketPCincludeARMV4Iwinnt.h 2978
　　错误 2 error C4430: missing type specifier - int assumed. Note: C++ does not support default-int F:Program FilesWindows Mobile 6 SDKPocketPCincludeARMV4Iwinnt.h 2978
　　错误 3 error C4430: missing type specifier - int assumed. Note: C++ does not support default-int F:Program FilesWindows Mobile 6 SDKPocketPCincludeARMV4Iwinnt.h 2978
　　错误 4 error C2146: syntax error : missing ';' before identifier 'LPCONTEXT' F:Program FilesWindows Mobile 6 SDKPocketPCincludeARMV4Iwinbase.h 1416
　　错误 5 error C4430: missing type specifier - int assumed. Note: C++ does not support default-int F:Program FilesWindows Mobile 6 SDKPocketPCincludeARMV4Iwinbase.h 1416
　　错误 6 error C4430: missing type specifier - int assumed. Note: C++ does not support default-int F:Program FilesWindows Mobile 6 SDKPocketPCincludeARMV4Iwinbase.h 1416
　　错误 7 error C2061: syntax error : identifier 'LPCONTEXT' F:Program FilesWindows Mobile 6 SDKPocketPCincludeARMV4Iwinbase.h 2119
　　错误 8 error C4430: missing type specifier - int assumed. Note: C++ does not support default-int F:Program FilesWindows Mobile 6 SDKPocketPCincludeARMV4Iwinbase.h 2127
　　错误 9 error C2143: syntax error : missing ',' before '*' F:Program 　　
FilesWindows Mobile 6 SDKPocketPCincludeARMV4Iwinbase.h 2127 
　　错误 10 error C2040: 'PCONTEXT' : '_CONTEXT *' differs in levels of indirection from 'int' F:Program FilesWindows Mobile 6 SDKPocketPCincludeARMV4Iexcpt.h 65
　　错误 11 error C2378: 'LPCONTEXT' : redefinition; symbol cannot be overloaded with a typedef F:Program FilesWindows Mobile 6 SDKPocketPCincludeARMV4Iexcpt.h 68
　　从上面错误中大略可以看出，都是系统文件的问题，应该是环境的问题，所以在经过一些资料查找后，将解决方法罗列如下(首先wm SDK肯定是要提前安装好的)：
　　1. 当前项目右键-属性-平台修改为windows mobile x SDK
　　2. 配置属性-C/C++-预处理器-预处理器定义栏里面：　　　　 NDEBUG;_WIN32_WCE=$(CEVER);UNDER_CE;$(PLATFORMDEFINES);WINCE;_WINDOWS;_USRDLL;TEST_DLL_EXPORTS;$(ARCHFAM);$(_ARCHFAM_);_UNICODE;UNICODE
　　3. 链接器-输入-附加依赖项：strmbase.lib strmiids.lib
　　4. 链接器-系统-子系统栏-windows CE
　　然后编译喽，如果还有错误应该跟环境问题没大的关系了，希望跟我有同样“遭遇”的新手们能尽快解决问题，由于此问题属于mobile的开发环境搭建中的基点，所以我故意将标题命名成如上。
//================================================================================================================
备注：
1》我是在把APP工程在WINCE5/6的SDK之间来回增加支持，再重新编译的情况下出的这个情况
----新增加对一个SDK的支持，最好从一个已经编译OK的SDK COPY配置过来
2》不过我的工程这样设置了还是不行----1>D:\Program Files\Microsoft Visual Studio 8\VC\ce\atlmfc\include/winres.h(33) : fatal error RC1015: cannot open include file 'winresrc.h'.
