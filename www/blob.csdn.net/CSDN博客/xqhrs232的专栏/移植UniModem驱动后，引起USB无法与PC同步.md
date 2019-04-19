# 移植UniModem驱动后，引起USB无法与PC同步 - xqhrs232的专栏 - CSDN博客
2013年02月05日 10:27:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：838
个人分类：[3G/RIL/TAPI/网络技术](https://blog.csdn.net/xqhrs232/article/category/906928)
原文地址::[http://www.netfoucs.com/brantyou/article/details/8178520](http://www.netfoucs.com/brantyou/article/details/8178520)
前段时间搞GPRS，为了实现同时上网和通话短信，移植了MUX驱动和UniModem驱动。移植完后出现了一个状况，USB无法与PC同步，CE上提示“端口不可用，请关闭其它正在使用的程序”之类的。当时一直没留意这个玩意，今天抽空看了下，发现这个问题既然是由移植后的UniModem驱动引起的。
这个无法同步有点奇怪，只要没有接上GPRS模块，MUX初始化失败后就会导致USB无法同步。如果接上GPRS模块，MUX可以初始化成功后，是可以正常使用USB同步的。
同步使用的是USB CABLE这个端口，跟踪发现这个具体配置USB同步的在common.reg中有配置：
[view plain](http://www.netfoucs.com/brantyou/article/details/8178520#)[copy
 to clipboard](http://www.netfoucs.com/brantyou/article/details/8178520#)[print](http://www.netfoucs.com/brantyou/article/details/8178520#)[?](http://www.netfoucs.com/brantyou/article/details/8178520#)
- [HKEY_LOCAL_MACHINE\Drivers\USB\FunctionDrivers\Serial_Class]    "Dll"="serialusbfn.dll""DeviceName"="USBFNS1:""Prefix"="COM""DeviceArrayIndex"=dword:0    "RxBufferSize"=dword:4000    "IClass"="{CC5195AC-BA49-48a0-BE17-DF6D1B0173DD}" ; idVendor must be changed. 045E belongs to Microsoft and is only to be used for ; prototype devices in your labs. Visit http://www.usb.org to obtain a vendor id.    "idVendor"=dword:045E    "Manufacturer"="Generic Manufacturer (PROTOTYPE--Remember to change idVendor)"    "idProduct"=dword:00ce    "Product"="Generic Serial (PROTOTYPE--Remember to change idVendor)"    "bcdDevice"=dword:0    "Tsp"="Unimodem.dll"     "DeviceType"=dword:0  [HKEY_LOCAL_MACHINE\Drivers\USB\FunctionDrivers\USBSER_Class]    "Dll"="serialusbfn.dll"    "DeviceName"="USBFNS2:"    "Prefix"="COM"    "DeviceArrayIndex"=dword:1    "RxBufferSize"=dword:4000    "IClass"=multi_sz:"{CC5195AC-BA49-48a0-BE17-DF6D1B0173DD}","{C375C787-B721-4b8e-B67F-A112D5C0A404}" ; idVendor must be changed. 045E belongs to Microsoft and is only to be used for ; prototype devices in your labs. Visit http://www.usb.org to obtain a vendor id.    "idVendor"=dword:045E    "Manufacturer"="Generic Manufacturer (PROTOTYPE--Remember to change idVendor)"    "idProduct"=dword:0079    "Product"="Generic Serial (PROTOTYPE--Remember to change idVendor)"    "bcdDevice"=dword:90    "Tsp"="Unimodem.dll"     "DeviceType"=dword:0
在这两个键中有引用到UniModem.dll。
这样一看，就大概明白应该是当时移植UniModem驱动，把其中两次打开同一个串口的修改成了打开两个串口导致的。
现有一解决办法，那就是同步的时候使用原来的Unimodem.dll，而GPRS拨号上网那个就使用移植后的Unimodem.dll。
经过测试，这个办法可行，一切正常。USB可以正常同步了。
修改方法如下：
1、把原来的unimodem.dll放到一个驱动的目录下,并重命名为unimodem1.dll（名字随你起）
2、然后在common.bib中引用这个unimodem1.dll
3、在common.reg中把上面贴出的这两段注册表信息中的"Tsp"="Unimodem.dll"修改成"Tsp"="unimodem1.dll"
4、上面3步都完成后，则进行sysgen，之后就可以了
但是，这样修改有一个缺点：
如果我在同一个工程中即使用GPRS模块又使用3G模块，3G模块使用的是自带的unimodem驱动，那么这个3G就有可能无法进行拨号上网。
不知道谁有没有更好的办法，欢迎来讨论~~
PS: Unimodem驱动中如果不修改成打开两个串口，而是采用原来的打开同一个串口两次，这样会有问题，导致无法正常使用。打开的串口都是MUX那里虚拟出来的串口，我这里虚拟出来三个串口，一个用于通话、短信的AT指令，一个用于拨号上网，剩下的一个用于第二次打开串口这里。
