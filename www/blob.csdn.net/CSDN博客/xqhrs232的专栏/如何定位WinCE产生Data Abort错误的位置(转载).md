# 如何定位WinCE产生Data Abort错误的位置(转载) - xqhrs232的专栏 - CSDN博客
2009年11月12日 09:30:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1328
 在调试WinCE程序的时候，有时候会碰到Data Abort的异常，系统会在调试控制台输出如下类似信息：
Exception 'Data Abort' (4): Thread-Id=03d3000e(pth=83a9e024), Proc-Id=00400002(pprc=81d48308) 'NK.EXE', VM-active=00fa000a(pprc=83a20ecc) 'explorer.exe'
PC=c0883660(s3c2440disp.dll+0x00003660) RA=a014f780(???+0xa014f780) SP=d2e8fc58, BVA=00000000
对于按Release编译的程序，需要一些额外的工作才能找到产生错误的地方，下面描述了如何根据错误提示信息定位错误代码的过程：
让编译器产生map和cod文件 
设置WINCEMAP和WINCECOD两个环境变量，然后重新编译整个项目，我是在BAT文件中加入这两行代码：
set WINCEMAP=1
set WINCECOD=1
计算错误产生的地址 
错误产生在s3c2440disp.dll内部，打开文本文件s3c2440disp.map（与s3c2440disp.dll在同一目录），在文件头中，"Preferred load address is 10000000"指出Rav地址为0x10000000，将其与错误产生的偏移地址0x00003660相加，得到Rva+Base地址为0x10003660(0x10000000 + 0x00003660)
找出错误来自哪个文件哪个函数 
在s3c2440disp.map文件中，根据Rva+Base地址，得知错误产生于s3c2440disp.obj中的S3C2440DISP::CursorOff成员函数内部
0001:00002274       [?CursorOn@S3C2440DISP@@QAAXXZ](mailto:?CursorOn@S3C2440DISP@@QAAXXZ) 10003274 f   s3c2440disp.obj
0001:00002578       [?CursorOff@S3C2440DISP@@QAAXXZ](mailto:?CursorOff@S3C2440DISP@@QAAXXZ) 10003578 f   s3c2440disp.obj
0001:000026f4       [?SetPointerShape@S3C2440DISP@@UAAJPAVGPESurf@@0HHHH@Z](mailto:?SetPointerShape@S3C2440DISP@@UAAJPAVGPESurf@@0HHHH@Z) 100036f4 f   s3c2440disp.obj
计算错误在函数中的偏移地址 
CursorOff函数起始地址为0x10003578，错误指令在代码中的偏移为0xE8 (0x10003660 - 0x10003578)
找到错误出自哪行代码 
打开文件s3c2440disp.cod文件(与s3c2440disp.obj在同一目录)，定位到CursorOff函数偏移0xE8处(0x10003660 - 0x10003578, 错误地址-函数开始地址)，即可找到了错误发生的汇编代码和对应的C源代码
; 624  :     {
; 625  :      break;
; 626  :     }
; 627  : 
; 628  :     ptrLine[x * (m_colorDepth >> 3)] = cbsLine[(x - m_CursorRect.left) * (m_colorDepth >> 3)];
  000d0 e59430c0  ldr         r3, [r4, #0xC0]
  000d4 e59420e4  ldr         r2, [r4, #0xE4]
  000d8 e1a001a3  mov         r0, r3, lsr #3
  000dc e0463002  sub         r3, r6, r2
  000e0 e0227093  mla         r2, r3, r0, r7
  000e4 e021e690  mla         r1, r0, r6, lr
  000e8 e5d23000  ldrb        r3, [r2]
  000ec e5c13000  strb        r3, [r1]
(错误行的汇编代码表明CPU在读入cbsLine指针指向的数据时发生了异常，经过调试，最终得知错误产生是因为某些情况下cbsLine指针无效而导致Data Abort异常)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/noodies/archive/2009/07/02/4315948.aspx](http://blog.csdn.net/noodies/archive/2009/07/02/4315948.aspx)
