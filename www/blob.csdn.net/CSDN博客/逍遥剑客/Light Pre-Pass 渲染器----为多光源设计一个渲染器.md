# Light Pre-Pass 渲染器----为多光源设计一个渲染器 - 逍遥剑客 - CSDN博客
2009年08月18日 22:55:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：7038
作者: [Wolfgang Engel](http://diaryofagraphicsprogrammer.blogspot.com/2008/03/light-pre-pass-renderer.html), 原文: [http://www.wolfgang-engel.info/RendererDesign.zip](http://www.wolfgang-engel.info/RendererDesign.zip)
渲染器设计就像为一座房子打地基, 结果可能比预期大了, 小了, 或者太笨重了. 预先了解要在它之上建造什么, 并且为未来的扩展做出合理的预测, 是建造一座稳定房子的先决条件. 
用软件设计的术语来说, 打地基好比渲染器设计, 而房子, 大小, 轻重就是项目开始前的图形需求列表, 一般它会在项目进行中变更很多东西.
这篇文章假设玩家处于一个能够看到时间变化的游戏中, 如GTA IV, 或者Midnight Club Los Angeles. 在这些游戏中, 光照环境是无法预计算的. 照亮场景所需要光源的庞大数目是当前渲染器设计的一个巨大挑战. 
而这个挑战, 正是这篇文章的主题.
现在我们看看过去8年里为解决这个问题而发展出的三种不同的渲染器设计模式: Z Pre-Pass 渲染器, 延迟(Deferred)渲染器和 Light Pre-pass 渲染器.
**Z Pre-Pass ****渲染器**
现在被称作Z Pre-Pass 的渲染器最初是John Carmack在他的blog上面描述DOOM III渲染器时提及的. 
它的思想是首先进行一个只有深度的pass(Z pre-pass), 用深度数据填充Z-buffer的同时完成Z剔除, 然后利用这些数据来进行光照, 避免了像素的重复绘制. 这个方法早已被硬件厂商用于改进他们的硬件设计.
只渲染深度的话可以比颜色和深度一起写入快2-8倍. 这里有一张表示这个渲染流程的图: 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090818/image001.jpg)
仔细考虑一下第一和第四阶段你会发现, 它只处理了不透明物体..
像第6阶段描述的那样, 透明物体会走一条单独的渲染路径(render path). 不透明物体绘制完毕后, 深度写入会关闭, 然后才从后到前地绘制透明物体. 所以它们在任何时候都不会写入深度缓冲, 并且也不在Z pre-pass的考虑范围内..
相对于Z Pre-Pass渲染器来说, 一个传统的解决方案就是在pixel shader里渲染特定数目的灯光. 假设一个pixel shader编写为8个光源上限, 这意味着我们可以在每次绘制调用(draw-call)时绘制8个点光源, 而不用关心物体是否透明. 
虽然这可以让透明和不透明的物体的shader很相似, 但这可能会导致在特定游戏设置下需要根据所受光照数目的不同而分割几何体. 而这样的代价过于昂贵, 因为当前的硬件管线很长, 对于绘制调用的次数十分敏感. 举例来说, 一般PC上每帧不应该多于500次绘制调用. 
一个更高级的方法是把游戏世界中的灯光属性(位置, 颜色等)保存到2D纹理中, 再从一张索引纹理从中取得可见灯的ID, 从而可以在灯光属性纹理中取得所有灯光的属性.
因为灯光属性的取得依赖索引纹理的结果, 而当前硬件对于这样的多次纹理读取仍然存在问题. 这种方式的优点就是透明与不透明物体的渲染路径几乎一致
.
这种方案在大量光源时的消耗引出了另一个渲染器设计模式, 现在被称作延迟光照(Deferred Lighting). 
**延迟渲染器**
一个延迟渲染器的基本思想基于SIGGRAPH 1988 [Deering]的论文.
与Z pre-pass渲染器相似, 延迟渲染器模式同样把不透明物体的渲染分为两个阶段. 相对于Z Pre-Pass只预先输出深度值并在第二个阶段利用硬件深度作遮挡剔除, 延迟渲染器则把所有光照和阴影计算所需要的数据渲染到了render target. 第一个阶段一般称作G-Buffer写入或几何阶段. 接下来的第二阶段称为光照阶段, 每个光照相加混合到光照缓冲(light buffer), 阴影在这过程中同时进行计算. 
一个典型的G-Buffer 保存了以下数据[Valient]:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090818/image003.jpg)
与透明物体的渲染路径一起, 看起来是这样的: 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090818/image005.jpg)
使用G-Buffer 的前提是这些数据可以一次性写入多个render target, 即Multiple-Render-Target (MRT).
Killzone 2 使用属于同一个MRT的4个render target和一个深度缓冲. 在第一阶段—几何阶段中, render target RT1, RT2, RT3和深度缓冲被填充. 在这基础上, 光照缓冲RT0中的数据在光照阶段才能被填充. 
通常G-Buffer中保存所有场景中可见物体的材质, 运动模糊向量, 深度数据(用于恢复出位置信息)和模板数据. 对于所有物体来说, 它们有着各不相同的镜面属性, 法线和颜色值. 所以物体的镜面反射计算结果因为不同的反射率而有所差异. 
延迟渲染器的优势就是大量数目的灯光可以相加混合到光照缓冲, 而与几何约束和几何体绘制调用无关. 
因为透明物体无法渲染到深度缓冲, 它们需要一个专门的阶段进行处理. 现今的游戏中这个阶段跟上面提到的Z Pre-Pass渲染器很相似. 在最新的DirectX 10硬件之上,像reverse depth peeling [Thibieroz07]或stencil routed K-buffer [Bavoli] 这样的新技术可以保存多层深度, 让透明物体可以像不透明物体那样进行渲染.
延迟渲染器模式的缺点就是在渲染很多灯光时所需要的巨大读写带宽. 
这里有一些技术可以用来优化内存带宽.
一个简单的优化带宽方法是剪裁灯光投影到2D矩形的3D包围盒 [Placeres].
别一个降低带宽使用的方法是绘制凸面几何体. 对于点光源, 这可以是一个球体, 对于聚光灯, 这可以是一个圆锥体. 为了适应摄像机在包围体内部的情况, 仅仅绘制包围体的背面. 另外如果只绘制包围体的背面的话, 深度缓冲的可见性测试会失败. 这可以在绘制包围体时反转深度测试(D3DCMP_GREATER 代替D3DCMP_LESSEQUAL)来解决 [Thibieroz04].
一个更完善的方法是使用像depth-fail stencil shadow volume 技术那样的模板测试. 与上一个方法类似, 当绘制反面的灯光几何体, 在深度缓冲以D3DCMP_GREATER替代D3DCMP_LESSEQUAL时测试失败时增加模板测试. 当绘制正面的灯光几何体时, 设置深度测试为D3DCMP_LESSEQUAL并且在深度测试失败时减少模板测试. 这样渲染器只会照亮模板值大于等于1的像素[Hargreaves][Valient].
延迟渲染器在硬件MSAA支持方面相对于Z Pre-Pass渲染器更加复杂, 因为在PC平台DirectX 9下MSAA是在运行时处理的. 程序员并不能影响MSAA在哪和怎样发生, 但这对于延迟光照渲染器是必不可少的. 在XBOX 360和PS3 硬件上虽然可行但是代价十分昂贵, 因为这并不适合当前的硬件设计.
延迟渲染器的另一个挑战是材质的种类. 因为G-Buffer中的空间是十分有限的, 这样与Z Pre-Pass相比能够使用的材质种类就少了许多. 另外, 所有材质需要使用一个很类似的光照公式. 光照阶段的pixel shader切换代价非常奢侈, 因为这是在屏幕空间逐像素发生的. 
材质受限和巨额的内存带宽需求引出了Light Pre-Pass渲染器的思想.
**Light Pre-Pass ****渲染器**
相对于延迟光照渲染器在G-Buffer中保存材质属性, Light Pre-Pass渲染器保存深度和法线到一或两个render target. 在第二个渲染阶段, Light Pre-Pass渲染器保存所有灯光的光照属性到一个光照缓冲(light buffer). 所以它只在光照缓冲中保存光照公式的很小一部分, 而不像延迟光照渲染器那样保存所需的全部数据.
这是渲染器的布局:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090818/image007.jpg)
与Z Pre-Pass和延迟着色渲染器设计类似, Light Pre-Pass渲染器利用一个单独的渲染路径来绘制透明物体. 与Z Pre-Pass渲染器类似, Light Pre-Pass渲染器把深度放在第一阶段. 主要的修改是不得不渲染法线, 因此丧失了只写深度在当前硬件下的双倍速度优势..
渲染输出光照属性的必要条件是找出一种分离光照公式的方法, 这样才能在之后重建分离的灯光属性和其它属性.
下面来看一个简单的点光源光照公式:
Color = Ambient + Shadow * Att * (N.L * DiffColor * DiffIntensity * LightColor + R.V^n * SpecColor * SpecIntensity * LightColor)
依赖灯光方向和颜色的属性有:
- N.L
- LightColor
- R.V^n
- Attenuation
一种使用render target的4个分量保存这些灯光属性的方法:
LightColor.r * N.L * Att
LightColor.g * N.L * Att
LightColor.b * N.L * Att
R.V^n * N.L * Att
换作聚光灯, 这里的Att分量可以用于表示聚光灯因子(spotlight factor). 在渲染灯光属性到光照缓冲时, 优化带宽的规则可以参考延迟着色渲染器. 
为了证明这个方案的可行性, 在RenderMonkey中与延迟渲染器一同实现了两个例子. 下面是Light Pre-Pass的截图:
[](http://bp0.blogger.com/_2YU3pmPHKN4/R93MVCsl4bI/AAAAAAAAABA/pFmdfubodSo/s1600-h/LightPrePass.jpg)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090818/image009.jpg)
下面的截图展示了在延迟渲染器下的同一个场景. 与Light Pre-Pass的结果相比应该没有什么差别:
[](http://bp3.blogger.com/_2YU3pmPHKN4/R93NIysl4cI/AAAAAAAAABI/WBF812V3x8Y/s1600-h/DeferredRenderer.jpg)![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090818/image010.jpg)
这个Light Pre-Pass渲染器的实现还没有办法在forward rendering pass中分别重建漫反射(diffuse)和高光(specular)分量, 因此它仍然无法支持很多种类的材质.
有三种可能可进的方案:
- 可以保存另一个分量用于在forward rendering pass中重建高光分量
- 高光分量的重建规则可以创建一个适合这个渲染器设计的新分量来绕过
- 更换一个不同的颜色空间, 使用这些思想来完成一个更为紧凑的render target
这篇文章只讨论前两个方案, Pat Wilson的文章[Wilson] 展示了怎样使用LUV颜色空间在一个8:8:8:8 render target上存储更多的灯光属性.
**保存额外的漫反射分量**
为了在forward渲染路径中重建高光分量, 需要保存一个漫反射分量. 所有需要保存的灯光属性最后有这些: 
LightColor.r * N.L * Att
LightColor.g * N.L * Att
LightColor.b * N.L * Att
R.V^n * N.L * Att
N.L * Att
有了额外的分量, 我们就可以用下面的公式重新构造R.V^n:
(R.V^n * N.L * Att) / (N.L * Att)
这样高光分量就表示了这个像素的灯光(点光源, 聚光灯, 方向光). 像这样可以增加一个高光强度(specular power)值:
(R.V^n)^mn
高光颜色是一个材质属性, 有些渲染器设计也会把它当做灯光的属性. 
例如, 要增加一个高光颜色可以像这样在forward rendering pass中乘上高光分量:
(R.V^n)^nm * Spec
因为漫反射和高光分量在forward rendering pass中是可以分离的, 所以可以支持广泛的材质.
这个方式的缺点就是, 它需要一个额外的通道来保存第5个分量, 超过了一个4通道render target的数目, 因此需要为光照缓冲增加一个render target.
**调整高光光照规则**
在上面这个灯光属性保存的最简单的方法的基础上, 可以借助高光材质强度值调整高光分量的重建.
(R.V^n * N.L * Att)^nm
实际的视觉效果证明这是切实可行的. 这样所有的灯光属性都可以放入一个4通道的render target, 同时我们可以为表面应用各种各样的材质属性.
Light Pre-Pass除了在forward rendering pass中有一个更灵活的分量, Z Pre-Pass中大多数针对光照公式做的修改也适用于Light Pre-Pass渲染器. 例如, 要在forward rendering pass中使用一个菲涅尔(Fresnel)分量, 你可以使用法线图中的法线和视图方向N.V. 
**总结**
就材质实现而言, Light Pre-Pass渲染器比延迟光照渲染器更加灵活. 与Z Pre-Pass渲染器相比, 虽然灵活性稍欠但有着更快的多光源解决方案.
与延迟光照渲染器相比主要的缺点是需要渲染主视图的几何次两次, 但这也是Z Pre-Pass的需求. 渲染两次主视图几何体的消耗与shadow map的4次几何体pass相比没有多大差异.
除却渲染两次主视图可见的几何体所带来的pixel shader性能消耗提升, pixel shader的性能仍然是大多数游戏的瓶颈.
因为Light Pre-Pass渲染器使用forward rendering 来渲染最终的场景, 所以它可以像forward renderer那样提供最好的MSAA支持.
每个光源读写render target的内存带宽理应得到降低, 因为每个灯光只需要取得两张纹理, 而延迟光照渲染却需要4张. 这在内存带宽受限的平台上会有很大的不同.
另外, Light Pre-Pass渲染器的简化版本可以在低端图形硬件上实现, 因为它不需要MRT的支持, 并且所需要的pixel shader也非常简单.
**鸣谢**
我要感谢这些人跟我一起讨论这篇文章中提到的各种技术(排名不分先后): Steve Reed, Thomas Johnstone, Raymond Kerr, Ray Tran, Christina Coffin, Simon Brown, Pat Wilson, Matthias Wloka, Michal Valient 还有 Michael Krehan. 很多这样的讨论形成了这篇文档中的新思想或是他们的思想融入进来.
**参考资料****:**
[Bavoil] Louis Bavoil, Kevin Myers, Deferred Rendering using a Stencil Routed K-Buffer, ShaderX6
[Calver] Dean Calver's article in deferred rendering on beyond3d http://www.beyond3d.com/content/articles/19/
[Carmack] John Carmack ??
[Deering] Michael Deering "The triangle processor and normal vector shader: a VLSI system for high performance graphics" SIGGRAPH 1988
[Hargreaves] Shawn Hargreaves, “Deferred Shading”, [http://www.talula.demon.co.uk/DeferredShading.pdf](http://www.talula.demon.co.uk/DeferredShading.pdf)
[Placeres] Frank Puig Placeres, “Overcoming Deferred Shading Drawbacks”, pp. 115 – 130, ShaderX5
[Thibieroz04] Nick, Thibieroz, “Deferred Shading with Multiple-Render Targets”, pp. 251- 269, ShaderX2 – Shader Programming Tips & Tricks with DirectX9
[Thibieroz07] Nick, Thibieroz, “Robust Order-Independent Transparency via Reverse Depth Peeling in DirectX® 10”, ShaderX6
[Valient] Michal Valient, “Deferred Rendering in Killzone 2”, [http://www.guerrilla-games.com/publications/dr_kz2_rsx_dev07.pdf](http://www.guerrilla-games.com/publications/dr_kz2_rsx_dev07.pdf)
[Wilson] Pat Wilson, “Light Pre-Pass Renderer: Using a LUV color model”, ShaderX7
