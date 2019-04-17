# STUN, TURN, ICE介绍 - DoubleLi - 博客园






### STUN

STUN协议为终端提供一种方式能够获知自己经过NAT映射后的地址，从而替代位于应用层中的私网地址，达到NAT穿透的目的。STUN协议是典型的Client-Server协议，各种具体应用通过嵌入STUN客户端与STUN Server端通讯来完成交互。 

![](http://ww2.sinaimg.cn/large/6cbb8645gw1ekup3hifpvj20ff08jjrw.jpg)

在典型的运用STUN进行NAT穿透的场景中，STUN客户端首先向位于公网上的STUN服务器 发送Binding Request消息，STUN服务器接收到请求消息后识别出经过NAT转换后的公网地址60.1.1.1:12345，将其附加在Binding Response消息中返回给客户端。客户端得到这个地址 后用它替换SDP中的私网地址与终端B完成媒体协商。使用STUN进行NAT穿透对应用的要 求是必须使用同样的端口与STUN服务器交互和进行应用层通讯，比如当希望使用端口 37000进行RTP包的NAT穿透时，必须同样使用37000端口与STUN服务器通讯，否则从STUN 服务器获得的NAT映射后的地址一般与实际地址时不一样的。另一个要求是STUN客户端与 服务器端的通讯和应用使用获得的NAT映射地址进行应用层通讯在时间上必须有连贯性， 这源于NAT设备建立的绑定有生存时间，当原绑定消亡后，NAT设备为同一个私网地址建 立的新绑定往往不同，因此转换后的公网地址是不同的。



STUN方案的特性如下表：


|特性|说明|
|----|----|
|实现复杂度|实现简单|
|TCP穿透支持|不支持|
|对现有设备的要求|要求客户端支持，对现有NAT设备无改动要求，需增加STUN服务器|
|可扩展性|可扩展性好，与具体协议无关|
|安全性|一般|
|健壮性|差，不支持symmentric型NAT|
|其他|支持自动检测NAT类型，使用户即使在使用STUN协议无法实现NAT穿透时还可以根据NAT类型自主选择其他可使用的NAT穿透方案|



### TURN

TURN解决NAT穿透的思路与STUN类似，都是通过修改应用层中的私网地址达到NAT穿透。 与STUN不同的是，TURN是通过两方通讯的“中间人”的方式实现穿透，在这种方式下， 要进行通讯的两方分别与位于公网上的TURN服务器建立各自的连接进行通讯，由服务器负 责在两方之间进行数据转发。要达到这个目的，实现TURN客户端的终端必须在通讯开始前 与TURN服务器进行交互，得到服务器为其临时分配的位于TURN服务器上的公网地址，客户端使用它替换位于应用层中的私网地址。

![](http://ww3.sinaimg.cn/large/6cbb8645gw1ekup91wzjnj20fo090js1.jpg)

TURN方案的特性如下表：


|特性|说明|
|----|----|
|实现复杂度|难于实现。TURN的安全性设计增加终端设置的复杂度|
|TCP穿透支持|支持|
|对现有设备的要求|对现有NAT设备无要求，要求客户端支持，需增加TURN服务器s|
|可扩展性|可扩展性好，与具体协议无关|
|安全性|一般|
|健壮性|好，支持所有类型的NAT|
|其他|与P2P穿透方式相比，性能时relay穿透方式的弱点。另外TURN无法实现负载分担，解决的方式是把media relay服务器的分配工作放在 SIP proxy完成|

### ICE

与STUN和TURN相比，ICE并非是解决NAT穿透问题的协议，而是一个框架，在这个框架中， 可以整合其他现存的NAT穿透协议，如STUN、TURN、RSIP等。区别于其他的NAT穿透解 决方案，ICE是一种探索和更新式的解决方案，通过搜集自身和对端尽可能多的网络信息（各种网络地址），尝试在这些地址间建立数据通道，并在这一过程中不断更新先前收集到的信息，从而找出和选择能够进行NAT穿透的数据通道。

![](http://ww1.sinaimg.cn/large/6cbb8645gw1ekupccaj7vj20gf09ymxu.jpg)

ICE方案的特性如下表：
|特性|说明|
|----|----|
|实现复杂度|一般|
|TCP穿透支持|支持|
|对现有设备的要求|对NAT设备无要求，支持所有类型的NAT设备。客户端必须支持，网路结构中需增加STUN/TURN服务器|
|可扩展性|可扩展性好，与具体协议无关|
|安全性|较好|
|健壮性|好，适用与所有NAT及NAT拓扑类型，且由于存在中继服务器，NAT穿透一般总是能成功|
|其他||





[http://www.voip-info.org/wiki/view/ICE](http://www.voip-info.org/wiki/view/ICE)

[http://www.voip-info.org/wiki/view/TURN](http://www.voip-info.org/wiki/view/TURN)

[http://www.voip-info.org/wiki/view/STUN-bis](http://www.voip-info.org/wiki/view/STUN-bis)



# 试验UDP打洞穿透NAT

[http://www.hankcs.com/program/network/test-udp-holes-penetrating-nat.html#comments](http://www.voip-info.org/wiki/view/STUN-bis)









