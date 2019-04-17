# Onvif开发之Linux下gsoap的使用及移植 - DoubleLi - 博客园






一直以来都是在CSDN上面学习别人的东西，很多次想写点什么但是又无从写起。由于公司项目需要，最近一段时间在研究onvif，在网上找了很多资料，发现资料是非常多，但是很少有比较全的资料，或者资料太多无从下手。我打算从做项目开始，用CSDN博客记录我的项目笔记，同时希望能帮助到需要帮助的人，以感谢这么多年来CSDN上各位高手对我的帮助。onvif的开发从gsoap的移植开始！今天完成了gsoap的移植，生成了代码。

一、 开发环境
本人开发环境为：
1. 电脑主频2.6G，内存4G；
2. 虚拟机：Product: VMware® Workstation;  Version: 7.1.3 build-324285
3. linux操作系统版本：

[root@localhost ~]# cat /proc/version
Linux version 2.6.25-14.fc9.i686 (mockbuild@) (gcc version 4.3.0 20080428 (Red Hat 4.3.0-8) (GCC) ) #1 SMP Thu May 1 06:28:41 EDT 2008
[root@localhost ~]#



4. linux编译环境：海思交叉编译器arm-hisiv100nptl-linux-g++

二、必备网址
1. Onvif官网：[http://www.onvif.org/](http://www.onvif.org/)
2. gsoap官网：[http://www.cs.fsu.edu/~engelen/soap.html](http://www.cs.fsu.edu/~engelen/soap.html)

三、操作步骤详解
1. 下载gsoap
根据以上提供的地址，登陆gsoap官网下载最新的安装包和相关文档；截止今天(2013年8月5日)，官网发布的最新版本的gsoap工具为2.8.15版本；最新版本的使用，可以参考文档：《gSOAP 2.8.15 User Guide .pdf》，该文档有非常详细的说明，现在网络上的大部分的demo都是copy这里的；该文档值得研究；该文档在其官网上可以下载，下载地址为：[http://www.cs.fsu.edu/~engelen/soap.html](http://www.cs.fsu.edu/~engelen/soap.html)；也可以在官网在线查看；或者，可以在gsoap安装包里面找到该文档，路径为：gsoap_2.8.15/gsoap-2.8/gsoap/doc, 名字为soapdoc2.pdf的文件。
2. 安装
将下载的安装包解压，并拷贝到linux的操作目录，本人操作目录为： 
$: /home/samba/onvif-2013.08.05/   
以上面的目录为例：（注意以下操作均在root用户权限下进行）
首先进入目录cd /home/samba/onvif-2013.08.05/ gsoap_2.8.15/gsoap-2.8/
然后，配置编译环境:
命令：$:./configure
然后，编译连接
命令：$:make
最后，安装gSOAP
命令：$:make install

以上命令执行完成之后可用wsdl2h或者soapcpp2查看gSOAP是否已经安装成功；安装成功即会显示该命令：如下所示

[root@localhost ~]# wsdl
wsdl    wsdl2   wsdl2h  
[root@localhost ~]# 
[root@localhost ~]# 
[root@localhost ~]# soap
soapcpp2  soapsuds  
[root@localhost ~]#



如果不知道被安装在哪个目录，可以用命令查询：
命令：$:find / -name wsdl2h
如下所示：

[root@localhost ~]# 
[root@localhost ~]# find / -name wsdl2h
/usr/local/bin/wsdl2h

^C
[root@localhost ~]# 
[root@localhost ~]#



即安装的wsdl2h和soapcpp2存在于系统的/usr/local/bin/目录
至此，完成了gsoap2.8.15工具的安装工作；

3. 头文件生成
以上两个命令中，命令wsdl2h用于生成头文件，命令soapcpp2用于生成源文件；这两个命令在/gsoap_2.8.15/gsoap-2.8/gsoap/bin中可以找到。bin目录下面包含三个目录，分别表示win32系统，linux系统以及macosx系统下的命令。但是bin目录下面的是源码包里面自带的，我们在自己系统里开发的话最好根据自己的编译器去编译生成新的命令。编译生成的新的命令可以在gsoap目录下看到。
工具安装完成之后可以在linux下重新建立一个自己的目录，用来生成代码：
这里建立目录：onvif_new
完整路径为：/home/samba/onvif-2013.08.05/ onvif_new/
将gsoap 目录下的typemap.dat文件拷贝到目录onvif_new
(1)在线生成头文件：
在onvif_new目录下执行命令：
~onvif_new$: wsdl2h -o onvif.h -c -s -t ./typemap.dat 
[http://www.onvif.org/onvif/ver10/device/wsdl/devicemgmt.wsdl](http://www.onvif.org/onvif/ver10/device/wsdl/devicemgmt.wsdl)
[http://www.onvif.org/onvif/ver10/media/wsdl/media.wsdl](http://www.onvif.org/onvif/ver10/media/wsdl/media.wsdl)
[http://www.onvif.org/onvif/ver10/event/wsdl/event.wsdl](http://www.onvif.org/onvif/ver10/event/wsdl/event.wsdl)
[http://www.onvif.org/onvif/ver10/display.wsdl](http://www.onvif.org/onvif/ver10/display.wsdl)
[http://www.onvif.org/onvif/ver10/deviceio.wsdl](http://www.onvif.org/onvif/ver10/deviceio.wsdl)
[http://www.onvif.org/onvif/ver20/imaging/wsdl/imaging.wsdl](http://www.onvif.org/onvif/ver20/imaging/wsdl/imaging.wsdl)
[http://www.onvif.org/onvif/ver20/ptz/wsdl/ptz.wsdl](http://www.onvif.org/onvif/ver20/ptz/wsdl/ptz.wsdl)
[http://www.onvif.org/onvif/ver10/receiver.wsdl](http://www.onvif.org/onvif/ver10/receiver.wsdl)
[http://www.onvif.org/onvif/ver10/recording.wsdl](http://www.onvif.org/onvif/ver10/recording.wsdl)
[http://www.onvif.org/onvif/ver10/search.wsdl](http://www.onvif.org/onvif/ver10/search.wsdl)
[http://www.onvif.org/onvif/ver10/network/wsdl/remotediscovery.wsdl](http://www.onvif.org/onvif/ver10/network/wsdl/remotediscovery.wsdl)
[http://www.onvif.org/onvif/ver10/replay.wsdl](http://www.onvif.org/onvif/ver10/replay.wsdl)
[http://www.onvif.org/onvif/ver20/analytics/wsdl/analytics.wsdl](http://www.onvif.org/onvif/ver20/analytics/wsdl/analytics.wsdl)
[http://www.onvif.org/onvif/ver10/analyticsdevice.wsdl](http://www.onvif.org/onvif/ver10/analyticsdevice.wsdl)
[http://www.onvif.org/ver10/actionengine.wsdl](http://www.onvif.org/ver10/actionengine.wsdl)
[http://www.onvif.org/ver10/pacs/accesscontrol.wsdl](http://www.onvif.org/ver10/pacs/accesscontrol.wsdl)
[http://www.onvif.org/ver10/pacs/doorcontrol.wsdl](http://www.onvif.org/ver10/pacs/doorcontrol.wsdl)
每个网址之间用空格分开，执行完命令即可生成头文件onvif.h。
以上地址来自onvif官网，可以自己登录官网查看。
注意，这种方式的话一定要保证您的虚拟机能出外网，简单说就是在虚拟机下能ping通百度或者其他外网地址；而且要保证网络比较快，否则在生成过程中会出现下载文件超时而导致生成头文件失败。该方式生成的话，所有文件均是在线实时下载。
(2)离线生成头文件
如果当前电脑无法联网，则可以通过离线的方式来生成，命令为：
~onvif_new$: wsdl2h -o onvif.h -c -s -t ./typemap.dat devicemgmt.wsdl media.wsdl event.wsdl display.wsdl
deviceio.wsdl imaging.wsdl ptz.wsdl receiver.wsdl recording.wsdl search.wsdl remotediscovery.wsdl
replay.wsdl analytics.wsdl analyticsdevice.wsdl actionengine.wsdl accesscontrol.wsdl doorcontrol.wsdl
注意不同的wsdl以空格分开。
目前Onvif官网提供的最新的WSDL文件一共有17个，可以将其下载下来。不过这些文件直接都是相互关联的，下载的话，除了这17个文件之外，还要下载所有相关联的文件。另外，下载之后，还需要更改文件中的路径，将wsdl文件中的schemaLocation以及location所指的路径进行修改，修改为本地的地址。将所有需要下载的文件下载到onvif_new目录，并修改相关地址。当然如果网速够快，提倡直接在线生成头文件和源文件。另外为了避免后续开发过程中出现某些功能的缺省，最好一次性生成包含所有功能的代码。
(3)离线生成头文件过程：
离线生成头文件过程如下所示：

[root@localhost onvif_new]# 

[root@localhost onvif_new]# 

[root@localhost onvif_new]# 



**  The gSOAP WSDL/Schema processor for C and C++, wsdl2h release 2.8.15

**  Copyright (C) 2000-2013 Robert van Engelen, Genivia Inc.

**  All Rights Reserved. This product is provided "as is", without any warranty.

**  The wsdl2h tool is released under one of the following two licenses:

**  GPL or the commercial license by Genivia Inc. Use option -l for details.



Saving onvif.h



Reading type definitions from type map file './typemap.dat'



Reading file 'devicemgmt.wsdl'...



Reading schema file 'onvif.xsd'...

Done reading 'onvif.xsd'



Reading schema file 'b-2.xsd'...

Done reading 'b-2.xsd'



Reading schema file 'bf-2.xsd'...

Done reading 'bf-2.xsd'



Reading schema file 't-1.xsd'...

Warning: unexpected element 'xsd:unique' at level 2 is skipped (safe to ignore)

Warning: unexpected element 'xsd:unique' at level 6 is skipped (safe to ignore)

Done reading 't-1.xsd'

Done reading 'devicemgmt.wsdl'



Reading file 'media.wsdl'...



Reading schema file 'onvif.xsd'...

Done reading 'onvif.xsd'



Reading schema file 'b-2.xsd'...

Done reading 'b-2.xsd'



Reading schema file 'bf-2.xsd'...

Done reading 'bf-2.xsd'



Reading schema file 't-1.xsd'...

Warning: unexpected element 'xsd:unique' at level 2 is skipped (safe to ignore)

Warning: unexpected element 'xsd:unique' at level 6 is skipped (safe to ignore)

Done reading 't-1.xsd'

Done reading 'media.wsdl'



Reading file 'event.wsdl'...



Reading file 'bw-2.wsdl'...



Reading file 'rw-2.wsdl'...



Reading schema file 'r-2.xsd'...

Done reading 'r-2.xsd'



Reading schema file 'bf-2.xsd'...

Done reading 'bf-2.xsd'

Done reading 'rw-2.wsdl'



Reading schema file 'b-2.xsd'...

Done reading 'b-2.xsd'



Reading schema file 't-1.xsd'...

Warning: unexpected element 'xsd:unique' at level 2 is skipped (safe to ignore)

Warning: unexpected element 'xsd:unique' at level 6 is skipped (safe to ignore)

Done reading 't-1.xsd'

Done reading 'bw-2.wsdl'

Done reading 'event.wsdl'



Reading file 'display.wsdl'...



Reading schema file 'onvif.xsd'...

Done reading 'onvif.xsd'



Reading schema file 'b-2.xsd'...

Done reading 'b-2.xsd'



Reading schema file 'bf-2.xsd'...

Done reading 'bf-2.xsd'



Reading schema file 't-1.xsd'...

Warning: unexpected element 'xsd:unique' at level 2 is skipped (safe to ignore)

Warning: unexpected element 'xsd:unique' at level 6 is skipped (safe to ignore)

Done reading 't-1.xsd'

Done reading 'display.wsdl'



Reading file 'deviceio.wsdl'...

Done reading 'deviceio.wsdl'



Reading file 'imaging.wsdl'...



Reading schema file 'onvif.xsd'...

Done reading 'onvif.xsd'



Reading schema file 'b-2.xsd'...

Done reading 'b-2.xsd'



Reading schema file 'bf-2.xsd'...

Done reading 'bf-2.xsd'



Reading schema file 't-1.xsd'...

Warning: unexpected element 'xsd:unique' at level 2 is skipped (safe to ignore)

Warning: unexpected element 'xsd:unique' at level 6 is skipped (safe to ignore)

Done reading 't-1.xsd'

Done reading 'imaging.wsdl'



Reading file 'ptz.wsdl'...



Reading schema file 'onvif.xsd'...

Done reading 'onvif.xsd'



Reading schema file 'b-2.xsd'...

Done reading 'b-2.xsd'



Reading schema file 'bf-2.xsd'...

Done reading 'bf-2.xsd'



Reading schema file 't-1.xsd'...

Warning: unexpected element 'xsd:unique' at level 2 is skipped (safe to ignore)

Warning: unexpected element 'xsd:unique' at level 6 is skipped (safe to ignore)

Done reading 't-1.xsd'

Done reading 'ptz.wsdl'



Reading file 'receiver.wsdl'...



Reading schema file 'onvif.xsd'...

Done reading 'onvif.xsd'



Reading schema file 'b-2.xsd'...

Done reading 'b-2.xsd'



Reading schema file 'bf-2.xsd'...

Done reading 'bf-2.xsd'



Reading schema file 't-1.xsd'...

Warning: unexpected element 'xsd:unique' at level 2 is skipped (safe to ignore)

Warning: unexpected element 'xsd:unique' at level 6 is skipped (safe to ignore)

Done reading 't-1.xsd'

Done reading 'receiver.wsdl'



Reading file 'recording.wsdl'...



Reading schema file 'onvif.xsd'...

Done reading 'onvif.xsd'



Reading schema file 'b-2.xsd'...

Done reading 'b-2.xsd'



Reading schema file 'bf-2.xsd'...

Done reading 'bf-2.xsd'



Reading schema file 't-1.xsd'...

Warning: unexpected element 'xsd:unique' at level 2 is skipped (safe to ignore)

Warning: unexpected element 'xsd:unique' at level 6 is skipped (safe to ignore)

Done reading 't-1.xsd'

Done reading 'recording.wsdl'



Reading file 'search.wsdl'...



Reading schema file 'onvif.xsd'...

Done reading 'onvif.xsd'



Reading schema file 'b-2.xsd'...

Done reading 'b-2.xsd'



Reading schema file 'bf-2.xsd'...

Done reading 'bf-2.xsd'



Reading schema file 't-1.xsd'...

Warning: unexpected element 'xsd:unique' at level 2 is skipped (safe to ignore)

Warning: unexpected element 'xsd:unique' at level 6 is skipped (safe to ignore)

Done reading 't-1.xsd'

Done reading 'search.wsdl'



Reading file 'remotediscovery.wsdl'...

Done reading 'remotediscovery.wsdl'



Reading file 'replay.wsdl'...



Reading schema file 'onvif.xsd'...

Done reading 'onvif.xsd'



Reading schema file 'b-2.xsd'...

Done reading 'b-2.xsd'



Reading schema file 'bf-2.xsd'...

Done reading 'bf-2.xsd'



Reading schema file 't-1.xsd'...

Warning: unexpected element 'xsd:unique' at level 2 is skipped (safe to ignore)

Warning: unexpected element 'xsd:unique' at level 6 is skipped (safe to ignore)

Done reading 't-1.xsd'

Done reading 'replay.wsdl'



Reading file 'analytics.wsdl'...



Reading schema file 'onvif.xsd'...

Done reading 'onvif.xsd'



Reading schema file 'b-2.xsd'...

Done reading 'b-2.xsd'



Reading schema file 'bf-2.xsd'...

Done reading 'bf-2.xsd'



Reading schema file 't-1.xsd'...

Warning: unexpected element 'xsd:unique' at level 2 is skipped (safe to ignore)

Warning: unexpected element 'xsd:unique' at level 6 is skipped (safe to ignore)

Done reading 't-1.xsd'

Done reading 'analytics.wsdl'



Reading file 'analyticsdevice.wsdl'...



Reading schema file 'onvif.xsd'...

Done reading 'onvif.xsd'



Reading schema file 'b-2.xsd'...

Done reading 'b-2.xsd'



Reading schema file 'bf-2.xsd'...

Done reading 'bf-2.xsd'



Reading schema file 't-1.xsd'...

Warning: unexpected element 'xsd:unique' at level 2 is skipped (safe to ignore)

Warning: unexpected element 'xsd:unique' at level 6 is skipped (safe to ignore)

Done reading 't-1.xsd'

Done reading 'analyticsdevice.wsdl'



Reading file 'actionengine.wsdl'...



Reading schema file 'onvif.xsd'...

Done reading 'onvif.xsd'



Reading schema file 'b-2.xsd'...

Done reading 'b-2.xsd'



Reading schema file 'bf-2.xsd'...

Done reading 'bf-2.xsd'



Reading schema file 't-1.xsd'...

Warning: unexpected element 'xsd:unique' at level 2 is skipped (safe to ignore)

Warning: unexpected element 'xsd:unique' at level 6 is skipped (safe to ignore)

Done reading 't-1.xsd'

Done reading 'actionengine.wsdl'



Reading file 'accesscontrol.wsdl'...



Reading schema file 'types.xsd'...

Done reading 'types.xsd'

Done reading 'accesscontrol.wsdl'



Reading file 'doorcontrol.wsdl'...



Reading schema file 'types.xsd'...

Done reading 'types.xsd'

Done reading 'doorcontrol.wsdl'



Warning: 8 service bindings found, but collected as one service (use option -Nname to produce a separate service for each binding)



Warning: 2 service bindings found, but collected as one service (use option -Nname to produce a separate service for each binding)



Warning: 2 service bindings found, but collected as one service (use option -Nname to produce a separate service for each binding)



Warning: WSDL "WS-BaseNotification" has no bindings to implement operations



To complete the process, compile with:

> soapcpp2 onvif.h



[root@localhost onvif_new]# 

[root@localhost onvif_new]# 

命令执行完成之后可以在当前目录下看到onvif.h。



4. 源文件的生成

生成onvif.h头文件之后，即可根据soapcpp2命令生成C源文件或者CPP源文件。

命令为：

onvif_new $: soapcpp2 -c onvif.h -x -I import路径 -I gsoap路径

如果要生成cpp文件，则为：

onvif_new $: soapcpp2 -cpp onvif.h -x -I import路径 -I gsoap路径

两种方式生成的文件基本相同，只是名字略有区别，大家可以生成对比看看。

如下所示：

[root@localhost onvif_new]# 

[root@localhost onvif_new]# soapcpp2 -c onvif.h -x -I /home/samba/onvif-2013.08.05/gsoap_2.8.15/

gsoap-2.8/gsoap/import -I /home/samba/onvif-2013.08.05/gsoap_2.8.15/gsoap-2.8/gsoap/



**  The gSOAP code generator for C and C++, soapcpp2 release 2.8.15

**  Copyright (C) 2000-2013, Robert van Engelen, Genivia Inc.

**  All Rights Reserved. This product is provided "as is", without any warranty.

**  The soapcpp2 tool is released under one of the following two licenses:

**  GPL or the commercial license by Genivia Inc.



Saving soapStub.h annotated copy of the source input

Saving soapH.h interface declarations

Using wsdd service name: wsdd

Using wsdd service style: document

Using wsdd service encoding: literal

Using wsdd service location: http://localhost:80

Using wsdd schema import: http://schemas.xmlsoap.org/ws/2005/04/discovery

Saving wsdd.nsmap namespace mapping table

Using ns1 service name: ActionEngineBinding

Using ns1 service style: document

Using ns1 service encoding: literal

Using ns1 service location: http://localhost:80

Using ns1 schema namespace: http://www.onvif.org/ver10/actionengine/wsdl

Saving ActionEngineBinding.nsmap namespace mapping table

Using ns2 service name: PACSBinding

Using ns2 service style: document

Using ns2 service encoding: literal

Using ns2 service location: http://localhost:80

Using ns2 schema namespace: http://www.onvif.org/ver10/accesscontrol/wsdl

Saving PACSBinding.nsmap namespace mapping table

Using ns4 service name: DoorControlBinding

Using ns4 service style: document

Using ns4 service encoding: literal

Using ns4 service location: http://localhost:80

Using ns4 schema namespace: http://www.onvif.org/ver10/doorcontrol/wsdl

Saving DoorControlBinding.nsmap namespace mapping table

Using tad service name: AnalyticsDeviceBinding

Using tad service style: document

Using tad service encoding: literal

Using tad service location: http://localhost:80

Using tad schema namespace: http://www.onvif.org/ver10/analyticsdevice/wsdl

Saving AnalyticsDeviceBinding.nsmap namespace mapping table

Using tan service name: RuleEngineBinding

Using tan service style: document

Using tan service encoding: literal

Using tan service location: http://localhost:80

Using tan schema namespace: http://www.onvif.org/ver20/analytics/wsdl

Saving RuleEngineBinding.nsmap namespace mapping table

Using tdn service name: RemoteDiscoveryBinding

Using tdn service style: document

Using tdn service encoding: literal

Using tdn service location: http://localhost:80

Using tdn schema namespace: http://www.onvif.org/ver10/network/wsdl

Saving RemoteDiscoveryBinding.nsmap namespace mapping table

Using tds service name: DeviceBinding

Using tds service style: document

Using tds service encoding: literal

Using tds service location: http://localhost:80

Using tds schema namespace: http://www.onvif.org/ver10/device/wsdl

Saving DeviceBinding.nsmap namespace mapping table

Using tev service name: PullPointSubscriptionBinding

Using tev service style: document

Using tev service encoding: literal

Using tev service location: http://localhost:80

Using tev schema namespace: http://www.onvif.org/ver10/events/wsdl

Saving PullPointSubscriptionBinding.nsmap namespace mapping table

Using timg service name: ImagingBinding

Using timg service style: document

Using timg service encoding: literal

Using timg service location: http://localhost:80

Using timg schema namespace: http://www.onvif.org/ver20/imaging/wsdl

Saving ImagingBinding.nsmap namespace mapping table

Using tls service name: DisplayBinding

Using tls service style: document

Using tls service encoding: literal

Using tls service location: http://localhost:80

Using tls schema namespace: http://www.onvif.org/ver10/display/wsdl

Saving DisplayBinding.nsmap namespace mapping table

Using tmd service name: DeviceIOBinding

Using tmd service style: document

Using tmd service encoding: literal

Using tmd service location: http://localhost:80

Using tmd schema namespace: http://www.onvif.org/ver10/deviceIO/wsdl

Saving DeviceIOBinding.nsmap namespace mapping table

Using tptz service name: PTZBinding

Using tptz service style: document

Using tptz service encoding: literal

Using tptz service location: http://localhost:80

Using tptz schema namespace: http://www.onvif.org/ver20/ptz/wsdl

Saving PTZBinding.nsmap namespace mapping table

Using trc service name: RecordingBinding

Using trc service style: document

Using trc service encoding: literal

Using trc service location: http://localhost:80

Using trc schema namespace: http://www.onvif.org/ver10/recording/wsdl

Saving RecordingBinding.nsmap namespace mapping table

Using trp service name: ReplayBinding

Using trp service style: document

Using trp service encoding: literal

Using trp service location: http://localhost:80

Using trp schema namespace: http://www.onvif.org/ver10/replay/wsdl

Saving ReplayBinding.nsmap namespace mapping table

Using trt service name: MediaBinding

Using trt service style: document

Using trt service encoding: literal

Using trt service location: http://localhost:80

Using trt schema namespace: http://www.onvif.org/ver10/media/wsdl

Saving MediaBinding.nsmap namespace mapping table

Using trv service name: ReceiverBinding

Using trv service style: document

Using trv service encoding: literal

Using trv service location: http://localhost:80

Using trv schema namespace: http://www.onvif.org/ver10/receiver/wsdl

Saving ReceiverBinding.nsmap namespace mapping table

Using tse service name: SearchBinding

Using tse service style: document

Using tse service encoding: literal

Using tse service location: http://localhost:80

Using tse schema namespace: http://www.onvif.org/ver10/search/wsdl

Saving SearchBinding.nsmap namespace mapping table

Saving soapClient.c client calling stubs

Saving soapClientLib.c client stubs with serializers (use only for libs)

Saving soapServer.c server request dispatcher

Saving soapServerLib.c server request dispatcher with serializers (use only for libs)

Saving soapC.c serializers



There were errors:

1 semantic error



[root@localhost onvif_new]# 





我们发现，生成过程中有错误：



wsa5.h(288): **ERROR**: remote method name clash: struct/class 'SOAP_ENV__Fault' already declared at line 274



可以发现这是由于重复定义导致，可以修改该文件。

打开文件gsoap_2.8.15/gsoap-2.8/gsoap/import/ wsa5.h



将277行int SOAP_ENV__Fault修改为int SOAP_ENV__Fault_alex



再执行命令

[root@localhost onvif_new]# 

[root@localhost onvif_new]# 

[root@localhost onvif_new]# soapcpp2 -c onvif.h -x -I /home/samba/onvif-2013.08.05/gsoap_2.8.15/

gsoap-2.8/gsoap/import -I /home/samba/onvif-2013.08.05/gsoap_2.8.15/gsoap-2.8/gsoap/



**  The gSOAP code generator for C and C++, soapcpp2 release 2.8.15

**  Copyright (C) 2000-2013, Robert van Engelen, Genivia Inc.

**  All Rights Reserved. This product is provided "as is", without any warranty.

**  The soapcpp2 tool is released under one of the following two licenses:

**  GPL or the commercial license by Genivia Inc.



Saving soapStub.h annotated copy of the source input

Saving soapH.h interface declarations

Using wsdd service name: wsdd

Using wsdd service style: document

Using wsdd service encoding: literal

Using wsdd service location: http://localhost:80

Using wsdd schema import: http://schemas.xmlsoap.org/ws/2005/04/discovery

Saving wsdd.nsmap namespace mapping table

Using ns1 service name: ActionEngineBinding

Using ns1 service style: document

Using ns1 service encoding: literal

Using ns1 service location: http://localhost:80

Using ns1 schema namespace: http://www.onvif.org/ver10/actionengine/wsdl

Saving ActionEngineBinding.nsmap namespace mapping table

Using ns2 service name: PACSBinding

Using ns2 service style: document

Using ns2 service encoding: literal

Using ns2 service location: http://localhost:80

Using ns2 schema namespace: http://www.onvif.org/ver10/accesscontrol/wsdl

Saving PACSBinding.nsmap namespace mapping table

Using ns4 service name: DoorControlBinding

Using ns4 service style: document

Using ns4 service encoding: literal

Using ns4 service location: http://localhost:80

Using ns4 schema namespace: http://www.onvif.org/ver10/doorcontrol/wsdl

Saving DoorControlBinding.nsmap namespace mapping table

Using tad service name: AnalyticsDeviceBinding

Using tad service style: document

Using tad service encoding: literal

Using tad service location: http://localhost:80

Using tad schema namespace: http://www.onvif.org/ver10/analyticsdevice/wsdl

Saving AnalyticsDeviceBinding.nsmap namespace mapping table

Using tan service name: RuleEngineBinding

Using tan service style: document

Using tan service encoding: literal

Using tan service location: http://localhost:80

Using tan schema namespace: http://www.onvif.org/ver20/analytics/wsdl

Saving RuleEngineBinding.nsmap namespace mapping table

Using tdn service name: RemoteDiscoveryBinding

Using tdn service style: document

Using tdn service encoding: literal

Using tdn service location: http://localhost:80

Using tdn schema namespace: http://www.onvif.org/ver10/network/wsdl

Saving RemoteDiscoveryBinding.nsmap namespace mapping table

Using tds service name: DeviceBinding

Using tds service style: document

Using tds service encoding: literal

Using tds service location: http://localhost:80

Using tds schema namespace: http://www.onvif.org/ver10/device/wsdl

Saving DeviceBinding.nsmap namespace mapping table

Using tev service name: PullPointSubscriptionBinding

Using tev service style: document

Using tev service encoding: literal

Using tev service location: http://localhost:80

Using tev schema namespace: http://www.onvif.org/ver10/events/wsdl

Saving PullPointSubscriptionBinding.nsmap namespace mapping table

Using timg service name: ImagingBinding

Using timg service style: document

Using timg service encoding: literal

Using timg service location: http://localhost:80

Using timg schema namespace: http://www.onvif.org/ver20/imaging/wsdl

Saving ImagingBinding.nsmap namespace mapping table

Using tls service name: DisplayBinding

Using tls service style: document

Using tls service encoding: literal

Using tls service location: http://localhost:80

Using tls schema namespace: http://www.onvif.org/ver10/display/wsdl

Saving DisplayBinding.nsmap namespace mapping table

Using tmd service name: DeviceIOBinding

Using tmd service style: document

Using tmd service encoding: literal

Using tmd service location: http://localhost:80

Using tmd schema namespace: http://www.onvif.org/ver10/deviceIO/wsdl

Saving DeviceIOBinding.nsmap namespace mapping table

Using tptz service name: PTZBinding

Using tptz service style: document

Using tptz service encoding: literal

Using tptz service location: http://localhost:80

Using tptz schema namespace: http://www.onvif.org/ver20/ptz/wsdl

Saving PTZBinding.nsmap namespace mapping table

Using trc service name: RecordingBinding

Using trc service style: document

Using trc service encoding: literal

Using trc service location: http://localhost:80

Using trc schema namespace: http://www.onvif.org/ver10/recording/wsdl

Saving RecordingBinding.nsmap namespace mapping table

Using trp service name: ReplayBinding

Using trp service style: document

Using trp service encoding: literal

Using trp service location: http://localhost:80

Using trp schema namespace: http://www.onvif.org/ver10/replay/wsdl

Saving ReplayBinding.nsmap namespace mapping table

Using trt service name: MediaBinding

Using trt service style: document

Using trt service encoding: literal

Using trt service location: http://localhost:80

Using trt schema namespace: http://www.onvif.org/ver10/media/wsdl

Saving MediaBinding.nsmap namespace mapping table

Using trv service name: ReceiverBinding

Using trv service style: document

Using trv service encoding: literal

Using trv service location: http://localhost:80

Using trv schema namespace: http://www.onvif.org/ver10/receiver/wsdl

Saving ReceiverBinding.nsmap namespace mapping table

Using tse service name: SearchBinding

Using tse service style: document

Using tse service encoding: literal

Using tse service location: http://localhost:80

Using tse schema namespace: http://www.onvif.org/ver10/search/wsdl

Saving SearchBinding.nsmap namespace mapping table

Saving soapClient.c client calling stubs

Saving soapClientLib.c client stubs with serializers (use only for libs)

Saving soapServer.c server request dispatcher

Saving soapServerLib.c server request dispatcher with serializers (use only for libs)

Saving soapC.c serializers



Compilation successful 



[root@localhost onvif_new]# 

[root@localhost onvif_new]#



可以看到，这次可以成功执行命令，在当前目录下可以看到生成的源文件以及不同功能所对应的命名空间。然后我们可以在新生成的源码上面进行开发。也许有人会觉得代码量很大，没关系，我们后续可以裁剪。另外，为了保证onvif所有的功能都包括在里面，我们必须在最开始开发的时候就生成包含所有功能的文件。

以上过程的wsdl文件可以在[http://download.csdn.net/detail/u011597695/5875143](http://download.csdn.net/detail/u011597695/5875143)下载。



后续介绍根据该过程生成的代码实现onvif发现功能。









