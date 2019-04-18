# AI 是否会代替插画师的工作？结合 AE 与 GAN 的一种新方法 - 知乎
# 



继续上次的话题，先看目标训练数据集，共有三万多张高质量 CG 图片，类型和内容主题非常丰富，代表着人类创造力的水准所在：
![](https://pic2.zhimg.com/v2-8368c7ad5c8e649582e5630d0ab2a961_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='729' height='602'></svg>)
有经验的同学会知道，这个图片数量，对于如此复杂的题材，远远不够电脑学习的需要；目前“效果惊人”的 GAN 都是在单一的题材，用大量的图片堆出来。

不过，下面我们就试试效果能到什么样。**我这里的方法，特别简单，速度很快，而且生成的图片的多样性很好。**




## **1. 效果**

使用1070显卡训练，训练1分钟后（速度基本上是每1分钟完成1个epoch），电脑首先学会的是冷暖二色：
![](https://pic3.zhimg.com/v2-12b071d65a6eca3f9aa4ff0da192399a_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
第3分钟，开始有一点梦幻色彩氛围：
![](https://pic2.zhimg.com/v2-9ab75445f1aecff51242499cdeee87f5_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
第6分钟，笔触有一点正常了，这个训练速度真是快：
![](https://pic2.zhimg.com/v2-2645225f0f818ecb130b4f3b82a364fd_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
第8分钟（这个训练过程很艺术，可以看到电脑在不断尝试各种笔触）：
![](https://pic1.zhimg.com/v2-527cbba8fe310016cde99e47b3a19f68_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
第10分钟（是不是感觉退步了？让我们继续看下去）：
![](https://pic3.zhimg.com/v2-413c559b665c0d43793c364c97137c0a_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
第15分钟，略好了一点，我认为电脑在试图描绘物体的边缘，所以看上去硬：
![](https://pic4.zhimg.com/v2-c860a1969f443d30597d7916bea2ff97_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
第20分钟，边缘化开了，有感觉了：
![](https://pic4.zhimg.com/v2-f0986ba36d380856317b9e0ec030ae83_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
第21分钟值得一看，由于电脑注意到许多图片里面有文字标题，于是随处乱画“文字标题”......就是那个黑白相间的结构（不过它很快就纠正了这个行为）：
![](https://pic3.zhimg.com/v2-198f9d5b37c623aabb93cf4f9db273fa_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
第25分钟，有一些人物形体：
![](https://pic4.zhimg.com/v2-89daef3288bb1f7743faff635c04c9bb_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
第30分钟，有一些隐隐约约的主题，色彩、构图、纹理都有可观之处：
![](https://pic4.zhimg.com/v2-fb3f4644515b165320d12936208d3c07_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
第34分钟， 真的画得挺有意思，每1分钟都令人想截图，这里的所有图片都没有经过任何筛选，全部都是由电脑随机生成：
![](https://pic2.zhimg.com/v2-319ed4975755f43240b9cb6b38cb9611_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
第35分钟：
![](https://pic4.zhimg.com/v2-33ee92d58bbd2d0b55b0b5747634f8bb_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
第36分钟，这色彩很大胆，有一些可以拿出来：
![](https://pic3.zhimg.com/v2-5071e4dcc23e7c231ece3e9f4bfaa106_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
第50分钟：
![](https://pic1.zhimg.com/v2-09893b829ad3d7caec958f08611e10f0_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
第70分钟，有的画得具有老练的感觉了。大家有没有注意到，电脑不但色彩好，构图其实也蛮好，其实大家如果眯着眼睛看，会感觉真的蛮像真的：
![](https://pic2.zhimg.com/v2-cf09137f756a3357cc706b24cfea7241_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
训练到后面，有时候还是会看到模式坍塌：
![](https://pic3.zhimg.com/v2-3deeee4843c49a1a7a96d55792717c66_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
这其实也与一些更复杂的因素有关，不过马上都会自行恢复：
![](https://pic1.zhimg.com/v2-fa24ffee7f73ea153cf186586942c56c_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
我这里的 E G D 网络都是最原始的 DCGAN 架构，相信有无数 tuning 的余地，但目前来说，确实效果已经相当不错。




## **2. 方法揭秘**

所有人都知道，AE 和 GAN 各有所长，但是实际并不容易把它们结合起来。比如说，VAE 和 GAN 的结合，很容易就把 GAN 给拖累了，也变成只有模模糊糊的图像。

**用什么办法，能在 AE 和 GAN 之间实现平滑过渡？**

我发现了一个非常简单的方法。首先，当然需要 E 编码器，于是有两个过程：
- AE ：X => E => z => G => X' 与 X 比较，将误差反向传递到 G 和 E。
- GAN ：z' => G => x => D => 0 和 X => D => 1 然后按照通常的方法传递误差。

比如这是第38分钟，G 生成的两种图片，首先是解码 E 的代码生成的图片，很模糊，就是 AE 的特点（AE 其实应该更清晰，不过我马上会说这里发生了什么）：
![](https://pic4.zhimg.com/v2-afabbfacfceb2c444f4404efb1f7b26b_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
这是解码随机代码生成的图片，就是普通 GAN 生成的图片，当然普通 GAN 做不到这里的多样性：
![](https://pic4.zhimg.com/v2-ed66220bd5b80630ad99003173ca30bf_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
这里只需加上一个 LOSS 项，就让网络实现了 AE 和 GAN 的结合：

**非常简单，就是让 AE 生成的编码 z 尽量靠近 0。就是把 E 生成的 z 计算出的 |z|^2 的梯度反向传回给 E。**

大家会说，这是什么方法？！ 

比如 VAE，难道不是更高明吗，它还会再考虑 z 的统计分布，是不是更先进？

错了。它在这里不适合。而且它就是造成 GAN 模糊的罪魁祸首。
- **实际上，AE 生成的东西，一定是模糊的，这来自于它的误差项（像素级别的误差项）**。
- 有很多人想了很多办法，比如比较中间层的输出，但是都太折腾了。我们可以放弃“让AE直接变清晰”的尝试。
- 因此，**E 生成的 z 的分布，可以和 GAN 输入的 z 的分布不同**。
- 如果强行要求 E 生成的 z 去覆盖 GAN 的输入（就是 VAE 的那一套），只会让 VAE 拖累了 GAN，让 GAN 也变得模糊。
- 那么，E 生成的 z，应该在什么位置？很简单，**它就应该在分布的最中心，靠近 0**。
- 这可以实现一个奇妙的结果：中心是模糊的AE结果，周围是清晰的GAN结果，这意味着什么？意味着我们得到了 AE 与 GAN 之间的插值方法，for free！
- 而且由于 E 生成的 z 在中心的那么小的区域里，已经可以解码出很丰富的图像（虽然不清晰），那么，GAN 采用的更丰富的 z，可以解码出的图像就更丰富了，这就解决了 GAN 的多样性不够的问题**（不过，如果 E 生成的 z 一直越来越小，会给 GAN 很大的压力（请思考为什么），所以其实还要加一点控制**）。
- 这样做之后，还要注意几点：
- 第一，**不要让 D 去试图识别 AE 的结果然后反向传回去**。因为 AE 生成的模糊图像是非常容易分辨的（分辨率一直100%），没意义，只会让 D 弱智化，造成一系列不良后果。
- 第二，**更加不要让 G 去让 AE 的结果骗过 D**。这是错上加错。AE 在此的真正作用，是做为“代码 z 的中心”，稳定 GAN，它生成的图像模糊，不是坏事，是好事。
- 因为我们都知道，z=0 对应的G的输出，应该是“所有样本的平均值”，这无疑就是最模糊的。
- 其实，我试过这个，效果是 G 学会把 AE 的结果加了一些恰到好处的灰尘，确实看上去更真实了！ 不过这只会影响 G 在正常 z 的效果。

## **这个方法确实是非常简单，大家有兴趣可以试试，有什么效果欢迎讨论。**

**如果觉得此文有帮助，请记得点个赞噢。**如需转载本文，请先与本人联系，谢谢。








