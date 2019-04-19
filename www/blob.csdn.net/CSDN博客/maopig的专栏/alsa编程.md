# alsa编程 - maopig的专栏 - CSDN博客
2013年03月13日 09:27:51[maopig](https://me.csdn.net/maopig)阅读数：1216
个人分类：[音频](https://blog.csdn.net/maopig/article/category/920338)
### [alsa 编程](http://blog.csdn.net/q553716434/article/details/7881546)
分类： [linux](http://blog.csdn.net/q553716434/article/category/813203)2012-08-18 20:13
124人阅读[评论](http://blog.csdn.net/q553716434/article/details/7881546#comments)(0)[收藏]()[举报](http://blog.csdn.net/q553716434/article/details/7881546#report)
[编程](http://blog.csdn.net/tag/details.html?tag=%E7%BC%96%E7%A8%8B)[parameters](http://blog.csdn.net/tag/details.html?tag=parameters)[buffer](http://blog.csdn.net/tag/details.html?tag=buffer)[loops](http://blog.csdn.net/tag/details.html?tag=loops)[access](http://blog.csdn.net/tag/details.html?tag=access)[playback](http://blog.csdn.net/tag/details.html?tag=playback)
转载自：[http://blog.csdn.net/spygg/article/details/7824750](http://blog.csdn.net/spygg/article/details/7824750)
ALSA(Advanced Linux Sound Architecture)是由内核驱动,标准的API库和一系列实用程序组成.因为涉及到版权和BUG的问题Linux 2.6内核抛弃了旧的OSS,ALSA作为声音编程的生力军被作为了合并到了内核中.
数字音频基础:
音频是由电器设备(麦克风等)将空气的变化转化成的电信号.模数转换器(A/D)将模拟电压转化成一系列不连续的值称之为采样,然后将采样值送往数模转化器(D/A)从而将声音还原.采样的频率是影响数字声音质量的一个关键因素,由Nyquist采样定理知,采样的频率至少是信号中最高频率的2倍方能的还原原始信号.
ALSA基础知识:
 ALSA由许多声卡的声卡驱动程序组成，同时它也提供一个称为libasound的API库。应用程序开发者应该使用libasound而不是内核中的ALSA接口。因为libasound提供最高级并且编程方便的编程接口。并且提供一个设备逻辑命名功能，这样开发者甚至不需要知道类似设备文件这样的低层接口。相反，OSS/Free驱动是在内核系统调用级上编程，它要求开发者提供设备文件名并且利用ioctrl来实现相应的功能。为了向后兼容，ALSA提供内核模块来模拟OSS，这样之前的许多在OSS基础上开发的应用程序不需要任何改动就可以在ALSA上运行。另外，libaoss库也可以模拟OSS，而它不需要内核模块。ALSA包含插件功能，使用插件可以扩展新的声卡驱动，包括完全用软件实现的虚拟声卡。ALSA提供一系列基于命令行的工具集，比如混音器(mixer)，音频文件播放器(aplay)，以及控制特定声卡特定属性的工具.
ALSA接口:
控制接口:用来管理已注册的声卡并检查其可用的设备
PCM接口:用来管理数字音频的录音和回放,这是一个用的最广泛的接口,我们将在下文中着重介绍.
原始 MIDI 接口:支持标准MIDI(Musical Instrument Digital Interface),提供了访问声卡MIDI的接口.
时间接口:用来声卡的计时声音事件的同步
Sequencer接口:高级MIDI和声音合成接口,可以处理更多的MIDI协议
混音接口:用来声卡设备的信号处理和音量,建立在控制接口之上
设备命名:
 API库使用逻辑设备名而不是设备文件。设备名字可以是真实的硬件名字也可以是插件名字。硬件名字使用hw:i,j这样的格式。其中i是卡号，j是这块声卡上的设备号。第一个声音设备是hw:0,0.这个别名默认引用第一块声音设备并且在本文示例中一直会被用到。插件使用另外的唯一名字。比如plughw:,表示一个插件，这个插件不提供对硬件设备的访问，而是提供像采样率转换这样的软件特性，硬件本身并不支持这样的特性。
声音缓存和数据传输:
  每个声卡都有一个硬件缓存区来保存记录下来的样本。当缓存区足够满时，声卡将产生一个中断。内核声卡驱动然后使用直接内存(DMA)访问通道将样本传送到内存中的应用程序缓存区。类似地，对于回放，任何应用程序使用DMA将自己的缓存区数据传送到声卡的硬件缓存区中。
      这样硬件缓存区是环缓存。也就是说当数据到达缓存区末尾时将重新回到缓存区的起始位置。ALSA维护一个指针来指向硬件缓存以及应用程序缓存区中数据操作的当前位置。从内核外部看，我们只对应用程序的缓存区感兴趣，所以本文只讨论应用程序缓存区。
      应用程序缓存区的大小可以通过ALSA库函数调用来控制。缓存区可以很大，一次传输操作可能会导致不可接受的延迟，我们把它称为延时(latency)。为了解决这个问题，ALSA将缓存区拆分成一系列周期(period)(OSS/Free中叫片断fragments).ALSA以period为单元来传送数据。
      一个周期(period)存储一些帧(frames)。每一帧包含时间上一个点所抓取的样本。对于立体声设备，一个帧会包含两个信道上的样本。图1展示了分解过程：一个缓存区分解成周期，然后是帧，然后是样本。图中包含一些假定的数值。图中左右信道信息被交替地存储在一个帧内。这称为交错(interleaved)模式。在非交错模式中，一个信道的所有样本数据存储在另外一个信道的数据之后。
设置参数，参数设置不当将会导致音频设备无法正常工作。在设置参数前，我们需要了解一下各个参数的含义以及一些基本概念。
样本长度(sample)：样本是记录音频数据最基本的单位，常见的有8位和16位。
通道数(channel)：该参数为1表示单声道，2则是立体声。
桢(frame)：桢记录了一个声音单元，其长度为样本长度与通道数的乘积。
采样率(rate)：每秒钟采样次数，该次数是针对桢而言。
周期(period)：音频设备一次处理所需要的桢数，对于音频设备的数据访问以及音频数据的存储，都是以此为单位。
