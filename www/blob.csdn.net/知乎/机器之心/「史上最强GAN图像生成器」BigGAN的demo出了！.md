# 「史上最强GAN图像生成器」BigGAN的demo出了！ - 知乎
# 



机器之心报道，参与：刘晓坤、路。

> BigGAN 一经提出即引起了大量关注，被称为「史上最强 GAN 图像生成器」。今日，DeepMind 放出了 BigGAN 的拿来即用 TF Hub demo，可以在 Colab 上运行图像生成和图像插值任务。
![](https://pic4.zhimg.com/v2-868d63620d291066294760091c8cf57f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='431'></svg>)- TF Hub demo 地址：[https://tfhub.dev/s?q=biggan](https://link.zhihu.com/?target=https%3A//tfhub.dev/s%3Fq%3Dbiggan)
- Colab 地址：[https://colab.research.google.com/github/tensorflow/hub/blob/master/examples/colab/biggan_generation_with_tf_hub.ipynb#scrollTo=Cd1dhL4Ykbm7](https://link.zhihu.com/?target=https%3A//colab.research.google.com/github/tensorflow/hub/blob/master/examples/colab/biggan_generation_with_tf_hub.ipynb%23scrollTo%3DCd1dhL4Ykbm7)

ICLR 2019 大会将在明年 5 月 6 日于美国举行，9 月 27 日论文提交截止后很多论文引起了大家的关注，其中就有一篇 GAN 生成图像的论文。该研究生成图像的目标和背景都高度逼真、边界自然，并且图像插值每一帧都相当真实，简直称得上「创造物种的 GAN」。该论文还引起了 Oriol Vinyals、Ian Goodfellow 的关注。上周，ICLR 2019 的论文评审结果出炉，评审们已经在论文的 openreview 页面公布了他们的评论和分数。这篇 BigGAN 论文获得了 8、7、10 的评分，三位评审人员对该论文给出了很高的评价，参见：[https://openreview.net/forum?id=B1xsqj09Fm。](https://link.zhihu.com/?target=https%3A//openreview.net/forum%3Fid%3DB1xsqj09Fm%25E3%2580%2582)

## **BigGAN 简介**

该论文出自 DeepMind，提出了一种新型 GAN 模型 BigGAN，该模型因其出色性能被称为「史上最强 GAN 图像生成器」。该研究的创新点是将正交正则化的思想引入 GAN，通过对输入先验分布 z 的适时截断大大提升了 GAN 的生成性能，在 ImageNet 数据集下 Inception Score 竟然比当前最好 GAN 模型 SAGAN 提高了 100 多分（接近 2 倍）！

该研究展示了 GAN 可以从训练规模中显著获益，并且能在参数数量很大和八倍批大小于之前最佳结果的条件下，仍然能以 2 倍到 4 倍的速度进行训练。作者引入了两种简单的生成架构变化，提高了可扩展性，并修改了正则化方案以提升条件化（conditioning），这可论证地提升了性能。作为修改方法的副作用（side effect），该模型变得服从「截断技巧」，这是一种简单的采样技术，允许对样本多样性和保真度进行精细控制。此外，该研究发现大规模 GAN 带来的不稳定性，并对其进行经验的描述。从这种分析中获得的洞察表明，将一种新型的和已有的技术结合可以减少这种不稳定性，但要实现完全的训练稳定性必须以显著降低性能为代价。
![](https://pic3.zhimg.com/v2-d5949f50ca3dec18b2ee5aaeca990bee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='277'></svg>)由 BigGAN 生成的类条件样本![](https://pic4.zhimg.com/v2-6629b6907ff6bbb8637eaea06d1d99cf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='249'></svg>)（a）增加截断的效应。从左到右，阈值=2, 1.5, 1, 0.5, 0.04。（b）应用截断和性能差的条件生成模型的饱和度伪影。![](https://pic2.zhimg.com/v2-1d60ddc405973b705c7bde1477da5dad_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='552'></svg>)由 BigGAN 在 512x512 分辨率下生成的其它样本![](https://pic4.zhimg.com/v2-08022c91942176bc27f605f81adfef6f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='797' height='493'></svg>)BigGAN 生成网络结构
现在 DeepMind 终于放出了 BigGAN 的 TF Hub demo，让我们来一探究竟。

## **教你怎么用 Colab Demo**

这个 Colab Demo 的使用非常简单，直接用 Chrome 浏览器打开 Colab 地址，登陆你的谷歌账号，就可以开始耍了。

刚打开 Colab 时，我们可以看到代码总体上分成四大块，分别是 BigGAN Demo、Setup、Explore BigGAN samples of a particular category、Interpolate between BigGAN samples。前面两块给出了大致的使用说明，以及具体实现（从 TF Hub 加载预训练模型、定义函数、创建会话和初始化变量等），之后两块才是重点，提供了特定类别的图像样本生成，以及指定两个类别的图像插值两个任务演示。只需要点击几个按钮，我们就能轻松启动运行和修改参数，完全不需要修改代码。
![](https://pic2.zhimg.com/v2-73ce054b45b1d42fd24ce85b92062ce1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='442'></svg>)
这个 Colab 的环境配置如下，打开「修改」-「笔记本设置」就可以看到。该 Colab 在 Python 2 环境和 GPU 上运行，这个不用修改，也不能修改，会报错。
![](https://pic4.zhimg.com/v2-aed2cb7b84fa2e81970e1861e450954f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='359' height='276'></svg>)
那么要怎么开始呢？很简单，直接打开「代码执行程序」-「全部运行」，就可以了。该 GPU 能提供 11G 左右的内存，运行过程中经常会出现内存不足的情况，如果没有报错，直接忽略就行。如果报错了，就打开「代码执行程序」-「重置所有代码执行程序」，重新开始吧。

在执行过程中，我们不一定需要同时执行生成和插值两个任务。为此，我们可以选中 Explore BigGAN samples of a particular category 这个单元格，然后打开「代码执行程序」-「运行当前单元格之前的所有单元格」，然后再分别执行之后的其中一个单元格，这样也能避免内存不足的问题。

在大部分情况下，内存不足并没有导致报错。在启动一个任务后，大概等待 10 秒左右的时间，我们就可以看到生成和插值的结果。
![](https://pic1.zhimg.com/v2-1008b86de52f67283f1a5dc2876f6ca4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='435'></svg>)
上图是样本生成单元格的控制界面，可以控制：生成样本数量、截断值、噪声种子、和类别的参数。我们可以看看下图的例子来说明各自的作用。
![](https://pic2.zhimg.com/v2-e647ae2ed5b35824c021931d5d9e0141_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='431'></svg>)
如上图所示，很容易看出，生成样本数量（10）和类别（芝士汉堡）就是字面的意思，那么截断值、噪声种子分别有什么作用呢？仅用一张图看不出来，我们修改一下参数看看区别就行了。

这时，我们只需要拖动滑块来修改参数，然后 Colab 会立刻执行新参数设置下的任务，但这也意味着我们一次只能修改一个参数。

经过多次实验，我们发现，截断值越大，生成样本的多样性越大；实际上，截断值控制的是隐变量分布（一般呈高斯型）的截断距离，也就是采样范围，因此不难理解其对多样性的作用。
![](https://pic4.zhimg.com/v2-9e8a14f34fecdcdbef4d4fd22dbb6abf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='431'></svg>)![](https://pic2.zhimg.com/v2-e647ae2ed5b35824c021931d5d9e0141_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='431'></svg>)![](https://pic3.zhimg.com/v2-9fbb1d51b6b7c43f5a730a1f24c9bd82_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='431'></svg>)![](https://pic4.zhimg.com/v2-868d63620d291066294760091c8cf57f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='431'></svg>)从上到下：截断值为 0.02、0.26、0.58、1.0 的芝士汉堡生成结果
而噪声种子的值对生成结果的影响主要是每次生成样本的初始条件，从而最终生成结果也会不同，可以用它来改善生成多样性。

最后是类别参数的控制，该 Colab 中提供了 1000 个类别选项，可玩性很足，但要找到自己想要的类别实在有点困难。可以这样操作，双击这个单元格，代码会显示在左侧，右侧会出现铅笔图标，点击该图标后就能查看完整列表，在这个列表下用 Ctrl+F 搜索类别的编号，再返回修改，这样就简单多了。
![](https://pic3.zhimg.com/v2-96b5023dd234ff3d7489cbf07a699c3a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='289'></svg>)![](https://pic2.zhimg.com/v2-c5a7cd1738f262fe43e8391fabe58ecd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1063' height='635'></svg>)
我们尝试生成了埃及猫（285）和大熊猫（388）的样本：
![](https://pic2.zhimg.com/v2-9380e694acbb2b52a09ec83ed625eb11_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='431'></svg>)![](https://pic1.zhimg.com/v2-0276457370bd1978868cf20c26bd3630_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='431'></svg>)
可以发现，埃及猫的眼神不太对，大熊猫有点营养不良。论文给出的结果很真实自然，但在这个 Colab 实验中我们也能发现存在很多「不自然」的生成样本。

接下来，我们来尝试图像插值任务。
![](https://pic2.zhimg.com/v2-fd36b3e9c3117af5c3cdff9f8d48170d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='628'></svg>)
如上图所示，这个任务的可控参数有：样本数量、插值数量、截断值、噪声种子、以及用于插值的图像类别 A 和 B。样本数量是指每次插值时类别 A 和类别 B 的的初始样本数量，如下图所示样本数量为 2。下图展示了一些示例，供大家参考。注意该图像插值是生成样本之间的插值，所以左右两侧的原图也会有些不自然。
![](https://pic4.zhimg.com/v2-6ee64963893b99f78b4ab0079505c127_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='431'></svg>)图像插值结果示例，其中样本数量为 2、插值数量为 5、截断值为 0.2、噪声种子为 0、类别 A 为金毛、类别 B 为鸡![](https://pic4.zhimg.com/v2-434b5d30d0baf2e3888f8452f6b87583_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='215'></svg>)其它参数不变，插值数量为 10 的插值结果示例![](https://pic3.zhimg.com/v2-2f5b4ebe8b1d8f8a5e481be7f11c7abe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='431'></svg>)金毛和埃及猫的图像插值结果
知道怎么用了吧？自己去耍吧～ 
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*



