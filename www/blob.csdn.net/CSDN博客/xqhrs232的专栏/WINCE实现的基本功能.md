# WINCE实现的基本功能 - xqhrs232的专栏 - CSDN博客
2008年09月19日 20:54:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1519标签：[wince																[exe																[flash																[数据结构																[dll																[image](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=flash&t=blog)](https://so.csdn.net/so/search/s.do?q=exe&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
2008--8--6
1。修改LCD参数--------支持的像素/RGB格式《5/6/5---5/5/5》/前后仰参数修改
2。USB键盘与鼠标/U盘
3。电池电量检测-------BAT组件
4。注册表永久保存
5。多国语言支持----MUI
6。ACTIVESYNC同步功能实现
7。CETK调试的CLIENTSIDE。EXE
8。COM/DCOM组件
9。SQL CE数据库
10。DB数据库
//--------------------------------------------------
11。三个串口的支持
12。I2C流驱动--------标准I2C接口/IO模拟接口-------接口协议
13。SPI流驱动--------标准SPI接口/IO模拟接口-------接口协议
14。CS8900/DM9000网络芯片支持
15。背光灯控制
16。挂起，唤醒，休眠的电流控制-----3MA
17。对话框的动画效果
18。MOUSE的去除
19。XP风格的设置
20。控件颜色设置
//--------------------------------------------------
21。IOCTL控制宏
22。DEVICEIOCONTROL
23。KERNERLIOCONTROL
24。系统关机
25。触摸屏上电校准
26。怎么调触摸屏校准程序
27。怎么修改注册表
28。LCD屏的风格设置-----水平画/肖像话
29。产生自己的字体
30。NK的本地化
//--------------------------------------------------
31。ATA硬盘驱动-------电子硬盘
32。怎么成为一个U盘
33。VIRTUALCOPY/VIRTUALALLOC/VIRTUALFREE----三个函数的使用
34。CEDDK。DLL里面的访问I/O口的一系列函数
35。NBOOT/EBOOT/NK-----三个的功能与区别
36。BIN/NB0的联系与区别
37。编译的几个配置文件------DIRS/SOURCE
38。如何更快地编译NK/BSP-----分开编译，只SYSGEN
39。各种文件系统-----BINFS/FAT/TFAT
40。NANDFLASH的低格式化与高格式化
//--------------------------------------------------
41。软键盘的加入与调用
42。怎么隐藏任务栏与我的电脑图标
43。MFC与ATL属性加入
44。各种文件浏览器的加入-----PDF/DOC/PPT/IMAGE
45。WIFI功能
46。RTC的实现
47。怎么修改REG/BIB文件
48。怎么加一个文件进内核NK
49。怎么创建一个快捷方式
50。键盘驱动修改-------------开线程扫描
//------------------------------------------------
51。对SD卡的支持
52。AC97/I2S驱动开发
53。CAMERA接口驱动开发
54。BTH蓝牙驱动开发
55。安全函数组件
56。怎么加开机画面
57。NBOOT/EBOOT在NAND FLASH的块号
58。中断机制什么实现----------------硬件中断怎么跟系统中断绑定
59。分区表---------DOS32分区，FAT32分区
60。系统怎么支持DIRECTSHOW/DIRECTDRAW
61。COMMON  CONTROL组件
62。SJF2440。EXE怎么找CPU，怎么烧NAND FLASH/NOR FLASH
63。KITL功能
64。通过USB接口烧EBOOT/NK
65。怎么发声----定制触摸点击的声音
66。怎么查看reginit.ini文件看，当前注册表有些什么内容
67。VIEWBIN -T NK。BIN 看NK的内容
68。DUMPBIN/EXPORTS  XXX。DLL看DLL导入/导出/节的内容
69。SET/set命令的使用
70。仔细查看BUILD的报错消息-----MS的东西有很多的提示
//----------------------------------------------------
71。NAND FLASH的大块驱动与小块驱动
72。加载地址----0X30038000/0X30200000
73。TOC数据结构------地址记录
74。同步组件----怎么看能不能同步（s3c2440usbfn.dll, asyncmac.dll,repllog.exe,rapisrv.exe,rnaapp.exe)
75。网络组件的加入-----CS8900/DM9000
76。CEMGR。EXE
77。SERVICES。EXE
78。GRAPHEDT。EXE
79。CEMON。EXE------用于调试
80。应用程序的调试------单步/断点/运行
//--------------------------------------------------
81。游戏组件
82。怎么改变BSP设置的环境变量------platform/setting/enivronment++++++SET命令
83。platform/setting/custom build action-----
     a.pre-sysgen
     b.post-sysgen
     c.pre-make image
     d.post-make image
84。platform/setting/build options-------怎么选择
85。怎么配置模拟环境，激活模拟环境
86。怎么管理CEC文件-----加入新的CEC文件/删除已有的CEC文件
87。怎么导出SDK开发包
88。TRACE0/1/2调试宏的使用
89。RETAILMSG命令的使用
90。DEVICE/EMULATOR的选择-----导出的SDK有两种（模拟环境下的SDK开发包）
//--------------------------------------------------------------------
91。远程的几个工具----ZOOM/REG
92。注册表查看工具-----远程工具/EXE文件
93。各个窗体的调出与隐藏
94。coredll.dll系统NK的核心函数全部在这里面导出
95。NK的ROMHDR说明----提供了一些有用的消息
96。LCD屏幕的旋转-----显示旋转支持类
97。怎么获取当前的内存使用状态
98。WIFI/BlueTooth/GPS/GPRS/GSM
99。SDK包中增加PLATFORM MANAGER的支持-------三种连接方式（ACTIVE SYNC/TCP IP /MANNUAL ）
100。CETK的使用-----测试驱动程序
//---------------------------------------------------------------------
101。MBR记录----分区表-----DOS分区/BINFS分区
102。4KB技术-----Stepping stone技术《上电就执行》
103。使用那一个串口来做为调试串口-----怎么指定，怎么进行初始化
104。JTAG口的原理----14/20PIN
105。NK重定像
106。DNW+++++USB驱动++++USB同步驱动
107。NK的初始化过程-------------ArmInit( )++++OEMInit( )
108。注意NAND FLASH对NBOOT，TOC，EBOOT，NK，剩余空间，的分配，LOADER。H文件指定。
109。怎么输出调试消息-----outputdebugstring/retailmsg/trace0/oalmsg/EdbgOutputDebugString
110。对IOCTL的支持-------WINIOCTRL。H头文件
//---------------------------------------------------------------------
111。OALVAtoPA( )----虚拟地址与物理地址的转换
112。ceddk.dll中的I/O接口函数
113。lstrcpy/lstrcat/lstrlen
114。memcpy/memset/
115。VirtualAlloc/VirtualCopy/VirtualFree
116。LocalAlloc/LocalCopy/LocalFree
117。GlobalAlloc/GlobalCopy/GlobalFree
118。LOADER。H文件中的常量定义与BOOT。BIB++++CONFIG。BIB文件中的定义怎么对应起来
119。Log2Phys( )函数的功能
120。FMD_XXX接口-------DSK_XXX接口
//---------------------------------------------------------------------
121。低级格式化(F)++++高级格式化(9)《产生MBR记录》++++产生BINFS分区《存放NK的地方》++++产生DOS32分区《NAND FLASH多余的空间，成为文件夹》
122。InterruptInitialize( )++++ InterruptDone( )++++CreateEvent( )++++CreateThread( )
123。OEMInterruptHandler( )被内核调用
124。Config.bib++++booot.bib--------------Loader.h文件的对应关系
125。通过EBOOT更新NBOOT+EBOOT++++NK
126。第一次通过JTAG烧NBOOT+EBOOT---------注意这个块号要跟LOADER。H文件里面定义的要对应起来，要不然系统会起不来的。
127。OEMInterruptEnable++++OEMInterruptDisable++++OEMInterruptDone
128。OALIntrEnableIrqs++++OALIntrDisableIrqs++++OALIntrDoneIrqs
129。BSPIntrEnableIrq++++BSPIntrDisableIrq++++ BSPIntrDoneIrq
130。MBR的数据结构
//-----------------------------------------------------------------------
131。BOOTPART库-----接口函数
132。MMU功能，开启与关闭
133。唤醒的外部中断设置
134。DMA的使用--------------快速交换数据
135。regedit.exe
136。rotate.exe旋转LCD屏幕
137。CE下得到当前的内存状态-----GlobalMemoryStatus（ ）++++ CeGlobalMemoryStatus（ ）
138。
