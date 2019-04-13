
# 在cmd命令行下使用VS Build Tools 编译运行C-C++源文件 - cocoonyang的专栏 - CSDN博客


2015年08月31日 16:37:35[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：8746



# Introduction
The following tools can help you to build a C/C++ project on the command line[10] .
CL
Use the compiler (cl.exe) to compile and link source code files into apps, libraries, and DLLs.
Link
Use the linker (link.exe) to link compiled object files and libraries into apps and DLLs.
MSBuild (Visual C++)
Use MSBuild (msbuild.exe) to build Visual C++ projects and Visual Studio solutions. This is equivalent to running the Build project or Build Solution command in the Visual Studio IDE.
DEVENV
Use DEVENV (devenv.exe) combined with a command-line switch—for example, /Build or /Clean—to perform certain build commands without displaying the Visual Studio IDE.
NMAKE
Use NMAKE (nmake.exe) to automate tasks that build Visual C++ projects by using a traditional makefile.
**安装**：
安装Visual Studio或Microsoft Visual C++ Build Tools。 我安装的是Visual Studio 2010 Express。

# CL
cl.exe is a tool that controls the Microsoft Visual C++ (MSVC) C and C++ compilers and linker. The compilers produce Common Object File Format (COFF) object (.obj) files. The linker produces executable (.exe) files or dynamic-link libraries (DLLs).
**cl编译器命令选项**

```python
C/C++ COMPILER OPTIONS
                             -CODE GENERATION-
/GF enable read-only string pooling     /Gm[-] enable minimal rebuild
/Gy[-] separate functions for linker    /GS[-] enable security checks
/GR[-] enable C++ RTTI                  /GX[-] enable C++ EH (same as /EHsc)
/EHs enable C++ EH (no SEH exceptions)  /EHa enable C++ EH (w/ SEH exceptions)
/EHc extern "C" defaults to nothrow     
/fp:<except[-]|fast|precise|strict> choose floating-point model:
    except[-] - consider floating-point exceptions when generating code
    fast - "fast" floating-point model; results are less predictable
    precise - "precise" floating-point model; results are predictable
    strict - "strict" floating-point model (implies /fp:except)
/Qfast_transcendentals generate inline FP intrinsics even with /fp:except
/GL[-] enable link-time code generation /GA optimize for Windows Application
/Ge force stack checking for all funcs  /Gs[num] control stack checking calls
/Gh enable _penter function call        /GH enable _pexit function call
/GT generate fiber-safe TLS accesses    /RTC1 Enable fast checks (/RTCsu)
/RTCc Convert to smaller type checks    /RTCs Stack Frame runtime checking
/RTCu Uninitialized local usage checks  
/clr[:option] compile for common language runtime, where option is:
    pure - produce IL-only output file (no native executable code)
    safe - produce IL-only verifiable output file
    oldSyntax - accept the Managed Extensions syntax from Visual C++ 2002/2003
    initialAppDomain - enable initial AppDomain behavior of Visual C++ 2002
    noAssembly - do not produce an assembly
/Gd __cdecl calling convention          /Gr __fastcall calling convention
/Gz __stdcall calling convention        /GZ Enable stack checks (/RTCs)
/QIfist[-] use FIST instead of ftol()   
/hotpatch ensure function padding for hotpatchable images
/arch:<SSE|SSE2|AVX> minimum CPU architecture requirements, one of:
    SSE - enable use of instructions available with SSE enabled CPUs
    SSE2 - enable use of instructions available with SSE2 enabled CPUs
    AVX - enable use of Intel(R) Advanced Vector Extensions instructions
/Qimprecise_fwaits generate FWAITs only on "try" boundaries, not inside "try"
/Qsafe_fp_loads generate safe FP loads  
                              -OUTPUT FILES-
/Fa[file] name assembly listing file    /FA[scu] configure assembly listing
/Fd[file] name .PDB file                /Fe<file> name executable file
/Fm[file] name map file                 /Fo<file> name object file
/Fp<file> name precompiled header file  /Fr[file] name source browser file
/FR[file] name extended .SBR file       /Fi[file] name preprocessed file
/doc[file] process XML documentation comments and optionally name the .xdc file
                              -PREPROCESSOR-
/AI<dir> add to assembly search path    /FU<file> forced using assembly/module 
/C don't strip comments                 /D<name>{=|#}<text> define macro
/E preprocess to stdout                 /EP preprocess to stdout, no #line
/P preprocess to file                   /Fx merge injected code to file
/FI<file> name forced include file      /U<name> remove predefined macro
/u remove all predefined macros         /I<dir> add to include search path
/X ignore "standard places"             
                                -LANGUAGE-
/Zi enable debugging information        /Z7 enable old-style debug info
/Zp[n] pack structs on n-byte boundary  /Za disable extensions
/Ze enable extensions (default)         /Zl omit default library name in .OBJ
/Zg generate function prototypes        /Zs syntax check only
/vd{0|1|2} disable/enable vtordisp      /vm<x> type of pointers to members
/Zc:arg1[,arg2] C++ language conformance, where arguments can be:
    forScope[-] - enforce Standard C++ for scoping rules
    wchar_t[-] - wchar_t is the native type, not a typedef
    auto[-] - enforce the new Standard C++ meaning for auto
    trigraphs[-] - enable trigraphs (off by default)
/ZI enable Edit and Continue debug info 
/openmp enable OpenMP 2.0 language extensions
                              -MISCELLANEOUS-
@<file> options response file           /?, /help print this help message
/bigobj generate extended object format /c compile only, no link
/errorReport:option Report internal compiler errors to Microsoft
    none - do not send report                
    prompt - prompt to immediately send report
    queue - at next admin logon, prompt to send report (default)
    send - send report automatically         
/FC use full pathnames in diagnostics   /H<num> max external name length
/J default char type is unsigned        
/MP[n] use up to 'n' processes for compilation
/nologo suppress copyright message      /showIncludes show include file names
/Tc<source file> compile file as .c     /Tp<source file> compile file as .cpp
/TC compile all files as .c             /TP compile all files as .cpp
/V<string> set version string           /w disable all warnings
/wd<n> disable warning n                /we<n> treat warning n as an error
/wo<n> issue warning n once             /w<l><n> set warning level 1-4 for n
/W<n> set warning level (default n=1)   /Wall enable all warnings
/WL enable one line diagnostics         /WX treat warnings as errors
/Yc[file] create .PCH file              /Yd put debug info in every .OBJ
/Yl[sym] inject .PCH ref for debug lib  /Yu[file] use .PCH file
/Y- disable all PCH options             /Zm<n> max memory alloc (% of default)
/Wp64 enable 64 bit porting warnings    
                                -LINKING-
/LD Create .DLL                         /LDd Create .DLL debug library
/LN Create a .netmodule                 /F<num> set stack size
/link [linker options and libraries]    /MD link with MSVCRT.LIB
/MT link with LIBCMT.LIB                /MDd link with MSVCRTD.LIB debug lib
/MTd link with LIBCMTD.LIB debug lib
```
**优化选项**
|Parameter|Function|功能|
|/O1|maximize speed|速度最大化|
|/O2|minimize space|空间最小化|
|/Ob|inline expansion (default n=0)|内联展开(默认 n=0)|
|/Od|disable optimizations (default)|禁用优化(默认值)|
|/Og|enable global optimization|启用全局优化|
|/Oi[-]|enable intrinsic functions|启用内部函数|
|/Os|favor code space|代码空间优先|
|/Ot|favor code speed|代码速度优先|
|/Ox|maximum optimizations|最大优化|
|/Oy[-]|enable frame pointer omission|可省略框架指针|
```python
-代码生成- 
/G3 为 80386 进行优化                    /Gh 启用 _penter 函数调用 
/G4 为 80486 进行优化                    /GH 启用 _pexit 函数调用 
/G5 为 Pentium 进行优化                  /GR[-] 启用 C++ RTTI 
/G6 对 PPro、P-II、P-III 进行优化        /GX[-] 启用 C++ EH (与 /EHsc 相同) 
/G7 对 Pentium 4 或 Athlon 进行优化      /EHs 启用 C++ EH (没有 SEH 异常) 
/GB 为混合模型进行优化(默认)              /EHa 启用 C++ EH(w/ SEH 异常) 
/Gd __cdecl 调用约定                     /EHc extern "C" 默认为 nothrow 
/Gr __fastcall 调用约定                  /GT 生成纤维安全   TLS 访问 
/Gz __stdcall 调用约定                   /Gm[-] 启用最小重新生成 
/GA 为 Windows 应用程序进行优化           /GL[-] 启用链接时代码生成 
/Gf 启用字符串池                          /QIfdiv[-] 启用 Pentium FDIV 修复 
/GF 启用只读字符串池                      /QI0f[-] 启用 Pentium 0x0f 修复 
/Gy 分隔链接器函数                        /QIfist[-] 使用 FIST 而不是 ftol() 
/GZ 启用堆栈检查(/RTCs)                   /RTC1 启用快速检查(/RTCsu) 
/Ge 对所有函数强制堆栈检查                  /RTCc 转换为较小的类型检查 
/Gs[num]   控制堆栈检查调用                /RTCs 堆栈帧运行时检查 
/GS 启用安全检查                           /RTCu 未初始化的本地用法检查 
/clr[:noAssembly] 为公共语言运行库编译 
           noAssembly - 不产生程序集 
/arch:<SSE|SSE2> CPU 结构的最低要求，以下内容之一: 
                        SSE - 启用支持 SSE 的 CPU 可用的指令 
                        SSE2 - 启用支持 SSE2 的 CPU 可用的指令 
-输出文件- 
/Fa[file] 命名程序集列表文件                /Fo<file> 命名对象文件 
/FA[sc] 配置程序集列表                      /Fp<file> 命名预编译头文件 
/Fd[file] 命名 .PDB 文件                   /Fr[file] 命名源浏览器文件 
/Fe<file> 命名可执行文件                    /FR[file] 命名扩展 .SBR 文件 
/Fm[file] 命名映射文件 
-预处理器- 
/AI<dir> 添加到程序集搜索路径                  /Fx 将插入的代码合并到文件 
/FU<file> 强制使用程序集/模块                  /FI<file> 命名强制包含文件 
/C 不抽出注释                                 /U<name> 移除预定义宏 
/D<name>{=|#}<text> 定义宏                    /u 移除所有预定义宏 
/E 预处理到 stdout                            /I<dir> 添加到包含搜索路径 
/EP 预处理到 stdout，没有 #line                /X 忽略“标准位置” 
/P 预处理到文件 
-语言- 
/Zi 启用调试信息                             /Ze 启用扩展(默认) 
/ZI 启用“编辑并继续”调试信息                  /Zl 省略 .OBJ 中的默认库名 
/Z7 启用旧式调试信息                          /Zg 生成函数原型 
/Zd 仅有行号调试信息                          /Zs 只进行语法检查 
/Zp[n] 在 n 字节边界上包装结构                 /vd{0|1} 禁用/启用 vtordisp 
/Za 禁用扩展(暗指 /Op)                         /vm<x> 指向成员的指针类型 
/Zc:arg1[,arg2] C++ 语言一致性，这里的参数可以是: 
                      forScope - 对范围规则强制使用标准 C++ 
                      wchar_t - wchar_t 是本机类型，不是 typedef 
- 杂项 - 
@<file> 选项响应文件                                  /wo<n> 发出一次警告 n 
/?, /help 打印此帮助消息                              /w<l><n> 为 n 设置警告等级 1-4 
/c 只编译，不链接                                     /W<n> 设置警告等级(默认 n=1) 
/H<num> 最大外部名称长度                                    /Wall 启用所有警告 
/J 默认 char 类型是 unsigned                               /Wp64 启用 64 位端口定位警告 
/nologo 取消显示版权消息                                  /WX 将警告视为错误 
/showIncludes 显示包含文件名                            /WL 启用单行诊断 
/Tc<source file> 将文件编译为 .c                          /Yc[file] 创建 .PCH 文件 
/Tp<source file> 将文件编译为 .cpp                   /Yd 将调试信息放在每个 .OBJ 中 
/TC 将所有文件编译为 .c                                       /Yl[sym] 为调试库插入 .PCH 引用 
/TP 将所有文件编译为 .cpp                                  /Yu[file] 使用 .PCH 文件 
/V<string> 设置版本字符串                                  /YX[file] 自动 .PCH 
/w 禁用所有警告                                       /Y- 禁用所有 PCH 选项 
/wd<n> 禁用警告 n                                               /Zm<n> 最大内存分配(默认为 %) 
/we<n> 将警告 n 视为错误 
-链接- 
/MD 与 MSVCRT.LIB 链接                                   /MDd 与 MSVCRTD.LIB 调试库链接 
/ML 与 LIBC.LIB 链接                                        /MLd 与 LIBCD.LIB 调试库链接 
/MT 与 LIBCMT.LIB 链接                                     /MTd 与 LIBCMTD.LIB 调试库链接 
/LD 创建 .DLL                                          /F<num> 设置堆栈大小 
/LDd 创建 .DLL 调试库                                       /link [链接器选项和库]
```

## 样例
### CL使用样例1 -- Hello world
**操作步骤[1]**：
1）编辑源代码文件。例如 D:\foo\main.c
2）启动cmd。  start -> app -> Microsoft Visual Studio 2010 Express -> Visual Studio Command Prompt (2010)
3）使用 cd 命令改变目录，进入源代码文件所在目录，例如d:\foo。
4）在cmd中输入命令  cl main.c，回车。
运行结果：
```python
Setting environment for using Microsoft Visual Studio 2010 x86 tools.
C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC>d:
D:\>cd foo
D:\foo>cl main.c
Microsoft (R) 32-bit C/C++ Optimizing Compiler Version 16.00.40219.01 for 80x86
Copyright (C) Microsoft Corporation.  All rights reserved.
main.c
Microsoft (R) Incremental Linker Version 10.00.40219.01
Copyright (C) Microsoft Corporation.  All rights reserved.
/out:main.exe
main.obj
D:\foo>dir
 Volume in drive D has no label.
 Volume Serial Number is A8F3-9697
 Directory of D:\foo\foo
08/31/2015  03:55 PM    <DIR>          .
08/31/2015  03:55 PM    <DIR>          ..
08/16/2015  09:13 AM               446 main.c
08/31/2015  03:55 PM            45,568 main.exe
08/31/2015  03:55 PM             1,452 main.obj
               5 File(s)        118,150 bytes
               2 Dir(s)  111,024,001,024 bytes free
D:\foo>main.exe
Hello, world.
D:\foo>
```
**编译过程中可能出现的错误**：
1） 链接错误：fatal error LNK1104: cannot open file 'kernel32.lib'
错误处理： 向系统环境变量LIB添加kernel32.lib的保存路径。
链接器link找不过kernel32.lib的原因可能是环境变量LIB中未包含保存kernel32.lib的路径。  在我的机器上，kernel32.lib的保存路径为：C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib。
在我的电脑->属性->高级->环境变量->系统变量  中找到 系统变量LIB，如果找不到就新建系统变量LIB。将 C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib;  添加到系统变量LIB中。
2)  链接错误：error LINK2005: "..." already defined in xxx.obj
错误处理：
Adding static before the declaration of the variable or function

