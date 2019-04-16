# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之65---BREW 应用中的音频播放 - 我相信...... - CSDN博客





2010年07月18日 12:15:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1939








BREW中支持的音频格式主要分为两类：基于MIDI的文件格式和特殊编码的音频文件格式。基于MIDI的文件格式有.mid,.dls..xmf和.pmd，其中的DLS文件是由设备定义的，自身是不能被播放的，XMF文件将MIDI和DLS结合起来进行播放。PMD文件与XMF类似是一种文件容器，可以包含MIDI, DLS和其他编码格式的音频，还可以包含图像，文本，LED或者振动同步信息等。编码的音频格式主要有QCELP,PCM和ADPCM三种，QCELP一个语音压缩的编译码器，并不适合音乐的播放。PCM是一种16 bit未压缩的波形抽样原始数据，非常适合播放音乐，但文件的尺寸非常大。ADPCM 只有四分之一的PCM的文件大小，比较适合手机上的音乐和声音效果。就采样率而言，QCELP只是8kHz 的采样率，而PCM支持4, 8, 11.025, 12, 16, 22.05, 24, 32, 44.1kHz的采用率，ADPCM支持从4.0 到44.1kHz的任何采用率。

以播放QCP文件为例，播放一个音频文件非常简单，首先创建AEECLSID_MEDIAQCP的IMedia接口，然后调用IMEDIA_SetMediaData()加载音频文件，最后调用IMEDIA_Play() 播放即可。

在循环播放的时候，在文件播放结束和文件重新播放之间通常会存在短暂的空白，BREW 3.1.5及以后的版本支持QCP, PCM和ADPCM的无间隙播放。例如，在加载QCP文件之前先调用IMEDIA_SetMediaParm(pIMediaQCPObject,MM_PARM_PLAY_REPEAT, repeatCount, 0)和IMEDIA_EnableChannelShare()，然后在调用IMEDIA_Play() 就可以实现QCP音频效果的无间隔循环播放。

在某些情况下，需要实现混合的音频效果。在BREW 2.1及以后的版本中，实现了4 个MIDI文件的混合，4 QCP文件的混合或者4 个ADPCM 文件的混合。需要注意的是，混合的文件必须为相同的文件格式，例如全部的QCP文件或者全部的ADPCM文件，而不能将QCP文件与PCM或者ADPCM文件进行混频，只有在07年以后（在中国市场）的部分手机才开始支持不同音频格式文件的混合播放。音频混合播放的过程与无间隙循环播放的过程类似，首先创建接口，然后通过IMEDIA_SetMediaData()来加载每一个音频文件，并且在创建媒体对象后立即调用IMEDIA_EnableChannelShare() ，最后就能够使用IMEDIA_Play()来进行混音播放了。3.1.4 以前的版本IMEDIA_EnableChannelShare()可能会有问题。另外，使用AEECLSID_MEDIAMIDIOUTQCP创建的接口可以兼顾QCP或者ADPCM文件。

实现混音播放的另一种简单方法就是将所需的各种音频文件嵌入到一个CMX的文件中，然后只播放这个CMX文件就可以了。这种方法的局限在于需要手机对CMX的支持。

可下载的音频（DLS）实际上是一种面向MIDI合成器的定制化波形表，可以通过MIDI指令来完成播放或停止，定调和音量调整等操作。在CMX 4.4 及其以后版本中，这种可下载的音频也可以嵌入到 .pmd 或者 .xmf 文件中。在SDK 3.1.4 之后的版本中，可以通过IDLS API 来加载或卸载这种可下载的音频。表14-2介绍了DLS的音效类型及适用的情形。
|类型|描述|示例|DLS 优势|
|----|----|----|----|
|单次音效|在最大的采样周期长度内播放音效|开枪的声音t开关门的声音|音准的确定可以通过按上下键来实现|
|循环音效|一个连续声音效果的循环播放|骏马奔驰的声音，溪水潺潺的声音|音准可变，可以保持在文件中|
|片段音效|将较短的音效循环播放，或者合成为新的音效|物品肢解的声音，机械引擎的声音|音准可变，可以保持在文件中，能够通过基本的音效产生较为复杂的音效|

**表**** 14-2****：**DLS的音效类型及适用的情形

使用IDLS的时候，首先要创建DLS 的对象:

ISHELL_CreateInstance(m_pIShell, AEECLSID_MEDIADLS,(void**)&m_pIDLS); 

接下来设定DLS 文件的媒体数据属性：

AEEMediaData MediaData;

MediaData.clsData = MMD_FILE_NAME;

MediaData.pData = DLS_FILE;

MediaData.dwSize = 0;

然后初始化DLS的回调函数，并加载DLS文件

CALLBACK_Init(&m_cbDLSLoad, LoadDLSCB, (void*)this);

iReturn = IDLS_Load(m_pIDLS, &MediaData, &m_cbDLSLoad, &m_LoadDLSRet);

如果DLS是全局性的加载，就可以使用IMediaMIDI的接口对象来播放MIDI文件了

boolean fIsGlobal = false;

iReturn = IDLS_IsGlobal(m_pIDLS, &fIsGlobal);

if(fIsGlobal){

PlayOneMidi(MIDI_FILE, &m_pIMediaMIDI);

return;

}

由于XMF/DLS并没有应用的很广泛，所以需要专门的工具来制作和生成此类文件，例如

Faith West Inc.的 “mXMFTool”工具等。

对中断和恢复操作时，应用程序的声音在被中断后要求能重新播放，但在某些手机可能无法恢复声音。这其中最主要的原因就是,当该手机接到一条短消息同时会产生提示用户接收的声音,而这种声音是不能在应用中禁止的，因为这种提示短消息的音乐会占用手机中声音资源的芯片。某些手机在收到短消息不能触发EVT_APP_SUSPEND 事件, 所以应用程序就不知道应用中所使用的声音已停止播放。通常情况下有两种解决办法,一种是设定计时器,让应用程序阶段性的设定音乐。以下就是用这种方法时应用所作的处理:

if (pApp - >J udgeMusicOpen) / / 判断要播放的音乐

{MusicPlayer (pApp ,pApp - > m- p ISoundPlayer , “musicname”) ;/ / 播放当前音乐

/ / 在音乐播放完后继续播放

ISHELL - Set Timer ( pApp - > a. m- p IShell ,3000 , (PFNNOTIFY) BackMusicPlay , (void 3 ) pApp) ; }

在应用程序中断时作的操作是IsoundPlayer_Stop(m-pISoundPlayer) ;恢复时的操作是

if (pApp - > J udgeMusicOpen) / / 判断要播放的音乐

BackMusicPlay (pApp - > m-p ISoundPlayer) ;/ / 循环播放

另一种办法就是调用回调函数如IMEDIA_RegisterNotify或ISOUNDPLAYER_ RegisterNotify。当音乐被短消息中断时,应用会得到AEE_SOUNDPLAYER_DONE 或MM_STATUS_DONE 状态的提示,一旦获得这种状态提示,就需要重新播放。特殊的情况，应用程序被中断后音乐不能在某些手机播放,用上述两种方法可以解决这些问题。



