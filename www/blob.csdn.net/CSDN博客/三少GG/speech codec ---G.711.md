# speech codec ---G.711 - 三少GG - CSDN博客
2012年06月18日 16:16:44[三少GG](https://me.csdn.net/scut1135)阅读数：2955
各种各样的编解码在各种领域得到广泛的应用，下面就把各种codec的压缩率进行一下比较，不正确之处望各位同行指正。
Speech codec：
    现主要有的speech codec 有: G.711, G.723, G.726 , G.729, ILBC
                                           QCELP, EVRC, AMR, SMV
   主要的audio codec 有:
                               real audio, AAC, AC3, MP3, WMA, SBC等，各种编解码都有其应用的重点领域。
   本文主要对speech codec相关指标进行总结：
   ITU 推出G.7XX系列的speech codec, 目前广泛应用的有：G.711，G.723, G.726, G.729. 每一种又有很多分支，如G.729就有g.729A, g.729B and g.729AB
  G.711:
            G.711就是语音模拟信号的一种非线性量化，细分有二种:G.711 A-law and G.711 u-law.不同的国家和地方都会选取一种作为自己的标准. G.711 bitrate 是64kbps. 详细的资料可以在ITU 上下到相关的spec，下面主要列出一些性能参数：
    G.711（PCM方式：PCM＝脉码调制 ：Pulse Code Modulation） 
• 采样率：8kHz 
• 信息量：64kbps／channel 
• 理论延迟：0.125msec 
• 品质：MOS值4.10               
 G.723.1:
        G.723.1是一个双速率的语音编码器，是 ITU-T建议的应用于低速率多媒体服务中语音或其它音频信号的压缩算法；
其目标应用系统包括H.323、H.324等多媒体通信系统，目前该算法已成为IP电话系统中的必选算法之一；编码器的帧长为30ms，还有7.5ms的前瞻，编码器的算法时延为37.5ms；编码器首先对语音信号进行传统电话带宽的滤波（基于G.712），再对语音信号用传统8000-Hz速率进行抽样（基于G.711），并变换成16 bit线性PCM码作为该编码器的输入；
在解码器中对输出进行逆操作来重构语音信号；高速率编码器使用多脉冲最大似然量化（MP-MLQ），低速率编码器使用代数码激励线性预测（ACELP）方法，编码器和解码器都必须支持此两种速率，并能够在帧间对两种速率进行转换；
此系统同样能够对音乐和其他音频信号进行压缩和解压缩，但它对语音信号来说是最优的；采用了执行不连续传输的静音压缩，这就意味着在静音期间的比特流中加入了人为的噪声。除了预留带宽之外，这种技术使发信机的调制解调器保持连续工作，并且避免了载波信号的时通时断。
G.726:
       G.726有四种码率：, 32, 24, 16 kbit/s Adaptive Differential Pulse Code Modulation (ADPCM)，最为常用的方式是 32 kbit/s，但由于其只是 G.711速率的一半，所以可将网络的可利用空间增加了一倍。G.726具体规定了一个 64 kbpsA-law 或 µ-law PCM 信号是如何被转化为40, 32, 24或16 kbps 的 ADPCM 通道的。在这些通道中，24和16 kbps 的通道被用于数字电路倍增设备(DCME)中的语音传输，而40
 kbps 通道则被用于 DCME 中的数据解调信号（尤其是4800 kbps 或更高的调制解调器）。 
G.726 encoder 输入一般都是G.711 encoder的输出：64kbps A-law or u-law.其算法实质就是一个ADPCM， 自适应量化算法。     
G.729:
    G..729语音压缩编译码算法
采用算法是共轭结构的代数码激励线性预测(CSACELP)，是基于CELP编码模型的算法；能够实现很高的语音质量（长话音质）和很低的算法延世；算法帧长为10ms，编码器含5ms前瞻，算法时延15ms；其重建语音质量在大多数工作环境下等同于32kb/s的ADPCM（G.726），MOS分大于4.0；编码时输入16bitPCM语音信号，输出2进制比特流；译码时输入为2进制比特流，输出16bitPCM语音信号；在语音信号8KHz取样的基础上，16bit线性PCM后进行编码，压缩后数据速率为8Kbps；具有相当于16：1的压缩率。
    G.729系列在当前的VOIP得到广泛的应用，且相关分支较多，可以直接从ITU网上得到source code 和相关文档。
   G.729（CS-ACELP方式：Conjugate Structure Algebraic Code Excited Linear Prediction） 
• 采样率：8kHz 
• 信息量：8kbps／channel 
• 帧长：10msec 
• 理论延迟：15msec 
• 品质：MOS值3.9 
iLBC(internet low bitrate codec):
         是全球著名语音引擎提供商Global IP Sound开发，它是低比特率的编码解码器，提供在丢包时具有的强大的健壮性。iLBC 提供的语音音质等同于或超过 G.729 和 G.723.1，并比其它低比特率的编码解码器更能阻止丢包。iLBC 以13.3 kb/s (每帧30毫秒)和15.2 kb/s (每帧20毫秒)速度运行，很适合拨号连接。
         iLBC的主要优势在于对丢包的处理能力。iLBC独立处理每一个语音包，是一种理想的包交换网络语音编解码。在正常情况下，iLBC会记录下当前数据的相关参数和激励信号，以便在之后的数据丢失的情况下进行处理；在当前数据接收正常而之前数据包丢失的情况下，iLBC会对当前解码出的语音和之前模拟生成的语音进行平滑处理，以消除不连贯的感觉；在当前数据包丢失的情况下，iLBC会对之前记录下来的激励信号作相关处理并与随机信号进行混合，以得到模拟的激励信号，从而得到替代丢失语音的模拟语音。总的来说，和标准的低位速率编解码相比，iLBC使用更多自然、清晰的元素，精确的模仿出原始语音信号，被誉为更适合包交换网络使用的可获得高语音质量的编解码。
　　此外，大部分标准的低位速率编解码，如G.723.1和G.729，仅对300Hz——3400Hz的频率范围进行编码。在这个频率范围里，用G.711编解码所达到的语音质量，就是传统PSTN网络进行语音通话的效果。
　　iLBC充分利用了0——4000Hz的频率带宽进行编码，拥有超清晰的语音质量，这大大超出传统300Hz——3400Hz的频率范围。
　　广受欢迎的Skype网络电话的核心技术之一就是iLBC语音编解码技术，Global IP Sound称该编码器语音品质优于PSTN，而且能忍受高达30%的封包损失。
　　总的来说，在相同的包交换通信条件下，iLBC的语音质量效果比G.729、G.723.1以及G.711更好，声音更加圆润饱满，且丢包率越高，iLBC在语音质量上的优势就越明显！
　　目前，在国际市场上已经有很多VoIP的设备和应用厂商把iLBC集成到他们的产品中。如：Skype, Nortel等。在国内市场上，目前尚无VoIP厂家正式推出支持“iLBC”的网关设备，迅时公司 率先推出支持“iLBC”的中继网关和IAD设备。
更多资料你链接：
www.itu.int
http://www.ilbcfreeware.org/documentation.html#presentations
http://itbbs-arch.pconline.com.cn/topic.jsp?tid=2648071
http://bbs.sdgb.cn/ShowThread.aspx?PostID=11843
[http://en.wikipedia.org/wiki/G.726](http://en.wikipedia.org/wiki/G.726)
http://www.itu.int/rec/T-REC-G.726/e

分类: [音频算法研究与应用](http://www.cnblogs.com/huaping-audio/category/154217.html)
绿色通道： [好文要顶]()[关注我]()[收藏该文]()[与我联系](http://space.cnblogs.com/msg/send/%E5%88%98%E5%93%81)![](http://static.cnblogs.com/images/icon_weibo_24.png)
![](http://pic.cnblogs.com/face/u31718.jpg)
[刘品](http://home.cnblogs.com/u/huaping-audio/)
[关注 - 1](http://home.cnblogs.com/u/huaping-audio/followees)
[粉丝 - 76](http://home.cnblogs.com/u/huaping-audio/followers)
[+加关注]()
1
0
(请您对文章做出评价)
[«](http://www.cnblogs.com/huaping-audio/archive/2008/06/15/1222694.html)博主前一篇：[月上旬忙加心烦啊](http://www.cnblogs.com/huaping-audio/archive/2008/06/15/1222694.html)
[»](http://www.cnblogs.com/huaping-audio/archive/2008/06/27/1231362.html)博主后一篇：[回绝导师澳洲留学的邀请](http://www.cnblogs.com/huaping-audio/archive/2008/06/27/1231362.html)
posted on 2008-06-20 02:49[刘品](http://www.cnblogs.com/huaping-audio/) 阅读(11320) 评论(28) [编辑](http://www.cnblogs.com/huaping-audio/admin/EditPosts.aspx?postid=1224287)[收藏](http://www.cnblogs.com/huaping-audio/archive/2008/06/19/1224287.html#)
![](http://www.cnblogs.com/huaping-audio/aggbug/1224287.html?type=1&webview=1)
**FeedBack:**
[#1楼](http://www.cnblogs.com/huaping-audio/archive/2008/06/19/1224287.html#1280249)
2008-08-07 09:12 | speech codec[未注册用户]
经过向版主请教与speech codec相关的知识后，真是学到了很多东西啊，版主对speech codec相关的算法和特定的平台移植和优化工作经验十分丰富啊! 
特此回帖表谢意！ 
|[ShouYanWang](http://www.shouyanwang.org/space-uid-1.html)![](http://180.86.140.215/uc_server/avatar.php?uid=1&size=middle)[](http://www.shouyanwang.org/home-spacecp-ac-usergroup-gid-1.html)|[楼主](http://www.shouyanwang.org/thread-1734-1-1.html)![](http://www.shouyanwang.org/static/image/common/online_admin.gif)发表于 2012-5-7 13:42|[只看该作者](http://www.shouyanwang.org/forum-viewthread-tid-1734-page-1-authorid-1.html)|[倒序浏览](http://www.shouyanwang.org/forum-viewthread-tid-1734-extra-page%3D1-ordertype-1.html)|[打印](http://www.shouyanwang.org/forum-viewthread-action-printable-tid-1734.html)今天弄了下android上的G711语音编解码库。首先简单介绍下，G711的压缩比，其实比较低2:1的压缩,一般采样的数据都是一次性的读取320字节,至于为什么是320呢？原因是这样的:语音帧的最小单位是20ms，也就是在8KHZ 16bit的基础上，一帧语音可以播放20ms，这20ms对应的长度就是320字节，按帧的概念来说，就是最少必须编码一帧的数据，也就是达到最小的语音播放单位.320个字节是这么算出来的：8Khz=8000个采样/秒      8采样/毫秒那么20毫秒就是160个采样，又因为每个采样16bit=2byte，所以160个采样就是320字节。上面的基础理论明白了不?因此才代码中我所采用的基本都是一个语音帧的数据，也就是8KHZ,16BIT，320字节，几乎所有的压缩比都是以320为基础点的.采集语音是采用AudioRecord来进行的:  int samp_rate =  8000 ;  int min = AudioRecord.getMinBufferSize(samp_rate,     AudioFormat.CHANNEL_CONFIGURATION_MONO,     AudioFormat.ENCODING_PCM_16BIT);//一个采用占据多少bit,目前设备支持16bit/8bit 2中情况  Log.e(TAG, "min buffer size:"+min);  AudioRecord record = null;  record = new AudioRecord(    MediaRecorder.AudioSource.MIC,//the recording source    samp_rate, //采样频率，一般为8000hz/s     AudioFormat.CHANNEL_CONFIGURATION_MONO, //声音采集通道    AudioFormat.ENCODING_PCM_16BIT,     min);播放声音采用的是:AudioTrack:android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_AUDIO);   int samp_rate = 8000 ;   int maxjitter = AudioTrack.getMinBufferSize(samp_rate,      AudioFormat.CHANNEL_CONFIGURATION_MONO,      AudioFormat.ENCODING_PCM_16BIT);   AudioTrack track = new AudioTrack(AudioManager.STREAM_VOICE_CALL,samp_rate,AudioFormat.CHANNEL_CONFIGURATION_MONO, AudioFormat.ENCODING_PCM_16BIT,     maxjitter, AudioTrack.MODE_STREAM);   track.play();原理几乎一样:本DEMO是边录音边播放的，中间并没有传说中的杂音.![](http://www.shouyanwang.org/static/image/filetype/zip.gif)[G711Codec.zip](http://www.shouyanwang.org/forum-attachment-aid-MjM5fDI5YmQ5ZmU5fDEzNDAwMDY4MTl8MA%3D%3D.html)(92.84 KB, 下载次数: 108)|今天弄了下android上的G711语音编解码库。首先简单介绍下，G711的压缩比，其实比较低2:1的压缩,一般采样的数据都是一次性的读取320字节,至于为什么是320呢？原因是这样的:语音帧的最小单位是20ms，也就是在8KHZ 16bit的基础上，一帧语音可以播放20ms，这20ms对应的长度就是320字节，按帧的概念来说，就是最少必须编码一帧的数据，也就是达到最小的语音播放单位.320个字节是这么算出来的：8Khz=8000个采样/秒      8采样/毫秒那么20毫秒就是160个采样，又因为每个采样16bit=2byte，所以160个采样就是320字节。上面的基础理论明白了不?因此才代码中我所采用的基本都是一个语音帧的数据，也就是8KHZ,16BIT，320字节，几乎所有的压缩比都是以320为基础点的.采集语音是采用AudioRecord来进行的:  int samp_rate =  8000 ;  int min = AudioRecord.getMinBufferSize(samp_rate,     AudioFormat.CHANNEL_CONFIGURATION_MONO,     AudioFormat.ENCODING_PCM_16BIT);//一个采用占据多少bit,目前设备支持16bit/8bit 2中情况  Log.e(TAG, "min buffer size:"+min);  AudioRecord record = null;  record = new AudioRecord(    MediaRecorder.AudioSource.MIC,//the recording source    samp_rate, //采样频率，一般为8000hz/s     AudioFormat.CHANNEL_CONFIGURATION_MONO, //声音采集通道    AudioFormat.ENCODING_PCM_16BIT,     min);播放声音采用的是:AudioTrack:android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_AUDIO);   int samp_rate = 8000 ;   int maxjitter = AudioTrack.getMinBufferSize(samp_rate,      AudioFormat.CHANNEL_CONFIGURATION_MONO,      AudioFormat.ENCODING_PCM_16BIT);   AudioTrack track = new AudioTrack(AudioManager.STREAM_VOICE_CALL,samp_rate,AudioFormat.CHANNEL_CONFIGURATION_MONO, AudioFormat.ENCODING_PCM_16BIT,     maxjitter, AudioTrack.MODE_STREAM);   track.play();原理几乎一样:本DEMO是边录音边播放的，中间并没有传说中的杂音.![](http://www.shouyanwang.org/static/image/filetype/zip.gif)[G711Codec.zip](http://www.shouyanwang.org/forum-attachment-aid-MjM5fDI5YmQ5ZmU5fDEzNDAwMDY4MTl8MA%3D%3D.html)(92.84 KB, 下载次数: 108)|
|----|----|----|
|今天弄了下android上的G711语音编解码库。首先简单介绍下，G711的压缩比，其实比较低2:1的压缩,一般采样的数据都是一次性的读取320字节,至于为什么是320呢？原因是这样的:语音帧的最小单位是20ms，也就是在8KHZ 16bit的基础上，一帧语音可以播放20ms，这20ms对应的长度就是320字节，按帧的概念来说，就是最少必须编码一帧的数据，也就是达到最小的语音播放单位.320个字节是这么算出来的：8Khz=8000个采样/秒      8采样/毫秒那么20毫秒就是160个采样，又因为每个采样16bit=2byte，所以160个采样就是320字节。上面的基础理论明白了不?因此才代码中我所采用的基本都是一个语音帧的数据，也就是8KHZ,16BIT，320字节，几乎所有的压缩比都是以320为基础点的.采集语音是采用AudioRecord来进行的:  int samp_rate =  8000 ;  int min = AudioRecord.getMinBufferSize(samp_rate,     AudioFormat.CHANNEL_CONFIGURATION_MONO,     AudioFormat.ENCODING_PCM_16BIT);//一个采用占据多少bit,目前设备支持16bit/8bit 2中情况  Log.e(TAG, "min buffer size:"+min);  AudioRecord record = null;  record = new AudioRecord(    MediaRecorder.AudioSource.MIC,//the recording source    samp_rate, //采样频率，一般为8000hz/s     AudioFormat.CHANNEL_CONFIGURATION_MONO, //声音采集通道    AudioFormat.ENCODING_PCM_16BIT,     min);播放声音采用的是:AudioTrack:android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_AUDIO);   int samp_rate = 8000 ;   int maxjitter = AudioTrack.getMinBufferSize(samp_rate,      AudioFormat.CHANNEL_CONFIGURATION_MONO,      AudioFormat.ENCODING_PCM_16BIT);   AudioTrack track = new AudioTrack(AudioManager.STREAM_VOICE_CALL,samp_rate,AudioFormat.CHANNEL_CONFIGURATION_MONO, AudioFormat.ENCODING_PCM_16BIT,     maxjitter, AudioTrack.MODE_STREAM);   track.play();原理几乎一样:本DEMO是边录音边播放的，中间并没有传说中的杂音.![](http://www.shouyanwang.org/static/image/filetype/zip.gif)[G711Codec.zip](http://www.shouyanwang.org/forum-attachment-aid-MjM5fDI5YmQ5ZmU5fDEzNDAwMDY4MTl8MA%3D%3D.html)(92.84 KB, 下载次数: 108)| | |