### CL使用样例2
```python
cl /EHsc hello.cpp
To compile a program that has multiple source code files, enter them all on the command line, like this:
cl /EHsc file1.cpp file2.cpp file3.cpp
When you supply multiple source files like this, the compiler uses the first input file to create the program name. In this case, it outputs a program called file1.exe. To change the name to program1.exe, add an /out linker option:
cl /EHsc file1.cpp file2.cpp file3.cpp /link /out:program1.exe
To catch more programming mistakes automatically, we recommend you compile by using either the /W3 or /W4 warning level option:
cl /W4 /EHsc file1.cpp file2.cpp file3.cpp /link /out:program1.exe
```

### CL使用样例3 -- 生成汇编代码文件
```python
cl /EHsc /FA foo.c
```

# LINK
**Link Options**[3]
|Option|Purpose|
|/ALIGN|Specifies the alignment of each section.|
|/ALLOWBIND|Specifies that a DLL cannot be bound.|
|/ALLOWISOLATION|Specifies behavior for manifest lookup.|
|/APPCONTAINER|Specifies whether the app must run within an appcontainer process environment.|
|/ASSEMBLYDEBUG|Adds the DebuggableAttribute to a managed image.|
|/ASSEMBLYLINKRESOURCE|Creates a link to a managed resource.|
|/ASSEMBLYMODULE|Specifies that a Microsoft intermediate language (MSIL) module should be imported into the assembly.|
|/ASSEMBLYRESOURCE|Embeds a managed resource file in an assembly.|
|/BASE|Sets a base address for the program.|
|/CGTHREADS|Sets number of cl.exe threads to use for optimization and code generation when link-time code generation is specified.|
|/CLRIMAGETYPE|Sets the type (IJW, pure, or safe) of a CLR image.|
|/CLRSUPPORTLASTERROR|Preserves the last error code of functions that are called through the P/Invoke mechanism.|
|/CLRTHREADATTRIBUTE|Specifies the threading attribute to apply to the entry point of your CLR program.|
|/CLRUNMANAGEDCODECHECK|Specifies whether the linker will apply the SuppressUnmanagedCodeSecurity attribute to linker-generated PInvoke stubs that call from managed code into native DLLs.|
|/DEBUG|Creates debugging information.|
|/DEBUGTYPE|Specifies which data to include in debugging information.|
|/DEF|Passes a module-definition (.def) file to the linker.|
|/DEFAULTLIB|Searches the specified library when external references are resolved.|
|/DELAY|Controls the delayed loading of DLLs.|
|/DELAYLOAD|Causes the delayed loading of the specified DLL.|
|/DELAYSIGN|Partially signs an assembly.|
|/DLL|Builds a DLL.|
|/DRIVER|Creates a kernel mode driver.|
|/DYNAMICBASE|Specifies whether to generate an executable image that can be randomly rebased at load time by using the address space layout randomization (ASLR) feature.|
|/ENTRY|Sets the starting address.|
|/errorReport|Reports internal linker errors to Microsoft.|
|/EXPORT|Exports a function.|
|/FIXED|Creates a program that can be loaded only at its preferred base address.|
|/FORCE|Forces a link to complete even with unresolved symbols or symbols defined more than once.|
|/FUNCTIONPADMIN|Creates an image that can be hot patched.|
|/GENPROFILE, /FASTGENPROFILE|Both of these options specify generation of a .pgd file by the linker to support profile-guided optimization (PGO). /GENPROFILE and /FASTGENPROFILE use different default parameters.|
|/GUARD|Enables Control Flow Guard protection.|
|/HEAP|Sets the size of the heap, in bytes.|
|/HIGHENTROPYVA|Specifies support for high-entropy 64-bit address space layout randomization (ASLR).|
|/IDLOUT|Specifies the name of the .idl file and other MIDL output files.|
|/IGNORE|Suppresses output of specified linker warnings.|
|/IGNOREIDL|Prevents the processing of attribute information into an .idl file.|
|/IMPLIB|Overrides the default import library name.|
|/INCLUDE|Forces symbol references.|
|/INCREMENTAL|Controls incremental linking.|
|/INTEGRITYCHECK|Specifies that the module requires a signature check at load time.|
|/KEYCONTAINER|Specifies a key container to sign an assembly.|
|/KEYFILE|Specifies a key or key pair to sign an assembly.|
|/LARGEADDRESSAWARE|Tells the compiler that the application supports addresses larger than two gigabytes|
|/LIBPATH|Specifies a path to search before the environmental library path.|
|/LTCG|Specifies link-time code generation.|
|/MACHINE|Specifies the target platform.|
|/MANIFEST|Creates a side-by-side manifest file and optionally embeds it in the binary.|
|/MANIFESTDEPENDENCY|Specifies a <dependentAssembly> section in the manifest file.|
|/MANIFESTFILE|Changes the default name of the manifest file.|
|/MANIFESTINPUT|Specifies a manifest input file for the linker to process and embed in the binary. You can use this option multiple times to specify more than one manifest input file.|
|/MANIFESTUAC|Specifies whether User Account Control (UAC) information is embedded in the program manifest.|
|/MAP|Creates a mapfile.|
|/MAPINFO|Includes the specified information in the mapfile.|
|/MERGE|Combines sections.|
|/MIDL|Specifies MIDL command-line options.|
|/NOASSEMBLY|Suppresses the creation of a .NET Framework assembly.|
|/NODEFAULTLIB|Ignores all (or the specified) default libraries when external references are resolved.|
|/NOENTRY|Creates a resource-only DLL.|
|/NOLOGO|Suppresses the startup banner.|
|/NXCOMPAT|Marks an executable as verified to be compatible with the Windows Data Execution Prevention feature.|
|/OPT|Controls LINK optimizations.|
|/ORDER|Places COMDATs into the image in a predetermined order.|
|/OUT|Specifies the output file name.|
|/PDB|Creates a program database (PDB) file.|
|/PDBALTPATH|Uses an alternate location to save a PDB file.|
|/PDBSTRIPPED|Creates a program database (PDB) file that has no private symbols.|
|/PGD|Specifies a .pgd file for profile-guided optimizations.|
|/PROFILE|Produces an output file that can be used with the Performance Tools profiler.|
|/RELEASE|Sets the Checksum in the .exe header.|
|/SAFESEH|Specifies that the image will contain a table of safe exception handlers.|
|/SECTION|Overrides the attributes of a section.|
|/STACK|Sets the size of the stack in bytes.|
|/STUB|Attaches an MS-DOS stub program to a Win32 program.|
|/SUBSYSTEM|Tells the operating system how to run the .exe file.|
|/SWAPRUN|Tells the operating system to copy the linker output to a swap file before it is run.|
|/TLBID|Specifies the resource ID of the linker-generated type library.|
|/TLBOUT|Specifies the name of the .tlb file and other MIDL output files.|
|/TSAWARE|Creates an application that is designed specifically to run under Terminal Server.|
|/VERBOSE|Prints linker progress messages.|
|/VERSION|Assigns a version number.|
|/WINMD|Enables generation of a Windows Runtime Metadata file.|
|/WINMDFILE|Specifies the file name for the Windows Runtime Metadata (winmd) output file that's generated by the /WINMD linker option.|
|/WINMDKEYFILE|Specifies a key or key pair to sign a Windows Runtime Metadata file.|
|/WINMDKEYCONTAINER|Specifies a key container to sign a Windows Metadata file.|
|/WINMDDELAYSIGN|Partially signs a Windows Runtime Metadata (.winmd) file by placing the public key in the winmd file.|
|/WX|Treats linker warnings as errors.|
[5]  如果没有设置/c编译选项，CL编译器自动调用 LINK. CL提供下列命令行编译选项和参数用以调控LINK:
(The CL compiler automatically calls LINK unless you specify the /c option. CL provides some control over the linker through command-line options and arguments. The following table summarizes the features in CL that affect linking. )
```python
Any file name extension other than .c, .cxx, .cpp, or .def
    Passes a file name as input to LINK
filename.def
    Passes /DEF:filename.def
/Fnumber
    Passes /STACK:number
/Fdfilename
    Passes /PDB:filename
/Fefilename
    Passes /OUT:filename
/Fmfilename
    Passes /MAP:filename
/Gy
    Creates packaged functions (COMDATs); enables function-level linking
/LD
    Passes /DLL
/LDd
    Passes /DLL
/link
    Passes remainder of command line to LINK
/MD or /MT
    Places a default library name in the .obj file
/MDd or /MTd
    Places a default library name in the .obj file. Defines the symbol _DEBUG
/nologo
    Passes /NOLOGO
/Zd
    Passes /DEBUG
/Zi or /Z7
    Passes /DEBUG
/Zl
    Omits default library name from .obj file
```
样例:
链接汇编代码
```python
ml -nologo -c "foo.asm"
link /NOLOGO /SUBSYSTEM:CONSOLE   kernel32.lib user32.lib "foo.obj"
```

