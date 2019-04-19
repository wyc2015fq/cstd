# WINCE data abort 内核调试错误分析 - xqhrs232的专栏 - CSDN博客
2013年04月16日 10:57:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：599
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://www.verydemo.com/demo_c248_i1993.html](http://www.verydemo.com/demo_c248_i1993.html)
1、  出错信息：PC=c01cf8ec(gwes.dll+0x0004f8ec) RA=c01cf8e4(gwes.dll+0x0004f8e4) SP=d1e9fde0, BVA=00001002
Gwes.map文件：
……………………………………………………………………………………………………
……………………………………………………………………………………………………
0001:0004e50c       ?GlobalFindAtomW_I@Atom_t@@SAGGPBG@Z 1004f50c f   gwes_lib:atom.obj
 0001:0004e600       ?GlobalGetAtomName@Atom_t@@SAPAGG@Z 1004f600 f   gwes_lib:atom.obj
 0001:0004e650       ?InitializeUspCE@@YAXXZ    1004f650 f   gwes_lib:uspceinit.obj
 0001:0004e844       ??0TimerEntry_t@@QAA@PAVMsgQueue@@PAUHWND__@@IIP6AX1IIK@ZPAX_N@Z 1004f844 f   gwes_lib:timer.obj
0001:0004e8c4       ?IsValidTimerEntry@TimerEntry_t@@QAA_NXZ 1004f8c4  f   gwes_lib:timer.obj
 0001:0004e940       ?TimerQueuesRemoveSingleEvent@TimerEntry_t@@SAPAU1@PAUHWND__@@IPAVMsgQueue@@@Z 1004f940 f   gwes_lib:timer.obj
 0001:0004ea38 ?TimerQueuesRemoveAllMsgQueueOrHwnd@TimerEntry_t@@SAXPAVMsgQueue@@PAUHWND__@@@Z 1004fa38 f   gwes_lib:timer.obj
0001:0004eb24       ?SetTimerCommon@TimerEntry_t@@CAIPAUHWND__@@IIP6AX0IIK@ZPAX_N@Z 1004fb24 f   gwes_lib:timer.obj
 0001:0004ec8c  ?TestAndReset@TimerEntry_t@@QAA_N_NPAPAUHWND__@@PAIPAP6AXPAU2@IIK@ZPAJ@Z 1004fc8c f   gwes_lib:timer.obj
0001:0004ecdc       ?IsFired@TimerEntry_t@@QAA_NXZ 1004fcdc f   gwes_lib:timer.obj
 0001:0004ece4       ?SetTimer_I@TimerEntry_t@@SAIPAUHWND__@@IIP6AX0IIK@ZPAX@Z 1004fce4 f   gwes_lib:timer.ob
Solution：PC=c01cf8ec(gwes.dll+0x0004f8ec) 中0x0004f8ec-00001000=0004e8ec，然后找与之最近的地址，如上在0004e8c4  和0004e940之间，那就是0004e8c4  为开始地址的函数中，那就是这个函数中有问题：IsValidTimerEntry@TimerEntry_
后面的地址1004f8c4 是由0004e8c4  加上1000000再加上00001000得来的
再看下面例子：
今天调试evc程序，总是出现几个莫名奇妙的错误如下：
Data Abort: Thread=8795c908 Proc=80becdf0 'OnLine.exe'
AKY=00000801 PC=03fa006c(coredll.dll+0x0002006c) RA=50616548(???+0x50616548) BVA=18332421 FSR=00000001
FMD_OEMIoControl::Not supported IOCtl : 0x10303FF
FMD_OEMIoControl::Not supported IOCtl : 0x10303FF
或者
Data Abort: Thread=866593e4 Proc=80becd00 'ONLINE.EXE'
AKY=00000401 PC=03fa006c(coredll.dll+0x0002006c) RA=50616548(???+0x50616548) BVA=162f2021 FSR=00000001
Assertion Failed: OnLine: File wincore.cpp, Line 1152
                                                     Data Abort: Thread=8795c908 Proc=80becd00 'ONLINE.EXE'
AKY=00000401 PC=03f9fb04(coredll.dll+0x0001fb04) RA=160bf4d8(ONLINE.EXE+0x000af4d8) BVA=2a3b14e9 FSR=00000001
FMD_OEMIoControl::Not supported IOCtl : 0x10303FF
Data Abort: Thread=8795c908 Proc=80becd00 'ONLINE.EXE'
AKY=00000401 PC=03f9fb04(coredll.dll+0x0001fb04) RA=160bf4d8(ONLINE.EXE+0x000af4d8) BVA=2a3b14e9 FSR=00000001
Data Abort: Thread=8795c908 Proc=80becd00 'ONLINE.EXE'
AKY=00000401 PC=03f9fb04(coredll.dll+0x0001fb04) RA=160bf4d8(ONLINE.EXE+0x000af4d8) BVA=2a3b14e9 FSR=00000001
FMD_OEMIoControl::Not supported IOCtl : 0x10303FF
查看coredll.map，
 0001:0001e52c       HeapCreate                 1001f52c f   coredll_ALL:heap.obj
 0001:0001e668       Int_HeapCreate             1001f668 f   coredll_ALL:heap.obj
 0001:0001e6c8       CeHeapCreate               1001f6c8 f   coredll_ALL:heap.obj
 0001:0001ebf4       HeapReAlloc                1001fbf4 f   coredll_ALL:heap.obj
 0001:0001ec14       HeapAlloc                  1001fc14 f   coredll_ALL:heap.obj
 0001:0001ec34       HeapAllocTrace             1001fc34 f   coredll_ALL:heap.obj
 0001:0001ec88       Int_HeapAlloc              1001fc88 f   coredll_ALL:heap.obj
 0001:0001eee8       HeapFree                   1001fee8 f   coredll_ALL:heap.obj
 0001:0001ef08       Int_HeapFree               1001ff08 f   coredll_ALL:heap.obj
 0001:0001f0e0       HeapSize                   100200e0 f   coredll_ALL:heap.obj
 0001:0001f100       Int_HeapSize               10020100 f   coredll_ALL:heap.obj
