# Unity 几种优化建议 - 凯奥斯的注释 - CSDN博客





2017年05月09日 21:16:11[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：565标签：[unity																[优化																[cpu																[gpu																[batch](https://so.csdn.net/so/search/s.do?q=batch&t=blog)
个人分类：[Unity3D](https://blog.csdn.net/ecidevilin/article/category/6456233)





**原文地址：http://www.narkii.com/club/thread-327263-1.html**




最简单的优化建议：

1.PC平台的话保持场景中显示的顶点数少于200K~3M，移动设备的话少于10W，一切取决于你的目标GPU与CPU。
2.如果你用U3D自带的SHADER，在表现不差的情况下选择Mobile或Unlit目录下的。它们更高效。
3.尽可能共用材质。
4.将不需要移动的物体设为Static，让引擎可以进行其批处理。
5.尽可能不用灯光。
6.动态灯光更加不要了。
7.尝试用压缩贴图格式，或用16位代替32位。
8.如果不需要别用雾效(fog)
9.尝试用OcclusionCulling,在房间过道多遮挡物体多的场景非常有用。若不当反而会增加负担。
10.用天空盒去“褪去”远处的物体。
11.shader中用贴图混合的方式去代替多重通道计算。
12.shader中注意float/half/fixed的使用。
13.shader中不要用复杂的计算pow,sin,cos,tan,log等。
14.shader中越少Fragment越好。
15.注意是否有多余的动画脚本，模型自动导入到U3D会有动画脚本，大量的话会严重影响消耗CPU计算。
16.注意碰撞体的碰撞层，不必要的碰撞检测请舍去。


1.为什么需要针对CPU（中央处理器）与GPU（图形处理器）优化？

CPU和GPU都有各自的计算和传输瓶颈，不同的CPU或GPU他们的性能都不一样，所以你的游戏需要为你目标用户的CPU与GPU能力进行针对开发。


2.CPU与GPU的限制

GPU一般具有填充率(Fillrate)和内存带宽(Memory Bandwidth)的限制，如果你的游戏在低质量表现的情况下会快很多，那么，你很可能需要限制你在GPU的填充率。

CPU一般被所需要渲染物体的个数限制，CPU给GPU发送渲染物体命令叫做DrawCalls。一般来说DrawCalls数量是需要控制的，在能表现效果的前提下越少越好。通常来说，电脑平台上DrawCalls几千个之内，移动平台上DrawCalls几百个之内。这样就差不多了。当然以上并不是绝对的，仅作一个参考。

往往渲染(Rendering)并不是一个问题，无论是在GPU和CPU上。很可能是你的脚本代码效率的问题，用Profiler查看下。

关于Profiler介绍：http://docs.[unity3d](http://www.narkii.com/club/forum-51-1.html).com/Documentation/Manual/Profiler.html

需要注意的是:
在GPU中显示的RenderTexture.SetActive()占用率很高，是因为你同时打开了编辑窗口的原因，而不是U3D的BUG。

3.关于顶点数量和顶点计算

CPU和GPU对顶点的计算处理都很多。GPU中渲染的顶点数取决于GPU性能和SHADER的复杂程度，一般来说，每帧之内，在PC上几百万顶点内，在移动平台上不超过10万顶点。

CPU中的计算主要是在蒙皮骨骼计算，布料模拟，顶点动画，粒子模拟等。GPU则在各种顶点变换、光照、贴图混合等。

【个人认为，具体还是看各位的项目需求，假设你项目的是3d游戏。你游戏需要兼容低配置的硬件、流畅运行、控制硬件发热的话，还要达到一定效果（LIGHTMAP+雾效），那么顶点数必定不能高。此时同屏2W顶点我认为是个比较合适的数目，DRAWCALL最好低于70。另，控制发热请控制最高上限的帧率，流畅的话，帧率其实不需要太高的。】



4.针对CPU的优化——减少DRAW
 CALL 的数量

为了渲染物体到显示器上，CPU需要做一些工作,如区分哪个东西需要渲染、区分开物体是否受光照影响、使用哪个SHADER并且为SHADER传参、发送绘图命令告诉显示驱动，然后发送命令告诉显卡删除等这些。

假设你有一个上千三角面的模型却用上千个三角型模型来代替，在GPU上花费是差不多的，但是在CPU上则是极其不一样，消耗会大很多很多。为了让CPU更少的工作，需要减少可见物的数目：

a.合并相近的模型，手动在模型编辑器中合并或者使用UNITY的Draw call批处理达到相同效果(Draw call batching)。具体方法和注意事项查看以下链接：

Draw call batching : http://docs.unity3d.com/Documentation/Manual/DrawCallBatching.html


b.在项目中使用更少的材质(material)，将几个分开的贴图合成一个较大的图集等方式处理。

如果你需要通过脚本来控制单个材质属性，需要注意改变Renderer.material将会造成一份材质的拷贝。因此，你应该使用Renderer.sharedMaterial来保证材质的共享状态。

有一个合并模型材质不错的插件叫Mesh Baker，大家可以考虑试下。

c.尽量少用一些渲染步骤，例如reflections,shadows,per-pixel light 等。

d.Draw call batching的合并物体，会使每个物体（合并后的物体）至少有几百个三角面。

假设合并的两个物体（手动合并）但不共享材质，不会有性能表现上的提升。多材质的物体相当于两个物体不用一个贴图。所以，为了提升CPU的性能，你应该确保这些物体使用同样的贴图。

另外，用灯光将会取消(break)引擎的DRAW CALL BATCH，至于为什么，查看以下：

Forward Rendering Path Details:
http://docs.unity3d.com/Documentation/Components/RenderTech-ForwardRendering.html

e.使用相关剔除数量直接减少Draw Call数量，下文有相关提及。


5.优化几何模型

最基本的两个优化准则：
a.不要有不必要的三角面。
b.UV贴图中的接缝和硬边越少越好。

需要注意的是，图形硬件需要处理顶点数并跟硬件报告说的并不一样。不是硬件说能渲染几个点就是几个点。模型处理应用通展示的是几何顶点数量。例如，一个由一些不同顶点构成的模型。在显卡中，一些集合顶点将会被分离(split)成两个或者更多逻辑顶点用作渲染。如果有法线、UV坐标、顶点色的话，这个顶点必须会被分离。所以在游戏中处理的实际数量显然要多很多。


6.关于光照

若不用光肯定是最快的。移动端优化可以采用用光照贴图(Lightmapping)去烘培一个静态的贴图，以代替每次的光照计算，在U3D中只需要非常短的时间则能生成。这个方法能大大提高效率，而且有着更好的表现效果（平滑过渡处理，还有附加阴影等）。

在移动设备上和低端电脑上尽量不要在场景中用真光，用光照贴图。这个方法大大节省了CPU和GPU的计算，CPU得到了更少的DRAWCALL，GPU则需要更少顶点处理和像素栅格化。

Lightmapping : http://docs.unity3d.com/Documentation/Manual/Lightmapping.html


7.对GPU的优化——图片压缩和多重纹理格式

Compressed Textures（图片压缩):

http://docs.unity3d.com/Documentation/Components/class-Texture2D.html

图片压缩将降低你的图片大小（更快地加载更小的内存跨度(footprint)），而且大大提高渲染表现。压缩贴图比起未压缩的32位RGBA贴图占用内存带宽少得多。

之前U3D会议还听说过一个优化，贴图尽量都用一个大小的格式（512 * 512 ， 1024 * 1024），这样在内存之中能得到更好的排序，而不会有内存之间空隙。这个是否真假没得到过测试。

MIPMA[ps](http://www.narkii.com/club/forum-71-1.html)（多重纹理格式）：

http://docs.unity3d.com/Documentation/Components/class-Texture2D.html

跟网页上的略缩图原理一样，在3D游戏中我们为游戏的贴图生成多重纹理贴图，远处显示较小的物体用小的贴图，显示比较大的物体用精细的贴图。这样能更加有效的减少传输给GPU中的数据。


8.LOD
 、 Per-Layer Cull Distances 、 Occlusion Culling

LOD (Level Of Detail) 是很常用的3D游戏技术了，其功能理解起来则是相当于多重纹理贴图。在以在屏幕中显示模型大小的比例来判断使用高或低层次的模型来减少对GPU的传输数据，和减少GPU所需要的顶点计算。

摄像机分层距离剔除(Per-Layer Cull Distances)：为小物体标识层次，然后根据其距离主摄像机的距离判断是否需要显示。

遮挡剔除（Occlusion Culling）其实就是当某个物体在摄像机前被另外一个物体完全挡住的情况，挡住就不发送给GPU渲染，从而直接降低DRAW
 CALL。不过有些时候在CPU中计算其是否被挡住则会很耗计算，反而得不偿失。

以下是这几个优化技术的相关使用和介绍：

Level Of Detail :
http://docs.unity3d.com/Documentation/Manual/LevelOfDetail.html

Per-Layer Cull Distances :
http://docs.unity3d.com/Documentation/ScriptReference/Camera-layerCullDistances.html

Occlusion Culling ：
http://docs.unity3d.com/Documentation/Manual/OcclusionCulling.html


9.关于Realtime
 Shadows（实时阴影）

实时阴影技术非常棒，但消耗大量计算。为GPU和CPU都带来了昂贵的负担，细节的话参考下面：

http://docs.unity3d.com/Documentation/Manual/Shadows.html


10.对GPU优化：采用高效的shader

a.需要注意的是有些(built-in)Shader是有mobile版本的，这些大大提高了顶点处理的性能。当然也会有一些限制。

b.自己写的shader请注意复杂操作符计算，类似pow,exp,log,cos,sin,tan等都是很耗时的计算，最多只用一次在每个像素点的计算。不推荐你自己写normalize,dot,inversesqart操作符，内置的肯定比你写的好。

c.需要警醒的是alpha test，这个非常耗时。

d.浮点类型运算:精度越低的浮点计算越快。

在CG/HLSL中--

float ：32位浮点格式，适合顶点变换运算，但比较慢。
half：16位浮点格式，适合贴图和UV坐标计算，是highp类型计算的两倍。
fixed: 10位浮点格式，适合颜色，光照，和其他。是highp格式计算的四倍。

写Shader优化的小提示:
http://docs.unity3d.com/Documentation/Components/SL-ShaderPerformance.html


11.另外的相关优化：

a.对Draw Call Batching的优化
http://docs.unity3d.com/Documentation/Manual/DrawCallBatching.html

b.对Rendering Statistics Window的说明和提示：
http://docs.unity3d.com/Documentation/Manual/RenderingStatistics.html

c.角色模型的优化建议
用单个蒙皮渲染、尽量少用材质、少用骨骼节点、移动设备上角色多边形保持在300~1500内(当然还要看具体的需求)、PC平台上1500~4000内(当然还要看具体的需求)。

http://docs.unity3d.com/Documentation/Manual/ModelingOptimizedCharacters.html](https://so.csdn.net/so/search/s.do?q=gpu&t=blog)](https://so.csdn.net/so/search/s.do?q=cpu&t=blog)](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=unity&t=blog)