# NMAKE
C/C++ applications can be built on the command line by using nmake.exe (the Microsoft Program Maintenance Utility), which is "a command-line tool that builds projects based on commands that are contained in a description file"[9].The command-line toolset includes a compiler, linker, and other build tools, and a command file that sets the required build environment [10].
NMAKE命令语法
NMAKE [option] [macros] [targets] [@CommandFile]
|Option|Purpose|
|/A|Forces a build of all evaluated targets, even if the targets are not out-of-date with respect to dependents. Does not force a build of unrelated targets.|
|/B|Forces a build even if the time stamps are equal. Recommended only for very fast systems with a resolution of two seconds or less.|
|/C|Suppresses default output, including nonfatal Nmake.exe errors or warnings, time stamps, and the Nmake.exe copyright message. Suppresses warnings issued by /K.|
|/D|Displays the time stamps of each evaluated target and dependent and a message when a target does not exist. Useful with /P for debugging a .mak file. Use !CMDSWITCHES to set or clear /D for part of a .mak file.|
|/E|Causes environment variables to override .mak file macro definitions.|
|/F|filename Specifies filename as a .mak file. Spaces or tabs can precede filename. Specify /F once for each .mak file. To supply a .mak file from standard input, specify a hyphen (-) for filename. To end keyboard input, press F6 or CTRL+Z.|
|/HELP, /?|Displays a brief summary of Nmake.exe command-line syntax.|
|/I|Ignores exit codes from all commands. To set or clear /I for part of a .mak file, use !CMDSWITCHES. To ignore exit codes for part of a .mak file, use a hyphen (-) command modifier or .IGNORE. Overrides /K if both are specified.|
|/K|Continues building unrelated dependencies, if a command returns an error. Also issues a warning and returns an exit code of 1. By default, Nmake.exe stops if any command returns a nonzero exit code. Warnings from /K are suppressed by /C. /I overrides /K if both are specified.|
|/N|Displays, but does not execute, commands. Preprocessing commands are executed. Does not display commands in recursive Nmake.exe calls. Useful for debugging .mak files and checking time stamps. To set or clear /N for part of a .mak file, use !CMDSWITCHES.|
|/NOLOGO|Suppresses the Nmake.exe copyright message.|
|/P|Displays information, such as macro definitions, inference rules, targets, and .SUFFIXES list, to standard output, and then runs the build. If no .mak file or command-line target exists, it displays information only. Use with /D to debug a .mak file.|
|/Q|Checks the time stamps of targets, but does not run the build. Returns a zero exit code if all targets are up-to-date and a nonzero exit code if any target is not up-to-date. Preprocessing commands are executed. Useful when running Nmake.exe from a batch file.|
|/R|Clears the .SUFFIXES list and ignores inference rules and macros that are defined in the Tools.ini file or that are predefined.|
|/S|Suppresses the display of executed commands. To suppress the display in part of a .mak file, use the @ command modifier or .SILENT. To set or clear /S for part of a .mak file, use !CMDSWITCHES.|
|/T|Updates the time stamps of command-line targets, or the first .mak file target, and executes preprocessing commands, but does not run the build.|
|/U|Must be used in conjunction with /N. Dumps inline Nmake.exe files so that the /N output can be used as a batch file.|
|/X|Sends Nmake.exe error output to filename instead of to a standard error output. Spaces or tabs can precede filename. To send standard error output, specify a hyphen (-) for filename. Does not affect output from commands to standard error output.|
|/Y|Disables batch-mode inference rules. When this option is selected, all batch-mode inference rules are treated as regular inference rules.|

