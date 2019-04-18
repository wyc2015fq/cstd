# VFW开发视频应用程序 - 深之JohnChen的专栏 - CSDN博客

2009年07月16日 10:29:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3366


当前，在Windows 平台下开发视频应用程序一般采用两种方式：一种是基于视频采集卡所附带的二次软件开发包SDK（Software development kit）进行。这种方式的优点是应用方便，容易上手，缺点是对硬件的依赖性较强，灵活性差，且功能参差不齐，不能充分满足各种视频应用程序的开发需要； 

另一种方式是基于VFW(Video for Windows)进行的。VFW 是Microsoft公司为开发Windows平台下的视频应用程序提供的软件工具包，提供了一系列应用程序编程接口（API），用户可以通过它们很方便地实现视频捕获[1]、视频编辑及视频播放等通用功能，还可利用回调函数开发更复杂的视频应用程序。它的特点是播放视频时不需要专用的硬件设备，而且应用灵活，可以满足视频应用程序开发的需要。Windows操作系统自身就携带了VFW，系统安装时，会自动安装VFW的相关组件。VC++自4.0以来就支持VFW，大大简化了视频应用程序的开发。目前，PC机上多媒体应用程序的视频部分，大都是利用VFW API开发的。

1  VFW 的体系结构 

VFW以消息驱动方式对视频设备进行存取，可以很方便地控制设备数据流的工作过程。目前，大多数的视频采集卡驱动程序都支持VFW接口，它主要包括多个动态连接库，通过这些组件间的协调合作，来完成视频的捕获、视频压缩及播放功能。VFW体系结构如图1所示。 

1）VICAP.DLL：主要实现视频捕获功能，包含了用于视频捕获的函数，为音像交错AVI (Audio video interleaved)格式文件和视频、音频设备程序提供一个高级接口。 

2）MSVIDEO.DLL：能够将视频捕获窗口与获驱动设备连接起来，支持ICM视频编码服务。 

3）MCIAVI.DRV：包含MCI（Media control interface）命令解释器，实现回放功能。 

4）AVIFILE.DLL：提供对AVI文件的读写操作等文件管理功能。 

5）ICM ( Installable compression manager )：即压缩管理器，提供对存储在AVI文件中视频图像数据的压缩、解压缩服务。 

6）ACM ( Audio Compression Manager )：即音频压缩管理器，提供实时音频压缩及解压缩功能。
2  视频捕获 

视频数据的实时采集，主要通过AVICAP模块中的消息、宏函数、结构以及回调函数来完成。视频捕获的一般过程如下： 

2.1　建立捕获窗口 

利用AVICAP 组件函数 capCreateCaptureWindow() 建立视频捕获窗口，它是所有捕获工作及设置的基础，其主要功能包括：① 动态地同视频和音频输入器连接或断开；② 设置视频捕获速率；③ 提供视频源、视频格式以及是否采用视频压缩的对话框；④ 设置视频采集的显示模式为Overlay或为Preview; ⑤ 实时获取每一帧视频数据；⑥ 将一视频流和音频流捕获并保存到一个AVI文件中； ⑦ 捕获某一帧数字视频数据，并将单帧图像以DIB格式保存；⑧ 指定捕获数据的文件名，并能将捕获的内容拷贝到另一文件。 

2.2　登记回调函数[2] 

登记回调函数用来实现用户的一些特殊需要。在以一些实时监控系统或视频会议系统中，需要将数据流在写入磁盘以前就必须加以处理，达到实时功效。应用程序可用捕获窗来登记回调函数，以便及时处理以下情况：捕获窗状态改变、出错、使用视频或音频缓存、放弃控制权等，相应的回调函数分别为 capStatusCallback(), capErrorCallback(), capVideoStreamCallback(), capWaveStreamCallback(),capYieldCallback()。 

2.3　获取捕获窗口的缺省设置 

通过宏capCaptureGetSetup(hWndCap,&m_Parms,sizeof(m_Parms))来完成。 

