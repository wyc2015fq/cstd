# 深度相机（五）--Kinect v2.0 - 工作笔记 - CSDN博客





2016年09月25日 22:20:07[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：12071
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









原文：[http://blog.csdn.net/qq1175421841/article/details/50412994](http://blog.csdn.net/qq1175421841/article/details/50412994)

----微软Build2012大会：Kinect for Windows Programming Deep Dive

    这个周末看了两天的微软[Build
 2012](http://channel9.msdn.com/Events/Build/2012/2-029)大会，真的是一次盛会，上面有很多的演讲主题，都是微软相关技术和产品的最新动态，我比较关注.NET、WPF、Office及Kinect相关开发，上面的演讲视频和PPT都可以下载，个人觉得比较精彩的有[The
 Evolution of .NET](http://channel9.msdn.com/Events/Build/2012/3-016) ，回顾了微软推出.NET以来的历次开发者大会推出的新版本和新特性，里面还有10多年前比尔盖子老师在开发者大会上宣布.NET战略时的演讲视频，不觉得感叹岁月是把杀猪刀啊。 [What's
 New for Developers in Office 2013 and SharePoint 2013](http://channel9.msdn.com/Events/Build/2012/2-008) 介绍了不同于以往的以VBA，SharedAddIn，以及VSTO的全新的Office开发方式OfficeApp，[Building
 apps for Office and SharePoint 2013 using the web technologies you know and love, Part 1](http://channel9.msdn.com/Events/Build/2012/3-020)、[Part2](http://channel9.msdn.com/Events/Build/2012/3-021)介绍了如何使用各种我们熟悉的技术如Html、Javascript、C#等来构建OfficeApp。上面还有很多关于Windows
 Phone 8，WPF4.5等相关的介绍，主题很多，强烈建议大家有空可以上去看看。

    大会上关于Kinect开发有三个，第一个是微软研究院讲的[Super-Natural
 Interaction](http://channel9.msdn.com/Events/Build/2012/2-007)这个PPT比演讲视频都大，达到了罕见的997M，为啥这么大呢，因为里面嵌入了一个长达40分钟视频，哈哈，这个演讲主要演示了微软研究院正在进行的各种人机交互，虚拟现实等研究，非常的Cutting-edge，里面也有和Kinect相关的部分，有兴趣的可以看看。第二个是[Kinect
 Design Considerations](http://channel9.msdn.com/Events/Build/2012/2-029)，一则关于Kinect应用程序设计是应该考虑的问题比如交互方式的设计的演讲，我没有太仔细看，不过内容应该和[Kinect
 Human Interface Guideline](http://download.microsoft.com/download/B/0/7/B070724E-52B4-4B1A-BD1B-05CC28D07899/Human_Interface_Guidelines_v1.5.0.pdf)内容差不多，您可以在Kinect Developer Toolkit中查看。第三篇演讲是 [Kinect
 for Windows Programming Deep Dive](http://channel9.msdn.com/Events/Build/2012/3-055)  我将这个翻译为了深入理解Kinect for Windows开发，相对来说该演讲和Kinect开发比较相关，现与大家分享，该演讲上面写的级别为 300-advanced，个人觉得这个演讲内容其实很简单，只是对Kinect能够获取的相关数据源，Kinect SDK处理过了的可供识别的数据源，以及未来的趋势和大家做了一下介绍。Kinect
 SDK中其实没有太多的东西，真正的则在于各种模式识别算法，比如通过深度数据，红外数据，进行各种物体识别等等，有了这些数据，尤其是1.6版本的SDK提供的获取红外原始数据，就可以通过这个数据，结合深度数据做出很多非常令人惊叹的应用来。废话不多说了，下面和大家分享一下该演讲的主要内容，也算是一个关于Kinect开发的比较好的入门介绍吧。

## 一 大纲

![Slide2](http://images.cnblogs.com/cnblogs_com/yangecnu/201211/201211050043077735.png)

    演讲大体分为5个部分，第一部分介绍了Kinect SDK的开发方式以及运行环境，第二部分是写代码，讲解Kinect中的一些彩色，深度，骨骼等数据的处理和显示，第三部分讲解了传感器直接产生的数据流，包括彩色，深度，红外，语音，加速器数据源，第四部分讲解了经过Kinect SDK对原始数据流处理后的可供用来进行直接识别的，骨骼，语音及面部识别数据源，最后一部分讲解了未来Kinect可进行的一系列应用。现在先来看第一部分吧。

## 二 Kinect应用场景及开发环境

    首先介绍了一下Kinect的应用场景。主要有三大类方面的应用。第一是自然人机交互界面。比如说一些像少数派报告中的那种用手指非接触即可操纵大屏幕上的显示内容。这在一些高科技或者科幻类电影中经常能够看到这类的场景。还有一些就是在国外已经有的应用如Kinect虚拟试衣间，Kinect车展演示广告，Kinect 手术室影像操作，还有[Kinect操作幻灯片](http://www.cnblogs.com/yangecnu/archive/2012/06/21/Kinect_Powerpoint_Control.html)，[Kinect进行照片浏览](http://www.cnblogs.com/yangecnu/archive/2012/08/22/Kinect_ImageViewer.html) 等等，这些自然人机交互界面的应用带来了新的用户体验。第二种是自然环境识别，比如根据Kinect
 产生的深度数据，红外数据对物体进行三维扫描重建，利用Kinect进行机器人导航进行障碍物自动回避等。第三种是自然用户的识别，比如说利用Kinect来进行姿势识别，人脸识别。比如说一些大家比较熟悉的XBOX360 中的 [舞林大会](http://www.amazon.com/Dance-Central-3-Xbox-360/dp/B002I0K3Z2/ref=sr_tr_sr_1?ie=UTF8&qid=1352039563&sr=8-1&keywords=Kinect)，[运动会](http://www.amazon.com/Kinect-Sports-Xbox-360/dp/B002I0JBVY/ref=sr_1_5?ie=UTF8&qid=1352039563&sr=8-5&keywords=Kinect)，[大冒险](http://www.amazon.com/Kinect-Adventures-Xbox-360/dp/B004HHIB8U/ref=sr_1_1?ie=UTF8&qid=1352039719&sr=8-1&keywords=kinect+adventures)等体感游戏，这些都是利用Kinect的一些数据来进行用户的动作识别，从而参与到游戏中的互动。还有一些增强现实的应用，利用了上面的三个方面的特性，比如说利用Kinect来将荧幕变成触摸屏，并在上面可以进行各种操作，等等应用。

![Slide3](http://images.cnblogs.com/cnblogs_com/yangecnu/201211/201211050043085194.png)

    要开发上面的应用，首先要了解一下开发环境，如这张幻灯片所示：

![Slide4](http://images.cnblogs.com/cnblogs_com/yangecnu/201211/201211050043095651.png)

    Kinect 提供了非托管(C++)和托管(.NET)两种开发方式的SDK，如果您用C++开发的话，需要安装Speech Runtime(V11)，Kinect for Windows Runtime和驱动的，如果您使用C#和VB.NET的话，需要Microsoft.Kinect.dll和Mirosoft.Speech.dll两个dll，这两个其实是对前C++里面的两个dll的.NET封装，不论何种开发，您都需要安装driver，所有这些都包含在Kinect
 SDK安装包中，安装方法您可以参考之前的[文章](http://www.cnblogs.com/yangecnu/archive/2012/03/30/KinectSDK_Geting_Started.html)。Kinect开发支持Windows7/Windows
 Embedded Standard/Windows8操作系统，最新的1.6版本的SDK还支持虚拟机里面的Windows系统，不过只要年代不太久远的Windows系统应该都是可以的。上面写的最好使用.NET 4.0/4.5也只是建议，老版本的应该也行。如果是使用.NET开发方式的话，您需要安装.NET Framework，IDE可以使用Visual Studio 2010/2012。

## 三 代码演示

    这部分主要是通过编写代码演示Kinect的各项功能，代码我在这里就不讲了。

    首先第一个演示是显示彩色影像，比较简单。然后是结合深度影像数据显示彩色影像数据上对应某一点的深度值。

![demo1](http://images.cnblogs.com/cnblogs_com/yangecnu/201211/20121105004310536.jpg)

     图上使用鼠标点击就能够加上一个标签，标签里面的值是该点的深度值，这个功能应该是比较好实现的，主要是展示如何使用彩色影像数据和深度数据，您如果感兴趣的话看完[这篇文章](http://www.cnblogs.com/yangecnu/archive/2012/04/04/KinectSDK_Depth_Image_Processing_Part1.html)应该就能实现这个功能。

    然后演示了骨骼追踪功能，图中追踪了头部，双手的位置，并显示了两只手所处的深度值，头部上显示的Tracking ID，Kinect能同时追踪6个待选目标，但只有两个目标处于活动状态。每一次追踪都会分配给目标一个Tracking ID。

![demo2](http://images.cnblogs.com/cnblogs_com/yangecnu/201211/20121105004311436.jpg)

这个功能主要演示了如何使用骨骼数据，您如果感兴趣的话看完[这篇文章](http://www.cnblogs.com/yangecnu/archive/2012/04/06/KinectSDK_Skeleton_Tracking_Part1.html)应该就能实现这个功能。

     除了1.5及之前能够提供的各种数据之外，1.6 的SDK提供了红外影像数据，您可以利用数据校准彩色影像数据，使得在较暗的条件下也能够进行人物识别。其实Kinect骨骼识别主要是通过深度数据来实现的，而深度数据是通过红外发射接收产生的，1.6版本的SDK提供了我们直接访问和操作红外数据的能力。

![demo7](http://images.cnblogs.com/cnblogs_com/yangecnu/201211/201211050043112879.jpg)

    第四个Demo是演示了Kinect的面部识别功能，Kinect的面部识别是在1.5 SDK中引入的，[面部识别](http://msdn.microsoft.com/en-us/library/jj130970)可以识别最多达一百多个面部特征点，下面是演示的Demo：

![demo3](http://images.cnblogs.com/cnblogs_com/yangecnu/201211/201211050043125811.jpg)

    上面的功能在Kinect Developer Toolkit中有实例和源代码，您可以下载查看，入门的话，您也可以看[这篇文章](http://www.cnblogs.com/yangecnu/archive/2012/10/12/KinectSDK_FaceTracking.html)，比较简单的对使用FaceTracking的一点介绍。

    Kinect除了这些功能之外还有强大的语音识别功能，在这篇演讲中没有演示出来，不过在[Super-Natural
 Interaction](http://channel9.msdn.com/Events/Build/2012/2-007)这篇演讲的视频中，您可以体会该改功能的强大，Kinect语音识别不仅能够识别出声音，而且还能对声音的来源方向进行识别，且具有强大的去噪增强功能，要了解这些你可以看[这篇文章](http://www.cnblogs.com/yangecnu/archive/2012/05/04/KinectSDK_SpeechRecognition_part2.html)，对Kinect的语音识别做了一些简单的介绍。

## 四 Kinect数据源及应用

    Kinect开发涉及到的数据源分为两类：

    一类是传感器本身产生的原始数据源，比如说彩色影像数据源，深度影像数据源，语音数据源，红外影像数据源，加速计数据源，其中红外影像数据源，加速计数据源是在1.6的SDK中新加入的。下面这张图很好的说明了各个数据源：

![Sensor Resource Image](http://images.cnblogs.com/cnblogs_com/yangecnu/201211/201211050043132124.png)

    第二类数据源是，SDK中通过一些算法识别出来的可以直接拿来进行识别的数据源，他们包括：骨骼追踪，语音识别，面部识别数据源。

![Recognize source](http://images.cnblogs.com/cnblogs_com/yangecnu/201211/2012110500431473.png)

    可以看到，骨骼追踪数据源是在深度影像数据源的再通过一系列算法实现的，语音识别是通过语音数据源再通过一系列算法实现的，而面部识别则综合了彩色影像、深度影像和骨骼追踪为系列数据源的再通过一系列算法实现的。这些功能都是通过SDK来实现的。

    有了这些数据源，就能够进行各种应用了：

![application](http://images.cnblogs.com/cnblogs_com/yangecnu/201211/201211050043154467.png)

    典型的应用有绿屏抠像，这个功能在一些电视节目录制，比如天气预报节目中都是应用的这个实现的，他把任务从背景中分离出来，然后可以随意的更换背景，这个功能用到了影像数据和深度数据，你可以参考[这篇文章](http://www.cnblogs.com/yangecnu/archive/2012/04/05/KinectSDK_Depth_Image_Processing_Part2.html)，里面有一个简单的例子。第二个应用是产生点阵云，就是通过Kinect的深度影像数据产生每个点的位置信息然后生成三维模型。如下面这个例子：

![demo4](http://images.cnblogs.com/cnblogs_com/yangecnu/201211/201211050043167400.jpg)

    图中左边是通过Kinect的深度影像数据产的三维建模图像。

    第三个应用是魔镜功能，我觉得应该是一些类似哈哈镜效果或者是IOS中的Photo Booth应用，主要用到了彩色影像数据，面部识别，骨骼识别等功能。还有一些就是虚拟试衣间这样的功能，最后的一个应用场景就是各种NUI交互界面了。

## 五 展望

    除了以上的几种典型的数据源之外，还展示了下一步的计划，比如说针对景深数据进行进一步处理的Kinect Fusion数据源。

![next](http://images.cnblogs.com/cnblogs_com/yangecnu/201211/201211050043178937.png)

    下面是演示的例子：图中，右边桌子上摆放了一个茶壶，然后利用Kinect对该茶壶进行了三维建模，然后。移除右边实物茶壶后，用户可以对虚拟的茶壶进行各种操作，神奇吧，这些都是下一个版本或者是将来的SDK能够方便我们或者简化我们实现这些功能准备添加的功能。

![demo6](http://images.cnblogs.com/cnblogs_com/yangecnu/201211/201211050043186645.jpg)

到最后展望了下一步要进行的工作：

![Slide22](http://images.cnblogs.com/cnblogs_com/yangecnu/201211/201211050043193166.png)

    可以看出Kinect的愿景是使得计算机能够看到、听到、能够更加理解周围的人和环境。

    Kinect SDK的版本发布是很快的，自从今年二月份发布了Kinect Sensor for Windows 体感仪及Kinect for Windows SDK 1.0官方版本以来；5月21日 发布了[1.5版本SDK](http://www.cnblogs.com/yangecnu/archive/2012/05/22/New-features-in-Kinect-for-Windows-SDK1_5.html)以及1.5的Developer
 Toolkit及调试工具Kinect Studio 1.5，不久又发布了Developer Toolkit 1.5.1及Developer Toolkit 1.5.2；10月9日发布了[1.6版本的SDK](http://www.cnblogs.com/yangecnu/archive/2012/10/09/New-features-in-Kinect-for-Windows-SDK1_6.html)和Developer
 ToolKit，同时宣布正式在大陆开售Kinect Sensor for Windows体感仪。每一个版本都增加了一些新的功能。相信下一个版本的SDK 也会有新的功能加入。

    相信大家看了这个演讲之后应该可以感受到Kinect带来的变化，也相信以后Kinect开发能应用到日常生活中的例子会越来越多。

    最后，希望大家有空到Channel9上看真人演讲哦，上面还有很多有意思主题演讲，所有演讲的PPT都可以在线看，视频都提供下载，当您不确定那个演讲想不想看时，可以看看下面的在线屁屁踢哦，当然除了那种900多M的变态PPT之外其他的都可以在线浏览，这样您就可以迅速的了解演讲内容以及对该内容有没有兴趣啦。

关于Kinect 网上资料很多，不做详细描述。

这里主要记录一下V1.0和V1.2的区别。原文：[http://www.cnblogs.com/TracePlus/p/4136297.html](http://www.cnblogs.com/TracePlus/p/4136297.html)

本文主要是对传感的配置和运行条件进行彻底的比较。

  本连载介绍的Kinect for Windows Developer Preview是暂定的，软件、硬件以及API有可能因为最终的产品版发生变更，还请谅解。





**关于本连载**



  本连载主要是比较次世代型的Kinect for Windows（后面称作Kinect v2预览版）和现行型的Kinect
 for Windows（后面称作Kinect v1）的同时，介绍面向c++开发者而进化的硬件和软件。（本网站也发布了对应C#/Visual Basic开发者的内容，.NET开发者可以同时参考[新型Kinect
 for Windows v2 Developer Preview programing入门]）




**次世代型的****Kinect for Windows**



  2012年美国微软发售的Kinect v1，因为可以很方便就能取得Depth（深度）和 skeleton（人物姿势）等信息，被全世界的开发者和研究人员关注。

  2014年预定发售的Kinect v2，预测在硬件和软件上会做很大的进化，在销售前，开发者向的预览版的Kinect
 v2(传感器)和SDK v2（软件开发套件）很早就发布了出来。



  还有，因为这个开发者向的早期提供程序是必须签订NDA(密码保持契约)，本稿有不能公布的事项也事先请各位谅解。



**Kinect v1和Kinect v2预览版的外观比较**

Kinect v1(图1)和Kinect v2 预览版(图2)的外观的照片。

![](https://img-blog.csdn.net/20160925221751048)

**图1 Kinect for Windows v1(现行型)**



  Kinect v1的Depth传感器，采用了「Light Coding」的方式，读取投射的红外线pattern，通过pattern的变形来取得Depth的信息。为此，Depth传感器分为投射红外线pattern的IR
 Projector（左）和读取的这个的IR Camera（右）。还有Depth传感器中间还搭载了Color Camera。


  Light Coding是以色列的PrimeSense公司的Depth传感器技术，于2013年被美国苹果公司收购。

![](https://img-blog.csdn.net/20160925221755829)

**图2 Kinect for Windows v2(次世代型)预览版**



  Kinect V2预览版的Depth传感器，采用的是「Time of Flight(TOF)」的方式，通过从投射的红外线反射后返回的时间来取得Depth信息。Depth传感器看不到外观，不过Color Camera旁边是红外线Camera(左)和投射脉冲变调红外线的Porjector（右）。



  微软过去收购过使用TOF方式处理Depth传感器技术的公司（注：应该是指的3DV），已经在使用这个技术，不过没有详细的公布。



**Kinect v1和Kinect v2预览版的配置比较**



  Kinect v1和Kinect v2预览版的传感器的配置比较在表1显示。
||Kinect v1|Kinect v2预览版|
|----|----|----|
|640×480|1920×1080| |
|30fps|30fps| |
|320×240|512×424| |
|30fps|30fps| |
|6人|6人| |
|2人|6人| |
|20関節／人|25関節／人| |
|△（Developer Toolkit）|○（SDK）| |
|0.8～4.0m|0.5～4.5m| |
|57度|70度| |
|43度|60度| |
|○|×（手動）| |
|×（単一的App）|○| |

** 表1是Kinect v1和Kinect v2预览版的传感器的配置比较**



Kinect v1的Color Camera的分辨率是640x480较低，不能取得非常漂亮的图像，Kinect v2预览版的分辨率大幅提高，能取得1920×1080非常漂亮的图像（图3）。

（注：v1的要求是USB2.0理论传输速率是60MB/s，v2是USB3.0理论传输速率是500MB/s。可以计算一下，以XRGB Color为例，30fps，那么每秒所需传输的数据大小为640
 x 480 x 4 x 30约为35M；再加上USHORT格式的Depth Color，30fps，大小为320 x 240 x 2 x 30约为4M。总计约为40MB/s，因为带宽有限，所以在保证画面帧率稳定的情况下，分辨率只能如此，而且基本上必须独占一个USB
 Controller。再算算v2的情况，Color =1920 x 1080x 4 x 30 约为237M，Depth = 512 x 424 x 2 x 30约为12M，总计约为250M/s。所以非USB3.0不可，否则传输不了这么大的数据量。显而易见，Color
 Map是最占带宽的，其实可以通过一些其他格式，比如I420或MJPG来减少数据量，然后通过CPU或GPU来进行解压和回放。）



![](https://img-blog.csdn.net/20160925221800986)

![](https://img-blog.csdn.net/20160925221804342)

**图3 Kinect v1和Kinect v2预览版的Color**

  Kinect v2预览版的Depth传感器的分辨率也提高到512×424，而Kinect v1是可以取640×480分辨率的Depth数据，乍一看规格好像下降了，其实Kinect v1的Depth传感器的物理分辨率是320x240，Up
 Sacling到640x480而已（注：猜测是Runtime处理的）。另外，Depth传感器的方式也是从Light Coding变更为Time of Flight(TOF)。



  不能详细叙述，不过Kinect V2预览版Depth数据的精度也提高了(图4)，关于精度还敬请等待产品版。

![](https://img-blog.csdn.net/20160925221810798)

![](https://img-blog.csdn.net/20160925221813686)

**图4 Kinect v1和Kinect v2预览版的Depth**

   Kinect v1，v2预览版可以取得Player（可识别的人体）数量都是6人。Kinect v2预览版因为Depth传感器的分辨率提高了，用Player数据只需要简单的剪切就可以很漂亮得把背景和人物分离。

   Kinect v1可以取得全部关节（Joint）的skeleton的数量是2人，随着Depth传感器的分辨率上升和视角的宽广，Kinect v2预览版变得能取得6人。



    还有，Kinect v1能取得的Joint是20个Joint每人，Kinect v2预览版变为能取得25个Joint。具体的如图5所示，头(Neck)，指尖（HAND_TIP_LEFT，HAND_TIP_RIGHT），大拇指（THUMB_LEFT，THUMB_RIGHT），增加了这5个Joint。不仅仅是手的位置，大拇指和指尖的细小信息也可以获取到。



Hand State（手的开闭状态）的识别，Kinect
 v1是靠Developer Toolkit里的「Kinect Interaction」库来支持，不过在Kinect V2预览版SDK里是标准支持。

![](https://img-blog.csdn.net/20160925221819626)

![](https://img-blog.csdn.net/20160925221824595)

Kinect v1和Kinect v2预览版可以取得的Joint

Kinect v1为了摇头装载了倾斜电机(Tilt motor)，也有视角扩展，Kinect v2预览版没有搭载Tilt motor，靠手动来摇头。



Kinect v1不能多个应用程序同时连接到一个传感器。Kinect v2预览版通过「Kinect Service」，可以让多个应用程序同时从传感器取得数据（参考图6）

现在，Kinect Service作为常驻程序被提供， 一般认为产品版里会成为Windows的服务(Service)。



![](https://img-blog.csdn.net/20160925221831705)

**图6 通过Kinect Service，对应多个应用程序**

  Kinect v1和Kinect v2 预览版的运行环境的比较表(表2)。

||Kinect v1|Kinect v2预览版|
|----|----|----|
|Windows 7以后|Windows 8以后| |
|Visual Studio 2010以后|Visual Studio 2012以后| |
|USB 2.0|USB 3.0| |
|Dual-Core 2.66GHz|Dual-Core 2.66GHz| |
|DirectX 9.0c|DirectX 11.0| |
|2.0GBytes|2.0 GBytes| |
表2　Kinect v1和Kinect v2预览版的最小运行环境比較



  Kinect v1要在Windows 7以后的版本上运行，Kinect v2要求是在Windows 8 运行。关于Visual Studio也要求是2012以后的版本。




  Kinect v1要求USB 2.0（或更快的USB）来运行，因为Kinect 2预览版传感器的分辨率也提高了，需要更快的USB 3.0来运行。Kinect v1和Kinect v2预览版的专有USB总线带宽都没有变化。



Kinect v1和Kinect v2预览版都有与部分USB Host Controller不兼容而导致不能正常运行的情况，现在是Renesas和Intel的USB 3.0 Host
 Controller可以运行。台式PC也可以增加USB3.0扩展卡来对应。



  CPU方面，和Kinect v1一样，要求Dual
 Core 2.66 GHz以上。「时钟频率较低」一类的运行环境也稍微下降了，不是特别差的情况都可以运行，不过注意传感器分辨率提高，取得的数据的处理消耗也上升了。




  Kinect v1要求的是支持DirectX 9.0c的GPU（Kinect Fusion除外），Kinect
 v2预览版要求支持DirectX 11.0以后的GPU，像笔记本这种没有装载NVIDIA GeForce和AMD Radeon外置GPU（独立显卡）的很多无法运行，而像有Intel
 HD Graphics这种支持DirectX 11.0以后的处理器内置的GPU（集成显卡）是可以运行的。



   如上展示了Kinect v2预览版的必要运行环境，和前述一样在产品版中有变更的可能性，现在还不需要着急准备对应环境。关于USB Host
 Controller的兼容性今后也有解决的可能，希望起到参考的作用。



**总结**

  这次彻底的比较了Kinect v1和Kinect v2预览版的传感器配置和必要运行环境。

补充：

1.V1的程序不能在V2上运行，也就是V2不向下兼容。

2.差不多与V2同时上市的，还有微软在上海自贸区生产的游戏机，就是Xbox One。这个在中国是合法销售的。这个游戏机如果配一台感应器，也能玩体感游戏。这台感应器叫“Kinect for Xbox One”。这个时候，微软在中国已经有了两台感应器：用于体感游戏机的叫Kinect for Xbox One，用于PC编程的叫Kinect for Windows V2。问题来了。V2只能用作开发，Xbox One只能玩游戏。如果你又想玩游戏，又想搞编程开发，就得买两种感应器Kinect for Xbox One和V2。于是，微软对这种复杂的硬件组合进行了优化，把这两种感应器精简为一种，它的名字叫：“Xbox One专用Kinect感应器”。为了节省篇幅，我们在后面把它简称为“全能感应器”。这个“全能感应器”的功能又强大了，它可以兼容Xbox游戏机。如果再加个小设备，就可以兼容PC机。这个小设备叫“适配器”。
﻿﻿



﻿﻿

﻿﻿