# Makefile
### Special characters
Special characters in NMAKE:
: ; \# ( ) $ ^ \ { } ! @ -
### Description Blocks
A description block or inference rule specifies a block of commands to run if the dependency is out-of-date. NMAKE displays each command before running it, unless /S, .SILENT, !CMDSWITCHES, or @ is used. NMAKE looks for a matching inference rule if a description block is not followed by a commands block.
"A description block is a dependency line optionally followed by a commands blocks:"
```python
targets ... : dependents ... 
	commands ...
```
A commands block contains one or more commands, each on its own line. No blank line can appear between the dependency or rule and the commands block. However, a line containing only spaces or tabs can appear; this line is interpreted as a null command, and no error occurs. Blank lines are permitted between command lines.
A command line begins with one or more spaces or tabs. A backslash ( \ ) followed by a newline character is interpreted as a space in the command; use a backslash at the end of a line to continue a command onto the next line. NMAKE interprets the backslash literally if any other character, including a space or tab, follows the backslash.
A command preceded by a semicolon (;) can appear on a dependency line or inference rule, whether or not a commands block follows:
```python
project.obj : project.c project.h ; cl /c project.c
```
Dependents are NOT case sensitive.
To split the line, using backslash (\) after the target or dependent.
### Targets
In a dependency line, specify one or more targets, using any valid filename, directory name,  or pseudotarget.
Separate multiple targets with one or more spaces or tabs.
A target musts be at the start of the line.
The target seperates from dependents by a colon (:), space or tabs are allowed.
Paths are permitted with filenames.
A target cannot exceed 256 characters.
If the target preceding the colon is a single character, use a
separating space; otherwise, NMAKE interprets the letter-colon combination as a drive specifier

