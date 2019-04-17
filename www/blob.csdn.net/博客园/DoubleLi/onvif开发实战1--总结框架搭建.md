# onvif开发实战1--总结框架搭建 - DoubleLi - 博客园






## Gsoap及开发框架生成：

一：gsoap下载和编译



1.下载Gsoap：地址：[http://sourceforge.net/projects/gsoap2/files/gSOAP/](http://sourceforge.net/projects/gsoap2/files/gSOAP/)

2.安装：  ./configure --prefix=你的安装目录 

                make

                make install

二：在线生成onvif.h(笔者试了离线生成，各种报错，所以劝各位还是在线生成)

wsdl2h -o onvif.h -c -s -t ./typemap.dat http://www.onvif.org/onvif/ver10/device/wsdl/devicemgmt.wsdl http://www.onvif.org/onvif/ver10/media/wsdl/media.wsdl http://www.onvif.org/onvif/ver10/event/wsdl/event.wsdl http://www.onvif.org/onvif/ver10/display.wsdl http://www.onvif.org/onvif/ver10/deviceio.wsdl http://www.onvif.org/onvif/ver20/imaging/wsdl/imaging.wsdl http://www.onvif.org/onvif/ver20/ptz/wsdl/ptz.wsdl http://www.onvif.org/onvif/ver10/receiver.wsdl  http://www.onvif.org/onvif/ver10/recording.wsdl http://www.onvif.org/onvif/ver10/search.wsdl http://www.onvif.org/onvif/ver10/network/wsdl/remotediscovery.wsdl http://www.onvif.org/onvif/ver10/replay.wsdl http://www.onvif.org/onvif/ver20/analytics/wsdl/analytics.wsdl http://www.onvif.org/onvif/ver10/analyticsdevice.wsdl http://www.onvif.org/ver10/actionengine.wsdl http://www.onvif.org/ver10/pacs/accesscontrol.wsdl http://www.onvif.org/ver10/pacs/doorcontrol.wsdl 

注：离线报错如下


1）使用wsdl2h出现    Reading file 'accesscontrol.wsdl'... 
Cannot open 'types.xsd' to retrieve schema

我这个问题是因为把onvif的 accesscontrol.wsdl 这个文件下载到本地，然后直接 wsdl2h accesscontrol.wsdl -o accesscontrol.h  出现的，我换一种做法，就是直接在wsdl2h 后面加上wsdl文件所在的URL，wsdl2h http://www.onvif.org/ver10/pacs/accesscontrol.wsdl -o accesscontrol.h 这个问题就没有啦。具体什么原因我也不知道是为什么。



如果直接生成对应C的库文件会发生重复定义错误，可以修改该文件。

wsa5.h(288): **ERROR**: remote method name clash: struct/class 'SOAP_ENV__Fault' already declared at line 274

打开文件gsoap_2.8.16/gsoap-2.8/gsoap/import/ wsa5.h



将277行int SOAP_ENV__Fault修改为int SOAP_ENV__Fault_alex



笔者没有使用这种方法，是将这个结构体直接注释的方式，最后的结果是，都可以使用。



同时上一步生成的onvif.h文件中没有打开wsse.h， 导致最后生成代码中SOAP_ENV__Header 结构体中缺少定义 wsse__Security数据段，无法进行鉴权命令。

即：添加对openssl的支持，在上一步生成的onvif.h中添加（可选）











- #import "wsse.h"  






随后使用命令生成：



- soapcpp2  -c onvif.h -x -I/root/Tools/Gsoap/gsoap-2.8/gsoap/import -I/root/Tools/Gsoap/gsoap-2.8/gsoap/ -I/root/Tools/Gsoap/gsoap-2.8/gsoap/custom -I/root/Tools/Gsoap/gsoap-2.8/gsoap/extras -I/root/Tools/Gsoap/gsoap-2.8/gsoap/plugin   
- 




 通过上面的两个步骤的话，onvif的基本代码框架就已经有了，里面可能有就开发而已不需要的文件，可以直接删除掉,还有一些文件需要从gsoap里面拷贝过来
        1.修改wsdd.nsmap文件名为wsdd.h，删掉其他所有的nsmap文件(rm *.nsmap)，命令空间文件文件都差不多，保留一个即可，需要的时候包含此文件就好
        2.直接删除soapClientLib.c和soapServerLib.c文件，在开发过程中没有实际用处
        3.将gsoap-2.8.14/gsoap-2.8/gsoap下的stdsoap2.h和stdsoap2.c文件和gsoap-2.8.14/gsoap-2.8/gsoap/custom下的duration.cw文件都拷贝到当前目录下
        4.如果是作客户端的开发的话，则删掉soapServer.c文件，不然的话，里面调用的很多接口只是写了申明了函数原型，而没有实现，需要服务端开发来实现的，剩余客户端开发代码如下（服务端的话，多一个soapServer.c文件）
剩下有效的源码文件如下：











- -rwxr-xr-x 1      6560 2013-12-25 10:44 duration.c*  
- -rw-r--r-- 1   2410694 2013-12-25 10:07 onvif.h  
- -rw-r--r-- 1  11308204 2013-12-25 10:14 soapC.c  
- -rw-r--r-- 1    713192 2013-12-25 10:14 soapClient.c  
- -rw-r--r-- 1   4882158 2013-12-25 10:14 soapH.h  
- -rw-r--r-- 1    855676 2013-12-25 10:14 soapStub.h  
- -rwxr-xr-x 1    477042 2013-12-25 10:31 stdsoap2.c*  
- -rwxr-xr-x 1     91109 2013-12-25 10:31 stdsoap2.h*  
- -rw-r--r-- 1      3634 2013-12-25 10:14 wsdd.h   




这样基本的ONVIF代码框架就已经生成完成了，如果想看一个简单的代码例子的话，可以参考这个例程