2.4　设置捕获窗口的相关参数 

通过宏capCaptureSetSetup(hWndCap,&m_Parms,sizeof(m_Parms))来完成。 

2.5　连接捕获窗口与视频捕获卡 

通过宏capDriveConnect(hWndCap,0)来完成。 

2.6　获取采集设备的功能和状态 

通过宏capDriverGetCaps(hWndCap,&m_CapDrvCap，sizeof(CAPDRIVERCAPS))来获取 

视频设备的能力，通过宏capGetStatus(hWndCap,&m_CapStatus,sizeof(m_CapStatus)) 

来获取视频设备的状态。 

2.7　设置捕获窗口显示模式 

视频显示有Overlay(叠加)和Preview(预览)两种模式。在叠加模式下，捕获视频数据布展系统资源，显示速度快，视频采集格式为YUV格式，可通过capOverlay(hWndCap,TRUE)来设置；预览模式下要占用系统资源，视频由系统调用GDI函数在捕获窗显示，显示速度慢，它支持RGB视频格式。 

2.8　捕获图像到缓存或文件并作相应处理 

若要对采集数据进行实时处理，则应利用回调机制，由capSetCallbackOnFrame（hWndCap, FrameCall- 

backProc)完成单帧视频采集；由capSetCallbackOnVideoStream(hWndCap, VideoCallbackProc)完成视频流采集。如果要保存采集数据，则可调用capCaptureSequence（hWnd）；要指定文件名，可调用capFileSetCap- 

ture(hwnd, Filename)。 

2.9　终止视频捕获　断开与视频采集设备的连接 

调用capCatureStop(hWndCap)停止采集，调用capDriverDisconnect(hWndCap), 断开视频窗口与捕获驱动程序的连接。 

3  视频编辑和播放 

利用VFW，不仅可以实现视频流的实时采集，还提供了编辑和播放功能，主要通过AVIFILE、ICM、ACM、MCIWnd 等组件之间的协作来完成。 

       1) AVIFileInit();//初始化; 

2) AVIFileOpen(); //打开一个AVI文件并获文件的句柄; 

3) AVIFileInfo(); //获取文件的相关信息，如图像的Width和Height等; 

4) AVIFileGetStream(); //建立一个指向需要访问的数据流的指针; 

5) AVIStreamInfo(); //获取存储数据流信息的AVISTREAMINFO结构; 

6) AVIStreamRead(); //读取数据流中的原始数据, 对AVI文件进行所需的编辑处理； 

7) AVIStreamRelease(); //释放指向视频流的指针; 

8) AVIFileRelease();AVIFileExit(); //释放AVI文件。 

若数据是压缩过的，则用AVIStreamGetFrameOpen(),AVIStreamGetFrame()和AVIStreamGetFrameClose()来操作，可以完成对视频流的逐帧分解。 

3.2  视频播放 

对于实现视频流的播放，VFW提供了MCIWnd窗口类[4]，主要用于创建视频播放区，控制并修改MCI窗口当前加载媒体的属性。一个由函数、消息和宏组成的库与MCIWnd相关联，通过它们可以进行AVI文件操作，很方便地使应用程序完成视频播放功能。 

1)MCIWndCreate(); //注册MCIWnd窗口类，创建MCIWnd窗口，并指定窗口风格； 

2)AVIFileInit(); //初始化； 

3) AVIFileOpen(); //打开AVI文件； 

4) AVIFileGetStream(); //获得视频流； 

5)运用相关函数进行各种播放任务：MCIWndPlay()正向播放AVI文件内容,MCIWndPlayReverse()反向播放,MCIWndResume() 恢复播放,MCIWndPlayPause()暂停播放，MCIWndStop()停止播放等等。 

6) AVIStreamRelease(); //释放视频流； 

7）AVIFileRease();AVIFileExit(); //断开与AVI文件的连接，释放视频源。 

由以上步骤可以看出，视频播放是视频编辑其中的一种操作。

