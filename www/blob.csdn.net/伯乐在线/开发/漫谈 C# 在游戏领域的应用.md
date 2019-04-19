# 漫谈 C# 在游戏领域的应用 - 文章 - 伯乐在线
原文出处： [慕容小匹夫](http://www.cnblogs.com/murongxiaopifu/p/5475828.html)
## 0x00 前言
随着微软越来越开放，C#也变得越来越吸引人们的眼球。而在游戏行业中，C#也开始慢慢地获得了关注。这不， 网易绝代双娇手游团队已经全面使用.Net Core支持前后端统一C#开发，跨平台部署了。
![](http://jbcdn2.b0.upaiyun.com/2016/06/dd14ea997afd556b4fe729329296b0f0.jpg)
所以，我们就来总结一下C#在游戏行业中一些具体的应用吧。
## 0x01 游戏引擎
说道游戏领域的应用，自然应该先来说说游戏引擎：
#### Unity
（[Unity – Game Engine](https://link.zhihu.com/?target=http%3A//unity3d.com/cn/)）
![](http://jbcdn2.b0.upaiyun.com/2016/06/57e9cd8af4a5b18eb1a93d9b1068913a.png)
不必多说了吧，随着“移动为王”潮流的到来，Unity3D也变得越来越流行。使用C/C++开发，提供C#作为脚本。
移动游戏开发必备神器，而且随着VR热潮的到来，使用U3D开发VR内容也十分便利。总之，你值得拥有。
#### Axiom
([Axiom](https://link.zhihu.com/?target=http%3A//axiom.codeplex.com/)）
![](http://jbcdn2.b0.upaiyun.com/2016/06/8b916d74395bea5463fd18f46d232a88.png)
Axiom是一款开源且跨平台的3D渲染引擎，提供了对Windows, Linux, Android, iPhone以及Windows Phone等各个平台的支持。
了解游戏开发的小伙伴们一定听说过OGRE渲染引擎吧？
Axiom使用C#移植了OGRE。
#### Torque X
（[Torque X | Products](https://link.zhihu.com/?target=https%3A//www.garagegames.com/products/torque-x/)）
![](http://jbcdn2.b0.upaiyun.com/2016/06/ccfa76da78efdb44cd9c8b9a200cc56a.png)
由GarageGames开发的一款基于XNA的2D/3D游戏引擎（但是不支持XNA Game Studio 4.0)。
使用C#开发，目标平台主要是PC以及XBox。
但是很多功能都没有开发完毕，目前已经被废弃。
#### truevision3d
（[3D Engine and Game Development SDK by Truevision3D](https://link.zhihu.com/?target=http%3A//www.truevision3d.com/)）
![](http://jbcdn2.b0.upaiyun.com/2016/06/5d4160308ec39c4421a27c980f6c7839.png)
底层使用C/C++开发，不过提供对VB6, VB.Net, C#, Delphi,APL和BlitzMax的支持。
相信各位能够迅速上手。
#### Xenko
（[Home | Xenko](https://link.zhihu.com/?target=http%3A//xenko.com/)）
![](http://jbcdn2.b0.upaiyun.com/2016/06/8a0f1a9e8624a77cc53401e39f8d3f6e.png)
来自日本的开发商SiliconStudio，开发商十分靠谱。且和Unity3D相比，Xenko支持最新的版本的C#6和.Net4.6。且由于借助了Xamarin，因此在跨平台策略上不比Unity3D差。
而且，它还是免费[开源](https://link.zhihu.com/?target=https%3A//github.com/SiliconStudio/xenko)的，不过由于使用了GPL3协议，因此如果是自己去修改引擎代码就必须公开。
个人觉得很有意思且值得研究的一个游戏引擎。
#### MonoGame
([MonoGame](http://www.monogame.net/))
![](http://jbcdn2.b0.upaiyun.com/2016/06/fd5b72845d6c076b831904edc46eec97.png)
MonoGame是开源项目Mono的一个分支，是Microsoft XNA 4.X Framework开源实现。该框架的目标是使XNA可以更容易地开发跨平台游戏，并且可以进行代码的高效复用。
作为Mono的一个分支，MonoGame的跨平台能力自然无需担心，它目前支持iOS, Android, MacOS, Linux, Windows,OUYA, PS4, PSVita以及Xbox One等平台。
## 0x02 图像相关
#### XNA
![](http://jbcdn2.b0.upaiyun.com/2016/06/152482413f6ef305b40b91f78d66aa5e.jpg)
不必多说了吧。微软自家的东西。XNA中的X表示能够在WindowsXbox和合作伙伴之间达到跨平台的强大的软件工具。N表示“下一代（Next- generation）”，A表示“架构（Architecture）”。基于DirectX的游戏开发环境。使用c#对directx进行了封装。可以 在xbox360和pc间实现跨平台。
#### OpenTK
（[The Open Toolkit Library](https://link.zhihu.com/?target=http%3A//www.opentk.com/)）
封装了OpenGL, OpenCL以及OpenAL的C#库。
#### SDL.Net
（[SDL.Net](https://link.zhihu.com/?target=http%3A//cs-sdl.sourceforge.net/)）
![](http://jbcdn2.b0.upaiyun.com/2016/06/1606a85b8dd7c904e62f75d85454f299.png)
SDL(Simple DirectMedia Layer)可以管理视频，音频，输入设备，CD-ROM，线程，共享对象装载，联网和定时器。对于3D图形，它可以处理OpenGL或Direct3D。开发库内部使用C语言编写，而SDL.Net是SDL库的C#版本。
#### SlimDX
（[SlimDX Homepage](https://link.zhihu.com/?target=https%3A//slimdx.org/)）
![](http://jbcdn2.b0.upaiyun.com/2016/06/d11c220b63cf85d5baf2c150a61dd8ea.png)
SlimDX是一个免费开源的框架，基于DirectX，开发者可以使用 C#, VB.NET, 或者IronPython进行开发。
## 0x03 物理相关
#### Box2Dx
（[Box2Dx](https://link.zhihu.com/?target=https%3A//code.google.com/p/box2dx/)）
我相信Box2D各位应该十分了解，Box2D本身是一个用于模拟2D刚体物体的C++引擎。而且它集成了大量的物理力学和运动学的计算，并将物理模拟过程封装到类对象中，将对物体的操作，以简单友好的接口提供给开发者。我们只需要调用引擎中相应的对象或函数，就可以模拟现实生活中的加速、减速、抛物线运动、万有引力、碰撞反弹等等各种真实的物理运动。
而Box2Dx是Box2D的C#版本。
#### Box2D.XNA
（[Box2D.XNA – Home](https://link.zhihu.com/?target=http%3A//box2dxna.codeplex.com/)）
![](http://jbcdn2.b0.upaiyun.com/2016/06/495c4addf69c1abcc7cc05a155831616.png)
和上面的类似，当然对Xbox 360做了特别的优化。
#### Farseer
（[Farseer Physics Engine](https://link.zhihu.com/?target=http%3A//farseerphysics.codeplex.com/)）
![](http://jbcdn2.b0.upaiyun.com/2016/06/33d1c00cbe4987dadf70a1b44cfc5c95.png)
同样是一个2d物理引擎，支持C#。当然它也基于Box2D，因此和Box2D有很多相同的功能。
#### Jitter
（[Jitter](https://link.zhihu.com/?target=http%3A//jitter-physics.com/wordpress/)）
最后介绍一个3D的物理引擎。支持使用C#。
## 0x04 结语
可以看到，C#在微软支持开源之前就已经在游戏领域有了很多应用。希望现在在微软开源的支持之下，C#能够在游戏领域获得更多的关注。
