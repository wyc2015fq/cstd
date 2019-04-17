# Bye Bye TPU，4个GPU就能训练“史上最强”BigGAN！作者开源完整PyTorch模型 - 知乎
# 



> 安妮 夏乙 假装发自 凹非寺
量子位 出品 | 公众号 QbitAI




“不，你不想。”

每次被BigGAN史上最强”的效果吸引，想要用其他数据集训练一番，脑海深处都会响起这样一个声音。

就仿佛DeepMind团队训练BigGAN用的512个TPU，齐刷刷发出不怀好意的嘲笑。



![](https://pic1.zhimg.com/v2-ddc305542c8b75434758ab85f6cc5ce0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='580' height='580'></svg>)



现在，

好消息来了！

BigGAN一作Andrew Brock新鲜放出了只需**4-8个GPU即可训练**的新版模型，这一次，他用的是**PyTorch**。



![](https://pic1.zhimg.com/v2-4a1633d958b793677a6d0122965dac1c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='968'></svg>)



虽然“**只需**”令人嫉妒，但比起原来动辄128个、512个TPU的硬件需求，简直就扶贫济困，平民之光。

网友们一夜之间把Brock这条Twitter转了100多次，在Reddit论坛上也夸这个新实现”fantastic”、”awesome”、”cool”，表示一定要玩。

更让人期待的是，打破了128个TPU的门槛之后，用其他数据集训练的BigGAN一定会层出不穷啊！风景、waifu指日可待。

另外，吸引网友们的不仅仅是算力需求的降低，还有：

**等你的PyTorch版实现等了好久了！**

## **摆脱算力束缚**

Brock开源的低成本版PyTorch实现，内容包括训练、测试、采样所需的脚本、把THHub原版BigGAN生成器权重迁移到PyTorch的脚本，还有完整的预训练检查点。

除了现有的ImageNet预训练模型之外，他还打算放出Places-365数据集上预训练的模型。



![](https://pic2.zhimg.com/v2-202fb61ff0596ed1b20bb0cca8afc959_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1047' height='605'></svg>)



如果你想用自己的数据来训练BigGAN，可以选择从头开始训练，也可以在ImageNet预训练模型的基础上微调。无论如何，只需4-8个GPU。

Brock还在Reddit论坛上说，其实一个GPU也行，但就是要花的时间太长了。

原版是128-512个TPU，新版是4-8个GPU，这之间有巨大的算力差距，Brock到底是怎样用Pytorch完整复现BigGAN的？

大思路就是：**靠梯度累加**，在**小硬件上使用大批量**（batch）。Brock已经将整个复现过程详细记录了下来。

我们先来看看这个低成本版BigGAN**怎么用**。

开始之前，需要准备三样训练工具：
- PyTorch（版本1.0.1）
- tqdm，numpy，scipy和h5py
- ImageNet训练集

首先，你可以选择为你的目标数据集准备一个预处理HDF5版本，以实现更快的I/O，这需要通过修改和运行计算FID需要的Inception moment。运行这行代码就可以：


```
sh scripts/utils/prepare_data.sh
```


默认情况下，它会假设你的ImageNet训练集下载到根目录的data位置下，并将以128×128像素的分辨率准备缓存的HDF5。

在脚本文件夹中，有多个bash脚本可以用来训练不同批大小的BigGAN。这些代码默认你无法访问完整的TPU pod，因此，会运用梯度累加技术来假冒大批量。

具体来说，先在多个小批量上将梯度平均，然后在N次累计后，再执行优化程序步骤。

在默认情况下，launch_BigGAN_bs256x8.sh脚本训练一个全尺寸96通道的BigGAN模型，用批大小为256的8个梯度累加，得到总体大小为2048的批量。

用的硬件少了，难免要多花点时间。按上面的默认情况，在128×128像素的ImageNet上训练BigGAN，用8个Tesla V100进行全精度的无张量训练，大约需要15天完成150000次迭代。

正确的流程是，你首先需要做的就是设置可以支持的最大的批大小，确定后再修改脚本，使批大小×梯度累加的数量=所需的批大小总量（BigGAN默认为2048）。

需要注意，梯度累加意味着需要将预估的奇异值（singular value，**SV**）和批归一化（Batch Normalization，**BN**）统计量提高8倍。

这意味着，BN的统计值更接近于静止状态，相较之下奇异值的估计值往往更加准确。

因此，Brock在默认情况下用测试模式下的G来衡量标准，即用BN的运行状态预估值来替代去计算静态统计值。不过，最终repo也支持静态统计，虽然可能会导致前期的梯度累加失效，但问题也不大。



![](https://pic3.zhimg.com/v2-a9e75b4e0079e20495287ffc344e9482_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='543'></svg>)



**标准和示例**

在训练期间，这个脚本会输出具有训练标准和测试标准的日志，并且将保存模型权重/优化的程序参数的多个副本，并且每次保存权重时，都将生成示例和插值。

日志文件夹中存储了处理这些日志的脚本，并将结果用Matlab绘制出来。

训练过程完成后，你可以用sample.py代码块生成其他示例和插值，还能改变截断值、批大小、静态统计累计值等进行测试。

默认情况下，所有内容都保存到权重/示例/日志/数据文件夹中，repo被假定到与它们在同一个文件夹里了。你可以用—base_root代码块将这些参数指向不同的基础文件夹，或者用各自的参数选择每个基础文件夹的特定位置。

在Brock小哥给出的实现中，包含了BigGAN-deep的运行脚本，但没有用它完全训练成一个模型，是未经测试的一个脚本。

此外，Brock还给出了包括在CIFAR上运行的脚本，以及在ImageNet上运行SA-GAN和SN-GAN的脚本。SA-GAN是假设用4个TitanX训练时的脚本，是在批大小为128时加2个梯度累加的情况下进行的。

**用自己的数据微调预训练模型**



![](https://pic3.zhimg.com/v2-b9329b668732ba093c2df0a3b2def31a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='774' height='258'></svg>)



我们前边也提到过，除了从头开始训练，还可以拿Brock预训练的模型来微调。

要运行的脚本和从头训练一样，但是要加上—resume。

继续进行中断了的训练也是用这个脚本。

要准备微调用的数据集，需要添加到datasets.py中，并在utils.py里调整convenience dicts，例如dset_dict、imsize_dict、root_dict、nclass_dict、classes_per_sheet_dict等等设置，这样才能为数据集创建合适的元数据。

然后，重复prepar_data.sh的流程。

## **我有一块1080Ti，行吗？**

低成本版一出，引来不少网友欢呼，很多一看就来自学神们。

他们很是关注Brock所用的方法，比如同步的批归一化(synchronized batchnorm)、梯度累加(gradient accumulation)技术等等，还打算用到自己的研究里。

当然，更多人关心的是：

用这个“官方非官方”PyTorch版实现训练，成本究竟怎么样？

按照Brock举的例子计算，8块Tesla V100训练15天，在亚马逊云服务AWS上的价格是8800多美元。

也就是说，如果你自己本地没有那么多GPU，还是挺贵……



![](https://pic4.zhimg.com/v2-5730c94f9e39effa6dd4a95293b7e647_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='175' height='218'></svg>)



于是，评论区除了欢呼的大神们之外，也冒出了很多贫穷的声音：

> 哇哦好赞啊！但是你说得好像4-8块GPU多便宜似的哈哈哈哈。
然而我只有一块1080ti……
在Colab里哭。（量子位注：就是蹭免费GPU的Colab啦）

也有人真诚提问：

> 我有一块2080ti，你估计半个星期能训练到收敛吗？还是需要几周？

Brock给出的答案不容乐观：一块2080ti啊，那得好几个月了。



![](https://pic3.zhimg.com/v2-a9e7d826f520559958570ca801e6c57a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='255'></svg>)



也就是说，如果预算不充沛，还是拿预训练模型微调的好。

不过，评论区也有高人，为新实现找了个绝佳的使用场景：

> 那些之前买显卡挖矿的人，终于有比较酷的事情可干了。

这一提议顿时有人响应：

> 哈……我有12块英伟达1080，就是因为这个。

如果你也有挖矿遗留下来的GPU，那就一切好办了。

## **转投PyTorch体验怎么样？**

除了省算力，“终于有了完整PyTorch版实现”也是网友们非常关心的。

毕竟，Brock作为一名（曾经的）DeepMind实习生，改用PyTorch实属难得。这一版本“官方非官方”实现是他和Alex Andonian一起完成的，而并没有提到BigGAN另外两位作者。



![](https://pic1.zhimg.com/v2-9fa7557059565f872e454cd0d0c80154_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='171' height='220'></svg>)Andrew Brock
他使用PyTorch体验怎样？如何看待在PyTorch和TensorFlow中实现BigGAN的差异？

在Reddit上，也有网友向Brock好奇地提出了这个问题。



![](https://pic1.zhimg.com/v2-2f70caeb15e060b95d942517b928e7a4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='231'></svg>)



Brock坦言，用PyTorch训练BigGAN，是种非常**可爱的体验**。

他认为PyTorch让很多事情都变得容易多了，比如组成层、测试、调试、实现模型并行性等操作，一切都可以信手拈来。

整个实现的过程中，当然也遇到了一些没想到的困难，不过Brock说都是他自己的错。比如直到开始检查梯度时，才发现错位的ReLU已经造成了大面积破坏。

此外，他遇到的最大挑战其实是算力不足：因为Brock只有多所大学的共享服务器可用，因此调试/测试周期也拉长了不少。每次训练只能运行24小时，等待下一次开始的时间，则不止24小时了。

## **传送门**
![](https://pic3.zhimg.com/v2-ac445a9e779c037d52dd4c6348ffac06_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='587'></svg>)
最后，附上GitHub地址:
[ajbrock/BigGAN-PyTorch​github.com![图标](https://pic1.zhimg.com/v2-e71a451b276a881085c59ea980a740f0_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/ajbrock/BigGAN-PyTorch)
还有论文：
[Large Scale GAN Training for High Fidelity Natural Image Synthesis​arxiv.org](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1809.11096)



— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤




