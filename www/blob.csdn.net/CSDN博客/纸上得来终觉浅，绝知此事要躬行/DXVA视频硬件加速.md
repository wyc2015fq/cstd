# DXVA视频硬件加速 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年09月26日 09:49:37[boonya](https://me.csdn.net/boonya)阅读数：342








原文地址：[https://baike.baidu.com/item/dxva/2860652?fr=aladdin](https://baike.baidu.com/item/dxva/2860652?fr=aladdin)

**About DXVA 2.0:[https://docs.microsoft.com/en-us/windows/desktop/medfound/about-dxva-2-0](https://docs.microsoft.com/en-us/windows/desktop/medfound/about-dxva-2-0)**

**DirectX Video Acceleration:[https://en.wikipedia.org/wiki/DirectX_Video_Acceleration](https://en.wikipedia.org/wiki/DirectX_Video_Acceleration)**

**DXVAChecker:[http://bluesky23.yukishigure.com/en/DXVAChecker.html](http://bluesky23.yukishigure.com/en/DXVAChecker.html)**

DXVA是DirectX Video Acceleration的简称，中文译为视频[硬件加速](https://baike.baidu.com/item/%E7%A1%AC%E4%BB%B6%E5%8A%A0%E9%80%9F)。DXVA是微软公司专门定制的[视频加速](https://baike.baidu.com/item/%E8%A7%86%E9%A2%91%E5%8A%A0%E9%80%9F/10256205)规范，它共有两个版本，分别是DXVA 1.0和DXVA 2.0。DXVA规范不仅仅是在视频播放过程中的解码计算规范，其还有与视频后期处理息息相关，例如[反交错](https://baike.baidu.com/item/%E5%8F%8D%E4%BA%A4%E9%94%99/8661466)、[色彩空间](https://baike.baidu.com/item/%E8%89%B2%E5%BD%A9%E7%A9%BA%E9%97%B4/4615427)转换或[图像缩放](https://baike.baidu.com/item/%E5%9B%BE%E5%83%8F%E7%BC%A9%E6%94%BE/4396039)等等。对于视频播放时CPU利用率影响最大的，通常也是我们最关心的，非视频解码莫属，尤其是 HD视频的解码（[硬件解码](https://baike.baidu.com/item/%E7%A1%AC%E4%BB%B6%E8%A7%A3%E7%A0%81/3649897)），对于CPU较弱的系统来说，让显卡来完成解码是不二之选。

![](https://img-blog.csdn.net/20180926094141753?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 概述

### 硬件加速

当前市面上几乎所有的显卡都具备一定的视频[硬件加速](https://baike.baidu.com/item/%E7%A1%AC%E4%BB%B6%E5%8A%A0%E9%80%9F/5702432)能力。微软公司为此专门制订了DXVA规范（包括DXVA 1.0和DXVA 2.0两个版本）。但使用硬件加速，在视频播放的总体流程上也受到诸多限制，尤其是后期处理部分。因此对于追求后期效果的高清发烧友，宁可放弃硬件加速，当然代价可能是需要升级CPU甚至整个系统。 DXVA规范中，不同的显卡的视频硬件加速能力有差异，有的显卡支持的加速项目较多，有的显卡支持的较少，从显卡的发展历程来看，视频硬件加速能力总体是逐渐增强的，但在同一代显卡中，高端显卡的视频硬件加速能力不见得比低端显卡的强，甚至有些时候还会弱些。 对于高清发烧友来说，可以接触到的高清视频源的编码格式主要有MPEG2, [H.264](https://baike.baidu.com/item/H.264/1022230)和[WMV](https://baike.baidu.com/item/WMV/1195900)和[VC-1](https://baike.baidu.com/item/VC-1/6928365)四种。DXVA 1.0规范里最早定义了MPEG2和WMV[硬件解码](https://baike.baidu.com/item/%E7%A1%AC%E4%BB%B6%E8%A7%A3%E7%A0%81)能力，在DXVA 2.0里又增加了H.264和VC-1硬件解码能力。

视频硬件解码能力的成功运用，需要几个环节的配合。首先，显卡硬件需要支持特定的[视频编码](https://baike.baidu.com/item/%E8%A7%86%E9%A2%91%E7%BC%96%E7%A0%81)格式；其次，[显卡驱动](https://baike.baidu.com/item/%E6%98%BE%E5%8D%A1%E9%A9%B1%E5%8A%A8)需要正确的报告显卡硬件的这种能力；最后，[视频解码器](https://baike.baidu.com/item/%E8%A7%86%E9%A2%91%E8%A7%A3%E7%A0%81%E5%99%A8)需要能够启用这种能力（这可能会进一步涉及到分离器和渲染器的配合）。当然，视频源本身也必须是这种编码格式。

还有一点需要特别注意的是，DXVA是一种接口规范，显卡硬件支持DXVA规范，不代表就实现了里面所有的功能，但是肯定的一点是，无论什么功能，显卡驱动的DXVA部分代码一定会告诉系统，这项功能它实现了还是没有实现，这是DXVA规范强制性要求的。好处是系统不需要自己去测试显卡有没有实现这项功能。解码器支持DXVA规范，不代表就能利用DXVA规范里面所有的功能组合，它可以选择自己有能力支持的，或者想要支持的那些功能组合，其他功能组合也许就放弃了。还有，某显卡虽然表示支持某种能力，但如果解码器认为这个能力的支持不够稳定，容易导致系统崩溃或错误，或者性能低下，它也可以放弃这种能力。



### 硬解能力级别

```
DXVA2_ModeH264_A (DXVA2_ModeH264_MoComp_NoFGT)

DXVA2_ModeH264_B (DXVA2_ModeH264_MoComp_FGT)

DXVA2_ModeH264_C (DXVA2_ModeH264_IDCT_NoFGT)

DXVA2_ModeH264_D (DXVA2_ModeH264_IDCT_FGT)

DXVA2_ModeH264_E (DXVA2_ModeH264_VLD_NoFGT)

DXVA2_ModeH264_F (DXVA2_ModeH264_VLD_FGT)

DXVA2_ModeMPEG2_IDCT

DXVA2_ModeMPEG2_MoComp

DXVA2_ModeMPEG2_VLD

DXVA2_ModeVC1_A (DXVA2_ModeVC1_PostProc)

DXVA2_ModeVC1_B (DXVA2_ModeVC1_MoComp)

DXVA2_ModeVC1_C (DXVA2_ModeVC1_IDCT)

DXVA2_ModeVC1_D (DXVA2_ModeVC1_VLD)

DXVA2_ModeWMV8_A (DXVA2_ModeWMV8_PostProc)

DXVA2_ModeWMV8_B (DXVA2_ModeWMV8_MoComp)

DXVA2_ModeWMV9_A (DXVA2_ModeWMV9_PostProc)

DXVA2_ModeWMV9_B (DXVA2_ModeWMV9_MoComp)

DXVA2_ModeWMV9_C (DXVA2_ModeWMV9_IDCT)
```

视频[硬件解码](https://baike.baidu.com/item/%E7%A1%AC%E4%BB%B6%E8%A7%A3%E7%A0%81)的能力的级别从高到低的顺序如下，能力越高，显卡能够分担的视频解码计算量就 越多。



VLD(视频bitstream处理)->IDCT([帧内压缩](https://baike.baidu.com/item/%E5%B8%A7%E5%86%85%E5%8E%8B%E7%BC%A9/3431175))->MoComp([帧间压缩](https://baike.baidu.com/item/%E5%B8%A7%E9%97%B4%E5%8E%8B%E7%BC%A9/3431119))->PostProc(后期处理)

VLD级别的解码包含IDCT, MoComp和PostProc,

IDCT级别的解码包含MoComp和PostProc

MoComp级别的解码包含PostProc

对于[H.264](https://baike.baidu.com/item/H.264/1022230)编码,还有一个称为FGT(Film Grain Technology)的技术，即所谓胶片感，目的是在不 牺牲压缩能力的同时保持视频源特有的颗粒信息。



## 功能

视频解码对于我们普通用户来说可以分成两类，一类是以[CPU](https://baike.baidu.com/item/CPU)为核心的[软件解码](https://baike.baidu.com/item/%E8%BD%AF%E4%BB%B6%E8%A7%A3%E7%A0%81/12734576)，另一类就是如今主流的以GPU为核心的[硬件加速](https://baike.baidu.com/item/%E7%A1%AC%E4%BB%B6%E5%8A%A0%E9%80%9F)解码。

就目前来看，二者各有利弊。

软件解码好处是，第三方软件[解码器](https://baike.baidu.com/item/%E8%A7%A3%E7%A0%81%E5%99%A8/84366)齐全通用性高，使用CPU来作为解码核心能够满足几乎所有的视频解码。而坏处也显而易见，在一台CPU性能较弱的平台上，尤其是在看高[码率](https://baike.baidu.com/item/%E7%A0%81%E7%8E%87)的高清视频时，CPU由于[线程数](https://baike.baidu.com/item/%E7%BA%BF%E7%A8%8B%E6%95%B0)和自身架构导致的效能上不足，视频画面卡、顿、慢成为不可避免的结局。

硬件加速解码好处是，如今主流显卡都能够很好的支持主流视频格式解码，尤其是在图形核心采用[统一架构](https://baike.baidu.com/item/%E7%BB%9F%E4%B8%80%E6%9E%B6%E6%9E%84)后，利用图形核心的并行架构设计，在[H.264](https://baike.baidu.com/item/H.264/1022230)和[VC-1](https://baike.baidu.com/item/VC-1/6928365)格式高清视频上有绝佳的支持效果。弊端是，[硬件解码](https://baike.baidu.com/item/%E7%A1%AC%E4%BB%B6%E8%A7%A3%E7%A0%81)受限视频播放总体流程，尤其是后期处理部分。所以很多追求最终画质效果的高清发烧友，宁可放弃硬件（[GPU](https://baike.baidu.com/item/GPU/105524)）加速解码的几倍性能，而选择基于CPU的[软件解码](https://baike.baidu.com/item/%E8%BD%AF%E4%BB%B6%E8%A7%A3%E7%A0%81/12734576)。

根据DXVA的规范制定，[硬件加速](https://baike.baidu.com/item/%E7%A1%AC%E4%BB%B6%E5%8A%A0%E9%80%9F)解码可分为四个级别，它们由高到低分别是：

**VLD（bitstream方面的处理）>IDCT（[帧内压缩](https://baike.baidu.com/item/%E5%B8%A7%E5%86%85%E5%8E%8B%E7%BC%A9/3431175)）>MoComp（[帧间压缩](https://baike.baidu.com/item/%E5%B8%A7%E9%97%B4%E5%8E%8B%E7%BC%A9/3431119)）>PostProc（显示后处理）**

从上面的等级分布上不难看出，VLD加速等级最高，所以其包含IDCT、MoCoopm和PostProc；IDCT加速次之，包含MoCoopm和PostProc；最后MoComp加速仅包含PostProc。

对于仅支持MC加速的显卡只有在2005年以前的老产品上出现，当NVIDIA进入GeForce 6000系列和AMD进入Radeon X800系列后，图形核心的[硬件加速](https://baike.baidu.com/item/%E7%A1%AC%E4%BB%B6%E5%8A%A0%E9%80%9F)能力达到了IDCT加速级别。而当显卡进入图形核心统一架构设计后，GeForce 8000和Radeon HD 2000开始进入目前最高的VLD加速。

不过值得一提的是，由于NVIDIA的高清硬件加速功能基于Video Pocesser单元设计，而最早出现的GeForce 8800系列（G80核心产品）没有开放此功能单元设计，所以NVIDIA真正进入VLD加速是从2007年4月发布的GeForce 8600/8500系列开始 [1] 。

## 视频实现

DXVA有三种[视频加速](https://baike.baidu.com/item/%E8%A7%86%E9%A2%91%E5%8A%A0%E9%80%9F/10256205)级别

MC加速

MC加速+iDCT加速

MC加速+iDCT加速+VLD[1]加速

[1]VLD: Variable-Length Decoding，即可变长度解码

## 解码器

[PowerDVD](https://baike.baidu.com/item/PowerDVD/7292673)([CyberLink](https://baike.baidu.com/item/CyberLink/8678471)): 著名的商业播放器

Nvidia PureVideo: 从名字就可以看出，[Nvidia公司](https://baike.baidu.com/item/Nvidia%E5%85%AC%E5%8F%B8/340213)出品的[视频解码器](https://baike.baidu.com/item/%E8%A7%86%E9%A2%91%E8%A7%A3%E7%A0%81%E5%99%A8/8437114)

[MPC-HC](https://baike.baidu.com/item/MPC-HC/2286213): 继承了历史悠久的开源播放器MPC的衣襟，并在其基础上增加了许多实用功能

[PotPlayer](https://baike.baidu.com/item/PotPlayer/7490828): 原[Kmplayer](https://baike.baidu.com/item/Kmplayer/1487355)作者开发的另一款播放器

[ffdshow](https://baike.baidu.com/item/ffdshow/7292596): 著名的开源编解码器，功能强大，内置了许多实用滤镜，在最近的版本中加入了DXVA模块

在当前，虽然有许多播放器支持DXVA解码，但综合考虑MPC-HC是最为实用并且方便实用的。首先[MPC-HC](https://baike.baidu.com/item/MPC-HC/2286213)是开源软件，其次MPC-HC是绿色软件，无需安装即可使用，再则MPC-HC内置的字幕滤镜比起其他播放器，对ASS/SSA的支持度最高。因此接下来着重介绍MPC-HC

MPC-HC的DXVA对显卡的限制:

nVidia 8(9)xxx系列，仅支持H.264

ATI Radeon HD系列，支持[H.264](https://baike.baidu.com/item/H.264/1022230)和VC-1

Intel G45，仅支持H.264

在未来，MPC-HC可能会增加对[运动补偿](https://baike.baidu.com/item/%E8%BF%90%E5%8A%A8%E8%A1%A5%E5%81%BF/9194264)模式的支持，以兼容更多古老的显卡。但在当前，还不支持MPEG2和[WMV](https://baike.baidu.com/item/WMV/1195900)的[硬件加速](https://baike.baidu.com/item/%E7%A1%AC%E4%BB%B6%E5%8A%A0%E9%80%9F/5702432)。

由于DXVA的限制，所使用的[渲染器](https://baike.baidu.com/item/%E6%B8%B2%E6%9F%93%E5%99%A8/410813)必须遵守以下规则:

Windows XP用户，必须使用覆盖合成器(Overlay Mixer)，VMR 7或[VMR9](https://baike.baidu.com/item/VMR9/5418484)作为视频渲染器(Video Renderer)

Windows Vista/7用户，必须使用增强型视频渲染器(Enhanced Video Renderer/EVR)或EVR custom renderer

MPC-HC[视频解码器](https://baike.baidu.com/item/%E8%A7%86%E9%A2%91%E8%A7%A3%E7%A0%81%E5%99%A8/8437114)必须直接连接视频渲染器，因此中间无法插入[DirectVobSub](https://baike.baidu.com/item/DirectVobSub/6609970)或[ffdshow](https://baike.baidu.com/item/ffdshow/7292596)之类的滤镜。 在DXVA模式中，[外挂字幕](https://baike.baidu.com/item/%E5%A4%96%E6%8C%82%E5%AD%97%E5%B9%95/7753341)可以通过[MPC-HC](https://baike.baidu.com/item/MPC-HC/2286213)内置的字幕滤镜来显示，但是对渲染器的要求更苛刻

Windows XP用户，必须使用VMR9

Windows Vista/7用户，必须使用EVR custom renderer

在 选项(Options)/播放(Playback) 中，勾上 自动装载字幕(Auto-load subtitles)

## 支持视频

可以通过软件来检测：[DXVAChecker](http://bluesky23.yukishigure.com/en/DXVAChecker.html)

说了那么多，其实只是想说[视频加速](https://baike.baidu.com/item/%E8%A7%86%E9%A2%91%E5%8A%A0%E9%80%9F/10256205)解码真是个复杂的东西。需要硬件，[解码器](https://baike.baidu.com/item/%E8%A7%A3%E7%A0%81%E5%99%A8/84366)，视频本身等多方面的支持方可促成视频硬解。虽然复杂，但这是值得的。通过视频加速解码，我们可以将CPU从繁重又重复的工作中解放出来，让它在别的地方发挥自己的性能，以及让那些由于CPU性能孱弱但有一块还不错的显卡的电脑同样可以享受高清视频。

压制篇([x264](https://baike.baidu.com/item/x264/10787441)版)接下来将介绍如何压制一个能使绝大多数[硬件加速](https://baike.baidu.com/item/%E7%A1%AC%E4%BB%B6%E5%8A%A0%E9%80%9F/5702432)解码方案都支持的视频 使用的编码器是x264，它是一个效率非常高的开源编码器，基于[H.264](https://baike.baidu.com/item/H.264/1022230)标准设计。

## 规格分类

首先[H.264](https://baike.baidu.com/item/H.264/1022230)的视频按照标准被分为几个规格

Profile在x264中被分为[baseline](https://baike.baidu.com/item/baseline/3589734)/main/high (Profile越高级，支持越多的高级特性，成品的[压缩率](https://baike.baidu.com/item/%E5%8E%8B%E7%BC%A9%E7%8E%87/6435712)越高)

x264相关参数：--profile 可强制指定Profile，一般情况下可不选，x264将会根据其他参数自行设定此项。如果设置此项，x264将会自动无视所设Profile无法支持的高级特性。

Level被分为1/1.1/1.2/1.3/2/2.1/2.2/3/3.1/3.2/4/4.1/4.2/5/5.1 (Level越高，对分辨率/[参考帧](https://baike.baidu.com/item/%E5%8F%82%E8%80%83%E5%B8%A7/8119681)/码率的限制越小)

x264相关参数：--level 可强制指定Level，一般情况下可不选，x264将会根据实际情况自行设定此项，但自动设定并不一定准确。如果设置此项，x264将会自动限制DBP[2]。

[2]DPB，即Decoded Picture Buffer。