### 运行NMAKE
操作步骤：
1) 准备好项目文件和Makefile文件
2)  在cmd命令行下启动VS Build Tools ( 直接点击开始-> 程序中VS Build Tools选项  )
3) 使用cd命令进入工作目录
4) 运行nmake.exe. nmake即在当前目录中寻找Makefile文件，并执行Makefile文件中设定的编译指令。
例如：
![](https://img-blog.csdn.net/20180225234454964)

# Benchmarks
## VS nmake makefile 样例_1 [4]
目标：将simple.c 和 challeng.c 两个源文件分别编译成可执行文件
准备好simple.c 和 challeng.c后，Makefile文件内容如下：
```python
# Sample makefile
!include <win32.mak>
all: simple.exe challeng.exe
.c.obj:
  $(cc) $(cdebug) $(cflags) $(cvars) $*.c
simple.exe: simple.obj
  $(link) $(ldebug) $(conflags) -out:simple.exe simple.obj $(conlibs) lsapi32.lib
challeng.exe: challeng.obj md4c.obj
  $(link) $(ldebug) $(conflags) -out:challeng.exe $** $(conlibs) lsapi32.lib
```

## VS nmake makefile 样例_2  [6]
```python
# Example Makefile for ArcEngine C++ Programming on Windows
#
# The PROGRAM macro defines the name of the program or project.  It
# allows the program name to be changed by editing in only one
# location
#
PROGRAM = basic_sample
#
# Command line parameters: Edit these parameters so that you can
# easily run the sample by typing "nmake -f Makefile.Windows run".
#
# You will need to:
# (1) Describe parameters here. ex: IN_SHAPEFILE is the input shapefile
# (2) Define parameters below this comment box.
#       ex: IN_SHAPEFILE = /mycomp/data/shapefile.shp
# (3) Add the parameters to the run target at the end of this file
#       ex: ./$(PROGRAM) $(IN_SHAPEFILE)
#
#
# The INCLUDEDIRS macro contains a list of include directories
# to pass to the compiler so it can find necessary header files.
#
# The LIBDIRS macro contains a list of library directories
# to pass to the linker so it can find necessary libraries.
#
# The LIBS macro contains a list of libraries that the the
# executable must be linked against.
#
INCLUDEDIRS = \
	/I "C:\Program Files\ArcGIS\include\CPPAPI" \
	/I "C:\Program Files\ArcGIS\Com"

LIBDIRS =

LIBS =
#
# The CPPSOURCES macro contains a list of source files.
#
# The CPPOBJECTS macro converts the CPPSOURCES macro into a list
# of object files.
#
# The CPPFLAGS macro contains a list of options to be passed to
# the compiler.  Adding "-g" to this line will cause the compiler
# to add debugging information to the executable.
#
# The CPP macro defines the C++ compiler.
#
# The LINKFLAGS macro contains all of the library and library
# directory information to be passed to the linker.
#
CPPSOURCES = basic_sample.cpp        # list of source files
CPPOBJECTS = $(CPPSOURCES:.cpp=.obj) # expands to list of object files
CPPOPT = /EHsc /D_CRT_SECURE_NO_DEPRECATE
CPPFLAGS = -DESRI_WINDOWS $(INCLUDEDIRS) $(CPPOPT)
CPP = cl.exe
LINKFLAGS = $(LIBDIRS) $(LIBS)
#
# Default target: the first target is the default target.
# Just type "nmake -f Makefile.Windows" to build it.
#
all: $(PROGRAM)
#
# Link target: automatically builds its object dependencies before
# executing its link command.
#
$(PROGRAM): $(CPPOBJECTS)
	link.exe /out:$(PROGRAM) $(CPPOBJECTS) $(LINKFLAGS)
#
# Object targets: rules that define objects, their dependencies, and
# a list of commands for compilation.
#
basic_sample.obj: basic_sample.cpp basic_sample.h
	$(CPP) $(CPPFLAGS) /c basic_sample.cpp
#
# Clean target: "nmake -f Makefile.Windows clean" to remove unwanted objects and executables.
#
clean:
	del $(CPPOBJECTS) $(PROGRAM)
#
# Run target: "nmake -f Makefile.Windows run" to execute the application
# You will need to add $(VARIABLE_NAME) for any command line parameters
# that you defined earlier in this file.
#
run:
	$(PROGRAM)
```

## VS nmake makefile 样例_3   [7]
```python
# Makefile : Illustrates the effective use of precompiled  
#            headers in a project  
# Usage:     NMAKE option  
# option:    DEBUG=[0|1]  
#            (DEBUG not defined is equivalent to DEBUG=0)  
#  
OBJS = myapp.obj applib.obj  
# List all stable header files in the STABLEHDRS macro.  
STABLEHDRS = stable.h another.h  
# List the final header file to be precompiled here:  
BOUNDRY = stable.h  
# List header files under development here:  
UNSTABLEHDRS = unstable.h  
# List all compiler options common to both debug and final  
# versions of your code here:  
CLFLAGS = /c /W3  
# List all linker options common to both debug and final  
# versions of your code here:  
LINKFLAGS = /NOD /ONERROR:NOEXE  
!IF "$(DEBUG)" == "1"  
CLFLAGS   = /D_DEBUG $(CLFLAGS) /Od /Zi /f  
LINKFLAGS = $(LINKFLAGS) /COD  
LIBS      = slibce  
!ELSE  
CLFLAGS   = $(CLFLAGS) /Oselg /Gs  
LINKFLAGS = $(LINKFLAGS)  
LIBS      = slibce  
!ENDIF  
myapp.exe: $(OBJS)  
    link $(LINKFLAGS) @<<  
$(OBJS), myapp, NUL, $(LIBS), NUL;  
<<  
# Compile myapp  
myapp.obj  : myapp.cpp $(UNSTABLEHDRS)  stable.pch  
    $(CPP) $(CLFLAGS) /Yu$(BOUNDRY)    myapp.cpp  
# Compile applib  
applib.obj : applib.cpp $(UNSTABLEHDRS) stable.pch  
    $(CPP) $(CLFLAGS) /Yu$(BOUNDRY)    applib.cpp  
# Compile headers  
stable.pch : $(STABLEHDRS)  
    $(CPP) $(CLFLAGS) /Yc$(BOUNDRY)    applib.cpp myapp.cpp
```

## VS nmake makefile 样例_4

```python
!if "$(DEBUG)" == ""
DEBUG = 1
!endif
!if "$(DEBUG)" == "1"
ODIR = debug
CFLAGS = $(CFLAGS) -Od -D_DEBUG -ZI 
D = d
!else
ODIR = release
CFLAGS = $(CFLAGS) -O1 -DNDEBUG -Zi
!endif

LIBS = \
    $(LIBS) \
    ole32.lib \
    oleaut32.lib

CFLAGS = $(CFLAGS) -MD$(D) -I"$(VSINSTALLDIR)\DIA SDK\include"
PCHNAME  = $(ODIR)\stdafx.pch
PCHHEADER = stdafx.h
PCHFLAGS = -Yu$(PCHHEADER) -Fp$(PCHNAME)
CFLAGS   = $(CFLAGS)   -nologo -c -Fd$(ODIR)\ -W3
LFLAGS   = $(LFLAGS)   -map -debug -PDB:$(ODIR)\dia2dump.pdb "-libpath:$(VSINSTALLDIR)\DIA SDK\lib"

!if "$(VERBOSE)" == "1"
!message DEBUG=$(DEBUG)
!endif
OBJS = \
    $(ODIR)\dia2dump.obj    \
    $(ODIR)\regs.obj        \
    $(ODIR)\printsymbol.obj \
    $(ODIR)\stdafx.obj      

##### Inference Rules
all : $(ODIR)\dia2dump.exe
$(PCHNAME) $(ODIR)\stdafx.obj : $(PCHHEADER) stdafx.cpp dia2dump.h
    cl $(CFLAGS) $(PCHFLAGS:Yu=Yc) -Fo$(ODIR)\ -FR$(ODIR)\ stdafx.cpp
{}.cpp{$(ODIR)\}.obj::
    cl $(CFLAGS) $(MPBUILDFLAGS) $(PCHFLAGS) -Fo$(ODIR)\ -FR$(ODIR)\ $<
$(ODIR)\dia2dump.exe : $(ODIR) $(PCHNAME) $(OBJS)
    link -out:$(ODIR)\dia2dump.exe $(OBJS) $(LFLAGS) $(LIBS)
$(ODIR):
    -md $(ODIR)
clean :
    del /q $(ODIR)
```

## VS nmake makefile 样例_5
使用一行命令编译多个源代码文件
源代码文件夹架构
`../  
  |_ inc
      |_ foo.h
  |_ src
      |_ Array.cc
      |_ foo.cc
      |_ main.cc
      |_ makefile`makefile文件内容
`# Target 
PROGRAM = test.exe
# Flags
CPPOPT = /w /EHsc /D_CRT_SECURE_NO_DEPRECATE 
# Compiler 
cc = cl 
CFLAGS = 
 
# list of source files 
CPPSOURCES =   main.cc    Array.cc   foo.cc   
# expands to list of object files        
CPPOBJECTS = $(CPPSOURCES:.cc=.obj) 
  
all: $(PROGRAM)
$(PROGRAM): $(CPPOBJECTS)
	link.exe /out:$(PROGRAM)  $(CPPOBJECTS) 
	
$(CPPOBJECTS):  $(CPPSOURCES)
	$(cc) $(CPPOPT) /c $(CPPSOURCES) 
		
clean:  
    del $(CPPOBJECTS) $(PROGRAM)`
## VS nmake makefile 样例_6
一个项目一般包含多个代码文件，例如 类的头文件和源文件等。 为了便于管理，通常将这些文件分别放入不同文件夹中。在本样例中:
1)  C++类头文件存放在 /inc文件夹中;
2)  C++类源文件存放在 /src文件夹中;
3)  项目编译过程中产生的临时文件放在 /tmp文件夹中;
项目文件架构如下所示：
```python
../  
  |_ inc
      |_ foo.h
  |_ src
      |_ foo.cpp
      |_ main.cpp
  |_ tmp 
      |_ Makefile
```
Makefile文件内容如下:
```python
# Target   
PROGRAM = test.exe  
  
inc = "../inc/"  
src = "../src/"    
INCLUDEDIRS =  /I $(inc)    
 
# Flags  
CPPOPT = $(INCLUDEDIRS) /w /EHsc /D_CRT_SECURE_NO_DEPRECATE  
  
# Compiler   
cc = cl   
CFLAGS =   
   
# list of source files   
CPPSOURCES =  main.cpp  foo.cpp      
  
# expands to list of object files          
CPPOBJECTS = $(CPPSOURCES:.cpp=.obj)   
    
all: $(PROGRAM)  
  
$(PROGRAM): $(CPPOBJECTS)  
    link.exe /out:$(PROGRAM)  $(CPPOBJECTS)    
      
main.obj:   
    $(cc) $(CPPOPT) /c ../src/main.cpp       
foo.obj:   
    $(cc) $(CPPOPT) /c ../src/foo.cpp        
          
clean:    
    del $(CPPOBJECTS) $(PROGRAM)
```

