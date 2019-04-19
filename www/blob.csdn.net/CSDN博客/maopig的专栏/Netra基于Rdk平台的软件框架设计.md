# Netra基于Rdk平台的软件框架设计 - maopig的专栏 - CSDN博客
2014年07月11日 22:09:40[maopig](https://me.csdn.net/maopig)阅读数：3955
# [Netra（DM8168）处理器是个多核处理器，每个核之间相互独立却又相互关联，如何高效简洁地利用每个核完成一套系统功能是非常关键的，RDK这套软件平台就是针对这种多核平台设计的一套多通道视频应用方案，主要用于DVR、NVR、IPNC之类的产品。](http://blog.csdn.net/crushonme/article/details/10245169)
        这个软件框架结构允许用户创建不同的多路数据流来实现视频的采集、视频处理（DEI、Noise Filter、Encode、Decode、SwMs、Merge等）和视频播放功能，其框架设计非常值得学习。
以下为相关缩写解释：
*****************************************************************************************************
HDVPSS：High Definition Video Processing Subsystem
HDVICP：High Definition Video and Image CoProcessor
Ducati：Dual Core M3 Processors controlling HDVPSS and HDVICP hardware engines
Video M3：ARM Cortex M3 Core（inside Ducati subSystem）controlling HDVICP codecs
VPSS M3：ARM Cortex M3 Core（inside Ducati subSystem）controlling HDVPSS，also called DSS M3
DEI：Deinterlacer
McFW：Multi Channel Framework
IPC：Inter Processor Communication
******************************************************************************************************
1、Rdk平台软件框架图
     在Rdk平台软件中做了很好的分层，如下图所示：
![](https://img-blog.csdn.net/20130824020138312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcm9iaW4xOTg5MDMwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
注意：上图中核间通过Syslink/IPC通信，任意2个核之间都可以直接通信，是一个星型网络，上图画的连接关系描述的是对等层次的概念。
上图中相关层的功能及相关描述如下表所示：
|层|处理器|描述|相关TI软件包|
|----|----|----|----|
|Linux|HOST A8|Linux OS, includes, filesystems,SATA, Ethernet, USB and otherIO drivers|Linux PSP|
|BIOS6|VPSS M3Video M3DSP|BIOS RTOS used as OS onVideo-M3, VPSS-M3, DSP.Provides features like threads,semaphores, interrupts.Queues and message passingbetween links is implementedusing BIOS semaphores.|BIOSXDC (used for BIOS andother configuration)|
|Syslink / IPC|HOST A8VPSS M3Video M3DSP|Software APIs used forcommunicating betweenprocessors. Provides featureslike processor loading andbooting, multiprocessor heaps,multiprocessor linked list(ListMP), message queues, notify etc|SyslinkIPC|
|HDVPSSDrivers|VPSS M3|HDVPSS drivers like capture,display, deinterlacer, scalingbased on FVID2 interface tocontrol and configure theHDVPSS HW|HDVPSS|
|VideoEncode/Decode|Video M3|Video encode / decode APIsbased on XDM / XDIAS interface.Uses framework components for resource allocation|XDIASFramework componentsIVAHD HDVICP2 APIH264 decoderH264 encoder|
|Links|Links HOST A8VPSS M3Video M3DSP|Implementation of individuallinks. Some links are specific toa processor while some links are common across processors|DVR RDK|
|Link API|HOST A8|The link API allows users tocreate , connect, and controllinks on HOSTA8, VPSS M3,Video M3 and DSP.Link API is used to create a chain of links which forms a user defined use-case.The connection of links to eachother is platform dependant.|McFW|
|McFW API|HOST A8|Multi-Channel FrameWork API.Multi-Channel Applicationspecific API which allows user to setup and control pre-definedapplication specific chains forDVR, NVR, using a single simplified API interface.This allows users to directly usethe links without having tounderstand the detailed link API.The McFW API is platformindependent and same API willwork on DM816x, DM814x,DM810x|McFW|
|UserApplication|HOST A8|Typically GUI and otherapplication specific componentslike file read/write, networkstreaming.|Customer specific|
2、基于Rdk框架的实例
                Capture (YUV422I) 16CH D1 60fps
                   |
                  NSF (YUV420SP)
                   |
                  dup1---->IPCM3OUT(VPS) -> IPCM3IN(VID) -> ENC -> IPC_BITS_OUT_M3 -> IPC_BITS_IN_A8 -> fwrite() - Write to filesystem
                   |                                                 ！
                   |                                                 ！
                   +-<----IPCM3IN(VPS) <- IPCM3OUT(VID) -> DEC <- IPC_BITS_IN_M3 <- IPC_BITS_OUT_A8 <- fread() - read from harddisk
                   |
                 MERGE
                   |
                  DUP2
                  |||
         +--------+|+------------+
         |         |             |       
         |                |             |
      SW Mosaic       SW Mosaic        SW Moasic
      (DEIH YUV422I)  (DEI YUV422I)    (SC5 YUV422I)
         |                |             |
 GRPX0   |       GRPX1,2  |             |
    |    |           |    |             |
    On-Chip HDMI    Off-Chip HDMI  SDTV (NTSC)
      1080p60         1080p60        480i60
3、Link Api机制
3.1 link api的概念
    link在上图中的视频数据流中是最基本的单元模块，每个link中包含了一个基于BIOS6/Linux的任务、线程、消息盒(使用操作系统的信号量实现)。由于每个link运行一个独立的线程，因此link之间可以并行运行。消息盒是关联用户指定的link，让link之间有个互相对话的机制，用来传递信令。而对于视频流数据、帧数据的传递link实现了专门的接口来实现，只传递指针，而不是数据。
在实现上节所介绍的数据流可以通过将多个link连接来实现chain，link API允许用户Create、Start、Stop、Delete、Control各个link。在Rdk中TI基于link API进行再次封装，用于特定的应用场合，其各种参数均是为特定的产品定制，可以是相关业务的开发更迅速。
3.2 link内部结构
![](https://img-blog.csdn.net/20130824024044859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcm9iaW4xOTg5MDMwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3.3 link的特性
- 视频处理的工作量可以均衡到每个核心上去执行，如下表：
|Processor|OS|Used for|
|----|----|----|
|HOST A8|Linux|System setup and control, GUI, IO peripheral control like SATA, Ethernet, USB, Audio|
|VPSS M3|BIOS6|HDVPSS control for video capture, video display, scaling, deInterlacing …|
|Video M3|BIOS6|HDVICP2 Video compression / decompression (H264 encode, H264 decode)|
|DSP|BIOS6|SW OSD, custom video processing algorithms|
- 每个link都有自己独立的任务/线程用于完成视频采集或播放等处理；
- 每个link都可以处理来自多个通道的视频帧数据，每个通道的视频宽高和数据格式都可以不同；
- A8作为HOST可以用来连接多个link形成数据流的链并对其进行控制；
- 数据链被建立并启动时，数据链中的每个link将和它的下游link进行帧数据的交互；
- links之间的帧数据交互可以在多个核心之间进行，并且并不需要A8 HOST的干涉，从而降低A8的开销；
- 在数据链运行过程中用户可以发送控制信令给任意link来动态设置相关的link参数；
3.4 link接口
    link接口可以分成以下几类：
- link API——被用户调用来配置和控制link的接口；
- Inter link API——被其他links调用来交换帧数据的接口；
- link output queue——被其他links通过Inter Link API接口实用的帧buffer队列；
3.5 link间的消息传递
每个Link通过一个32位的该LinkId来识别，ID高4位标识了这个Link是在哪个核上运行，低24位标识了该Link的名称：
|Bits|Description|
|----|----|
|[0..27]|Link ID|
|[28..31]|Processor ID on which this link runs0:DSP1:Video M32:VPSS M33:A8|
每个Link API都需要这个LinkID参数来发送消息，当用户发送消息到一个Link时，根据这ID函数内部判断这个消息是发给本地的Link还是远端核心的Link；如果是本地的直接调用BIOS/LINUX API函数，否则就通过Syslink模块的MessageQ发送这个消息到指定的核心，让对端的核心调用对应的函数处理。
3.6 link API
    下面将通过表格的形式来看看link API的部分接口及相关功能：
|API|Description|
|----|----|
|System_linkCreate|Creates a link ——allocates driver, codec, memory resources.|
|System_linkGetInfo|Get information about a link like number of channels, properties of each channel. MUST be called after System_linkCreate() for a link|
|System_linkStart|Start the link ——starts the driver or codec|
|System_linkControl|Send a link specific control command with optional arguments|
|System_linkStop|Stop the link —— stops the driver or codec|
|System_linkDelete|Deletes a link —— free’s driver, codec, memory resources|
3.7 Inter link API
    下面的表格介绍了在link内部实现的交换帧数据的API，对于用户来说，这部分API是不需要关心的：
|API|Description|
|----|----|
|System_GetLinkInfoCb|Function to return information about a link like number of channels, properties of each channel|
|System_LinkGetOutputFramesCb|Function to return captured or generated oroutput frames to the caller (another link)|
|System_LinkPutEmptyFramesCb|Function to release consumed frames back to the original link for reuse|
|System_LinkGetOutputBitBufsCb|Function to return generated or output bitstream frame to the caller (another link) –Valid only for Encode Link|
|System_LinkPutEmptyBitBufsCb|Function to release consumed bitstream frames back to the original link for reuse –Valid only for Encode Link|
    在每个link中必须实现一些函数并在初始化时注册这些函数指针给link管理的核心模块，用于帧数据的获取、释放、dump相关状态等。
    对于任一个link想从它的上游link获取帧数据都需要调用link管理核心函数System_getLinksFullFrames()，该函数内部会发送消息到对应的上游link，触发该link向管理模块注册的回调函数System_LinkGetOutputFramesCb()将帧数据传递给该link；
    同样的，在当一个link想释放处理完毕的帧buffer给上游link时需要调用link管理核心函数System_putLinksEmptyFrames()，该函数内部会发送消息到对应的上游link，触发它注册的回调函数System_LinkPutEmptyFramesCb()将帧buffer回收，用于后续的数据处理；
    建立chain时，你肯定还会关注一个信息，那就是上游link的相关参数如何传递给下游的link，从源程序仔细琢磨琢磨就可以看出来，和上面的处理类似，所有有下游link的link都会注册一个System_GetLinkInfoCb()的回调函数，在下游link的driver中会在创建driver时调用System_linkGetInfo()函数来获取上游link的相关参数。
    通过上述的方法，对于一个link来说就不需要关心和它交互的是哪一个link，所有的寻址都通过linkID来自动查找，并且同一个link实现可以和不同的link交互，而不需要改变函数的实现。
3.8 Link Output Queues 的管理
一个Link可以有一个或多个输出队列用来存放采集到的或处理完毕的帧数据，每个Link的输出队列内存由自己分配；
    大多数Links只有一个输出队列，但是有些link有多个，从而可以实现多路不同的输出数据流满足不同的应用需求，例如，Noise filter Link可以输出16路帧数据到2个输出队列，每个输出队列输出8路通道数据跟别给2个DEI Links模块处理。
一个输出队列中可以有多个视频channels的数据，每个channel可以有不同的大小和数据格式；
数据结构 FVID2_Frame是在VPSS驱动中定义的，Links之间就通过该结构参数传递帧数据的信息，如帧数据的Buf指针，而帧数据本身并不会被拷贝，从而节省内存开销；
    当一个Link采集完或处理完一帧数据后会发送一个消息“SYSTEM_CMD_NEW_DATA”给下游的Link，从而通知它有数据可取；当下游Link收到该消息后会调用System_getLinksFullFrames()函数来获取对用的帧数据，处理完后再调用System_putLinksEmptyFrames()函数来归还给上游Link继续使用。
因此一个Link需要知道：
- 上游Link的LinkID和QueID，从而从该队列里面获取帧数据
- 下游LinkID，从而在有新数据产生时通知下游Link来取
上游Link的LinkID和QueID 以及下游LinkID 都是在System_linkCreate()时由A8
 HOST端来指定的。
3.9 IPC link核间帧数据交互
IPC Link，是用来多核之间的帧数据传递的。
如VPSS上的采集Link想把帧数据发送给Video Link处理，先将帧数据传递给本地的IPC Link，然后IPC Link再通过Syslink/IPC发送到Video Link上的IPC Link，然后再转发给Video Link，这样的话对于采集Link的实现来说就非常清晰简单，它的实现都是发送给本地的另一个Link；
IPC Link的实现有点复杂，因为它涉及的帧数据传递是在多个核之间，这里面就牵扯到cache的一致性问题，考虑到每个核的特性以及高效性，总共设计了3个内部Links用于帧数据的传递机制：
- Intra-processor links
     即同一核心内部的link，如采集与降噪之间的帧数据传递，这种内部的link间传递帧数据都是在VPSS M3内部完成，因此采用简单且高效的队列机制实现。
- Inter M3 (Video / VPSS) links
     即M3内部核心之间的link，由于Video和Vpss所在的2个M3核心是同属于一个双核M3处理器，它们的cache是共享的；如降噪模块（VPSS NF）到编码模块（VIDEO Enc）之间传递帧数据，带有Notify的IPC ListMP机制被用来在这2个M3核心之间传递帧信息（FVID2_Frame），该过程中不需要任何cache操作和地址转换。
- Inter processor (M3 to A8 or DSP)
    即处理器内部核心之间，如编码(VideoM3)到BitStream In(HostA8)之间传递帧数据，同样使用带Notify的IPC ListMP机制在2个核心之间传递帧信息（FVID2_Frame），但该过程中需要做cache同步和地址转换操作。
4、chain数据链路的建立
 一个Chain是由多个links按照一定的应用需求按顺序连接成一条视频处理的数据流。
    一个Chain可以销毁后重新按照新的需求组成新的Chain，不需要重启系统。
 Chain创建是特别需要相关link的顺序
- 通过System_linkCreate()函数按照由source>>sink 的顺序创建需要的Links，Source Link即没有上游Link的Link，如：视频采集；Sink Link是没有下游Link的Link，如：视频播放；这个创建顺序是非常重要的，因为一个Link创建时它会查询上游Link的一些信息，如上游Link需要的channel的个数和属性，从而按照这些参数配置自己。
- 下一步调用System_linkStart()函数启动每个Link，启动顺序一般从Sink Link往前到Sorce Link，当然你也可以不按照这个顺序，不过不推荐，因为这样可以保证每个Link在它的上游Link启动前准备好接收数据，避免过多的缓冲引入额外的时延。
- 当一个Chain运行后控制命令就可以发送到各个Links来控制它，如调用System_linkControl()函数发送改变画面合成风格的命令给相应的Link，具体的命令定义由每个Link的功能实现来决定；
- 注意：一般来说System_linkControl()函数是在System_linkCreate()创建了Link之后才能调用，不过有些控制命令可以在System_linkCreate()调用之前调用，以完成Link创建之前必须的一些初始化，如复位；
- 当Chain工作完成或销毁时可以调用System_linkStop()函数先停止每个Link，注意：停止的顺序必须从Source开始依次到Sink结束；因为一个Link可能阻塞着等待下游Link释放当前Link的输出Buffer，如果下游Link先停止的话当前Link可能会出于wait for ever的状态而永久退不出来，因此上游Link必须先停止，之后才能停止下游Link；
- 最后等所有Link全部停止后，可以调用System_linkDelete()函数删除所有Links，删除顺序没有要求；
- 当Chain销毁后就可以按照之前的顺序重新创建一个新的Chain来完成另一个工作了。
