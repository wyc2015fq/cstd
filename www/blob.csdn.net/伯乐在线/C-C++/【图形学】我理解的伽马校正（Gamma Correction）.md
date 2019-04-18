# 【图形学】我理解的伽马校正（Gamma Correction） - 文章 - 伯乐在线
原文出处： [妈妈说女孩子要自立自强](http://blog.csdn.net/candycat1992/article/details/46228771)
### 写在前面
我相信几乎所有做图像处理方面的人都听过伽马校正（Gamma Correction）这一个名词，但真正明白它是什么、为什么要有它、以及怎么用它的人其实不多。我也不例外。
最初我查过一些资料，但很多文章的说法都不一样，有些很晦涩难懂。直到我最近在看《Real Time Rendering，3rd Edition》这本书的时候，才开始慢慢对它有所理解。
本人才疏学浅，写的这篇文章很可能成为网上另一篇误导你的“伽马传说”，但我尽可能把目前了解的资料和可能存在的疏漏写在这里。如有错误，还望指出。
### 伽马的传说
关于这个方面，龚大写过[一篇文章](http://www.klayge.org/2011/02/26/gamma%E7%9A%84%E4%BC%A0%E8%AF%B4/)，但我认为其中的说法有不准确的地方。
从我找到的资料来看，人们使用伽马曲线来进行显示最开始是源于一个巧合：在早期，CRT几乎是唯一的显示设备。但CRR有个特性，它的输入电压和显示出来的亮度关系不是线性的，而是一个类似幂律（pow-law）曲线的关系，而这个关系又恰好跟人眼对光的敏感度是相反的。这个巧合意味着，虽然CRT显示关系是非线性的，但对人类来说感知上很可能是一致的。
我来详细地解释一下这个事件：在很久很久以前（其实没多久），全世界都在使用一种叫CRT的显示设备。这类设备的显示机制是，使用一个电压轰击它屏幕上的一种图层，这个图层就可以发亮，我们就可以看到图像了。但是，人们发现，咦，如果把电压调高两倍，屏幕亮度并没有提高两倍啊！典型的CRT显示器的伽马曲线大致是一个伽马值为2.5的幂律曲线。显示器的这类伽马也称为display gamma。由于这个问题的存在，那么图像捕捉设备就需要进行一个伽马校正，它们使用的伽马叫做encoding gamma。所以，一个完整的图像系统需要2个伽马值：
– encoding gamma：它描述了encoding transfer function，即图像设备捕捉到的场景亮度值（scene radiance values）和编码的像素值（encoded pixel values）之间的关系。
– display gamma：它描述了display transfer function，即编码的像素值和显示的亮度（displayed radiance）之间的关系。
如下图所示：
![这里写图片描述](http://jbcdn2.b0.upaiyun.com/2016/11/e2c6377d62da2c71885d17c9e89d2138.png)
而encoding gamma和display gamma的乘积就是真个图像系统的end-to-end gamma。如果这个乘积是1，那么显示出来的亮度就是和捕捉到的真实场景的亮度是成比例的。
上面的情景是对于捕捉的相片。那么对于我们渲染的图像来说，我们需要的是一个encoding gamma。如果我们没有用一个encoding gamma对shader的输出进行校正，而是直接显示在屏幕上，那么由于display gamma的存在就会使画面失真。
至此为止，就是龚大[所说的伽马传说](http://www.klayge.org/2011/02/26/gamma%E7%9A%84%E4%BC%A0%E8%AF%B4/)。由此，龚大认为全部的问题都出在CRT问题上，跟人眼没有任何关系。
但是，在《Real-time Rendering》一书中，指出了这种乘积为1的end-to-end gamma的问题。看起来，乘积为1的话，可以让显示器精确重现原始场景的视觉条件。但是，由于原始场景的观察条件和显示的版本之间存在两个差异：1）首先是，我们能够显示的亮度值其实和真实场景的亮度值差了好几个数量级，说通俗点，就是显示器的精度根本达不到真实场景的颜色精度（大自然的颜色种类几乎是无穷多的，而如果使用8-bit的编码，我们只能显示256^3种颜色）；2）这是一种称为surround effect的现象。在真实的场景中，原始的场景填充了填充了观察者的所有视野，而显示的亮度往往只局限在一个被周围环境包围的屏幕上。这两个差别使得感知对比度相较于原始场景明显下降了。也就是我们一开始说的，对光的灵敏度对不同亮度是不一样的。如下图所示（来源：[Youtube: Color is Broken](http://www.youtube.com/watch?v=LKnqECcg6Gw)）：
![这里写图片描述](http://jbcdn2.b0.upaiyun.com/2016/11/5d1179ea4505dd37e40705b88bdf4a8c.png)
为了中和这种现象，所以我们需要乘积不是1的end-to-end gamma，来保证显示的亮度结果在感知上和原始场景是一致的。根据《Real-time Rendering》一书中，推荐的值在电影院这种漆黑的环境中为1.5，在明亮的室内这个值为1.125。
个人电脑使用的一个标准叫sRGB，它使用的encoding gamma大约是0.45（也就是1/2.2）。这个值就是为了配合display gamma为2.5的设备工作的。这样，end-to-end gamma就是0.45 * 2.5 = 1.125了。
这意味着，虽然CRT的display gamma是2.5，但我们使用的encoding gamma应该是1.125/2.5 = 1/2.2，而不是1/2.5。这样才能保证end-to-end gamma为1.125，从而在视觉上进行了补偿。
虽然现在CRT设备很少见了，但为了保证这种感知一致性（这是它一直沿用至今的很重要的一点），同时也为了对已有图像的兼容性（之前很多图像使用了encoding gamma对图像进行了编码），所以仍在使用这种伽马编码。而且，现在的LCD虽然有不同的响应曲线（即display gamma不是2.5），但是在硬件上做了调整来提供兼容性。
**重要：**上面的说法主要来源于Real-time Rendering》一书。
### 来自其他领域的伽马传说
今天很幸运听了知乎上[韩世麟童鞋](http://www.zhihu.com/people/han-shi-lin)的讲解。在听了他的讲座后，我听到了另一个版本的伽马传说。和上面的讨论不同，他认为伽马的来源完全是由于人眼的特性造成的。对伽马的理解和职业很有关系，长期从事摄影、视觉领域相关的工作的人可能更有发言权。我觉得这个版本更加可信。感兴趣的同学可以直接去[知乎上](http://www.zhihu.com/question/27467127#answer-10413243)领略一下。
我在这里来大致讲一下他的理解。
事情的起因可以从在真实环境中拍摄一张图片说起。摄像机的原理可以简化为，把进入到镜头内的光线亮度编码成图像（例如一张JEPG）中的像素。这样很简单啦，如果采集到的亮度是0，像素就是0，亮度是1，像素就是1，亮度是0.5，像素就是0.5。这里，就是这里，出现了一点问题！如果我们假设只用8位空间来存储像素的话，以为着0-1可以表示256种颜色，没错吧？但是，人眼有的特性，就是对光的灵敏度在不同亮度是不一样的。还是这张图[Youtube: Color is Broken](http://www.youtube.com/watch?v=LKnqECcg6Gw)：
![这里写图片描述](http://jbcdn2.b0.upaiyun.com/2016/11/2adca8d684a5ab280174e2878fd28566.png)
这张图说明一件事情，即亮度上的线性变化在人眼看来是非均匀的，再通俗点，从0亮度变到0.01亮度，人眼是可以察觉到的，但从0.99变到1.0，人眼可能就根本差别不出来，觉得它们是一个颜色。也就是说，人眼对暗部的变化更加敏感，而对亮部变化其实不是很敏感。也就是说，人眼认为的中灰其实不在亮度为0.5的地方，而是在大约亮度为0.18的地方（18度灰）。强烈建议去看一下Youtube上的视频，[Color is Broken](http://www.youtube.com/watch?v=LKnqECcg6Gw)。
那么，这和拍照有什么关系呢？如果在8位图中，我们仍然用0.5亮度编码成0.5的像素，那么暗部和亮部区域我们都使用了128种颜色来表示，但实际上，亮部区域使用这么多种其实相对于暗部来说是种存储浪费。不浪费的做法是，我们应该把人眼认为的中灰亮度放在像素值为0.5的地方，也就是说，0.18亮度应该编码成0.5像素值。这样存储空间就可以充分利用起来了。所以，摄影设备如果使用了8位空间存储照片的话，会用大约为0.45的encoding gamma来对输入的亮度编码，得到一张图像。0.45这个值完全是由于人眼的特性测量得到的。
那么显示的时候到了。有了一张图片，显示的时候我们还是要把它还原成原来的亮度值进行显示。毕竟，0.454只是为了充分利用存储空间而已。我们假设一下，当年CRT设备的输入电压和产生亮度之间完全是线性关系，我们还是要进行伽马校正的。这是为了把用0.45伽马编码后的图像正确重现在屏幕上。巧合的是，当年人们发现CRT显示器竟然符合幂律曲线！人们想，“天哪，太棒了，我们不需要做任何调整就可以让拍摄的图像在电脑上看起来和原来的一样了”。这就是我们一直说的“那个巧合”。当年，CRT的display gamma是2.5，这样导致最后的end-to-end gamma大约是0.45 * 2.5 = 1.125，其实是非1的。
直到后来，微软联合爱普生、惠普提供了sRGB标准，推荐显示器中display gamma值为2.2。这样，配合0.45的encoding gamma就可以保证end-to-end gamma为1了。当然，上一节提到的两个观察差异，有些时候我们其实更希望end-to-end gamma非1的结果，例如，在电影院这种暗沉沉的环境中，end-to-end gamma为1.5我们人看起来更爽、更舒服，而在明亮的办公室这种环境中1.125的end-to-end gamma值更舒服、更漂亮。所以，我们可以根据环境的不同，去选择使用什么样的display gamma。
总之，**伽马校正一直沿用至今说到底是人眼特性决定的**。你会说，伽马这么麻烦，什么时候可以舍弃它呢？按[韩世麟童鞋](http://www.zhihu.com/people/han-shi-lin)的说法，如果有一天我们对图像的存储空间能够大大提升，通用的格式不再是8位的时候，例如是32位的时候，伽马就没有用了。因为，我们不需要为了提高精度而把18度灰编码成0.5像素，因为我们有足够多的颜色空间可以利用，不需要考虑人眼的特性。
好啦，上面就是来自摄影、建筑领域的看法和理解。希望这两种看法可以让大家更深地理解伽马校正的存在意义。
### 这和渲染有什么关系
其实，对伽马传说的理解就算有偏差，也不会影响我们对伽马校正的使用。我们只要知道，根据sRGB标准，大部分显示器使用了2.2的display gamma来显示图像。
前面提到了，和渲染相关的是encoding gamma。我们知道了，显示器在显示的时候，会用display gamma把显示的像素进行display transfer之后再转换成显示的亮度值。所以，我们要在这之前，像图像捕捉设备那样，对图像先进行一个encoding transfer，与此相关的就是encoding gamma了。
**而不幸的是，在游戏界长期以来都忽视了伽马校正的问题，也造成了为什么我们渲染出来的游戏总是暗沉沉的，总是和真实世界不像。**
回到渲染的时候。我们来看看没有正确进行伽马校正到底会有什么问题。
以下实验均在Unity中进行。
### 光照
我们来看一个最简单的场景：在场景中放置一个球，使用默认的Diffuse材质，打一个平行光：
![Gamma](http://jbcdn2.b0.upaiyun.com/2016/11/8f4eb0a1d6f0364a997c207e9211ae84.png)
看起来很对是吗？但实际上，这和我们在真实场景中看到的是不一样的。在真实的场景中，如果我们把一个球放在平行光下，它是长这个样子的：
![Linear](http://jbcdn2.b0.upaiyun.com/2016/11/ff6b11d17e7ff7b8f0febc37364ce624.png)
假设球上有一点B，它的法线和光线方向成60°，还有一点A，它的法线和光线方向成90°。那么，在shader中计算diffuse的时候，我们会得出B的输出是（0.5, 0.5, 0.5），A的输出的（1.0, 1.0, 1.0）。
在第一张图中，我们没有进行伽马校正。因此，在把像素值转换到屏幕亮度时并不是线性关系，也就是说B点的亮度其实并不是A亮度的一半，在Mac显示器上，这个亮度只有A亮度的1/1.8呗，约为四分之一。在第二章图中，我们进行了伽马校正，此时的亮度才是真正跟像素值成正比的。
### 混合
混合其实是非常容易受伽马的影响。我们还是在Unity里创建一个场景，使用下面的shader渲染三个Quad：


```
Shader "Custom/Gamma Correction For Quad" {
    Properties {
        _MainTex ("Base (RGB)", 2D) = "white" {}
        _Color ("Color", Color) = (1, 1, 1, 1)
    }
    SubShader {     
        Tags
        {
            "Queue" = "Transparent"
            "IgnoreProjector" = "True"
            "RenderType" = "Transparent"
        }
        Pass {
//          Blend One One
            Blend SrcAlpha OneMinusSrcAlpha
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #include "UnityCG.cginc"
            sampler2D _MainTex;
            float4 _Color;
            struct v2f {
                float4 pos : SV_POSITION;
                float4 uv : TEXCOORD0;
                float4 normal : TEXCOORD1;
            };
            v2f vert(appdata_base i) {
                v2f o;
                o.pos = mul(UNITY_MATRIX_MVP, i.vertex);
                o.uv = i.texcoord;
                return o;
            }
            float4 circle(float2 pos, float2 center, float radius, float3 color, float antialias) {
                float d = length(pos - center) - radius;
                float t = smoothstep(0, antialias, d);
                return float4(color, 1.0 - t);
            }
            float4 frag(v2f i) : SV_Target {
                float4 background = float4(0.0);
                float4 layer1 = circle(i.uv, float2(0.5, 0.5), 0.3, _Color.rgb, 0.2);
                float4 fragColor = float4(0.0);
                fragColor = lerp(fragColor, layer1, layer1.a);
//              fragColor = pow(fragColor, 1.0/1.8);
                return fragColor;
            }
            ENDCG
        }
    } 
    FallBack "Diffuse"
}
```
上面的shader其实很简单，就是在Quad上画了个边缘模糊的圆，然后使用了混合模式来会屏幕进行混合。我们在场景中画三个这样不同颜色的圆，三种颜色分别是(0.78, 0, 1)，(1, 0.78, 0)，(0, 1, 0.78)：
![这里写图片描述](http://jbcdn2.b0.upaiyun.com/2016/11/815c55346f308f769ca15735f13a3b21.png)
看出问题了吗？在不同颜色的交接处出现了不正常的渐变。例如，从绿色(0, 1, 0.78)到红色(0.78, 0, 1)的渐变中，竟然出现了蓝色。
正确的显示结果应该是：
![这里写图片描述](http://jbcdn2.b0.upaiyun.com/2016/11/f9f221648ca8920b8d624d29835c5acc.png)
第一张图的问题出在，在混合后进行输出时，显示器进行了display transfer，导致接缝处颜色变暗。
### 非线性输入
shader中非线性的输入最有可能的来源就是纹理了。
为了直接显示时可以正确显示，大多数图像文件都进行了提前的校正，即已经使用了一个encoding gamma对像素值编码。但这意味着它们是非线性的，如果在shader中直接使用会造成在非线性空间的计算，使得结果和真实世界的结果不一致。
### Mipmaps
在计算纹理的Mipmap时也需要注意。如果纹理存储在非线性空间中，那么在计算mipmap时就会在非线性空间里计算。由于mipmap的计算是种线性计算——即降采样的过程，需要对某个方形区域内的像素去平均值，这样就会得到错误的结果。正确的做法是，把非线性的纹理转换到线性空间后再计算Mipmap。
### 扩展
由于未进行伽马校正而造成的混合问题其实非常常见，不仅仅是在渲染中才遇到的。
Youtube上有[一个很有意思的视频](http://www.youtube.com/watch?v=LKnqECcg6Gw)，非常建议大家看一下。里面讲的就是，由于在混合前未对非线性纹理进行转换，造成了混合纯色时，在纯色边界处出现了黑边。用数学公式来阐述这一现象就是：
![%e5%85%ac%e5%bc%8f](http://jbcdn2.b0.upaiyun.com/2016/11/d08640737855c12b942f00d66231230a.png)
我们可以把和看成是两个非线性空间的纹理，如果直接对它们进行混合（如取平均值），得到的结果实际要暗于在线性空间下取平均值再伽马校正的结果。
所以，在处理非线性纹理时一定要格外小心。
### 进行伽马校正
我们的目标是：保证所有的输入都转换到线性空间，并在线性空间下做各种光照计算，最后的输出在通过一个encoding gamma进行伽马校正后进行显示。
在Unity中，有一个专门的设置是为伽马校正服务的，具体可以参见[官方文档（Linear Lighting）](http://docs.unity3d.com/Manual/LinearLighting.html)。
简单来说就是靠Edit -> Project Settings -> Player -> Other Settings中的设置：
![这里写图片描述](http://docs.unity3d.com/uploads/Main/GammaPlayerSetting.png)
它有两个选项：一个是Gamma Space，一个Linear Space。
– 当选择Gamma Space时，实际上就是“放任模式”，不会对shader的输入进行任何处理，即使输入可能是非线性的；也不会对输出像素进行任何处理，这意味着输出的像素会经过显示器的display gamma转换后得到非预期的亮度，通常表现为整个场景会比较昏暗。
- 当选择Linear Space时，Unity会背地里把输入纹理设置为sRGB模式，这种模式下硬件在对纹理进行采样时会自动将其转换到线性空间中；并且，也会设置一个sRGB格式的buffer，此时GPU会在shader写入color buffer前自动进行伽马校正。如果此时开启了混合（像我们之前的那样），在每次混合是，之前buffer中存储的颜色值会先重新转换回线性空间中，然后再进行混合，完成后再进行伽马校正，最后把校正后的混合结果写入color buffer中。这里需要注意，Alpha通道是不会参与伽马校正的。
sRGB模式是在近代的GPU上才有的东西。如果不支持sRGB，我们就需要自己在shader中进行伽马校正。对非线性输入纹理的校正通常代码如下：


```
float3 diffuseCol = pow(tex2D( diffTex, texCoord ), 2.2 );
```
在最后输出前，对输出像素值的校正代码通常长下面这样：


```
fragColor.rgb = pow(fragColor.rgb, 1.0/2.2);
return fragColor;
```
但是，手工对输出像素进行伽马校正在使用混合的时候会出现问题。这是因为，校正后导致写入color buffer的颜色是非线性的，这样混合就发生在非线性空间中。一种解决方法时，在中间计算时不要对输出进行伽马校正，在最后进行一个屏幕后处理操作对最后的输出进行伽马校正，但很显然这会造成性能问题。
还有一些细节问题，例如在进行屏幕后处理的时候，要小心我们目前正在处理的图像到底是不是已经伽马校正后的。
总之，一切工作都是为了“保证所有的输入都转换到线性空间，并在线性空间下做各种光照计算，最后的输出（最最最最后的输出）进行伽马校正后再显示”。
虽然Unity的这个设置非常方便，但是其支持的平台有限，目前还不支持移动平台。也就是说，在安卓、iOS上我们无法使用这个设置。因此，对于移动平台，我们需要像上面给的代码那样，手动对非线性纹理进行转换，并在最后输出时再进行一次转换。但这又会导致混合错误的问题。
### 在  Unity 中使用 Linear Space
如果我们在Edit -> Project Settings -> Player -> Other Settings中使用了Linear Space，那么之前的光照、混合问题都可以解决（这里的解决是说和真实场景更接近）。但在处理纹理时需要注意，所有Unity会把所有输入纹理都设置成sRGB格式，也就说，所有纹理都会被硬件当成一个非线性纹理，使用一个display gamma（通常是2.2）进行处理后，再传递给shader。但有时，输入纹理并不是非线性纹理就会发生问题。
例如，我们绘制一个亮度为127/255的纹理，传给shader后乘以2后进行显示：
![这里写图片描述](http://jbcdn2.b0.upaiyun.com/2016/11/abad4979616dfd92d96996ac2cd3c268.png)![这里写图片描述](http://jbcdn2.b0.upaiyun.com/2016/11/df8d66f8fb9aabd989e07d5c6a1e986f.png)
可以看出，Gamma Space的反而更加正确。这是因为，我们的输入纹理已经是线性了，而Unity错误地又进行了sRGB的转换处理。这样一来，右边显示的亮度实际是，(pow(0.5, 2.2) * 2, 1/2.2)。
为了告诉Unity，“嘿，这张纹理就是线性的，不用你再处理啦”，可以在Texture的面板中设置：
![这里写图片描述](http://jbcdn2.b0.upaiyun.com/2016/11/387889d339e801909cfe0a9e2852ac80.png)
上面的“Bypass sRGB Sample”就是告诉Untiy要绕过sRGB处理，“它是啥就是啥！”。
这样设置后，就可以得到正确采样结果了。
### 写在最后
伽马校正一直是个众说纷纭的故事，当然我写的这篇也很可能会有一些错误，如果您能指出不胜感激。
即便关于一些细节问题说法很多，但本质是不变的。GPU Gems上的一段话可以说明伽马校正的重要性：
> 
This is one reason why most (but not all) CG for film looks much better than games—a reason that has nothing to do with the polygon counts, shading, or artistic skills of game creators. (It’s also sometimes a reason why otherwise well-made film CG looks poor—because the color palettes and gammas have been mismatched by a careless compositor.)
最后，给出GPU Gems中的一段总结，以下步骤应该在游戏开发中应用：
1. 假设大部分游戏使用没有校正过的显示器，这些显示器的display gamma可以粗略地认为是2.2。（对于更高质量要求的游戏，可以让你的游戏提供一个伽马校正表格，来让用户选择合适的伽马值。）
2. 在对非线性纹理（也就是那些在没有校正的显示器上看起来是正确的纹理）进行采样时，而这些纹理又提供了光照或者颜色信息，我们需要把采样结果使用一个伽马值转换到线性空间中。不要对已经在线性颜色空间中的纹理，例如一些HDR光照纹理、法线纹理、凹凸纹理（bump heights）、或者其他包含非颜色信息的纹理，进行这样的处理。对于非线性纹理，尽量使用sRGB纹理格式。
3. 在显示前，对最后的像素值应用一个伽马校正（即使用1/gamma对其进行处理）。尽量使用sRGB frame-buffer extensions来进行有效自动的伽马校正，这样可以保证正确的混合。
所幸的是，在Unity中，上面的过程可以通过设置Edit -> Project Settings -> Player -> Other Settings->Color Space轻松地完成，需要注意的是对纹理的处理。但不幸的是，不支持移动平台。
最后，一句忠告，**在游戏渲染的时候一定要考虑伽马校正的问题，否则就很难得到非常真实的效果**。
