# 开源倾情奉献：基于.NET打造IP智能网络视频监控系统 - DoubleLi - 博客园






转载自 http://www.cnblogs.com/gaochundong/p/opensource_ip_video_surveillance_system_part_1_introduction.html





## 开源倾情奉献系列链接
- [开源倾情奉献：基于.NET打造IP智能网络视频监控系统（一）开放源代码](http://www.cnblogs.com/gaochundong/p/opensource_ip_video_surveillance_system_part_1_introduction.html)
- [开源倾情奉献：基于.NET打造IP智能网络视频监控系统（二）基础类库介绍](http://www.cnblogs.com/gaochundong/p/opensource_ip_video_surveillance_system_part_2_foundation.html)
- [开源倾情奉献：基于.NET打造IP智能网络视频监控系统（三）命令行工具集](http://www.cnblogs.com/gaochundong/p/opensource_ip_video_surveillance_system_part_3_commandline.html)
- [开源倾情奉献：基于.NET打造IP智能网络视频监控系统（四）服务端介绍](http://www.cnblogs.com/gaochundong/p/opensource_ip_video_surveillance_system_part_4_service_side.html)
- [开源倾情奉献：基于.NET打造IP智能网络视频监控系统（五）客户端介绍](http://www.cnblogs.com/gaochundong/p/opensource_ip_video_surveillance_system_part_5_client_side.html)

## 这是什么系统？



OptimalVision（OV）网络视频监控系统（Video Surveillance System），是一套基于.NET、C#、WCF、WPF等技术构建的IP网络视频监控系统。设计与实现该系统的初衷是希望在家用电脑中部署该系统，连接本地或局域网设备，通过浏览器或手机客户端浏览宝宝实时视频，也就是俗称的“宝宝在线”或“家庭看护”。

详情参考博文《[基于.NET打造IP智能网络视频监控系统](http://www.cnblogs.com/gaochundong/archive/2013/04/11/dotnet_csharp_ip_video_surveillance.html)》。

![](https://images0.cnblogs.com/blog/175043/201311/21171823-bdc7970cfa0f4a21a52d020468f85238.png)

更多系统截图请参考博文《[基于.NET打造IP智能网络视频监控系统](http://www.cnblogs.com/gaochundong/archive/2013/04/11/dotnet_csharp_ip_video_surveillance.html)》。

## 为什么开源？

由于业余时间总是有限，完成系统中的服务、配置、采集、传输和桌面GUI部分后，继续完成后续移动客户端的动力与兴趣明显减弱，尤其是宝宝出生后的很长一段时间内基本没有进展，逐渐地演变为演练和测试所关注技术和模式的实验性项目。

总结了几点开放源代码的原因：
- 社区分享。独乐乐不如众乐乐，孤独腐朽的代码没有意义，与社区分享和探讨也是一种乐趣。
- 精力有限。利用业余时间构建系统，时间挤占的是陪家人和孩子的时间，要不也不会积累了三年。
- 知识积累。据说自己查看六个月前写的代码，如果感觉需要改进，则就成长了，我现在是无力改进。
- 拓展视野。个人的视角总是短视，接受更多人的批评与指正，可以促进更快的学习和拓展视野。

## 代码在哪里？

代码托管在 [GitHub](https://github.com/gaochundong) 网站，所有代码基于 Microsoft .NET Framework 4.0 编译。
- [https://github.com/gaochundong/Gimela](https://github.com/gaochundong/Gimela)
- [GitHub 一键下载](https://github.com/gaochundong/Gimela/archive/master.zip)
- [Star 代码](https://github.com/gaochundong/Gimela/stargazers)，感谢支持

## 代码目录结构

下面是源代码目录结构：
- assets - 系统使用到的资源，例如图片等
- bin - 系统编译输出目录
- Debug - Debug模式输出目录

- build - 编译系统脚本
- docs - 系统设计和文档
- lib - 系统引用的类库
- src - 系统代码
- Crust - MVVM模式框架（改编自 [MVVM Light Toolkit](http://mvvmlight.codeplex.com/)）
- Foundation - 系统基础类库（参考《[开源倾情奉献：基于.NET打造IP智能网络视频监控系统（二）基础类库介绍](http://www.cnblogs.com/gaochundong/p/opensource_ip_video_surveillance_system_part_2_foundation.html)》）
- Knifer - 命令行工具集（参考《[开源倾情奉献：基于.NET打造IP智能网络视频监控系统（三）命令行工具集](http://www.cnblogs.com/gaochundong/p/opensource_ip_video_surveillance_system_part_3_commandline.html)》）
- Rukbat - 视频监控系统代码
- Common - 前端和后端通用代码
- Service - WCF服务代码（参考《[开源倾情奉献：基于.NET打造IP智能网络视频监控系统（四）服务端介绍](http://www.cnblogs.com/gaochundong/p/opensource_ip_video_surveillance_system_part_4_service_side.html)》）
- GUI - WPF客户端代码（参考《[开源倾情奉献：基于.NET打造IP智能网络视频监控系统（五）客户端介绍](http://www.cnblogs.com/gaochundong/p/opensource_ip_video_surveillance_system_part_5_client_side.html)》）


- tests- 各种测试
- tools - 各种工具，例如清理编译目录脚本等

## 参考了哪些类库？

下面罗列了系统所学习、引用、参考、改编、摘抄的开源类库的不完全列表。
- [AForge.NET](http://www.aforgenet.com/)
- [Accord.NET](http://accord-framework.net/)
- [NAudio](http://naudio.codeplex.com/)
- [nVLC](http://www.codeproject.com/Articles/109639/nVLC)
- [Speex](http://speex.org/)
- [C# WebServer](http://webserver.codeplex.com/)
- [FFmpeg](http://www.ffmpeg.org/)
- [FFmpeg.NET](http://ffmpegdotnet.codeplex.com/)
- [Flowplayer](http://flowplayer.org/)
- [iSpy](http://www.ispyconnect.com/)
- [ZoneMinder](http://www.zoneminder.com/)
- [ONVIF Device Manager](http://sourceforge.net/projects/onvifdm/)
- [Google GData](https://developers.google.com/gdata/)
- [GMap.NET](http://gmapdotnet.codeplex.com/)
- [Json.NET](http://json.codeplex.com/)
- [Dynamic Json](http://dynamicjson.codeplex.com/)
- [Hammock](https://github.com/danielcrenna/hammock)
- [AutoMapper](http://automapper.org/)
- [SocialKit / LightRest](http://lightrest.codeplex.com/)
- [Microsoft Enterprise Library](http://msdn.microsoft.com/en-us/library/ff648951.aspx)
- [Microsoft Unity](https://unity.codeplex.com/)
- [System.Data.SQLite](http://system.data.sqlite.org/)
- [RavenDB](http://ravendb.net/)
- [RaptorDB](http://raptordb.codeplex.com/)
- [MongoDB](http://www.mongodb.org/)
- [MongoDB C# Driver](http://docs.mongodb.org/ecosystem/tutorial/use-csharp-driver/)
- [Windows Presentation Foundation (WPF)](http://wpf.codeplex.com/)
- [MVVM Light Toolkit](http://mvvmlight.codeplex.com/)
- [Caliburn.Micro](http://caliburnmicro.codeplex.com/)
- [MEFedMVVM](http://mefedmvvm.codeplex.com/)
- [MVVM Foundation](http://mvvmfoundation.codeplex.com/)
- [Phoenix MVVM](http://phoenixframework.codeplex.com/)
- [WPF MediaKit](http://wpfmediakit.codeplex.com/)
- [WPF Multiple Document Interface (MDI)](http://wpfmdi.codeplex.com/)
- [MDI Container for WPF](http://mdicontainer.codeplex.com/)
- [AvalonDock](http://avalondock.codeplex.com/)
- [WPF Google Map Control](http://wpfgooglemap.codeplex.com/)
- [WPFSpark](http://wpfspark.codeplex.com/)
- [WPF 3DTools](http://3dtools.codeplex.com/)
- [Odyssey WPF Controls](http://odyssey.codeplex.com/)
- [Extended WPF Toolkit™ Community Edition](http://wpftoolkit.codeplex.com/)
- [Calendar Control in WPF](http://wpfcalendarcontrol.codeplex.com/)
- [patterns & practices: Prism](http://compositewpf.codeplex.com/)
- [Facebook Developer Toolkit](http://facebooktoolkit.codeplex.com/)
- [Microsoft Health Common User Interface (CUI)](http://mscui.codeplex.com/)
- [Mahapps.Metroa UI Toolkit for WPF](http://mahapps.com/MahApps.Metro/)
- [Spark View Engine](http://sparkviewengine.codeplex.com/)
- [ReactiveUI](http://www.reactiveui.net/)
- [CommonServiceLocator](http://commonservicelocator.codeplex.com/)
- [Ninject](http://www.ninject.org/)
- [StructureMap](http://docs.structuremap.net/)
- [NetMQ](https://github.com/zeromq/netmq)
- [FunnelWeb](http://www.funnelweblog.com/)
- [Quartz.NET](http://www.quartz-scheduler.net/)
- [Task Scheduler Engine](http://taskschedulerengine.codeplex.com/)
- [SuperSocket](http://supersocket.codeplex.com/)
- [CommonLibrary.NET](http://commonlibrarynet.codeplex.com/)
- [NSubstitute](http://nsubstitute.github.io/)

## Copyright and license

Copyright (c) 2011-2013 Dennis Gao
All rights reserved.

Licensed under the BSD License.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## GitHub 本周上榜，感谢各位朋友的支持。

![](https://images0.cnblogs.com/blog/175043/201311/25234624-442a495e29794747b538aca6e1e7665f.png)









