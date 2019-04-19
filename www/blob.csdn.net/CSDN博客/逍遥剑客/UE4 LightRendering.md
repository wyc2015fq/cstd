# UE4 LightRendering - 逍遥剑客 - CSDN博客
2018年11月09日 19:10:11[xoyojank](https://me.csdn.net/xoyojank)阅读数：271
最近把HairWorks整合到了[UE4 4.20](https://github.com/xoyojank/UnrealEngineHairWorks), 整合的过程中发现了一些没法直接merge的代码, 主要集中在LightRendering.cpp和DeferredLightingPixelShader.usf中. 所以顺便简单读了一下代码, 把其中的一些关键字概念整理出来方便查阅理解.
# Uniform Buffer Struct
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181105233307610.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r,size_16,color_FFFFFF,t_70)
UE4使用了一些宏来辅助定义UniformBuffer中的结构体, 在Shader中是可以打到对应的定义的.
# Simple Light
不如叫[Particle Light](https://docs.unrealengine.com/en-us/Engine/Rendering/ParticleSystems/ParticleLights), 粒子系统中发射的简单点光源. 因为数量很多, 所以算法做了简化, 没有阴影之类的效果
# Light Function
[Light Function](https://docs.unrealengine.com/en-us/Engine/Rendering/LightingAndShadows/LightFunctions)在我看来更像是一个可以用材质节点控制的Shadow Mask, 改变光照区域的强度, 不能叠加颜色.
# Stenciling Geometry
有三种:
- Sphere: 点光源, 面积光
- Vector Sphere: 好像是做BoundSphere的?
- Cone: 聚光灯
# DBT (Depth Bound Test)
用来剔除挡住的光源的
# Radial Light
点光源或聚光灯都是Radial Light, 方向光就不是, 做选择题时字面意思很好理解, 但是第一眼看到时就很蒙.
# IES Profile
见[IES Light Profiles](https://docs.unrealengine.com/en-US/Engine/Rendering/LightingAndShadows/IESLightProfiles), 参考现实中照明设备的标准的一个亮度曲线算法, 跟强度用流明一个套路
# Lighting Channel
Forward Rendering时代美术惯用的技俩, UE4一开始用了Deferred Rendering没加的, 估计架不住用户习惯又加回来了, 见[Lighting Channels](https://docs.unrealengine.com/en-us/Engine/Rendering/LightingAndShadows/LightingChannels)
# Transmission
SSS相关
# LTC(Linearly Transformed Cosines)
Rect Light用的
参考[ Heitz et al. 2016, “[Real-Time Polygonal-Light Shading with Linearly Transformed Cosines](https://eheitzresearch.wordpress.com/415-2/)”
# Direct Lighting
渲染simple light时用的tiled deferred lighting, 估计粒子点光太多了架不住.
其它光源就是延迟渲染的标准做法了.
# Indirect Lighting
这里有两个关键名词, 实时GI估计大家都是这么搞的吧:
- [Reflective Shadow Maps](http://www.klayge.org/material/3_12/GI/rsm.pdf)
- [LPV(Light Propagation Volumes)](https://docs.unrealengine.com/en-us/Engine/Rendering/LightingAndShadows/LightPropagationVolumes)
# Shadowed Lights
其实就是开了阴影的光源
