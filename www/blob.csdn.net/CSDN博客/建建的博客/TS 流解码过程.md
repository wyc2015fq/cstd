# TS 流解码过程 - 建建的博客 - CSDN博客
2017年11月28日 17:33:44[纪建](https://me.csdn.net/u013898698)阅读数：104
个人分类：[视频编解码学习](https://blog.csdn.net/u013898698/article/category/6703370)

**TS 流解码过程:**
**1. 获取TS中的PAT**
**2. 获取TS中的PMT**
**3. 根据PMT可以知道当前网络中传输的视频（音频）类型（H264），相应的PID，PCR的PID等信息。**
**4. 设置demux 模块的视频Filter 为相应视频的PID和stream type等。**
**5. 从视频Demux Filter 后得到的TS数据包中的payload 数据就是 one piece of PES，在TS header中有一些关于此 payload属于哪个 PES的 第多少个数据包。 因此软件中应该将此payload中的数据copy到PES的buffer中，用于拼接一个PES包。**
**6. 拼接好的PES包的包头会有 PTS，DTS信息，去掉PES的header就是 ES。**
**7. 直接将 被拔掉 PES包头的ES包送给decoder就可以进行解码。解码出来的数据就是一帧一帧的视频数据，这些数据至少应当与PES中的PTS关联一下，以便进行视音频同步。**
**8. I，B，B，P 信息是在ES中的。**
ES是直接从编码器出来的数据流，可以是编码过的视频数据流，音频数据流，或其他编码数据流的统称。ES流经过PES打包器之后，被转换成PES包。PES包由包头和payload组成.
在PES层，主要是在PES包头信息中加入PTS(显示时间标签)和DTS（解码时间标签）用于视频、音频同步。其实，Mpeg-2用于视音频同步以及系统时钟恢复的时间标签分别在ES，PES和TS这3个层次中。在ES层，与同步有关的主要是视频缓冲验证VBV（Video
 Buffer Verifier），用以防止解码器的缓冲器出现上溢或下溢；在PES层，主要是在PES头信息里出现的显示时间标签PTS（Presentation
 Time Stamp）和解码时间标签DTS（Decoding Time Stamp）；在TS层中，TS头信息包含了节目时钟参考PCR（Program
 Clock Reference），用于恢复出与编码端一致的系统时序时钟STC（System Time Clock）。
基本流程如下：首先MPEG-2压缩编码得到的ES基本流，这个数据流很大，并且只是I，P，B的这些视频帧或音频取样信息，然后加入一些同步信息，打包成长度可变长度的数据包PES，原来是流的格式，现在成了数据包的分割形式。同时要注意的是，ES是只包含一种内容的数据流，如只含视频，或只含音频等，打包之后的PES也是只含一种性质的ES,如只含视频ES的PES,只含音频ES的PES等。可以知道，ES是编码视频数据流或音频数据流，每个ES都由若干个存取单元（AU）组成，每个视频AU或音频AU都是由头部和编码数据两部分组成，1个AU相当于编码的1幅视频图像或1个音频帧，也可以说，每个AU实际上是编码数据流的显示单元，即相当于解码的1幅视频图像或1个音频帧的取样。PEG-2对视频的压缩产生I帧、P帧、B帧。把帧顺序I1,P4,B2,B3,P7,B5,B6帧的编码ES，通过打包并在每个帧中插入PTS/DTS标志，变成PES。在插入PTS/DTS标志时，由于在B帧PTS和DTS相等，所以无须在B帧多插入DTS。而对于I帧和P帧，由于经过复用后数据包的顺序会发生变化，显示前一定要存储于视频解码器的从新排序缓存器中，经过从新排序后再显示，所以一定要同时插入PTS和DTS作为从新排序的依据。
其中，有否PTS/DTS标志，是解决视音频同步显示、防止解码器输入缓存器上溢或下溢的关键所在。**PTS表明显示单元出现在系统目标解码器（STD- System Target Decoder）的时间, DTS表明将存取单元全部字节从STD的ES解码缓存器移走的时刻。**视频编码图像帧次序为I1,P4,B2,B3,P7,B5,B6,I10,B8,B9的ES，加入PTS/DTS后，打包成一个个视频PES包。每个PES包都有一个包头，用于定义PES内的数据内容，提供定时资料。每个I、P、B帧的包头都有一个PTS和DTS，但PTS与DTS对B帧都是一样的，无须标出B帧的DTS。对I帧和P帧，显示前一定要存储于视频解码器的重新排序缓存器中，经过延迟（重新排序）后再显示，一定要分别标明PTS和DTS。例如，解码器输入的图像帧次序为I1,P4,B2,B3,P7,B5,B6,I10,B8,B9，依解码器输出的帧次序，应该P4比B2、B3在先，但显示时P4一定要比B2、B3在后，即P4要在提前插入数据流中的时间标志指引下，经过缓存器重新排序，以重建编码前视频帧次序I1,B2,B3,P4,B5,B6,P7,B8,B9,I10。显然，PTS/DTS标志表明对确定事件或确定信息解码的专用时标的存在，依靠专用时标解码器，可知道该确定事件或确定信息开始解码或显示的时刻。例如，PTS/DTS标志可用于确定编码、多路复用、解码、重建的时间。
**PCR**
PCR是TS里面的，即TS packet的header里面可能会有，他用来指定所期望的该ts packet到达decoder的时间，他的作用于SCR类似。
**DTS, PTS**
对于一个ES来说，比如视频，他有许多I,P,B帧，而P, B帧都是以I，P帧作为参考。由于B帧是前向后向参考，因此要对B帧作decode的话，就必须先decode该B帧后面的帧（P,或者I帧），于是，decode的时间与帧的真正的present的时间就不一致了，按照DTS一次对各个帧进行decode，然后再按照PTS对各个帧进行展现。
有时候PES包头里面也会有DTS，PTS，对于PTS来说，他代表了这个PES包得payload里面的第一个完整地audio access unit或者video access unit的PTS时间（并不是每个audio/video access unit都带有PTS/DTS，因此，你可以在PES里面指定一个，作为开始）。
PES包头的DTS也是这个原理，需要注意的是：对于video来说他的DTS和PTS是可以不一样的，因为B帧的存在使其顺序可以倒置。而对于audio来说，audio没有双向的预测，他的DTS和PTS可以看成是一个顺序的，因此可一直采用一个，即可只采用PTS。
