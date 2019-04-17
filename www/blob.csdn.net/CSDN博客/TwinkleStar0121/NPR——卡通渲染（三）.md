# NPR——卡通渲染（三） - TwinkleStar0121 - CSDN博客





2018年08月08日 14:13:15[TwinkleStar0121](https://me.csdn.net/jvandc)阅读数：581










- - - [NPR——卡通渲染](#npr卡通渲染)
- [1.1 Illustrative Rendering in Team Fortress 2 [1]](#11-illustrative-rendering-in-team-fortress-2-1)- [1.1.1 《军团要塞2》插画风格抽象描述](#111-军团要塞2插画风格抽象描述)
- [1.1.2 视觉无关的光照](#112-视觉无关的光照)
- [1.1.3 视觉相关的光照](#113-视觉相关的光照)

- [1.2 一点想法](#12-一点想法)
- [Reference](#reference)






### NPR——卡通渲染

这是我“NPR——卡通渲染”系列的第三篇文章，以此结束卡通渲染第一个阶段的研究学习。在本系列文章第一篇“NPR——卡通渲染（一）”文中，讲美式卡通[[2](https://blog.uwa4d.com/archives/usparkle_cartoonshading.html)]时，提到过 Valve 公司制作《军团要塞2》一篇著名的卡通渲染论文[1]，本文正是对该论文学习的总结。 

（本文着重于论文的分析学习，在 Unity 实现了相应的效果，本文不再贴出代码，有需要可参考其他文章如[[4](https://zhuanlan.zhihu.com/p/25844886)]。）

### 1.1 Illustrative Rendering in Team Fortress 2 [1]

本文不会翻译完整的译文（较完整的中文译文可参见[[3](https://blog.csdn.net/candycat1992/article/details/37696187)]），以我的理解，《Illustrative Rendering in Team Fortress 2》[1]这篇论文大体分为四段讲述了《军团要塞2》的制作过程，**首先**是《军团要塞2》这款游戏的风格定位，论文第一节提到《军团要塞2》的艺术风格灵感来自于20世纪中时期的几位商业插画家——J. C. Leyendecker、Dean Cornwell 和 Norman Rockwell [Schau 1974]，插画人物衣物褶皱轮廓鲜明，并且以加强轮廓和边缘高光而不是暗色描边的着色技巧凸出物体或者人物的内部细节；**其次**论文介绍了《军团要塞2》的技术基础，比如“Gooch Shading”——一种插画着色技巧，使用冷暖色调的变化表示表面方向（法线方向）和光源的关系（几何上关系就是二者的夹角）改变传统的  Phong Light Model，相比传统的光照模型，在复杂的光照环境下，会得到更好的 3D 视觉表现。  [Decaudin 1996] [Lakeet al. 2000] [Barla et al. 2006]，Decaudin 使用 constant diffuse color 并依赖于 Shadow mapping 渲染物体达到卡通风格表现，Lakeet 使用的方法不依赖于 Shadow mapping，他使用一张一维贴图（基于Lambertian 采样纹理）模拟卡通绘画的调色板（降低色阶），Barla 更进一步扩展了 Lakeet 的方法，他使用一张二维贴图调和 view-dependent（视线相关）和 level-of-detail effects，同时，Barla 还应用了 Fresnel-Like（菲涅尔效应）模拟“virtual backlight“，也就是边缘光照（rim-lighting）；**第三**，论文讲述了《军团要塞2》的制作流程，从”场景“、”人物“，再到”贴图绘制“等；**最后**，论文将角色的绘制（着色）分为了两部分，一是视觉无关（好理解一点的说法是”与视线无关“），包括”半兰伯特光照“、”漫反射变形函数“以及”平行环境光“。二是视觉相关，包括”多重 Phong 光照“和”风格化的边缘光照“。

#### 1.1.1 《军团要塞2》插画风格抽象描述

论文的第三节（Third Section）总结了五点插画风格要素，以此作为《军团要塞2》着色技术的基础（我认为这一节的总结对于图形程序员来说才是最重要的，只有将插画的风格要素提取出来，具象为程序员可以量化的值，我们才可以很好的实现插画风格。总言之，这种思考问题的方法是我应该学习且具备的）：
- 着色从暖到冷变化，阴影向冷色变化，而非黑色。
- 在光源的明暗分界处增加颜色饱和度，分界颜色往往偏红色。
- 高频细节尽可能忽略它。
- 对于角色而言，其内部细节如衣物褶皱通过重复轮廓线来强调它。
- 使用边缘高光而不是暗色描边来强调轮廓线。

#### 1.1.2 视觉无关的光照

视觉无关的光照算法用公式（1）概括： 


$k_d[a(\widehat{n}) + \sum_{i=1}^{L}{c_iw((\alpha(\widehat{n} \cdot \widehat{l_i}) +\beta)^\gamma)}  ] \tag{1}$

$k_d$ 是主纹理颜色，$L$ 表示光源数量，$c_i$ 表示光源光照颜色，$\alpha$，$\beta$，$\gamma$ 分别是一个缩放因子、偏移量和指数因子，用来改变 Lambert 光照结果，$a()$ 函数是一个平行环境光函数，$w()$ 是一个变形函数，它将值映射到区间 $[0,1]$。 

半兰伯特光照不做讨论了，我们首先看看“漫反射变形函数”： 
![](https://img-blog.csdn.net/20180808141042722?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2p2YW5kYw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以看到，这个一维贴图左边区块偏黑色，中间区块偏红色，而右侧偏灰色，为什么右侧颜色也仅仅是比“mid-gray”稍稍亮一些而已呢？论文中提到，这是因为在 pixel shader 中，对该纹理采样之后的颜色会乘以 2，这样可以让美术同学更好的控制明亮度（two times “overbright” 两倍的亮度）。贴图分为三块趋于，这样就使得插画风格的阴影偏向冷色而不是黑色，在明暗分界处则偏向红色，光照处则偏向暖色。 

接下来我们看看论文中比较重要的“平行环境光部分”，论文中使用的“ambient cube”技术等价于“irradiance environment map（辐照度环境贴图）”（可参考 GPU Gems 2 第 10 章）。二者的区别在于后者是计算一个 9 项的球谐函数（9-term spherical harmonic basis），前者则仅仅需要沿坐标轴正负方向计算 6 项的余弦平方的“ambient cube”。它使得角色和模型在游戏场景中显得更加真实生动。
#### 1.1.3 视觉相关的光照

视觉相关的光照算法用公式（2）表示如下： 


$\sum_{i=1}^{L}[c_ik_smax(f_s(\widehat{v} \cdot \widehat{r_i})^{k_{spec}}, f_rk_r(\widehat{v} \cdot \widehat{r_i})^{k_{rim}})] + (\widehat{n} \cdot \widehat{u})f_rk_ra(\widehat{v}) \tag{2}$

可以看到，视觉相关的光照算法加入了菲涅尔因子。 

首先我们看看“多重 Phong 高光部分”，相比传统的高光，这里多了 $f_rk_r(\widehat{v} \cdot \widehat{r_i})^{k_{rim}}$，论文提到菲涅尔因子 $f_r$ 使得“rim highlights”仅仅在“grazing angles”处才会显现出来，通常情况下常量$k_{rim}$ 小于 $k_{spec}$。 

我们再看看公式（2）的另一部分“Dedicate Rim Lighting”，$(\widehat{n} \cdot \widehat{u})f_rk_ra(\widehat{v})$，这里的边缘光照一方面使用 viewDir 求取 “ambient cube” 值， 另一方面计算向上向量$\widehat{u}$ 和表面法向量的点积，这样可以获得一种由上方照下的间接环境光照表现。
### 1.2 一点想法

纵览整篇论文，我所得不在于论文中卡通渲染的算法公式，而在于论文中一点一点如抽丝剥茧的般细致的描述了《军团要塞2》插画渲染的过程——从灵感来源，到抽取插画风格要素，再到以此为基础建立相应的数学模型，获益良多。

### Reference

[1] Jason Mitchell, Moby Francke, Dhabih Eng. Illustrative Rendering in Team Fortress 2 
[[2] 侑虎科技——卡通渲染技术总结](https://blog.uwa4d.com/archives/usparkle_cartoonshading.html)
[[3] 【Shader拓展】Illustrative Rendering in Team Fortress 2](https://blog.csdn.net/candycat1992/article/details/37696187)
[[4] 风格化渲染实践](https://zhuanlan.zhihu.com/p/25844886)





