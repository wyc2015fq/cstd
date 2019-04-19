# WinCE6.0虚拟串口的若干关键问题 - xqhrs232的专栏 - CSDN博客
2013年02月21日 00:25:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：833
原文地址::[http://jazka.blog.51cto.com/809003/601409](http://jazka.blog.51cto.com/809003/601409)
在产品开发中，总会出现串口不够用的情况，尤其是GPS导航系列产品中。最近单位一直在做车载终端的开发，也遇到了这个问题，导航软件肯定要占用一个串口读取GPS的数据，而我们自己又想获取GPS数据实现自己的一些功能，但是导航软件不会给我们提供他们的接口的，所以只能通过虚拟串口的方案来实现。（其实网络上出现的vspd.dll的实现也可以，见我的文章[WinCE利用vspd.dll实现虚拟串口](http://jazka.blog.51cto.com/809003/595554)，但是测试发现有流量限制，要么你自己破解，要么花钱买个注册码）
早在2009年，norains老大就已经对这种方案进行了设计和实现，网络上出现的虚拟串口实现也都在此基础上实现，包括后来googleman的数据丢失改进等等。惭愧啊，2011年了还在研究别人的东西，可能自己是初级入门选手吧，感觉norains的博文只给出了主要的方案，里面的一些细节问题，本人在这里补充一下。
1、在COM端口列表中显示所虚拟的端口号
当把某一个物理端口虚拟为两个端口以后，肯定希望可以通过串口助手等看到你虚拟的端口号。比如，把COM3虚拟成了COM5和COM6，那么在串口调试助手中就可以看到COM5和COM6。这一目的其实很容易实现，只需要添加注册表就可以了。如下所示：
;--------------- VirtualSerial(virtual serial port 5)---------------------------------- 
[HKEY_LOCAL_MACHINE\Drivers\BuiltIn\VSP0] 
  "DeviceArrayIndex"=dword:0 
  "Priority"=dword:0 
  "PortIsRO"=dword:0 
  "SplitPort"=dword:3 
  "Index"=dword:5 
  "Prefix"="COM" 
  "Dll"="virtualserial.dll" 
  "DeviceType"=dword:0 
  "Tsp"="unimodem.dll" 
  "FriendlyName"="Virtual port COM5" 
  "DevConfig"=hex: 10,00, 00,00, 05,00,00,00, 10,01,00,00, 00,4B,00,00, 00,00, 08, 00, 00, 00,00,00,00 
;-------------------------------------------------------------------------- 
;--------------- VirtualSerial(virtual serial port 6)---------------------------------- 
[HKEY_LOCAL_MACHINE\Drivers\BuiltIn\VSP1] 
  "DeviceArrayIndex"=dword:0 
  "Priority"=dword:0 
  "PortIsRO"=dword:0 
  "SplitPort"=dword:3 
  "Index"=dword:3 
  "Prefix"="COM" 
  "Dll"="virtualserial.dll" 
  "DeviceType"=dword:0 
  "Tsp"="unimodem.dll" 
  "FriendlyName"="Virtual port COM6" 
  "DevConfig"=hex: 10,00, 00,00, 05,00,00,00, 10,01,00,00, 00,4B,00,00, 00,00, 08, 00, 00, 00,00,00,00 
;--------------------------------------------------------------------------
2、注册表中子键标识物理串口号
为了能够灵活的配置要虚拟的物理串口，在注册表中添加一个子键来标识。如上面注册表中的SplitPort子键的值，表示的就是两个虚拟串口虚拟的是物理串口3。
3、两个虚拟串口COM_Open一个物理串口
当应用程序调用CreateFile打开一个串口的时候，串口驱动调用的便是COM_Open函数。这里虚拟串口驱动的COM_Open函数其实也是通过CreateFile函数来实现的，没有操作实际的硬件。但是不同的是，通过一个全局变量g_uiOpenCount来表示已经打开该物理串口的数量，当数量大于等于1时，不在调用CreateFile，而是直接返回之前CreateFile返回的全局句柄。这是norains老大的原创思路。
4、如何检测串口有数据
这个也采用的norains老大的想法，也是虚拟串口中很关键的一步。在COM_Open之后，就启动一个监视线程，在线程中循环的调用WaitCommEvent函数，通过WaitCommEvent进行检测，当返回的状态中具备EV_RXCHAR时，说明可以进行数据读取了，记得两点，要马上读取数据，防止再次来数据覆盖掉物理串口的上次数据，二是要读取时的Buffer要大于READ_BUFFER_LENGTH，否则读到的数据时残缺的。
5、为何驱动在WaitCommEvent中阻塞
这一问题在norains博文中没有特别说明，但是我却遇到了这个问题。其实WaitCommEvent函数确实是阻塞的，但是如果有数据来还是阻塞就有问题了，问题出在了没有进行SetCommEvent操作，SetCommEvent告诉串口要监听哪些事件，这里只要设置EV_RXCHAR就可以了。该函数要放到监听线程的WaitCommEvent的前面，这样每次循环WaitCommEvent就知道要监听什么事件了，不然就会发生阻塞。
6、读取数据之后如何分发给两个虚拟串口
在读取数据之后，就应该进行数据的分发。norains的思路是两个虚拟串口共享一个缓冲区，读取数据后就将数据放入了那个共享Buffer中，在短时间内让两个虚拟串口轮询读取，都读取后删除相应的缓冲区空间。但是这有一个问题就是两个虚拟串口的读取操作时不可控的，什么时候发生不知道，那样的话轮询读取的时间很难定，大了两个虚拟串口都会丢失数据，小了至少有一个很容易丢数据。所以我的想法是开辟两个同样大小的缓冲区Buffer1和Buffer2，分别对应两个虚拟串口。这样两个虚拟串口的读取就互相没有影响了，确定就是有点浪费内存，但是我感觉很值。
7、数据丢失问题
针对数据丢失的问题，googleman提出来一些方法，可以参看他的博文。但如果采用我上面提出的双缓冲区模式的话，数据丢失的问题不会出现在两个虚拟串口互相影响的情况了。那么可能出现数据丢失的情况是读取物理串口数据的速度赶不上物理串口数据的更新速度。这时从两方面解决：增大读取串口数据的Buffer和减小在监视线程中Sleep的时间。
8、在应用程序读取数据时如何判断是哪个虚拟串口要读数据
这个问题估计是初级选手都会迷惑的一个问题吧。其实读懂了串口驱动，你就会明白了。解决的关键集中在了那几个接口函数的传入参数上面。下面分析详细分析一下。这里假设有两个驱动，在注册表中的键值分别为VSP0和VSP1
COM_Init函数的参数DWORD dwContext是驱动在HKEY_LOCAL_MACHINE\Driver\Active下面添加的子键，该子键的内容下有一个子键key，键值等于Drivers\BuiltIn\VSP0，正是通过该参数提供的键值变可以判断当前初始化的是哪个COM端口。在COM_Init函数结尾，返回包含物理串口号和虚拟串口号的结构体。
COM_Open函数的参数DWORD dwData其实就是从COM_Init函数返回的那个结构体，这样就可以CreateFile相应的物理串口，获得共同的串口句柄。最后记得将同样的结构体返回。
COM_Read函数的参数DWORD dwHandle也是从COM_Open函数返回的那个结构体，这样就可以判断出是哪个虚拟串口要读数据，从而从相应的buffer中读取数据就可以了。
   上面有什么不对之处，还望高手指出，也希望有什么不全的地方，大家一起补充。本人已经实现了该虚拟串口，也进行了一定测试，没有发现什么问题了。如果需要，可以留下email索取源码，希望大家一起测试改进。
本文出自 “[飞雪待剑](http://jazka.blog.51cto.com/)” 博客，请务必保留此出处[http://jazka.blog.51cto.com/809003/601409](http://jazka.blog.51cto.com/809003/601409)
