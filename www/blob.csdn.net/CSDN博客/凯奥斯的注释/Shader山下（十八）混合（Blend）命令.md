# Shader山下（十八）混合（Blend）命令 - 凯奥斯的注释 - CSDN博客





2016年10月19日 23:14:46[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：8600
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









# 混合（Blend）被用于创建透明的对象。渲染图像时，当所有的着色器执行完并且所有的纹理都被应用之后，像素点会被写入屏幕。如何将这些像素与已有图像进行组合？我们就需要使用混合命令来告诉GPU混合的方式。

混合命令有以下这些：


|Blend Off|关闭混合（默认）|
|----|----|
|Blend SrcFactor DstFactor|片元产生的颜色乘以SrcFactor，加上屏幕上已有的颜色乘以DstFactor，得到最终的颜色（写入颜色缓存）|
|Blend SrcFactor DstFactor, SrcFactorA DstFactorA|同上，只不过使用单独的因子SrcFactorA和DstFactorA来混合透明度通道|
|BlendOp BlendOperation|用其他的操作来取代加法混合。|
|BlendOp OpColor, OpAlpha|同上，只不过对于透明度通道使用不同的操作。|

注：此外还有AlphaToMask On命令，用来开启alpha-to-coverage，如果有机会我们会在后续文章中介绍。




混合操作（BlendOp）：


|Add|加法：FinalColor=SrcFactor*SrcColor+DstFactor*DstColor|
|----|----|
|Sub|减法（源-目标）：FinalColor=SrcFactor*SrcColor-DstFactor*DstColor|
|RevSub|减法（目标-源）：FinalColor=DstFactor*DstColor-SrcFactor*SrcColor|
|Min|较小值（逐个通道比较）|
|Max|较大值（逐个通道比较）|

注：还有其他一些混合操作，但是目前只有DX11.1支持



混合因子（Blend）


|One|混合因子1，表示完全的源颜色或目标颜色|
|----|----|
|Zero|混合因子0，舍弃掉源颜色或目标颜色|
|SrcColor|源颜色值|
|SrcAlpha|源透明度|
|DstColor|目标颜色值|
|DstAlpha|目标透明度|
|OneMinusSrcColor|1-SrcColor|
|OneMinusSrcAlpha|1-SrcAlpha|
|OneMinusDstColor|1-DstColor|
|OneMinusDstAlpha|1-DstAlpha|

常用的混合命令有：


```cpp
Blend SrcAlpha OneMinusSrcAlpha // 传统透明度
Blend One OneMinusSrcAlpha // 预乘透明度
Blend One One // 叠加
Blend OneMinusDstColor One // 柔和叠加
Blend DstColor Zero // 相乘——正片叠底
Blend DstColor SrcColor // 两倍相乘
```

当然，相乘/正片叠底的混合命令写成：



```cpp
Blend Zero SrcColor
```

也是合情合理合法的。




最后，我们还可以扩展一下：

参考百度百科[滤色](http://baike.baidu.com/link?url=EV5cUYkm4tGAA5_uz4h1DfmsGubqGcHJPG-qI6G1lkwLudQhkpA92gk7a3cZyIlO0Wx6c1YoPVUvTKWrFtmhpOf4DYX-VOu_Q3r1vDyoajG)，我们知道，滤色的公式是：

结果色=255－[（255－基色）×（255－混合色）]/255

对应于[0,1]范围内也就是

FinalColor=1-(1-Dst)*(1-Src)

也就是

Dst+Src-Dst*Src

也就是

Src*1+Dst*(1-Src)

或者

Src*(1-Dst)+Dst*1

所以滤色的混合命令我们可以写成下面两种：



```cpp
Blend OneMinusDstColor One
Blend One OneMinusSrcColor
```












