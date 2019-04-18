# HDMI介绍与流程 - TaigaComplex求职中 - 博客园







# [HDMI介绍与流程](https://www.cnblogs.com/TaigaCon/p/3840653.html)





HDMI，全称为（High Definition Multimedia Interface）高清多媒体接口，主要用于传输高清音视频信号。



# HDMI引脚：

HDMI有A,B,C,D,E五种引脚类型，目前市面中比较常见的就是Type A：

![](https://images0.cnblogs.com/i/421096/201407/122345120985070.jpg)

其中
- 1-9 都是TMDS数据传输实际上用到的引脚，分为0,1,2三组
- 10-12 为TMDS时钟信号，如当前Video Timing为480p@60Hz（Htotal：800，Vtotal:525），则TMDS clock = 800x525x60 = 25.2MHz。TMDS clock就像是对像素的打包，一个clock分别在三个Channel传输一个像素的R、G、B（8bit）信号。
- 13 为CEC（consumer electronic control）类似一种扩展的HDMI功能，供厂家自己定制HDMI消息，（比如说你有一台sony的DVD与TV，两者用HDMI线接上，如果你用TV的遥控器可以控制DVD，另DVD执行某种功能，那么该功能的命令信号就是通过TV与DVD间的CEC引脚传输的）
- 14 为保留引脚，未使用（或者也可以为CEC提供多一个引脚）
- 15-16 为I2C引脚，用于DDC（Display Data Channel，主要用于EDID与HDCP的传输）传输，具体可以[查看](http://www.cnblogs.com/TaigaCon/archive/2012/11/22/2782584.html)。在HDMI的流程中，DDC通信几乎是最先做的（前有Hotplug），因为HDMI的主从两个设备需要通过DDC来获得他们对方设备的EDID，从而得到各种信息，并且通过比较timming以确定以后送出来的timming为最合适的
- 17 为接地引脚
- 18 为5v的AC引脚
- 19 为Hotplug（热拔插）引脚（用于监测HDMI设备有没有存在，如果存在（Hotplug为high）那么可以通过DDC去读EDID），HDMI有规定在HDMI 5vAC断电时source device可以读reciever device的EDID，也就是需要Hotplug为High。其中有两种Hotplug相关的情况会导致HDMI被识别为DVI：- Hotplug为High，不过EDID并没有准备好，那么信号源设备会由于无法读到EDID而认为接收设备为DVI，这样会导致HDMI有图像无声的问题。
- Hotplug为Low，也会导致信号源无法读到EDID而认为接收设备为DVI，从而导致HDMI有图无声
- 在TV这种有多个HDMI通道的情况下，有时会在多个HDMI通道进行切换，切换后HDMI通道应当先初始化，即先把Hotplug拉低，通知HDMI source device之前所用的EDID已经改变，需要重新读取，那么source device在Hotplug被拉高的时候会去读取新的EDID，但是拉低这个过程至少需要100ms，否则source device有可能不会去读取新的EDID，从而输出DVI信号




# 传输流程

HDMI TMDS传输的数据类型有三种（加上Hsync与Vsync就算4种）：
- Preamble（控制信息），主要用于控制接下来传输的数据是Data Island或者Video Data
- Data Island（数据包），各种类型的包信息，包括音频数据包，图像信息包等
- Video Data （视频信息），视频像素数据，HDMI可以传输RGB与YUV两种格式的像素数据
- 还有Hsync与Vsync

HDMI的数据传输有TMDS0，TMDS1，TMDS2三个通道，每个通道的传输流程都是一样的：

![](https://images0.cnblogs.com/i/421096/201407/131242029423462.jpg)

如果是8bit的数据进入TMDS编码器，得到抗干扰性强的10bit TMDS信号，然后再进行串行化输出；在接收端收到串行的HDMI信号后，进行信号复原，得到10bit的TMDS信号，最后用TMDS解码器解码得到原来的8bit数据。

总体传输流程如下：

![](https://images0.cnblogs.com/i/421096/201407/131249098799729.jpg)


- 如果传输的是Video Data，并且格式为RGB，那么会占用三个通道的所有24bit输入，Channel0[7:0]用于传输B，Channel1[7:0]用于传输G，Channel2[7:0]用于传输R。
- 如果传输的是Data Island，则占用三个通道共10bit输入，Channel0[3:2]用于传输Data Island Header（包头），Channel1[0:3]与Channel2[0:3]用于传输Data Island Content（包内数据）。
- 如果传输的是Preamble，则占用1，2两个通道共4bit输入，Channel1[1:0]与Channel2[1:0]分别为CTL0,CTL1,CTL2,CTL3，用于判断接下来输入的是Video Data或者Data Island

![](https://images0.cnblogs.com/i/421096/201407/131507387709650.jpg)

对于Hsync与VSync，会占用Channel0通道的两个bit输入，Channel0[0]为Hsync，Channel0[1]为Vsync



# 传输周期

HDMI的TMDS数据传输可以分为三个传输周期：

![](https://images0.cnblogs.com/i/421096/201407/131540065674788.jpg)
- Control Period期间会传输Hsync，Vsync，并且在该时期的最后阶段会传输Preamble
- Data Island Period期间会传输Data Island（数据包），也会有Hsync与Vsync
- Video Data Period期间会传输Video Data（视频像素数据）

某帧的总体周期如下：

![](https://images0.cnblogs.com/i/421096/201407/131544223325782.jpg)

三个传输周期的过渡如下：

![](https://images0.cnblogs.com/i/421096/201407/131558526451664.jpg)
- 左边是Control Period，传输有Hsync，Vsync与Preamble
- 中间是Data Island Period，传输有Hsync，Vsync，以及两个Packet Header与Packet（每32个clock 一个packet）；另外Data Island的两端会用Guard Band保护并隔开Data Island的数据，因为这个阶段传输的数据大多是非常重要的，比如其中就有图像分辨率，决定后面的Video Data数据的显示方式
- 右边是Video Data Island，传输视频像素数据，在该时期的开头也有Guard Band



# Data Island Packet结构

![](https://images0.cnblogs.com/i/421096/201407/131618485828530.jpg)



所有Data Island Packet都以32个时钟脉冲为一个周期，也就是说每32 clk传输一个包。

以上图为例，
- 包头部是BCH block 4，由Channel0[2]传输，32clk表示有32bit，则为4byte，前三个byte为包头，最后一byte为校验码
- 包体为BCH block 0,1,2,3，分别由Channel1，Channel2共8根线传输，共有24byte与6byte的校验码
- Parity Bits校验码是用于检验HDMI Cable传输过程中是否发生了错误，如果该Packet在HDMI接收端校验错误，如果只有一个bit的错误，那么可以修正，超过1bit的错误会被判别为无效Packet（由于HDMI是一直在发送数据因此无法重发错误Packet？）

所以说，在接收端，在解完包之后，需要取出各个BCH block的Parity bit，进行Calibration（校验）

Packet类型各种各样，详细请看HDMI Spec



# Audio Clock

Audio的采样率有44100,48000，192000等，是各种各样，在HDMI传输时，Audio是PCM级（无压缩）传输，把PCM数据打散到各个包内，为了得到每个音频帧的数据，也需要知道Audio的采样率。HDMI中规定Audio的传输方式：

![](https://images0.cnblogs.com/i/421096/201407/131718133488509.jpg)

Audio采样率fs重建依靠的主要参数为：
- TMDS Clock
- CTS
- N

在发送设备这端，已知参数有采样率fs，视频时钟Video Clock(TMDS clock)，以及预先设定好的参数N，求CTS:

$CTS = \frac{N *f_{TMDS}}{128\times{f_x}}$

在接收设备这端，TMDS clock通过硬件设备可以得到，N，与CTS通过Audio Packet传输过来，求fs:

$128*f_s = \frac{N\times{f_{TMDS}}}{CTS}$

在接收端为了保持fs的稳定与精确，需要进行锁相，即用VCO（[Voltage-controlled oscillator](http://en.wikipedia.org/wiki/Voltage-controlled_oscillator)压控振荡器，通过电压控制产生的频率）产生合适的频率，然后用PFD（[Phase Frequency Detector](http://en.wikipedia.org/wiki/Phase_Frequency_Detector)）来锁频

![](https://images0.cnblogs.com/i/421096/201407/142142540849782.jpg)


- 首先，由于VCO有个最佳的工作区域如（200MHz~500MHz），那么为了保证VCO在最佳工作频率内，我们可以从后倒推回来，先对输出的fa128做乘法得到

$f_{vco} = {f_{a128}}\times{S}\times{S_2}$

由于$f_{a128}$只有那么几种（44.1k，48k等），所以比较容易得到$S$与$S_2$

- 然后，为了更快进行频率匹配，需要对近来的频率$f_x$（就是晶振时钟$f_{crystal}$）或者$f_v$（pixel clock）做除法，也对$f_{vco}$做除法，令两个趋向相等。对于细微的区别可以用D Code 进行修正

$\frac{f_{vco}}{M} = \frac{f_x}{K}$

- 最后做PFD锁相
- 第2,3步的反馈操作循环地进行，最后可以得出比较稳定的$f_{vco}$
- 最终得到

$f_{a128} = \frac{f_{vco}}{{S}\times{S_2}}$




# HotPlug

HotPlug即热拔插，当接上接口时就可以判定设备是否存在，以进行后续工作。

HDMI source device（HDMI HPD）会监测sink device的Hotplug端口，如果Hotplug为High，则证明设备可以工作，然后去读取DCC，如果为low，则证明设备已断开。

HDMI sink device应该通过把Hotplug拉低，来通知source device EDID已经被改变，那么source device在Hotplug被拉高后，就会重新来读取新的EDID，拉低这段时间应该多于100ms。

HDMI规定，HDMI 的5v引脚通电时，可以通过DCC去读取EDID，即需要保证Hotplug为high，有些Hotplug是直接接到5V上的（如下）。

Hotplug接法：（HDMI HPD(Hotplug detect ?)检测sink的Hotplug端）

![](https://images0.cnblogs.com/i/421096/201407/131832067232455.jpg)

上面用5V引脚进行供电，并接上Hotplug，这样做就能保证每次source device接上sink device时，都可以去读取到EDID。但是这样做有一个缺点，当5V电源断开时，会有5v的电压回灌给HDMI HPD与Hotplug，5V电压会冲击Hotplug，一旦Hotplug引脚无法承受5V电压的回灌，会被打穿。

下面有个较好的Hotplug接法：



![](https://images0.cnblogs.com/i/421096/201407/131827120823202.jpg)

上面用的是额外的GPIO引脚加上三极管控制HDMI HPD为0还是1，如果HDMI0_HPD_CTL输出0，那么三极管断开，HDMI0_HPD侦测到High，如果HDMI0_HPD_CTL输出1，那么三极管打通，HDMI0_HPD侦测到low。



# HDMI Sink

例如像TV这种就是HDMI的接收端，那么HDMI接收端需要做些什么东西。

HDMI可以接收到的有三个通道的TMDS Data，TMDS Clock，可以设置Hotplug，还有DCC传输用的I2C引脚。上面已经讲了TMDS Data，与设置Hotplug，接下来分析TMDS Clock。

TMDS Clock 就是Pixel Clock，即一个像素点所用的时钟频率。TMDS Clock通过clk 引脚传输到接收端，但是接收端并不清楚发送端发过来的TMDS Clock 频率为多少，因此需要通过Phy（PHY是模拟数字转换部分，不同于ADC，PHY是不知道采样频率的，需要自己锁频、锁相，侦测确切的输入频率）来进行锁相得到。但是由于HDMI频宽太宽（480P@60Hz为25.2MHz，1080P@60Hz为162MHz，甚至还有高达340MHz的），一般VCO（压控振荡器，通过电压控制产生的频率）无法覆盖这么大的范围，因此需要分频带来设置Phy：

先侦测输入频率落在哪个频带，然后根据不同频带做不同设置。

用TV产生的晶振来数count，数得count后就知道TDMS Clock了

$f_{crystal} = {count}\times{f_{TMDS}}$？？

或者用1024个TMDS Clock来数晶振个数

${1024}\times{f_{TMDS}} = {count}\times{f_{crystal}}$？？



由于视频信号从RGB个8bit通过TMDS编码后变成了10bit，然后又串行化，所以实际用于接收TMDS Data所用的时钟应该为：

$f_{ReceiveClock} = {10}\times{f_{TMDS}}$

另外ReceiveClock也可以不用直接采用上面的乘法，而是采用TMDSClock为参考、硬件锁相的方法来得到。

得到ReceiveClock后就可以去设置频率PLL，然后对三个通道进行采样得到TMDS Data。



# Timming Detect

在Sink端还有需要进行Timming Detect，因为如果设备可以支持（如chroma），HDMI可以自由更换Timming，而当Timming更换了之后，Sink需要重新设定Phy。因此，通过侦测频率的改变来检测是否更换了Timing是必要的。一般会有一个中断服务（或循环）线程来侦测频率的改变，一旦频率改变后，该进程会通知重新设定Phy，保证HDMI的正确运行　





# HDMI版权内容保护之HDCP

HDCP通过DDC传输

HDCP主要用于版权视频的保护，举例来说，如果有一台蓝光DVD播放机可以播放blueray DVD，并且该DVD已经获得HDCP授权，你现在想把该DVD影像输出到某台TV，但是该TV没有获得HDCP授权，那么该TV可能就没法播放影像，或者播放质量下降，如出现雪花，图像从1080p变为480p，或者没有声音，都有可能。

HDCP是靠两个设备的交互进行HDCP授权认证的，认证流程如下

![](https://images0.cnblogs.com/i/421096/201407/142253013037154.jpg)


- Transmitter会发送一个key An（64bit）与Aksv（key selection vector 40bit）给Receiver
- Receiver接收到An后，也会发送一个Bkvs以及REPEATER（表明B设备是否为Repeater设备）给Transmitter
- Transmitter开始HDCP认证码算法:


要理解算法，首先我们需要知道ksv是用来干嘛的

- 在每个HDMI设备内部，都会保存40组64bit的key，key[40]
- 40bit的kvs，每一个bit都是一个索引，当kvs的某一位n为1时，会把key[n]取出来，
- 把所有的key[n]相加，得到km，

- Receiver也会做HDCP认证码算法这个步骤得到km'
- Transmitter与Receiver都会用km\km'去做hdcpBlkCipher，得到一个值R0与R0'
- 100ms后Receiver把R0'发送到Transmitter与R0做比较，相等则认为认证完毕。当然km = km'才能保证R0 = R0'。
- 此后的每一帧，Transmitter与Receiver都会运行一次hdcpBlockCipher，不过参数为上次生成的Ks与M，生成的新参数为Ks,M,T

![](https://images0.cnblogs.com/i/421096/201407/142339321151380.jpg)

- 在第128帧的时候，另R = T
- 在间隔第一次通信的2s后，再次进行认证
- 后续都采用7,8,9这三个步骤进行迭代认证

![](https://images0.cnblogs.com/i/421096/201407/142344162877196.jpg)



此外HDMI自1.1后还支持一个更快速与频繁的认证方式，就是上方设备通信图的下半部分
- 在每第16的倍数帧，用T与当前帧的Channel0的0像素做异或得到Pj
- Channel0的0像素到达Sink后，也与Sink的T‘做异或得到P'j
- Sink把P'j发送到Source，与Pj做比较，相同则通过认证

![](https://images0.cnblogs.com/i/421096/201407/142352340993511.jpg)

了解HDCP对于处理HDMI的异常现象很有帮助，比如说如果时而出现雪花，有可能是信号不好导致Channel0的0像素出错，从而第二阶段的认证有时会不成功...



# HDMI Sink总流程

![](https://images0.cnblogs.com/i/421096/201407/150012568498487.jpg)


- 提取与分割10bit的TMDS串行数据
- 10bit的数据，通过不同线路进来的，判断是哪种类型的：DE，Data Island，RGB，Hs，Vs，Ctrl
- TMDS解码
- HDCP解码，同时Hs，Vs，DE做delay
- RGB与DE，Hs，Vs...
- BCH解码得到Packet，错误验证
- Packet含义解析
- 如果是信息，则存到内存
- 如果是Audio Data，生成采样频率
- Audio输出