## VS nmake makefile 样例_7  [11]
```python
1. MYSQL_HOME=D:\downloads\mysql-connector-c++-noinstall-1.0.5-win32
 2. MYSQL_INCS=$(MYSQL_HOME)\include
 3. MYSQL_LIBS=$(MYSQL_HOME)\lib
 4. 
 5. mysqlbuild: mysqlpool.obj examplemysql.obj crosshelper.obj
 6.   cl /o hello.exe mysqlpool.obj examplemysql.obj crosshelper.obj /link /LIBPATH:$(MYSQL_LIBS) mysqlcppconn.lib
 7. 
 8. 
 9. examplemysql.obj: ExampleMySQL.cpp
10.     cl /c ExampleMySQL.cpp /D WINDOWSXX -I $(MYSQL_INCS)
11. 
12. mysqlpool.obj: ..\mysql\MySQLPool.cpp 
13.     cl /c  ..\mysql\MySQLPool.cpp /D WINDOWSXX  -I $(MYSQL_INCS)
14. 
15. crosshelper.obj: ..\CrossHelper.cpp
16.     cl /c ..\CrossHelper.cpp /D WINDOWSXX 
17. 
18. oraclebuild:
19.     echo "oracle build goes here"
20. 
21. all:oraclebuild mysqlbuild
22. 
23. clean:
24.   del *.exe *.obj
```

