# BREW中的特殊语音处理 - 我相信...... - CSDN博客





2010年11月04日 11:36:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：945标签：[brew																[服务器																[算法																[手机																[2010](https://so.csdn.net/so/search/s.do?q=2010&t=blog)




从 BREW2.X 开始，针对语音处理提供了 IVocoder 接口。 IVocoder 接口使用默认的声音路径，以捕获和播放声码器帧。由于 IVocoder 使用的设备也可以用来执行语音呼叫，因此使用 IVocoder 的应用程序必须注意并集成 TAPI 通知，以便在启动语音呼叫时适时停止使用声码器。 IVocoder 接口实现的基本流程如下：

1 ）创建 IVocoder 接口，从系统层面上来看，这是给 IVocoder 分配一段内存空间，内存空间的使用根据应用的需要来设置。

2 ）配置 IVocoder 的用例，包括声码器算法、数据速率、回调参数等。

增强可变速率声码器 EVRC （ IS-127 ）可以很好的提高语音质量。实时的数据速率实质上由手机底层的声码器根据信噪比和语音空闲度控制，在程序里无法强制其按某速率进行编码，但 BREW 仍要求设置上限和下限速率。

当与调用声码器相关的事件发生时，都要向系统注册一个相关的回调函数在系统执行队列中排队。当系统执行到该回调函数时，系统可以根据回调函数的内容来决定是否继续、中断或者结束声码器的操作，当然决定的过程也是新事件产生的过程，也就意味着又注册一个

回调函数。也就是说，声码器的回调函数是用户自定义但却是系统自动调用的函数。回调函

数的配置好坏，直接关系着整个语音的流程顺畅与否。

3 ）启动并进行语音的编码。

由接口函数 IVOCODER_VocInStart() 启动声码器进行编码语音帧；如果成功则调用已经配置的回调函数 HavaDataCB 。回调函数 HavaDataCB 中，用接口函数 IVOCODER_VocInRead() 一帧帧的采集语音并把数据写入缓存区。然后打包准备发往服务器端。缓冲区大小必须足以支持声码器的最大帧速率，而不仅仅是当前速率限制所用的最大值。

由接口函数 IVOCODER_VocOutStop() 停止声码器的播放，然后停止语音的处理，并释放 IVocoder 接口。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/09/18/5892552.aspx](http://blog.csdn.net/wireless_com/archive/2010/09/18/5892552.aspx)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




