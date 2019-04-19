# Wireshark抓包iOS入门教程 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月22日 13:31:57[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：590
[本文来自简书，原文地址:http://www.jianshu.com/p/c67baf5fce6d](http://www.jianshu.com/p/c67baf5fce6d)
网络抓包是个基础技能，对于网络协议的掌握有一定的要求。iOS上实现网络抓包可以用Charles（针对http和https），tcpdump（快速分析网络包），和Wireshare。之前写过一篇介绍[tcpdump抓包的入门文章](http://mrpeak.cn/blog/tutorial-tcpdump/)，和tcpdump相比，Wireshark提供丰富的GUI交互，而且能分析所有的网络协议，关键还是免费的，掌握好Wireshark就能应付绝大部分需要分析网络协议的场景了。
### 安装初识
Wireshark提供[Mac版本](https://www.wireshark.org/download.html)，可以从官网下载安装，到这篇博客为止最新版本应该是2.2.1。安装好之后打开的第一个界面如下：
![](http://upload-images.jianshu.io/upload_images/1534161-e577456249d93ef2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Wireshark在第一个界面就把当前系统所包含的网卡列出来了，直接点击任何一项就可以开始监听通过该网卡的所有网络流量。
当我们把iPhone通过usb连接macbook时，Wireshark并不能直接监听通过iPhone的网络流量，需要通过一个系统程序在我们的Mac系统上，建立一个映射到iPhone的虚拟网卡，在terminal中输入如下命令即可：
![](http://upload-images.jianshu.io/upload_images/1534161-7a69d6034c6f61e7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
格式是rvictl -s [设备udid]，设备的udid可以通过itunes或者itools获取，执行命令之后Wireshark能立即识别新增加的rvi0网卡，也就是上图中高亮的部分，双击rvi0这一项，Wireshare即进入如下界面开始监听iPhone设备上的所有流量。
![](http://upload-images.jianshu.io/upload_images/1534161-46abb75aae93d1b9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
此时，启动iPhone上的任意App，只要有网络流量产生，对应的网络包都会在Wireshark上述的列表中展示出来。
Wireshark的流量监控界面主要分为四块，由上至下第一部分（标号为1）是工具栏，通过工具栏我们可以控制监控的行为，比如开始抓包，停止抓包，重新开始抓包，以及在包之间跳转等等。工具栏的底部有个输入框，可以让我们手动输入包的过滤条件，这部分对于熟练使用Wireshark抓包非常重要，后面会详细的讲解。
第二部分（标号为2）是历史流量包列表展示界面，这里展示的是从抓包开始，所有通过我们iPhone设备的流量。列表界面不同的包有不同的颜色，Wireshark通过颜色来区分包的类型，对于特定场景快速识别目标流量非常有用，后面也会专门讲解。
第三部分（标号为3）是单个包的详细信息展示面板，我们在第二部分选中的网络包在这一部分会将其结构以可阅读的文本形式展示出来，要正确阅读这一部分的信息需要对tcp/ip协议有一定的掌握。
第四部分（标号为4）是单个包的二进制流信息展示面板，这一部分展示的信息是包的原始数据，也是一个网络包所包含内容的真实展现，我们在第三部分多选中的协议头，都会在这一部分以同步高亮的形式标记出来。这一部分的展示是为了让我们对包的真实内容做直观的判断，能具体到单个byte。
初步认识上述四块主要面板之后，可以尝试开始分析网络包，在开始分析网络包之前，先要对网络包有个大致的概念。
### 网络包结构
我们最初学习网络协议的时候，无论是OSI七层模型，还是经典的TCP/IP五层结构，都是以下图中的左边部分的形式展示的。
![](http://upload-images.jianshu.io/upload_images/1534161-5da4a0257b1409a4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这是一种经典的分层架构，确实也符合网络协议设计上的思路，但却不能表达网络包真实的包含关系。上图右边部分是我所绘制的一个包结构示意图。在我看来，这种洋葱式的结构更符合网络包的真实形态。Application是最内层的payload，除了Application这一层之外，其他层都是用自己这一层的协议header+所包含那一层的payload。可以用如下公式表示：
TCP Layer = TCP Header + Application Payload
IP Layer = IP Header + TCP Payload
...
我们分析每个网络包的时候要能理解每一个包它所代表的抽象含义，再进一步将相关联的包串联起来，形成一次完整的网络会话。
### 分析包结构
对于iOS程序员来说，我们绝大部分的流量分析都集中在HTTP或者基于TCP的socket长连接。从这一层面来说，和我们最贴近的三层是应用层(http)，传输层(tcp or udp)，网络层(ip)。
对于应用层来说主要是http协议的学习，对于http request和response格式的阅读，比如下图表示的一个http request包：
![](http://upload-images.jianshu.io/upload_images/1534161-75d2c673c83f1ea6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Packet详情面板以符合http协议的表述，将header中各个field清晰的罗列出来了，阅读起来很方便。
传输层我们应用较多的是tcp，这一层的阅读主要是tcp header的学习：
![](http://upload-images.jianshu.io/upload_images/1534161-60f7f7e6fb3e45a0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
典型的tcp header一般长度为20个字节，将这20个字节逐一学习一遍就可以分析大部分的tcp流量了。
网络层的分析主要是针对于IP Header，header结构如下：
![](http://upload-images.jianshu.io/upload_images/1534161-1a04b095ab973164.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这其中IP Header第十三个字节各个filed的理解，对于我们分析tcp流量的起始和结束尤其有用，典型的IPV4 Header也是20个字节，梳理一遍就可以分析IP包了。
所以对于包结构的分析关键在于三个知识点的学习：http header, tcp header, ip header，这么一看好像也没多少东西 ;)
### 使用Filter过滤包
使用Wireshark和使用Charles最大的区别在于，Charles只捕获HTTP流量，而Wireshark捕捉的是经过目标网卡所有的流量，流量包可以在几秒内膨胀到难以阅读的数量，所以此时我们需要使用Filter来做包的过滤，Filter规则定的越细，剔除掉的干扰信息就越多，分析起来就越快。
Wireshark的Filter分为两种，一种为Capture Filter，另一种是Display Filter。
Capture Filter出现在初始界面，在网卡列表的上方有个输入框，允许我们输入capture filter，一旦输入了特定的capture规则，Wireshark就只捕获符合该规则的流量包了。
![](http://upload-images.jianshu.io/upload_images/1534161-38c3413b983e4255.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Display Filter出现在流量监控界面，在工具栏的下方有个输入框，允许我们输入display filter，display filter只是从界面上过滤掉不符合规则的包，Wireshark实际上还是监听了这些包，一旦去掉display filter，所有的包又会出现在同一界面。
![](http://upload-images.jianshu.io/upload_images/1534161-91ce12131d33184e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Capture Filter的规则和我们平常使用tcpdump的filter语法是一致的，比如为了只监控http的流量，我们可以先在初始化界面选中rvi0网卡，再在capture filter输入框里输入：
```
//只捕获HTTP流量
port 80 or port 443
```
回车之后Wireshark就开始监控我们iPhone上所有的http和https流量了 ，非常简单，我们还可以使用其他的capture filter来捕获特定的流量，比如想分析DNS解析过程，可以使用：
```
//只捕获DNS流量
port 53
```
比如只想捕获和特定服务器相关的流量:
```
//只捕获和特定主机的流量
host 171.10.191.10
```
Display Filter的语法是由Wireshark自定义的，和Capture filter的语法不能混用。比如我们只想看某个主机的流量，可以使用如下Display Filter：
`ip.addr==171.10.191.10`
如果只看http或者https的流量，可以用：
`tcp.port == 80 || tcp.port == 443`
更多的语法规则可以查看[Wireshark官方文档](https://www.wireshark.org/docs/wsug_html_chunked/ChWorkBuildDisplayFilterSection.html)，Wireshark实际上提供了便捷的UI操作帮助我们来书写Display Filter，在Display
 Filter输入框的最右边有个Expression按钮，点击之后可以弹出如下界面：
![](http://upload-images.jianshu.io/upload_images/1534161-c016169b9190a778.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Display Filter的语法本质上是个等是关系描述，我们可以在search当中输入我们感兴趣的协议比如http，再在展开的协议头里选择我们的条件比如http.host，最后设置Relation和Value就可以生成一个Display Filter条件了。
### 包颜色规则
Wireshark在大多数时候捕获的包数量都远超我们感兴趣的数量，而且各个连接的包都混杂在一起，为了方便我们识别不同的连接会话，Wireshark默认使用一种着色规则帮助我们来进行包类型区分。
具体的规则可以通过菜单View->Coloring Rules...查看，默认规则如下：
![](http://upload-images.jianshu.io/upload_images/1534161-f990e7b97d9b0aea.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这里有个小技巧，如上图所示，我只将我感兴趣的协议包上了色，集中在http，tcp，udp包，这样分析起来更加直观。比如根据上图的规则，tcp三次握手中的Sync包是使用灰色标记的，这样我就可以在下图的包中迅速定位一次tcp连接的开始包位置：
![](http://upload-images.jianshu.io/upload_images/1534161-f1250d830f90281c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
当然，包的颜色也可以按照自己的视觉习惯进行定制，我个人习惯把Sync包和FIN包设置一个高亮的颜色，方便判断一次HTTP会话的起始和结束。
### 流量跟踪
Wireshark默认情况下将不同网络连接的流量都混在一起展示，即使给不同协议的包上色之后，要单独查看某个特定连接的流量依然不怎么方便，我们可以通过Wireshark提供的两种方式来实现这个目标。
##### 方式一：Follow Stream
当我们选中某个包之后，右键弹出的菜单里，有个选项允许我们将当前包所属于的完整流量单独列出来，如下图：
![](http://upload-images.jianshu.io/upload_images/1534161-a180032b5c1f5fc3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Wireshark支持我们常见的四种Stream，TCP，UDP，HTTP，SSL。比如我们选中Follow TCP Stream之后可以得到如下的详细分析输出（样本为监控iPhone手机的流量）：
![](http://upload-images.jianshu.io/upload_images/1534161-6af64727dfe47e53.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
上图中将iPhone和Server之间某次的连接流量完整的呈现出来，包括iPhone发送了多少个包，Server回了多少个包，以及iPhone上行和下行的流量，还提供流量编解码选择，文本搜索功能等。
##### 方式二：Flow Graph
Flow Graph可以通过菜单Statistics->Flow Graph来生成，这样我们可以得到另一种形式的流量呈现：
![](http://upload-images.jianshu.io/upload_images/1534161-f7fe8067a08fa739.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
和Follow Stream不同的是我们获取到的是完整的流量，从上图中可以看出从10.136.66.127(我的iPhone手机IP地址)发出的流向多个服务器的网络流量，包括DNS解析和SSL安全握手等。当然我们也可以在上图中下方的操作区域做进一步的过滤，可以使用Display Filter做进一步的流量定位。
Follow Stream更适合分析针对某一个服务器地址的流量，而Flow Graph更适合分析某个App的整体网络行为，包含从DNS解析开始到和多个服务器交互等。
其实Statistics菜单下还有更多的图表分析模式，可以根据不同的分析目标来选择，比如Statistics->HTTP->Requests可以得到如下按主机分门别类的HTTP请求分析图，和收费的Charles的展示结果类似。
![](http://upload-images.jianshu.io/upload_images/1534161-4039be44318aed58.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### HTTPS包分析
介绍完使用方式再来实际分析下HTTPS的流量。下图是我使用Wireshark在iPhone上抓包知乎App网络请求的结果：
![](http://upload-images.jianshu.io/upload_images/1534161-c016169b9190a778.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
当我使用Follow TCP Stream之后，一次完整的HTTPS会话流量就被单独过滤出来了，第一步先分析包列表界面。
通过高亮颜色找到会话的其实Sync包，继而可以快速的定位到HTTP建立连接之初的tcp三次握手所产生的三个包：
Sync： iPhone发送Sync。
Sync+Ack： Server发送Sync+Ack。
Ack： iPhone Ack。
三次握手之后是ssl handshake，ssl handshake分为以下几步：
Client Hello
这一个包是ssl握手的起始包，客户端（我的iPhone）会携带当前会话所依赖的一些关键信息：使用的tls版本（当前为tls1.2），上次的Session ID（如果可以session重用，就可以避免当前这次的安全握手），客户端所支持的加密算法套件（从下图中可以看出可以从22个suites里面挑选）等。
![](http://upload-images.jianshu.io/upload_images/1534161-529847fa5963f3a4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Server Hello
Server Hello这个包带上服务器这一端的一些信息，比如Server所选择的tls版本，或者带上可以重用的Session ID避免重新握手，在Client传过来的Cipher Suites当中挑选一个Cipher Suite进行后续的安全通话等。
![](http://upload-images.jianshu.io/upload_images/1534161-03c295700487e542.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Server 下发Certificate
Server同时会下发自己的Certificate，如下图所示：
![](http://upload-images.jianshu.io/upload_images/1534161-cc7a873fe00e7c90.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
从包列表界面可以看出，Certificate（大小为2407个bytes）这个包由于超过了1440个字节，被拆成了2个包，所以我们可以在包Info里面看到[TCP segment of a reassembled PDU]，我们使用Wireshark抓包的时候经常会看到reassembled PDU，出现这种情况是因为包太大，超过了MSS，需要拆成两个来发送。
接下来几个包是Client和Server基于上面交换的信息协商最后使用的密钥。
Server Key Exchange
Client Key Exchange
Change Cipher Spec
...
Send Application Data
各个包里面所包含的详细内容分析涉及到非对称加密算法的相关知识，这里就不展开了，使用Wireshark可以将整个HTTPS的握手过程非常清晰的展现出来，感兴趣的同学可以阅读[这篇文章](http://www.moserware.com/2009/06/first-few-milliseconds-of-https.html)。
当然大部分时候我们需要分析iPhone上HTTPS流量里的具体包内容，Wireshark虽然支持配置RSA私钥，但我们没办法直接获取iPhone设备上各个App所使用的私钥，这种场景下我们一般使用MITM（Man In The Middle）中间人攻击来破解HTTPS包内容，收费工具Charles可以通过代理的方式来实现此功能，免费版抓包工具mitmproxy同样也可以，Charles的使用教程比较多了，后续我们会再写一篇mitmproxy的教程介绍如何使用破解调试HTTPS的流量。
### 结束语
Wireshark就介绍到这里，现在在iPhone上抓包的方式有很多，有面向所有协议的tcpdump和Wireshark，也有针对HTTP的Charles和mitmproxy，无论使用哪个工具，前提都是我们需要对网络协议有全面的认识，所以在学习使用这些工具的同时，要持续深入的学习网络协议知识。
