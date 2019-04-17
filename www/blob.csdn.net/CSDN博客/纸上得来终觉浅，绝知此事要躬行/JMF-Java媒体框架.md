# JMF-Java媒体框架 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年05月19日 13:59:35[boonya](https://me.csdn.net/boonya)阅读数：962








Java术语，意为Java媒体框架（JMF）。该核心框架支持不同媒体（如：[音频输出](http://baike.baidu.com/edit/JMF/3792189#)和视频输出）间的[时钟同步](http://baike.baidu.com/edit/JMF/3792189#)。它是一个标准的扩展框架，允许用户制作纯[音频流](http://baike.baidu.com/edit/JMF/3792189#)和[视频流](http://baike.baidu.com/edit/JMF/3792189#)。



JMF实际上是Java的一个类包。JMF 2.1.1技术提供了先进的媒体处理能力，从而扩展了[Java平台](http://baike.baidu.com/edit/JMF/3792189#)的功能。这些功能包括：媒体捕获、压缩、流转、回放，以及对各种主要媒体形式和编码的支 持，如[M-JPEG](http://baike.baidu.com/edit/JMF/3792189#)、H.263、MP3、RTP/RTSP
 ([实时传送协议](http://baike.baidu.com/edit/JMF/3792189#)和实时流转协议)、Macromedias Flash、IBM的HotMedia和Beatniks的Rich Media Format (RMF)等。JMF 2.1.1还支持广受欢迎的媒体类型，如Quicktime、Microsofts AVI和[MPEG-1](http://baike.baidu.com/edit/JMF/3792189#)等。此外，JMF
 2.1.1软件中包括了一个开放的媒体架构，可使开发人员灵活采用各种媒体回放、捕获组件，或采用他们自己的定制的内插组件。

Sun以四种不同的专用版本提供JMF 2.1.1技术，满足专业开发人员的各类需求，第一个是一个轻便型版本，它完全采用[Java语言](http://baike.baidu.com/edit/JMF/3792189#)编写，适用于任何Java兼容系统。此外，开发人员还可选 择分别适用于Solaris、Windows或Linux等操作系统的性能最优化软件包，以提高性能和能力。

JMF 2.1.1软件也是Java Media系列软件的一部分。Java Media系列软件包括Java 3D、Java 2D、Java Sound和Java Advanced Imaging等API。采用各种Java Media API，软件开发商人员就能容易、快速地为他们已有的各种应用程序和客户端Java小程序增添丰富的媒体功能，如流式视频、3D图像和影像处理等。就是 说，各种Java Media API发挥了Java平台的固有优势，将"编写一次，到处运行"的能力扩展到了图像、影像和数字媒体等各种应用领域，从而大大缩减了开发时间和降低了开发
 成本。

JMF系统介绍

[ Adigan 发表于 2005-10-2 17:25:15 ]

一.简介

1.1JMF 体系结构简介

在开发JMF 应用程序之前要完全理解JMF 的体系架构、接口和类。就拿我们的家用摄像机系统作个例子。首先用摄像机拍摄内容，拍摄下来的内容录制在DV带中。DV带可以放在放像机里播放、放像机提供视频信号给电视机，提供[音频信号](http://baike.baidu.com/edit/JMF/3792189#)给音箱，这样我们就可以在电视机上看到画面，从音箱里听到声音。JMF API 提供的也是同样的模型。

JMF 提供的模型可大致分为七类

* [数据源](http://baike.baidu.com/edit/JMF/3792189#)（Data source）

* 截取设备（Capture Device，包括视频和音频截取设备）

* 播放器（Player）

* 处理器（Processor）

* 数据池（DataSink）

* 数据格式（Format）

* 管理器（Manager）

1.2 如何应用JMF捕获媒体数据

可以应用JMF从摄像头和麦克风中采集数据，采集后的数据可以被处理、保存、渲染为以后应用。

捕获媒体数据需要做以下工作。1.定位所需要用的捕获设备，可以通过查询CaptureDeviceManager来定位。2.获取这个捕获设备的信息CaptureDeviceInfo对象。3.从CaptureDeviceInfo对象中获取捕获设备的位置Medialocator。4.利用MediaLocator创建DataSource。5.使用DataSource创建Player或是Processor。6.然后启动Player就开始了媒体的捕获。

如果只是想观看捕获的内容，只需要使用捕获数据源创建播放器Player即可。如果想把捕获的数据发送到网络或者保存起来，就需要创建处理器Processor。

可以从CaptureDeviceManager中获取捕获设备的信息。CaptureDeviceManager是可在JMF中使用的全部捕获设备的注册中心。可以通过调用CaptureDeviceManager的getDeviceList方法获取可用的捕获设备列表。

每个捕获设备都由CaptureDeviceInfo对象代表，要获取一个特定的设备只需要调用CaptureDeviceManager的getDevice方法。

从特定的设备捕获媒体数据，需要从它的CaptureDeviceInfo对象中获取这个设备的位置信息MediaLocator。可以用MediaLocator直接构建一个Player或者Processor。还可以用MediaLocator构建一个DataSource作为Player的输入。当Play开始的播放的时候，就开始捕获过程。

一个捕获设备通常有一系列属性可以进行配置。通常控制分为2种，一种是端口控制PortControl，另一种是监视MonitorControl。可以通过调用捕获DataSource的getControl方法，来获取这两种控制对象。

和其他的控制对象一样，对于PortControl或者MonitorControl都可以通过调用getControlCompent方法来得到它们的可视化组件。这些组件可以添加到Applet中或者应用程序中，这样用户可以和这些控制对象交互。

处理显示控制组件，还可以显示标准的Player控制组件。

1.3如何使用RTP接收和发送流媒体数据

JMF Player 和 Processor 可以提供显示、获取、转换RTP 数据流的机制。

可以按照标准的方式通过Manager为RTP数据流创建Player。使用包含RTP Session的MediaLocator作为参数，调用Manager的createPlayer(MediaLocator)来创建Player。也可以用某个接收数据流的DataSource作为参数来创建Player。

如果用MediaLocator作为参数来创建Player, Player 只会显示Session中检测到的第一个RTP 数据流。如果想播放Session中的所有[视频流](http://baike.baidu.com/edit/JMF/3792189#)，则需要调用SessionManager为每个接收数据流创建一个Player。

当使用MediaLocator为RTP Session 创建Player，Manager为Session中的第一个数据流创建Player。当检测到数据时，Player发出RealizeCompleteEvent。

通过监听RealizeCompleteEvent,可以确定数据是否到了以及Player是否可以播放任何数据。一旦Player发出这个事件，就可以获取Player的可视组件和控制组件。

要注意的是，如果在没有检测到Session中数据时，Player 是不会结束Realizing状态的。对于Rtp 媒体流Player 不能试图调用Manager的CreateRealizedPlayer。如果调用，会引起死锁。

1.4如何在网络发送流媒体数据

在发送RTP数据流之前，需要应用Processor产生RTP编码的数据源。通过构建一个SessionManager或者是DataSink来控制传输过程。

Processor处理器的输入可以是现有的媒体文件，也可以是实时捕获的数据。对于现有的媒体数据，可以使用MediaLocaotr来代表媒体文件。并根据MediaLocator来创建Processor。对于捕获的媒体数据，一个捕获数据源Capture DataSource 会用作Processor的输入。

有两种方式来发送RTP数据流，一种是用MediaLocator作为RTP session的参数，调用Manager的createDataSink方法来创建一个RTP DataSink。另一种方式是利用session manager 来创建发送数据流，并控制传输过程。

当利用MediaLocator来构建RTP DataSink的时候，你只能发送DataSource中的第一个数据流。如果需要在一个进程中发送多个RTP数据流或者需要监控进程的统计状态，就需要直接使用SessionManager。

无论采用哪种方式传输RTP数据流，都需要做如下工作：

1、使用DataSource创建Processor,这个DataSource代表你要传输的数据

2、设置Processor的输出为RTP 编码的数据

3、取得Processor的输出作为DataSource

二.具体的试验-应用JMF开发视频应用

2.1配置开发环境

需要J2SDK，JRE1.42, JMF Windows Performance Pack,JMF 的 下载地址：http://java.sun.com/products/java-media/jmf/2.1.1/setup.html 针对WINDOWS 用户，可下载Windows Performance Pack。只有安装Windows Performance Pack才支持视频、音频捕获。下载后会自动安装，自动配置路径。安装后需去测试一下是否安装成功。测试地址：http://java.sun.com/products/java-media/jmf/2.1.1/jmfdiagnostics.html

2.2用APPLET在浏览器中播放视频短片

在浏览器中实现APPLET 视频播放中用到了两个基本概念 manager ,和player。JMF APPLET 是播放视频文件的过程是这样的。首先,提供媒体文件的位置作为创建PLAYER的参数。Player创建后会根据媒体文件的格式寻找解码器，并且进行初始化。然后为PLAYER 添加一个ControllerListener，用来监听PLAYER的状态。通过监听player 的状态采取不同的措施。在Player 发出RealizeCompleteEvent的时候，可以获取player 的VisualComponent
 宽度，高度，VisualControllpane的宽度，高度。最后一步调用player的start方法，就可以显示媒体文件的内容了。如果不使用PLUGIN，JMF 只支持标准的AVI,MOV,WAV 文件。对于流行的MPEG4视频文件，MP3并不支持。通过安装解码器，可以播放这些文件。

2.3采集摄像头数据

采集摄像头数据，需要两步工作。连接视频设备，然后获取视频设备的数据。视频，音频设备在Jmf Performance Pack 安装时会在JMF Registry中注册自己的设备名。需要连接摄像头时，可以通过查找摄像头的设备名来连接摄像头。具体的实现步骤，首先要得到Device的名字，有两种方法可以办到，

方法一

通过菜单，把系统支持的Device都列出来，根据客户的选择来确定。

方法二

用程序来判断。在第二种方法中，通过传递一个Format 对象到CaptureDeviceManager的getDeviceList 方法中，获得Device的名字。得到Device 的名字以后，可以获得CaptureDeviceInfo，从而得到MediaLocator。第一部分连接摄像头的工作就完成了。下一步工作是捕获摄像头视频，并显示出来。有视频设备构成的数据源可分为2种，例如照相机这种设备，属于Pull DataSource,用的时候才使用。类似于摄像头这种设备构成的数据源属于Push Datasource,源源不断的发送视频数据。以获取的MediaLocator
 作为参数构建Player,就可以播放摄像头内容了。

2.4获取摄像头视频数据和麦克风音频数据

基本步骤是首先获取视频、音频捕获设备的信息，然后再根据取得的设备信息确定设备的位置。找到设备的位置以后,调用Manager的CreatePlayer方法创建Player，当Player开始播放的时候，就开始了捕获的过程。

有多种方法获得捕获设备信息，可以通过查询指定的格式，也可以查询特定的设备名字。具体步骤如下：

1. 先把所有的捕获设备调出来

2. 所有的视频捕获设备的名字都是以vfw开头的，所以取得捕获设备列表中以vfw 开头的设备名字就是视频捕获设备

3. 获取设备的medialocator地址

4. 创建Player,当Play开始的时候，就开始了视频捕获的过程

2.5保存摄像头数据为视频文件

首先是找到捕获设备，然后取的捕获设备的位置信息。如果要保存所采集的数据，就要创建一个处理器。然后将处理器的输出作为数据池的输入。当启动数据池的时候，就可以把捕获的数据保存成文件了。具体步骤如下：

1. 获取视频捕获设备的medialocator地址

2. 以获取的medialocator为参数，创建Processor

3. 设置Processor,输出类型为AVI格式

4. 以Processor的输出为参数，创建数据源

5. 建立一个保存文件位置的MediaLocator

6. 以输入数据源和输出数据源为参数，建立一个DATASINK

7. 调用Processor的StreamWriterControl,设置Processor控制生成文件的大小

8. 设置完毕，可以启动Processor和DataSink来保存捕获数据了

2.6 捕获音频、视频数据为QuickTime格式的文件

使用处理器模型来捕获实时的视频、音频数据并且将数据轨道进行编码、混合，最后保存为QuickTime 格式的文件。

首先构建一个处理器模型，设定好特定的数据轨道和输出内容格式。然后使用这个处理器模型来创建一个处理器。处理器会自动连接到符合文件格式要求的捕获设备。具体步骤如下：

1. 设定要输出的音频格式和视频格式

2. 设定要输出的文件类型为QuickTime格式

3. 以数据轨道输出格式和文件输出类型为参数，创建ProcessorModel

4. 由ProcessorModel对象创建Processor对象

5. 以Processor的输出为参数创建数据源

6. 建立一个记录保存文件的MediaLocator

7. 创建DataSink

8. 开始保存数据文件，启动处理器

9. 当结束捕获视频时，停止处理器，并关掉它

10. 当数据池接到EndOfStream事件时，停止保存文件。

2.7 发送实时媒体数据

如果需要将媒体数据发送到网络，首先需要用Processor生成RTP编码的数据源，然后创建SessionManager或者DataSink来控制传输过程。具体步骤如下：

1. 获取[视频采集](http://baike.baidu.com/edit/JMF/3792189#)设备

2. 创建Processor，连接到视频采集设备

3. 等待，直到processor的状态变成configured,设置输出格式为RAW_RTP

4. 列出所有的和数据轨道相关的控制器

5. 遍历这些控制器，找到可以控制输出格式的控制器

6. 找到了数据轨道的格式控制器之后，设置输出格式，输出格式输出为VideoFormat.JPEG_RTP

7. 准备工作已经做好，可以调用Processor的realize方法

8. 当Processor进出realized状态以后，可以取得Processor的输出作为数据源

9. 可以把这个数据源作为[参数传递](http://baike.baidu.com/edit/JMF/3792189#)给manager,通过manager创建一个RTP的数据池。

10. 先设置一个发送数据流的多播目的网址

11. 确定了数据源和发送地址，可以建立数据池DataSink了,建立成功后调用open()和start()方法，就可以在网上以[多播](http://baike.baidu.com/edit/JMF/3792189#)方式发送捕获的内容了

2.8 接收实时视频数据

接收网上传输的数据流时，首先要构建一个MediaLocator来描述RTP 进程，Manager为进程中的第一个数据流创建播放器，当从进程中检测到数据时，Player会发出RealizeComplete事件。

通过监听RealizeComplete事件，你可以确定是否有数据到了，还可以确定Player是否准备好播放数据了。当Player发出这个事件时，就可以获取播放器的可视面板和控制面板了。具体实现步骤如下：

1. 确定RTP 数据源的地址，例如：String url= "rtp://224.144.251.104:49150/audio/1"

2. 以RTP数据源地址为参数，创建MediaLocator

3. 以MediaLocator为参数，创建Player

4. 为PLAYER添加监听器，以便监听RealizeComplete事件

三. 实验效果总结

JMF2.0 支持媒体捕获，并且考虑到了程序开发者需求。允许程序开发者在媒体处理、媒体控制上添加自己的控制器。JMF 提供的[插件](http://baike.baidu.com/edit/JMF/3792189#)的架构使得直接访问媒体数据成为可能。这样JMF可以更方便的被扩展。JMF的设计目的是使开发简便、支持媒体捕获、支持使用JAVA技术开发媒体传输以及视频会议等应用、支持高级开发者开发插件扩展JMF应用，新的特性还可以和原有的系统很好的融合。允许开发者开发定制的，可下载的编码器、处理器、分离器和组合器。

JMF的类库还可以提供对RTP（[实时传输协议](http://baike.baidu.com/edit/JMF/3792189#)）的支持。RTP支持在网上实时的发送、接收媒体流。RTP可以用在开发视频点播、音频点播或者网上的交互内容服务等应用。

四. 进一步的研究工作

本文研究了JAVA视频应用基本内容，为进一步扩展JMF的应用开发打下了基础。在这一段的研究中积累的很多经验。由于时间的关系，只是初步了解了如何通过[插件](http://baike.baidu.com/edit/JMF/3792189#)技术扩展JMF的应用能力。今后需要做的主要研究工作和研究方向主要有

1、编写解码器插件，扩展JMF的应用能力。使得JMF可以支持更多的媒体类型;

2、编写处理器插件，增强JMF的视频处理能力，使得JMF的[视频编辑](http://baike.baidu.com/edit/JMF/3792189#)能力更强。

文章来自：http://baike.baidu.com/link?url=qBMbAeQ35hPx5htgYgxC4LuPlax_sJp7X3WmkLuZG0NoWoR7J9dawW16AUauUJOj200TVmLCB9_t8tKy3vMvjK






# Downloads



[JMF Home Page](http://www.oracle.com/technetwork/java/javase/tech/index-jsp-140239.html)



**Download JMF 2.1.1e Software **

![Download](http://www.oracle.com/ocom/groups/public/@otn/documents/digitalasset/148599.gif)

**Hardware Requirements **
- 166 MHz Pentium, 160 MHz PowerPC, or 166 MHz UltraSparc 
- 32 MB RAM or greater 
- Optional: An appropriate sound card for audio play back, if necessary. For example, a SoundBlaster-compatible card for Windows machines without built-in audio support, or an Ultimedia sound card for AIX machines without built-in audio
 support. 

**Installation Instructions **
- **Download the version of JMF you want to install by selecting a download format and and clicking the continue button.**
The downloaded file is an executable. For Windows, it's an InstallShield executable; for Solaris, it's a self-extracting shell script; otherwise it's a generic zip file.




- **If you are installing the JMF Performance Pack for Windows**

- Run the executable by double-clicking on the 
**jmf-2_1_1e-windows-i586.exe**
**NOTE**: The Windows installation program automatically detects whether or not Netscape Communicator 4.03 or later is installed and then installs JMF for its use! If you are using Netscape Communicator 4.04 or 4.05, then
 you *must* install the JDK 1.1 patch for Netscape Communicator before you can run any JMF applets. Netscape Communicator 4.06 or greater already supports JDK 1.1 without the patch.



**If you are installing the JMF Performance Pack for Solaris SPARC**


- Change directories to the install location. 
- Run the command % **/bin/sh ./jmf-2_1_1e-solaris-sparc.bin**

**If you are installing the JMF Performance Pack for Linux**


- Change directories to the install location. 
- Run the command % **/bin/sh ./jmf-2_1_1e-linux-i586.bin**

**If you are installing a zip file**


- Run the appropriate zip command for your system, e.g. **winzip** or **unzip** to extract JMF onto your system. 



- **IMPORTANT: You must finish setting up JMF by following the[setup instructions](http://www.oracle.com/technetwork/java/javase/setup-138642.html) included in the JMF Implementation Documentation. **