## makefile 样例_8 --  编译资源文件
资源文件一般包含向用户提供信息的用户界面，例如：应用程序所需数据的自定义资源，由安装 API 使用的版本资源，以及菜单和对话框资源等。
资源编译器rc.exe将编写的资源脚本文件(*.rc)编程成资源文件(*.res)，然后由cvtres.exe转换为.obj文件
makefile样例[12]
`NAME = Test
OBJS = $(NAME).obj rsrc.obj
LINK_FLAG = /subsystem:windows
ML_FLAG = /c /coff
RC_FLAG = /v
CVTRES_FLAG = /machine:ix86
$(NAME).exe: $(OBJS)
	Link $(LINK_FLAG) $(OBJS) 
.asm.obj:
	ml $(ML_FLAG) {1}lt;
rsrc.obj:
	rc $(RC_FLAG) rsrc.rc
	Cvtres	$(CVTRES_FLAG) rsrc.res
clean:
	del *.obj`
## makefile 样例_9 --  C语言代码中调用汇编语言编写的函数
汇编语言函数代码文件foo.asm
`.386
.model flat, c
public foo
.code
foo proc
    mov eax, 20 ;返回 20
    ret
foo endp
end`C语言主程序源代码文件a.c
`#include <stdio.h>
int  foo();
int main() {
    int r;
    r = foo();
    printf("result = %d \n", r);
    return 0;
}`C++语言主程序源头代码a.cc
`#include <stdio.h>
extern "C" {
 int   foo();
}
int main() {
    int r;
    r = foo();
    printf("result = %d \n", r);
    return 0;
}`
代码编译，链接
`编译汇编语言代码，生成obj文件
> ml /coff -c foo.asm
编译C语言代码，生成obj文件
> cl /c /FA a.c 
链接obj文件，生成可执行文件
> link /out:a.exe a.obj foo.obj`

