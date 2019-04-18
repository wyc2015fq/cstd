# MQTT-SN协议乱翻之功能描述 - weixin_33985507的博客 - CSDN博客
2015年07月13日 11:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
### 前言
紧接上文，这是第三篇，主要是对MQTT-SN 1.2协议进行总体性功能描述。
嗯，这一部分可以结合着MQTT协议对比着来看。
### 网关的广播和发现
网关只能在成功连接到MQTT Server之后，才能够周期性的在无线个人区域网WPNs内对所有客户端广播ADVERTISE消息，便于客户端被动知道网关的存在。
在同一网络下，多个拥有不同Id的网关可有同时运行中，但会由客户端根据信号强弱决定连接具体网关，无论何时只能连接一个网关。
客户端可维护一份可用网关列表（包含网关地址），在接收到包含有新的网关id的ADVERTISE和GWINFO消息后，其列表需要添加新的网关元素进去。
ADVERTISE广播消息包含的下一次广播间隔时长Duration属性，单位秒，设为变量T_ADV，应该尽可能大与15分钟（900秒），频率降低是为了避免低速个人区域网络的拥塞。
针对接收ADVERTISE消息频率，处理能力较强客户端可以用于监督网关是否可用。eg：客户端连续N_ADV次接收不到某个网关ADVERTISE广播消息，可认为此网关经死掉不可用并且从已维护的网关列表中移除。同样的，作为备用的网关认为主网关已挂掉，此时可处于激活状态，正常发挥作用。
网关发送广播消息ADVERTISE的时间间隔很长，这对导致新加入的客户端不利，但客户端可以直接发送SEARCHGW广播消息进行查询网关。大量的新入设备会造成广播风暴造成网络拥挤，每一个新加入的客户端在发送SEARCHGW广播消息之前都需要获取一个随机的延迟发送值(0-Tsearchgw)，在延迟等待发送期间若接收到其它客户端发送的SEARCHGW广播消息，会取消掉自己的SEARCHGW广播消息发送，等待网关GWINFO消息通知。
SEARCHGW消息属性radius广播半径，记为变量Rb，1跳（1 hop）在一般密集部署下的MQTT-SN客户端基本可用。
网关接收到SEARCHGW会即刻回复包含自身id的GWINFO消息。客户端收到SEARCHGW后，若有需要延迟发送的SEARCHGW会取消掉，若自身维护一份多个可用网关列表，在等待T_GWINFO时间内没有收到GWINFO消息，会从列表中取出一条网关信息组装成GWINFO消息并广播出去。这就要求客户端已运行多时，并且维护多个可用网关列表。
GWINFO和SEARCHGW所包含半径radius属性值一致，这就要求底层网络在传输时进行决定是否需要传输到其它类型网络中。
若没有接收到响应，SEARCHGW消息可能被重新传输。两个连续的SEARCHGW消息重传间隔应该呈指数形式增加，避免太密集传输。
### 客户端的连接建立
无论是基于哪一种传输协议，TCP or UDP，客户端都需要建立连接，并且保持心跳，逻辑上和服务器端保持一条不断线的双向通道。下面一张图，演示了客户端建立连接的过程，并且设定客户端在CONNECT消息中标志位字段中遗嘱WILL属性为true，然后就有了遗嘱主题/消息的请求过程。
![](http://www.blogjava.net/images/blogjava_net/yongboy/mqtt-sn-03.jpg)
很多情况下，连接CONNECT是不需要遗嘱支持的，网关会直接返回CONNACK消息，但网关会因为拥塞或不支持一些CONNET特性，CONNACK所包含返回代码字段ReturnCode中包含拒绝代码，要求客户端检查是否连接成功，区别对待。比如：
```
CONNACK消息返回状态码为0x01（Rejected: congestion，因拥塞被拒绝），客户端需要在T_WAIT时间间隔后进行重试。
```
### 回话清理
已经连接的客户端断线后，若之前在CONNECT中没有设置过会话清理（Clean Session）标识，那么之前的订阅等信息在网关处将会持久存在。相比MQTT，MQTT-SN中的“Clean Session”标识被扩展到遗嘱特性中。在CONNECT消息中，CleanSession和Will组合将会产生以下效果：
- **CleanSession=true, Will=true**: 网关将会删除之前对应的所有订阅和遗嘱，新的遗嘱主题/消息稍后即将重新处理
- **CleanSession=true, Will=false**: 网关将会删除之前对应的所有订阅和遗嘱，返回CONNACK消息
- **CleanSession=false, Will=true**: 网关将继续持有之前对应的所有订阅，新的遗嘱主题/消息稍后即将重新处理
- **CleanSession=false, Will=false**: 网关将会继续持有之前对应的所有订阅和遗嘱等数据，并返回CONNACK消息
### 更新遗嘱流程
- CONNEECTION中标志位Will中设置是否需要更新遗嘱主题/消息
- 空WILLTOPIC（两个字节）消息将会促使网关删除对应遗嘱数据
- WILLTOPICUPD/WILLMSGUPD可以更新/修改遗嘱主题、遗嘱消息
- 空白WILLTOPICUPD（两个字节）消息意味着请求网清空对应已有的遗嘱数据
### 主题注册流程
受限于无线传感器网络的有限带宽和微小消息负载，PUBLISH消息中不能够包含完整的主题名称topic name。这就需要客户端和网关之间通过注册流程，获取主题名称对应的（16位的自然数）topic id，然后塞入PUBLISH消息的topicId属性中。
客户端发送REGISTER消息，网关返回REGACK消息，其所包含的ReturenCode属性决定注册成功与否：
- ReturnCode = “accepted”，topicId可以很愉快的使用在稍后的PUBLISH消息中
- ReturnCode = “rejected: congestion”，客户端需要稍等一段时间（T_WAIT表示，大于5分钟）再次重新注册
- ReturnCode = “rejected: invalid topic ID/not supported”，客户端需要稍作调整，再次重新注册
任意时间，只能执行一个REGISTER消息，有没有完成注册流程，需要等待。
网关->客户端方向，网关发送REGISTER消息给通知客户端指定topicId对应某个主题，以便后面发送PUBLISH消息使用。若客户端在订阅SUBSCRIBE消息时使用了通配符（#/+），那么与之相匹配的topic name也将被一一通知到。因此不建议使用通配符，较为低效。
### 客户端发布流程
客户端一旦获取到topic name对应topic id，就可以直接发送PUBLISH消息了。这和MQTT协议相比，PUBLISH消息中Topic Name被替换成Topic Id，除此之外，还要注意ReturnCode：
- ReturnCode = “rejected: congestion”，客户端需要稍等一段时间（>5分钟）后再次重试
- ReturnCode = “rejected: invalid topic ID”，客户端需要重新注册topic name获取topic id，然后再次重新发布
QoS 1和 QoS 2在任一时间，都必须等待已有PUBLISH消息完成，才能进行下面的PUBLISH消息发布流程。
### 预定义topic id和两个字符的topic name
预定义的topic id已提前指派好对应的topic name，需要客户端和网关在代码层级支持，省略了中间注册流程，在连接建立之后可以马上进行PUBLISH消息，但这需要在PUBLISH标志Flags字段中设置TopicIdType值为0b01（0b10表示两个字节长度的短topic name）。虽然可以快速发送PUBLISH消息，但客户端想订阅预定义的topic id，接收对应的PUBLISH消息，一样需要发送SUBSCRIBLE消息请求进行订阅。若乱指定预定义topic id，会收到ReturnCode=“Rejection: invalid topic Id”的异常。
预定义的短topic name只有两个字符长度的字符串（也是两个字节），topic id为两个字节表示的一个自然数（0-65535），两者使用场景一致，都需要在标志位Flags设置TopicIdType具体值，0b01表示预定义topic id，0b10表示两个字节长度的短topic name，需要分清。
### PUBLISH对应QoS -1值
这对仅仅支持PUBLISH QoS -1的非常简单的客户端实现而言，除此之外不支持任何特性。它不关心连接是否建立，也没有注册、订阅这一说，按照已经固化到代码中的网关地址直接发送PUBLISH消息，不关心网关地址是否正确、网关是否存活、消息是否发送成功。
下面的PUBLISH属性值依赖于QoS -1的情况：
- QoS标志，被置为0b11
- TopicIdType标志，可能是（预定义topic id）0b01也可能是（短topic name）0b10
- TopicId字段，预定义topic id或短topic name
- Data字段，需要发送的数据，没啥变化
### 客户端的订阅和退订
客户端对某个主题感兴趣，可以发起SUBSCRIBLE流程，携带上感兴趣的主题名（topic id），服务器一般会返回包含有指定主题Id（topic id）的SUBACK消息。订阅失败，可以从PUBACK的ReturnCode中获知：
- ReturnCode = “rejected: congestion”，客户端需要稍等一段时间T_WAIT（>5分钟）后再次重试
有一种情况是SUBSCRIBLE订阅主题包含通配符，网关的处理就很简单，在SUBACK中返回的topic id为0x0000。稍后，网关向客户端发送REGISTER消息走注册流程，通知通配符匹配到的主题对应的topic id值。
来自客户端的SUBSCRIBLE消息一样支持预定义topic id，以及短topic name，这和PUBLISH消息差不多。
退订就很简单，客户端发送UNSUBSCRIBLE消息，网关返回UNSUBACK消息。
但同一时刻，客户端只允许处理订阅SUBSCRIBLE或取消订阅UNSUBSCRIBLE按照串行化顺序，下一个操作依赖于上一个操作完全成功。
### 网关发布流程
服务器发布流程和客户端类似，在发布之前需要检测其主题是否已经向客户端提前注册过，若无需要把主题和指定的topic id放入REGISTER消息中发送给客户端进行注册流程，然后等待客户端处理结果REGACK。注册通过，然后才能正常发送PUBLISH消息。
网关需要确保REGISTER的主题以及PUBLISH消息的内容负载都不能太长超过当前网络负载上限（比如在ZigBee环境下不能超过60个字节），取消注册/发布流程就好了。
网关发布PUBLISH消息时，客户端检测到未知的topic id，把拒绝理由封装到PUBACK后，网关遇到ReturnCode=“Rejected: invalid Topic ID”非法topic id，需要考虑删除或重新注册。
客户端或许会拒绝其注册，或许会不允许PUBLISH消息，网关如上静默处理就好了，失败就失败了，不需要告知别人。
客户端发布流程于此类似，需要在发布之前进行主题注册以获取指定的topic id，提交PUBLISH消息后，同样需要检查PUBACK所包含的ReturnCode字段是接受还是拒绝，因网络拥塞而产生的拒绝，客户端需要在T_WAIT时间后再次重试。
客户端的发布必须是串行方式，下一个需要发送到PUBLISH消息需要等待上一个发送成功被网关接受之后才能进行处理。
### 心跳保活流程
一般是客户端->网关，网关->客户端也没有问题。但要求PINGREQ -> PINGRESP 一定要单个时针循环，PINGREQ发送者不能也是PINGRESP的发送者，那样不但乱了流程，也浪费了网络资源。嗯，不允许双向互发。
客户端可基于心跳机制监测已连接网关健康与否，连续多次接收不到来自网关的PINGRESP消息后，客户端连接下一个可替换的网关。因为客户端的连接和心跳和其它客户端状态属性不同步，但这可能会带来一个问题，同一时间若有大量的客户端洪水般同时连接一个网关，网关可能毫无征兆的会被冲垮掉。这就要求网关要有批量的连接处理能力，并发特性增强才行。
### 客户端断线流程
客户端主动发送DISCONNECT消息告知网关需要断线之后，若有交换信息的需要可以重新发起一个新的会话连接。DISCONNECT消息之后，网关不会清理掉已有订阅和遗嘱数据，除非在之前的CONNECT消息中已硬性设置了CleanSession会话清理标识为true。网关接收到DISCONNECT消息之后会返回一个DISCONNECT消息作为响应。
有一种情况是客户端会突然接收到来自网关的DISCONNECT消息，这也许是网关自身发生了异常错误，或网关无法定位客户端的消息归属（客户端的消息和客户端无法关联到一起），此时客户端需要发送CONNECT消息重建与网关的会话连接。
### 客户端重传流程
客户端->网关的消息都是单路传播的，这依赖于客户端所持有的已连接网关的单播地址。
客户端发送一个消息之后，需要启动一个重试定时器Tretry和一个重试计数器Nretry用以监督网关消息响应。定时器会被客户端在指定时间内接收到来自网关的消息后取消掉，若没有准时接收到则会触发定时器执行消息重发流程，连续Nretry次重发后，客户端会直接取消掉当前流程，判断当前网关已经断线，需要连接到另外一个可用的网关。假如另外的网关也是连接失败，会尝试重连之前的网关。
若在休眠状态下，一旦超过重试计数器值，客户端直接进入休眠状态。
### 客户端休眠支持策略
这里所说的客户端指的是依赖电池驱动的电子设备，你要明白一个事实，节省电池资源是多麽的重要，省电就是关键，没电了就没得玩了嘛。当不处于激活状态时为了省电就得需要进入睡眠/休眠状态，当有数据需要接收或发送时就可以醒过来。网关嘛需要追踪设备的休眠状态并且支持缓存需要发送给休眠设备的消息，在设备唤醒时一一发送。
下面是客户端的状态转换图，很清晰描述了各种状态之间的交互： ![](http://www.blogjava.net/images/blogjava_net/yongboy/mqtt-sn-04.jpg)
客户端具有五种状态：激活（active），休眠（asleep），唤醒（awake），断线（disconnected），丢失（lost），每次只能是其中一种。
网关需要监督客户端的状态，开始于CONNECT消息中存活时长字段（keep alive），在大于存活时长时间内网关接收不到来自客户端消息，网关认为客户端已经处于丢失状态（lost），会激活对应的遗嘱特性若存在的话。
客户端发送DISCONNECT消息但没有duration休眠时长字段，网关这将处于没有时间监督的断线状态。一旦包含duration休眠时长字段，表示客户端需要休眠一段时间，网关这客户端被转换为休眠状态，休眠时长为duration所定义在值。超过此休眠时长的一段时间内，网关若接收不到客户端发送过来的任何消息，那么客户端会被转化为丢失状态，若已设置遗嘱特性，此时遗嘱特性会生效。客户端休眠期间需要被发送的消息都会被网关缓存。
睡眠状态下流程图会更形象的说明流程： ![](http://www.blogjava.net/images/blogjava_net/yongboy/mqtt-sn-05.jpg)
毫无疑问，网关可使用一个休眠定时器维护客户端的休眠状态等，休眠定时器会被停掉当网关接收到客户端发送过的PINGREQ消息，网关从PINGREQ消息所包含的Client Id检索是否存在已缓存的PUBLISH消息，若有会一一按照顺序发送到客户端。所有对应已缓存消息发送完毕后，会随之发送一个PINGRESP消息。若没有缓存消息，网关直接返回一个PINGRESP消息。网关会重新启动休眠定时器，网关维护的客户端状态被转换为休眠状态，客户端在接收到PINGRESP消息之后，将直接转向休眠状态，节省用电。
客户端在唤醒状态下处理消息，遵守“客户端重传流程”行为，一旦达到重试计数器限制，将进入睡眠状态。
客户端从休眠状态转向唤醒状态用于检查网关是否为其缓存消息时，需要发送一个PINGREQ消息到网关；从休眠/唤醒状态转换为激活状态，需要发送一个CONNECT消息告知网关；转换为断线状态时需要发送两个字节的DISCONNECT（没有休眠时长字段duration）消息；需要重新定义的休眠时长，发送一个DISCONNECT消息（包含新的duration时长值）通知网关即可。
### 小结
功能性描述介绍完了，基本上MQTT-SN协议介绍已接近尾声，最后面的篇章就是短短的实现描述了。
原文 http://www.blogjava.net/yongboy/archive/2015/01/09/422156.html
