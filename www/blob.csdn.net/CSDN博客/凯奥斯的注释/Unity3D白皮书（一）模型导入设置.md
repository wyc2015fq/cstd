# Unity3D白皮书（一）模型导入设置 - 凯奥斯的注释 - CSDN博客





2017年08月03日 13:31:47[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：7715
所属专栏：[Unity3D白皮书](https://blog.csdn.net/column/details/13147.html)









**在研究一个东西的时候，我们首先要问三个问题？**

**What？什么是导入设置？**

**我们可以在资源文件的Inspector界面看到这个文件的相关设置，导入设置本质上是AssetImporter或其衍生类。**

**Why？为什么我们要使用导入设置？**

**我们导入的资源大部分是由第三方软件生成的，而这些资源需要指定一些参数或者附加一些信息才能被U3D使用或者更好的使用。**

**How？导入设置是如何产生效果的？**

**在资源导入的同时，会生成与之对应的meta文件，meta文件除了保存资源的GUID和meta文件的创建时间外，还包含了一些其他的设置，其中就包含导入设置的参数，当U3D使用这些资源文件的时候，就会根据meta文件中的设置来进行对应的处理。当然资源文件的Inspector界面上的数值也是根据meta文件反序列化（到对应的AssetImporter）得到的。**




（注：本文中在一些名词上添加了超链接，如果想了解更多，可以直接链接过去）




如下图，是一个模型的导入设置。

![](https://img-blog.csdn.net/20170803125742320)




首先是Model页签：



Scale Factor:缩放因子，不同的建模程序生成的模型文件，尺度单位不同，为了在U3D里统一，就需要进行缩放。模型文件的缩放因子：

> 


.fbx, .max, .jas, .c4d = 0.01



.mb, .ma, .lxo, .dxf, .blend, .dae = 1 



.3ds = 0.1




Use File Scale:使用文件的默认缩放值。

Mesh Compression:网格压缩，会导致失真。官方的想法是，尽可能的压缩网格，只要模型看起来不至于太奇怪。



Optimize Mesh:优化网格，如果开启，网格的定点和三角形会按照U3D既定的一套规则重新排序用以提高GPU性能。



Import BlendShapes:导入BlendShapes（用于表情动画）。



Generate Colliders:生成碰撞体。
KeepQuads:保持四边形，不转换为三角形。我们在U3D中使用的网格，大部分是把所有的面都转换成了三角形，但是某些特定的需求下，四边形会得到更好的效果，例如Tessellation shaders（[细分曲面着色器](http://www.cnblogs.com/zenny-chen/p/4280100.html)）。



Weld Vertices:焊接顶点，如果开启，相同位置的顶点会被合并。

Swap UVs:交换uv，如果有光照贴图的对象UV通道不正确，则使用本项。会交换主uv和光照贴图uv（uv2）。



Generate Lightmap UV:为光照贴图生成UV。如果开启Generate Lightmap UV，会在该选项下面多出四个参数。

![](https://img-blog.csdn.net/20170803130721809)

关于这些参数的含义，可以参考：


http://www.ceeger.com/Manual/LightmappingUV.html





Normals：法线：




- Import：从文件导入。
- Calculate：根据平滑角度（[Smoothing Angle](https://forum.unity3d.com/threads/what-exactly-smoothing-angles-do.81056/)）计算。
- None：禁用。



Tangents：切线（以及[次法线](https://baike.baidu.com/item/%E6%AC%A1%E6%B3%95%E7%BA%BF/3505625)）：


- Import：从文件导入，前提是法线也是导入的。
- Calculate Tangent Space：计算[切线](https://www.zhihu.com/question/23706933/answer/25591714)[空间](https://www.zhihu.com/question/23706933/answer/25591714)。
- Calculate Legacy：计算切线。
- Calculate Legacy – SpliteTangents：如果法线贴图的光照被网格上的缝隙破坏，则选用此项。


（更多：[Normal](http://www.cnblogs.com/lookof/p/3509970.html)[Map](http://www.cnblogs.com/lookof/p/3509970.html)[中的值，](http://www.cnblogs.com/lookof/p/3509970.html)[Tangent
 Space](http://www.cnblogs.com/lookof/p/3509970.html)[， 求算](http://www.cnblogs.com/lookof/p/3509970.html)[Tangent](http://www.cnblogs.com/lookof/p/3509970.html)[与](http://www.cnblogs.com/lookof/p/3509970.html)[Binormal](http://www.cnblogs.com/lookof/p/3509970.html)[](http://www.cnblogs.com/lookof/p/3509970.html)[与](http://www.cnblogs.com/lookof/p/3509970.html)[TBN
 Matrix](http://www.cnblogs.com/lookof/p/3509970.html)）


Import Materials：是否导入材质。如果否，则不生成新的材质，默认使用“Default-Diffuse”。

Material Naming：材质命名方式。

Material Search：材质搜索方式。


然后是Rig页签：

![](https://img-blog.csdn.net/20170803131131820)


Animation Type：动画类型。

- None：无。
- Legacy：传统的。（Animation）
- Generic：一般Mecanim动画。
- Humanoid：人形Mecanim动画。


Avatar Definition：皮肤定义。

- Createfrom thismodel：从模型创建。
- Copyfrom other avatar：复制其他皮肤设置。（[配置Avatar](https://docs.unity3d.com/560/Documentation/Manual/ConfiguringtheAvatar.html)）


[Root Node](https://docs.unity3d.com/560/Documentation/Manual/GenericAnimations.html)：根节点。（Generic）

Optimize Game Object：优化游戏对象，开启此项，可以优化有动画的角色的性能。


最后是Animations页签：

![](https://img-blog.csdn.net/20170803131418626)




Import Animation：是否导入动画。

Bake Animations：用于反向动力学（IK）和拟态。只对Maya,3dsMax &Cinema4D文件有效。

Resample Curves：重采样[动画曲线](https://docs.unity3d.com/560/Documentation/Manual/AnimationCurvesOnImportedClips.html)。（详细解释可以参考[AnimationClip](https://docs.unity3d.com/560/Documentation/Manual/class-AnimationClip.html)中的对应设置）

Anim. Compression：动画压缩。

- Off：不压缩。
- KeyframeReduction：减少关键帧，移除相似的关键帧。
- KeyframeReduction and Compression：减少关键帧并压缩Clip数据。（Legacy ）
- Optimal：最优化，（GenericorHumanoid)）。


Rotation Error：旋转误差（度数）。

Position Error：位置误差。

Scale Error：缩放误差。
（注：当对动画进行优化的时候，会根据以上三种来对动画进行删减。)

Clips：动画剪辑。




当Rig页签选择的Animation Type选择[Legacy](https://docs.unity3d.com/560/Documentation/Manual/Animations.html)时，Animations页签稍微有些变化：

![](https://img-blog.csdn.net/20170803131911697)


其中Wrap Mode代表动画的循环模式，如果不是Default会覆盖掉所有Clip的循环模式：


Default：默认，根据Clip的模式。Once：只播放一次。Loop：循环。PingPong：结束之后，倒着播。Forever：结束之后，循环播放最后一帧