## 样例_10 --  编译汇编语言程序
`.386                        ;使用80386指令集
.model flat,stdcall         ;内存模型flat,编译器参数的传递约定stdcall
option casemap:none         ;大小写敏感
;Include File and Library
includelib msvcrt.lib
includelib kernel32.lib
printf   PROTO C:ptr sbyte, :VARARG
ExitProcess   PROTO, dwExitCode:DWORD
;Data  Segment
.data
szMsg   byte "Hello world.", 0ah, 0  
;Code Segment
.code
start:
	mov eax,10000h		; EAX = 10000h
	add eax,40000h		; EAX = 50000h
	sub eax,20000h		; EAX = 30000h
	push 1h
	call ExitProcess
end start`
编译链接
`:>ml /coff /c /Cp h.asm
Microsoft (R) Macro Assembler Version 14.00.23506.0
Copyright (C) Microsoft Corporation.  All rights reserved.
 Assembling: h.asm
:>link /out:h.exe /SUBSYSTEM:CONSOLE  msvcrt.lib kernel32.lib user32.lib  h.obj
Microsoft (R) Incremental Linker Version 14.00.23506.0
Copyright (C) Microsoft Corporation.  All rights reserved.`

## makefile 样例_11 --  编译Latex
```python
BINPATH = D:\CTEX\MiKTeX\miktex\bin\
TeX = $(BINPATH)\pdflatex.exe 
TeX_FLAGS = -shell-escape -interaction=nonstopmode -file-line-error
PRE =  $(TeX) -ini -job-name="preamble" "&pdflatex preamble.tex\dump"
BIB = $(BINPATH)\bibtex.exe
FileName = main
all: $(FileName).pdf
main.pdf: $(FileName).tex 
	$(TeX) $(TeX_FLAGS) $(FileName).tex
	$(BIB) $(FileName).tex
	$(TeX) $(FileName).tex
	$(TeX) $(FileName).tex
clean: 
	del $(FileName).pdf
	del $(FileName).log
```

## 参考文献
[1] Walkthrough: Compiling a C Program on the Command Line. Hadi Braishttps://msdn.microsoft.com/en-us/library/bb384838.aspx
[2] NMAKE Reference https://msdn.microsoft.com/en-us/library/dd9y37ha.aspx
[3] Linker Options  https://msdn.microsoft.com/en-us/library/y0zzbyt4.aspx
[4] makefile example https://msdn.microsoft.com/en-us/library/x6bt6xe7.aspx
[5]  https://msdn.microsoft.com/en-us/library/92b5ab4h.aspx
﻿﻿[6] http://resources.esri.com/help/9.3/arcgisengine/com_cpp/Cpp/reference/Makefile.Windows.htm
[7] https://msdn.microsoft.com/en-us/library/d9b6wk21.aspx
[9] https://msdn.microsoft.com/en-us/library/dd9y37ha.aspx
[10] https://msdn.microsoft.com/en-us/library/f35ctcxw.aspx
[11] http://www.tidytutorials.com/2009/08/nmake-makefile-tutorial-and-example.html
[12] https://blog.csdn.net/erick08/article/details/6871128

