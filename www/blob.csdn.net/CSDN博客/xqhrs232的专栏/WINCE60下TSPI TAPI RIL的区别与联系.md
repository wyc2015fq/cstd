# WINCE60下TSPI/TAPI/RIL的区别与联系 - xqhrs232的专栏 - CSDN博客
2013年01月22日 22:27:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：640
个人分类：[3G/RIL/TAPI/网络技术](https://blog.csdn.net/xqhrs232/article/category/906928)

windows下安装sim100 modem----**SIM100 拨号上网设置----[http://www.docin.com/p-96348395.html](http://www.docin.com/p-96348395.html)**
一直对WINCE60下的TSPI/TAPI/RIL，还有什么Line Device/Phone Device这几个东西不清楚.今天看了下WINCE60的帮助文件，是乎有点明白了，记录如下:
WINCE60的帮助文件有如下一段话，值得深思
Windows Embedded CE ships with one TSP, the Unimodem service provider, which supports AT-command-based modems. Windows Embedded CE supports installable service providers, which enables independent software vendors (ISVs), original equipment manufacturers
 (OEMs), and independent hardware vendors (IHVs) to add additional TSPs under TAPI — for example VoIP, such as H323 and session initiation protocol (SIP), Integrated Services Digital Network (ISDN), cell phones, and so on.
1。WINCE下实现的 Unimodem驱动不支持语音MODEM
The Windows Embedded CE implementation of Unimodem does not support voice modems, but you can create a customized TSP that supports them.
2。WINCE下实现的 Unimodem驱动只支持硬件流控制MODEM
The Windows Embedded CE implementation of Unimodem supports hardware flow control modems only. Unimodem traces the carrier detect (CD) signal to determine if a connection exists. For software flow control modems, Unimodem is unable to detect a line drop,
 so the connection must be detected and dropped by an upper-layer application, such as the Point-to-Point Protocol (PPP).
自己的理解如下：
1》TSPI是电话服务接口----每一个电话服务都要进行实现那些标准的接口，TSPI是真正跟底层MODEM打交道的接口
2》TAPI是上层的电话API----电话API的功能需要借助底层的TSP来真正实现
3》VoIP/H323/SIP/ISDN/cell phones也都可以做为一个TSP来为TAPI服务，所以RIL也可以做为一个TSPI来实现
4》 Unimodem service provider，也就是 Unimodem.dll是WINCE60下提供的一个一般性的TSPI接口，其源代码的位置为D:\WINCE600\PRIVATE\WINCEOS\COMM\TAPI\UNIMODEM
5》WINCE60下还提供了一个TSPI的例子实现参考，其源代码的位置为D:\WINCE600\PUBLIC\COMMON\OAK\DRIVERS\SAMPTSPI
6》不管是 Unimodem.dll还是RIL实现的celltsp.dll，都只是做为一个TSPI接口来实现的，要打电话最好还是用TAPI来做，并且这两者的底层都是通过打开串口用AT命令来对
实际的MODEM硬件进行操作的。只是为了增加软件的可靠性增加了很多的软件技巧与实现，具体可以参考他们的代码
   A。RIL 的MDD层代码----D:\WINCE600\PUBLIC\CELLCORE
   B。RIL的PDD层代码（例子代码，不同的MODEM有不同的实现）----D:\WINCE600\PLATFORM\DEVICEEMULATOR\SRC\DRIVERS\RILPDD
7》WINDOWS下也有TAPI版本，所以很多需要在WINCE下调试的MODEM功能可以先在PC上进行实验与尝试，比如MODEM的通信与数据的收发功能都可以先在WINDOWS下
走通再转移到WINCE上，可以估计会更好弄一点，至少学习曲线不会那样陡峭！
8》The following table shows the Catalog items for Cellcore.
|Cellular Telephony API (TAPI) Service Provider|SYSGEN_CELLCORE_CELLTSP|Brings in CellTSP.dll, the cellular TAPI Service Provider|
就是不知道这个celltsp.dll的源代码在什么地方！！！估计MS只提供有LIB库，在D:\WINCE600\PUBLIC\CELLCORE\OAK\LIB\ARMV4I\RETAIL下有celltsp.lib文件，并有1.6MB多。
