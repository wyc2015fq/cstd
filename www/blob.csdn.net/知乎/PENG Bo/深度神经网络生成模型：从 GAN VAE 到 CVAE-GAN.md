# 深度神经网络生成模型：从 GAN VAE 到 CVAE-GAN - 知乎
# 



在几年前，深度神经网络的热门话题是分类问题：

> ***给定一张图 x，神经网络可以告诉你其中是什么内容，或者说属于什么类别 c。 ***

而近年来，深度神经网络的热门话题是生成模型，它想做的事情相反：

> ***给定一个类别 c，神经网络就可以无穷无尽地自动生成真实而多变的此类别的图像 x。***

这无疑听上去更神奇，也更困难。这里介绍的 CVAE-GAN 方法，其论文是 [Fine-Grained Image Generation through Asymmetric Training](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1703.10155) ，它可以在各个数据集上生成目前综合效果最好的结果：
![](https://pic3.zhimg.com/v2-65abac1e0671814b47112f811f62b50a_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1052' height='582'></svg>)
**请注意，以上的图像都是神经网络根据少量参考图，凭空想象出来的全新图片。而且它可以无穷无尽地自动生成下去，例如包括各种角度，表情，而且会在此过程中不断进步（简直正如 AlphaGo 可以无穷无尽地左右互搏）。**

在人脸数据集上，效果已经好得惊人，栩栩如生。而在其它数据集上，虽然看似还有瑕疵，但这可能已经只是因为参考图的数量不够多（正如最近的 [[1707.02968] Revisiting Unreasonable Effectiveness of Data in Deep Learning Era](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1707.02968) 所述，目前的瓶颈可能已经在于数据，而非模型。文中发现，采用3亿张训练图片作预训练后，所有模型的效果都可以得到明显提升。而且看上去3亿张训练图片也没有“喂饱”神经网络，依旧会发现训练图片越多效果越好）。

我们将会看到，CVAE-GAN 的思想实际简单而直接。**它有四大组件，对应到四个神经网络，互为补充，互相促进**（这是否又让我们想起 AlphaGo？是的，多个神经网络协同组合，确实会有惊人的威力。而且其实还有更多的相似之处，因为它们最终都属于博弈问题）。

**如需转载本文，请与本人联系，谢谢。**

## **1. 生成模型的派别：VAE 和 GAN**

在最近几年，生成模型分为 VAE 和 GAN 两条路（其实还有一条值得继续的路是 PixelCNN/RNN，另外我可能还有个不一样的路子...）。十分有趣的是，它们拥有互补的长处和短处：
![](https://pic3.zhimg.com/v2-71d68cfabc6c7b7155e34f0b30d97fa6_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1042' height='378'></svg>)
以上都加上了类别作为输入，所以称之为 CVAE 和 CGAN。直观地看：
- CVAE 生成的图像很中规中矩，但是模糊。
- CGAN 生成的图像清晰，但是喜欢乱来。

所以刚好是互补。这其实来自于它们各自的构造。首先，在构造生成模型时，人们渐渐发现，最常用的是下面四个网络：
- **E：编码器 Encoder。**给定一张图 x，可以将其编码为隐变量 z（且希望满足高斯分布）。如果还给定了类别 c，那么生成的隐变量就会质量更高（更随机）。
- **G：生成器 Generator。**给定隐变量 z（如随机噪声），就可以生成像模像样的图像。如果还给定了类别 c，那么就会生成像模像样的属于类别 c 的图像。
- **C：分类器 Classifier。**给定一张图 x，输出所属类别 c。这是大家的老朋友。
- **D：辨别器 Discriminator。**给定一张图 x，判断它是真实的图片，还是“电脑乱想出来的”。这是 GAN 首先引入的网络，它可以和 G 左右互搏，互相进步。

那么，CVAE 是这样的：
![](https://pic1.zhimg.com/v2-9e7f57cdf2bcb49a6780913887005034_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='270' height='79'></svg>)
它来自于经典的自编码器 AE，其实 E 有点类似于人脑把图片变成文字描述的过程，z 就类似于文字描述，然后 G 试图按 z 的描述还原图片。VAE 的目标是， x' 接近 x，同时 z 接近高斯分布（这样我们可以很容易地构造大量 z，从而生成大量 x'）。

为什么 VAE 生成的图像会模糊？如图，可以真的很模糊：
![](https://pic3.zhimg.com/v2-4bcca28d1b00910d335056b6d50205ce_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='189' height='191'></svg>)
**最重要的原因是，它不容易找到合理的判断 “x' 接近 x” 的标准，只能用 MSE 之类的粗略误差。**

而 CGAN 是这样的：
![](https://pic2.zhimg.com/v2-9cd690e38fd1903fbecb0a727ea01701_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='259' height='79'></svg>)
也是有个 G（其实和之前的 VAE 的 G 类似，只是由于优化的目标不同，所以生成的图像的特点不同），目标是从 z 生成尽量真实的 x'，而且我们还再做了一个网络 D，用于判断“真实度”。

D 的做法很暴力，就是一直认为 x 是真的，x' 是假的，并以此训练自己。而 G 的目标当然就是尽量骗过 D，做法也很暴力，直接把 D 的输入梯度信息拿出来用于训练自己，尽量生成“最能骗过 D 的图像”。其实就像之前大家构造能骗过神经网络 C 的图片一样，直接用输入的梯度。

随着 G 和 D 的互相斗争，最终大家达成 Nash 均衡，生成的 x' 也就比较真实了。

为什么 GAN 生成的图像喜欢乱来？我认为，**原因之一是 D 的能力不够，**是因为 D 看过的样本太单调（要么是固定的原始样本，要么都是 G 生成的），所以辨别能力容易存在盲区，而 G 是发现空子就去钻，所以就容易乱来了。

**原因之二是 G 的钻空子太暴力。**比如说，如果 G 学会了每次都生成一模一样的和训练集中某个图像一模一样的图？或者一个固定的特别容易骗过 D 的图？那 D 就分辨不出来了，但生成这样的图片当然也没有意义了。这其实就是个常见的问题，叫做模式的坍塌，就像下图：
![](https://pic1.zhimg.com/v2-d771e6a2b61e6ec10efed9e6dee3d3e4_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='235' height='234'></svg>)
而之前 VAE 的 G 是不会犯这种错误，因为它必须有足够的生成能力，对于训练集中每个图片，它都需要有能力生成相似的图片。

其实，某种意义上，AlphaGo 的策略网络类似于 G（试图一步步生成“完美的神之棋局”），AlphaGo 的价值网络类似于 D（如果胜率在走了某步之后倒向一方，就说明棋局不够完美）。




## **2. 取长补短，E+G+C+D = CVAE-GAN**

看 CVAE-GAN 的结构图：
![](https://pic2.zhimg.com/v2-454d25bf1dd43e0ca479ada2001a01b1_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='399' height='96'></svg>)
首先，之前说过了，VAE 的 G 比 GAN 的 G 更有前途，所以前面是 VAE 的结构。

然后，由于 VAE 的判断 x' 和 x 相似的标准不够好，所以要再加入 D 来判断。

最后，我们还得真的保证生成的图像是属于 c 类别，所以还要加上 C。

所以 G 的 Loss 其实有三大部分：
- 对于从 x 生成的 z，G 应该更还原出接近 x 的 x' （像素上的接近）。称为 L_G(img)。
- G 生成的图像应该可以由 C 鉴别为属于 c。称为 L_G(C)。
- G 生成的图像应该可以由 D 鉴别为属于真实图像。称为 L_G(D)。

这三者都很重要。举例，原始图像 x：
![](https://pic1.zhimg.com/v2-7ac32f29c4a7bb92ca8129ba68295a5c_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1185' height='216'></svg>)
CVAE-GAN 的复原 x'，与 x 接近，只是少了皱纹之类的噪音，这也说明 D 对于皱纹是不在意的，它认为皱纹不是判断“真实性”的标准，也许没皱纹还会更“真实”：
![](https://pic1.zhimg.com/v2-e761697d0b328d8ede7e6e312e6ea764_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1469' height='262'></svg>)
如果我们去掉 D，马上就模糊了：
![](https://pic4.zhimg.com/v2-05c75c4d83687690ffba742eb836e3cb_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1467' height='256'></svg>)
如果我们去掉 C，那么 artifact 就多了：
![](https://pic2.zhimg.com/v2-51406a39f5a65cfa685bbe1a2168be95_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1468' height='264'></svg>)
如果我们去掉 img，那么就容易乱来了：
![](https://pic4.zhimg.com/v2-3c335b65737982010862d3c50560d153_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1473' height='268'></svg>)
最终的隐变量 z 可以相当好地自动刻画图片：
![](https://pic1.zhimg.com/v2-2114c811a640f882923d23a05efd78b4_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='757' height='483'></svg>)
编码转移，给定左边的照片，要求网络更换其中的明星，生成的是右边的图：
![](https://pic1.zhimg.com/v2-7b9bad5aa90877020f2e293be86301a0_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='998' height='661'></svg>)
插值效果，给定左右两张图片，可以生成“中间的渐变过程”：
![](https://pic1.zhimg.com/v2-44a9440557b1f84972bc969940ddbbf8_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='857' height='376'></svg>)
自动填充缺失区域的效果（左边的原始图片都是网络从来没有见过的全新图片）：
![](https://pic3.zhimg.com/v2-82d1b722a90a403199e16ff668f5defa_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1077' height='793'></svg>)
由于网络生成的样本质量很高，还可以用于增强训练样本集，使其它模型得到更好的效果：
![](https://pic3.zhimg.com/v2-9912ed6234c2c48c0bd0200ad60022f2_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='710' height='177'></svg>)
最后，CVAE-GAN 的详细架构如图：
![](https://pic3.zhimg.com/v2-a07b6a20ff4de63aa937f269bcec92ea_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='750' height='526'></svg>)
CVAE-GAN 的训练算法如图，其中的每一项都是很直观的。注意里面还用了一个重要 trick，就是希望 x' 和 x 在 D 和 C 的网络的中间层的特征也相似。这有助于稳定网络：
![](https://pic3.zhimg.com/v2-8127b9b611186785a9d4ca3046752cee_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='622' height='954'></svg>)



## **3. 总结**

通过使用 E+G+C+D 四大网络， CVAE-GAN 实现了相当令人满意的生成模型。目前的 GAN 变种非常多，大家都是把 E G C D 到处串起来，不过一般只用到三个，很少有用到四个的。本来我想试把四个都串起来，然后发现 CVAE-GAN 已经做了，效果也确实好。

最近还有一篇 Triple GAN [https://arxiv.org/pdf/1703.02291.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1703.02291.pdf) 类似，但它没有用 VAE 作为前端，少了 E 网络，所以效果会不如。而 CycleGAN DualGAN DiscoGAN 等等把网络串起来的方式，无疑可以与 CVAE-GAN 结合，进一步提升效果。相信 U-NET PatchGAN 等等 trick 也可以进一步提升效果。

最后图片展示，看看目前的生成模型的发展，尤其是风格转移：
![](https://pic4.zhimg.com/v2-9c85e98338c5e879328cd78e0925d757_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='999' height='471'></svg>)
着重看一下画，右边的画都是根据左边的照片完全自动生成的，效果确实相当专业了：
![](https://pic4.zhimg.com/v2-853bd0874019e27caff41b1784519c63_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1237' height='869'></svg>)
然后是编码改变：
![](https://pic2.zhimg.com/v2-f7a0df3a8dc05e0a788a8b636d657401_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='450' height='552'></svg>)
同样是风格转移：
![](https://pic2.zhimg.com/v2-6985e8152b820f4409b088f7c716517d_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='670' height='493'></svg>)
风格转移，从手袋生成匹配的鞋设计，从鞋设计生成匹配的手袋设计：
![](https://pic1.zhimg.com/v2-2e14e92dd98d48207d164070ae590a54_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='598' height='495'></svg>)
**如需转载本文，请与本人联系，谢谢。**

**最后，欢迎关注我的专栏： [技术备忘录 - 知乎专栏](https://zhuanlan.zhihu.com/mathNote)，其中有更多文章。**

**欢迎关注本司产品 [Blink Sunshine护眼无频闪97显色指数灯泡系列](https://link.zhihu.com/?target=https%3A//item.taobao.com/item.htm%3Fid%3D40134613056)，提升生活品质~**


