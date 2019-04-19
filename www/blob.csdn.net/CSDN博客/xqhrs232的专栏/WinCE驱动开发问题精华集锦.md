# WinCE驱动开发问题精华集锦 - xqhrs232的专栏 - CSDN博客
2013年01月29日 09:30:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：720
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/maoxiaosan/article/details/7661955](http://blog.csdn.net/maoxiaosan/article/details/7661955)
如何让系统加载自己写的驱动程序？
两种办法：  
1、在[HKEY_LOCAL_MACHINE/Drivers/BuiltIn]下添加注册键。  
2、在应用程序中调用ActivateDeviceEx。  
在一些文件中用分号来表示注释，例如下面的内容   
; @CESYSGEN IF SERVERS_MODULES_HTTPD  
; @CESYSGEN ENDIF  
在“CESYSGEN...”前加了“@”，有没有什么特别的含义？  
在WINCE的一些文件中，用“;”作为注释并在注释文字中用@CESYSGEN作为标记，后面接条件语句。Cefilter.exe工具负责按照条件来筛选文件内容，所以不要轻易地删除包含@CESYSGEN的注释语句。  
 通过串口建立ActiveSync联接,串口线用三线的可以吗?   
不可以，因为用串口同步时要用到其余口的状态。  
    WINCE是否支持MAPI？
不支持。WINCE自带的pmail.exe软件也不是很好用。建议自开发邮件收发软件。如果需要购买WINCE下邮件收发软件可以联系我。  
如何旋转屏幕显示的内容？
例子代码如下（前提是显示驱动程序支持旋转）：  
DEVMODE  devmode = {0};  
 devmode.dmSize = sizeof(DEVMODE);  
 devmode.dmDisplayOrientation = DMDO_90;       ///垂直模式  
 devmode.dmFields = DM_DISPLAYORIENTATION;  
 ChangeDisplaySettingsEx(NULL, &devmode, NULL, 0, NULL);  ///改变显示的设置  
 CRect  rcWorkArea(0, 0, 320, 240);    ///整个屏幕尺寸  
 ///设置客户区大小并广播消息，这样所有软件也就随之更改显示  
 SystemParametersInfo(SPI_SETWORKAREA, 0, (void*)&rcWorkArea, SPIF_SENDCHANGE);   
 请问如何修改字形缓存的容量？  
[HKEY_LOCAL_MACHINE/System/GDI/GLYPHCACHE]  
"limit"=dword:0400  
 如何得到从WINCE启动开始到现在的时间？
调用API GetTickCount，得到的值为32位整数，单位为毫秒。  
 如何调用WINCE的软键盘？
调用API SipShowIM(SIPF_ON)，前提是内核加入了软键盘组件。  
 基于HIVE的注册表，如何在系统关闭前保存注册表的数据到文件system.hv？
调用API RegFlushKey函数。  
 使用VirtualAlloc和VirtualCopy的时候需要注意哪些事项？
1、VirtualAlloc的作用是申请虚拟地址空间，这肯定不是最终的目的，最终目的可能是申请物理内存、映射寄存器、提交文件等。没有一个目的会在意虚拟地址空间的位置，所以尽量传递参数1为0，也就是让WINCE自动分配虚拟地址空间。VirtualAlloc分配地址空间实际上是以64KB为单位，所以要指定申请的虚拟空间的首地址的话，参数1应该为64KB的整数倍，申请的长度也应该为64KB的整数倍，即使你不需要那么大。  
2、VirtualCopy的主要作用是映射物理地址空间，如果参数2为物理地址，那么最后一个参数要添加PAGE_PHYSICAL，参数2必须是256的整数倍。如果参数2为虚拟地址（0x80000000以上），那么最后一个参数就不要添加PAGE_PHYSICAL，WINCE内核会根据这个虚拟地址找到对应的物理地址。  
 驱动程序和应用程序之间传递数据时何时调用MapPtrToProcess？  
因为设备管理器负责加载驱动程序DLL，这意味着当应用程序调用驱动程序接口函数的时候，WINCE内核会将调用驱动程序接口函数的线程转移到设备管理器的进程空间然后执行具体的驱动程序代码，应用程序和设备管理器处于两个进程空间，这就造成设备管理器无法访问应用程序传递的指针（虚拟地址），所以当我们在应用程序中传递指针给流驱动程序接口函数时，WINCE内核从中作了一个地址映射，例如ReadFile、WriteFile、DeviceIoControl函数的参数凡是指针都经过了映射才传递给驱动程序，所以很多驱动程序开发者并不了解其中的奥秘就可以编程了。但是如果参数是一个指向一个结构体的指针，而结构体里包括一个或多个指针，那么WINCE内核并不负责映射，所以就需要开发者在驱动程序接口函数中调用API函数MapPtrToProcess来映射地址。例如：pPointer_retval = MapPtrToProcess(pPointer, GetCallerProcess());   
如何判断可插拔的设备是否存在？
1、通过查找注册表的值。凡是由API ActivateDeviceEx加载的驱动程序都在[HKEY_LOCAL_MACHINE/Drivers/Active]键下有注册键，通过查找“name”或者其它键值就能够找到。设备管理器就调用这个API。如果是PCI设备，在注册表[HLM/Drivers/BuiltIn/PCI/Instance]下查找关键字，例如[HLM/Drivers/BuiltIn/PCI/Instance/WaveDev1]，说明音频驱动已经加载。  
2、调用驱动程序接口函数，根据返回值或者执行结果来判断。  
如何做到通过串口过来的一个信号启动自己开发的应用程序？  
创建一个线程负责等待串口过来的信号，调用API SetCommMask设置要等待的信号种类，具体可以等待的信号种类参见参数2的说明。然后再调用API WaitCommEvent函数等待这个信号，接收之后再调用API CreateProcess启动应用程序。  
 在WINCE中如何只能启动应用程序的一个实例？
常用的两种办法：  
1、如果应用程序实例创建了窗口，可通过API FindWindow函数通过窗口类名和窗口标题名称来查找，前提是系统内不会出现窗口名称重复的情况。  
2、应用程序初始化的时候创建一个事件或互斥等内核对象，因为内核对象是由内核创建，名称在系统内唯一。  
 能不能自己编辑一个数字签名文件导入到手机上，这样就可以用这个签名签自己的程序了？
WINCE的内核签名机制的用途是限制非法的可执行模块EXE、DLL等在设备上运行。要求内核的加载模块用公钥验证请求加载的EXE、DLL的签名是否合法，而这个公钥是在定制内核的时候加进去的，所以除内核的定制者以外的人无法修改这个验证机制。  
 我按照版主的文章《加密WINCE系统》里操作，提示错误如下：  
Error 80090016 during CryptSignHash 1!  
Error signing hash  
这是因为传递了无效的钥容器名称，使CryptoAPI调用失败。应该在使用signfile工具之前创建一个钥容器，在桌面Windows中调用API CryptAcquireContext创建一个指定名称的钥容器，接着再创建一个签名密钥对，这时再使用signfile工具就可以了。我在文章里写成-kfulinlin是因为我创建钥容器的时候没有指定名称，系统就采用当前登录的用户名为容器名。  
编译错误：CVTRES : fatal error CVT1102: out of memory; 42 bytes required ？
多数情况下出现这种错误是因EVC的bug而起，应该在安装EVC之后就立刻安装EVC的SP补丁。另外为了避开BUG，使用EVC编程应该养成一些习惯，比如定期备份工程所有文件，每次编译时采用Clean + Rebuild All，正调试时不要关闭模拟器等等。  
 在WINCE下是否能够得到某一进程使用的物理内存总量？
目前没发现有这样一个API能够得到指定进程使用的物理内存总量。只有GlobalMemoryStatus能够得到整个系统使用的物理内存总量。  
 应用程序如何控制lcd的亮度？如何获得电池的电量？
从常见的平台如Geode、三星ARM系列来看，的确在驱动方面没有统一的控制LCD或者其它种类屏幕亮度的接口函数，所以只能根据具体平台提供的接口来做。从帮助文档来看微软的带有DirectDraw功能的显示驱动程序的确有标准的增加亮度的接口函数，关于背景光参见标题为“Enabling a Backlight”的帮助文档。  
获得电池电量有标准的接口函数GetSystemPowerStatusEx，前提是驱动程序和硬件都要支持。  
WINCE的socket函数好像不支持发送/接收超时？
是的，最早版本的WINCE支持选项SO_RCVTIMEO、SO_SNDTIMEO，后来却不支持了。  
WINCE下如何设置窗口最大化和最小化？
WINCE的帮助文档在介绍API ShowWindow函数的参数时指出SW_MAXIMIZE, SW_MINIMIZE, SW_RESTORE, SW_SHOWDEFAULT, SW_SHOWMAXIMIZED, SW_SHOWMINIMIZED, SW_SHOWMINNOACTIVE都不被支持，但实际上并不完全是这样，具体来说：  
SW_MAXIMIZE            比原来窗口大，但不是最大化  
SW_MINIMIZE             编译成功，但是不起作用  
SW_SHOWMAXIMIZED     最大化  
SW_SHOWMINIMIZED      编译出错  
SW_RESTORE              能恢复  
SW_SHOWDEFAULT        编译出错  
SW_SHOWMINNOACTIVE   编译出错  
SW_HIDE                  能够隐藏  
 如何用程序调用控制面板的触摸屏校对程序？
两种办法：  
1、调用API TouchCalibrate函数  
2、调用CreateProcess，参数1为L"//windows//ctlpnl.exe"，参数2为L"cplmain.cpl,9"。  
如何获得U盘或者其它类型的存储器总容量和剩余可用容量？
调用API GetStoreInfo得到扇区数、每扇区字节数，相乘即是总容量。调用API GetDiskFreeSpaceEx得到剩余可用容量。  
三星2440头文件定义#define IIC_BASE 0xB1400000 // 54000000，datasheet是54000000，那么怎么转成0xB1400000？
物理地址映射方法分为两种，一种静态映射另一种为动态映射。在OEMAddressTable中定义了物理地址与虚拟地址的映射关系属于静态映射，用VirtualCopy映射属于动态映射，采用哪种办法都可以。问题中提到的属于静态映射，2440的BSP在map.a文件中定义了IIC控制寄存器的物理起始地址和对应的虚拟地址如下：  
DCD 0x91400000, 0x54000000, 1 ;  
在OEMAddressTable中定义的虚拟地址范围在0x8000 0000—0x9FFF FFFF，这部分可缓存，适合内核程序和应用程序使用，同时WINCE内核在0xA000 0000—0xBFFF FFFF中映射了另一份，指向了同样的物理地址，这部分不可缓存，适合驱动程序使用。三星ARM处理器带有L1级高速缓存，可缓存会提高执行效率。对于特殊的设备寄存器适合映射到不可缓存的虚拟地址。  
当驱动程序调用VirtualCopy对0xB1400000地址读写时，WINCE自动将这个地址减去0x2000 0000，也就是0x91400000，对应的物理地址就是0x54000000，也就是IIC控制寄存器的物理起始地址。  
基于RAM的注册表如何保存数据？
调用API RegCopyFile备份注册表。调用API RegRestoreFile恢复注册表，然后调用KernelIoControl热启动使恢复生效。  
如何隐藏和显示winCE下标准外壳的任务栏？
 HANDLE  hTaskBar = FindWindow(L"HHTaskBar", NULL);   
 ShowWindow(hTaskBar, SW_HIDE);   
 ShowWindow(hTaskBar, SW_SHOWNORMAL);   
如果能让WINCE的IE浏览器播放flash动画？
播放flash需要Macromedia Flash Player SDK，参见http://www.adobe.com/products/flashplayer_sdk/。这和real player相似，都需要WINCE平台的SDK，都需要申请。  
WINCE下内核模式和用户模式有什么区别？
为了使读者能够详细了解WINCE的地址映射原理还有两种模式，在这里我分几个部分说明：  
1、WINCE内核nk.exe的任务是管理操作系统核心功能。按照OEMAddressTable的映射要求，所有物理地址都映射到0x80000000以上，所以对于内核程序nk.exe和内核模式下的线程来说，只要访问0x80000000以上的有效虚拟地址经MMU就能够访问物理地址，无需再映射是内核模式的一个特点。内核模式的第二个特点是没有地址访问限制，内核模式线程可以访问任何有效虚拟地址，所谓有效虚拟地址是指有实际事物对应。  
2、用户模式线程只能访问0x80000000以下的虚拟地址空间，WINCE6.0之前版本的内核为每个进程划分32MB的地址空间，在不调用特殊函数的情况下不能相互访问，这样的设计使得WINCE系统更安全、更稳定，限制访问地址是用户模式的第一个特点。第二个特点就是需要多一层映射，如果线程要访问物理内存的话需要先映射到0x80000000以上，再经MMU访问物理内存地址。  
WINCE的线程具有转移性（参考API GetCallerProcess的说明，有一个很好的例子），当应用程序的线程调用API或者调用驱动程序接口函数时，该线程会转移到gwes.exe、device.exe、filesys.exe等进程中执行，转移是由WINCE内核操作的，它会修改线程的上下文，记录线程的当前进程、调用者进程、拥有者进程三个值。  
3、如果在定制内核的时候选择了“Full Kernel Mode”，那么在这个内核上运行的所有线程都处于内核模式，即使调用SetKMode(FALSE)后线程仍然具有内核模式的特点，能够访问任何有效的虚拟地址。假设现有一个64MB RAM的WINCE产品，RAM映射从0x80000000到0x84000000，如果线程处于内核模式，它就直接可以访问这个范围的虚拟地址：  
在OnButton1()中编写  
DWORD oldMode = SetKMode(FALSE);  
volatile int *piTemp = (volatile int*)(0x20000000+0x84000000-0x00019000); ///或者(0x84000000-0x00019000)  
*piTemp = 12345;  
在OnButton2()中编写  
DWORD oldMode = SetKMode(FALSE);  
volatile int *piTemp = (volatile int*)(0x20000000+0x84000000-0x00019000); ///或者(0x84000000-0x00019000)  
int iTemp = *piTemp;  
先只执行OnButton1()然后关闭程序，再重启程序然后执行OnButton2()，iTemp仍然等于12345。结果说明了两点：内核模式线程可以直接访问0x80000000以上的有效虚拟地址；我们写到RAM中的数据没有丢失，说明虚拟地址有效。  
如果在定制内核的时候没有选择“Full Kernel Mode”，那么在这个内核上运行的所有线程都处于用户模式。可以调用SetKMode(TRUE)使调用线程暂时处于内核模式，还是原来的假设环境，我再举个例子：  
在OnButton1()中编写  
DWORD oldMode = SetKMode(TRUE);  
volatile int *piTemp = (volatile int*)(0x20000000+0x84000000-0x00019000); ///或者(0x84000000-0x00019000)  
*piTemp = 12345;  
在用户模式下，如果不调用SetKMode(TRUE)，那么执行*piTemp = 12345一定会弹出对话框，提示地址访问非法，如果调用SetKMode(TRUE)就不会提示地址访问非法，而且在OnButton2()中仍然能得到12345这个值。  
通过这两个例子我相信读者能够完全了解两种模式的区别了。  
4、WINCE提供了两个函数SetKMode和SetProcPermissions，其中SetKMode能够把调用线程切换到内核模式，还可以切换回用户模式。SetProcPermissions + GetCurrentPermissions添加当前进程访问权限给调用线程，SetProcPermissions (0xFFFFFFFF)能让调用线程访问所有进程空间，但是调用线程仍然处于用户模式。SetKMode和SetProcPermissions函数使得用户模式的特点不那么明晰。  
如上所说一个应用程序的线程可能转移到其它两个进程地址空间中读写数据，而每一个线程在被创建的时候只有访问创建它的进程地址空间的权限，所以驱动程序开发者必须在驱动程序读写数据前调用SetKMode或者SetProcPermissions增加调用此函数的线程访问其它进程空间的权限。如果一个应用程序的线程只转移到一个进程地址空间，一般为设备管理器进程device.exe，这种情况下不必增加线程访问其它进程空间的权限，但如果驱动程序本身创建了一个线程，那还是要调用SetKMode或者SetProcPermissions增加新的线程访问其它进程的权限的，因为驱动程序创建线程时，当前进程为设备管理器，所以新线程只具有访问设备管理器进程空间的权限，而不具备访问应用程序进程空间的权限。  
5、可能一个编写过简单的流驱动的初学者会很疑惑，因为开发一个简单的流驱动程序根本不需要调用这些函数，也没有调用过MapPtrToProcess，那是因为如果标准流驱动接口函数的参数为指针（ReadFile、WriteFile、DeviceIoControl参数都有指针），WINCE内核会自动映射指针包含的地址，但仅此而已，其余任何情况都要求开发者自行处理，比如流接口函数的参数是一个指向结构体的指针PA，而结构体中包括指针PB，PB指针就必须在流接口函数中映射，映射后才能访问，否则就会造成地址访问非法。所以结构体中每个指针都要映射。  
为了让读者能了解其中的原因，我举个例子：  
假设设备管理器被加载到Slot4，应用程序A被加载到Slot 8，A只有一个主线程T，T开始执行，按照WINCE的规定，正获得CPU的进程必须映射到Slot0，那么在执行代码的时候A的所有虚拟地址都被减去一个偏移值，也就是8×0x02000000，A调用DeviceIoControl，传递一个指向一个结构体的指针B，而这个结构体中包含一个指针C，指针C包含的地址假设为0x00030000，当执行DeviceIoControl时WINCE把设备管理器的进程地址空间映射到Slot0，因为放在注册表[HKLM/Drivers/BuiltIn]下的驱动程序是由设备管理器加载的，自然驱动程序的代码段被加载到设备管理器进程空间，但是线程仍然是T，此时T的当前所在进程为设备管理器（CurrentProcess），A变成了T的调用者进程（CallerProcess），T自动具有了访问调用者进程空间的权限。这时访问Slot0中的虚拟地址其实质就是访问设备管理器的进程地址空间，要把地址加上一个偏移值，也就是4×0x02000000，所以DeviceIoControl访问指针C包含的地址时本应该加上8×0x02000000，却加上4×0x02000000，结果地址并不是设备管理器的合法区域，系统就会提示地址访问非法。而如果做了一个映射，指针C包含的地址就会被加一个正确的偏移值，使地址处于A的地址空间Slot 8中，T此时具有访问A进程空间的权限，访问到正确的虚拟地址当然会得到正确的数据了。  
 为什么WINCE目录下的例子用build+sysgen能够编译成EXE文件，而我添加的例子就不能编译呢？
如果这个例子是一个应用程序，那么肯定包括代码文件（.h .c .cpp）和资源文件（.rc和其它资源文件），build工具根据source文件内容把代码文件编译成lib文件，资源文件编译成.res文件，sysgen工具根据makefile文件内容将source文件中列出的需要链接的各个库文件合并成一个EXE文件。所以说关键在于makefile文件，WINCE目录下凡是能够用build+sysgen编译的都在makefile中有如何链接的设置，而我们添加的例子当然没有在makefile中找到如何链接的设置，nmake工具就会提示不知道如何创建。  
pcienum.exe干什么用的？
如果你要开发某一个PCI设备的驱动程序，首先要知道这个PCI设备的信息（如VendorID、DeviceID、BaseClass、SubClass）和PCI总线的信息。运行这个pcienum.exe就能得到相关信息。pcienum.exe提供了源码，位置/Public/Common/Oak/Drivers/Ceddk/Test/Pcienum。  
wince下如何让操作系统进入待机模式？又如何把它激活？
通过注册表就可以设置，前提是你的驱动和硬件都支持。注册表项参见标题为“GWES Suspend Time-outs”的帮助文档。  
[HKEY_LOCAL_MACHINE/System/CurrentControlSet/Control/Power]  
    "BattPowerOff"=dword:300  
    "ExtPowerOff"=dword:0  
    "WakeupPowerOff"=dword:60  
    "ScreenPowerOff"=dword:0  
现有一个GPRS模块，如何通过GPRS连接到Internet？
1、先在内核中加入WAN下面的几个组件，如RAS/PPP、TAPI。WINCE采用unimodem驱动，所以不必担心没有Modem驱动的支持。  
2、WINCE启动后新建一个拨号连接，比如名称叫“gprs1”，输入用户名、密码、电话号码。电话号码不同，所采用的模式不一样，例如“*99#”是GPRS模式，“17201”是普通的数据模式，速度差很多，价钱也差很多。  
3、开始连接，连接过程会在对话框中显示，直到显示“连接成功”。  
4、打开浏览器或者自己开发的通讯软件测试网络连接情况。  
5、关闭连接。  
6、保存[HKEY_CURRENT_USER/Comm/RasBook/gprs1]下的所有数据，添加到project.reg中，重新编译后内核中就有了一个拨号连接“gprs1”。  
7、调用RAS函数可以修改拨号连接“gprs1”的参数，如用户名、密码、电话号码，但是不能修改硬件设置，如波特率、串口、数据位、停止位等。RAS函数还能够拨号、挂断。为了修改波特率可以多保存几个拨号连接，也可以直接调用TAPI开发拨号软件，另外WINCE自带的拨号连接是有源码的，位置在/PUBLIC/COMMON/OAK/DRIVERS/NETSAMP/CONNMC。  
采用基于HIVE的注册表如何删除用户保存在注册表中的数据，恢复到出厂时的注册表？
用户修改的数据保存在user.hv文件中，直接删除一定失败，所以不能通过删除文件实现恢复出厂设置。微软考虑到了这个问题，在WINCE启动过程中filesys.exe加载注册表时会调用OEMIoControl函数并传递一个IOCTL，这个IOCTL在pkfuncs.h中定义如下：  
#define  IOCTL_HAL_GET_HIVE_CLEAN_FLAG  CTL_CODE(FILE_DEVICE_HAL, 49, METHOD_BUFFERED, FILE_ANY_ACCESS) 
filesys.exe会分别传递参数HIVECLEANFLAG_SYSTEM和HIVECLEANFLAG_USERS，如果返回值为TRUE那么filesys.exe清除原来的注册表文件，如果返回值为FALSE那么filesys.exe保留原来的注册表文件。默认WINCE并没有实现这个IOCTL，所以OEM要删除注册表文件就必须先编写这个IOCTL代码。代码的例子可参考标题为“IOCTL_HAL_GET_HIVE_CLEAN_FLAG”的帮助文档。另外必须在ioctl.h和ioctl.c两个文件中编写该代码。在ioctl.c文件中找到const OAL_IOCTL_HANDLER g_oalIoCtlTable[]，添加IOCTL和对应的处理函数。要进一步了解这个全局数组，参见标题为“IOCTL Library”的帮助文档。  
如何在不删除必要组件的前提下减小内核文件长度？
要减小内核文件长度首先要在使用PB的定制内核向导中选择自定义，也就是说对于每个组件都由自己来选择，而不是选择PB的标准配置。但减小内核文件长度最有效最直接的办法是缩小字体，尤其对于东亚字体，采用字体压缩技术并且选择合理的字库文件将明显缩小文件长度。  
1、在定制内核时选择AGFA AC3 Font Compression组件。SYSGEN变量为SYSGEN_AGFA_FONT。  
2、参考标题为“East Asian Font Versions”的帮助文档，从中选择你需要的字库文件加到内核中，从文档可以看出加AC3压缩比不加压缩在文件长度方面差距很大。  
如何得到WAV文件播放的总时间？
1、直接读取wav文件头信息，从文件起始地址偏移28个字节长度为4个字节保存的是每秒钟播放的字节数，从文件起始地址偏移40个字节长度为4个字节保存的是声音数据的总的字节数，相除就是播放时间。  
2、调用IGraphBuilder::RenderFile打开一个wav文件，然后通过IGraphBuilder得到IMediaSeeking指针，再调用IMediaSeeking::GetDuration得到总的时间（结果要除以10000000），IMediaSeeking::GetCurrentPosition得到当前播放时间。  
如何在Dialog-Based程序中加入menubar？
先调用CommandBar_Create再调用CommandBar_InsertMenubar。  
请问MultiByteToWideChar与_T、L、TEXT的区别？
MultiByteToWideChar函数转换的对象可以是常量也可以是变量。其它只能转换常量。_T和TEXT会根据当前系统是否定义_UNICODE宏来决定是否转换，而L就是转换成宽字符，当然也包括其他类型常量的转换。  
在用UBS线缆通过ActiveSync同步有效的情况下，如何插上USB线缆后WINCE自动与PC同步？
1、新建一个拨号连接，假设名称为“usb1”，选择连接类型为“直接连接”，并在连接设备里选择通过USB线缆连接。  
2、将注册表[HKEY_CURRENT_USER/Comm/RasBook/usb1]下的数据添加到project.reg或者platform.reg中。  
3、在[HKEY_CURRENT_USER/ControlPanel/Comm]下添加如下：  
"AutoCnct"=dword:1  ///直接连接  
"Cnct"="usb1"       ///连接名称  
4、重新编译内核。为了节省编译时间也可以在内核工程下搜索*.reg文件，将2、3步骤中的注册表数据添加其中，然后直接make image。  
如何通过进程句柄来获得该进程的主窗口句柄？
好像没有API能够通过进程句柄直接获得主窗口的句柄，因为并非每个应用程序都带UI。但是可以反过来，先枚举当前系统所有主窗口，然后根据每个窗口的句柄调用GetWindowThreadProcessId函数得到进程的ID，再调用OpenProcess得到进程句柄，与现有的进程句柄比较。  
 我做的显示驱动DLL已经编译成功了，但是在加载显示驱动的过程中弹出话框，提示如下：  
unhandled exception  in  gwes.exe  (0xc0000005 access violation)  
提示的错误——地址访问非法，表明你的驱动程序代码并没有在读写数据前添加SetKMode(TRUE)或者SetProcPermissions(0xFFFFFFFF)函数让线程能够访问任何进程的地址空间。你可以调用 IsBadReadPtr和IsBadWritePtr函数检测地址是否能够合法访问。编写和gwes有关的驱动程序应该首先调用SetKMode(TRUE)或者SetProcPermissions(0xFFFFFFFF)函数，这是一个好习惯。  
 请问在嵌入式系统中如何设置GPRS拔号用的APN？
对一个拨号连接比如“我的连接”单击鼠标右键，在弹出的菜单中选择“属性”，然后单击“配置”—“拨号选项”，在“附加设置”中添加AT命令如“+cgdcont=1,"ip","cmnet"”。“cmnet”位置即为APN。  
WINCE的IP Phone功能如何？
WINCE的voip需要c-s-c结构，既需要服务器的中转，而skype采用第三代p2p技术就不需要中转，但是在gprs下也做不到语音流畅。skype有pocket pc版本，但是无线方面需要wlan或者cdma。  
三星ARM平台如何定义自己的中断ID？
以S3C2410为例，在oalintr.h文件中定义中断ID，也称SYSINTR，例如 #define SYSINTR_MYINT  (SYSINTR_FIRMWARE+20)，最大值不能超过SYSINTR_FIRMWARE+23。然后在armint.c文件中找到OEMInterruptHandler函数，用if (IntPendVal == INTSRC_XXX) 判断当前发生的中断源号，然后返回SYSINTR_MYINT。内核分别调用OEMInterruptDisable（禁止当前中断）、OEMInterruptDone（中断处理结束）、OEMInterruptEnable（当前中断有效）三个函数，参数都为中断ID，在这三个函数中用 case SYSINTR_MYINT判断当前要处理的中断。  
如何开发软件从PC端复制文件到基于WINCE的设备？
调用RAPI（Remote Application Programming Interface）函数，此函数集由桌面计算机调用，由基于WINCE的设备执行。一旦连接上就可以在桌面计算机端调用RAPI。通过注册表还可以限制RAPI能够访问目录的范围。具体参考RAPI和RDP（远程桌面协议）。  
请问如何对NandFlash分区、格式化？
你看看WINCE420/PUBLIC/COMMON/OAK/DRIVERS/ETHDBG/BOOTPART/bootpart.cpp，在Eboot中先要调用BP_LowLevelFormat(  
DWORD dwStartBlock, DWORD dwNumBlocks, DWORD dwFlags)再flash的一个区域建立空的MBR,然后连续两次调用BP_OpenPartition(DWORD dwStartSector, DWORD dwNumSectors, DWORD dwPartType, BOOL fActive, DWORD dwCreationFlags)函数来建立BINFS和FAT分区。建好后，将nk.bin烧入binfs分区中。  
要做个弹出对话框具有 always on top 属性，如何实现？
调用SetWindowPos(.. , HWND_TOPMOST, ...., SWP_NOACTIVATE)。  
s3c2410＋WINCE下网络PING一会就断，如何解决？
原因在于中断处理程序把已经产生的中断标志清除掉了，这样就丢失一次中断。因为原驱动里配置中断为上升沿触发，一次中断丢失就导致不会再产生中断信号跳变，因为只有在中断服务中读取了cs8900的 Interrupt status queue寄存器后，才会产生下一次中断！解决办法：  
1、在cfw.c文件中全局定义BOOL Inited = FALSE  
2、修改OEMInterruptEnable()中case SYSINTR_ETHER: 下面的语句为：  
if(Inited == FALSE)   
{   
s2410IOP->rEINTPEND = 0x200;   
s2410INT->rSRCPND = BIT_EINT8_23;   
if (s2410INT->rINTPND & BIT_EINT8_23)   
s2410INT->rINTPND = BIT_EINT8_23;   
Inited = TRUE;   
}   
s2410IOP->rEINTMASK &= ~0x200;   
s2410INT->rINTMSK &= ~BIT_EINT8_23;   
break；   
注：本解决办法转载于http://stoned.blogchina.com/stoned/3083045.html，非我本人研究成果。  
已经搜索到文件，如何用CListBox以图标形式显示出来？
CListCtrl        ListCtrl;  
CImageList      ImageList;  
ImageList.Create(IDB_BITMAP, 48, 2, RGB(0,0,0));  
ListCtrl.SetImageList(&ImageList, LVSIL_NORMAL);  
ListCtrl.InsertItem(iListIndex, strItem, 1);   
如何改变控制面板中电源属性对话框的尺寸?
1、需要修改对话框的尺寸是因为对话框是以资源方式加载的，不会根据当前系统显示分辨率而自我调节尺寸。  
2、安装WINCE后有一些组件（feature）的资源文件*.res就已经有了，如果你不改变，那么build内核的时候PB只是把这些.res复制到工程目录下，然后与*.obj合并成EXE、DLL、CPL。所以修改了.rc文件里面的对话框尺寸后要重新编译.rc文件为.res文件，然后再覆盖原来WINCE自带的.res文件。  
3、改变对话框尺寸有两种办法：一种方法是更改系统字体字号，系统字体的字号变化会影响对话框的尺寸，但是缺点是所有系统字体有关的UI都会改变。另一种是在.rc文件中调整对话框尺寸，然后编译成.res文件，再将.res复制到对应的语言目录里，比如目录名为0804（中文），再执行Rebuild命令重新编译内核，或者执行sysgen＋build。在研究中我发现.res文件虽然能够直接用EVC打开、修改、保存，但是和其它Obj链接成EXE、DLL、CPL后并不能运行，所以还是建议读者用CE自带的rc工具编译最好。读者可在PB的命令行中键入“rc /?”了解rc.exe工具的用途和参数。  
使用EVC build之后连接模拟器的时候，提示download file等了一会又出现download failed？
一般这样的问题从下面几个步骤解决：  
1、如果之前能启动模拟器而现在不能，那么先clean然后重启计算机再build。  
2、如果开发的主机为WINXP+SP2，可能存在与EVC模拟器不兼容的情况，检查C:/boot.ini，将/noexecute=optin改为/execute。  
3、检查你的模拟器是否能运行，假设你正用的SDK名称为MYSDK，单击菜单tools—configure platform manager，选择MYSDK—MYSDK emulator，再单击properties—test，看看模拟器是否能够启动，如果能启动那问题就不大。  
4、单击菜单build—update remote output files，看看模拟器是否能够启动。  
5、如果上述办法均不行，关闭EVC然后重新建立一个新的工程，编译，看看模拟器是否能够启动，如果能启动说明原来工程出了问题，最好恢复原工程的备份。  
如何设置能够自动拨号、禁止自动拨号？
在[HKEY_LOCAL_MACHINE/Comm/Autodial]下是自动拨号的注册表设置。  
Enabled=DWORD:1          ///是否能够自动拨号  
FailRetryWaitMS=DWORD    ///如果失败再次拨号的等待时间  
RasEntryName1= REG_SZ     ///自动拨号采用的拨号连接名称  
更多细节请参考标题为“Auto Dial Registry Settings”的帮助文档。
