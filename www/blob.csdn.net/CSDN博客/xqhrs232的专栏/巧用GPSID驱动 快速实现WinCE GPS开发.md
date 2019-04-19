# 巧用GPSID驱动 快速实现WinCE GPS开发 - xqhrs232的专栏 - CSDN博客
2014年09月01日 16:34:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1237
原文地址::[http://tech.it168.com/a2009/1204/819/000000819899_all.shtml](http://tech.it168.com/a2009/1204/819/000000819899_all.shtml)
相关网帖
1、[关于GPSID（GPS Intermediate Driver
 ）的学习了解](http://blog.csdn.net/lqxandroid2012/article/details/8203517)----[http://blog.csdn.net/lqxandroid2012/article/details/8203517](http://blog.csdn.net/lqxandroid2012/article/details/8203517)
2、[GPS Intermediate Driver](http://blog.csdn.net/lqxandroid2012/article/details/8200141)----[http://blog.csdn.net/hugengyong/article/details/5651177](http://blog.csdn.net/hugengyong/article/details/5651177)
3、wince GPS C++编程完整精间版----[http://wenku.baidu.com/link?url=pD9gol0RqN5-ghTh7V1sKHSPtxXjXaNyo9EPeRRhrao4UvrzA23LTyufSE1i5C3rKwFQt1dsFTg1nheg-wWRi8ryEN-dQQ8Pra-SSUbPafK](http://wenku.baidu.com/link?url=pD9gol0RqN5-ghTh7V1sKHSPtxXjXaNyo9EPeRRhrao4UvrzA23LTyufSE1i5C3rKwFQt1dsFTg1nheg-wWRi8ryEN-dQQ8Pra-SSUbPafK)
4、wince_GPSOpenDevice用法----[http://www.apihome.cn/api/wince/GPSOpenDevice.html](http://www.apihome.cn/api/wince/GPSOpenDevice.html)
5、[WinCE6.0
 GPS驱动和GPSAPI(GPSID driverI)](http://www.cnblogs.com/ProEmbedded/archive/2011/07/15/wince-gps.html)----[http://www.cnblogs.com/ProEmbedded/archive/2011/07/15/2107450.html](http://www.cnblogs.com/ProEmbedded/archive/2011/07/15/2107450.html)
6、wince里关于gps的问题 ----[http://bbs.csdn.net/topics/210082933](http://bbs.csdn.net/topics/210082933)
**【IT168 专稿】**全球导航设备目前正成为嵌入式领域的一个新热点，因此在WinCE平台上开发导航设备也正成为潮流热点。[GPS](http://gps.it168.com/) 系统最初只用于军事、航空、航海等专业领域，但近年来随着技术的发展，体积、功耗和成本都得到极大改善，使到GPS
 系统在民用市场开始显山露水，例如在汽车导航系统和手持设备中均得到了广泛的应用。我公司近来也积极的参与到这股热潮之中，在上周我成功的完成了公司委派给我负责的一个WinCE平台的GPS导航项目。
　　这个基于WinCE平台的GPS导航项目主要包括WinCE内核和GPS[驱动](http://driver.it168.com/)的定制、电子地图制作和嵌入式GIS应用[软件](http://software.it168.com/)的开发等几个方面。其中，我负责WinCE系统软硬件平台的搭建和底层驱动的开发。本文不涉及复杂的GIS地理信息应用程序，主要是结合本次项目的实践，详细介绍GPSID中间驱动层的运行机制和内部原理，以及如何使用GPSID驱动协议和底层接口来开发GPS导航设备。
**　　1. 什么是GPS全球定位系统?**
　　一部完整的GPS导航设备分为软件和硬件两大部份。GPS硬件模块主要包括天线、低噪音放大器(LNA)、射频接收[转换器](http://product.it168.com/list/b/0420_1.shtml)(RF
 Section)、数字基带(Digital Baseband)、微处理器、微处理器周边外设(Processor Peripherals)、输入输出和驱动(I/O and Driver)等几个部分。软件部分则分为[操作系统](http://product.it168.com/list/b/0501_1.shtml)软件、GPS底层驱动软件和应用软件。应用软件主要分为电子监控地图软件、导航软件和功能性软件。其中，基于WinCE系统的GPS底层驱动程序是本次项目中我负责开发的核心。
　　(1)GPS技术的定位原理
　　全球定位系统(Global Positioning System)是美国研制的，历时20年于1994年全面建成，具有在海、陆、空进行全方位实时三维导航与定位能力的新一代卫星导航与定位系统。全球定位系统由三部分构成：①太空卫星部份：由 24 颗卫星所组成，分成六个轨道，绕行地球一周约12 小时。每个卫星均持续发射载有卫星轨道数据及时间的无线电波。②地面监控部份：这是为了追踪及控制上述卫星的运转所设置的地面管制站，主要工作是负责修正与维护每个卫星能保持正常运转，以确保每个卫星都能提供正确的讯息。③GPS用户设备：主要由接收机硬件和处理软件组成。
　　用户通过用户设备接收GPS卫星信号，并实时地计算出接收机所在位置的坐标、移动速度及时间。其中GPS的太空卫星部份、地面监控部份均为美国控制，我们通常所说的GPS设备是指第3 部分:用户设备部分。
　　一般来说，在地面上的GPS 接收器能接收5～12 个卫星信号，而为了获得地面上的定位坐标，GPS 导航至少需要四个卫星信号。三个用来确定GPS 接收器的纬度、经度和海拔高度，第四个则提供同步校正时间。当然所能接收到的卫星数量越多，译码出来的位置就越精确。GPS 是利用卫星和用户GPS接收器进行球面几何运算，一般地参与计算的卫星越多，几何球面的计算越精确，但同时计算的时间也会越长。
　　通常是采用四颗卫星定位的较多，不过一般会是先用三颗卫星快速的计算进行粗定;然后再用四颗卫星来进行精确定位，这是一个较为实用的方法。这四颗卫星形成四个方程式，即四元二次方程，包括未知数为接收机所处位置的三维坐标X、Y、Z 及卫星钟差，经度和纬度的计算都是在GPS 模块里面完成的。
　　GPS 定位计算的数学模型主要分为：最小二乘法(LS)、TDOA 双曲线模型、Fang 算法、Chan 算法、Friedlander 算法、SX 和SI 算法。其中，Chan 算法较为准确。当然，GPS 定位只能得到位置、方向和速度等信息。然后，还要再根据经度和纬度来配合电子地图里面的经度和纬度来确定在地图上的位置，最后才是完成GPS 定位在电子地图上的显示。
　　(2)硬件平台和数据格式的实现
　　本项目的嵌入式GPS导航系统的硬件核心是三星公司的ARM9系列的RISC处理器S3C2410A芯片，该芯片强大的实时处理能力和丰富的外围接口非常适合GPS嵌入式系统的开发，本项目许多的应用功能正是基于该芯片的特点而设计的。但在多次的测量和调试中，我们发现这款产品还有一定的不足，值得进一步的深入去研究。例如：在打开GPS接收模块之后，系统的响应速度将有显着的下降，虽然我们对WinCE内核和编译器进行了适当的优化，但情况并没有显着的改善。
　　问题可能是在硬件平台的处理速度和可调用的资源有限，也有可能是在软件接收上的程序结构不合理，例如嵌套过多导致最终出现了不应有的误差。后来，考虑到硬件成本和综合性能上的比较，我们决定从软件上着手。例如加强WinCE内核优化、动态GPSID驱动响应速度等来纠正响应误差和改善反应性能，实践证明这是很值得研究的一个方向。
　　(3)GPS信号的接收和读取
　　虽然接收机与卫星的位置、伪时延构成的多元多次方程看起来有点复杂。但庆幸的是，程序员不需要做与此相关的工作，它们都被GPS接收机自己处理了。GPS接收模块可以自己接收并处理卫星信号，并完成定位的计算，同时把定位计算结果通过接口以某种电文格式递交给计算机。因此，嵌入式系统只需要简单的打开与GPS模块的连接口，就能不断的收到GPS接收机传送过来的数据了。
　　通常，GPS接收机都会被设计成为标准串口设备，不管是CF接口还是蓝牙接口，都被映射成为串口设备。因此，在程序员的眼中传送的GPS数据都是按一定的数据格式传输的，而和硬件设备的接口类型没有关系，只需如同一般的串口设备来对待即可。GPS接收机的定位数据格式一般会在接收机的使用说明书上明确的标注，通常所见的都是采用NMEA(美国国家航海电子协会)标准。
　　NMEA 0183是一种航海、海运方面有关于数字信号传递的标准，此标准定义了电子信号所需要的传输协议、传输数据时间。这个协议是文本格式的。因此，开发GPS导航设备的要点在于要理解相关的GPS数据格式和协议，以及主机系统和GPS接收机的信息传输接口。
**　　2. 方便快捷的GPSID中间驱动层**
　　(1)WinCE和GPS接收器的[通信](http://tele.it168.com/)方式
　　一般来说，在提供GPS功能的嵌入式设备中都有一个GPS接收器(Receiver)。它是用来接收GPS信号的，同时GPS接收器也是把接收到的卫星信号转换成NMEA Data的设备。因此，GPS接收器一旦启动后，会自动连接卫星接收信号，并通过内含的算法计算出位置等信息，然后以NMEA Data的格式输出。再余下的工作就是WinCE系统如何获取信息的通信过程和应用软件如何实现具体的导航功能了。
　　因此，在WinCE上实现GPS信息传输有3种选择：①是直接使用串口连接GPS接收器来获取信息;②是通过GPS Intermediate Driver中间驱动层来实现信息传输;③是使用第三方类库来实现通信传输。
　　(2)什么是GPS中间驱动程序(GPSID)?
　　GPS传统的编程其实很麻烦，既要同串口进行通信，又要去解析NMEA的语法，同时访问GPS的程序会一直都在运行，这就需要保证这些访问程序是在单独的线程中运行。因此，传统的通过串口传输GPS数据的开发方式是很痛苦的事情。但现在随着WinCE系统新版本的升级，WinCE系统内置了GPS Intermediate Driver。
　　通过它我们能够很方便的取得GPS数据，而且开发基于 GPS定位系统的 WinCE应用程序也变得简单多了。目前，GPS中间驱动程序(GPSID)正成为导航设备开发者手中的利器。
　　GPS Intermediate Drive(中间驱动程序，GPSID)是一个位于应用程序和GPS设备中间层的设备驱动。GPSID是对串口操作进行了封装，因此对于系统层来说它是一个普通的驱动，是一个插在GPS设备与应用程序之间的系统中间件。这样的好处是使开发人员不需要通过串口直接访问 GPS 设备，而是访问 GPSID 提供的 API 函数，然后再由GPSID 去访问 GPS 设备。这样一来，编写出来的 GPS 应用程序就具有设备无关性了，也使到只要应用GPSID就可以用相同的代码去访问几乎所有类型的GPS接收器了。
　　GPSID的API函数主要有两类：一类是gpsapi.dll函数：GPSOpenDevice、GPSCloseDevice、GPSGetPosition、GPSGetDeviceState。另一类是：coredll.dll函数：CreateEvent、CloseHandle、WaitForSingleObject、WaitForMultipleObjects、EventModify。
　　同时，GPSID 还实现了另外的两个好处：一是多个应用程序可以同时访问同一个 GPS 设备，因为访问的是 GPSID，而仅仅把GPSID当作虚机硬件克隆，避免了对串口重叠操作的等候弊端。二是GPSID最大的好处是可以帮助解析NMEA语法，这样可以用非常简单的应用程序接口就得到GPS位置信息了。而且，GPSID 的所有设置信息都保存在注册表中，可以通过修改注册表来更改 GPSID 的设置。
　　例如，可以让 GPSID 从一个包含 NMEA 命令的 .txt 文件读取 GPS 数据，而不需要再从 GPS 设备中读取数据。因此，应用GPSID函数库后既可以访问解析后的GPS数据，也可以访问原始的GPS数据，还可以通过IOCTL函数来控制GPSID的执行以及进行GPS硬件的[电源](http://product.it168.com/list/b/0222_1.shtml)管理。
**3.如何应用GPSID实现WinCE和GPS模块的通信?**
　　(1)查看GPS设备的工作状态
　　在和GPS模块通信之前，我们需要先查询GPS模块的工作状态。例如，可以调用API函数 GPSGetDeviceState来获得GPS设备的工作状态。在这里我建议参考微软在WinCE SDK安装目录下的GPS工程样例。在该Demo的GPS.cs中微软封装了GPS的操作类。比如GPSOpenDevice ()，GPSCloseDevice ()，GPSConnectDevice ()，这些函数都可以很方便的使用。其中，GPSDeviceState.cs是用于取得目前GPS设备的状态信息。GPSPosition.cs是每次GPS数据取得后，都会放入该类。而LocationChangedEventArgs.cs是一旦位置改变，即可取得新的GPSPosition。
　　(2)创建GPS事件对象，获取GPS数据
　　在调用GPSDeviceState.cs取得目前GPS设备的状态信息后，我们下一步的工作是需要启动一个工作线程，以创建GPS事件对象。具体流程是：先通过调用CreateEvent来创建Handles，然后调用GPSOpenDevice API函数将Handle传入得到GPS设备的Handle。得到GPS设备的Handle之后，可再创建一个线程来监听GPS数据及设备状态。然后，再通过调用CreateGpsEventThread方法来创建线程。
　　例如，当GPS设备的状态改变时，就可以调用DeviceStateChanged事件取得当前设备的状态。当GPS设备的位置改变时，就可以调用LocationChanged事件取得当前的坐标。然后，再使用WaitForSingleObject()函数或WaitForMultipleObjects()函数来处理事件通知。最后，通过重复调用DeviceStateChanged事件、LocationChanged事件、WaitForSingleObject()函数或WaitForMultipleObjects()函数，和调用GPSGetPosition()以获取完整的GPS信息。这样一个基本的获取GPS数据的过程就完成了。
　　(3)应用程序的相应处理
　　在获取GPS事件的线程中，不断的通过GPSID中间驱动层就能从串口读入数据，然后分析就能得到定位信息、卫星状态和定位误差，再通过回调函数指针将这些信息送回主线程就可以实现应用程序的相应处理了。最后，可调用GPSCloseDevice()函数关闭设备，以节省GPS接收模块的电源管理。
　　在本次项目的调试中，因为经常会出现整个程序陷入缓慢的等待情况。所以，我们在WinCE 下的GPS编程时，除了采用单独的线程来处理读写串口操作外，还采用了利用通信事件的方法。通信事件就是当发生重要事件时，WinCE可以向应用程序发送的通知，这样就可以大大的提高GPS的响应速度，避免让用户陷入不知所措的长久等待之中。
//===================================================================================================================================
备注::
1》gpsapi封在gpsapi.dll中，所以应用去调gpsapi函数需要先把gpsapi.dll加载起来----hDLL=LoadLibrary(L"\\gpsapi.dll");然后去获取各个gpsapi的函数指针，然后才可以调到
gpsapi里面的函数，gpsapi.dll跟coredll.dll不一样，并不是随系统被直接加载的
2》调用gpsapi函数的时候，这样传进去的数据结果要合乎要求！！！
- **dwVersion**
Version of the GPS Intermediate Driver expected by the application. Must be set before the structure is passed to
**GPSGetPosition**. Must be GPS_VERSION_1.

