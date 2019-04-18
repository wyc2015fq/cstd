# Unity3D白皮书（三）MeshRenderer设置 - 凯奥斯的注释 - CSDN博客





2017年08月26日 17:18:24[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：6782
所属专栏：[Unity3D白皮书](https://blog.csdn.net/column/details/13147.html)









# **[Unity3D白皮书（一）模型导入设置](http://blog.csdn.net/ecidevilin/article/details/76616690)这边博文中，我们介绍了模型的导入设置。那么当模型导入之后，我们不可避免的要接触到的组件之一便是MeshRenderer。那么本文将介绍MeshRenderer的相关设置。**

（注：本文中在一些名词上添加了超链接，如果想了解更多，可以直接链接过去）


（注：U3D版本5.6.1f1）


**![](https://img-blog.csdn.net/20170826170548315)**






[Light Probes](https://docs.unity3d.com/Manual/LightProbes.html):光探头插值方式（[Unity3D的LightProbe动态光探头用法介绍](http://www.cnblogs.com/slysky/p/4303343.html)）。

- Off：关闭。
- BlendProbes：混合。
- UseProxy Volume：使用代理集。






[Reflection Probes](https://docs.unity3d.com/Manual/ReflectionProbes.html)：如果启用并且场景中存在反射探头，会为这个对象生成一张反射贴图，并设置为内置着色器的统一变量（uniformvariable）。（[Unity5.x
 Reflection Probe反射探针](http://blog.csdn.net/yupu56/article/details/53487216)）

- Off：关闭。
- BlendProbes：混合反射探头，适用于室内场景，如果附近没有反射探头，渲染器（Renderer）会使用默认反射，即天空盒，但是不会两者都混合。
- BlendProbes And Skybox：混合反射探头和天空盒，适用于室外场景。
- Simple：简单。如果有两个重叠量时，不会在探头之间发生混合。


 Anchor Override：当使用光探头或反射探头系统时，用来决定插值位置的Transform。


 Cast Shadows：投射阴影方式。

- Off：关闭。
- On：当产生阴影的光（shadow-castingLight）照射在网格上时，它会投射出一个阴影。
- TwoSided：可以从网格的任意一遍投射出阴影，适用于半封闭物体。
- ShadowsOnly：只显示阴影，不显示网格。


 Receive Shadows：是否接受阴影。


[Motion Vectors](https://docs.unity3d.com/ScriptReference/Renderer-motionVectorGenerationMode.html)：运动向量生成方式。（[DepthTextureMode.MotionVectors](https://docs.unity3d.com/ScriptReference/DepthTextureMode.MotionVectors.html)）

 通过跟踪相邻两帧之间的像素速度产生运动向量。可以使用它来应用某些特殊的图像特效，例如[运动模糊](https://en.wikipedia.org/wiki/Motion_blur)和时序抗锯齿。

- CameraMotion Only：仅适用相机移动来跟踪运动。
- PerObject Motion：会渲染出一个“每个对象”运动向量通道。
- ForceNo Motion：不会渲染出运动向量。


 Lightmap Static：告诉Unity，这个对象的位置是固定的，并将参与全局光照（GlobalIllumination）的计算。

 Optimize Realtime UVs：指定是否为了实时全局光照来优化生成的网格UV。如果开启，这些UV会为了优化而被合并、缩放和打包。如果不开启，这些UV会被缩放和打包，但是不会合并。需要注意的是，这种优化有时候会造成误判和原始UV图的不连续。

 Max Distance：为简化UV图指定一个最大的世界距离，这个距离之间的UV图会被简化。

 Max Angle：指定共享一个UV边缘的两个面之间的最大角度。如果两个面之间的角度小于这个值，UV图会被简化。

 Ignore Normals：忽略法线，可以防止在实时GI的预计算过程中UV图被分裂。

 Min Char Size：指定UV表的最小纹元（texel）尺寸。如果需要拼接，那么就会创建一个4x4的纹元来存放光照和方向。如果不需要拼接，那么就可以降低纹元密度并提供更快的光照构建速度和更好的游戏性能。

 Scale In Lightmap：指定对象UV在光照贴图中的相对尺寸。

- 0：不会受光照贴图影响，但是依然可以照亮场景的其他对象。
- >1：增加在光照贴图中的像素数量。
- <1：减少在光照贴图中的像素数量。


 可以使用这个值来优化光照贴图，那些重要的和细致的区域就可以被更准确的照亮。

 Prioritize Illumination：告诉Unity，在光照计算中，要始终包含这个物体。适用于拥有强烈自发光的对象，可以确保其他对象可以被这个对象照射到。

 Lightmap Parameters：选择光照贴图配置表。


Materials：材质。材质的数量跟子Mesh的数量是对应的。






