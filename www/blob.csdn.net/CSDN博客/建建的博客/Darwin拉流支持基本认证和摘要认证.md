# Darwin拉流支持基本认证和摘要认证 - 建建的博客 - CSDN博客
2018年09月29日 10:27:51[纪建](https://me.csdn.net/u013898698)阅读数：55
RTSP认证作为RTSP标准协议的一部分，其内容在网上可以找到很多，在此就不在赘述。EasyDarwin作为RTSP服务器也提供了对RTSP请求的基本认证和摘要认证。
EasyDarwin用RTSPSession对象来处理RTSP请求，在其Run函数的kAuthenticatingRequest状态和kAuthorizingRequest状态及其调用的QTSS_RTSPAuthenticate_Role角色和QTSS_RTSPAuthorize_Role角色用来处理RTSP请求的认证部分，角色的具体处理在 QTSSAccessModule模块的AuthenticateRTSPRequest函数和QTSS_RTSPAuthorize_Role函数中，其中前者主要处理用户名/密码认证，后者处理权限认证。如果想对细节有更多了解，就多分析这方面的源码，因为我也不是完全熟悉，所以就对细节不在详述，仅仅把自己研究的怎样通过文件的配置来使EasyDarwin在进行拉流基本认证和摘要认证的时候添加我们自己创建的用户名和密码。
首先下载最新版本的EasyDarwin,在***\EasyDarwin\EasyDarwin\WinNTSupport目录下存在3个文件：qtaccess、qtusers和qtgroups，qtaccess是控制访问权限的，qtusers是管理用户名和密码的，qtgroups是用来说明用户是属于那一个组的。
我们先来配置qtusers，下面是我配置好的一个qtusers文件，内容如下：
realm EasyDarwin
aGFja21l:0muym.z3yrHSw:18060847e049b6fd5d993aaa62e96585
admin:admin:6351fbbe4f7b8f2672515fed518df79d
其中第一行realm EasyDarwin，说明我们设置了认证的realm 为EasyDarwin，当然这里也可以为其他字符串。
第二行是以两个冒号分割的3个字符串，分别为我们设置的用户名、密码和加密的密码，其中密码对应于基本验证，加密的密码对应于摘要认证，用户名和密码我们可以随意设置，但加密的密码需要计算，其计算规则为MD5(username:realm:password)，在这里username为aGFja21l，realm为EasyDarwin，password为0muym.z3yrHSw，经计算得到加密的密码为18060847e049b6fd5d993aaa62e96585。
第三行是设置另一个用户名和密码，其规则和第二行一样，在此不再赘述。
现在我们看下qtaccess文件，下面是我配置好的一个qtaccess文件，内容如下：
AuthScheme digest
require user aGFja21l admin
第一行表示我们采取的是摘要认证，如果想采用基本认证，在此更换为AuthScheme basic
第二行表示我们希望有哪些用户可以访问，在此将我们刚才创建的用户aGFja21l 和admin添加到这里。
OK,到此文件配置已经结束，如果你想验证下，可以使用VLC,地址键入rtsp://user:password@ip:port/XXX.sdp来进行验证，或者键入rtsp://ip:port/XXX.sdp且在接下来弹出的认证对话框里输入账号名和密码。
如果你不想添加认证，请参考文档[http://blog.csdn.net/xiejiashu/article/details/39523765](http://blog.csdn.net/xiejiashu/article/details/39523765)
EasyDarwin代码详见[https://github.com/EasyDarwin/EasyDarwin](https://github.com/EasyDarwin/EasyDarwin)。
如有错误，欢迎指正！
