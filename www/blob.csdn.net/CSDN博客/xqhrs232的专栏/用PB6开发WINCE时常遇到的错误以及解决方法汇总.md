# 用PB6开发WINCE时常遇到的错误以及解决方法汇总 - xqhrs232的专栏 - CSDN博客
2012年12月05日 17:07:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：959
原文地址::[http://blog.csdn.net/kejiazhw/article/details/7079272](http://blog.csdn.net/kejiazhw/article/details/7079272)
相关网帖
1.wince系统报错总结 ----[http://blog.chinaunix.net/uid-22597935-id-3290516.html](http://blog.chinaunix.net/uid-22597935-id-3290516.html)
Exception 'Raised Exception' (-1):   数组越界
Exception 'Data Abort' (4):          栈出问题了/空指针/访问的数据块无效了(如内存已经回收无效了)
Exception 'Prefetch Abort' (3):      空指针
\\==================================================================================================
在调试时首先要打开串口调试信息，我的开发板是mini2440，打开串口调试信息，只需要修改..\src\inc\option.h的文件,选择：#define KITL_SERIAL_UART0
下面，是我在实际的开发过程中遇到的一些问题，希望此文能给一些同样遇到下面这些错误的朋友一些帮助……
**1、错误代码：**CEFilter error. Missing "@CESYSGEN IF" statement(s) detected.
解决方法：删除..BSP\Src\Common DIRS file中的:
“# @CESYSGEN ENDIF CE_MODULES_SDNPCID_IMGUPD”
**2、错误代码：**U1073: don't know how to make  XXXXXXX
解决方法：
修改相应的source文件，看看下面两项是否齐全：
TARGETLIBS=\
   $(_COMMONSDKROOT)\lib\$(_CPUINDPATH)\coredll.lib \
SOURCELIBS=$(_TARGETPLATROOT)\lib\$(_CPUINDPATH)\ceddk.lib
如果不全，补全它。
**3、错误代码：**local function definitions are illegal
解决方法：程序中有漏写了大括号，找到该位置并补全它
**4、错误代码：**驱动编译通过，加载驱动时出现错误
(1)VirtualAlloc 和VirtualCopy调用失败
(2)Exception 'Data Abort' (4): Thread-Id=00d60002(pth=83cc7464), Proc-Id=00400002(pprc=82008308) 'NK.EXE', VM-active=00d70006(pprc=83cbae88) 'udevice.exe'
PC=c0821744(mykey.dll+0x00001744) RA=c0821734(mykey.dll+0x00001734) SP=d042e938, BVA=d09d0060
Exception 'Raised Exception' (-1): Thread-Id=00d60002(pth=83cc7464), Proc-Id=00400002(pprc=82008308) 'NK.EXE', VM-active=00d70006(pprc=83cbae88) 'udevice.exe'
PC=c003b6c8(k.coredll.dll+0x0001b6c8) RA=8023848c(kernel.dll+0x0000648c) SP=d042e1d4, BVA=ffffffff 
（3）测试应用程序中CreateFile失败，用GetLastError返回的错误代码为：55
解决方法：
错误说明：虚拟分配地址时出错，导致后面的数据访问出现错误。VirtualCopy中第二个参数要用虚拟地址，而不是物理地址，所以把把第二个参数S3C2440A_BASE_REG_PA_IOPORT改为IOP_BASE则可以了。如果要用物理地址(例如驱动访问cpu寄存器)，则第三个参数则要加上PAGE_PHYSICAL，并且第二个参数要除以8（>>8）。
#define S3C2440A_BASE_REG_PA_IOPORT 0x56000000
#define IOP_BASE  0xB1600000
修改前：
//GPIO virtual allocation
 IO_REG=(S3C2440A_IOPORT_REG*)VirtualAlloc(0,sizeof(S3C2440A_IOPORT_REG),
MEM_RESERVE,PAGE_NOACCESS);
 if(!IO_REG) RETAILMSG(debug,(TEXT("gpg virtual allocation failure.\r\n")));
 else {
 VirtualCopy(IO_REG,(S3C2440A_IOPORT_REG*)S3C2440A_BASE_REG_PA_IOPORT,
sizeof(S3C2440A_IOPORT_REG),PAGE_READWRITE);
 RETAILMSG(debug,(TEXT("gpg virtual allocation success.\r\n")));
}
改正后：（1）
s2440IOP = (volatile S3C2440A_IOPORT_REG *) VirtualAlloc(0,sizeof(S3C2440A_IOPORT_REG),MEM_RESERVE, PAGE_NOACCESS);
 if(!s2440IOP) RETAILMSG(debug,(TEXT("gpg virtual allocation failure.\r\n")));
 else {
 RETAILMSG(debug,(TEXT("gpg virtual alloc success.\r\n"))); if(!(VirtualCopy((PVOID)s2440IOP,(PVOID)(S3C2440A_BASE_REG_PA_IOPORT>>8),sizeof(S3C2440A_IOPORT_REG), PAGE_PHYSICAL | PAGE_READWRITE | PAGE_NOCACHE)))
RETAILMSG(debug,(TEXT("gpg virtual copy failure.\r\n")));
 else
RETAILMSG(debug,(TEXT("gpg virtual copy success.\r\n")));
}
改正后：（2）
//GPIO virtual allocation
 IO_REG=(S3C2440A_IOPORT_REG*)VirtualAlloc(0,sizeof(S3C2440A_IOPORT_REG),
MEM_RESERVE,PAGE_NOACCESS);
 if(!IO_REG) RETAILMSG(debug,(TEXT("gpg virtual allocation failure.\r\n")));
 else {
 VirtualCopy(IO_REG,(S3C2440A_IOPORT_REG*)( IOP_BASE),
sizeof(S3C2440A_IOPORT_REG),PAGE_READWRITE | PAGE_NOCACHE);
 RETAILMSG(debug,(TEXT("gpg virtual allocation success.\r\n")));
}
其它原因：寄存器配置（初始化）错误
**5、错误代码：**
regcomp: Error: InitRegistry FAILED on or near line 970 of D:\Mini2440-with-QQ\Mini2440-with-QQ\RelDir\Mini2440_ARMV4I_Release\reginit.ini.makeimg: FATAL ERROR: Command returned non-zero exit code 1 (dec).
解决方法：？？？？？？？？？？？？？？？？？？？
A、清理解决方案-》生成解决方案--------这个有待验证，我当时好像这样有效
**6、错误现象：**外部IO中断（EINT8—23），中断没反应，在waitforoneobject处一直阻塞
分析：外部中断有一个总中断源EINT8_23(5号中断)，EINT8～EINT23都是其的子中断源，在用KernelIoControl(IOCTL_HAL_REQUEST_SYSINTR,&IRQK,sizeof(UINT32),
&keySYSIntr,sizeof(UINT32),NULL)，(其中IRQK=5)实现由物理中断申请系统中断时不应该用总中断号EINT8_23(5号中断)，而应该采用具体的中断号比如IRQ_EINT8(比如中断号36)。
**7、错误现象：**oal_rtc_pcf8563.lib(pcf8563.obj) : error LNK2019: unresolved external symbol ResetEvent referenced in function OEMGetRealTime
分析：查msdns可得ResetEvent定义在coredll.lib中，而source文件中也加入了
TARGETLIBS= \$(_COMMONSDKROOT)\lib\$(_CPUINDPATH)\coredll.lib \这个库，为什么会找不到呢。原来RTC这个项目类型我是设为静态库（library)，而TARGETLIBS后面跟的库是动态链接进去的。而这里我们需要静态链接进去，进行编译。所以，只要把上面的改为：
SOURCELIBS=\$(_COMMONSDKROOT)\lib\$(_CPUINDPATH)\coredll.lib \就OK了。
TARGETNAME=oal_rtc_pcf8563
FILE_VIEW_ROOT_FOLDER= \
    ReadMe.txt \
FILE_VIEW_RESOURCE_FOLDER= \
FILE_VIEW_INCLUDES_FOLDER= \
SOURCES= \
    pcf8563.c \
    i2c.c \
RELEASETYPE=PLATFORM
TARGETLIBS= \
    $(_COMMONSDKROOT)\lib\$(_CPUINDPATH)\coredll.lib \
INCLUDES= $(_TARGETPLATROOT)\SRC\inc;$(_COMMONOAKROOT)\inc;$(INCLUDES)
SOURCELIBS=\
$(_COMMONSDKROOT)\lib\$(_CPUINDPATH)\coredll.lib \
TARGETTYPE=LIBRARY
**8、错误现象：**出现变量未定义（但是明明上面已经定义了）
分析：在c语言中，变量要定义在最前面，否则编译器识别不到会认为该变量没有定义
**9、错误现象：**fatal error LNK1112: 模块计算机类型“THUMB”与目标计算机类型“ARM”冲突
解决方法：1.新建项目时,在"平台"->"选择要添加到当前项目中的 Platform SDK。"中,把"已安装的 SDK"全部添加到"选定的 SDK"
2.在"属性页"->"配置属性"->"链接器"->"命令行"中的"附加选项"里,有如下命令:"/subsystem:windowsce,4.20 /machine:ARM /ARMPADCODE",在其后加入命令:" /MACHINE:THUMB",即附加选项现在内容为:" /subsystem:windowsce,4.20 /machine:ARM /ARMPADCODE /MACHINE:THUMB",确定即可.
**10、错误现象：**ActiveSync自动断开，如果拔掉usb线重连，则会导致死机
解决方法：点击wince系统的桌面（非PC端）的状态栏的Async同步图标（只要同步上了就会出现），在弹出的对话框中单击断开连接，再操作就不会出现PC端死机的现象
（这个现象有些电脑会出现，有些又不会，至于我的解决方法，可能有很大的局限性……）
**11、错误现象：**IIC驱动，用EEPROM（at24c08）调试，读数据时出现读数出错的情况：
interrupt occured.
interrupt occured.
interrupt occured.
interrupt occured.
READ address is:70
READ DATA1:1
interrupt occured.
READ DATA2:2
interrupt occured.
READ DATA3:3
interrupt occured.
READ DATA4:4
interrupt occured.
READ DATA5:5
interrupt occured.
READ DATA6:6
interrupt occured.
READ DATA7:7
interrupt occured.
READ DATA8:8
interrupt occured.
READ DATA9:9
interrupt occured.
READ DATA10:10
interrupt occured.
READ DATA finish.
iic bus read.
interrupt occured.
interrupt occured.
interrupt occured.
interrupt occured.
READ address is:71
READ DATA1:255
interrupt occured.
READ DATA2:255
interrupt occured.
READ DATA3:255
interrupt occured.
READ DATA4:255
interrupt occured.
READ DATA5:255
interrupt occured.
READ DATA6:255
interrupt occured.
READ DATA7:255
interrupt occured.
READ DATA8:255
interrupt occured.
READ DATA9:255
interrupt occured.
READ DATA10:255
interrupt occured.
READ DATA finish.
解决方法：降低传输速度
**12、错误现象：**error LNK2019: unresolved external symbol iic_init referenced 
但是我明明用extern声明了它为外部函数，按照错误9的方法处理后，错误变成了：
BUILD: [00:0000000036:ERRORI] Tried to look up directory info for directorywith no drive:\D:\WINCE600\Mini2440-with-QQ\Mini2440-with-QQ\Wince600\Mini2440_ARMV4I\cesysgen\sdk\lib\ARMV4I\retail\coredll.lib.
再之后，我把加进去的SOURCESLIBS语句删除了，再进行BUILD，竟然通过了，不解？
**13、问题：**BUILD: [01:0000000136:ERRORE] NMAKE :  U1073: don't know how to make 'RTC.def'
BUILD: [01:0000000138:ERRORE] NMAKE.EXE TargetLibFiles  -i -c BUILDMSG=Stop.  BUILDROOT=J:\programfile\WindowsCE6\PLATFORM\Mini2440 LINKONLY=1 NOPASS0=1 failed - rc = 2.
解决方法：def文件名跟你实际的不匹配
**14、问题：**supervivi烧进nandflash后，启动时直接跳到启动内核，没有出现supervivi的菜单
分析：友善的supervivi规定：当它烧进norflash时，并从norflash启动时，它会自动出现supervivi的菜单
当它烧进nandflash时，并从nandflash启动时，它会直接跳到内核的启动，除非按着开发板上的按键（不是电脑键盘，这6个按键分别连接EINT8,EINT11,EINT13,EINT14,EINT15,EINT19），它才会进入supervivi的菜单。
**15、问题：**下载WINCE内核时，出现Checksum error（注：可以下载其它）
分析：可能的错误原因：
n 使用其它版本的supervivi
n USB接口有问题
n 晶振问题
n 时钟问题
n 内存问题
结论：换成其它型号的SDRAM就可以checksum成功。原先的型号为winbond 的W9825G6EH-6，替换的型号为：PSC的A3V56S40ETP
**16问题：**Hjtag烧写flash，can't download driver to specified address
问题原因：初始化脚本设置错误，还有就是SDRAM管脚短路或断路（待验证）
**17问题：**4X4矩阵键盘驱动，按一次按键产生多个中断
分析：当中断产生进行中断扫描，并且未等到按键的释放就重新打开中断，故有此问题
  经此理后还是存在这个问题，找了还久，原来是自己写代码时不规范造成的，晕！
  扫描完键值后，首先把所有行拉低（F0~3行输出，F4~7列输入（中断））
  GPFDAT |=0x0F;//应改为：GPFDAT &=0x0F;
  ……
while(((GPFDAT & 0x0F) & 0x0F)!=0x0F);//wait for key release
  总结：对寄存器操作时要特别小心，对要清0的位用与0来实现，要置1的位用或1来实现，对于其它没有用到的位最好要保持原值不变，实现方法例如：
GPFDAT |=0x0F;//低四位置1
GPFDAT &=0xF0;//低四倍清0
**GPFDAT =(GPFDAT & 0xF0) | 0x0E; /*  低四位设为1110，对于同时设几位，而这几位又不都为1时，要特别注意避免直接使用|=  */**
以上是我实习二个月来，从事一个WINCE开发项目时遇到的一些问题，由于当时也是初次接触WINCE，而且事隔半年后的今天，才从笔记本中整理出来，
难免有疏漏之处，请大家多多担待和不吝指正……
