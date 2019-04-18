# PC访问profibus协议设备的方式 - 深之JohnChen的专栏 - CSDN博客

2017年10月05日 23:46:38[byxdaz](https://me.csdn.net/byxdaz)阅读数：1151


PC访问profibus协议设备方式包括：

1、购买Profibus网卡CP5611，pc上安装CP5611驱动，CP5611驱动程序提供了User_DP.h和DPnlib.lib文件，利用此文件访问profibus协议设备数据。

2、串口或网络上走PROFIBUS协议，你需要花费大量的时间来了解这个协议类容的本身。所以 开发周期会很长。

3、使用OPC的通信方式来做，用VB/VC/C#编写OPC客户机的程序简单多了！至于OPC服务器，你可以选择第三方的OPC服务器也可以选择SIMATICNET OPC。simaticnet是西门子的通信软件，它包含cp5613、cp5614和cp1613、cp1616等通信卡的驱动程序，step7集成的“设置pg/pc接口”工具不支持上述通信卡。将这类cp卡插入计算机，在安装simaticnet时，将自动地为它们安装驱动程序。

4、组态王软件。

5、通过profibus转modbus协议模块（比如：西门子以太网通讯处理器unnet-s7），模块内提供了modbus服务，通过串口编程就可以实现数据访问。

