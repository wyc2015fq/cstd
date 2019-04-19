# Alsa驱动分析（转） - maopig的专栏 - CSDN博客
2013年04月11日 23:49:11[maopig](https://me.csdn.net/maopig)阅读数：1292
个人分类：[音频](https://blog.csdn.net/maopig/article/category/920338)
[1.Abstract](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039743)
[2.Introduction](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039744)
[3.音频驱动框架介绍](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039745)
[3.1音频设备的注册](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039746)
[3.2音频驱动的注册](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039747)
[3.2.1Probe函数的调用](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039748)
[3.2.2Soc_probe函数](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039749)
[4.通常的使用流程的分析](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039750)
[4.1.1open过程介绍](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039751)
[4.1.2snd_pcm_hw_params流程分析](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039752)
[4.1.3prepare流程分析](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039753)
[4.1.4write的流程](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039754)
[4.1.5使用流程的总结t](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039755)
[5.Amixer调用的相关逻辑](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039756)
[5.1.1Amixer调用的上层逻辑](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039757)
[5.1.2Amixer的内核流程](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039758)
[6.总结](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039759)
[7.未讨论](http://blogold.chinaunix.net/u2/67984/showart_2136624.html#_Toc250039760)
# 1.Abstract
*主要是讲2.6.21内核里面的alsa驱动的架构，以及在我们的平台上需要注意的东西。.*
# 2.Introduction
分成几个部分:
驱动整体框架，一个简单的播放流程介绍，以及我们的平台需要注意的地方；
# 3.音频驱动框架介绍
## 3.1音频设备的注册
![Alsa驱动分析（转） - wilson - Wilsons blog](http://blog.chinaunix.net/photo/67984_091231164436.jpg)
这就是设备的注册了，设备本身非常简单，复杂的是这个设备的drvdata，drvdata里面包含了三部分，关于machine的，关于platform的，关于codec的，从大体上说machine主要是关于cpu这边的也可以说是关于ssp本身设置的，而platform是关于平台级别的，就是说这个平台本身实现相关的，而codec就是与我们所用的音频codec相关的；基本上这里就可以看出整个音频驱动的架构特点，就是从alsa层进入——>内核alsa层接口->core层，这里再调用上面说的三个方面的函数来处理，先是cpu级别的，再是platform的，再是codec级别的，这几层做完了，工作也就做得差不多了，后面会详细讲讲，当然这个执行顺序不是固定的(不知道是不是marvel写代码不专业导致的)，但多半都包括了这三部分的工作;
## 3.2音频驱动的注册
### 3.2.1Probe函数的调用
![Alsa驱动分析（转） - wilson - Wilsons blog](http://blog.chinaunix.net/photo/67984_091231164410.jpg)
前面讲了设备的注册，里面的设备的名字就是”soc-audio”,而这里的driver的注册时名字也是”soc-audio”，对于platform的设备匹配的原则是根据名字的，所以将会匹配成功，成功后就会执行audio驱动提供的probe函数soc_probe;
### 3.2.2Soc_probe函数
这个函数本身架构很简单，和前面说的逻辑一样，先调用了cpu级别的probe，再是codec级别的，最后是platform的（这里三个的顺序就不一样），但是因为cpu级别的和platform级别的都为空，最后都调用了codec级别的probe函数，也就是micco_soc_probe，这个函数基本上就完成了所有应该完成的音频驱动的初始化了；简单的划分，分成两部分，对上和对下：对上主要是注册设备节点，以及这些设备节点对应的流的创建；对下主要是读写函数的设置，codec本身的dai设置，初始化寄存器的设置，最重要的就是后面的control的创建和门的创建了，如下图所示：
![Alsa驱动分析（转） - wilson - Wilsons blog](http://blog.chinaunix.net/photo/67984_091231164423.jpg)这里面的第一部分就是负责初始化的；
第二部分就是创建卡和流，对于alsa驱动来说，是先分成卡0，卡1…,然后对于每一个卡的每一个cpu支持的dai（digit
 audio interface）也就是pcm接口或者i2S接口等都要建立对应的流，一个dai有可能包含两个流，一个是录的一个是play的，但在我们的平台上对于i2S的dai是没有录音功能的，所以我们的平台只有一个卡，三个流，pcm的录和play，i2S的play；流的创建还是更多的考虑为上层服务的，它所提供的接口都是soc层的，这里非常重要的地方在于驱动的一个典型做法那就是如何把关键的内核数据结构和export到外部的/dev下的设备节点实现关联，比如:
**关键数据结构struct snd_pcm，是根据cpu所固有的dai创建的，而对于每一个struct snd_pcm又可能用到两个substream（它们实现具体的流的播放等），它们之间的链接是通过它的内部数据成员struct snd_pcm_str streams[2];来连接的，而这个snd_pcm类型的指针是在函数snd_device_new里面通过device_data放到设备里面的，这个设备会在snd_device_register_all**
**的时候注册到/dev下面，并且调用dev_set_drvdata(preg->dev, private_data);来把这个指针放到设备的私有数据里面；而在需要使用的时候通过snd_pcm_playback_open里面的snd_lookup_minor_data函数取得其私有数据并返回的，这样就实现了设备节点和对应的驱动的数据结构的关联，这是一种非常普遍的做法；有了这个数据结构它就可以根据一定的原则取得对应于这个需求的substream，于是一切的操作都可以交给这个substream了；**
第三部分就是control的创建，这个函数比较简单，就是把表micco_snd_controls里面已经定义好的controls模板创建controls，然后加入到card的controls列表中去；本身功能很清晰，但是对于我们平台来说，**需要非常小心，因为这里决定了各个****controls的序号，而这个序号是audio_controller访问硬件的索引，所以千万要小心尽量要维持目前的controls的序号，如果要额外添加新的controls一定要记得要放在micco_add_widgets后面来做，这样可以做到兼容，否则audio_controller的工作量就大了**！
第四部分就是门的创建了，这个函数也是很清楚，就是把codec对应的门都加入到codec->dapm_widgets列表中去（这里的门的概念可以简单的理解为水管与水管之间的连接的地方，声音数据像水一样从水管里面流出来，源头可以是CPU了，也可以是modem，然后通过不同的门，流向不同的地方，比如speaker，比如蓝牙耳机等等），然后根据micco_audio_map把所有可能连在一起的门连接起来，这个表micco_audio_map的意思是{目的名字，控制点名字，源头名字}，然后函数snd_soc_dapm_connect_input会根据这些名字去查表codec->dapm_widgets(先前已经把所有的门都加入了)找到它们再根据不同的类型做不同的连接，比如是mux之间的连接，mux和pga之间的连接等等，注意这里的连接其实只不过是说找到连接的可能性，它对于不同的门，找到其可能的source和sink，加入到对应的列表中去，具体细节如下：
**首先，扫描整个codec所拥有的所有的门，如果它的名字和传入的sink的名字相同，则认为它就是这个路径的sink，如果它的名字和传入的source名字相同，则认为它是这个路径的source，如果源头或者sink没有找到都返回错误；然后分配一个struct snd_soc_dapm_path，这个数据结构的主要成分包括名字，source门，sink门，这条路径的control，这个源头和sink是否已经连接，是否已经走过（用在后面），这个数据结构会被挂在三个链表里面，一个是source的就是这个门会在很多的路径中，把它在这个路径中做source的path都连在一起，一个是sink的就是把这个门在所有这些由它做sink的path都连接在一起，一个是把所有的路径都需要连接在一起的这个是通过codec的dapm_paths来访问的；**
**list_add(&path->list, &codec->dapm_paths);**
**list_add(&path->list_sink, &wsink->sources);**
**list_add(&path->list_source, &wsource->sinks);**
**需要注意的时候，这里把路径的list_sink加入到了wsink门的sources列表里面，而把路径的list_source加入到wsource的sinks列表里面，所以当访问的时候从wsink门的sources出发就可以找到连接这个门作为sink的所有的路径，而从wsource的sinks列表出发就可以找到所有以这个门作为source的路径；**
**第三步就是为这个数据结构赋值：source，sink，初始化三个链表；第四步：如果control为空则把这个路径加入到相应的三个链表中去，并且路径设为已经连接，并返回；第五步：否则，根据sink的类型，如果是adc，dac，input，output，micbias，vmid，pre，post，则把路径加入到三个链表，设置已经连接的标志；如果是snd_soc_dapm_mux则调用dapm_connect_mux来处理；如果是mixer和switch则调用dapm_connect_mixer来处理，如果是hp，mic，line，spk，则把path加入到三个链表中去，但是设置成为连接的状态。**
**大约就是这样的了。**
**也许您要问，为什么要这么做呢？**
**这个，我也有想过，甚至我认为在门比较少的时候，确实没什么必要，但是这么做的好处在于当要播放音乐的时候，它可以实现自动的寻找路径并且自动poweron那些门，不需要上层做任何的控制，因为它真的到达目的地的所有的路径，这样它可以自己选择走哪条路；如果不这么连接起来的话，就需要提供给上层连接的接口，完全由上层来决定该连接哪些门，也必须由上层来负责poweron和off这些门；**
第五部分就是注册了，这里就是向/dev注册设备节点，因为这些设备节点会由alsa层来访问的，这些设备节点和驱动的连接我前面已经说了，主要是提供了对上的alsa接口，给alsa层调用。
# 4.通常的使用流程的分析
通常使用alsalib来播放声音包括以下几个步骤：
1，open,这个和oss相同，对应于alsa就是snd_pcm_open；
2，param设置，这个就是snd_pcm_hw_params;
3，上层的alsa在设置param的成功以后或者出错的时候恢复都需要调用snd_pcm_prepare;
4，write函数；
现在一个个的来看；
### 4.1.1open过程介绍
如下图所示：![Alsa驱动分析（转） - wilson - Wilsons blog](http://blog.chinaunix.net/photo/67984_091231164352.jpg)
就是我先前说的分成三部分，先是cpu级别的，包括clock的enabe，中断的申请，空间的申请；
然后就是平台级别的包括DMA所需要的空间的分配等；
不过这里codec级别的没有提供相关的函数，由machine提供了一些函数主要是设置channel，格式，频率范围等等；
### 4.1.2snd_pcm_hw_params流程分析
![Alsa驱动分析（转） - wilson - Wilsons blog](http://blog.chinaunix.net/photo/67984_091231164312.jpg)
流程就是这样，至于里面做的具体的事情，我觉得只需要对照spec看看就知道了，具体的寄存器设置下面有一点讲解，主要是格式的设置（采样率的设置会留到prepare的时候），至于中断上来的时候那个更新hw_ptr函数很有用，这样上层就可以知道数据到底写了多少或者说还有多少空间可以写；
### 4.1.3prepare流程分析
当alsa层调用snd_pcm_prepare的时候会触发驱动对应的prepare的函数执行,如下：
![Alsa驱动分析（转） - wilson - Wilsons blog](http://blog.chinaunix.net/photo/67984_091231164333.jpg)可以看出基本上还是分为了三段，一段是cpu级别的，主要是对于ssp
 port的设置，具体设置如下：
**对于voice通道，littleton_micco_voice_prepare的设置：**
**the sscr0 0xc0163f,sscr1 0xf01dc0,sspsp 0x800085**
**其中对于pcm的ssp地址是：**
**#define SSCR0_P4        __REG(0x41A00000)  /* SSP Port 4 Control Register 0 */**
**#define SSCR1_P4        __REG(0x41A00004)  /* SSP Port 4 Control Register 1 */**
**#define SSPSP_P4        __REG(0x41A0002C)  /* SSP Port 4 Programmable Serial Protocol */**
**所以结果就相当于：**
** SSCR0_P4 0x41A00000：0xc0163f ——》0000，0000，1100，0000，0001，0110，0011，1111**
**对于这个地址高8位为0，**
**31 （MOD）-》0：普通模式；30（ACS）-》0：时钟选择是由NCS和ECS位绝决定，看后面；**
**29（FPCKE）-》0：FIFO packing mode disabled；28（）-》0：reserved**
**27（52MM）-》0：13mbps模式；26：24（FRDC）-》0：每帧的时隙数**
**23（TIM）->1:表示禁止传输fifo underrun中断；22（RIM）-》1：表示禁止接收fifo overrun中断**
**21（NCS）->0:表示时钟选择由ECS决定；20（EDSS）-》0：表示前面填充DSS来达到8-16位**
**19：8（SCR）-》0x16:决定串口bit率，=sspx clock/(scr+1)？？？；7（SSE）-》0：表示disable port**
**6（ECS）-》0：表示片内的时钟用来计算serial clock rate；5：4（FRF）-》0b11：表示psp模式用来模拟I2S协议**
**3：0（DSS）-》0b1111：表示16bit数据（EDSS为0）**
**SSCR1_P4 0x41A00004:  0xf01dc0——》0000，0000，1111，0000，0001，1101，1100，0000**
**对于这个地址高8位也为0，**
**31（TTELP）-》0：表示最后一个bit传输（LSB）传完后有半个时钟处于高阻（三态）状态；**
**30(TTE)-》0：表示传输信号不是三态的；29（EBCEI）-》0：bit传输错误不产生中断**
**28（SCFR）-》0：表示SSPSCLK的时钟信号需要连续的工作，主模式ignore；27（ECRA）-》0：表示禁止其它ssp向它发起始终请求**
**26（ECRB）-》0：表示同27；25（SCLKDIR）-》0：表示主模式，SSP端口，驱动SSPSCLK；**
**24（SFRMDIR）-》0：表示主模式，SSP端口，驱动SSPSFRM信号；**
**23（RWOT）-》1：表示只接收不传输？？？；22（TRAIL）-》1：表示trailing bytes 由dma burst来处理；**
**21（TSRE）-》1：表示传输DMA sevice request 是enabled；20（RSRE）-》1：表示接收DMA service request允许**
**19（TINTE）-》0：表示接收超时中断disable；18（PINTE）-》0：表示外设trail byte 中断disable；**
**17：保留；16（IFS）->0:表示帧的极性由PSP的极性位决定；**
**15（STRF）-》0：表示传输FIFO（读，写）由SSDR_X来决定；14（EFWR）-》0：表示FIFO读写特别函数disable**
**13：10（RFT）-》0b0111：表示到达什么级别rxfifo断言中断；9：6（TFT）-》0111:表示TXFIFO断言中断级别**
**5：保留；4（SPH）-》0：表示在每一个帧开始之前要等一个时钟，结束后要等0.5个时钟；**
**3（SPO）-》0：表示SSPSCLK在inactive的时候是低电平；2（LBM）-》0：表示非循环模式**
**1（RIE）-》0：表示RXFIFO门槛到达的中断disable；0（RIE)->0:表示接收FIFO门槛到达中断disable**
**SSPSP_P4 0x41A0002C:  0x800085-》0000，0000，1000，0000，0000，0000，1000，0101**
**这个地址的高8位为0，**
**31：reverved；30：28（EDMYSTOP）-》0：extended dummy stop；**
**27：26（EDMYSTART）-》0：extended dummy start；25（FSRT）-》0：下一帧的开始由前面的扩展STOP决定；**
**24：23（DMYSTOP）-》0b01：表示最后一bit传输完毕后保持active的clock数1clock的延迟；22：保留**
**21：16（SFRMWDTH）-》0：表示最小位帧宽度；15：9（SFRMDLY）-》0：serial frame dealy**
**8：7（DMYSTRT）-》0b01：表示1clock的延迟在开始的时候；6：4（STRTDLY）-》0：start delay**
**3（ETDS）-》0：表示结束时的传输状态为low；2（SFRMP）-》1：serial frame的极性；**
**1：0（SCMODE）-》0b01：data driven 上升沿，数据采样下降沿，idle状态，低；**
**对于littleton_micco_hifi_prepare的设置：**
** The sscr0 e1c0003f,sscr1 701dc0,sspsp 40200004,sstsa 3,ssrsa 3,ssacd 60,ssacdd 6590040**
**其中对于I2s的spp地址是：**
**#define SSCR0_P3  __REG(0x41900000)  /* SSP Port 3 Control Register 0 */**
**#define SSCR1_P3  __REG(0x41900004)  /* SSP Port 3 Control Register 1 */**
**#define SSPSP_P3  __REG(0x4190002C)  /* SSP Port 3 Programmable Serial Protocol */**
**#define SSTSA_P3  __REG(0x41900030)  /* SSP Port 3 Tx Timeslot Active */**
**#define SSRSA_P3  __REG(0x41900034)  /* SSP Port 3 Rx Timeslot Active */**
**#define SSACD_P3 __REG(0x4190003C)  /* SSP Port 3 Audio Clock Divider */**
**#define     SSACDD_P3   __REG(0x41900040)  /* SSP Port 3 Audio Clock Dither Divider Register */**
**SSCR0_P3==__REG(0x41900000):e1c0003f——》1110，0001，1100，0000，0000，0000，0011，1111**
**31 （MOD）-》1：网络模式；30（ACS）-》1：时钟选择是audio clock和audio clock divider决定，由ssacd寄存器决定；**
**29（FPCKE）-》1：FIFO packing mode enabled；28（）-》0：reserved**
**27（52MM）-》0：13mbps模式；26：24（FRDC）-》1：每帧的时隙数**
**23（TIM）->1:表示禁止传输fifo underrun中断；22（RIM）-》1：表示禁止接收fifo overrun中断**
**21（NCS）->0:这里ignore，由ACS决定了（为1）；20（EDSS）-》0：表示前面填充DSS来达到8-16位**
**19：8（SCR）-》0:由ACS那里决定；7（SSE）-》0：表示disable port，工作时应为1**
**6（ECS）-》0：表示片内的时钟用来计算serial clock rate；5：4（FRF）-》0b11：表示psp模式用来模拟I2S协议**
**3：0（DSS）-》0b1111：表示16bit数据（EDSS为0）**
**SSCR1_P3==__REG(0x41900004):701dc0——》0000，0000，0111，0000，0001，1101，1100，0000**
**31（TTELP）-》0：表示最后一个bit传输（LSB）传完后有半个时钟处于高阻（三态）状态；**
**30(TTE)-》0：表示传输信号不是三态的；29（EBCEI）-》0：bit传输错误不产生中断**
**28（SCFR）-》0：表示SSPSCLK的时钟信号需要连续的工作，主模式ignore；27（ECRA）-》0：表示禁止其它ssp向它发起始终请求**
**26（ECRB）-》0：表示同27；25（SCLKDIR）-》0：表示主模式，SSP端口，驱动SSPSCLK；**
**24（SFRMDIR）-》0：表示主模式，SSP端口，驱动SSPSFRM信号；**
**23（RWOT）-》0：接收和传输都可以；22（TRAIL）-》1：表示trailing bytes 由dma burst来处理；**
**21（TSRE）-》1：表示传输DMA sevice request 是enabled；20（RSRE）-》1：表示接收DMA service request允许**
**19（TINTE）-》0：表示接收超时中断disable；18（PINTE）-》0：表示外设trail byte 中断disable；**
**17：保留；16（IFS）->0:表示帧的极性由PSP的极性位决定；**
**15（STRF）-》0：表示传输FIFO（读，写）由SSDR_X来决定；14（EFWR）-》0：表示FIFO读写特别函数disable**
**13：10（RFT）-》0b0111：表示到达什么级别rxfifo断言中断；9：6（TFT）-》0111:表示TXFIFO断言中断级别**
**5：保留；4（SPH）-》0：表示在每一个帧开始之前要等一个时钟，结束后要等0.5个时钟；**
**3（SPO）-》0：表示SSPSCLK在inactive的时候是低电平；2（LBM）-》0：表示非循环模式**
**1（RIE）-》0：表示RXFIFO门槛到达的中断disable，0（RIE)->0:表示接收FIFO门槛到达中断disable**
**SSPSP_P3==__REG(0x4190002C):40200004——》0100，0000，0010，0000，0000，0000，0000，0100**
**31：reverved；30：28（EDMYSTOP）-》4：extended dummy stop；**
**27：26（EDMYSTART）-》0：extended dummy start；25（FSRT）-》0：下一帧的开始由前面的扩展STOP决定；**
**24：23（DMYSTOP）-》0b00：表示最后一bit传输完毕后保持active的clock数的延迟；22：保留**
**21：16（SFRMWDTH）-》0b20：表示最小位帧宽度；15：9（SFRMDLY）-》0：serial frame dealy**
**8：7（DMYSTRT）-》0b00：表示0clock的延迟在开始的时候；6：4（STRTDLY）-》0：start delay**
**3（ETDS）-》0：表示结束时的传输状态为low；2（SFRMP）-》1：serial frame的极性；**
**1：0（SCMODE）-》0b00：data driven 下降沿，数据采样上升沿，idle状态，低；**
**SSTSA_P3==__REG(0x41900030):3——》0011**
**31：8->0:reserved;7:0（TTSA）->0b0011:表示在那个time slot里面是传输数据的0，不传输，1传输；**
**SSRSA_P3==__REG(0x41900034):3——》0011**
**31：8 ：reserved；7：0（RTSA）-》0：表示在那个slot里面接收数据，0，不接受，1 接收；**
**SSACD_P3==__REG(0x4190003C):60——》0110，0000**
**31：8：reserved；7（SCDX8）-》0：sysclk/4产生内部audio clock，1，sysclk/8产生audio clock；**
**6：4（ACPS）-》0b110:PLL输出时钟由Audio clock dither Divider register value决定；**
**3（SCDB）-》0：如果SCDX8为0则scdx8决定，为1，则sysclk不分频；**
**2：0（ACDS）-》0：表示clock divider select 为/1；**
**SSACDD_P3==__REG(0x41900040):6590040——》0000，0110，0101，1001，0000，0000，0100，0000**
**31：reserved；30：16（NUM）-》1625;除数（0x659）**
**15：12：reserved；11：0（DEN）-》64：被除数**
**比如我们的板子上是这样计算这些值的：**
**比如，在我们的机子上的一个实例是这样的，**
**那么第一步取得采样率：48K，它也就是Sync clock；**
**第二步球的bit率：48X64=3.072M**
**第三步求的sysclk：这个根据scdx8决定是X4还是X8，在我们的例子中是4，所以：3.072X4=12.288**
**第四步求得我们要的dither divider y，公式为：**
**624*(y)/2=12.288，**
**算出y=0.039384615384615384615384615384615**
**所以查可能的分子和分母表，得出，分子是：**
**64，分母是1625**
**如下图所示：**
![Alsa驱动分析（转） - wilson - Wilsons blog](http://blog.chinaunix.net/photo/67984_091231164939.jpg)
当然更加详细的请参阅spec；
第二段是平台级别的，主要是对于DMA的初始化；
第三段是codec级别的，这里主要是对codec本身的设置，通过i2c接口对codec的寄存器操作，比如采样率等等；
最后面还有一个poweron的函数，这个函数前面有提到，但是没有详细分析，这里分析一下：
**首先根据事件类型，决定是关闭门序列，还是启动门序列，我只分析启动过程；**
**得到启动序列，就开始遍历整个序列，对于这个序列的每一个类型，查找所有的门的序列，直到有一个门的类型和当前启动序列的类型相同，然后再根据不同的类型做不同的检查和power：**
**1，如果是snd_soc_dapm_vmid则继续，不做任何处理；**
**2，**
**A）如果是snd_soc_dapm_adc，并且其active为1，这个active在上一步已经分析过了，必须要包含这个流的名字的sname的门才会被激活，假设我们现在讨论的是用pcm通道播放声音，那么流的名字就是“Voice Playback”，所以，将置有dac3的active被设成1，这样就避免了power on dac1，dac2，和adc了。如果这两个条件都满足，那么必然是“Voice Capture”了，因为只有这时候，我们才会用到adc，现在看看，如果用了adc将会启动什么,于是调用函数is_connected_input_ep，这是一个通用递归函数，从名字上来说就是看是否是已经连接了输入的门，我们只考虑adc的情况，其余的情况待会再讨论，对于adc，在is_connected_input_ep函数里面，是通过遍历所有以这个门作为sink的source门（list_for_each_entry(path, &widget->sources, list_sink)，可以看到，这里的最后一个参数是list_sink，而第二个参数却是widget->sources,这个原因我在“门连接分析”页里面已经分析过了，总之sources就表示这个门的sources列表，而sinks就是这个门的sink列表），通过递归调用is_connected_input_ep来查看这些source门是否其中有一个是连通的，返回的是所有是否连通的和（联通为1，否则为0），所以返回的结果可能是大于1的数，表示不只一个源是联通的。**
**B）如果这个函数返回为真则表示此adc是联通的，于是调用dapm_update_bits来处理，这个函数过对mux（它的reg<0)，input，output，mic，hp，line，spk，不做任何处理就返回了；过了这一关，开始查是否men的revert为真，如果为真，则把power取反，原来为真现在变假，于是调用snd_soc_read（micco_soc_read）开始读这个寄存器的值（注意，这里读的值是可能和物理上的这个寄存器的值不一样的，这里读的值是cache里面的值），读出来后强制把1<<shift后的位置为1，比较新旧值是否有变化，如果有，则调用snd_soc_write(codec, widget->reg, new)把值写到cache里面（实际负责写的是micco_soc_write，而且，它对于0x70+0x15以下包含0x15的值是直接写到寄存器的地址的，否则只是些到数组cache里面去）好对于adc的情况我们就分析完了。**
**3，**
**A)如果此类型是snd_soc_dapm_dac并且active为1，则调用is_connected_output_ep来取得是否要power，下面来看看函数is_connected_output_ep，这也是一个通用的判断是否有连接到输出的递归函数，我们只分析dac的情况，list_for_each_entry(path, &widget->sinks, list_source)，上面已经讲过，这里实际上查的是这个门的所有的sink列表，通过递归调用is_connected_output_ep来看是否它的sink是联通的，只要有一条路是联通的，则power为真。**
**B)返回后调用dapm_update_bits来处理，上面已经分析过了，这个函数就是判断是否需要设置此门的寄存器的1<<shift位。**
**4，如果此类型是snd_soc_dapm_pga，则调用is_connected_input_ep来判断是否联通输入，再调用is_connected_output_ep判断是否联通输出，对于pga is_connected_input_ep函数的处理和adc是一样的，对于is_connected_output_ep和dac的处理是一样的，接着调用dapm_set_pga，根据power的值决定是mute pga还是启用pga，但是就我打印的结果来看，基本上这个函数所起的作用为0，因为对于pga的门似乎都没有设置相应的control，最后调用dapm_update_bits，设置power 位。**
**5，对于other widget，这里在我们的平台上多半是指mux，首先调用is_connected_input_ep判断是否连接输入，再调用is_connected_output_ep判断是否有输出，调用dapm_update_bits位设置power 位，最后调用w->event（do_post_event)来进行后期处理，这里主要就是对mux进行寄存器设置，因为mux的寄存器的地址都是大于0x70+0x15的，所以它们的地址需要转化，这个函数就是根据mux的类型，访问不同的寄存器。**
基本上prepare后，一切就都就绪了，只等一个trigger；而trigger的执行会在上层的alsalib调用write的函数触发；
### 4.1.4write的流程
![Alsa驱动分析（转） - wilson - Wilsons blog](http://blog.chinaunix.net/photo/67984_091231164300.jpg)
用户层的write到内核里面都是通过ioctl来做的，这里面会触发trigger函数的执行，等trigger执行完以后，才会真正调用函数把用户层的东西copy到dma分配的空间；
这里面基本上只是画了最简单的逻辑，其实里面非常的复杂特别是函数snd_pcm_lib_write1，这里面有同步的操作，也就是要等到有空余的空间的时候才允许写，否则就要等待，唤醒是通过函数snd_pcm_update_hw_ptr_post来做的，这个函数会在DMA传输完一帧的中断到来的时候被调用，用来更新缓冲区指针；
其中trigger的逻辑如下：
![Alsa驱动分析（转） - wilson - Wilsons blog](http://blog.chinaunix.net/photo/67984_091231164249.jpg)
简单的说就是启动DMA，enable ssp口；
### 4.1.5使用流程的总结t
简单总结一下，用户的使用流程；
         A,调用snd_pcm_open打开设备节点对应的pcm流的substream也就是录音或者play；
B,调用snd_pcm_hw_params设置硬件参数，包括格式，通道，采样率，DMA空间的分配，中断的申请等等，这里面会调用prepare函数使硬件准备好硬件，包括codec的寄存器设置，各种路径的建立，门的power
 on等；
C,调用write函数实现把数据写到设备里面去，这里会触发trigger函数也就是DMA的启动，SSP端口的启动等。
# 5.Amixer调用的相关逻辑
我们的audio controller所调用的驱动的接口都是amixer 的cset，cget，所以有必要分析一下它的逻辑：
### 5.1.1Amixer调用的上层逻辑
![Alsa驱动分析（转） - wilson - Wilsons blog](http://blog.chinaunix.net/photo/67984_091231164237.jpg)
也就是说通过/dev下面的设备节点调用相应的ioctl，然后进入到内核的范围；
### 5.1.2Amixer的内核流程
![Alsa驱动分析（转） - wilson - Wilsons blog](http://blog.chinaunix.net/photo/67984_091231164223.jpg)
这里只分析了控制函数为snd_soc_dapm_put_enum_double的处理逻辑，其它的都类似，而具体的应该是哪个处理函数来处理是在control的new的时候就已经确立了的，对于我们的平台其实在表micco_dapm_widgets建立的时候就已经确立了；
为了方便后来者的调试，我这里把各个numid的对应的控制函数都列出来了,如下：
**numid=1到12：snd_soc_put_volsw；**
**numid=13到20：snd_soc_dapm_put_enum_double**
.
# 6.总结
Alsa驱动的架构主要是分成对上为alsalib提供接口，对下实现硬件的管理，对上的内容基本都是在sound/core目录里面的文件来完成，而设计硬件的操作分成两部分一部分相关与cpu这边的是由sound/soc/pxa目录里面的文件来完成的，另外一部分设计codec是由sound/soc/codec来完成的，这部分主要就是对codec这边的寄存器的设置；简单示意如下：
![Alsa驱动分析（转） - wilson - Wilsons blog](http://blog.chinaunix.net/photo/67984_091231164205.jpg)
它复杂的地方在于用户态的alsa lib。
# 7.未讨论
还有一些地方没有讨论到，比如timer，不过留到以后补充吧
备注：
内核版本：2.6.21（+marvel patch）
硬件平台：pxa310+9034codec；
 作者：wylhistory
转载请注明出处！
