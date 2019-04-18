# 使用DUMPBIN命令 - ljx0305的专栏 - CSDN博客
2010年01月14日 22:20:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：489标签：[编译器																[dll																[microsoft																[file																[duplicates																[汇编](https://so.csdn.net/so/search/s.do?q=汇编&t=blog)](https://so.csdn.net/so/search/s.do?q=duplicates&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)
个人分类：[VC](https://blog.csdn.net/ljx0305/article/category/401790)
在使用VC时，可以用DUMPBIN。EXE来得到某个DLL中所输出的符号的清单。如下面的
命令：dumpbin -exports Cmpnt1.dll
如：
C:/WINDOWS/system32>dumpbin -exports msgsvc.dll
Microsoft (R) COFF Binary File Dumper Version 6.00.8447
Copyright (C) Microsoft Corp 1992-1998. All rights reserved.
Dump of file msgsvc.dll
File Type: DLL
  Section contains the following exports for msgsvc.dll
           0 characteristics
    41107F60 time date stamp Wed Aug 04 14:17:04 2004
        0.00 version
           1 ordinal base
           2 number of functions
           2 number of names
    ordinal hint RVA      name
          1    0 00004ABF ServiceMain
          2    1 00004595 SvchostPushServiceGlobals
  Summary
        1000 .data
        1000 .reloc
        1000 .rsrc
        8000 .text
C:/>dumpbin
Microsoft (R) COFF Binary File Dumper Version 6.00.844
Copyright (C) Microsoft Corp 1992-1998. All rights res
usage: DUMPBIN [options] [files]
   options:
      /ALL
      /ARCH
      /ARCHIVEMEMBERS
      /DEPENDENTS
      /DIRECTIVES
      /DISASM
      /EXPORTS
      /FPO
      /HEADERS
      /IMPORTS
      /LINENUMBERS
      /LINKERMEMBER[:{1|2}]
      /LOADCONFIG
      /OUT:filename
      /PDATA
      /RAWDATA[:{NONE|BYTES|SHORTS|LONGS}[,#]]
      /RELOCATIONS
      /SECTION:name
      /SUMMARY
      /SYMBOLS
若要运行 DUMPBIN，请使用下列语法：
DUMPBIN [*options*] *files*...
指定一个或多个二进制文件，以及控制信息所需的任何选项。DUMPBIN 将该信息显示到标准输出。可以将输出重定向到文件，或者使用 /OUT 选项为输出指定文件名。
当在文件上运行 DUMPBIN 但未指定选项时，DUMPBIN 显示 /SUMMARY 输出。
当键入命令 `dumpbin` 但没有任何其他命令行输入时，DUMPBIN 显示汇总其选项的用法语句。
[DUMPBIN 选项](http://msdn.microsoft.com/library/chs/vccore/html/_core_dumpbin_options.asp)
/ALL
此选项显示除代码反汇编外的所有可用信息。使用 [/DISASM](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.disasm.asp) 显示反汇编。可以与 /ALL 一起使用 [/RAWDATA](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.rawdata.asp):NONE 来省略文件的原始二进制详细资料。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/ARCHIVEMEMBERS
此选项显示有关库成员对象的最少信息。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/CLRHEADER *file*
此处： 
- *file*
用 [/clr](http://msdn.microsoft.com/library/chs/vccore/html/vcrefeecomcompilation.asp) 生成的图像文件。 
#### 备注
CLRHEADER 显示有关在任何托管程序中使用的 .NET 头的信息。输出显示 .NET 头及其中各节的位置和大小（以字节计）。
File Format Spec.doc 描述 .NET 头中的信息。NET SDK 将 File Format Spec.doc 安装在 Tools Developers Guide 目录中。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/DIRECTIVES
此选项转储图像中由编译器生成的 .directive 节。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/DEPENDENTS
转储图像从中导入函数的 DLL 的名称。不要转储导入函数名。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/DISASM
此选项显示代码段的反汇编，如果出现在文件中则使用符号。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/EXPORTS
此选项显示从可执行文件或 DLL 导出的所有定义。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/FPO
此选项显示框架指针优化 (FPO) 记录。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/HEADERS
此选项显示文件头和每节的头。当用于库时，显示每个成员对象的头。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp?frame=true#_dumpbin_.2f.headers) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/IMPORTS[:*file*]
此选项显示导入到可执行文件或 DLL 的 DLL 列表（静态链接的和[延迟加载](http://msdn.microsoft.com/library/chs/vccore/html/vcconlinkersupportfordelayedloadingofdlls.asp)）和上述每个 DLL 的各个导入。
可选 *file* 规范允许指定仅显示某个 DLL 的导入。例如：
dumpbin /IMPORTS:msvcrt.dll
此选项显示的输出与 [/EXPORTS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.exports.asp) 输出相似。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/LINENUMBERS
此选项显示 COFF 行号。如果对象文件是用程序数据库 (/Zi)、C7 兼容 (/Z7) 或仅限行号 (/Zd) 编译的，则它包含行号。如果可执行文件或 DLL 是与生成调试信息 (/DEBUG) 链接的，则它包含 COFF 行号。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/LINKERMEMBER[:{1|2}]
此选项显示库中定义的公共符号。指定参数 1 将按对象顺序显示符号及其偏移量。指定参数 2 将显示对象的偏移量和索引号，然后按字母顺序列出这些符号及每个符号的对象索引。若要两个输出都获得，指定不带数字参数的 /LINKERMEMBER。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/LOADCONFIG
此选项转储 IMAGE_LOAD_CONFIG_DIRECTORY 结构，此结构是由 Windows NT 加载程序使用并在 WINNT.H 中定义的可选结构。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/OUT:*filename*
此选项指定输出的 *filename*。默认情况下，DUMPBIN 将信息显示到标准输出。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/PDBPATH[:VERBOSE] *filename*
此处： 
- *filename*
要为其查找匹配 .pdb 文件的 .dll 或 .exe 文件名。 - VERBOSE（可选） 
报告曾尝试在其中定位 .pdb 文件的所有目录。 
#### 备注
/PDBPATH 将沿调试器搜索 .pdb 文件的同一路径搜索计算机，并将报告哪些 .pdb 文件（若有）和 *filename* 中指定的文件相对应。
使用 Visual Studio 调试器时可能会遇到问题，这是因为调试器对调试文件的不同版本使用 .pdb 文件。
/PDBPATH 将沿下列路径搜索 .pdb 文件： 
- 检查可执行文件驻留的位置。 
- 检查写入可执行文件的 PDB 的位置。这通常是图像被链接时的位置。 
- 沿 Visual Studio IDE 中配置的搜索路径检查。 
- 沿 _NT_SYMBOL_PATH 和 _NT_ALT_SYMBOL_PATH 环境变量中的路径检查。 
- 在 Windows 目录中检查。 
/PDATA
仅用于 RISC 处理器。
此选项从图像或对象转储异常表 (.pdata)。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/RAWDATA[:{1|2|4|8|NONE[**,***number*]]
此选项显示文件中每节的原始内容。参数控制显示格式，如下所示：
|参数|结果|
|----|----|
|1|默认值。内容以十六进制字节显示，如果内容具有打印的表示形式，则还显示为 ASCII 字符。|
|2|内容显示为十六进制的 2 字节值。|
|4|内容显示为十六进制的 4 字节值。|
|8|内容显示为十六进制的 8 字节值。|
|NONE|取消显示原始数据。此参数对控制 /ALL 输出很有用。|
|*Number*|显示的行被设置为每行具有 *number* 个值的宽度。|
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/RELOCATIONS
此选项显示对象或图像中的任何重定位。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/SECTION:*section*
此选项限制与指定的 *section* 有关的信息的输出。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/SUMMARY
此选项显示有关节的最少信息（包括总大小）。如果未指定其他选项，则此选项为默认值。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/SYMBOLS
此选项显示 COFF 符号表。符号表存在于所有对象文件中。而对于图像文件，只有当它是与 /DEBUG 链接的时，它才包含 COFF 符号表。
下面是关于 /SYMBOLS 输出的说明。通过查阅 winnt.h（IMAGE_SYMBOL 和 IMAGE_AUX_SYMBOL）或 COFF 文档，可找到有关 /SYMBOLS 输出含义的附加信息。
假设有下列示例转储：
Dump of file main.obj
File Type: COFF OBJECT
COFF    SYMBOL    TABLE
000    00000000   DEBUG      notype      Filename      | .file
      main.cpp
002   000B1FDB   ABS      notype      Static      | @comp.id
003   00000000   SECT1      notype      Static      | .drectve
      Section length       26, #relocs   0, #linenums    0, checksum 722C964F
005   00000000   SECT2      notype      Static      | .text
      Section length      23, #relocs      1, #linenums    0, checksum 459FF65F, selection    1 (pick no duplicates)
007   00000000   SECT2      notype ()   External      | _main
008   00000000   UNDEF      notype ()   External      | ?MyDump@@YAXXZ (void __cdecl MyDump(void))
String Table Size = 0x10 bytes
Summary
      26 .drectve
      23 .text
对于以符号号码开头的行，下列说明描述了含有与用户相关的信息的列： 
- 开头的 3 位数字是符号索引/号码。 
- 如果第三列包含 SECT*x*，则符号在对象文件的那一节中定义。但如果出现 UNDEF，则它不在那个对象中定义并且必须在其他地方被解析。 
- 第五列 (Static, External) 说明符号是否只在那个对象的内部可见，或者是否是公共的（外部可见）。静态符号 _sym 不会链接到公共符号 _sym；这些符号是名为 _sym 的函数的两种不同实例。 
编号行中的最后一列是符号名（修饰名和未修饰名）。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
/UNWINDINFO
在程序图像（例如 exe 和 dll）中转储结构化异常处理 (SEH) 表的展开描述符。/UNWINDINFO 仅适用于 IA64 图像。
只有 [/HEADERS](http://msdn.microsoft.com/library/chs/vccore/html/_dumpbin_.2f.headers.asp) DUMPBIN 选项可用于由 [/GL](http://msdn.microsoft.com/library/chs/vccore/html/vcgrfglwholeprogramoptimization.asp) 编译器选项产生的文件。
引用:http://dev.csdn.net/article/59/59037.shtm
