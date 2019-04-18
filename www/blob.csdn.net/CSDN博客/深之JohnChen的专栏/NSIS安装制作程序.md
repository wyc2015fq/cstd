# NSIS安装制作程序 - 深之JohnChen的专栏 - CSDN博客

2009年09月14日 14:43:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2962标签：[function																[components																[脚本																[file																[windows																[installer](https://so.csdn.net/so/search/s.do?q=installer&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=components&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)
个人分类：[开源软件																[技术资料](https://blog.csdn.net/byxdaz/article/category/144574)](https://blog.csdn.net/byxdaz/article/category/577652)


NSIS（Nullsoft Scriptable Install System）是一个开源的 Windows 系统下安装程序制作程序。它提供了安装、卸载、系统设置、文件解压缩等功能。这如其名字所指出的那样，NSIS 是通过它的脚本语言来描述安装程序的行为和逻辑的。NSIS 的脚本语言和通常的编程语言有类似的结构和语法，但它是为安装程序这类应用所设计的。

**1. NSIS****的主要特点：**

—开销小，一个完整功能的安装程序仅需要 34k 的额外开销。

—支持大多数 Windows 平台，包括：Windows 9.x，Windows NT，Windows 2000，Windows XP，Windows 2003 

—支持三大压缩算法：Zlig，BZips， LZMA 

—支持脚本

—支持多语言

—支持安装界面定制

—提供可扩展的插件接口

—支持网络安装、补丁

—支持无人值守的安装模式

—此外，NSIS的license允许任何用途免费使用。

**2. ****开发一个****NSIS****的安装程序通常有以下几步：**

—确定安装的功能和界面元素

—编写 NSIS 脚本

—使用 NSIS 提供的 makensis 或者 makensisw 程序，将步骤 2 编写的脚本编译成可执行的安装程序

—调试安装程序，如果有问题退到第二步重复

—随着 NSIS 的流行，有一些第三方的 NSIS 脚本开发环境出现了，如 HMNISEdit，Venis IX。

　　前者是完全开源的，后者仅对个人和非商业用途免费。在这些集成开发环境下，步骤2，3可以方便的组合在一起。

**3. NSIS ****脚本的结构**

3.1 NSIS 脚本（下称 nsi 脚本）主要包含安装程序属性、页面、区段、函数。

　　属性用来定义安装程序的行为和界面风格，这些属性大部分是编译时刻属性，即不能在运行时刻改变。

　　页面是指安装程序的向导页面。

　　示例：

Page license 

Page components 

Page directory 

Page instfiles 

UninstPage uninstConfirm 

UninstPage instfiles

3.2 区段是对应某种安装/卸载选项的处理逻辑，该段代码仅当用户选择相应的选项才被执行。

　　卸载程序的区段名用"un."作　为前缀。

　　示例：

Section "Installer Section"

SectionEnd

Section "un.Uninstaller Section"

SectionEnd

3.3 在区段中可以使用很多指令用来完成诸如解压缩文件、读写注册表、创建目录、创建快捷方式等任务。

　　但最常用的指令是 SetOutPath 和 File。前者用于指定目的位置，后者用于指定文件。

　　示例：

Section "My Program" 

SetOutPath $INSTDIR 

File "My Program.exe" 

File "Readme.txt" 

SectionEnd

3.4 区段名的修饰符“/o”表示该区段默认不选上，“-”表示隐藏区段（匿名区段也是隐藏区段），“!”表示需要粗体显示的区段。

SectionIn 表示该区段和安装类型之间的关系：

SectionIn insttype_index [RO] 

RO 修饰符表示不可修改。

3.5 子区段用于包含多个区段：

SubSection [/e] Caption [subsection_name index output]

　　修饰符“/e”用于该子区段的所有区段是否默认展开。

3.6 函数包含了模块化的安装逻辑，在 nsi 脚本中函数分为两种：用户自定义函数和回调函数。

　　用户自定义函数仅当是 Call 指令调用时才被执行。

　　如果函数体中没有abort语句，则安装程序执行完了用户自定义函数，继续运行 Call 语句和指令。

　　用户自定义函数的语法如下：

Function <函数名> 

# some commands 

FunctionEnd

　　函数的调用则使用以下语法：

Call <函数名> 

　　可见无论是函数的定义还是函数的调用都没有参数传递。

　　通常 nsi 的参数传递是通过堆栈操作 Pop，Push 和 20 个寄存器变量 $0～$9、$R0～$R9 进行的。

　　也可以通过全局变量完成参数传递。

　　如：

Var input ; 

Var output ; 

Section bla 

DeteailPrint "input is $input$/n" 

Call square 

DeteailPrint "square of $input is $output$/n" 

SectionEnd 

Function square 

output = input^2 

FunctionEnd 

　　回调函数则是由在特定的时间点触发的程序段。常用的回调函数如 .onInit：

Function .onInit 

MessageBox MB_YESNO "This will install My Program. Do you wish to continue?" IDYES gogogo 

Abort 

gogogo: 

FunctionEnd 

NSIS 对于安装逻辑定义以下回调函数：

.onGUIInit

.onInit

.onInstFailed

.onInstSuccess

. onGUIEnd

.onMouseOverSection

.onRebootFailed

.onSelChange

.onUserAbort

.onVerifyInstDir

NSIS 对于卸载逻辑定义以下回调函数：

un.onGUIInit

un.onInit

un.onUninstFailed

un.onUninstSuccess

un.onGUIEnd

un.onRebootFailed

un.onUserAbort

**4. nsi ****脚本的变量定义**

nsi 脚本的变量定义用 Var 关键字，后跟变量名，变量是全局的并且是大小写敏感的。变量引用时需要加上前缀“$”。

　　除了用户自定义的变量外，nsi 脚本中与定义寄存器变量 $0～$9、$R0～$R9 用于参数传递，以及系统变量用于特定用途。

　　这些变量主要有：

$INSTDIR，$OUTDIR，$CMDLINE，$LANGUAGE 这些变量都是可写的。

$PROGRAMFILES，$COMMONFILES，$DESKTOP，$EXEDIR，$WINDIR，$SYSDIR，$ TEMP，$STARTMENU

$SMPROGRAMS，$SMSTARTUP，$QUICKLAUNCH，$DOCUMENTS，$SENDTO，$ RECENT，$FAVORITES

$MUSIC，$PICTURES，$VIDEOS，$NETHOOD，$FONTS，$TEMPLATES，$ APPDATA，$PRINTHOOD

$INTERNET_CACHE，$COOKIES，$HISTORY，$PROFILE，$ ADMINTOOLS，$RESOURCES

$RESOURCES_LOCALIZED，$CDBURN_AREA，$HWNDPARENT，$PLUGINSDIR。

nsi 脚本中可用于调试的系统函数有 MessageBoxes、DetailPrint、Dumpstate。

**5. nsi****脚本的编译器指令**

nsi 脚本的编译器指令主要指仅在编译时刻执行的命令。

　　这些命令主要用来包含文件、条件化编译、定义常量、定义宏等。

　　定义常量和宏是编译器指令最主要应用。

　　定义常量的示例：

!define VERSION "1.0.3" 

Name "My Program $" 

OutFile "My Program Installer - $.exe" 

　　定义宏的示例：

!macro MyFunc UN 

Function $MyFunc 

Call $DoRegStuff 

ReadRegStr HKLM Software/MyProgram key 

DetailPrint 

FunctionEnd

**6. Modern UI**

Modern UI 是感观上模仿最新的 Windows 系统的界面风格，它由欢迎页面、结束页面和其他向导页面构成。

**7. ****插件**

nsi 支持插件，通过插件可以方便的扩展 NSIS 安装程序的功能。

NSIS 插件是用 C++、Delphi 等语言编写的 dll，在 nsi 脚本中调用 nsi 中的函数使用如下语法：

DLLName::FunctionName "参数1" "参数2" "参数3" 

　　示例1：

nsExec::ExecToLog '"$/makensis.exe" /CMDHELP'

　　执行makensis.exe命令，显示该命令用法。

　　示例2：

InstallOptions::dialog "$PLUGINSDIR/test.ini"

　　显示对话框

　　示例3：

NSISdl::download http://download.nullsoft.com/winamp/client/winamp291_lite.exe $R0

　　下载文件

**8. NSIS ****搜索插件的策略**

　　默认情况下 NSIS 在其安装目录的子目录 Plugins 中搜索插件，用户可以使用 !addplugindir 指定增加插件的目录位置。

**9. nsi ****脚本的基本语法**

9.1 注释

　　单行注释用井号“#”或分号“;”，跨行注释用可以用“C/C++”中注释语法。

9.2 数据类型

9.2.1数字

　　数字常量可以用十进制、十六进制（0x为前缀）、八进制（0为前缀）表示。

　　颜色用类似 HTML 的中 RGB 表示法，但去　井号“#”。

9.2.2字符串

　　字符串常量可以用引号引用，转意字符用"$/"作前缀。

　　美元符号、常用转意字符换行、回车、制表符的nsi语法表示分别为：$$,$/n,$/r,$/t 

9.2.3续行符

nsi 脚本用行尾的反斜杠“/”表示下一行和当前行逻辑上是同一行。

9.2.4默认头文件

　　如果在makensis同目录下有nsisconf.nsh文件，该文件会被自动包含，除非编译时指定/NOCONFIG选项。

9.2.5标号

nsi 使用 GOTO 语句和 IfErrors、MessageBox、IfFileExists 及 StrCmp 进行程序控制流表示，标号是这些语句的目标语句。

　　标号定义的语法：标号：语句

　　标号必须定义在函数和区段中，其作用范围仅限于定义它的区段或函数。以点号“.”开头的标号是全局标号。

9.3 相对跳转

nsi 脚本常常使用相对跳转表示条件分枝，其语法是 [+-][1-9]，加号表示从当前位置往前跳转，减号则表示从当前位置往后跳转。

　　数字表示跳转的语句条数。

　　示例：

Goto +4 

MessageBox MB_OK "The following message will be skipped" 

Goto +3 

MessageBox MB_OK "You will never ever see this message box" 

Goto -3 

MessageBox MB_OK "Done"

**10. ****页面**

　　向导页面是 NSIS 安装程序中最重要的界面元素，在 nsi 脚本中可以使用 NSIS 内置页面或者定制界面，通过脚本可以指定页面的顺序、显示样子和行为。Page指令用来定义安装程序中的页面，UninstPage用来定义，此外PageEx指令提供类是功能，但提供更多选项。页面的顺序和它在nsi脚本中出现的次序一致。

　　示例：

Page license 

Page components 

Page directory 

Page instfiles 

UninstPage uninstConfirm 

UninstPage instfiles

　　规定安装程序首先显示license页面，然后显示components选择页面，接着显示安装目录选择页面。

10.1 页面选项

　　不同的页面有不同的选项：

License page 有 LicenseText、LicenseData、LicenseForceSelection；

Components selection 页面有 ComponentText；

Directory selection 页面有 DirText、DirVar (仅能在 PageEx 中使用)、DirVerify；

Un/Installation log 页面有 DetailsButtonText、CompletedText；

Uninstall confirmation 页面有 DirVar (仅能在 PageEx 中使用)、UninstallText。

　　对于内置的 Page，NSIS 支持三个回调函数用于定制界面和验证，对于自定义页面 NSIS 支持两个回调函数。

Page 指令语法：

Page license|components|directory|instfiles|uninstConfirm) [pre_function] [show_function] [leave_function] 

　　或者：

Page custom [creator_function] [leave_function] [caption] 

　　示例：

Page license skipLicense "" stayInLicense 

Page custom customPage "" ": custom page" 

Page instfiles

Function skipLicense 

MessageBox MB_YESNO "Do you want to skip the license page?" IDNO no 

Abort 

no: 

FunctionEnd 

Function stayInLicense 

MessageBox MB_YESNO "Do you want to stay in the license page?" IDNO no 

Abort 

no: 

FunctionEnd

Function customPage 

GetTempFileName $R0 

File /oname=$R0 customPage.ini 

InstallOptions::dialog $R0 

Pop $R1 

StrCmp $R1 "cancel" done 

StrCmp $R1 "back" done 

StrCmp $R1 "success" done 

error: MessageBox MB_OK|MB_ICONSTOP "InstallOptions error:$/r$/n$R1" 

done: 

FunctionEnd 

UninstPage 指令语法

UninstPage custom [creator_function] [leave_function] [caption] 

　　或

UninstPage (license|components|directory|instfiles|uninstConfirm) [pre_function] [show_function] [leave_function] 

PageEx 语法

PageEx 使用嵌套结构，比如：

PageEx license 

LicenseText "Readme" 

LicenseData readme.rtf 

PageCallbacks licensePre licenseShow licenseLeave 

PageExEnd

**11. ****常用的**** nsi ****指令**

nsi 大致可以分为基本指令、注册表及 ini 操作指令、通用指令、流程控制指令、文件操作指令、卸载指令、字符串处理指令、多语言支持指令、重启指令。

　　以下是常用的基本指令：

(1) Delete：

Delete [/REBOOTOK] file 

(2) Exec：

Exec command 

(3) ExecShell：

ExecShell action command [parameters] [SW_SHOWNORMAL | SW_SHOWMAXIMIZED | SW_SHOWMINIMIZED | SW_HIDE]ExecShell "open" 示例"http://nsis.sf.net/" 

ExecWait 

ExecWait command 

　　示例：

ExecWait '"$INSTDIR/someprogram.exe"' 

ExecWait '"$INSTDIR/someprogram.exe"' 

DetailPrint "some program returned " 

(4) File：

File [/nonfatal] [/a] ([/r] [/x file|wildcard [...]] (file|wildcard) [...] | /oname=file.dat infile.dat) 

/r选项用作递归模式，/x用于排出文件

　　示例：

File something.exe 

File /a something.exe 

File *.exe 

File /r *.dat 

File /r data 

File /oname=$TEMP/temp.dat somefile.ext 

File /nonfatal "a file that might not exist" 

File /r /x CVS myproject 

File /r /x *.res /x *.obj /x *.pch source 

(5) Rename：

Rename [/REBOOTOK] source_file dest_file

(6) RMDir：

RMDir [/r] [/REBOOTOK] directory_name

NSIS发布版本与源代码下载：[http://nsis.sourceforge.net/Download](http://nsis.sourceforge.net/Download)

NSIS安装包制作教程详细版：[http://download.csdn.net/source/664095](http://download.csdn.net/source/664095)

NSIS入门教程：http://blog.163.com/nine_skys/blog/static/293423252007991104695/


