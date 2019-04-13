
# OpenGL 20岁生日礼物：OpenGL 4.3 和OpenGL ES 3.0发布 - 雷锋网 - CSDN博客


2012年08月07日 18:00:11[leiphone](https://me.csdn.net/leiphone)阅读数：1190标签：[图形																](https://so.csdn.net/so/search/s.do?q=图形&t=blog)[direct3d																](https://so.csdn.net/so/search/s.do?q=direct3d&t=blog)[textures																](https://so.csdn.net/so/search/s.do?q=textures&t=blog)[游戏																](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)[shader																](https://so.csdn.net/so/search/s.do?q=shader&t=blog)[工作																](https://so.csdn.net/so/search/s.do?q=工作&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=shader&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)个人分类：[业界																](https://blog.csdn.net/leiphone/article/category/873390)
[
																								](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=textures&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=textures&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=direct3d&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=图形&t=blog)
![](http://www.leiphone.com/wp-content/uploads/2012/08/images2.jpg)也许像小编一样，很多人对OpenGL
 并不了解，甚至是闻所未闻，但相信广大开发人员——特别是游戏开发者，一定对OpenGL不陌生。自SGI（Silicon Graphics）公司于1992年1月发布OpenGL 1.0版本以来，OpenGL已经陪伴大家走过了20个年头。在OpenGL迎来20岁生日之际，Khronos集团宣布推出OpenGL 4.3和面向移动平台的OpenGL ES 3.0，作为其最好的生日礼物。
[OpenGL](http://www.leiphone.com/?s=OpenGL+)（即开放性图形库Open
 Graphics Library），是一个三维的计算机图形和模型库，最初是美国SGI公司为图形工作站开发的一种功能强大的三维图形机制（或者说是一种图形标准）。它源于SGI公司为其图形工作站开发的IRIS GL，在跨平台移植过程中发展成为OpenGL。作为一个性能优越的图形应用程序设计界面（API），OpenGL适用于广泛的计算机环境。从个人计算机到工作站和超级计算机，OpenGL都能实现高性能的三维图形功能。
OpenGL本身针对移动端和桌面端有多个版本，它是开发iPhone、Android以及很多控制器和桌面系统3D图形应用和游戏的重要标准之一。目前OpenGL工作小组中的合作伙伴中有多家重量级的厂商，比如AMD、Intel、ARM、NVIDIA、Broadcom、苹果、[Google](http://www.leiphone.com/tag/google)、诺基亚以及像Epic、EA、Unity这样的游戏开发商。
![](http://www.leiphone.com/wp-content/uploads/2012/08/khronos-siggraph-press-briefing-aug12-nt3-pdf-page-8-of-66.jpg)
**OpenGL ES 3.0**
3.0版本的OpenGL ES在OpenGL 3.x 和 4.x的基础上增加了许多新的功能，比如着色器的功能变得更加强大、全面支持32位整数和浮点型数据。
OpenGL ES 3.0可以让GPU处理更复杂的纹理，如任意尺寸的纹理、浮点纹理等。其中最重要的就是新的纹理算法带来了更好的纹理压缩表现（包括ETC2和EAC标准），以及一些像Instance Rendering、Occlusion Queries、Transform Feedback这样的新的硬件加速特性。
这些更新为OpenGL ES的移动开发人员带来很大的便利，之前很多开发人员都会在APK中为不同设备创建不同的textures，但现在他们可以在移动端和桌面端使用相同的组件。由于压缩方法有所改进，现在的压缩包比起以前小了很多，在提升应用程序下载速度的同时，也降低了内存的占用空间。
即使新增了如此多的功能，OpenGL在性能上还是稍稍落后Direct3D 10和Open GL 3.2，其中最大的缺陷是OpenGL ES 3.0不支持几何渲染，这些几何渲染可以生成新的线条和轮廓，使得曲面更加细分：选择一个现有的图形，加入新的线条，让曲线变得更加顺滑，整个图形看起来会更加美观。
**OpenGL 4.3**
除了OpenGL ES 3.0，Khronos 集团也在桌面端推出了OpenGL 4.3，新版标准不仅带来了更好的图形表现，同时也增加了更多之前没有的效果，比如不同类型的模糊效果。此外，开发者还可以不通过OpenCL直接使用OpenGL技巧来使用运算着色器（compute shaders）释放GPU上的任务。这样开发者可以在游戏中不耗用更多CPU功耗的情况下实现更好的物理和人工智能的模拟。
从当前的现状来看，Open GL仍然是唯一能够取代微软对3D图形技术的完全控制的API。它仍然具有一定的生命力，但是Silicon Graphics已经不再以任何让微软不悦的方式推广Open GL，因而它存在较高的风险。游戏开发人员是一个有着独立思想的群体，很多重要的开发人员目前仍然在使用Open GL。因此，硬件开发商正在设法加强对它的支持。Direct3D目前还不能支持高端的图形设备和专业应用； Open GL在这些领域占据着统治地位。
最后通过一张图来简略回顾下OpenGL 20年的风雨历程：
![](http://www.leiphone.com/wp-content/uploads/2012/08/khronos-siggraph-press-briefing-aug12-nt3-pdf-page-37-of-66.jpg)
以NVIDIA公司于2012年3月22日发布的新一代Kepler架构显卡GeForce GTX 680为例，OpenGL的三角形输出能力、像素输出能力、浮点计算能力已经分别达到20前的1800倍、60倍、4830倍。[ ](http://news.mydrivers.com/picture/236928/236928_1.html)
PS：关于OpenGL，我们所熟悉的[李开复老师](http://www.leiphone.com/0803-danice-olympic-tweet-scandal.html)在SGI工作的时候就主持过基于OpenGL的多媒体应用的研究，当时开老师曾经希望说服微软采用OpenGL，但最终微软却用自己的亲儿子Direct
 3D替代OpenGL。DirectX与OpenGL的战争由此展开。一开始OpenGL有着较大的优势，但自从2001年微软推出DirectX 8，并引入了两项对于整个电脑绘图界来说极为重要的创新与变革Vertex Shader 以及Pixel Shader后形势开始逆转，DirectX 9的推出更是让DirectX开始领先。然而就在数天前，著名游戏开放商Valve发布了一条消息重提当年恩怨，称即便在Windows环境这个主场，Direct X的表现也不如OpenGL。您觉得Direct X和OpenGL孰优孰劣？OpenGL
 4.3 和OpenGL ES 3.0的发布能否打败DirectX ？欢迎在文后留下您的评论。
Via[TC](http://techcrunch.com/2012/08/06/new-opengl-standards-2012/?utm_source=feedburner&utm_medium=feed&utm_campaign=Feed%3A+Techcrunch+%28TechCrunch%29)&[arstechnica](http://arstechnica.com/information-technology/2012/08/opengl-celebrates-its-20th-birthday-with-two-new-versions/)

**（****[Danice](http://www.leiphone.com/author/danice)****供****雷锋网****专稿，转载请注明来自雷锋网及作者，并链回本页)**

