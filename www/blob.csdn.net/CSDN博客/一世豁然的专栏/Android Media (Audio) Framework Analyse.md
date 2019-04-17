# Android Media (Audio) Framework Analyse - 一世豁然的专栏 - CSDN博客





2017年05月23日 22:11:41[一世豁然](https://me.csdn.net/Explorer_day)阅读数：740标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









本博客转载于：[http://blog.csdn.net/myzhzygh/article/details/7429687#comments](http://blog.csdn.net/myzhzygh/article/details/7429687#comments)









## **1 Android多媒体框架结构**



[Android](http://lib.csdn.net/base/android)多媒体系统纵向跨越了Android系统的所有4个层次：[Java](http://lib.csdn.net/base/javase)应用程序层、Java框架层、本地代码层、[Linux](http://lib.csdn.net/base/linux)驱动层。多媒体本地代码层是多媒体系统的重点。

Android媒体播放器的模块结构如图1所示。

![](https://img-my.csdn.net/uploads/201301/05/1357387292_7656.jpg)


从上图可以看到，Android系统中多媒体框架的分层结构。

Framework层，对外提供构建与媒体相关应用程序的API接口。

Native层作为Android多媒体系统的核心，针对上图，主要由MediaPlayer、MediaPlayerService、Stagefrightplayer、AudioFlinger和Audio
 HAL几个部分组成。针对Native层，我们可以将其划分为两个逻辑范畴进行分析。

从媒体框架的角度来说，MediaPlayer、MediaPlayerService和Stagefrightplayer三个部分构成了Android多媒体的基本框架。多媒体框架部分采用了C/S的结构，MediaPlayer作为C/S结构的Client端，MediaPlayerService和Stagefrightplayer作为C/S结构Server端，承担着播放多媒体文件的责任，通过Stagefrightplayer，Server端完成Client端的请求并作出响应。

从Audio的角度来说，MediaPlayer、MediaPlayerService、Stagefrightplayer、AudioFlinger和Audio
 HAL又构成了Android系统中Audio系统的框架。MediaPlayer、MediaPlayerService和Stagefrightplayer作为Audio系统框架的Client端，最终由StagefrightPlayer将Audio数据交给Server端的AudioFlinger处理，由AudioFlinger将最终的Audio数据交由Audio
 HAL层输出到硬件设备上，完成Audio的播放。

从纵向的角度来看，上层的应用程序将媒体的URI作为输入，交给Java
 Framework层的MediaPlayer，经过JNI将请求交给本地框架层的MediaPlayer，可以认为Java
 Framework层的MediaPlayer模块就是本地MediaPlayer在Java层的代理，然后由本地框架层的MediaPlayer向MediaPlayerService发出请求，MediaPlayerService将接收到的请求交给Stagefrightplyaer组件进行处理。Stagefrightplayer将处理的结果反馈给MediaPlayerService，最后由MediaPlayerService将处理后的Audio数据交给AudioFlinger处理，最终AudioFlinger将处理的数据通过Audio
 HAL层交给硬件驱动层，完成Audio的播放。

Stagefright 是Android多媒体本地实现的核心。Stagefright中包括的内容很多，单从播放的角度来看StagefrightPlayer输入的是文件或网络媒体流，输出的是送往[音视频](http://www.dzsc.com/product/searchfile/11647.html)输出设备播放的数据流，基本功能包括了媒体流控制、文件解析、音视频文件解码等方面。

## **2 多媒体系统音频数据播放的流程**

　　从多媒体系统具体实现的角度来看，音频数据播放主要经过音频格式文件解析、音频编码流解码、P[CM](http://wiki.dzsc.com/info/2755.html)输出播放3个阶段。音频播放器的基本结构如图2所示。

![](https://img-my.csdn.net/uploads/201301/05/1357387299_3485.jpg)


基于Android多媒体系统音频播放流程，对音频文件的播放主要有以下4个流程：

　　（1）音频文件的识别；

　　（2）音频文件的解析；

　　（3）编码数据的读取；

　　（4）编码数据的解码和输出。

### **2.1 Media **Framework层状态机

一个Android媒体播放应用程序在进行媒体文件播放的过程中，有多种状态。文件播放的过程就是这些状态的转换过程。基本的状态及其转换过程如下图所示：



![](https://img-my.csdn.net/uploads/201301/05/1357387306_6544.jpg)


Notice: About the details of each state, please reference the contents on the following URL：

http://developer.android.com/reference/android/media/MediaPlayer.html

### **2.2 Media Native层框架**

Native层的调用关系如下图所示，从图4中可以看到，Native层由三大部分组成，MediaPlayer、MediaPlayerService和用于完成具体播放功能的MediaPlayerBase实现部分。

![](https://img-my.csdn.net/uploads/201301/05/1357387313_4480.jpg)


Framework层的调用最终通过Native层的调用完成相应的功能。MediaPlayer和MediaPlayerService之间通过Binder进行通信。

从图4中可以看到，MediaPlayerService端的播放任务最终会交由MediaPlayerBase抽象类的子类来完成。MediaPlayerBase作为执行具体播放模块的超类，为了实现具体的播放功能派生了两个子类，一个叫MediaPlayerHWInterface，用于提供直接通过硬件输出的接口。另一个子类叫MediaPlayerInterface，StagefrightPlayer类作为MediaPlayerInterface的一个实现类。从逻辑上StagefrightPlayer担当了执行具体播放功能的责任。

### **2.3 Media Stagefright框架**

StageFright是一个framework，在这里也可以理解为是一个[Container](http://lib.csdn.net/base/docker)，对上它提供了调用接口，对下它负责创建并管理那些实现具体功能的Palyer组件。



针对Audio播放，如图5所示，图中展现了Audio在Stagefright框架中的基本结构。通过这个基本的结构，Audio最终与Audio
 Server端AudioFlinger建立连接。

在具体的Stagefright框架实现中，StagefrightPlayer并没有实现具体的方法，仍然作为Player的一个抽象层存在，具体的功能由其Wrapper类AwesomePlayer来完成的，所以Native层的具体动作是通过AwesomePlayer类来完成。



![](https://img-my.csdn.net/uploads/201301/05/1357387319_2758.jpg)


从图中看到，Audio数据经过AwesomePlayer处理之后，具体应该是Decoder之后，交给AudioPlayer，在AudioSink存在的情况下，AudioSink作为MediaPlayerService的组成部分，负责完成Audio的输出。AudioPlayer会通过AudioSink的实现类AudioOutput完成数据的输出。而在具体实现中AudioOutput会创建用于声音播放的AudioTrack，最终通过AudioTrack与AudioFlinger建立连接，实现声音的输出。

### **2.4 Native层Audio播放流程**

以播放本地媒体文件为例，在媒体播放的数据源设置阶段，对应Framework层的状态图，也就是IdleàInitialized的Initial阶段，如图6所示，在这个过程中需要进行setDataSource，Framework层的调用在AwesomePlayer中则会看到相应的处理，最终会通过调用AwesomePlayer对象的setDataSource方法来设置Native层的数据源；AwesomePlayer通过调用Stagefright包中的MediaExtractor类（提取器类的超类）的Create方法，通过识别数据源的格式，在获得数据源对应的数据格式之后，创建该格式对应的提取器实例<XXX>Extractor，例如，如果数据源格式是AAC格式，那么就创建一个AACExtractor的实例，AACExtractor类继承于MediaExtractor。在Create方法创建并返回一个<XXX>Extractor的实例之后，AwesomePlayer会通过该实例提供的接口方法解析数据源，获得数据源的元数据（metadata）随后通过调用<XXX>Extractor的getTrack方法获得一个<XXX>Source的实例，如果数据格式为AAC，创建的实例名可能是AACSource，然后将获得的实例进行保存。在这个阶段，AwesomePlayer通过MediaExtractor提供的接口完成了对数据源中Audio和Video的Split(这时只是从逻辑上建立Audio和Video，还没有真正地进行split，只是初步的分析，来创建响应的组件。)，到此Media框架完成了整个数据源的设置过程；

StageFright创建了AwesomePlayer实例，AwesomePlayer通过MediaExtractor创建了真正的对应于具体文件格式的extractor，通过此具体的extractor创建了对应的读取数据类，即<xxx>source类的实例。

setDataSource的动作到此结束，它的动作是完成了对文件的初步解析，确定了文件格式，最终的目的是为了创建相应的<xxx>extractor以及<xxx>source。此时播放还没有开始。



![](https://img-my.csdn.net/uploads/201301/05/1357387326_7708.jpg)


值得注意的是：针对数据源的设置，Android媒体框架提供了多种方式，本地文件只是其中的一种形式方式，还有其它URL或者网络媒体等形式。不同数据源形式处理的过程也不一样。有同步的处理方式，也有异步的处理方式。



![](https://img-my.csdn.net/uploads/201301/05/1357387334_1838.jpg)


在完成了数据源的设置之后，到达Initialized状态，下一个状态就是对应Framework层的Prepared状态。InitializedàPrepared阶段有两种Prepare方式，一种是同步prepare，另一种是异步Prepare。这个阶段无论是同步还是异步，都需要初始化Audio和Video的Decoder组件。如果需要播放的媒体数据源（Audio和Video）是经过数据编码压缩过的，AwesomePlayer都会通过OMXCodec创建针对该压缩编码格式的OMXCodec对象，OMXCodec类继承于MediaSource类，通过OMXCodec对象我们可以为AudioPlayer提供原始的Audio数据。

在通过OMXCodec创建原始Audio数据的过程中，我们必须通过OMXClient得到OMX的接口，通过这个接口，我们才能够创建具体数据格式的OMXCodec对象。

在完成Prepare动作之后，到达Prepared状态，下一个状态就是对应Framework层的Started状态。上层调用start方法之后，在AwesomePlayer层会调用play方法，针对Audio，AwesomePlayer层Play阶段会创建一个[Audio](http://www.dzsc.com/stock-ic/AUDIO.html)Player类的对象mAudioPlayer，并调用该对象的setSource方法，把之前OMXCodec::Create返回的mAudioSource作为参数，设置该对象的成员变量mSource；随后调用该对象的start方法；开始Audio的播放（播放还是解码？这里的播放时逻辑上的概念，实际是一边播放，一边解码，异步过程）。

这里AudioPlayer就是对一个audio播放设备的模拟，通过该类完成audio数据的播放。

AudioPlayer 获取OMXCodec对象中的相关参数：文件类型、采样率、声道数，之后调用AudioSink对象的open方法创建Audio数据的AudioTrack，因为最终的Audio播放都是通过AudioTrack和AudioFlinger打交道的。（调用此方法做什么？），并把AudioPlayer类的AudioSinkCallback方法做为回调函数传给AudioSink。AudioPlayer先调用XXXDecoder解第一帧数据，并把该数据传给AudioSink去播放，当播放完成后AudioSink会调用回调函数AudioSinkCallback再去取解码后的数据，在这个函数中主要实现两个功能，一是从解码缓冲区中读取解码后的数据，二是把读到的数据拷贝到AudioFlinger提供的环形缓冲区中。这个过程中，涉及数据从Codec缓冲区到AudioFlinger缓冲区的一次拷贝。（AudioSinkCallback函数的动作有哪些？）具体过程是AudioSinkCallback会调用FillBuffer函数获取解码后的原始数据，解码后数据如果被取完后，AudioPlayer又会调用XXXDecoder解下一帧数据给AudioSink，来回反复，直到文件中数全部被播放。在拉动滚动条时，上层会传来SeekTime，经AudioPlayer传给XXXDecoder再传给XXXExtractor，XXXExtractor根据上层传来的SeekTime判断出要播放的原始数据的起始位置，然后从该位置读取一个数据包传给XXXDecoder解码。

AudioSink是audio播放过程的控制者，以一个thead的形式存在。它不停地调用decoder的接口来获取数据，并将这些数据送往audio播放设备。（AudioSink与decoder没太大关系，逻辑上没关系）

![](https://img-my.csdn.net/uploads/201301/05/1357387506_3712.jpg)


在整个数据格式解码播放过程中，主要涉及两个模块：XXXExtractor和XXXDecoder。

XXXExtractor主要执行数据格式文件解析和数据读取功能;

XXXDecoder主要执行编码数据的解码功能，是由OMXCodec来提供的。





这里最终的一个东西就是AudioSinkCallback回调函数。首先我们要知道，这个函数是谁实现的：


size_tAudioPlayer::AudioSinkCallback(


        MediaPlayerBase::AudioSink *audioSink,


        void *buffer, size_t size, void*cookie) {


    AudioPlayer *me = (AudioPlayer *)cookie;




    return me->fillBuffer(buffer, size);


}

我们看到这个函数是由AudioPlayer类实现的。函数的内容直接说明了这个函数的功能，那就是填充所给的buffer。这个buffer是谁给的，这就要看是谁调用了这个函数。我们知道回调函数是实现出来给别人调用的，而不是自己调用。通过不断的注册，最终调用这个函数的类是AudioTrack。所以一切都明白了，是AudioTrack提供一个Buffer，让AudioPlayer来填充这个Buffer。只不过这个注册和调用的过程曲折了一点，注册的时候通过AudioSink转了个手，而调用的时候，也由AudioSink转了个手。本质上在这个过程中AuidoSink并没有做什么。AudioSink是衔接MediaPlayerService和AudioTrack的。是一个逻辑上的声音池。MediaPlayerService处理后的Audio都要丢到这个声音池中，而AudioTrack是由这个声音池创建和控制。至于AudioSink在什么时候创建，由谁创建的，AudioSink的定义和结构就说明了这个，因为它是MediaPlayerService的组成部分，AudioSink是由MediaPlayerService在setDataSource阶段创建的。




