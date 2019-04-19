# PPP和PPPoE的工作原理 - xqhrs232的专栏 - CSDN博客
2014年09月12日 15:56:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：957
原文地址::[http://blog.csdn.net/easebone/article/details/7370369](http://blog.csdn.net/easebone/article/details/7370369)
相关文章
1、PPPOE协议----[http://blog.csdn.net/you_jinjin/article/details/7235250](http://blog.csdn.net/you_jinjin/article/details/7235250)
2、[PPPoE
 与PPP 协议分析](http://www.cnblogs.com/yzhe/archive/2010/02/08/1665616.html)----[http://www.cnblogs.com/yzhe/archive/2010/02/08/1665616.html](http://www.cnblogs.com/yzhe/archive/2010/02/08/1665616.html)
3、PPPoE协议详解----[http://wenku.baidu.com/link?url=uo7ZfcfsRm-vsPCskVABo-8qQcvh9EQcnNExVwxsL00bnrR6ps6zTImThCq4hGDdmwqjDvHlL6PFsC88JCL7AfBjYq7-ujO_eWz2CAiCdti](http://wenku.baidu.com/link?url=uo7ZfcfsRm-vsPCskVABo-8qQcvh9EQcnNExVwxsL00bnrR6ps6zTImThCq4hGDdmwqjDvHlL6PFsC88JCL7AfBjYq7-ujO_eWz2CAiCdti)
4、第八章 PPPoE协议----[http://wenku.baidu.com/view/128ab11ca8114431b90dd889.html?re=view](http://wenku.baidu.com/view/128ab11ca8114431b90dd889.html?re=view)
**PPP协议简介**
----------------------------
PPP: Point-to-Point Protocol，链路层协议。用户实现点对点的通讯。
PPP协议中提供了一整套方案来解决链路建立、维护、拆除、上层协议协商、认证等问题。具体包含这样几个部分：链路控制协议LCP（Link Control Protocol）；网络控制协议NCP（Network Control Protocol）；认证协议，最常用的包括口令验证协议PAP（Password Authentication Protocol）和挑战握手验证协议CHAP（Challenge-Handshake
 Authentication Protocol）。
帧格式与HDLC相似，不同的是PPP是面向字符，HDLC是面向位的。PPP帧格式如下：
![](http://hi.csdn.net/attachment/201203/19/0_13321570026Jp3.gif)
看到总共多了8个字节，其中首尾字节都是帧的起始和结束标志位，A表示地址，C表示控制。
协议的两个字段，表示后面信息部分的数据协议是什么，包括：
0x0021——信息字段是IP数据报
0xC021——信息字段是链路控制数据LCP
0x8021——信息字段是网络控制数据NCP
0xC023——信息字段是安全性认证PAP
0xC025——信息字段是LQR
0xC223——信息字段是安全性认证CHAP
**PPP协议的工作过程**
---------------------------------
PPP通讯是两个端点之间的通讯，每一端必须首先发送LCP packets数据来设定和测试数据链路，当链路建立后，peer才可以被认证，认证完成后，再通过发送NCP packets来选定网络层协议，这些后续的通讯就可以在网络层进行了。
具体的过程如下：
1. **链路静止状态**：链路一定开始并结束于这个阶段。当一个外部事件（例如载波侦听或网络管理员设定）指出物理层已经准备就绪时，PPP将进入链路建立阶段。在这个阶段，LCP自动机器将处于初始状态，向链路建立阶段的转换将给LCP自动机器一个UP事件信号。
2. **链路建立状态**：LCP用于交换配置信息包（Configure packets），建立连接。一旦一个配置成功信息包（Configure-Ackpacket）被发送且被接收，就完成了交换，进入了LCP开启状态。所有的配置选项都假定使用默认值，除非被配置交换所改变。有一点要注意：只有不依赖于特别的网络层协议的配置选项才被LCP配置。在网络层协议阶段，个别的网络层协议的配置由个别的网络控制协议（NCP）来处理。在这个阶段接收的任何非LCPpackets必须被silentlydiscarded（静静的丢弃）。收到LCPConfigure-Request（LCP配置要求）能使链路从网络层协议阶段或者认证阶段返回到链路建立阶段。
3. **认证阶段**：在一些链路上，在允许网络层协议packets交换之前，链路的一端可能需要peer去认证它。认证是不需要强制执行的。如果一次执行希望peer根据某一特定的认证协议来认证，那么它必须在链路建立阶段要求使用那个认证协议。应该尽可能在链路建立后立即进行认证。链路质量检查可以同时发生。在认证完成之前，禁止从认证阶段前进到网络层协议阶段。如果认证失败，认证者应该跃迁到链路终止阶段。
在这一阶段里，只有链路控制协议、认证协议，和链路质量监视协议的packets是被允许的。在该阶段里接收到的其他的packets必须被静静的丢弃。
4. **网络层协议阶段**：一旦PPP完成了前面的阶段，每一个网络层协议（例如IP，IPX，或AppleTalk）必须被适当的网络控制协议（NCP）分别设定。比如，NCP可以给新接入的PC机分配一个临时的IP地址，这样PC机就成为Internet上一个主机了。每个NCP可以随时被打开和关闭。当一个NCP处于Opened状态时，PPP将携带相应的网络层协议packets。当相应的NCP不处于Opened状态时，任何接收到的被支持的网络层协议packets都将被静静的丢弃。
5. **链路终止阶段**：PPP可以在任意时间终止链路。引起链路终止的原因很多：载波丢失、认证失败、链路质量失败、空闲周期定时器期满、或者管理员关闭链路。LCP用交换Terminate（终止）packets的方法终止链路。当链路正被关闭时，PPP通知网络层协议，以便他们可以采取正确的行动。交换Terminate（终止）packets之后，执行应该通知物理层断开，以便强制链路终止，尤其当认证失败时。Terminate-Request（终止-要求）的发送者，在收到Terminate-Ack（终止-允许）后，或者在重启计数器期满后，应该断开连接。收到Terminate-Request的一方，应该等待peer去切断，在发出Terminate-Request后，至少也要经过一个Restarttime（重启时间），才允许断开。PPP应该前进到链路死亡阶段。在该阶段收到的任何非LCPpackets，必须被静静的丢弃。
![](http://hi.csdn.net/attachment/201203/19/0_1332157030i8un.gif)
**PPPoE协议及其工作过程**
-----------------------------------------
PPPoE: PPP over Ethernet，是经常被用在DSL链接上的变种协议(RFC 2516)，另外PPPoA 有时也被使用(PPP over ATM)。
PPPoE一般用在DSL接入网络，如下图所示：
![](http://hi.csdn.net/attachment/201203/19/0_13321570562Rwr.gif)
PPPoE的报文就是在PPP的报文前面再加上以太网的报头，使得PPPoE可以通过简单桥接设备连入远端接入设备。但这里我们发现PPPoE报文中的PPP内容与原始的PPP并不相同。
![](http://hi.csdn.net/attachment/201203/19/0_1332157079ZPVh.gif)
也可参考整个PPPoE的报文（包括Ethernet Frame）：
![](http://hi.csdn.net/attachment/201203/19/0_1332157097fKKx.gif)
详细的说，就是下面的内容：
![](http://hi.csdn.net/attachment/201203/19/0_1332157113DbQ6.gif)
解释一下上面PPPoE报文中的关键字段的意义。
ETHER_TYPE： 
|0x8863|Discovery Stage|
|----|----|
|0x8864|PPP Session Stage|
CODE： 
|0x00|PPP Session Stage|
|----|----|
|0x09|PPPOE Active Discovery Initiation (PADI) packet|
|0x07|PPPOE Active Discovery Offer (PADO) packet|
|0x19|PPPOE Active Discovery Request (PADR) packet|
|0x65|PPPOE Active Discovery Session-confirmation (PADS) packet|
|0xa7|PPPOE Active Discovery Terminate (PADT) packet|
TAG_TYPES：（用于Discovery Stage中的协商参数）
0x0000 End-Of-List
0x0101 Service-Name
0x0102 AC-Name
0x0103 Host-Uniq
0x0104 AC-Cookie
0x0105 Vendor-Specific
0x0110 Relay-Session-Id
0x0201 Service-Name-Error
0x0202 AC-System-Error
0x0203 Generic-Error
PPPoE的工作过程分成两个阶段，即发现阶段(Discorvery)和PPP会话阶段。
发现阶段(Discovery Stage)的具体过程如下：
1. 用户主机用广播的方式发出PADI (PPPOE Active Discovery Initiatio) 包，准备去获得所有可连接的接入设备（获得其MAC地址）；
2. 接入设备收到PADI包后，返回PADO (PPPOE Active Discovery Offer) 作为回应；
3. 用户主机从收到的多个PADO包中，根据其名称类型名或者服务名，选择一个合适的接入设备，然后发送PADR (PPPOE Active Discovery Request) 包，另外如果一个用户主机在发出PADI后在规定时间内没有收到PADO，则会重发PADI；
4. 接入设备收到PADR包后，返回PAS (PPPOE Active Discovery Session-confirmation) 包，其中包含了一个唯一session ID，双方进入PPP会话阶段。
PPP会话阶段，即在session建立后的通讯阶段。
另外，无论是用户主机还是接入设备可随时发起PADT包，中止通讯。
使用PPPoE进行通讯的整个过程如下图所示：
![](http://hi.csdn.net/attachment/201203/19/0_1332157133TTi5.gif)
