# 多媒体的框架 - OpenCore框架概述 - DoubleLi - 博客园






OpenCore是一个多媒体的框架，从宏观上来看，它主要包含了两大方面的内容：PVPlayer：提供媒体播放器的功能，完成各种音频 （Audio）、视频（Video）流的回放（Playback）功能。PVAuthor：提供媒体流记录的功能，完成各种音频（Audio）、视频 （Video）流的捕获（Recording）功能PVPlayer和PVAuthor以SDK的形式提供给开发者，可以在这个SDK之上构建多种应用程 序和服务。在移动终端中常常使用的多媒体应用程序，例如媒体播放器、照相机、录像机、录音机等等。

OpenCore的层次结构图：

![多媒体的框架 - OpenCore框架概述(一)](http://static.open-open.com/lib/uploadImg/20120126/20120126122238_373.gif)

OpenCore是一个多媒体的框架，从宏观上来看，它主要包含了两大方面的内容：
 OSCL（Operating System Compatibility Library，操作系统兼容层）
 PVMF（PacketVideo Multimedia Framework，PV多媒体框架） 文件解析（parser）和组成（composer）两个部分 编解码部分
NODEs（节点）
Player Engine（播放器引擎）
Author Engine（作者引擎）

        OpenCore中包含的内容非常多：从播放的角度，PVPlayer 的输入的（Source）是文件或者网络媒体流，输出（Sink）是音频视频的输出设备，其基本功能包含了媒体流控制、文件解析、音频视频流的解码 （Decode）等方面的内容。除了从文件中播放媒体文件之外，还包含了与网络相关的RTSP流（RealTime Stream Protocol,实时流协议）。在媒体流记录的方面，PVAuthor 的输入的（Source）是照相机、麦克风等设备，输出（Sink）是各种文件， 包含了流的同步、音频视频流的编码（Encode）以及文件的写入等功能。在使用OpenCore 的SDK的时候，有可能需要在应用程序层实现一个适配器（Adaptor），然后在适配器之上实现具体的功能，对于PVMF的NODE也可以基于通用的接 口，在上层实现，以插件的形式使用。

OpenCore的OSCL部分：

OSCL，全称为Operating System Compatibility Library (操作系统兼容库)，它包含了一些在不同操作系统中移植层的功能，它的在OpenCore oscl/oscl目录中，一般每一个目录表示一个模块。OSCL对应的功能是非常细致的，几乎对C语言中每一个细节的功能都进行封装，并使用了C++的 接口提供给上层使用。事实上，OperCore中的PVMF、Engine部分都在使用OSCL，而整个OperCore的调用者也需要使用OSCL。

OpenCore的文件格式和编解码部分：

1.文件格式的处理部分：

OpenCore有关文件格式处理和编解码部分两部分的内容，分别在目录fileformats和codecs_v2当中。这两部分都属于基础性的 功能，不涉及具体的逻辑，因此它们被别的模块调用来使用。文件格式处理有两种类型，一种是parser（解析器），另一种是composer（组成器）。 其代码的目录为fileformats的目录，其中包含mp3，mp4，wav等子目录。其中包含了AVI，mp3，mp4，wav等多种文件的解析器和 组成器，各个目录中对应的是不同的文件格式等。

fileformats的目录结构如下所示：
fileformats
|-- avi
| `-- parser
|-- common
| `-- parser
|-- id3parcom
| |-- Android.mk
| |-- build

| |-- include
| `-- src
|-- mp3
| `-- parser
|-- mp4
| |-- composer
| `-- parser
|-- rawaac
| `-- parser
|-- rawgsmamr
| `-- parser
`-- wav
`-- parser



2.编解码部分：

目录结构如下所示：

codecs_v2
|-- audio
| |-- aac
| |-- gsm_amr
| |-- mp3
| `-- sbc
|-- omx
| |-- factories
| |-- omx_aac
| |-- omx_amr
| |-- omx_common
| |-- omx_h264
| |-- omx_m4v
| |-- omx_mp3
| |-- omx_proxy
| `-- omx_queue
|-- utilities
| |-- colorconvert
| |-- m4v_config_parser
| `-- pv_video_config_parser
`-- video
|-- avc_h264
`-- m4v_h263

编解码部分主要针对Audio和Video的软件编解码，其目录为codecs_v2，其中包含了audio、omx 、utilities、video等几个目录。在audio 和video目录中，对应了针对各种流的子目录，其中可能包含dec和enc两个目录，分别对应解码和编码。

`-- video


|-- avc_h264 
| |-- common 
| |-- dec 
| |-- enc 
| `-- patent_disclaimer.txt 
`-- m4v_h263 
|-- dec 
|-- enc 
`-- patent_disclaimer.txt

codecs_v2目录的子目录omx实现了一个khronos OpenMAX的功能。OpenMAX是一个多媒体应用程序的框架标准，由NVIDIA公司和Khronos在2006年推出。OpenMAX IL 1.0
（集成层）技术规格定义了媒体组件接口，以便在嵌入式器件的流媒体框架中快速集成加速式编解码器。OpenMAX的设计实现可以让具有硬件编辑码功能的平台
提供统一的接口和框架，在OpenMAX中可以直接使用硬件加速的进行编解码乃至输出的功能，对外保持统一的接口。此处的OpenMAX则是基于一个纯软件的实现，其实现的代码即是调用这里的video和audio目录中的软件编辑码的代码。

OpenCore Node介绍：


Node是OpenCore中基本的功能模块，OpenCore本身提供了一些Node，也可以由上层软件来实现。本身提供的Node在 OpenCore的nodes目录中。OpenCore的Node主要分成三个类型：编解码的Node，文件格式的Node，输入输出模块的Node。 



### OpenCore框架概述(二)



OpenCore Player介绍：

OpenCore的Player的编译文件是pvplayer/Android.mk，将生成动态库文件 libopencoreplayer.so。这个库包含了两方面的内容：一方是Player的engine（引擎），一方面是为Android构件的 Player，这实际上是一个适配器（adapter）。

libopencoreplayer.so中包含了以下内容：

一些解码工具；
文件的解析器（MP4）；
解码工具对应的Node；
Player的引擎部分（编译文件：engines/player/Android.mk）；

  为Android构建的Player适配器（编译文件：android/Android.mk）；

 识别工具（pvmi/recognizer）；
 编解码工具中的OpenMAX部分（codecs_v2/omx）；
 对应插件Node的注册。
libopencoreplayer.so中的内容较多，其中主要为各个文件解析器和解码器，PVPlayer的核心功能在engines/player /Android.mk当中；而android/Android.mk的内容比较特殊，它是在PVPlayer之上构建的一个为Android使用的播放 器。

![多媒体的框架 - OpenCore框架概述(一)](http://static.open-open.com/lib/uploadImg/20120126/20120126122239_104.gif)

OpenCore的Player Engine具有清晰明确的接口，在这个接口上，不同的系统可以根据情况实现不同的Player。位于OpenCore中的engines/player /目录下，其中，engines/player/include目录中保存的是接口头文件，engines/player/src目录中保存是源文件和私 有头文件 。

Player Engine的类结构：

![多媒体的框架 - OpenCore框架概述(一)](http://static.open-open.com/lib/uploadImg/20120126/20120126122239_745.gif)



PVPlayer 的结构： 


![多媒体的框架 - OpenCore框架概述(一)](http://static.open-open.com/lib/uploadImg/20120126/20120126122240_990.gif)


OpenCore Author介绍： 



目录为OpenCore中的engines/author/，是Author引擎目录，其主要包含include和src两个目录，头文件中的pvauthorenginefactory.h和pvauthorengineinterface.h两个文件 
为接口，源文件为主要的具体实现pvauthorengine.cpp。 


![多媒体的框架 - OpenCore框架概述(一)](http://static.open-open.com/lib/uploadImg/20120126/20120126122240_100.gif)




PVAuthor 的结构： 
![多媒体的框架 - OpenCore框架概述(一)](http://static.open-open.com/lib/uploadImg/20120126/20120126122240_187.gif)


PVAuthor 的类的结构： 
![多媒体的框架 - OpenCore框架概述(一)](http://static.open-open.com/lib/uploadImg/20120126/20120126122241_821.gif)

转自： [http://blog.csdn.net/itachi85/article/details/7080682](http://blog.csdn.net/itachi85/article/details/7080682)









