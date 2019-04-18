# ACE的构建（VC++6.0环境） - ljx0305的专栏 - CSDN博客
2009年02月23日 15:14:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：499标签：[vc++																[library																[preprocessor																[mfc																[linux																[include](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=preprocessor&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=vc++&t=blog)
个人分类：[C																[C++](https://blog.csdn.net/ljx0305/article/category/380566)](https://blog.csdn.net/ljx0305/article/category/394717)
ACE的构建（VC++6.0环境）
Windows下ACE的构建
1. 将ACE-5.5.zip解压到所需的安装目录，此处以E:/为例，解压后形成ACE_wrappers文件夹，因此ACE将会存在于ACE_wrappers/ace目录中。ACE_ROOT=E:/ACE_wrappers。
2. 在系统中新建ACE_ROOT环境变量，值设为 E:/ACE_wrappers。具体设置为：我的电脑->属性->高级->环境变量->新建
3. 在E:/ACE_wrappers/ace目录中创建一个头文件，命名为config.h，其内容为：
#include "ace/config-win32.h"（必选项）
#define ACE_HAS_STANDARD_CPP_LIBRARY 1 （MSVC自带的标准C++头文件）
#define ACE_HAS_MFC 1（与MFC一起使用）
#define ACE_NO_INLINE（取消函数内联从而减小静态库和运行码的大小）
#define ACE_AS_STATIC_LIBS（构建ACE的静态版本）
#define ACE_HAS_WINNT4 0（在Windows 9x/me上构建所需，在ACE中移掉一些WinNT/Win2K中特有的代码）
以上只需要添加前两项就可以了。
4. 在VC++中加载ACE的工程文件（ACE_ROOT /ace/ace.dsw），在release和debug两种编译方式下进行编译，得到相应的库文件(ACE.dll、ACE.lib 和 ACEd.dll、ACEd.lib)，其中ACE.dll 、ACE.lib是由release版生成的，ACEd.dll、ACEd.lib是由debug版生成的。具体操作方法：运行VC++，选择Build ->Batch Build…，将ACE-Win32 Debug和ACE-Win32 Release全部选中，Rebuild All即可。如下图所示：
按照上面的方法可能只编译出ACE.dll 、ACE.lib库文件，具体原因还不详。按照下面的操作方法进行编译可以成功生成全部库文件。具体操作方法：Build->Set Active Configuration…分别选中ACE-Win32 Release进行编译即可。如下图所示
同样打开ace_static.dsw编译生成ACEs.lib、 ACEsd.lib、ACE_RMCasts.lib、ACE_RMCastsd.lib、TMCasts.lib、TMCastsd.lib
5. 配置ACE环境。 打开VC++6.0，选择Tools->Options->Directories 加入以下内容：
1) Executable File：E:/ACE_wrappers/bin
2) Include File：E:/ACE_wrappers
3) Library Files：E:/ACE_wrappers/lib
4) Source Files：E:/ACE_wrappers
在编写程序时要在Project->Settings菜单中进行如下设置： 
C/C++选项卡：
Code Generation：选择多线程版运行时库（Multithreaded DLL）。
Preprocessor：在“Additional include directories”域中加入$ACE_ROOT。　　
Link选项卡：
Input：在“Object/library modules”域中包含适当的ACE库文件，在“Additional library 
path”域中包含ACE库链接文件的路径。这通常是ACE_ROOT/ace。
其中 DLL debug对应ACEd.lib
    DLL release对应ACE.lib
    Static library debug对应ACEsd.lib
    Static library release对应ACEs.lib
    MFC DLL debug对应ACEmfcd.lib
    MFC DLL release对应ACEmfc.lib
   主要用到以上两个库。
6. 在系统环境的变量 path中加入E:/ACE_wrappers/bin，否则在运行程序的时候碰到寻找ace.dll或者aced.dll的问题。
7. 在MFC程序中使用时，应该在InitInstance()中添加：
WSDATA wsData;
WORD VerRequested = MAKEWORD (2, 2);
WSAStarup (VerRequested, &wsData)
注：可以在头文件中加入如下代码就不用再手动填加lib库文件
#ifdef _DEBUG
#pragma comment (lib,"aced.lib")
#else
#pragma comment (lib,"ace.lib")
#endif
8. 在windows下release时有可能报错，错误如下：
error C2039: 'TryEnterCriticalSection' : is not a member of '`global
error C2065: 'TryEnterCriticalSection' : undeclared identifier
解决办法：
工程 Project-->setting-->C/C++   写入:_WIN32_WINNT=0x0500
Linux 下ACE的构建
1. 设置环境变量ACE_ROOT，命令如下：
“vi /etc/profile” ，在“export PATH USER….”后加入4行 
 ACE_ROOT=/opt/ACE 
 export ACE_ROOT
 LD_LIBRARY_PATH=$ACE_ROOT/ace![](http://www.ace-tao.org/bbs/images/smilies/shy.gif)LD_LIBRARY_PATH
 export LD_LIBRARY_PATH
完 成后将/etc/profile执行一次，命令：“chmod 555 /etc/profile”，“/etc/profile”。这样我们的ACE_ROOT就设置好了，可以用如下命令查看ACE_ROOT是否设置好了： “echo $ACE_ROOT” ，这个时候最好reboot启动一次linux。
2. 解压ACE.tar.gz包：
不 要自己手工将ACE.tar.gz包完全解开，特别强调不要完全解开，后面可以看到安装脚本会给我们解开它的；如果你手工解开全部，安装脚本将会将你的解 压缩目录删除的！我们要做的事就只是将ACE.tar.gz包中的ACE-install.sh安装脚本单独解出来，并对其修改。我们只要这一个文件就可 以。用mkdir建目录，如/home/my/ACE，然后将你的ACE.tar.gz放在此目录下（用cp命令拷贝或mv命令移动到此目录）。注意这个 目录将是你的一个存放ACE.tar.gz的目录，安装脚本会来这个目录找这个文件的，这个目录和ACE_ROOT是不一样的，且ACE- install.sh也提示不要将这两个目录设置成同一个目录。ACE_ROOT将是我们的安装目标目录。
3. 从ACE.tar.gz解压出单个文件ACE-install.sh，命令如下：
“tar –zxvf ACE+TAO+CIAO.tar.gz ACE_wrappers/ACE-install.sh”
解 好后，会出来一个新的子目录“ACE_wrappers”。此时可以先将ACE-install.sh移到外面的目录来：“mv /ACE_wrappers/ACE-install.sh .” “.”代表当前目录。当然ACE-install.sh在什么路径下是没什么影响的，因为我们要对ACE-install.sh的内容进行修改的，里面有 很多路径要修改。现在在/home/my/ACE就有了ACE-install.sh，我们现在用vi对其进行修改，修改前自己可以先备份一个。命令： “cp ACE-install.sh ACE-install.sh.bak”，“vi ACE-install.sh”先找到“MY_ACEDIR=${HOME}/aceconfig”，将其改成“MY_ACEDIR= /home/my/ACE”，就是改成我们自己建的、放有ACE.tar.gz文件的目录，之后去掉前面的注释“#”号。接着在下面一点，找到 “MY_ACE_CONFIG=config-sunos5.4-sunc++-4.x.h”，将其修改成“MY_ACE_CONFIG=config- linux.h”，即改成linux下的配置文件。继续改，找到“MY_ACE_GNU_MACROS=platform_sunos5_sunc++. GNU”，将其改成“MY_ACE_GNU_MACROS=platform_linux.GNU”。这个是各种操作系统平台下的GNU宏文件设置。文件 修改完毕，保存退出“:wq”。
4. 安装ACE：
此时应该是在/home/my/ACE目录下，我们执行： h./ACE-install.sh 
此 后将是较长时间的解包、编译等待时间。这个过程将不会出现任何错误，因为安装脚本将为我们做很多事情，如：生成config.h，生成 platform_macros.GNU。我们不需要按ACE-install.html中说的手工改东西：不用做连接“ln -s config-sunos5-sunc++-4.x.h config.h”，安装脚本会给我们做好；(题外话：Windows中没有文件连接这一说，所以要自己生成一个config.h文件，再在它的里面加一 句话“#include "ace/config-win32.h"”。) 
如果编译中出现错误，如“../apps/gperf”等，请编辑$ACE_ROOT/apps下的Makefile，将其中的有关“gperf”的部分注释掉，这个东西只是一个例子，可以不去编译它。 
强调一下，用ACE-install.sh来安装，会编译ACE、tests、apps等。最后会生成libACE.so及tests、apps下也会出来很多经过编译的文件。 
当然如果你只是下载了ACE.tar.gz这样的单个包来安装，你可以手工改文件，再make，因为只编译ACE，工作就少了很多，手工做还可以接受。
ACE学习资料
1. ACE 程序员指南:网络与系统编程的实用设计模式 网络与系统编程的实用设计模式   ACE Programmer's Guide practical Design Patterns for Network and Systems Programming
(美)Stephen D. Huston,(美)James CE Johnson,(美)Umar Syyid著 
马维达 译 中国电力出版社
2. C++网络编程 卷1：运用ACE和模式消除复杂性 
C++ Network Programming Volume 1 Mastering Complexity with ACE and Patterns 
[美]Douglas C.Schmidt,Stephen D.Huston /著 [美]Douglas C.Schmidt，Stephen D.Huston 著，於春景 译   华中科技大学出版社
3. C++网络编程,卷2 基于ACE和框架的系统化复用 
C++ network Programming, Volume 2 Systematic Reuse with ACE and Frameworks 
Douglas C.Schmidt, Stephn D.Huston /著   | 马维达 译 电子工业出版社
4. ACE的官方网站  [http://www.cs.wustl.edu/~schmidt/ACE.html](http://www.cs.wustl.edu/~schmidt/ACE.html)
5. 马维达的主页 [http://www.flyingdonkey.com/](http://www.flyingdonkey.com/)
6. RIVERACE公司 [http://www.riverace.com](http://www.riverace.com/)
7. 灰狐          [http://www.huihoo.com/](http://www.huihoo.com/)
8.     ACE类库    [http://dev.csdn.net/article/40/40700.shtm](http://dev.csdn.net/article/40/40700.shtm)
常见错误
错误1：
“Cannot open include file: ace/SOCK_Acceptor.h: No such file        or directory”
解决方法：
Project->settings->C/C++: Preprocessor的Additional include directories中加入$ACE_ROOT，如E:/ACE_wrappers 
错误2：
“You must link against multi-threaded libraries when using ACE (check your project settings)”
解决方法：
Project->settings->C/C++: Code Generation 的Use run-time library : Debug Multithreaded Dll
错误3：
“error C2065: ACE_ERROR : undeclared identifier”
“error C2065: ACE_DEBUG : undeclared identifier”
解决方法：
在原马中加入#include ace/Log_Msg.h
错误4：“error C4716: ace_main_i : must return a value”
解决方法：
在main中加入return 0;
错误5：
“error LNK2001: unresolved external symbol __declspec(dllimport) int __cdecl”
解决方法：
Project->settings->Link->Input: Object/library Modules加入aced.lib ，Additional Library Path中加入$ACE_ROOT/ace，如E:/ACE_wrappers/ace 
编译client中遇到的问题:
错误6：
error C2039: sprintf : is not a member of ACE_OS
解决方法：#include ace/OS_NS_stdio.h
错误7：error C2039: strlen : is not a member of ACE_OS
解决方法：#include ace/OS_NS_string.h
错误8：无法找到动态链接库aced.dll于指定的路径
解决方法：为系统变量PATH加上D:/ACE_wrappers/bin 
静态包含：
   工程-设置-C/C++-预处理程序定义中加入 ,ACE_AS_STATIC_LIBS
例１server编译后生成
server.exe 16K   serverd.exe 97K     servers.exe 124K serversd.exe 669K
（servers.exe较适合发布）
例2 client编译后生成
client.exe 16K   clientd.exe 97K     clients.exe 124K clientsd.exe 669K
（clients.exe较适合发布）
测试 clients 127.0.0.1 1919
       servers 1919
如果编译器支持64位，通过什么宏定义来区分64位还是32位阿？
答：#define ver ((2>>31)>0?64:32)
-------------------------------------------------------
给ACE初哥初妹的话
给ACE初哥初妹的话
version 0.1.0 Stone jIang 2006-11-6
下面列出了初学ACE必要的基础知识，希望能给初学ACE的朋友们一点帮助。特加上版本号，这是为了欢迎批判，补充和修订。
1. C++/OOP　（有一部分高手是C语言背景根深蒂固的，思考问题不是面向对象，这类人学ACE需要先补补OOP思想)
2. Design Pattern (ACE不仅仅是类库，而是通过模式协同在一起的一系列相关的类，如果对设计模式熟悉，那么会用助于学习ACE)
3. Framework (Framework应该还不正式，相似的词还可以是architecture,不管用啥词语，所要指出的是，ACE不仅仅是类库，更是通过模式组合在一 起相互协作的类形成的半成品。框架有别于类库，后者通常是被动的被程序员写的代码调用，而框架则往往主动地调用用户实现的接口，即著名的好莱坞原则—— Don't call me , I will call you)
4.Linux(我是Linux初不者，但自从我开始进入Linux 编程的时候，我就发现,ACE许多精华源自Linux，以前对ACE理解有限，不知道为什么这样，其实在Linux中能找到答案，ACE里闪烁着Linux的智慧。)
5. Richard Stevens. (英年早逝的Richard,应该是Douglas的良师益友，他的著作影响了Douglas,也就影响着ACE,这里要说的是TCP/IP Illustrated ，其实Richard其他著作，像APUE都是大滋大补的）
6. 没有6了。如果你认为有，一定告诉我。
引用:http://blog.csdn.net/adcxf/archive/2008/04/24/2321005.aspx
