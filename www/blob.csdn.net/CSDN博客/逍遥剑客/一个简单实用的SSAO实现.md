# 一个简单实用的SSAO实现 - 逍遥剑客 - CSDN博客
2010年07月14日 15:27:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：26704

by José María Méndez 
原文链接: http://www.gamedev.net/reference/programming/features/simpleSSAO/
**绪论**
全局照明(global illumination, GI)是一个计算机图形学术语, 它指的是所有表面之间相互作用的光照现象(光线来回跳动, 折射, 或者被遮挡), 例如: 渗色(color bleeding), 焦散(caustics), 和阴影. 很多情况下, GI这个术语代表的只是渗色和逼真的环境光照(ambient lighting). 
直接照明– 光线直接来自光源– 对于今天的硬件来说已经非常容易计算, 但这对于GI并不成立, 因为我们需要收集场景中每个面的邻近面信息, 这样的复杂度很快就会失控. 不过, 也有一些容易控制的GI近似模拟方式. 当光线在场景中传播和跳动时, 有一些地方是不容易被照到的: 角落, 物体之间紧密的缝隙, 折缝, 等等. 这就导致了这些区域看起来比它们周围要暗一些. 
这个现象被称为环境遮蔽(ambient occlusion, AO), 一般用于模拟这种区域变暗的方法是: 对于每个面, 测试它被其它面”阻挡”了多少. 这样的计算比起全局光照来说要快得多, 但大多数现有的AO算法还没法实时地运行. 
实时AO在屏幕空间环境遮蔽(Screen Space Ambient Occlusion, SSAO)出现之前一直被认为是达不成的目标. 它的第一次应用是在Crytek的”Crysis”这款游戏中, 之后的很多其它游戏也使用了这项技术. 在这篇文章中, 我会讲解一种简单明了, 但效果又好于传统实现的SSAO方法.
![](http://hi.csdn.net/attachment/201007/14/0_1279096007z77S.gif)
*Crysis中的SSAO*

**准备工作 **
最初Crytek的实现是用一个深度缓冲做为输入, 粗暴地进行这样的工作: 对于每个深度缓冲中的像素, 采样周围3D空间中的一些点, 投影回屏幕空间并比较采样点和深度缓冲中相同位置的深度值, 以此判断采样点是在面前(没被遮挡)还是在面后(遇到一个遮挡体). 这样经过对深度缓冲的采样, 平均遮挡体的距离后得出就得到了一个遮闭缓冲. 但是这种方式存在一些问题(如自遮闭, 光环), 之后我会说明. 
这里我叙述的算法的所有计算都是在2D空间中进行, 不需要进行投影变换. 它用到了每个像素的位置和法线缓冲, 所以如果你已经使用了延迟渲染的话, 一半的工作已经完成了. 如果没有, 你可以[从深度缓冲中重建位置信息](http://blog.csdn.net/xoyojank/archive/2010/02/06/5294575.aspx), 或者直接把每个像素的位置保存到浮点缓冲中去. 如果你是第一次实现SSAO, 那么我建议后者, 因为在这里我不会讲解如何从深度缓冲中去重建位置信息. 无论是哪种方式, 在接下来的文章中, 我会假设你已经有这两个缓冲可用. 另外, 位置和法线需要是视图空间的. 
接下来我们要做的事情就是: 使用位置和法线缓冲生成一个每像素对应一个分量的遮闭缓冲. 怎么使用遮闭信息的决定权在你; 通常的方法是从场景的环境光照中减去它, 但是如果你愿意的话, 也可以用来做一些非真实(NPR, non-photorealistic)渲染效果. 
**算法**
对场景中的任意像素, 可以这么计算它的环境遮闭: 把所有周围的像素当做小球, 计算它们的贡献度之和. 为了简单起见, 我们把所有的小球当成点: 遮挡者仅仅是没有朝向的点, 那么被遮挡者(接受遮闭的像素)只是一个<位置, 法线>对. 
因此, 每个遮挡者的遮闭贡献度取决于两个因素: 
- 到被遮挡者的距离“d”.
- 被遮挡者的法线”N”与两者(遮挡者与被遮挡者)之间向量”V”的夹角.
有了这两个因素, 一个计算遮闭的简单公式就出来了: 
**Occlusion = max( 0.0, dot( N, V) ) * ( 1.0 / ( 1.0 + d ) ) **
第一项max( 0.0, dot( N,V ) ), 直觉上来说就是位于被遮挡者正上方的的点比其它点的贡献度更大. 第二项的作用是按距离线性衰减效果, 当然你也可以选择使用平方衰减或其它衰减函数, 但凭个人喜好了.
![](http://hi.csdn.net/attachment/201007/14/0_1279096011kcz7.gif)
这个算法非常简单: 从当前像素周围采样一些邻近点, 用上面的公式统计出遮闭贡献度. 为了收集遮闭, 我使用45o和90o时旋转的4次采样 (<1,0>,<-1,0>,<0,1>,<0,-1>), 并且使用一张随机法线纹理做镜像. 
一些小技巧可以加速计算: 如使用一半大小的位置和法线缓存, 当然如果你愿意的话, 同时也可以对最后的SSAO缓存应用一个双向的模糊以减少采样产生的噪点. 注意这两个技巧是可以应用于任何SSAO算法的. 
下面是应用于屏幕矩形的HLSL pixel shader代码: 
sampler g_buffer_norm;
sampler g_buffer_pos;
sampler g_random;
float random_size;
float g_sample_rad;
float g_intensity;
float g_scale;
float g_bias;
struct PS_INPUT
{
 float2 uv : TEXCOORD0;
};
struct PS_OUTPUT
{
 float4 color : COLOR0;
};
float3 getPosition(in float2 uv)
{
 return tex2D(g_buffer_pos,uv).xyz;
}
float3 getNormal(in float2 uv)
{
 return normalize(tex2D(g_buffer_norm, uv).xyz * 2.0f - 1.0f);
}
float2 getRandom(in float2 uv)
{
 return normalize(tex2D(g_random, g_screen_size * uv / random_size).xy * 2.0f - 1.0f);
}
float doAmbientOcclusion(in float2 tcoord,in float2 uv, in float3 p, in float3 cnorm)
{
 float3 diff = getPosition(tcoord + uv) - p;
 const float3 v = normalize(diff);
 const float d = length(diff)*g_scale;
 return max(0.0,dot(cnorm,v)-g_bias)*(1.0/(1.0+d))*g_intensity;
}
PS_OUTPUT main(PS_INPUT i)
{
 PS_OUTPUT o = (PS_OUTPUT)0;
 
 o.color.rgb = 1.0f;
 const float2 vec[4] = {float2(1,0),float2(-1,0),
            float2(0,1),float2(0,-1)};
 float3 p = getPosition(i.uv);
 float3 n = getNormal(i.uv);
 float2 rand = getRandom(i.uv);
 float ao = 0.0f;
 float rad = g_sample_rad/p.z;
 //**SSAO Calculation**//
 int iterations = 4;
 for (int j = 0; j < iterations; ++j)
 {
  float2 coord1 = reflect(vec[j],rand)*rad;
  float2 coord2 = float2(coord1.x*0.707 - coord1.y*0.707, coord1.x*0.707 + coord1.y*0.707);
  
  ao += doAmbientOcclusion(i.uv,coord1*0.25, p, n);
  ao += doAmbientOcclusion(i.uv,coord2*0.5, p, n);
  ao += doAmbientOcclusion(i.uv,coord1*0.75, p, n);
  ao += doAmbientOcclusion(i.uv,coord2, p, n);
 } 
 ao/=(float)iterations*4.0;
 //**END**//
//Do stuff here with your occlusion value “ao”: modulate ambient lighting, write it to a buffer for later //use, etc.
 return o;
}
这个屏幕空间的方案与 “Hardware Accelerated Ambient Occlusion Techniques on GPUs” [1]十分相似, 主要是采样模式和AO函数的不同. 另外也可以理解成“Dynamic Ambient Occlusion and Indirect Lighting” [2]的图像空间版本. 
代码中有些值得提下的细节: 
- **半径除以p.z, 按到摄像机的距离进行了缩放. 如果你忽略这个除法, 所有的屏幕上的像素会使用同样的采样半径, 输出的结果就失去了透视感. **
- **在for循环中, coord1是位于90o的原始采样坐标, coord2是相同的坐标, 只不过旋转了45o. **
- 随机纹理包含了随机的法线向量, 所以这是你的平均法线贴图. 下面这张是我使用的随机法线纹理: ![](http://hi.csdn.net/attachment/201007/14/0_1279096019Ca3m.gif)
它被平铺到整个屏幕, 被每个像素使用下面的纹理坐标采样: 
**g_screen_size * uv / random_size**
“g_screen_size” 包含了屏幕的宽和高(像素单位), “random_size”是随机纹理的大小(我使用的是64x64). 采样出的法线用来镜像for循环中的采样向量, 以此获得每个屏幕像素各不相同的采样模式. (详见参考文献中的“interleaved sampling”) 
最后, shader减少到只需要遍历几个遮挡者, 为它们调用我们的AO函数, 累积出最后的结果. 其中共有4个artist变量: 
- g_scale: 缩放遮挡者和被遮挡者之间的距离. 
- g_bias: 控制被遮挡者所受的遮挡圆锥宽度. 
- g_sample_rad: 采样半径. 
- g_intensity: AO强度. 
当你调节它们同时观察效果的变化, 可以很直观地达到想要的效果. 
**结果**
![](http://hi.csdn.net/attachment/201007/14/0_12790960246Zxu.gif)
*a) 直接输出, 1个pass16次采样 b) 直接输出, 1个pass8次采样 c) 只有直接光照d) 直接光照– ao, 2个pass 每pass16次采样.*
如你所见, 代码既短小又简单, 结果也没有自遮闭, 只有很微弱的光环. 这两个现象也是使用深度缓冲作为输入的算法的主要问题, 可以从下面的图片中看出来:
![](http://hi.csdn.net/attachment/201007/14/0_1279096029k162.gif)![](http://hi.csdn.net/attachment/201007/14/0_12790960348qTk.gif)
自遮闭出现的原因是传统算法是在每个像素周围的球体上采样的, 所以没有被遮挡的平面上至少有一半的采样被标记成”被遮挡”. 这就导致了整体的遮闭效果是偏灰色的. 光环是物体周围出现的白色软边, 因为这些区域自遮闭是没有起作用的. 所认, 避免自遮闭的同时也能减弱光环问题. 
这个方法在你移动摄像机时会产生今人惊呀的效果. 如果你对效果的追求高于速度, 可以使用两个或更多不同半径的pass(复制代码中的for循环), 一个用于采集更多的全局AO, 其它的用于消除小裂缝. 在光照或纹理应用之后, 采样产生的瑕疵几乎看不出来, 也正是因为这个原因, 通常你不需要额外的模糊pass.
**进阶**
上面我已经叙述了一个简单实用的, 非常适合游戏使用的SSAO实现. 但是, 如果能把背离摄像机的面也考虑在内, 这样就可以获得更好的质量. 一般这需要三个缓冲: 两个位置/深度缓冲, 和一个法线缓冲. 
不过你也可以用两个缓冲来实现: 把正面和背面的深度分别保存在一个缓冲的红绿分量里, 然后再从每个分量中重建位置. 这样你就可以第一个缓冲用于保存”位置”, 第二个缓冲用于保存法线了. 
下面是每个位置缓冲采样16次的结果:
![](http://hi.csdn.net/attachment/201007/14/0_1279096042r89T.gif)
*左**: **正面遮蔽**, 右: 背面遮蔽*
实现它只需要在搜索遮挡者时, 在循环中调用“doAmbientOcclusion()”采样背面的位置缓冲. 显然, 背面的贡献度很小, 却使得采样的数目增加了一倍, 几乎把渲染时间变成了原来的两倍. 虽然你可以减小背面的采样, 但这仍然不太实用. 
这是需要增加的额外代码: 
在循环内部加入下面的调用:
ao += doAmbientOcclusionBack(i.uv,coord1*(0.25+0.125), p, n);
ao += doAmbientOcclusionBack(i.uv,coord2*(0.5+0.125), p, n);
ao += doAmbientOcclusionBack(i.uv,coord1*(0.75+0.125), p, n);
ao += doAmbientOcclusionBack(i.uv,coord2*1.125, p, n);
把这两个函数加入shader: 
float3 getPositionBack(in float2 uv)
{
 return tex2D(g_buffer_posb,uv).xyz;
}
float doAmbientOcclusionBack(in float2 tcoord,in float2 uv, in float3 p, in float3 cnorm)
{
 float3 diff = getPositionBack(tcoord + uv) - p;
 const float3 v = normalize(diff);
 const float d = length(diff)*g_scale;
 return max(0.0,dot(cnorm,v)-g_bias)*(1.0/(1.0+d));
}
增加一个保存了背面位置的sampler “g_buffer_posb”. (开启正面剔除绘制场景来生成它) 
另一个可以做的更改(这次我们改进的是速度而不是效果)是在我们的shader中增加一个简单的LOD (level of detail) 系统. 把固定次数的采样改成这样: 
**int iterations = lerp(6.0,2.0,p.z/g_far_clip);**
变量“g_far_clip” 是远裁剪面的距离, 必须做为参数传入shader. 现在每个像素应用的迭代次数取决于到摄像机的距离, 因此远处的像素只进行了粗糙的采样, 这就以不明显的质量下降换来了效率的提高. 不过, 在下面的性能衡量中我没有使用这个技巧.
**总结和性能衡量**
文章开头我提到过, 这个方法非常适用于延迟光照的游戏, 因为它需要的两个缓冲已经具备了. 它的实现很直接, 质量也不错, 又解决了自遮闭问题并减弱了光环现象. 不过除了这些, 它也跟其它SSAO技术一样有着共同的缺陷: 
缺点: 
- 没有把隐藏的几何体考虑在内(特别是视锥体外的). 
- 性能很大程度上决定于采样半径和到摄像机的距离, 因为近裁剪面附近的物体使用的半径从远处的大. 
- 输出有噪点. 
权衡一下速度, 可以对16次采样的实现做4x4的高斯模糊, 因为每次采样只采样了一个纹理, 并且AO函数十分简单, 不过实际应用的话还是有点慢. 这里有一个表格显示900x650的包含Hebe模型的场景, 没有模糊的情况下nVidia8800GT下的速度: 
|设定|FPS|SSAO时间(ms)|
|----|----|----|
|高 (32次正/反采样)|150|3.3|
|中 (16次正采样)|290|0.27|
|低 (8 次正采样)|310|0.08|
最后的这些截图你可以看到这个算法下不同模型的效果. 最高质量 (32 次正反采样, 较大的半径, 3x3 双向模糊):
![](http://hi.csdn.net/attachment/201007/14/0_1279096046wLr5.gif)
最低质量(8次正采样, 无模糊, 小半径):
![](http://hi.csdn.net/attachment/201007/14/0_1279096052131K.gif)
对比一下这项技术和光线追踪的AO也很有用. 比较的目的是看一下这项技术在有多少采样的情况下可以逼近实际的AO. 
![](http://hi.csdn.net/attachment/201007/14/0_1279096056466U.gif)
*左: SSAO**每像素**48**次采样**(32 正面 16 背面), 没有模糊. 右: Mental Ray**中的**光线追踪AO. 32 次采样, spread = 2.0, maxdistance = 1.0; falloff = 1.0.*
最后的一个建议: 不要想着把shader插入到你的管线中就能自动得到逼真的效果. 尽管这个实现有着很好的性能/质量比, 但SSAO是一项很耗费时间的效果, 你需要精心地调整它来达到尽可能高的性能. 如加减采样次数, 增加一个双向模糊, 改变强度, 等等. 另外, 你需要考虑SSAO是不是适合你. 除非你有很多动态物件在你的场景中, 要不然根本不需要SSAO; 可能light map对你来说已经足够了, 而且可以为静态场景提供更好的质量. 
希望你能够从这篇文章中获益. 所有包含在这篇文章中的代码都遵循[MIT license](http://www.opensource.org/licenses/mit-license.php)
**关于作者**
José María Méndez 是一个23岁的计算机工程学生. 他业余写游戏已经有6年, 现在在一家刚起步的Minimal Drama Game Studio公司任首席程序员. 
**参考资料**
[1] Hardware Accelerated Ambient Occlusion Techniques on GPUs
(Perumaal Shanmugam) 
[2] Dynamic Ambient Occlusion and Indirect Lighting 
(Michael Bunnell) 
[3] Image-Based Proxy Accumulation for Real-Time Soft Global Illumination 
(Peter-Pike Sloan, Naga K. Govindaraju, Derek Nowrouzezahrai, John Snyder) 
[4] Interleaved Sampling
(Alexander Keller, Wolfgang Heidrich) 
![](http://hi.csdn.net/attachment/201007/14/0_1279096062Spz7.gif)
*1024x768下渲染Crytek的 Sponza, 175 fps, **有一个方向光**.*
*![](http://hi.csdn.net/attachment/201007/14/0_1279096068d186.gif)*
1024x768同样的场景, 110 fps, 使用 SSAO中级设置: 16次采样, 正面(front faces), 没有模糊. 环境光已经乘了(1.0-AO). 
Sponza 可以从[Crytek网站](http://www.crytek.com/downloads/technology/)上下载.