发现coredll.dll+0x0002006c对应的函数为：
0x0002006c-00001000=0x0001F06C,那么对应的起始函数为 Int_HeapFree ，堆资源释放问题，
同理，coredll.dll+0x0001fb04对应的函数为： CeHeapCreate，堆资源创建问题。
追踪程序发现：plastSaveTime = new char[mea_TotalNumber*7+14]，在初始化时mea_TotalNumber =0，
导致实际申请容量与想要使用的不一致，这样操作plastSaveTime 时，操作到别的堆空间了，导致释放别的堆资源出错。
2、
**     如何定位WinCE产生Data Abort错误的位置**
在调试WinCE程序的时候，有时候会碰到Data Abort的异常，系统会在调试控制台输出如下类似信息：
Exception 'Data Abort' (4): Thread-Id=03d3000e(pth=83a9e024), Proc-Id=00400002(pprc=81d48308) 'NK.EXE', VM-active=00fa000a(pprc=83a20ecc) 'explorer.exe'
PC=c0883660(s3c2440disp.dll+0x00003660) RA=a014f780(???+0xa014f780) SP=d2e8fc58, BVA=00000000
对于按Release编译的程序，需要一些额外的工作才能找到产生错误的地方，下面描述了如何根据错误提示信息定位错误代码的过程：
·让编译器产生map和cod文件
设置WINCEMAP和WINCECOD两个环境变量，然后重新编译整个项目，我是在BAT文件中加入这两行代码：
set WINCEMAP=1
set WINCECOD=1
·计算错误产生的地址
错误产生在s3c2440disp.dll内部，打开文本文件s3c2440disp.map（与s3c2440disp.dll在同一目录），在文件头中，"Preferred load address is 10000000"指出Rav地址为0x10000000，将其与错误产生的偏移地址0x00003660相加，得到Rva+Base地址为0x10003660(0x10000000
 + 0x00003660)
·找出错误来自哪个文件哪个函数
在s3c2440disp.map文件中，根据Rva+Base地址，得知错误产生于s3c2440disp.obj中的S3C2440DISP::CursorOff成员函数内部
0001:00002274       ?CursorOn@S3C2440DISP@@QAAXXZ 10003274 f   s3c2440disp.obj
0001:00002578       ?CursorOff@S3C2440DISP@@QAAXXZ 10003578 f   s3c2440disp.obj
0001:000026f4       ?SetPointerShape@S3C2440DISP@@UAAJPAVGPESurf@@0HHHH@Z 100036f4 f   s3c2440disp.obj
·计算错误在函数中的偏移地址
CursorOff函数起始地址为0x10003578，错误指令在代码中的偏移为0xE8 (0x10003660 - 0x10003578)
·找到错误出自哪行代码
打开文件s3c2440disp.cod文件(与s3c2440disp.obj在同一目录)，定位到CursorOff函数偏移0xE8处(0x10003660 - 0x10003578, 错误地址-函数开始地址)，即可找到了错误发生的汇编代码和对应的C源代码
; 624  :     {
; 625  :      break;
; 626  :     }
; 627  :
; 628  :     ptrLine[x * (m_colorDepth >> 3)] = cbsLine[(x - m_CursorRect.left) * (m_colorDepth >> 3)];
  000d0 e59430c0  ldr         r3, [r4, #0xC0]
  000d4 e59420e4  ldr         r2, [r4, #0xE4]
  000d8 e1a001a3  mov         r0, r3, lsr #3
  000dc e0463002  sub         r3, r6, r2
  000e0 e0227093  mla         r2, r3, r0, r7
  000e4 e021e690  mla         r1, r0, r6, lr
000e8 e5d23000  ldrb        r3, [r2]
  000ec e5c13000  strb        r3, [r1]
(错误行的汇编代码表明CPU在读入cbsLine指针指向的数据时发生了异常，经过调试，最终得知错误产生是因为某些情况下cbsLine指针无效而导致Data Abort异常)
//================================================================
备注：：
1》这篇文章提到了在WINCE OS下怎么产生MAP跟COD文件，很方便调试！！！
