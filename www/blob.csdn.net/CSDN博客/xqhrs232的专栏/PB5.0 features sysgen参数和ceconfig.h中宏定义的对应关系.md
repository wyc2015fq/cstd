# PB5.0 features/sysgen参数和ceconfig.h中宏定义的对应关系 - xqhrs232的专栏 - CSDN博客
2015年07月10日 11:36:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：744
原文地址::[http://blog.csdn.net/luixing67/article/details/5701350](http://blog.csdn.net/luixing67/article/details/5701350)
使用PB建立一个wince image之前先要建立一个workspace来控制除OEM BSP之外的所有的image的属性，包括的内容等等。这些内容主要是通过PB的catalog来管理的，通过PB的viewàcatalog打开的视窗，我们从全部的window ce的可选组件中选择我们需要的加入到workspace中，这样的话我们sysgen出来的image就具备了相应的功能。从catalog的视窗中，我们可以通过查询任意一个组件的属性来获得它唯一对应的一个sysgen_开头的变量。但是这些变量并不能直接用来在runtime的情况下用于判断image具备哪些组件。在实际的运用中，AP可以＃include一个名为“ceconfig.h”的头文件，这个头文件里面都是类似”CE_MOULDLE_*** SHELL_MODULES_*** COREDLL_****”之类的宏定义，这些宏定义可以直接被AP用来判定image内是否具备其需要的一些组件，比如MFC，AYGSHELL等等。Ceconfig.h是在sysgen的阶段自动由系统的批处理生成的，由于ceconfig.h的重要性，所以有必要搞清楚通过PB的catalog生成的sysgen_环境变量是如何和ceconfig.h内部的宏对应起来的。
       首先从PB开发环境的OSDesignView中修改的feature将直接反映到以sysgen_开头的环境变量中，为了验证有哪些sysgen_已经被设置，可以执行Build OSàOpen release directory进入命令行模式，然后执行“set sysgen”命令，就可以看到所有的sysgen_XXX的环境变量了。这些环境变量是属于桌面windows的，可以dos下或是bat批处理文件中使用。由于dos下的命令也是支持一些条件判断语法，如IF
 ELSE等指令，所以利用dos批处理文件就可以根据既有的环境变量做进一步的处理了。
       现在来介绍微软是如何利用bat批处理对sysgen_环境变量做转换，变成cecongfig.h中的宏定义的。
首先来看cesysgen.bat，这个批处理文件每次在Build OsàSysgen的时候会被调用，其位于“D:/WINCE500/PUBLIC/CEBASE/OAK/MISC”（假设wince500安装在D盘）。
在这个目录下你可以看到很多bat文件：
D:/WINCE500/PUBLIC/CEBASE/OAK/MISC 的目录
cebase.bat、cesysgen.bat、datasync.bat、dcom.bat、directx.bat、gdiex.bat、ie.bat、netcf.bat、netcfv2.bat、rdp.bat、script.bat、servers.bat、shell.bat、shellsdk.bat、speech.bat、sqlce.bat、sqlmobile.bat、viewers.bat、voip.bat、wceappsfe.bat、wceshellfe.bat、winceos.bat
后面我们会知道，其他的bat批处理文件都会被cesysgen.bat调用到的。
调用“D:/WINCE500/PUBLIC/CEBASE/OAK/MISC >cesysgen report”的时候，你可以看到一些类似ceconfig.h中的宏定义的字符串：
Report:
SYSGEN_ASYNCMAC=1
SYSGEN_AS_BASE=1
SYSGEN_AUDIO=1
…….. 省略
CE_MODULES=coredll nk msim commctrl commdlg rsaenh ….. 省略 (和ceconfig.h中的CE_MODULES_XXX一一对应）
COREDLL_COMPONENTS=coremain lmem showerr thunks correct ….. 省略(和ceconfig.h中的COREDLL _XXX一一对应）
……
这说明，在这个批处理里面已经有了sysgen->宏定义的转换了。
用ultraedit打开cesysgen.bat，跟踪report参数，可以走到113行，看到下面的命令：
113 :doReport    #标号
114 echo Report: #语句显示Report
115 set SYSGEN ＃显示所有的sysgen_环境变量
116 if "%__IN_BLD_RECURS%"=="1" set SYSGEN >%_WINCEROOT%/__BLD_RECURS_DEP.txt
117 set SYSGEN >%_WINCEROOT%/public/%_TGTPROJ%/__CEBASE_FEATURES.txt
117 for %%f in (%_DEPTREES%) do call :CallProjFile %%f report
主要看117行，这里是一个for循环，f是变量，%_DEPTREES%是f可以取值的范围，这个语句将%_DEPTREES%中的每个字符串作为参数传递给CallProjFile子函数（批处理标号）。为了了解%_DEPTREES%中有那些字符串，可以执行echo %_DEPTREES%，你会看到类似下面的一串字符：
winceos dcom gdiex ie script servers shellsdk shell rdp wceshellfe wceappsfe viewers directx voip datasync netcf SQLCE ostest speech NETCFV2 SQLMOBILE PROJECTNAME
基本上，上面的字符串和D:/WINCE500/PUBLIC/CEBASE/OAK/MISC目录下的每一个bat文件是一一对应的。 而且大致可以看出，它们的名字就是wince所有组件归类之后的组名。
跟踪到:CallProjFile标号，大概在156行，有如下内容：
:CallProjFile       
    if not exist %__CESYSGEN_PATH%/%1.bat goto :CPF_NotCE
        call %__CESYSGEN_PATH%/%1.bat %2 %3 %4 %5 %6 %7 %8 %9
        goto :EOF
   :CPF_NotCE
    if not exist %_PUBLICROOT%/cebase/oak/misc/%1.bat goto :CPF_NotCE2
        call %_PUBLICROOT%/cebase/oak/misc/%1.bat %2 %3 %4 %5 %6 %7 %8 %9
        goto :EOF
    :CPF_NotCE2
    if not exist %_PUBLICROOT%/%1/cebasecesysgen.bat goto :CPF_Unknown
        call %_PUBLICROOT%/%1/cebasecesysgen.bat %2 %3 %4 %5 %6 %7 %8 %9
        goto :EOF
    :CPF_Unknown
    if /i "%___PUBLICPROJECT%"=="common" if "%_ECHOON%"=="1" echo WARNING: Unable to find project file %1, args %2 %3 %4 %5 %6 %7 %8 %9
    goto :EOF
大致的功能就是到三个路径去寻找 “参数1.bat“ 的批处理文件，如果找到并执行之，首先根据%_DEPTREES%可知，第一个参数是winceos，所以我们在D:/WINCE500/PUBLIC/CEBASE/OAK/MISC下面的winceos.bat会被执行。依次类推，dcom.bat gdiex.bat ie.bat script.bat servers.bat ….这些批处理会顺序执行。
接下来就到了揭开谜底的时候了，打开winceos.bat. 你可以发现有很多类似
if "%SYSGEN_XXX%"=="1" set CE_MODULES=%CE_MODULES% YYY的语句，这XXX和YYY的对应关系就是sysgen_和ceconfig的宏定义之间的关系。
所以类似的，打开其他的.bat文件可以分析其他类别的XXXàYYY的对应了。
这样你可以search到所有的，你想知道的对应关系。
引用本文请注明出处：http://blog.csdn.net/fredzeng
