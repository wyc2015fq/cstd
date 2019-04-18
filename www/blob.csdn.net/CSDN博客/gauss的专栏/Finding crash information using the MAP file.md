# Finding crash information using the MAP file - gauss的专栏 - CSDN博客
2013年01月04日 00:25:13[gauss](https://me.csdn.net/mathlmx)阅读数：240
个人分类：[开发工具/调试技巧](https://blog.csdn.net/mathlmx/article/category/1317877)
## Introduction
Programming neat applications is one thing. But when a user informs you your software has crashed, you know it's best to fix this before adding other features. If you're lucky enough, the user will have a crash address. This will go a long way in solving the
 problem. But how can you determine what went wrong, using this crash address?
## Creating a MAP file
Well first of all, you'll need a MAP file. If you don't have one, it will be nearly impossible to find where your application crashed using the crash address. So first, I'll show you how to create a good MAP file. For this, I will create a new project (MAPFILE).
 You can do the same, or adjust your own project. I create a new project using the Win32 Application option in VC++ 6.0, selecting the 'typical "Hello Word!" application' to keep the size of the MAP file reasonable for explanation.
Once created we need to adjust the project settings for the release version. In the C/C++ tab, select "Line Numbers Only" for Debug Info.
![](http://www.codeproject.com/KB/debug/mapfile/mapc.jpg)
Many people forget this, but you'll need this option if you want a good MAP file. This will not affect your release in any way. Next is the Link tab. Here you need to select the "Generate mapfile" option. Also, type the switches`/MAPINFO:LINES `and`/MAPINFO:EXPORTS `in
 the Project Options edit box.
![](http://www.codeproject.com/KB/debug/mapfile/maplink.jpg)
Now, you're ready to compile and link your project. After linking, you will find a .map file in your intermediate directory (together with your exe).
## Reading the MAP file
After all this dull work, now comes the neat part: how to read the MAP file. We'll do this by using a crash example. So first: how to crash your application. I did this by adding these two lines at the end of the`InitInstance()`function:
![](http://www.codeproject.com/images/minus.gif)Collapse|[Copy
 Code](http://www.codeproject.com/Articles/3472/Finding-crash-information-using-the-MAP-file#)
char* pEmpty = NULL;
*pEmpty = 'x';	// This is line 119
I'm sure you can find other instructions which will crash your application. Now recompile and link. If you start the application, it will crash and you'll get a message like this: 'The instruction at "0x004011a1" referenced memory at "0x00000000". The memory
 could not be "Written".' .
Now, it's time to open the MAP file with notepad or something similar. You MAP file will look like this:
The top of the MAP file contains the module name, the timestamp indicating the link of the project, and the preferred load address (which will probably be`0x00400000`unless
 you're using a dll). After the header comes the section information that shows which sections the linker brought in from the various OBJ and LIB files.
![](http://www.codeproject.com/images/minus.gif)Collapse|[Copy
 Code](http://www.codeproject.com/Articles/3472/Finding-crash-information-using-the-MAP-file#)
MAPFILE
 Timestamp is 3df6394d (Tue Dec 10 19:58:21 2002)
 Preferred load address is 00400000
 Start         Length     Name                   Class
 0001:00000000 000038feH .text                   CODE
 0002:00000000 000000f4H .idata$5                DATA
 0002:000000f8 00000394H .rdata                  DATA
 0002:0000048c 00000028H .idata$2                DATA
 0002:000004b4 00000014H .idata$3                DATA
 0002:000004c8 000000f4H .idata$4                DATA
 0002:000005bc 0000040aH .idata$6                DATA
 0002:000009c6 00000000H .edata                  DATA
 0003:00000000 00000004H .CRT$XCA                DATA
 0003:00000004 00000004H .CRT$XCZ                DATA
 0003:00000008 00000004H .CRT$XIA                DATA
 0003:0000000c 00000004H .CRT$XIC                DATA
 0003:00000010 00000004H .CRT$XIZ                DATA
 0003:00000014 00000004H .CRT$XPA                DATA
 0003:00000018 00000004H .CRT$XPZ                DATA
 0003:0000001c 00000004H .CRT$XTA                DATA
 0003:00000020 00000004H .CRT$XTZ                DATA
 0003:00000030 00002490H .data                   DATA
 0003:000024c0 000005fcH .bss                    DATA
 0004:00000000 00000250H .rsrc$01                DATA
 0004:00000250 00000720H .rsrc$02                DATA
After the section information, you get the public function information. Notice the "public" part. If you have static-declared C functions, they won't show up in the MAP file. Fortunately, the line numbers will still reflect the static functions. The important
 parts of the public function information are the function names and the information in the`Rva+Base`column, which is the starting address of the function.
![](http://www.codeproject.com/images/minus.gif)Collapse|[Copy
 Code](http://www.codeproject.com/Articles/3472/Finding-crash-information-using-the-MAP-file#)
  Address         Publics by Value              Rva+Base     Lib:Object
 0001:00000000       _WinMain@16                00401000 f   MAPFILE.obj
 0001:000000c0       ?MyRegisterClass@@YAGPAUHINSTANCE__@@@Z 004010c0 f   MAPFILE.obj
 0001:00000150       ?InitInstance@@YAHPAUHINSTANCE__@@H@Z 00401150 f   MAPFILE.obj
 0001:000001b0       ?WndProc@@YGJPAUHWND__@@IIJ@Z 004011b0 f   MAPFILE.obj
 0001:00000310       ?About@@YGJPAUHWND__@@IIJ@Z 00401310 f   MAPFILE.obj
 0001:00000350       _WinMainCRTStartup         00401350 f   LIBC:wincrt0.obj
 0001:00000446       __amsg_exit                00401446 f   LIBC:wincrt0.obj
 0001:0000048f       __cinit                    0040148f f   LIBC:crt0dat.obj
 0001:000004bc       _exit                      004014bc f   LIBC:crt0dat.obj
 0001:000004cd       __exit                     004014cd f   LIBC:crt0dat.obj
 0001:00000591       __XcptFilter               00401591 f   LIBC:winxfltr.obj
 0001:00000715       __wincmdln                 00401715 f   LIBC:wincmdln.obj
 //SNIPPED FOR BETTER READING
 0003:00002ab4       __FPinit                   00408ab4     <common>
 0003:00002ab8       __acmdln                   00408ab8     <common>
 entry point at        0001:00000350
 Static symbols
 0001:000035d0       LeadUp1                    004045d0 f   LIBC:memmove.obj
 0001:000035fc       LeadUp2                    004045fc f   LIBC:memmove.obj
  //SNIPPED FOR BETTER READING
 0001:00000577       __initterm                 00401577 f   LIBC:crt0dat.obj
 0001:0000046b       _fast_error_exit           0040146b f   LIBC:wincrt0.obj
The public function part is followed by the line information (you got this if you used the`/MAPINFO:LINES `in the Link tab and selected the "Line numbers"
 in the C/C++ tab). After this, you will get the export information if your project contains exported functions and you included`/MAPINFO:EXPORTS `in the
 link tab.
![](http://www.codeproject.com/images/minus.gif)Collapse|[Copy
 Code](http://www.codeproject.com/Articles/3472/Finding-crash-information-using-the-MAP-file#)
Line numbers for .\Release\MAPFILE.obj(F:\MAPFILE\MAPFILE.cpp) segment .text
    24 0001:00000000    30 0001:00000004    31 0001:0000001b    32 0001:00000027
    35 0001:0000002d    53 0001:00000041    40 0001:00000047    43 0001:00000050
    45 0001:00000077    47 0001:00000088    48 0001:0000008f    52 0001:000000ad
    53 0001:000000b3    71 0001:000000c0    80 0001:000000c3    81 0001:000000c8
    82 0001:000000ff    86 0001:00000114    88 0001:00000135    89 0001:00000145
   102 0001:00000150   108 0001:00000155   110 0001:00000188   122 0001:0000018d
   115 0001:0000018e   116 0001:0000019a   119 0001:000001a1   121 0001:000001a8
   122 0001:000001ae   135 0001:000001b0   143 0001:000001cc   172 0001:000001ee
   175 0001:0000020d   149 0001:00000216   157 0001:0000022c   175 0001:00000248
   154 0001:00000251   174 0001:0000025f   175 0001:00000261   151 0001:0000026a
   174 0001:00000287   175 0001:00000289   161 0001:00000294   164 0001:000002a8
   165 0001:000002b6   166 0001:000002d8   174 0001:000002e7   175 0001:000002e9
   169 0001:000002f2   174 0001:000002fa   175 0001:000002fc   179 0001:00000310
   186 0001:0000031e   193 0001:0000032e   194 0001:00000330   188 0001:00000333
   183 0001:00000344   194 0001:00000349
Now we will look up where the crash occurred. First, we'll determine which function contains the crash address. Look in the "`Rva+Base`" column and search the first function with an
 address bigger than the crash address. The preceding entry in the MAP file is the function that had the crash. In our example our crash address is`0x004011a1`. This is
 between`0x00401150`and`0x004011b0`so we know the
 crash function is`?InitInstance@@YAHPAUHINSTANCE__@@H@Z`. Any function name that starts with a question mark is a C++ decorated name. To translate the name,
 pass it as a command-line parameter to the Platform SDK program UNDNAME.EXE (in the bin dir). You won't need to do this most of the time as you might figure it out just by looking at it (here:`InitInstance() `in
 MAPFILE.obj).
This is a big step for bug tracking. But it gets even better: we can find out on which line the crash occurred! We need to do some basic hexadecimal mathematics, so people whom can't do this without a calculator: now is the time to use it. The first step is
 the following calculation:`crash_address - preferred_load_address - 0x1000`
Addresses are offsets from the beginning of the first code section, se we need to do this calculation. Subtracting the preferred load address is logical, but why do we need to substract another 0x1000? The crash address is an offset from the beginning of the
 code section, but the first part of the binary isn't the code section! The first part of the binary is the Portable Executable (PE), which is 0x1000 bytes long. Mystery solved. In our example, this is:
```
0x004011a1
 - 0x00400000 - 0x1000 = 0x1a1
```
Now it's time to look in the line information section of the MAP file. The lines are shown like this:`30 0001:00000004`. The first number is the line number, the second
 number is the offset from the beginning of the code section in which this line occurred. If we want to look for our line number, we just have to do the same thing we did for the function: determine the first occurrence of a bigger offset than the one we just
 calculated. The crash occurred in the preceding entry. In our example:`0x1a1`is before`0x1a8`.
 So our crash occurred on`line 119 `in MAPFILE.CPP.
## Keeping track of MAP files
Each release had it's own MAP file. It's not a bad idea to include the MAP file with the exe distribution. This way, you can be certain you have the correct MAP file for this exe. You could keep every MAP file with every exe on your system, but we all know
 this might give some troubles later on. The MAP file doesn't contain any information you wouldn't want the user to see (unless maybe class and function names ?) . A user would have no use with it, but at least you can ask for the MAP file if you don't have
 a copy yourself.
## Acknowledgements
John Robbins for his "Debugging Applications" book
## License
This article, along with any associated source code and files, is licensed under[The Code Project Open License (CPOL)](http://www.codeproject.com/info/cpol10.aspx)
## About the Author
|**[Wouter Dhondt](http://www.codeproject.com/Members/Wouter-Dhondt)**![](http://www.codeproject.com/script/Membership/ProfileImages/%7B294AFDBA-C95F-426C-90E8-607946CF5970%7D.jpg)Web Developer![Belgium](http://www.codeproject.com/script/Geo/Images/BE.gif)BelgiumMember|Wouter got interested in computers and programming at the age of 12 (using a 286 and basic). Several years and an electronics degree later, he started working as a software engineer. In the summer of 2001, Wouter created Fping as an alternative to the windows ping program (just for his own amusement). Amazed by the response / interest, he founded[kwakkelflap.com](http://www.kwakkelflap.com/)to ensure a better distribution for the tool. Several other applications have been released since.|
